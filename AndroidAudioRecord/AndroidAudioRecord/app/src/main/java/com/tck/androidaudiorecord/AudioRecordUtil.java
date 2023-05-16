package com.tck.androidaudiorecord;

import android.media.AudioFormat;
import android.media.AudioRecord;
import android.media.MediaRecorder;

/**
 * <p>description:</p>
 * <p>created on: 2019/7/17 10:03</p>
 *
 * @author tck
 * @version 1.0
 */
public class AudioRecordUtil {

    private AudioRecord audioRecord;
    private int bufferSizeInBytes;
    private boolean start = false;
    private int readSize = 0;

    private OnRecordListener onRecordListener;

    public AudioRecordUtil() {
        bufferSizeInBytes = AudioRecord.getMinBufferSize(
                44100,
                AudioFormat.CHANNEL_IN_STEREO,
                AudioFormat.ENCODING_PCM_16BIT);

        audioRecord = new AudioRecord(
                MediaRecorder.AudioSource.MIC,
                44100,
                AudioFormat.CHANNEL_IN_STEREO,
                AudioFormat.ENCODING_PCM_16BIT,
                bufferSizeInBytes
        );

    }

    public void startRecord() {
        new Thread() {
            @Override
            public void run() {
                start = true;
                audioRecord.startRecording();
                byte[] audioData = new byte[bufferSizeInBytes];
                while (start) {
                    readSize = audioRecord.read(audioData, 0, bufferSizeInBytes);
                    if (onRecordListener != null) {
                        onRecordListener.recordByte(audioData, readSize);
                    }
                }

                if (audioRecord != null) {
                    audioRecord.stop();
                    audioRecord.release();
                    audioRecord = null;
                }
            }
        }.start();
    }

    public void stopRecord() {
        start = false;
    }

    public boolean isStart() {
        return start;
    }

    public void setOnRecordListener(OnRecordListener onRecordListener) {
        this.onRecordListener = onRecordListener;
    }

    public interface OnRecordListener {
        void recordByte(byte[] audioData, int readSize);
    }
}
