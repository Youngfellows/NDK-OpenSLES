package com.tck.audiorecordopensl;

/**
 * <p>description:</p>
 * <p>created on: 2019/7/17 10:44</p>
 *
 * @author tck
 * @version 1.0
 */
public class AudioRecordUtil {

    static {
        System.loadLibrary("native-lib");
    }


    public native static void startRecord(String path);

    public native static void stopRecord();


}
