// ---------------------------------------------------------------------
//
// Glider
//
// Copyright (C) 1996-2014 Wayne Piekarski
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

import android.app.Activity
import android.graphics.Bitmap
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Matrix
import android.graphics.Paint
import android.os.Bundle
import android.view.Gravity
import android.view.View
import android.widget.FrameLayout
import android.widget.ImageView
import android.widget.LinearLayout

import com.google.android.gms.common.ConnectionResult
import com.google.android.gms.common.api.GoogleApiClient
import com.google.android.gms.wearable.MessageApi
import com.google.android.gms.wearable.Node
import com.google.android.gms.wearable.NodeApi
import com.google.android.gms.wearable.Wearable


class MyActivityMobile : Activity() {

    internal fun createArrowView(ansi: AnsiTerminalView, key: Byte, angle: Double, gravity: Int, layout: FrameLayout): ImageView {
        val arrow = ImageView(this)
        layout.addView(arrow, FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT, gravity))
        arrow.setImageBitmap(generateArrow(angle.toFloat()))
        arrow.setOnClickListener { ansi.injectKeyboardEvent(key) }
        return arrow
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val f = FrameLayout(this)

        val ansi = AnsiTerminalView(this, null)
        f.addView(ansi)

        // Create arrow overlays to show the user how to control the UI
        createArrowView(ansi, '4'.toByte(), 180.0, Gravity.LEFT or Gravity.CENTER_VERTICAL, f)
        createArrowView(ansi, '6'.toByte(), 0.0, Gravity.RIGHT or Gravity.CENTER_VERTICAL, f)
        createArrowView(ansi, '8'.toByte(), -90.0, Gravity.LEFT or Gravity.TOP, f)
        createArrowView(ansi, '2'.toByte(), +90.0, Gravity.LEFT or Gravity.BOTTOM, f)
        createArrowView(ansi, '8'.toByte(), -90.0, Gravity.RIGHT or Gravity.TOP, f)
        val bottomRight = createArrowView(ansi, '2'.toByte(), +90.0, Gravity.RIGHT or Gravity.BOTTOM, f)

        setContentView(f)

        // Implement a button to launch the wear app
        val wear = ImageView(this)
        wear.setImageResource(R.drawable.wearable_icon)
        wear.setOnClickListener {
            // Must run this all on a background thread since it uses blocking calls for compactness
            Logging.debug("onClick for wearable start button")
            Thread(Runnable {
                Logging.debug("Connecting to Google Play Services to use MessageApi")
                val googleApiClient = GoogleApiClient.Builder(this@MyActivityMobile).addApi(Wearable.API).build()
                val result = googleApiClient.blockingConnect()
                if (result.isSuccess) {
                    Logging.debug("Searching for list of wearable clients")
                    val nodesResult = Wearable.NodeApi.getConnectedNodes(googleApiClient).await()
                    Logging.debug("Found " + nodesResult.nodes.size + " wearables clients to send to")
                    for (node in nodesResult.nodes) {
                        Logging.debug("Launching wearable client " + node.id + " via message")
                        val sendResult = Wearable.MessageApi.sendMessage(googleApiClient, node.id, "/start-on-wearable", ByteArray(0)).await()
                        if (sendResult.status.isSuccess) {
                            Logging.debug("Successfully sent to client " + node.id)
                        } else {
                            Logging.debug("Failed to send to client " + node.id + " with error " + sendResult)
                        }
                    }
                } else {
                    Logging.debug("Failed to connect to Google Play Services: " + result)
                }
            }).start()
        }
        f.removeView(bottomRight)
        val linear = LinearLayout(this)
        f.addView(linear, FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT, Gravity.RIGHT or Gravity.BOTTOM))
        linear.orientation = LinearLayout.VERTICAL
        linear.addView(wear)
        linear.addView(bottomRight)
    }

    companion object {

        internal var mArrow: Bitmap? = null
        internal fun generateArrow(angle: Float): Bitmap {
            if (mArrow == null) {
                // Generic arrow icon I can reuse everywhere needed here
                mArrow = Bitmap.createBitmap(128, 128, Bitmap.Config.ARGB_8888)
                val canvas = Canvas(mArrow!!)
                val paint = Paint()
                paint.color = Color.GRAY
                paint.style = Paint.Style.FILL
                val w = mArrow!!.width
                val h = mArrow!!.height
                val triangle = floatArrayOf(0f, 0f, w.toFloat(), (h / 2).toFloat(), 0f, h.toFloat())
                canvas.drawVertices(Canvas.VertexMode.TRIANGLES, triangle.size, triangle, 0, null, 0, null, 0, null, 0, 0, paint)
            }

            val m = Matrix()
            m.postRotate(angle)
            return Bitmap.createBitmap(mArrow!!, 0, 0, mArrow!!.width, mArrow!!.height, m, true)
        }
    }
}
