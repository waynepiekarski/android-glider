package net.waynepiekarski.ansiterminal;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;

public class AnsiTerminalView extends SurfaceView implements SurfaceHolder.Callback {

    // Thread object that does most of the work here, AnsiTerminalView is just a wrapper
    RenderThread mRenderThread;

    // Thread object that runs the native C code, started after the SurfaceView is visible
    Thread mNativeThread = null;

    class RenderThread extends Thread {
        public SurfaceHolder mSurfaceHolder;
        public Context mContext;
        public boolean mRunning = true;
        public Canvas mCanvas;
        public Bitmap mBitmap;
        public Paint mPaintText;
        public Paint mPaintBackground;
        public int mCanvasWidth;
        public int mCanvasHeight;
        public boolean mCanvasDirty;
        public int mCharWidth;
        public int mCharHeight;
        public int mCharWidthOffset;
        public int mCharHeightOffset;
        public static final int mCharSpacingX = 2;
        public static final int mCharSpacingY = 10;
        public static final int mTerminalWidth = 80;
        public static final int mTerminalHeight = 25;
        public static final int mDefaultForeground = Color.WHITE;
        public static final int mDefaultBackground = Color.BLACK;
        public int mCursorRow = 0;
        public int mCursorCol = 0;
        public boolean mCursorReverse = false;
        private LinkedBlockingQueue<byte[]> ansiBuffer = new LinkedBlockingQueue<byte[]>();

        public RenderThread (SurfaceHolder holder, Context context) {
            mSurfaceHolder = holder;
            mContext = context;

            mPaintText = new Paint();
            mPaintText.setTypeface(Typeface.create("Monospace", Typeface.BOLD));
            mPaintText.setColor(mDefaultForeground);

            mPaintBackground = new Paint();
            mPaintBackground.setColor(mDefaultBackground);

            mCanvasDirty = true;
        }

        public void submitAnsiBuffer(int len, byte[] buffer) {
            try {
                if (len == 0) Logging.fatal("Empty buffer found");
                // Make a copy because the native code will reuse the buffer
                byte[] copy = Arrays.copyOfRange(buffer, 0, len);
                Logging.debug("Storing ANSI buffer with " + copy.length + " to the processing queue");
                ansiBuffer.put(copy);
            } catch (InterruptedException e) {
                Logging.fatal("Failed to put into ANSI buffer with InterruptedException " + e);
            }
        }

        // Given the current canvas dimensions, find the font size which fills up the screen
        // as much as possible given the desired terminal dimensions
        public void recalculateFontSize (Canvas canvas, int terminalWidth, int terminalHeight) {
            int fontSize = 1;
            int lastWidth = -1;
            int lastHeight = -1;
            while (true) {
                if (fontSize >= 1000)
                    Logging.fatal("Unable to find suitable font size to fill canvas");
                mPaintText.setTextSize(fontSize);
                Rect rect = new Rect();
                mPaintText.getTextBounds("X", 0, 1, rect); // How big is "X" when drawn
                if ((rect.width() == 0) || (rect.height() == 0))
                    Logging.fatal("Invalid width or height from getTextBounds");
                lastWidth = mCharWidth;
                lastHeight = mCharHeight;
                mCharWidth = rect.width() + mCharSpacingX;
                mCharHeight = rect.height() + mCharSpacingY;
                Logging.debug("Calculated for size=" + fontSize + " bounds=" + mCharWidth + "x" + mCharHeight + " for total=" + (mCharWidth*terminalWidth) + "x" + (mCharHeight*terminalHeight) + " canvas=" + mCanvasWidth + "x" + mCanvasHeight);
                if ((mCharWidth * terminalWidth > mCanvasWidth) || (mCharHeight * terminalHeight > mCanvasHeight)) {
                    // This font size is too big, we need to go back one step and finish
                    fontSize--;
                    mCharWidth = lastWidth;
                    mCharHeight = lastHeight;
                    if ((mCharWidth <= 0) || (mCharHeight <= 0))
                        Logging.fatal("Failed to compute expected width and height");
                    // Compute any remaining padding to center the image on the display
                    mCharWidthOffset = (mCanvasWidth - mCharWidth*terminalWidth) / 2;
                    mCharHeightOffset = (mCanvasHeight - mCharHeight*terminalHeight) / 2;
                    Logging.debug("Size is " + fontSize + ", offset is (" + mCharWidthOffset + "," + mCharHeightOffset + ") from terminal " + terminalWidth + "x" + terminalHeight + ", canvas " + mCanvasWidth + "x" + mCanvasHeight + " and char " + mCharWidth + "x" + mCharHeight);
                    return;
                }
                fontSize++;
            }
        }

