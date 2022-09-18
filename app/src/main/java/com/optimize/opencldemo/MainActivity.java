package com.optimize.opencldemo;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("opencldemo");
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ReadPlatform();
        ReadDevices();
        CreateContext();
        CreateCommandQueue();
    }

    public native void ReadPlatform();
    public native void ReadDevices();
    public native void CreateContext();
    public native void CreateCommandQueue();
}