#include "handclassifier.h"

HandClassifier::HandClassifier()
{
    caffemodel_dir = "/home/ubuntu/lichenyang/caffe-for-cudnn-v2.5.48-master/ui_final/model/solver_iter_18522.caffemodel";
    prototxt_dir = "/home/ubuntu/lichenyang/caffe-for-cudnn-v2.5.48-master/ui_final/model/mobilenet_deploy.prototxt";
    meanfile_dir = "";
    scale_value = 0.017;
    mean_value.push_back(103.94);
    mean_value.push_back(116.78);
    mean_value.push_back(123.68);
    blob_name = "prob";
    device = "GPU";
    device_id = 0;
    im_height = 224;
    im_width = 224;

    if (strcmp(device.c_str(), "GPU") == 0) {
      cout << "Using GPU" << endl;
      CHECK_GE(device_id, 0);   // ensure device_id is larger than 0
      cout << "Using Device_id=" << device_id << endl;
      Caffe::SetDevice(device_id);
      Caffe::set_mode(Caffe::GPU);
    } else {
      cout << "Using CPU" << endl;
      Caffe::set_mode(Caffe::CPU);
    }

    net = new caffe::Net<float>(prototxt_dir, caffe::TEST);
    caffe::shared_ptr<caffe::Net<float> > hand_cls_net_tmp(net);
    hand_cls_net = hand_cls_net_tmp;
    hand_cls_net->CopyTrainedLayersFrom(caffemodel_dir);
}

HandClassifier::~HandClassifier()
{
    delete net;
}

void HandClassifier::MatVec2BlobVec(const vector<cv::Mat>& srcs, vector<Blob<float>* >& transformedBlobVec)
{
  // get size
  const int num = transformedBlobVec[0]->num();
  const int height = transformedBlobVec[0]->height();
  const int width  = transformedBlobVec[0]->width();
  const int channel  = transformedBlobVec[0]->channels();
  CHECK_EQ(num, srcs.size());

  float* transformed_data = transformedBlobVec[0]->mutable_cpu_data();
  for(int n=0; n<num; n++){
    // resize
    cv::Mat cv_img;
    cv::resize(srcs[n], cv_img, cv::Size(width, height));
    // transform into blob
    int num_offset = transformedBlobVec[0]->offset(n, 0, 0, 0);
    int top_index;
    for (int h = 0; h < height; ++h) {
      const uchar* ptr = cv_img.ptr<uchar>(h);
      int img_index = 0;
      for (int w = 0; w < width; ++w) {
        for (int c = 0; c < channel; ++c) {
            top_index = (c * height + h) * width + w;   // channel first
            float pixel = static_cast<float>(ptr[img_index++]);
//            transformed_data[num_offset+top_index] = pixel / float(255);
            transformed_data[num_offset+top_index] = (pixel - mean_value[c]) * scale_value;
        }
      }
    }
  }
}

int  HandClassifier::handClassify(cv::Mat InputImg)
{
    // define a input blob vector
    std::vector<Blob<float>*> input_vec;
    input_vec.push_back(new Blob<float>);
    vector<int> input_shape(4);
    input_shape[0] = 1;
    input_shape[1] = 3;
    input_shape[2] = im_height;
    input_shape[3] = im_width;
    input_vec[0]->Reshape(input_shape);

    // define a Mat vector
    vector<cv::Mat> src_vec;
    src_vec.push_back(InputImg);

    MatVec2BlobVec(src_vec, input_vec);

    hand_cls_net->Forward(input_vec);

    const caffe::shared_ptr<Blob<float> > score_blob = hand_cls_net->blob_by_name(blob_name);

    const float* score_blob_data = score_blob->cpu_data();

    return getLabel(score_blob_data, 4) ;

}

int HandClassifier::getLabel(const float* score, int gestNum)
{
    float score_max = 0;
    int idx = 0;
    for(int k = 0; k < gestNum; k++)
    {
        if (score[k]>score_max)
        {
            score_max = score[k];
            idx = k;
        }
    }
    return idx;
}
