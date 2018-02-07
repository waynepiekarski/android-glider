// ---------------------------------------------------------------------
//
// Glider
//
// Copyright (C) 1996-2018 Wayne Piekarski
// wayne@tinmith.net http://tinmith.net/wayne
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// ---------------------------------------------------------------------

package net.waynepiekarski.glider

import android.app.UiModeManager
import android.content.Context
import android.content.res.Configuration
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.graphics.Rect
import android.graphics.Typeface
import android.util.AttributeSet
import android.view.GestureDetector
import android.view.KeyEvent
import android.view.MotionEvent
import android.view.SurfaceHolder
import android.view.SurfaceView

import java.io.UnsupportedEncodingException
import java.util.ArrayList
import java.util.Arrays
import java.util.concurrent.LinkedBlockingQueue
import java.util.concurrent.TimeUnit

class AnsiTerminalView(context: Context, attrs: AttributeSet?) : SurfaceView(context, attrs), SurfaceHolder.Callback {

    // Constants defining the terminal
    internal val mTerminalWidth = 80
    internal val mTerminalHeight = 25
    internal val mDefaultForeground = Color.WHITE
    internal val mDefaultBackground = Color.BLACK

    // Thread object that does most of the work here, AnsiTerminalView is just a wrapper
    internal var mRenderThread: RenderThread

    // Thread object that runs the native C code, started after the SurfaceView is visible
    internal var mNativeThread: Thread? = null

    // Gesture detector
    private val mGestureDetector: GestureDetector? = null

    // Single element queue to store the last character in the virtual keyboard
    private val keyBuffer = LinkedBlockingQueue<Byte>()

    val packageVersion: Int
        get() {
            Logging.debug("Returning back VERSION_CODE " + BuildConfig.VERSION_CODE)
            return BuildConfig.VERSION_CODE
        }

