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

import android.content.Context
import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import android.util.AttributeSet
import android.view.View

// Implement a View which has a thin height, and we just draw a strip of pixels as some percentage
// of the whole view width
class MyAccelView(context: Context, attrs: AttributeSet) : View(context, attrs) {

    private val mPaint: Paint = Paint()
    private var mValue = +0.5
    private val mHeight = 5

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        val parentWidth = View.MeasureSpec.getSize(widthMeasureSpec)
        val parentHeight = View.MeasureSpec.getSize(heightMeasureSpec)
        this.setMeasuredDimension(parentWidth, mHeight)
    }

    override fun onDraw(canvas: Canvas) {
        super.onDraw(canvas)
        // Change color depending on its value compared to the threshold for a keypress
        if (mValue < -mThreshold || mValue > mThreshold)
            mPaint.color = Color.WHITE
        else
            mPaint.color = Color.GRAY

        // Draw a bar from the center out to the left or right depending on the mValue
        if (mValue >= 0)
            canvas.drawRect((canvas.width / 2).toFloat(), 0f, (canvas.width / 2.0 + mValue * canvas.width / 2.0).toInt().toFloat(), (canvas.height - 1).toFloat(), mPaint)
        else
            // Cannot render right to left so need to flip around X arguments
            canvas.drawRect((canvas.width / 2.0 + mValue * canvas.width / 2.0).toInt().toFloat(), 0f, (canvas.width / 2).toFloat(), (canvas.height - 1).toFloat(), mPaint)
    }

    // Adjust the currently displayed info and schedule a refresh (ranges from -1..+1)
    fun setValue(`in`: Double) {
        mValue = `in`
        invalidate()
    }

    companion object {
        val mThreshold = 0.3
    }
}

