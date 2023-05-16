package com.tck.audiorecordopensl;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";
    private TextView tvStartRecordAudio;
    private TextView tvStopRecordAudio;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        tvStartRecordAudio = (TextView) findViewById(R.id.tv_start_record_audio);
        tvStopRecordAudio = (TextView) findViewById(R.id.tv_stop_record_audio);


        tvStartRecordAudio.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                recordAudioWithPermission();
            }
        });
        tvStopRecordAudio.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                stopRecordAudio();
            }
        });
    }


    private void recordAudioWithPermission() {
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED
                && ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE) == PackageManager.PERMISSION_GRANTED
                && ContextCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO) == PackageManager.PERMISSION_GRANTED
        ) {
            recordAudio();
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

    private void recordAudio() {
        AudioRecordUtil.startRecord(Environment.getExternalStorageDirectory().getAbsolutePath() + "/wl_opensl_record.pcm");
    }

    private void stopRecordAudio() {
        AudioRecordUtil.stopRecord();
    }


}
