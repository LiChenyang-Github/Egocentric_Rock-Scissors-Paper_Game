#include "videocap.h"

VideoCap::VideoCap()
{
    capture.open(0);
}

Mat VideoCap::getNextFrame()
{
    Mat frame;
    capture>>frame;
    return frame;
}
