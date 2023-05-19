package com.speex.audiorecord.activity;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.net168.audiorecord.AudioCapture;
import com.net168.audiorecord.OnAudioCaptureCallback;
import com.speex.audiorecord.R;
import com.speex.audiorecord.SpeexAudioCapture;
import com.speex.audiorecord.api.wav.WavFileWriter;
import com.speex.audiorecord.tester.AudioPlayerTester;
import com.speex.audiorecord.tester.Tester;

import java.io.File;
import java.io.IOException;

public class NativeAudioRecordActivity extends AppCompatActivity implements View.OnClickListener {

    private String TAG = this.getClass().getSimpleName();

    private static final String RECORD_AUDIO_WAV = "record.wav";
    private static final String OPENSLES_AUDIO_PCM = "opensles.pcm";

    private Button mButtonAudioRecordStart;
    private Button mButtonAudioRecordStop;
    private Button mButtonOpenSLESStart;
    private Button mButtonOpenSLESStop;

    private SpeexAudioCapture mAudioCapture;//AudioRecord录音
    private WavFileWriter mWavFileWriter;
    private Tester mTester;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_audio_record);
        checkPermission();
        mButtonAudioRecordStart = findViewById(R.id.btn_audio_record_start);
        mButtonAudioRecordStop = findViewById(R.id.btn_audio_record_stop);
        mButtonOpenSLESStart = findViewById(R.id.btn_opensles_start);
        mButtonOpenSLESStop = findViewById(R.id.btn_opensles_stop);

        mButtonAudioRecordStart.setOnClickListener(this);
        mButtonAudioRecordStop.setOnClickListener(this);
        mButtonOpenSLESStart.setOnClickListener(this);
        mButtonAudioRecordStart.setOnClickListener(this);
        mButtonOpenSLESStop.setOnClickListener(this);
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

    private File getPcmFilePath(String child) {
        File dirFile = this.getExternalCacheDir();
        if (!dirFile.exists()) {
            dirFile.mkdirs();
        }
        File pcmFile = new File(dirFile, child);
        Log.i(TAG, "getPcmFilePath:: pcm file path=" + pcmFile.getAbsolutePath());
        return pcmFile;
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btn_audio_record_start:
                audioRecordStart();
                break;
            case R.id.btn_audio_record_stop:
                audioRecordStop();
                break;
            case R.id.btn_opensles_start:
                openslesStart();
                break;
            case R.id.btn_opensles_stop:
                openslesStop();
                break;
            default:
                break;
        }
    }

    private void audioRecordStart() {
        Log.i(TAG, "audioRecordStart:: audio_record_start");
        File file = getPcmFilePath(RECORD_AUDIO_WAV);
        if (file.exists()) {
            file.delete();
            Log.i(TAG, "audioRecordStart:: delete file," + file.getAbsolutePath());
        }
        mWavFileWriter = new WavFileWriter();
        try {

            mWavFileWriter.openFile(file.getAbsolutePath(), AudioCapture.AUDIO_SAMPLE_RATE_44_1, 1, 16);
        } catch (IOException e) {
            e.printStackTrace();
        }
        // 开启录音
        mAudioCapture = new SpeexAudioCapture();
        mAudioCapture.init(AudioCapture.AUDIO_CAPTURE_TYPE_AUDIORECORD, AudioCapture.AUDIO_SAMPLE_RATE_44_1,
                AudioCapture.AUDIO_CHANNEL_MONO, AudioCapture.AUDIO_FORMAT_PCM_16BIT);
        if (mAudioCapture.getState() == AudioCapture.STATE_IDLE) {
            mAudioCapture.setAudioCaptureCallback(new MyOnAudioCaptureCallback());
            mAudioCapture.startRecording();
        } else {
            mAudioCapture.releaseRecording();
        }
    }

    private void audioRecordStop() {
        Log.i(TAG, "audioRecordStop:: audio_record_stop");
        if (mAudioCapture != null) {
            //关闭录音
            mAudioCapture.stopRecording();
            mAudioCapture.releaseRecording();
        }

        if (mWavFileWriter != null) {
            try {
                mWavFileWriter.closeFile();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private void openslesStart() {
        Log.i(TAG, "openslesStart:: opensles_start");
        File file = getPcmFilePath(OPENSLES_AUDIO_PCM);
        if (file.exists()) {
            file.delete();
            Log.i(TAG, "openslesStart:: delete file," + file.getAbsolutePath());
        }
        mWavFileWriter = new WavFileWriter();
        try {

            mWavFileWriter.openFile(file.getAbsolutePath(), AudioCapture.AUDIO_SAMPLE_RATE_44_1, 1, 16);
        } catch (IOException e) {
            e.printStackTrace();
        }
        // 开启录音
        mAudioCapture = new SpeexAudioCapture();
        mAudioCapture.init(AudioCapture.AUDIO_CAPTURE_TYPE_OPENSLES, AudioCapture.AUDIO_SAMPLE_RATE_44_1,
                AudioCapture.AUDIO_CHANNEL_MONO, AudioCapture.AUDIO_FORMAT_PCM_16BIT);
        if (mAudioCapture.getState() == AudioCapture.STATE_IDLE) {
            mAudioCapture.setAudioCaptureCallback(new MyOnAudioCaptureCallback());
            mAudioCapture.startRecording();
        } else {
            mAudioCapture.releaseRecording();
        }
    }

    private void openslesStop() {
        Log.i(TAG, "onClick:: opensles_stop");
        if (mAudioCapture != null) {
            //关闭录音
            mAudioCapture.stopRecording();
            mAudioCapture.releaseRecording();
        }
        if (mWavFileWriter != null) {
            try {
                mWavFileWriter.closeFile();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void onPointerCaptureChanged(boolean hasCapture) {

    }

    /**
     * PCM数据回调
     */
    private class MyOnAudioCaptureCallback implements OnAudioCaptureCallback {
        @Override
        public void onPCMDataAvailable(byte[] data, int size) {
            //Log.i(TAG, "onPCMDataAvailable:: size:" + size);
            mWavFileWriter.writeData(data, 0, size);
        }
    }

    public void onWavPlay(View view) {
        File file = getPcmFilePath(RECORD_AUDIO_WAV);
        mTester = new AudioPlayerTester(file);
        if (mTester != null) {
            mTester.startTesting();
            Toast.makeText(this, "Start play wav!", Toast.LENGTH_SHORT).show();
        }
    }

    public void onOpenSLESPlay(View view) {
        File file = getPcmFilePath(OPENSLES_AUDIO_PCM);
        mTester = new AudioPlayerTester(file);
        if (mTester != null) {
            mTester.startTesting();
            Toast.makeText(this, "Start play wav!", Toast.LENGTH_SHORT).show();
        }
    }

    public void onStopPlay(View v) {
        if (mTester != null) {
            mTester.stopTesting();
            Toast.makeText(this, "Stop Testing !", Toast.LENGTH_SHORT).show();
        }
    }
}