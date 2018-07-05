#ifndef HANDCLASSIFIER_H
#define HANDCLASSIFIER_H

#include "caffe/caffe.hpp"
#include "caffe/blob.hpp"
#include "caffe/common.hpp"
#include "caffe/net.hpp"
#include "caffe/util/io.hpp"

#include "common.h"

using namespace caffe;

class HandClassifier
{
private:
    string caffemodel_dir;
    string prototxt_dir;
    string meanfile_dir;
    string blob_name;
    string device;
    int device_id;
    int im_height;
    int im_width;
    float scale_value;
    vector<float> mean_value;
    Net<float>* net;
    caffe::shared_ptr<Net<float> > hand_cls_net; //there are std::shared_ptr and caffe::shared_ptr, so need to add namespace here
public:
    HandClassifier();
    ~HandClassifier();

    void MatVec2BlobVec(const vector<cv::Mat>&, vector<Blob<float>* >&);
    int handClassify(cv::Mat);
    int getLabel(const float*, int);
};

#endif // HANDCLASSIFIER_H
