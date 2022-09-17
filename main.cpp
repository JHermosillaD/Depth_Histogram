#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

struct Statistics {
  vector<int> histogram;
  
  double mean() {
    double sum = 0, mean;
    for(int k=0; k<65536; k++)
      sum += (double)histogram[k];
    mean = sum/65536.0;
    return mean;
  }

  double var(double mean) {
    double var = 0;
    for(int k=0; k<65536; k++)
      var += ((double)histogram[k] - mean)*((double)histogram[k] - mean);
    var = var/(65536.0-1.0);
    return var;
  }

  double sd(double var) {
    double sd = sqrt(var);
    return sd;
  }  

  void printStats(double mean, double var, double sd) {
    cout << "-----------------------------" << endl;
    cout << "mean:                 " << mean << endl;
    cout << "variance:             " << var << endl;
    cout << "standard deviation:   " << sd << endl;
    cout << "-----------------------------" << endl;
  }
};

void infoImage(Mat img) {
  cout << "image width: " << img.cols << '\n'
       << "image height: " << img.rows << '\n'
       << "image size: " << (int)img.size().width*(int)img.size().height << '\n'
       << "image depth: " << img.depth() << '\n'
       << "image channels " << img.channels() << '\n'
       << "image type: " << img.type() << '\n' << endl;
}

vector<int> makeHistogram(Mat img) {
  vector<int> histogram;
  for(int i=0; i<65537; i++) 
    histogram.push_back(0);

  for (int i = 0; i < img.rows; ++i) {
    ushort* pixel_val = img.ptr<ushort>(i);
    for (int j = 0; j < img.cols; ++j) 
      histogram[(int)pixel_val[j]]++;
  }

  return histogram;
}

Mat drawHistogram(Mat img, vector<int> histogram) {
  int maxHist = *max_element(histogram.begin(), histogram.end());
  int hist_w = 512; int hist_h = 400;
  int bin_w = cvRound((double) hist_w/256);

  for(int i = 0; i < 65536; i++)
    histogram[i] = (int)(((double) histogram[i]/maxHist)*hist_h);
  
  Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

  for(int i = 0; i < 65536; i++)
    line(histImage, Point(bin_w*(i*256)/65536.0, hist_h), Point(bin_w*(i*256)/65536.0, hist_h-histogram[i]), Scalar(0, 0, 255), 2, 8, 0);

  return histImage;
}   
		  
int main(int argc, char** argv) {

  Mat img  = imread("/home/jhermosilla/GitHub/Depth_Histogram/img/person.pgm", IMREAD_ANYDEPTH);
  img.convertTo(img, CV_16U);

  vector<int> vectHistogram = makeHistogram(img);
  Mat imgHistogram = drawHistogram(img, vectHistogram);
  double mean, var, sd;
  Statistics statsHistogram;
  statsHistogram.histogram = vectHistogram;
  mean = statsHistogram.mean();
  var = statsHistogram.var(mean);
  sd = statsHistogram.sd(var);
  statsHistogram.printStats(mean, var, sd);

  imshow("Source image", img);
  imshow("Depth Histogram", imgHistogram);
  infoImage(img);
  waitKey();
  return 0;
}
