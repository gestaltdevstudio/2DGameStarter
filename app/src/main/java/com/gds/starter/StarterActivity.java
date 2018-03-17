package com.gds.starter;

import android.app.NativeActivity;

/**
 * Created by czuber on 09/03/18.
 */

public class StarterActivity extends NativeActivity {

    static {
        System.loadLibrary("Starter");
    }

}
