package com.optimize.opencldemo;

import android.content.Context;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;

import androidx.appcompat.app.AppCompatActivity;

import java.io.ByteArrayOutputStream;
import java.io.Closeable;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = MainActivity.class.getSimpleName();

    static {
        System.loadLibrary("opencldemo");
    }

    private static void releaseResource(Closeable resource) {
        if (resource != null) {
            try {
                resource.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public static boolean copyCLFilesToAppDir(AssetManager am, String modelName, String destDir) {
        InputStream fileInput = null;
        FileOutputStream fileOutput = null;
        ByteArrayOutputStream byteOut = null;
        try {
            fileInput = am.open(modelName);
            String filename = destDir + modelName;

            fileOutput = new FileOutputStream(filename);
            byteOut = new ByteArrayOutputStream();
            byte[] buffer = new byte[1024];
            int len;
            while ((len = fileInput.read(buffer)) != -1) {
                byteOut.write(buffer, 0, len);
            }
            fileOutput.write(byteOut.toByteArray());
            return true;
        } catch (IOException exception) {
            return false;
        } finally {
            releaseResource(byteOut);
            releaseResource(fileOutput);
            releaseResource(fileInput);
        }
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        AssetManager assetManager = getAssets();
        File dir = getDir("clkernels", Context.MODE_PRIVATE);
        String path = dir.getAbsolutePath() + File.separator;
        Log.e(TAG, "OpenCL kernel file path: " + path);
        copyCLFilesToAppDir(assetManager, "opencl_add_kernel.h", path);
        copyCLFilesToAppDir(assetManager, "opencl_add_kernel.cl", path); // /data/user/0/com.optimize.opencldemo/app_clkernels/

        ReadPlatform();
        ReadDevices();
        CreateContext();
        CreateCommandQueue();
        CreateProgram();
        CreateKernel();
    }

    public native void ReadPlatform();
    public native void ReadDevices();
    public native void CreateContext();
    public native void CreateCommandQueue();
    public native void CreateProgram();
    public native void CreateKernel();
}