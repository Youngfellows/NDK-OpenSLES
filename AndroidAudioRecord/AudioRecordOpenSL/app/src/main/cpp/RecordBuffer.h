//
// Created by tck on 2019/7/17.
//

#ifndef AUDIORECORDOPENSL_RECORDBUFFER_H
#define AUDIORECORDOPENSL_RECORDBUFFER_H



class RecordBuffer {

public:
    short **buffer;
    int index = -1;

public:
    RecordBuffer(int buffersize);
    ~RecordBuffer();

    short *getRecordBuffer();

    short * getNowBuffer();


};



#endif //AUDIORECORDOPENSL_RECORDBUFFER_H
