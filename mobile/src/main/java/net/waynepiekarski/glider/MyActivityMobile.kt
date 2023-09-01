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


class MyActivityMobile : Activity() {

    internal fun createArrowView(ansi: AnsiTerminalView, key: Byte, angle: Double, gravity: Int, layout: FrameLayout): ImageView {
        val arrow = ImageView(this)
        layout.addView(arrow, FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT, gravity))
        arrow.setImageBitmap(generateArrow(angle.toFloat()))
        arrow.setOnClickListener { ansi.injectKeyboardEvent(key) }
        arrow.setFocusable(false)
        arrow.setFocusableInTouchMode(false)
        return arrow
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val f = FrameLayout(this)

        val ansi = AnsiTerminalView(this, null)
        // Android tablets with keyboards need to have focus prevented on all views otherwise it will try
        // to put a highlight on one of them. So grab focus here and prevent it everywhere else. I could
        // not get rid of the focus outline that occurs if you touch the screen beforehand.
        ansi.requestFocus()
        f.addView(ansi)

        // Create arrow overlays to show the user how to control the UI
        createArrowView(ansi, '4'.toByte(), 180.0, Gravity.LEFT or Gravity.CENTER_VERTICAL, f)
        createArrowView(ansi, '6'.toByte(), 0.0, Gravity.RIGHT or Gravity.CENTER_VERTICAL, f)
        createArrowView(ansi, '8'.toByte(), -90.0, Gravity.LEFT or Gravity.TOP, f)
        createArrowView(ansi, '2'.toByte(), +90.0, Gravity.LEFT or Gravity.BOTTOM, f)
        createArrowView(ansi, '8'.toByte(), -90.0, Gravity.RIGHT or Gravity.TOP, f)
        val bottomRight = createArrowView(ansi, '2'.toByte(), +90.0, Gravity.RIGHT or Gravity.BOTTOM, f)

        setContentView(f)

        f.removeView(bottomRight)
        val linear = LinearLayout(this)
        f.addView(linear, FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT, Gravity.RIGHT or Gravity.BOTTOM))
        linear.orientation = LinearLayout.VERTICAL
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
