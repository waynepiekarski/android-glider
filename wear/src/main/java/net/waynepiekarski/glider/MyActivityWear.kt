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
import android.content.Context
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager
import android.os.Bundle
import android.view.MotionEvent
import android.view.WindowManager
import android.widget.RelativeLayout
import kotlinx.android.synthetic.main.activity_my.*

class MyActivityWear : Activity() {

    private lateinit var mAccelView: MyAccelView
    private lateinit var mAnsiTerminalView: AnsiTerminalView
    private lateinit var mMainLayout: RelativeLayout

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_my)

        mAnsiTerminalView = findViewById(R.id.ansi_terminal) as AnsiTerminalView
        mAccelView = findViewById(R.id.accel_view) as MyAccelView
        mMainLayout = findViewById(R.id.main_layout) as RelativeLayout

        // Pass in if we detected round or not to the font resizing algorithm
        mAnsiTerminalView.surfaceRound(getResources().getConfiguration().isScreenRound())

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
    }
}
