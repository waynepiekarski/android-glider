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

package net.waynepiekarski.ansiterminal;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.View;

// Implement a View which has a thin height, and we just draw a strip of pixels as some percentage
// of the whole view width
public class MyAccelView extends View {

    private Paint mPaint;
    private double mValue = +0.5;
    private final static int mHeight = 5;
    public final static double mThreshold = 0.5;

    public MyAccelView(Context context, AttributeSet attrs) {
        super(context, attrs);
        mPaint = new Paint();
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec){
        int parentWidth = MeasureSpec.getSize(widthMeasureSpec);
        int parentHeight = MeasureSpec.getSize(heightMeasureSpec);
        this.setMeasuredDimension(parentWidth, mHeight);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        // Change color depending on its value compared to the threshold for a keypress
        if ((mValue < -mThreshold) || (mValue > mThreshold))
            mPaint.setColor(Color.WHITE);
        else
            mPaint.setColor(Color.GRAY);

        // Draw a bar from the center out to the left or right depending on the mValue
        if (mValue >= 0)
            canvas.drawRect(canvas.getWidth()/2, 0, (int)(canvas.getWidth()/2.0 + mValue*canvas.getWidth()/2.0), canvas.getHeight()-1, mPaint);
        else // Cannot render right to left so need to flip around X arguments
            canvas.drawRect((int)(canvas.getWidth()/2.0 + mValue*canvas.getWidth()/2.0), 0, canvas.getWidth()/2, canvas.getHeight()-1, mPaint);
    }

    // Adjust the currently displayed info and schedule a refresh (ranges from -1..+1)
    public void setValue(double in) {
        mValue = in;
        invalidate();
    }
}

