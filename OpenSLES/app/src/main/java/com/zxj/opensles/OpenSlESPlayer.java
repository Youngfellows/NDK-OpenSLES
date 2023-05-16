package com.zxj.opensles;

/**
 * @author zuo
 * @date 2022/5/30/030 16:18
 */
public class OpenSlESPlayer {
    static {
        System.loadLibrary("native-lib");
    }

    public native void init();
    public native void setPcmData(String path);
    public native void pause();
    public native void resume();
    public native void stop();
    public native void release();
}
