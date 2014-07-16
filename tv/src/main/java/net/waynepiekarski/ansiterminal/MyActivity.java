package net.waynepiekarski.ansiterminal;

import android.app.Activity;
import android.os.Bundle;

public class MyActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        AnsiTerminalView ansi = new AnsiTerminalView (this);
        setContentView(ansi);
    }
}