        public void clearFixedChar(Canvas canvas, int row, int col) {
            int x = mCharWidthOffset + (col-1) * mCharWidth;
            int y = mCharHeightOffset + row * mCharHeight;
            canvas.drawRect(x, y - mCharHeight + mCharSpacingY - mCharSpacingY/2, x + mCharWidth, y + mCharSpacingY/2, mPaintBackground);
        }

        public void clearFixedString(Canvas canvas, String str, int row, int col, int border) {
            int clen = border*2 + str.length();
            int rlen = border*2 + 1;
            for (int c = 1; c <= clen; c++)
                for (int r = 1; r <= rlen; r++)
                    clearFixedChar(canvas, row+r-border, col+c-border);
        }

        public StringBuilder ansiClearFixedString(String str, int row, int col, int border) {
            StringBuilder result = new StringBuilder();
            int clen = border*2 + str.length();
            int rlen = border*2 + 1;
            for (int c = 0; c < clen; c++)
                for (int r = 0; r < rlen; r++)
                    // Make sure we never generate a negative coordinate, but overflow is good
                    if ((col+c-border >= 1) && (row+r-border >= 1))
                        result.append(AnsiString.putChar(row+r-border, col+c-border, ' '));
            return result;
        }

        public void drawFixedChar (Canvas canvas, char ch, int row, int col) {
            int x = mCharWidthOffset + (col-1) * mCharWidth;
            int y = mCharHeightOffset + row * mCharHeight;
            canvas.drawText(String.valueOf(ch), x, y, mPaintText);
        }

        public void drawFixedChar (Canvas canvas, byte b, int row, int col) {
            int x = mCharWidthOffset + (col-1) * mCharWidth;
            int y = mCharHeightOffset + row * mCharHeight;
            canvas.drawText(String.valueOf((char) (b & 0xFF)), x, y, mPaintText);
            // Logging.debug ("Drawing " + String.valueOf((char)(b&0xFF)) + " at RC" + row + "," + col + " at XY" + x + "," + y + " with OFS" + mCharWidthOffset + "," + mCharHeightOffset);
        }

        public void drawFixedString(Canvas canvas, String str, int row, int col) {
            int c = col;
            for (char ch : str.toCharArray()) {
                drawFixedChar(canvas, ch, row, c);
                c++;
            }
        }

        public void drawDebug(Canvas canvas, int width, int height) {
            for (int c = 1; c <= width; c++) {
                for (int r = 1; r <= height; r++) {
                    if ((c == 1) || (c == width) || (r == 1) || (r == height))
                        drawFixedChar(canvas, '#', r, c);
                    else {
                        drawFixedChar(canvas, Character.forDigit(c % 10, 10), r, c);
                    }
                }
            }
        }

        public StringBuilder ansiDrawDebug(int width, int height) {
            StringBuilder result = new StringBuilder();
            for (int c = 1; c <= width; c++) {
                for (int r = 1; r <= height; r++) {
                    if ((c == 1) || (c == width) || (r == 1) || (r == height))
                        result.append(AnsiString.putChar(r, c, '#'));
                    else {
                        result.append(AnsiString.putChar(r, c, Character.forDigit(c % 10, 10)));
                    }
                }
            }
            return result;
        }

        public void drawClear(Canvas canvas) {
            canvas.drawColor(mDefaultBackground);
        }

        public void doDraw(Canvas canvas, byte[] buffer) {
            if (mCanvasDirty) {
                mCanvasDirty = false;
                recalculateFontSize(canvas, mTerminalWidth, mTerminalHeight);
            }
            Logging.debug("Received ANSI buffer with " + buffer.length + " bytes from native code");
            drawAnsiBuffer(canvas, buffer);
        }

