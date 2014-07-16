package net.waynepiekarski.ansiterminal;

import com.google.android.glass.app.Card;
import com.google.android.glass.media.Sounds;
import com.google.android.glass.widget.CardScrollAdapter;
import com.google.android.glass.widget.CardScrollView;

import android.app.Activity;
import android.content.Context;
import android.media.AudioManager;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;

/**
 * If your Glassware intends to intercept swipe gestures, you should set the content view directly
 * and use a {@link com.google.android.glass.touchpad.GestureDetector}.
 * @see <a href="https://developers.google.com/glass/develop/gdk/touch">GDK Developer Guide</a>
 */
public class MyActivityGlass extends Activity {

    private View mView;

    @Override
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);

        // Create the Glider game object here
        mView = new AnsiTerminalView(this);
        setContentView(mView);
    }

    @Override
    protected void onResume() {
        super.onResume();
        // mCardScroller.activate();
    }

    @Override
    protected void onPause() {
        // mCardScroller.deactivate();
        super.onPause();
    }
}
