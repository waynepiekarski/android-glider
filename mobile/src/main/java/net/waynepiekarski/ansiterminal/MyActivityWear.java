package net.waynepiekarski.ansiterminal;

import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.support.wearable.view.BoxInsetLayout;
import android.support.wearable.view.WatchViewStub;
import android.view.WindowManager;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class MyActivityWear extends Activity {

    private TextView mTextView;
    private MyAccelView mAccelView;
    private AnsiTerminalView mAnsiTerminalView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_my);

        final WatchViewStub stub = (WatchViewStub) findViewById(R.id.watch_view_stub);
        stub.setOnLayoutInflatedListener(new WatchViewStub.OnLayoutInflatedListener() {
            @Override
            public void onLayoutInflated(WatchViewStub stub) {
                mTextView = (TextView) stub.findViewById(R.id.text);

                // Grab the layout object to embed later things in code
                RelativeLayout rel = (RelativeLayout)findViewById(R.id.top_layout);

                mAnsiTerminalView = new AnsiTerminalView(MyActivityWear.this);
                rel.addView(mAnsiTerminalView);

                mAccelView = new MyAccelView(MyActivityWear.this);
                rel.addView(mAccelView);

                // Prevent display from sleeping
                getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
            }
        });

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
    }
}