        int tempR = 1;
        int tempC = 1;
        public void doDrawDebug(Canvas canvas) {
            if (mCanvasDirty) {
                mCanvasDirty = false;
                recalculateFontSize(canvas, mTerminalWidth, mTerminalHeight);
            }

            StringBuilder ansiTest = new StringBuilder();
            ansiTest.append(AnsiString.defaultAttr());
            ansiTest.append(AnsiString.clearScreen());

            // Debug the full layout of the display
            ansiTest.append(ansiDrawDebug(mTerminalWidth, mTerminalHeight));
            ansiTest.append(AnsiString.putString(2, 1, " "));
            ansiTest.append(AnsiString.putString(1, 2, " "));
            ansiTest.append(AnsiString.putString(mTerminalHeight, mTerminalWidth-1, " "));
            ansiTest.append(AnsiString.putString(mTerminalHeight-1, mTerminalWidth, " "));

            // Draw some ANSI text over the top of everything
            ansiTest.append(AnsiString.putString(2, 2, "Hello ANSI default"));
            ansiTest.append(AnsiString.setForeground(AnsiString.BLUE));
            ansiTest.append(AnsiString.putString(3, 3, "Hello ANSI blue"));
            ansiTest.append(AnsiString.setForeground(AnsiString.CYAN));
            ansiTest.append(AnsiString.putString(4, 4, "Hello ANSI cyan"));
            ansiTest.append(AnsiString.setColor(AnsiString.REVERSE, AnsiString.WHITE, AnsiString.BLACK));
            ansiTest.append(AnsiString.putString(5, 5, "Inverted black on white"));
            ansiTest.append(AnsiString.defaultAttr());
            ansiTest.append(AnsiString.putString(6, 6, "Normal white on black"));
            ansiTest.append(AnsiString.setForeground(AnsiString.RED));
            ansiTest.append(AnsiString.setBackground(AnsiString.YELLOW));
            ansiTest.append(AnsiString.putString(7, 7, "Red on yellow"));
            ansiTest.append(AnsiString.setAttr(AnsiString.REVERSE));
            ansiTest.append(AnsiString.putString(8, 8, "Inverted with yellow on red"));
            ansiTest.append(AnsiString.defaultAttr());
            ansiTest.append(AnsiString.putString(9, 9, "Normal white on black"));
            ansiTest.append(AnsiString.setForeground(AnsiString.GREEN));
            ansiTest.append(AnsiString.setAttr(AnsiString.REVERSE));
            ansiTest.append(AnsiString.putString(20, 10, "Green inverse text at row 20, col 10"));
            ansiTest.append(AnsiString.setAttr(AnsiString.NORMAL));
            ansiTest.append(AnsiString.setForeground(AnsiString.YELLOW));
            ansiTest.append(AnsiString.putString(10, 20, "Yellow text at row 10, col 20"));
            ansiTest.append(AnsiString.setAttr(AnsiString.NORMAL));
            ansiTest.append(AnsiString.putString(11, 11, "________________"));
            ansiTest.append(AnsiString.putString(12, 12, "________________"));
            ansiTest.append(AnsiString.putString(13, 13, "yyyyyyyyyyyyyyyy"));
            ansiTest.append(AnsiString.putString(14, 14, "yyyyyyyyyyyyyyyy"));

            ansiTest.append(AnsiString.setBackground(AnsiString.GREEN));
            ansiTest.append(AnsiString.setForeground(AnsiString.RED));
            ansiTest.append(ansiClearFixedString("Clear box should not wrap-around", 10, 75, 1));
            ansiTest.append(AnsiString.putString(10, 75, "Clear box should not wrap-around"));

            // Animated text with clear box to show things are updating
            ansiTest.append(AnsiString.defaultAttr());
            ansiTest.append(ansiClearFixedString("R=" + tempR + ",C=" + tempC, tempR, tempC, 1));
            ansiTest.append(AnsiString.putString(tempR, tempC, "R="+tempR+",C="+tempC));
            tempR += 1;
            if (tempR > 25) tempR = 1;
            tempC += 1;
            if (tempC > 80) tempC = 1;

            // Now parse the ANSI buffer and render it
            drawAnsiBuffer(canvas, ansiTest.toString());
        }

