package com.zxj.opensles;

/**
 * @author zuo
 * @date 2022/5/30/030 16:18
 */
public class OpenSlESAudioRecorder {
    static {
        System.loadLibrary("native-lib");
    }

    public native void startRecord(String path);
    public native void pauseRecord();
    public native void recordingRecord();
    public native void stopRecord();
    public native void release();
}
