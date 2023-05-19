package com.speex.audiorecord.activity;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.speex.audiorecord.R;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void onClickJava(View view) {
        Intent intent = new Intent(this, JavaAudioRecordActivity.class);
        startActivity(intent);
    }

    public void onClickNative(View view) {
        Intent intent = new Intent(this, NativeAudioRecordActivity.class);
        startActivity(intent);
    }
}