        public void run() {
            while (mRunning) {
                Canvas c = null;
                try {
                    // Block until ANSI commands are available, and then render them
                    Logging.debug ("Waiting for new ANSI buffer, sleeping ...");
                    byte[] buffer = ansiBuffer.take();
                    Logging.debug ("Rendering new ANSI buffer of length " + buffer.length);

                    // Do the drawing to our local bitmap
                    doDraw(mCanvas, buffer);
                    // doDrawDebug(mCanvas);

                    // Lock the canvas and render the buffer to the display
                    c = mSurfaceHolder.lockCanvas(null);
                    synchronized (mSurfaceHolder) {
                        // Copy our local bitmap to the surface canvas provided
                        c.drawBitmap(mBitmap, 0, 0, null);
                    }
                } catch (InterruptedException e) {
                    Logging.fatal("ansiBuffer.take() failed with InterruptedException " + e);
                } finally {
                    if (c != null) {
                        mSurfaceHolder.unlockCanvasAndPost(c);
                    }
                }
            }
        }

        public void setSurfaceSize(int width, int height) {
            synchronized (mSurfaceHolder) {
                // Change bitmap attributes atomically
                mCanvasWidth = width;
                mCanvasHeight = height;
                mCanvasDirty = true;
                mBitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
                mCanvas = new Canvas (mBitmap);
                Logging.debug("Detected change in surface size to " + width + "x" + height);
            }
        }

        // Pass in any arbitrary string which contains text and ANSI escape sequences for rendering
        public void drawAnsiBuffer(Canvas canvas, String str)
        {
            byte[] bytes = null;
            try {
                bytes = str.getBytes("US-ASCII");
            } catch (UnsupportedEncodingException e) {
                Logging.fatal ("Cannot convert string to ASCII");
            }
            drawAnsiBuffer(canvas, bytes);
        }

        class ByteStream {
            ByteStream(byte[] bytes) {
                mBytes = bytes;
                ofs = 0;
            }

            public byte getByte()
            {
                if (ofs < mBytes.length)
                {
                    byte result = mBytes[ofs];
                    ofs++;
                    return result;
                }
                else
                {
                    return End;
                }
            }

            static final byte End = '\0';

            private int ofs;
            private byte[] mBytes;
        }

        public boolean isAnsiNumberList(byte in) {
            if ((in >= '0') && (in <= '9'))
                return true;
            else if (in == ';')
                return true;
            else
                return false;
        }

        public int ansiColors[] = { Color.BLACK, Color.RED, Color.GREEN, Color.YELLOW, Color.BLUE, Color.MAGENTA, Color.CYAN, Color.WHITE };

