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
import android.content.Context
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager
import android.os.Bundle
import android.support.v4.view.GestureDetectorCompat
import android.support.wearable.view.DismissOverlayView
import android.view.GestureDetector
import android.view.MotionEvent
import android.view.WindowManager
import android.widget.RelativeLayout
import kotlinx.android.synthetic.main.activity_my.*

class MyActivityWear : Activity() {

    private lateinit var mAccelView: MyAccelView
    private lateinit var mAnsiTerminalView: AnsiTerminalView
    private lateinit var mDismissOverlayView: DismissOverlayView
    private lateinit var mMainLayout: RelativeLayout
    private lateinit var mGestureDetector: GestureDetectorCompat

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_my)

        watch_view_stub.setOnApplyWindowInsetsListener { _, windowInsets ->
            // Need to also call the original insets since we have overridden the original
            // https://developer.android.com/reference/android/view/View.OnApplyWindowInsetsListener.html
            watch_view_stub.onApplyWindowInsets(windowInsets)

            // Pass in if we detected round or not to the font resizing algorithm.
            // WatchViewStub seems to call onApplyWindowInsets() multiple times before
            // the layout is inflated, so make sure we check the reference is valid.
            // TODO: Should use ::mAnsiTerminalView.isInitialized() but the AS3.0 compiler fails
            if (mAnsiTerminalView != null)
                mAnsiTerminalView.surfaceRound(windowInsets.isRound)
            windowInsets
        }

        watch_view_stub.setOnLayoutInflatedListener {
            // Grab the views we need to reference later on. Since we have round and square
            // layouts we cannot use Kotlin's automatically generated view variables
            mAnsiTerminalView = findViewById(R.id.ansi_terminal) as AnsiTerminalView
            mAccelView = findViewById(R.id.accel_view) as MyAccelView
            mMainLayout = findViewById(R.id.main_layout) as RelativeLayout

            // Prevent display from sleeping
            window.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON)

            // Register for sensor updates now that the UI is created
            val sm = getSystemService(Context.SENSOR_SERVICE) as SensorManager
            val sensorType = Sensor.TYPE_ACCELEROMETER
            sm.registerListener(object : SensorEventListener {
                override fun onSensorChanged(sensorEvent: SensorEvent) {
                    if (sensorEvent.sensor.type == Sensor.TYPE_ACCELEROMETER) {
                        val xAccel = sensorEvent.values[0] / -10.0 // 9.8 m/s^2 is the maximum value
                        mAccelView.setValue(xAccel)

                        // Send keyboard events if the tilt is greater than a threshold
                        if (xAccel < -MyAccelView.mThreshold)
                            mAnsiTerminalView.injectKeyboardEvent('4'.toByte())
                        else if (xAccel > MyAccelView.mThreshold)
                            mAnsiTerminalView.injectKeyboardEvent('6'.toByte())
                    }
                }

                override fun onAccuracyChanged(sensor: Sensor, accuracy: Int) {
                    // Not needed
                }
            }, sm.getDefaultSensor(sensorType), SensorManager.SENSOR_DELAY_NORMAL)

            // Add a listener to handle closing the app on a long press to the activity
            mDismissOverlayView = DismissOverlayView(this@MyActivityWear)
            mMainLayout = findViewById(R.id.main_layout) as RelativeLayout
            mMainLayout.addView(mDismissOverlayView, RelativeLayout.LayoutParams(
                    RelativeLayout.LayoutParams.MATCH_PARENT,
                    RelativeLayout.LayoutParams.MATCH_PARENT))
            mGestureDetector = GestureDetectorCompat(this@MyActivityWear, object : GestureDetector.SimpleOnGestureListener() {
                override fun onLongPress(e: MotionEvent) {
                    Logging.debug("Detected long press, showing exit widget")
                    mDismissOverlayView.show()
                }
            })
        }
    }

    // Deliver touch events from the activity to the long press detector
    override fun dispatchTouchEvent(e: MotionEvent): Boolean {
        return mGestureDetector.onTouchEvent(e) || super.dispatchTouchEvent(e)
    }
}
