package com.gds.starter;

import android.app.NativeActivity;
import android.os.Bundle;

/**
 * Created by czuber on 09/03/18.
 */

public class StarterActivity extends NativeActivity {

    static {
        System.loadLibrary("Starter");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        if (!isTaskRoot()) {
            finish();
            return;
        }

    }
}
