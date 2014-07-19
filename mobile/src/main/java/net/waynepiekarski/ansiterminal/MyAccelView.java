package net.waynepiekarski.ansiterminal;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.view.View;

// Implement a View which has a thin height, and we just draw a strip of pixels as some percentage
// of the whole view width
public class MyAccelView extends View {

    private Paint mPaint;
    private double mValue = +0.5;
    private final static int mHeight = 5;
    public final static double mThreshold = 0.5;

    public MyAccelView(Context context) {
        super(context);
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

