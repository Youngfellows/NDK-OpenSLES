package com.speex.audiorecord;

import com.net168.audiorecord.OnAudioCaptureCallback;
import com.orhanobut.logger.Logger;

import java.nio.ByteBuffer;


/**
 * 在native层调用audiocapture.cpp
 */
public class SpeexAudioCapture implements SpeexAudioRecord {

    private String TAG = this.getClass().getSimpleName();

    static {
        System.loadLibrary("speex_audiorecord");
    }

    private native long _initRecord(int captureType, int sampleRate, int channelConfig, int audioFormat);

    private native void _stop(long instance);

    private native void _start(long instance);

    private native void _release(long instance);

    private native int _getState(long instance);

    private native void _setNativeCallback(long instance, boolean need);


    private long mNativeInstance = 0;   //native层 AudioCapture 实例的引用内存地址
    private OnAudioCaptureCallback mCallback;
    private byte[] mData;  //从native接收pcm数据缓存区


    public boolean init(int captureType, int sampleRate, int channelConfig, int audioFormat) {
        mNativeInstance = _initRecord(captureType, sampleRate, channelConfig, audioFormat);
        if (mNativeInstance == 0) {
            Logger.e("SpeexAudioCapture creat error");
        }
        return mNativeInstance != 0;
    }

    public void startRecording() {
        if (mNativeInstance == 0) {
            Logger.e("SpeexAudioCapture not exit");
            return;
        }
        _start(mNativeInstance);
    }


    public void stopRecording() {
        if (mNativeInstance == 0) {
            Logger.e("SpeexAudioCapture not exit");
            return;
        }
        _stop(mNativeInstance);
    }

    public void releaseRecording() {
        if (mNativeInstance == 0) {
            Logger.e("SpeexAudioCapture not exit");
            return;
        }
        _release(mNativeInstance);
        mNativeInstance = 0;
    }

    public int getState() {
        if (mNativeInstance == 0) {
            Logger.e("SpeexAudioCapture not exit");
            return STATE_UNINIT;
        }
        return _getState(mNativeInstance);
    }

    public void setAudioCaptureCallback(OnAudioCaptureCallback callback) {
        if (mNativeInstance == 0) {
            Logger.e("SpeexAudioCapture not exit");
            return;
        }
        mCallback = callback;
        _setNativeCallback(mNativeInstance, callback != null);
    }


    /**
     * 1.提供native层回调使用，传出nio接口的bytebuffer数据、
     * 2.再把数据回调给调用者
     */
    private void onNativeDataRead(ByteBuffer buffer) {
        if (mCallback != null) {
            //获取返回的size大小，一般来说是固定一个数
            int size = buffer.capacity();
            //如果数据缓存区未初始化或者空间不够存放则重新申请一个缓存空间
            if (mData == null || mData.length < size)
                mData = new byte[size];
            //从NIO获取PCM数据
            buffer.get(mData, 0, size);
            mCallback.onPCMDataAvailable(mData, size);
        }
    }
}