        public void drawAnsiBuffer(Canvas canvas, byte[] bytes) {
            int ofs = 0;
            ByteStream buffer = new ByteStream(bytes);
            byte current = buffer.getByte();
            while (current != buffer.End)
            {
                if (current == '\033') {
                    // Start of ANSI sequence
                    current = buffer.getByte();
                    if (current == '[') {
                        // Definitely ANSI sequence. So need to search for a list of integers
                        // separated by ; and ending with a single ending character
                        ArrayList<Integer> values = new ArrayList<Integer>();
                        int parseValue = -1;
                        current = buffer.getByte();
                        while (isAnsiNumberList(current) && (current != buffer.End)) {
                            if (current == ';') {
                                if (parseValue == -1)
                                    Logging.fatal("Unexpected empty parseValue");
                                values.add(parseValue);
                                parseValue = -1;
                            } else {
                                if (parseValue == -1) parseValue = 0;
                                parseValue *= 10; // Shift current value over by multiplying
                                parseValue += (current - '0'); // Store character as an integer
                            }
                            current = buffer.getByte();
                        }
                        // We have finished our sequence, if the parseValue is valid then add this
                        if (parseValue != -1)
                            values.add(parseValue);
                        // Do we have a character at the end? This tells us what to do next
                        switch (current) {
                            case 'H': // Set absolute position to row;col
                                if (values.size() != 2)
                                    Logging.fatal("Found " + values.size() + " instead of expected 2 values for ANSI H command");
                                mCursorRow = values.get(0);
                                mCursorCol = values.get(1);
                                break;
                            case 'J': // Clear screen, value is always 2
                                drawClear(canvas);
                                break;
                            case 'm': // Set color or attribute. 0 is default. 3x is FG, 4x is BG
                                if (values.size() != 1)
                                    Logging.fatal("Found " + values.size() + " instead of expected 1 value for ANSI m command");
                                if (values.get(0) == 0) { // Normal attribute
                                    mPaintText.setColor(mDefaultForeground);
                                    mPaintBackground.setColor(mDefaultBackground);
                                    mCursorReverse = false;
                                } else if (values.get(0) == 7) { // Reverse attribute
                                    if (!mCursorReverse) {
                                        int temp = mPaintText.getColor();
                                        mPaintText.setColor(mPaintBackground.getColor());
                                        mPaintBackground.setColor(temp);
                                        mCursorReverse = true;
                                    }
                                } else if ((values.get(0) >= 30) && (values.get(0) <= 39)) {
                                    int index = values.get(0) - 30;
                                    if (index >= ansiColors.length)
                                        Logging.fatal("Found ANSI color " + values.get(0) + " which is too large for ANSI m command");
                                    if (!mCursorReverse)
                                       mPaintText.setColor(ansiColors[index]);
                                    else
                                       mPaintBackground.setColor(ansiColors[index]);
                                } else if ((values.get(0) >= 40) && (values.get(0) <= 49)) {
                                    int index = values.get(0) - 40;
                                    if (index >= ansiColors.length)
                                        Logging.fatal("Found ANSI color " + values.get(0) + " which is too large for ANSI m command");
                                    if (!mCursorReverse)
                                        mPaintBackground.setColor(ansiColors[index]);
                                    else
                                        mPaintText.setColor(ansiColors[index]);
                                } else {
                                    Logging.fatal("Unsupported color value " + values.get(0) + " for ANSI m command");
                                }
                                break;
                            default:
                                Logging.fatal("Unknown ANSI command " + current);
                        }
                        // The sequence was processed successfully!
                    } else {
                        // Invalid ANSI, throw an exception
                        Logging.fatal("Found start ESC but found " + current + " instead of left bracket");
                    }
                } else if (current == '\n') {
                    // Carriage return, so go to the next line
                    mCursorCol = 0;
                    mCursorRow++;
                } else {
                    // Regular character, just print it out using the current paint attributes
                    clearFixedChar(canvas, mCursorRow, mCursorCol);
                    drawFixedChar(canvas, current, mCursorRow, mCursorCol);

                    // Move the cursor to the next spot
                    mCursorCol++;
                    if (mCursorCol >= mTerminalWidth) {
                        mCursorCol = 0;
                        mCursorRow++;
                    }
                }

                // We processed this character successfully, so go to the next one now
                current = buffer.getByte();
            }
        }

        public int getCanvasWidth() { return mCanvasWidth; }
        public int getCanvasHeight() { return mCanvasHeight; }
    }


    public AnsiTerminalView (Context context) {
        super(context);

        // Callback to tell us when the surface is changed
        SurfaceHolder holder = getHolder();
        holder.addCallback(this);

        // Drawing is done on a separate thread, so create it here but wait until surfaceCreated()
        // before we actually start the drawing
        Logging.debug("Starting thread for RenderThread");
        mRenderThread = new RenderThread(holder, context);

        // Get key events
        setFocusable(true);
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        Logging.debug("surfaceChanged");
        mRenderThread.setSurfaceSize(width, height);

        // Create thread to run our native code implementation
        if (mNativeThread == null) {
            Logging.debug("Starting thread for nativeAnsiCode");
            mNativeThread = new Thread(new Runnable() {
                public void run() {
                    Logging.debug("Calling into JNI nativeAnsiCode()");
                    nativeAnsiCode();
                    Logging.fatal("nativeAnsiCode() has returned unexpectedly");
                }
            });
            mNativeThread.start();
        }
    }

    public void surfaceCreated(SurfaceHolder holder) {
        // Start the thread here so that we know the surface is ready before we start drawing
        Logging.debug("surfaceCreated");
        mRenderThread.start();
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        Logging.debug("surfaceDestroyed");
        Logging.fatal("surfaceDestroyed not implemented yet");
        // TODO: Stop the rendering thread
    }

    public void submitAnsiBuffer(int len, byte[] buffer) {
        // Native code will call this to submit a new buffer for processing in the queue
        mRenderThread.submitAnsiBuffer(len, buffer);
    }

    // Single element queue to store the last character in the virtual keyboard
    private LinkedBlockingQueue<Byte> keyBuffer = new LinkedBlockingQueue<Byte>();

