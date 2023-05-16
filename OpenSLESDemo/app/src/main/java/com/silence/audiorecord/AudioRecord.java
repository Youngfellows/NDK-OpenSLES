package com.silence.audiorecord;

import android.util.Log;

/**
 * Author silence.
 * Time：2019-09-27.
 * Desc：
 */
public class AudioRecord {


    public void startRecord(String recordFilePath) {
        if (_startRecord(recordFilePath)) {
            Log.d("tianzhao","正在录制");
        } else {
            Log.d("tianzhao","录制失败");
        }
    }

    public void stopRecord() {
        _stopRecord();
    }

    private static native boolean _startRecord(String filePath);

    private static native void _stopRecord();

}
