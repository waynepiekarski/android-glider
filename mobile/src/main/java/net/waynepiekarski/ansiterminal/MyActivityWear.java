package net.waynepiekarski.ansiterminal;

import android.app.Activity;
import android.os.Bundle;
import android.support.wearable.view.BoxInsetLayout;
import android.support.wearable.view.WatchViewStub;
import android.widget.RelativeLayout;
import android.widget.TextView;

public class MyActivityWear extends Activity {

    private TextView mTextView;

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

                AnsiTerminalView ansi = new AnsiTerminalView(MyActivityWear.this);
                rel.addView(ansi);
            }
        });
    }
}
