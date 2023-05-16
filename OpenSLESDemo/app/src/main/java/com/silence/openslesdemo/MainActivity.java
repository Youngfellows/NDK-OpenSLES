package com.silence.openslesdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

import com.silence.audiorecord.AudioRecord;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("audio-record");
    }

    AudioRecord record = new AudioRecord();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }


    public void startRecord(View view) {
        record.startRecord(getExternalFilesDir("pcm") + "/record.pcm");
    }

    public void stopRecord(View view) {
        record.stopRecord();
    }
}
