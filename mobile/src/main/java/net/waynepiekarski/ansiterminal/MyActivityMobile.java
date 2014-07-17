package net.waynepiekarski.ansiterminal;

import android.app.Activity;
import android.os.Bundle;


public class MyActivityMobile extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(new AnsiTerminalView(this));
    }
}
