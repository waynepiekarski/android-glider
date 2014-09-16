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
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.support.v4.view.GestureDetectorCompat;
import android.support.wearable.view.BoxInsetLayout;
import android.support.wearable.view.DismissOverlayView;
import android.support.wearable.view.WatchViewStub;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowInsets;
import android.view.WindowManager;
import android.widget.RelativeLayout;

public class MyActivityWear extends Activity {

    private MyAccelView mAccelView;
    private AnsiTerminalView mAnsiTerminalView;
    private DismissOverlayView mDismissOverlayView;
    private RelativeLayout mMainLayout;
    private GestureDetectorCompat mGestureDetector;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_my);

        final WatchViewStub stub = (WatchViewStub) findViewById(R.id.watch_view_stub);
        stub.setOnApplyWindowInsetsListener(new View.OnApplyWindowInsetsListener() {
            @Override
            public WindowInsets onApplyWindowInsets(View view, WindowInsets windowInsets) {
                // Need to also call the original insets since we have overridden the original
                // https://developer.android.com/reference/android/view/View.OnApplyWindowInsetsListener.html
                stub.onApplyWindowInsets(windowInsets);

                // Pass in if we detected round or not to the font resizing algorithm.
                // WatchViewStub seems to call onApplyWindowInsets() multiple times before
                // the layout is inflated, so make sure we check the reference is valid.
                if (mAnsiTerminalView != null)
                    mAnsiTerminalView.surfaceRound(windowInsets.isRound());
                return windowInsets;
            }
        });

        stub.setOnLayoutInflatedListener(new WatchViewStub.OnLayoutInflatedListener() {
            @Override
            public void onLayoutInflated(WatchViewStub stub) {
                // Grab the views we need to reference later on
                mAnsiTerminalView = (AnsiTerminalView)findViewById(R.id.ansi_terminal);
                mAccelView = (MyAccelView)findViewById(R.id.accel_view);
                mMainLayout = (RelativeLayout)findViewById(R.id.main_layout);

                // Prevent display from sleeping
                getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

                // Register for sensor updates now that the UI is created
                SensorManager sm = (SensorManager)getSystemService(Context.SENSOR_SERVICE);
                int sensorType = Sensor.TYPE_ACCELEROMETER;
                sm.registerListener(new SensorEventListener() {
                    public void onSensorChanged(SensorEvent sensorEvent) {
                        if (sensorEvent.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
                            double xAccel = sensorEvent.values[0] / -10.0; // 9.8 m/s^2 is the maximum value
                            mAccelView.setValue(xAccel);

                            // Send keyboard events if the tilt is greater than a threshold
                            if (xAccel < -mAccelView.mThreshold)
                                mAnsiTerminalView.injectKeyboardEvent((byte)'4');
                            else if (xAccel > mAccelView.mThreshold)
                                mAnsiTerminalView.injectKeyboardEvent((byte)'6');
                        }
                    }
                    public void onAccuracyChanged(Sensor sensor, int accuracy) {
                        // Not needed
                    }
                }, sm.getDefaultSensor(sensorType), SensorManager.SENSOR_DELAY_NORMAL);

                // Add a listener to handle closing the app on a long press to the activity
                mDismissOverlayView = new DismissOverlayView(MyActivityWear.this);
                mMainLayout = (RelativeLayout)findViewById(R.id.main_layout);
                mMainLayout.addView(mDismissOverlayView,new RelativeLayout.LayoutParams(
                        RelativeLayout.LayoutParams.MATCH_PARENT,
                        RelativeLayout.LayoutParams.MATCH_PARENT));
                mGestureDetector = new GestureDetectorCompat(MyActivityWear.this, new GestureDetector.SimpleOnGestureListener(){
                    @Override
                    public void onLongPress (MotionEvent e){
                        Logging.debug("Detected long press, showing exit widget");
                        mDismissOverlayView.show();
                    }
                });

            }
        });
    }

    // Deliver touch events from the activity to the long press detector
    @Override
    public boolean dispatchTouchEvent (MotionEvent e) {
        return mGestureDetector.onTouchEvent(e) || super.dispatchTouchEvent(e);
    }
}
