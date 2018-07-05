#ifndef VIDEOCAP_H
#define VIDEOCAP_H

#include "common.h"

class VideoCap
{
public:
    VideoCap();

    Mat getNextFrame();

private:
    VideoCapture capture;

};

#endif // VIDEOCAP_H
