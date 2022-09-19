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
 
  for(int i = 0; i < img.rows; ++i) {
    ushort* pixel_val = img.ptr<ushort>(i);
    for (int j = 0; j < img.cols; ++j) 
      histogram[(int)pixel_val[j]]++;
  }

  return histogram;
}

vector<int> Equalizedfunction(Mat img, vector<int> histogram) {
  for(int i=1; i<65537; i++)
    histogram[i] = histogram[(int)(i-1)] + histogram[(int)i];

  int minCDF = histogram[0];
  for(int i=1; i<65537; i++) 
    if(minCDF > histogram[i])
      minCDF = histogram[i];
  
  int img_size = img.cols*img.rows;
  for(int i=0; i<65537; i++) 
    histogram[i] = (int)cvRound( ( (double)(histogram[i] - minCDF)/(double)(img_size - minCDF))*65536.0);

  return histogram;
}

Mat equalizedImage(Mat img, vector<int> equalFunct) {
  for(int i = 0; i < img.rows; ++i) {
    ushort* pixel_val = img.ptr<ushort>(i);
    for (int j = 0; j < img.cols; ++j) 
      pixel_val[j] = equalFunct[pixel_val[j]];
  }

  return img;
}

Mat drawHistogram(Mat img, vector<int> histogram) {
  double maxHist = *max_element(histogram.begin(), histogram.end());
  int hist_w = 512; int hist_h = 400;
  double bin_w = (double)hist_w/65537.0;
  Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0,0,0));

  for(int i = 0; i < 65537; i++)
    histogram[i] = ((double)histogram[i]/maxHist)*hist_h;
      
  for(int i = 0; i < 65537; i++)
    line(histImage, Point(bin_w*(double)i, hist_h), Point(bin_w*(double)i, hist_h-histogram[i]), Scalar(0,0,255), 1, 8, 0);

  return histImage;
}   
		  
int main(int argc, char** argv) {

  Mat img  = imread("/home/jhermosilla/GitHub/Depth_Histogram/img/square.png", IMREAD_ANYDEPTH);
  img.convertTo(img, CV_16U);
  /*  for(int i = 0; i < img.rows; ++i) {
    ushort* pixel_val = img.ptr<ushort>(i);
    for (int j = 0; j < img.cols; ++j) 
      cout << (int)pixel_val[j] << ",";
  }
  cout << endl;
  cout << "----------------------" << endl;*/

  vector<int> Histogram = makeHistogram(img);
  Mat imgHistogram = drawHistogram(img, Histogram);

  vector<int> equalFunction = Equalizedfunction(img, Histogram);
  for(int i=0; i<65537; i++)
    cout << equalFunction[i] << endl;
  
  Mat img_equal = equalizedImage(img, equalFunction);
  vector<int> Histogram_equal = makeHistogram(img_equal);
  Mat imgHistogram_equal = drawHistogram(img_equal, Histogram_equal);

  imshow("Source image", img);
  imshow("Depth Histogram", imgHistogram);
  imshow("Equalized image", img_equal);
  imshow("Equalized Depth Histogram", imgHistogram_equal);
  infoImage(img_equal);
  waitKey();
  return 0;
}
