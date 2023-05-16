package com.zxj.opensles;

import androidx.appcompat.app.AppCompatActivity;

import android.Manifest;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.tbruyelle.rxpermissions2.Permission;
import com.tbruyelle.rxpermissions2.RxPermissions;
import com.zxj.opensles.databinding.ActivityMainBinding;

import io.reactivex.functions.Consumer;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private static final String TAG = "MainActivity";

    private boolean mHasWriteStoragePermission = false;

    private ActivityMainBinding binding;
//    private String mPcmFile = "test.raw";
    private String mPcmFile = "audio.pcm";
    private String mPcmFilePrivatePath;
    private OpenSlESPlayer mOpenSlEsPlayer;
    private OpenSlESAudioRecorder mOpenSlEsRecorder;
    private Button mPauseBtn,mRecorderBtn,mPauseRecord;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        initView();

        requestPermission();
        mOpenSlEsPlayer = new OpenSlESPlayer();
        mOpenSlEsPlayer.init();
        mOpenSlEsRecorder = new OpenSlESAudioRecorder();

        mPcmFilePrivatePath = FileUtils.copyData(getApplicationContext(), mPcmFile, mPcmFile);
        Log.e(TAG, "filePath=" +  mPcmFilePrivatePath);

    }

    private void initView() {
        mPauseBtn = findViewById(R.id.pauseBtn);
        mRecorderBtn = findViewById(R.id.recorderBtn);
        mPauseRecord = findViewById(R.id.pauseRecord);
        mPauseBtn.setOnClickListener(this);
        mRecorderBtn.setOnClickListener(this);
        mPauseRecord.setOnClickListener(this);
    }

    private void requestPermission(){
        RxPermissions rxPermissions = new RxPermissions(this);

        rxPermissions.requestEach(
                Manifest.permission.WRITE_EXTERNAL_STORAGE,Manifest.permission.RECORD_AUDIO)
                .subscribe(new Consumer<Permission>() {
                    @Override
                    public void accept(Permission permission) throws Exception {
                        if (permission.granted) {
                            mHasWriteStoragePermission = true;
                            Log.d(TAG, "testRxPermission CallBack onPermissionsGranted() : " + permission.name +
                                    " request granted , to do something...");

                        } else if (permission.shouldShowRequestPermissionRationale) {
                            mHasWriteStoragePermission = false;
                            Log.d(TAG, "testRxPermission CallBack onPermissionsDenied() : " + permission.name + "request denied");
                            Toast.makeText(MainActivity.this, "拒绝权限，等待下次询问哦", Toast.LENGTH_SHORT).show();
                        } else {
                            mHasWriteStoragePermission = false;
                            Log.d(TAG, "testRxPermission CallBack onPermissionsDenied() : this " + permission.name + " is denied " +
                                    "and never ask again");
                            Toast.makeText(MainActivity.this, "拒绝权限，不再弹出询问框，请前往APP应用设置中打开此权限", Toast.LENGTH_SHORT).show();
                        }
                    }
                });

    }

    public void onOpenSLESPlayClick(View view){
        if(TextUtils.isEmpty(mPcmFilePrivatePath)){
            Toast.makeText(this,"路径不能为空!",Toast.LENGTH_SHORT).show();
//            mPcmFilePrivatePath = "/data/user/0/com.zxj.opensles/files/test.raw";
            return;
        }
        mPcmFilePrivatePath = getExternalFilesDir("audio").getAbsoluteFile()+"/audio.pcm";
        mOpenSlEsPlayer.setPcmData(mPcmFilePrivatePath);
    }

    public void onOpenSLESStopClick(View view){
        mOpenSlEsPlayer.stop();
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.pauseBtn:
                if("OpenSLES暂停".equals(mPauseBtn.getText().toString().trim())){
                    mPauseBtn.setText("OpenSLES继续");
                    mOpenSlEsPlayer.pause();
                }else {
                    mOpenSlEsPlayer.resume();
                    mPauseBtn.setText("OpenSLES暂停");
                }
                break;
            case R.id.recorderBtn:
                if("OpenSLES录音".equals(mRecorderBtn.getText().toString().trim())){
                    mRecorderBtn.setText("OpenSLES停止");
                    String path = getExternalFilesDir("audio").getAbsoluteFile()+"/audio.pcm";
                    mOpenSlEsRecorder.startRecord(path);
//                    mOpenSlEsRecorder.startRecord("/sdcard/audio.pcm");
//                    mOpenSlEsRecorder.startRecord(getFilesDir().getAbsolutePath() +"/audio.pcm");
                }else {
                    mRecorderBtn.setText("OpenSLES录音");
                    mOpenSlEsRecorder.stopRecord();
                }
                break;
            case R.id.pauseRecord:
                if("OpenSLES暂停录音".equals(mPauseRecord.getText().toString().trim())){
                    mPauseRecord.setText("OpenSLES继续录音");
                    mOpenSlEsRecorder.pauseRecord();
                }else {
                    mPauseRecord.setText("OpenSLES暂停录音");
                    mOpenSlEsRecorder.recordingRecord();
                }
                break;
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mOpenSlEsPlayer.release();
        mOpenSlEsRecorder.release();
    }


}