package net.waynepiekarski.ansiterminal;

import android.content.Intent;

import com.google.android.gms.wearable.MessageEvent;
import com.google.android.gms.wearable.WearableListenerService;

public class MyActivityWearStartService extends WearableListenerService {

    @Override
    public void onMessageReceived(MessageEvent messageEvent) {
        if (messageEvent.getPath().equals("/start-glider-on-wearable")) {
            Logging.debug("Received message to start Glider game on wearable");
            Intent startIntent = new Intent(this, MyActivityWear.class);
            startIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            startActivity(startIntent);
        }
    }
}
