package net.waynepiekarski.ansiterminal;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.os.Bundle;
import android.view.Gravity;
import android.view.View;
import android.widget.FrameLayout;
import android.widget.ImageView;


public class MyActivityMobile extends Activity {

    static Bitmap mArrow = null;
    static Bitmap generateArrow (float angle) {
        if (mArrow == null) {
            // Generic arrow icon I can reuse everywhere needed here
            mArrow = Bitmap.createBitmap(128, 128, Bitmap.Config.ARGB_8888);
            Canvas canvas = new Canvas(mArrow);
            Paint paint = new Paint ();
            paint.setColor(Color.GRAY);
            paint.setStyle(Paint.Style.FILL);
            int w = mArrow.getWidth();
            int h = mArrow.getHeight();
            float triangle[] = {
                    0, 0,
                    w, h/2,
                    0, h
            };
            canvas.drawVertices(Canvas.VertexMode.TRIANGLES, triangle.length, triangle, 0, null, 0, null, 0, null, 0, 0, paint);
        }

        Matrix m = new Matrix();
        m.postRotate(angle);
        return Bitmap.createBitmap(mArrow, 0, 0, mArrow.getWidth(), mArrow.getHeight(), m, true);
    }

    void createArrowView(final AnsiTerminalView ansi, final byte key, double angle, int gravity, FrameLayout layout) {

        ImageView arrow = new ImageView(this);
        layout.addView(arrow, new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT, gravity));
        arrow.setImageBitmap(generateArrow((float)angle));
        arrow.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                ansi.injectKeyboardEvent(key);
            }
        });


    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        FrameLayout f = new FrameLayout(this);

        final AnsiTerminalView ansi = new AnsiTerminalView(this);
        f.addView(ansi);

        // Create arrow overlays to show the user how to control the UI
        createArrowView(ansi, (byte)'4', 180.0, Gravity.LEFT  | Gravity.CENTER_VERTICAL, f);
        createArrowView(ansi, (byte)'6',   0.0, Gravity.RIGHT | Gravity.CENTER_VERTICAL, f);
        createArrowView(ansi, (byte)'8', -90.0, Gravity.LEFT | Gravity.TOP, f);
        createArrowView(ansi, (byte)'2', +90.0, Gravity.LEFT | Gravity.BOTTOM, f);
        createArrowView(ansi, (byte)'8', -90.0, Gravity.RIGHT | Gravity.TOP, f);
        createArrowView(ansi, (byte)'2', +90.0, Gravity.RIGHT | Gravity.BOTTOM, f);

        setContentView(f);
    }
}
