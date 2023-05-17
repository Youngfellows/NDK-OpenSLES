package com.tiffany.audiorecord.tester;

import android.content.Context;
import android.media.AudioFormat;
import android.media.MediaRecorder;
import android.os.Environment;
import android.util.Log;

import com.tiffany.audiorecord.api.audio.AudioCapturer;
import com.tiffany.audiorecord.api.wav.WavFileWriter;

import java.io.File;
import java.io.IOException;

public class AudioCaptureTester extends Tester implements AudioCapturer.OnAudioFrameCapturedListener {
    private String TAG = this.getClass().getSimpleName();
    private String DEFAULT_TEST_FILE = Environment.getExternalStorageDirectory() + "/test.wav";

    private AudioCapturer mAudioCapturer;
    private WavFileWriter mWavFileWriter;

    public AudioCaptureTester(Context context) {
        File dirFile = context.getExternalCacheDir();
        if (!dirFile.exists()) {
            dirFile.mkdirs();
        }
        File originalFile = new File(dirFile, "test.wav");
        DEFAULT_TEST_FILE = originalFile.getAbsolutePath();
        Log.i(TAG, "AudioCaptureTester:: DEFAULT_TEST_FILE=" + DEFAULT_TEST_FILE);
    }

    @Override
    public boolean startTesting() {
        mAudioCapturer = new AudioCapturer();
        mWavFileWriter = new WavFileWriter();
        try {

            mWavFileWriter.openFile(DEFAULT_TEST_FILE, 44100, 1, 16);
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
        mAudioCapturer.setOnAudioFrameCapturedListener(this);
        return mAudioCapturer.startCapture(MediaRecorder.AudioSource.MIC, 44100, AudioFormat.CHANNEL_IN_MONO,
                AudioFormat.ENCODING_PCM_16BIT);
    }

    @Override
    public boolean stopTesting() {
        mAudioCapturer.stopCapture();
        try {
            mWavFileWriter.closeFile();
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

    @Override
    public void onAudioFrameCaptured(byte[] audioData) {
        mWavFileWriter.writeData(audioData, 0, audioData.length);
    }
}
