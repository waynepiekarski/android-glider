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

package net.waynepiekarski.glider;

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
import android.widget.LinearLayout;
import android.widget.TextView;

import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.wearable.Wearable;


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

    ImageView createArrowView(final AnsiTerminalView ansi, final byte key, double angle, int gravity, FrameLayout layout) {
        ImageView arrow = new ImageView(this);
        layout.addView(arrow, new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT, gravity));
        arrow.setImageBitmap(generateArrow((float)angle));
        arrow.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                ansi.injectKeyboardEvent(key);
            }
        });
        return arrow;
    }

    private GoogleApiClient mGoogleApiClient;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        FrameLayout f = new FrameLayout(this);

        final AnsiTerminalView ansi = new AnsiTerminalView(this, null);
        f.addView(ansi);

        // Create arrow overlays to show the user how to control the UI
        createArrowView(ansi, (byte)'4', 180.0, Gravity.LEFT  | Gravity.CENTER_VERTICAL, f);
        createArrowView(ansi, (byte)'6',   0.0, Gravity.RIGHT | Gravity.CENTER_VERTICAL, f);
        createArrowView(ansi, (byte)'8', -90.0, Gravity.LEFT | Gravity.TOP, f);
        createArrowView(ansi, (byte)'2', +90.0, Gravity.LEFT | Gravity.BOTTOM, f);
        createArrowView(ansi, (byte)'8', -90.0, Gravity.RIGHT | Gravity.TOP, f);
        ImageView bottomRight =
        createArrowView(ansi, (byte)'2', +90.0, Gravity.RIGHT | Gravity.BOTTOM, f);

        setContentView(f);

        // Open up Google API to implement wearable messaging if needed
        mGoogleApiClient = new GoogleApiClient.Builder(this)
                .addApi(Wearable.API)
                .build();
        mGoogleApiClient.connect();

        // Implement a button to launch the wear app
        ImageView wear = new ImageView(this);
        wear.setImageResource(R.drawable.wearable_icon);
        wear.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Logging.debug("Launching wearable client via message");
                Wearable.MessageApi.sendMessage(mGoogleApiClient, "empty", "/start-glider-on-wearable", new byte[0]);
            }
        });
        f.removeView(bottomRight);
        LinearLayout linear = new LinearLayout (this);
        f.addView(linear, new FrameLayout.LayoutParams(FrameLayout.LayoutParams.WRAP_CONTENT, FrameLayout.LayoutParams.WRAP_CONTENT, Gravity.RIGHT | Gravity.BOTTOM));
        linear.setOrientation(LinearLayout.VERTICAL);
        linear.addView(wear);
        linear.addView(bottomRight);
    }
}
