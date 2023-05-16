package com.jhuster.audiodemo.tester;

import android.content.Context;
import android.os.Environment;
import android.util.Log;

import com.jhuster.audiodemo.api.audio.AudioPlayer;
import com.jhuster.audiodemo.api.wav.WavFileReader;

import java.io.File;
import java.io.IOException;

public class AudioPlayerTester extends Tester {
    private String TAG = this.getClass().getSimpleName();
    private String DEFAULT_TEST_FILE = Environment.getExternalStorageDirectory() + "/test.wav";

    private static final int SAMPLES_PER_FRAME = 1024;

    private AudioPlayer mAudioPlayer;
    private WavFileReader mWavFileReader;
    private volatile boolean mIsTestingExit = false;

    public AudioPlayerTester(Context context) {
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
        mWavFileReader = new WavFileReader();
        mAudioPlayer = new AudioPlayer();

        try {
            mWavFileReader.openFile(DEFAULT_TEST_FILE);
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }

        mAudioPlayer.startPlayer();

        new Thread(AudioPlayRunnable).start();

        return true;
    }

    @Override
    public boolean stopTesting() {
        mIsTestingExit = true;
        return true;
    }

    private Runnable AudioPlayRunnable = new Runnable() {
        @Override
        public void run() {
            byte[] buffer = new byte[SAMPLES_PER_FRAME * 2];
            while (!mIsTestingExit && mWavFileReader.readData(buffer, 0, buffer.length) > 0) {
                mAudioPlayer.play(buffer, 0, buffer.length);
            }
            mAudioPlayer.stopPlayer();
            try {
                mWavFileReader.closeFile();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    };
}