    internal inner class RenderThread(var mSurfaceHolder: SurfaceHolder, var mContext: Context) : Thread() {
        var mRunning = true
        lateinit var mCanvas: Canvas
        var mCanvasRound = false
        lateinit var mBitmap: Bitmap
        var mPaintText: Paint
        var mPaintBackground: Paint
        var canvasWidth: Int = 0
        var canvasHeight: Int = 0
        var mCanvasWidthPadding: Int = 0
        var mCanvasHeightPadding: Int = 0
        var mCharWidth: Int = 0
        var mCharHeight: Int = 0
        var mCharWidthOffset: Int = 0
        var mCharHeightOffset: Int = 0
        var mCursorRow = 1
        var mCursorCol = 1
        var mCursorReverse = false
        private val ansiBuffer = LinkedBlockingQueue<ByteArray>()
        var mBackingStore: Array<Array<BackingStore>>

        var tempR = 1
        var tempC = 1

        var ansiColors = intArrayOf(Color.BLACK, Color.RED, Color.GREEN, Color.YELLOW, Color.BLUE, Color.MAGENTA, Color.CYAN, Color.WHITE)

        init {
            mPaintText = Paint()
            mPaintText.typeface = Typeface.create("monospace", Typeface.BOLD)
            mPaintText.color = mDefaultForeground

            mPaintBackground = Paint()
            mPaintBackground.color = mDefaultBackground

            mBackingStore = Array(mTerminalHeight, { Array(mTerminalWidth, { BackingStore() }) })
        }

        fun submitAnsiBuffer(len: Int, buffer: ByteArray) {
            try {
                if (len == 0) Logging.fatal("Empty buffer found")
                // Make a copy because the native code will reuse the buffer
                val copy = Arrays.copyOfRange(buffer, 0, len)
                // Logging.debug("Storing ANSI buffer with " + copy.length + " to the processing queue");
                ansiBuffer.put(copy)
            } catch (e: InterruptedException) {
                Logging.fatal("Failed to put into ANSI buffer with InterruptedException " + e)
            }

        }

        fun triggerAnsiRefresh() {
            // Used to trigger the ANSI parser for a refresh but do not do anything
            try {
                val b = ByteArray(0)
                ansiBuffer.put(b)
            } catch (e: InterruptedException) {
                Logging.fatal("Failed to put into ANSI buffer with InterruptedException " + e)
            }

        }

        // Given the current canvas dimensions, find the font size which fills up the screen
        // as much as possible given the desired terminal dimensions
        fun recalculateFontSize(terminalWidth: Int, terminalHeight: Int) {
            var fontSize = 1
            while (true) {
                if (fontSize >= 1000)
                    Logging.fatal("Unable to find suitable font size to fill canvas")
                mPaintText.textSize = fontSize.toFloat()

                val metrics = mPaintText.fontMetricsInt
                val rectW = Rect()
                mPaintText.getTextBounds("X", 0, 1, rectW) // How big is "X" horizontally when drawn
                if (rectW.width() == 0)
                    Logging.fatal("Invalid width from getTextBounds")
                val lastWidth = mCharWidth
                val lastHeight = mCharHeight
                mCharWidth = rectW.width()
                mCharHeight = metrics.bottom - metrics.top
                Logging.debug("Calculated for size=" + fontSize + " bounds=" + mCharWidth + "x" + mCharHeight + " for total=" + mCharWidth * terminalWidth + "x" + mCharHeight * terminalHeight + " canvas=" + canvasWidth + "x" + canvasHeight + " padding=" + 2 * mCanvasWidthPadding + "x" + 2 * mCanvasHeightPadding + " fontWidth=" + rectW.width() + " top=" + metrics.top + " bottom=" + metrics.bottom + " descent=" + metrics.descent)
                var done = false

                if (mCanvasRound) {
                    // We have a round watch, so the goal is to try and fit the rectangle to the radius of the circle
                    val x = mCharWidth * terminalWidth / 2.0
                    val y = mCharHeight * terminalHeight / 2.0
                    // Use ellipse formula (x/rw)^2+(y/rh)^2 to see if we are inside the circle still
                    val xrw = x / ((canvasWidth - 2 * mCanvasWidthPadding) / 2.0)
                    val yrw = y / ((canvasHeight - 2 * mCanvasHeightPadding) / 2.0)
                    if (xrw * xrw + yrw * yrw > 1.0)
                        done = true
                } else {
                    // Regular device, so just make sure we don't exceed the rectangle that we have
                    if (mCharWidth * terminalWidth > canvasWidth - 2 * mCanvasWidthPadding || mCharHeight * terminalHeight > canvasHeight - 2 * mCanvasHeightPadding) {
                        done = true
                    }
                }

                if (done) {
                    // This font size is too big, we need to go back one step and finish
                    fontSize--
                    mCharWidth = lastWidth
                    mCharHeight = lastHeight
                    if (mCharWidth <= 0 || mCharHeight <= 0)
                        Logging.fatal("Failed to compute expected width and height")
                    // Compute any remaining padding to center the image on the display
                    mCharWidthOffset = (canvasWidth - mCharWidth * terminalWidth) / 2
                    mCharHeightOffset = (canvasHeight - mCharHeight * terminalHeight) / 2
                    Logging.debug("Size is " + fontSize + ", offset is (" + mCharWidthOffset + "," + mCharHeightOffset + ") from terminal " + terminalWidth + "x" + terminalHeight + ", canvas " + canvasWidth + "x" + canvasHeight + " and char " + mCharWidth + "x" + mCharHeight)
                    return
                }
                fontSize++
            }
        }

        internal inner class BackingStore {
            var ch: Byte = 0
            var fg: Int = 0
            var bg: Int = 0
            fun BackingStore() {
                reset(mDefaultForeground, mDefaultBackground)
            }

            fun reset(f: Int, b: Int) {
                ch = '~'.toByte()
                fg = f
                bg = b
            }
        }

        fun refreshBackingStore(canvas: Canvas) {
            // The bitmap was destroyed and needs to have the backing store redrawn
            val metrics = mPaintText.fontMetricsInt
            // Create local paint objects and copy over things like font settings, although
            // we will set the color in the loop below for each character
            val foreground = Paint()
            foreground.set(mPaintText)
            val background = Paint()
            background.set(mPaintBackground)
            for (row in 0 until mTerminalHeight)
                for (col in 0 until mTerminalWidth) {
                    // Note the array is zero based, so code is different than drawFixedChar
                    val element = mBackingStore[row][col]

                    val x = mCharWidthOffset + col * mCharWidth
                    val y = mCharHeightOffset + (row + 1) * mCharHeight
                    // Clear the box with the current background attributes
                    foreground.color = element.fg
                    background.color = element.bg
                    canvas.drawRect(x.toFloat(), (y + metrics.top).toFloat(), (x + mCharWidth + 1).toFloat(), (y + metrics.bottom).toFloat(), background)
                    // Draw the text over the top with the foreground attributes
                    canvas.drawText((element.ch.toInt() and 0xFF).toChar().toString(), x.toFloat(), y.toFloat(), foreground)
                }
        }

        fun drawFixedChar(canvas: Canvas, b: Byte, row: Int, col: Int) {
            val metrics = mPaintText.fontMetricsInt
            val x = mCharWidthOffset + (col - 1) * mCharWidth
            val y = mCharHeightOffset + row * mCharHeight
            // Clear the box with the current background attributes
            canvas.drawRect(x.toFloat(), (y + metrics.top).toFloat(), (x + mCharWidth + 1).toFloat(), (y + metrics.bottom).toFloat(), mPaintBackground)
            // Draw the text over the top with the foreground attributes
            canvas.drawText((b.toInt() and 0xFF).toChar().toString(), x.toFloat(), y.toFloat(), mPaintText)
            // Update the backing store (note the array indexes start at zero)
            mBackingStore[row - 1][col - 1].ch = b
            mBackingStore[row - 1][col - 1].fg = mPaintText.color
            mBackingStore[row - 1][col - 1].bg = mPaintBackground.color
            // Logging.debug ("Drawing " + String.valueOf((char)(b&0xFF)) + " at RC" + row + "," + col + " at XY" + x + "," + y + " with OFS" + mCharWidthOffset + "," + mCharHeightOffset);
        }

        fun drawClear(canvas: Canvas) {
            for (row in 0 until mTerminalHeight)
                for (col in 0 until mTerminalWidth)
                    mBackingStore[row][col].reset(mDefaultBackground, mDefaultBackground)
            canvas.drawColor(mDefaultBackground)
        }

        fun doDraw(canvas: Canvas, buffer: ByteArray) {
            // Logging.debug("Received ANSI buffer with " + buffer.length + " bytes from native code");
            drawAnsiBuffer(canvas, buffer)
        }

        fun doDrawDebug(canvas: Canvas) {
            val ansiTest = StringBuilder()
            ansiTest.append(AnsiString.defaultAttr())
            ansiTest.append(AnsiString.clearScreen())

            // Debug the full layout of the display
            ansiTest.append(AnsiString.putDebugGrid(mTerminalWidth, mTerminalHeight))
            ansiTest.append(AnsiString.putString(2, 1, " "))
            ansiTest.append(AnsiString.putString(1, 2, " "))
            ansiTest.append(AnsiString.putString(mTerminalHeight, mTerminalWidth - 1, " "))
            ansiTest.append(AnsiString.putString(mTerminalHeight - 1, mTerminalWidth, " "))

            // Draw some ANSI text over the top of everything
            ansiTest.append(AnsiString.putString(2, 2, "Hello ANSI default"))
            ansiTest.append(AnsiString.setForeground(AnsiString.BLUE))
            ansiTest.append(AnsiString.putString(3, 3, "Hello ANSI blue"))
            ansiTest.append(AnsiString.setForeground(AnsiString.CYAN))
            ansiTest.append(AnsiString.putString(4, 4, "Hello ANSI cyan"))
            ansiTest.append(AnsiString.setColor(AnsiString.REVERSE, AnsiString.WHITE, AnsiString.BLACK))
            ansiTest.append(AnsiString.putString(5, 5, "Inverted black on white"))
            ansiTest.append(AnsiString.defaultAttr())
            ansiTest.append(AnsiString.putString(6, 6, "Normal white on black"))
            ansiTest.append(AnsiString.setForeground(AnsiString.RED))
            ansiTest.append(AnsiString.setBackground(AnsiString.YELLOW))
            ansiTest.append(AnsiString.putString(7, 7, "Red on yellow"))
            ansiTest.append(AnsiString.setAttr(AnsiString.REVERSE))
            ansiTest.append(AnsiString.putString(8, 8, "Inverted with yellow on red"))
            ansiTest.append(AnsiString.defaultAttr())
            ansiTest.append(AnsiString.putString(9, 9, "Normal white on black"))
            ansiTest.append(AnsiString.setForeground(AnsiString.GREEN))
            ansiTest.append(AnsiString.setAttr(AnsiString.REVERSE))
            ansiTest.append(AnsiString.putString(20, 10, "Green inverse text at row 20, col 10"))
            ansiTest.append(AnsiString.setAttr(AnsiString.NORMAL))
            ansiTest.append(AnsiString.setForeground(AnsiString.YELLOW))
            ansiTest.append(AnsiString.putString(10, 20, "Yellow text at row 10, col 20"))
            ansiTest.append(AnsiString.setAttr(AnsiString.NORMAL))
            ansiTest.append(AnsiString.putString(11, 11, "________________"))
            ansiTest.append(AnsiString.putString(12, 12, "________________"))
            ansiTest.append(AnsiString.putString(13, 13, "yyyyyyyyyyyyyyyy"))
            ansiTest.append(AnsiString.putString(14, 14, "yyyyyyyyyyyyyyyy"))

            // Now parse the ANSI buffer and render it
            drawAnsiBuffer(canvas, ansiTest.toString())
        }

        override fun run() {
            while (mRunning) {
                var c: Canvas? = null
                try {
                    // Block until ANSI commands are available, and then render them
                    // Logging.debug ("Waiting for new ANSI buffer, sleeping ...");
                    val buffer = ansiBuffer.take()
                    // Logging.debug ("Rendering new ANSI buffer of length " + buffer.length);

                    // Do the drawing to our local bitmap
                    doDraw(mCanvas, buffer)
                    // doDrawDebug(mCanvas);

                    // Lock the canvas and render the buffer to the display
                    c = mSurfaceHolder.lockCanvas(null)
                    synchronized(mSurfaceHolder) {
                        // Copy our local bitmap to the surface canvas provided
                        c!!.drawBitmap(mBitmap, 0f, 0f, null)
                    }
                } catch (e: InterruptedException) {
                    Logging.fatal("ansiBuffer.take() failed with InterruptedException " + e)
                } finally {
                    if (c != null) {
                        mSurfaceHolder.unlockCanvasAndPost(c)
                    }
                }
            }
        }

        fun setSurfaceRound(round: Boolean) {
            synchronized(mSurfaceHolder) {
                mCanvasRound = round
            }
        }

        fun setSurfaceSize(width: Int, height: Int, overscanPercent: Float) {
            synchronized(mSurfaceHolder) {
                // Resize the internal bitmap to match the new display dimensions
                canvasWidth = width
                canvasHeight = height
                mCanvasWidthPadding = (canvasWidth * overscanPercent).toInt()
                mCanvasHeightPadding = (canvasHeight * overscanPercent).toInt()
                mBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888)
                mCanvas = Canvas(mBitmap)
                Logging.debug("Detected change in surface size to " + width + "x" + height)

                // Recalculate the font size which fits into the display
                recalculateFontSize(mTerminalWidth, mTerminalHeight)

                // Replay the backing store into the new resized canvas
                refreshBackingStore(mCanvas)

                // Trigger off the onDraw() method by generating an empty ANSI sequence
                triggerAnsiRefresh()
            }
        }