    @Override
    public boolean onTouchEvent(MotionEvent ev) {
        if (ev.getAction() == MotionEvent.ACTION_DOWN) {
            // This is a touch event, so lets decide what to do with it. The 25% edges are defined
            // to map to the numeric keypad 4,8,6,2 keys, and the center maps to the space bar.
            int width = mRenderThread.getCanvasWidth();
            int height = mRenderThread.getCanvasHeight();
            byte result = 0;
            if ((ev.getX() < width*1/4) && (ev.getY() > height*1/4) && (ev.getY() < height*3/4))
                result = '4'; // Left
            else if ((ev.getX() > width*3/4) && (ev.getY() > height*1/4) && (ev.getY() < height*3/4))
                result = '6'; // Right
            else if ((ev.getY() < height*1/4) && (ev.getX() > width*1/4) && (ev.getX() < width*3/4))
                result = '8'; // Up
            else if ((ev.getY() > height*3/4) && (ev.getX() > width*1/4) && (ev.getX() < width*3/4))
                result = '2'; // Down
            else
                result = '\n'; // Middle is enter key
            Logging.debug ("Generating keystroke '" + String.valueOf((char)(result & 0xFF)) + "' from W,H=" + width + "," + height + " " + ev.toString());
            keyBuffer.clear();
            try {
                keyBuffer.put(result);
            } catch (InterruptedException e) {
                Logging.fatal ("Failed to store keystroke from " + e);
            }
        }
        return true; // We processed this event
    }

    public void blockUntilKeypress() {
        // Native code will call this, sleep until a key is available, but do not read it
        byte b = 0;
        try {
            // Sleep here
            b = keyBuffer.take().byteValue();
            // Put it back into the buffer
            keyBuffer.put(b);
        } catch (InterruptedException e) {
            Logging.fatal ("Failed to read keystroke from " + e);
        }
        Logging.debug("Found that key " + b + " is now available, it is ready in the buffer");
    }

    public boolean timeUntilKeypress(int microseconds) {
        Logging.debug("Waiting for " + microseconds + " usec in timeUntilKeypress");
        // Native code will call this, sleep until a key is available, but do not read it
        boolean ret = false;
        try {
            // Sleep here
            Byte result = keyBuffer.poll(microseconds, TimeUnit.MICROSECONDS);
            if (result != null) {
                keyBuffer.put(result); // Put it back
                ret = true;
            }
        } catch (InterruptedException e) {
            Logging.fatal ("Failed to read keystroke from " + e);
        }
        return ret;
    }


    public byte waitForKeypress() {
        // Native code will call this to block for a key press
        byte b = 0;
        try {
            b = keyBuffer.take().byteValue();
        } catch (InterruptedException e) {
            Logging.fatal ("Failed to read keystroke from " + e);
        }
        Logging.debug("Unblocking from character " + b + " in waitForKeypress");
        return b;
    }

    public byte waitForArrowpress() {
        // Native code will call this to block for a key press
        // Do not touch the input buffer if the result is not a numeric keypad direction
        byte b = 0;
        try {
            b = keyBuffer.take().byteValue();
            if ((b != '4') && (b != '8') && (b != '6') && (b != '2'))
            {
                // Not a direction key, so put the byte back
                keyBuffer.put(b);
                // Return back DIR_unknown which the native code knows how to interpret
                b = -1;
            }
        } catch (InterruptedException e) {
            Logging.fatal ("Failed to read keystroke from " + e);
        }
        Logging.debug("Unblocking from character " + b + " in waitForArrowpress");
        return b;
    }


    public byte pollForKeypress() {
        // Native code will call this to read a key but do not block if none available
        byte b = 0;
        Byte result = keyBuffer.poll();
        if (result != null)
            b = result.byteValue();
        Logging.debug("Polled with character " + b);
        return b;
    }

    public boolean checkForKeypress() {
        // Native code will call this to see if there is a key available
        return !keyBuffer.isEmpty();
    }

    public void clearForKeypress() {
        // Flush out all key presses
        keyBuffer.clear();
    }

    // This will be called within a new thread and can make calls to the three methods above
    public native void nativeAnsiCode();

    static {
        System.loadLibrary("native-jni");
    }
}
