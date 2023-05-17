package com.silence.openslesdemo;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.silence.audiorecord.AudioRecord;

import java.io.File;

public class MainActivity extends AppCompatActivity {
    private String TAG = this.getClass().getSimpleName();

    static {
        System.loadLibrary("audio-record");
    }

    AudioRecord record = new AudioRecord();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        checkPermission();
    }

    private String getPcmFilePath() {
        File dirFile = this.getExternalCacheDir();
        if (!dirFile.exists()) {
            dirFile.mkdirs();
        }
        File originalFile = new File(dirFile, "record.pcm");
        Log.i(TAG, "getPcmFilePath:: pcm file path=" + originalFile.getAbsolutePath());
        return originalFile.getAbsolutePath();
    }

    public void startRecord(View view) {
        //record.startRecord(getExternalFilesDir("pcm") + "/record.pcm");
        String pcmFilePath = getExternalFilesDir("pcm") + "/record.pcm";
        String pcmFilePath2 = getPcmFilePath();
        Log.i(TAG, "startRecord:: pcmFilePath=" + pcmFilePath);
        Log.i(TAG, "startRecord:: pcmFilePath2=" + pcmFilePath2);
        record.startRecord(pcmFilePath);
    }

    public void stopRecord(View view) {
        record.stopRecord();
    }

    private void checkPermission() {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED
                && ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED
                && ContextCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED
        ) {

        } else {
            requestPermissionAll();
        }
    }

    private void requestPermissionAll() {
        ActivityCompat.requestPermissions(this, new String[]{
                Manifest.permission.RECORD_AUDIO,
                Manifest.permission.READ_EXTERNAL_STORAGE,
                Manifest.permission.WRITE_EXTERNAL_STORAGE,

        }, 103);
    }
}