        // Pass in any arbitrary string which contains text and ANSI escape sequences for rendering
        fun drawAnsiBuffer(canvas: Canvas, str: String) {
            var bytes: ByteArray? = null
            try {
                bytes = str.toByteArray(charset("US-ASCII"))
            } catch (e: UnsupportedEncodingException) {
                Logging.fatal("Cannot convert string to ASCII")
            }

            drawAnsiBuffer(canvas, bytes!!)
        }

        internal inner class ByteStream(private val mBytes: ByteArray) {

            private var ofs: Int = 0
            val End: Byte = '\u0000'.toByte()

            val byte: Byte
                get() {
                    if (ofs < mBytes.size) {
                        val result = mBytes[ofs]
                        ofs++
                        return result
                    } else {
                        return End
                    }
                }
        }

        fun isAnsiNumberList(`in`: Byte): Boolean {
            return if (`in` >= '0'.toByte() && `in` <= '9'.toByte())
                true
            else if (`in` == ';'.toByte())
                true
            else
                false
        }

        fun drawAnsiBuffer(canvas: Canvas, bytes: ByteArray) {
            val buffer = ByteStream(bytes)
            var current = buffer.byte
            while (current != buffer.End) {
                if (current == '\u001b'.toByte()) {
                    // Start of ANSI sequence
                    current = buffer.byte
                    if (current == '['.toByte()) {
                        // Definitely ANSI sequence. So need to search for a list of integers
                        // separated by ; and ending with a single ending character
                        val values = ArrayList<Int>()
                        var parseValue = -1
                        current = buffer.byte
                        while (isAnsiNumberList(current) && current != buffer.End) {
                            if (current == ';'.toByte()) {
                                if (parseValue == -1)
                                    Logging.fatal("Unexpected empty parseValue")
                                values.add(parseValue)
                                parseValue = -1
                            } else {
                                if (parseValue == -1) parseValue = 0
                                parseValue *= 10 // Shift current value over by multiplying
                                parseValue += current - '0'.toByte() // Store character as an integer
                            }
                            current = buffer.byte
                        }
                        // We have finished our sequence, if the parseValue is valid then add this
                        if (parseValue != -1)
                            values.add(parseValue)
                        // Do we have a character at the end? This tells us what to do next
                        when ((current.toInt() and 0xFF).toChar()) {
                            'H' // Set absolute position to row;col
                            -> {
                                if (values.size != 2)
                                    Logging.fatal("Found " + values.size + " instead of expected 2 values for ANSI H command")
                                mCursorRow = values[0]
                                mCursorCol = values[1]
                            }
                            'J' // Clear screen, value is always 2
                            -> drawClear(canvas)
                            'm' // Set color or attribute. 0 is default. 3x is FG, 4x is BG
                            -> {
                                if (values.size != 1)
                                    Logging.fatal("Found " + values.size + " instead of expected 1 value for ANSI m command")
                                if (values[0] == 0) { // Normal attribute
                                    mPaintText.color = mDefaultForeground
                                    mPaintBackground.color = mDefaultBackground
                                    mCursorReverse = false
                                } else if (values[0] == 7) { // Reverse attribute
                                    if (!mCursorReverse) {
                                        val temp = mPaintText.color
                                        mPaintText.color = mPaintBackground.color
                                        mPaintBackground.color = temp
                                        mCursorReverse = true
                                    }
                                } else if (values[0] >= 30 && values[0] <= 39) {
                                    val index = values[0] - 30
                                    if (index >= ansiColors.size)
                                        Logging.fatal("Found ANSI color " + values[0] + " which is too large for ANSI m command")
                                    if (!mCursorReverse)
                                        mPaintText.color = ansiColors[index]
                                    else
                                        mPaintBackground.color = ansiColors[index]
                                } else if (values[0] >= 40 && values[0] <= 49) {
                                    val index = values[0] - 40
                                    if (index >= ansiColors.size)
                                        Logging.fatal("Found ANSI color " + values[0] + " which is too large for ANSI m command")
                                    if (!mCursorReverse)
                                        mPaintBackground.color = ansiColors[index]
                                    else
                                        mPaintText.color = ansiColors[index]
                                } else {
                                    Logging.fatal("Unsupported color value " + values[0] + " for ANSI m command")
                                }
                            }
                            else -> Logging.fatal("Unknown ANSI command " + current)
                        }
                        // The sequence was processed successfully!
                    } else {
                        // Invalid ANSI, throw an exception
                        Logging.fatal("Found start ESC but found $current instead of left bracket")
                    }
                } else if (current == '\n'.toByte()) {
                    // Carriage return, so go to the next line
                    mCursorCol = 1
                    mCursorRow++
                } else {
                    // Regular character, just print it out using the current paint attributes
                    drawFixedChar(canvas, current, mCursorRow, mCursorCol)

                    // Move the cursor to the next spot
                    mCursorCol++
                    if (mCursorCol >= mTerminalWidth) {
                        mCursorCol = 1
                        mCursorRow++
                    }
                }

                // We processed this character successfully, so go to the next one now
                current = buffer.byte
            }
        }
    }


    init {

        // Callback to tell us when the surface is changed
        val holder = holder
        holder.addCallback(this)

        // Create the object to manage drawing, but do not start it here, or we can get into
        // a race condition where the internal bitmap is not ready. Wait until surfaceChanged().
        mRenderThread = RenderThread(holder, context)

        // Get key events
        isFocusable = true
    }

    fun surfaceRound(round: Boolean) {
        mRenderThread.setSurfaceRound(round)
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        Logging.debug("surfaceChanged")

        // Detect if we are running on a TV which needs overscan padding of 10% on all sides
        val overscanPercent: Float
        val uiModeManager = mRenderThread.mContext.getSystemService(Context.UI_MODE_SERVICE) as UiModeManager
        val isTV = uiModeManager.currentModeType == Configuration.UI_MODE_TYPE_TELEVISION
        overscanPercent = if (isTV == true) 0.1f else 0.0f

        // Resize the internal bitmap to match the size of the surface we have been given
        mRenderThread.setSurfaceSize(width, height, overscanPercent)

        // Create thread to run our native code implementation
        if (mNativeThread == null) {
            // Start the rendering thread that will manage the surface interactions
            mRenderThread.start()

            // Start the native code, which generates the ANSI strings and reads inputs
            Logging.debug("Starting thread for nativeAnsiCode")
            mNativeThread = Thread(Runnable {
                Logging.debug("Calling into JNI nativeAnsiCode()")
                nativeAnsiCode()
                Logging.fatal("nativeAnsiCode() has returned unexpectedly")
            })
            mNativeThread!!.start()
        }
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        // Do nothing in here, we don't know the dimensions yet, so lets wait for surfaceChanged
        // so that we know everything is ready and the bitmap is created properly
        Logging.debug("surfaceCreated")
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        Logging.debug("surfaceDestroyed")
        // Officially, we should notify both render and native threads that we are done, they should
        // finish their work and exit, then we should wait here for them to finish. However, surfaceDestroyed
        // is called when the Activity is being hidden, so we might as well just exit and be done with it.
        Logging.debug("surfaceDestroyed not implemented yet, causing a clean exit to shut everything down")
        System.exit(0)
    }

    fun submitAnsiBuffer(len: Int, buffer: ByteArray) {
        // Native code will call this to submit a new buffer for processing in the queue
        mRenderThread.submitAnsiBuffer(len, buffer)
    }

    // Method to inject keyboard events into the queue
    fun injectKeyboardEvent(key: Byte) {
        Logging.debug("Generating keystroke '" + (key.toInt() and 0xFF).toChar().toString() + "'")
        keyBuffer.clear()
        try {
            keyBuffer.put(key)
        } catch (e: InterruptedException) {
            Logging.fatal("Failed to store keystroke from " + e)
        }

    }

    // Handle gamepad devices and the D-pad
    override fun onKeyDown(keyCode: Int, ev: KeyEvent): Boolean {
        var result: Byte = 0
        when (keyCode) {
            KeyEvent.KEYCODE_DPAD_LEFT -> result = '4'.toByte()
            KeyEvent.KEYCODE_DPAD_RIGHT -> result = '6'.toByte()
            KeyEvent.KEYCODE_DPAD_UP -> result = '8'.toByte()
            KeyEvent.KEYCODE_DPAD_DOWN -> result = '2'.toByte()
            KeyEvent.KEYCODE_DPAD_CENTER, KeyEvent.KEYCODE_BUTTON_A, KeyEvent.KEYCODE_BUTTON_B, KeyEvent.KEYCODE_BUTTON_C, KeyEvent.KEYCODE_ENTER, KeyEvent.KEYCODE_SPACE -> result = '\n'.toByte()
            KeyEvent.KEYCODE_BACK, KeyEvent.KEYCODE_HOME -> System.exit(0) // Exit out of the game
        }
        if (result.toInt() == 0) {
            Logging.debug("Ignoring onKeyDown event for unknown keyCode " + keyCode)
        } else {
            injectKeyboardEvent(result)
        }
        return true // We processed this event
    }

    override fun onTouchEvent(ev: MotionEvent): Boolean {
        if (ev.action == MotionEvent.ACTION_DOWN) {
            // This is a touch event, so lets decide what to do with it. The 25% edges are defined
            // to map to the numeric keypad 4,8,6,2 keys, and the center maps to the space bar.
            val width = mRenderThread.canvasWidth
            val height = mRenderThread.canvasHeight
            var result: Byte
            if (ev.x < width * 1 / 4 && ev.y > height * 1 / 4 && ev.y < height * 3 / 4)
                result = '4'.toByte() // Left
            else if (ev.x > width * 3 / 4 && ev.y > height * 1 / 4 && ev.y < height * 3 / 4)
                result = '6'.toByte() // Right
            else if (ev.y < height * 1 / 4 && ev.x > width * 1 / 4 && ev.x < width * 3 / 4)
                result = '8'.toByte() // Up
            else if (ev.y > height * 3 / 4 && ev.x > width * 1 / 4 && ev.x < width * 3 / 4)
                result = '2'.toByte() // Down
            else
                result = '\n'.toByte() // Middle is enter key
            injectKeyboardEvent(result)
        }
        return true // We processed this event
    }

    fun blockUntilKeypress() {
        // Native code will call this, sleep until a key is available, but do not read it
        try {
            // Sleep here
            val b = keyBuffer.take().toByte()
            // Put it back into the buffer
            keyBuffer.put(b)
            // Logging.debug("Found that key " + b + " is now available, it is ready in the buffer");
        } catch (e: InterruptedException) {
            Logging.fatal("Failed to read keystroke from " + e)
        }
    }

    fun timeUntilKeypress(microseconds: Int): Boolean {
        // Logging.debug("Waiting for " + microseconds + " usec in timeUntilKeypress");
        // Native code will call this, sleep until a key is available, but do not read it
        var ret = false
        try {
            // Sleep here
            val result = keyBuffer.poll(microseconds.toLong(), TimeUnit.MICROSECONDS)
            if (result != null) {
                keyBuffer.put(result) // Put it back
                ret = true
            }
        } catch (e: InterruptedException) {
            Logging.fatal("Failed to read keystroke from " + e)
        }

        return ret
    }


    fun waitForKeypress(): Byte {
        // Native code will call this to block for a key press
        var b: Byte = 0
        try {
            b = keyBuffer.take().toByte()
        } catch (e: InterruptedException) {
            Logging.fatal("Failed to read keystroke from " + e)
        }

        // Logging.debug("Unblocking from character " + b + " in waitForKeypress");
        return b
    }

    fun waitForArrowpress(): Byte {
        // Native code will call this to block for a key press
        // Do not touch the input buffer if the result is not a numeric keypad direction
        var b: Byte = 0
        try {
            b = keyBuffer.take().toByte()
            if (b != '4'.toByte() && b != '8'.toByte() && b != '6'.toByte() && b != '2'.toByte()) {
                // Not a direction key, so put the byte back
                keyBuffer.put(b)
                // Return back DIR_unknown which the native code knows how to interpret
                b = -1
            }
        } catch (e: InterruptedException) {
            Logging.fatal("Failed to read keystroke from " + e)
        }

        // Logging.debug("Unblocking from character " + b + " in waitForArrowpress");
        return b
    }


    fun pollForKeypress(): Byte {
        // Native code will call this to read a key but do not block if none available
        var b: Byte = 0
        val result = keyBuffer.poll()
        if (result != null)
            b = result.toByte()
        Logging.debug("Polled with character " + b)
        return b
    }

    fun checkForKeypress(): Boolean {
        // Native code will call this to see if there is a key available
        return !keyBuffer.isEmpty()
    }

    fun clearForKeypress() {
        // Flush out all key presses
        keyBuffer.clear()
    }

    // This will be called within a new thread and can make calls to the three methods above
    external fun nativeAnsiCode()

    companion object {
        init {
            System.loadLibrary("native-jni")
        }
    }
}
