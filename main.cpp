#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat drawHistogram(Mat img) {
  int histsize = 255;
  int histogram[histsize+1];

  for(int i=0; i<histsize; i++)
    histogram[i] = 0;

  for(int y=0; y<img.rows; y++)
    for(int x=0; x<img.cols; x++)
      histogram[(int)img.at<uchar>(y,x)]++;

  int hist_w = 512;
  int hist_h = 400;  
  int bin_w = cvRound((double) hist_w/(histsize+1));
  int max = histogram[0];
  
  // Mat histImage(hist_h, hist_w, CV_16UC1, Scalar(0,0,0));
  Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
  
  for(int i=1; i<(histsize+1); i++)
    if(max < histogram[i])
      max = histogram[i];

  for(int i=0; i<histsize; i++)
    histogram[i] = ((double)histogram[i]/max)*histImage.rows;
  
  for(int i=0; i<histsize; i++)
  line(histImage, Point(bin_w*(i), hist_h), Point(bin_w*(i), hist_h - histogram[i]), Scalar(0,0,0), 1, 8, 0);
  
  //imshow("Source image", img);
  return histImage;
}

int main(int argc, char **argv) {
  Mat img, histogram;
  img = imread("/home/jhermosilla/Proyects backup/C-Party/OpenCV/Images/image.jpg", 0);
  if (!img.data) {
    printf("No image data \n");
    return -1;
  }

  histogram = drawHistogram(img);
  imshow("Output", histogram);
  waitKey(0);  
  return 0;
}
