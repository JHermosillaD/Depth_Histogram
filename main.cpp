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

  int hist_w = 255;
  int hist_h = 255;  
  int bin_w = cvRound((double) hist_w/(histsize+1));
  int max = histogram[0];

  Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0,0,0));
  
  for(int i=1; i<(histsize); i++)
    if(max < histogram[i])
      max = histogram[i];

  for(int i=0; i<histsize; i++)
    histogram[i] = ((double)histogram[i]/max)*hist_h;

  float sum = 0, mean;
  for(int k=0; k<histsize; k++)
    sum += histogram[k];

  mean = sum/(float)histsize;
  float treshold = mean + 0.60*mean;

  float var = 0;
  float sd = 0;
  for(int k=0; k<histsize; k++)
    var += (histogram[k]-mean)*(histogram[k]-mean);

  var = var/(histsize-1);
  sd = sqrt(var);
  cout << mean << ", " << var << ", " << sd << endl;
  for(int i = 0; i < histsize; i++)
    if(histogram[i] > (int(treshold)))
      line(histImage, Point(bin_w*(i), hist_h), Point(bin_w*(i), hist_h-histogram[i]), Scalar(0,0,255), 1, 8, 0);
  line(histImage, Point(bin_w, hist_h-cvRound(treshold)), Point(hist_w,hist_h-cvRound(treshold)), Scalar(255,0,0), 2, 8, 0);
  
  return histImage;
}

int main(int argc, char **argv) {
  Mat img, histogram;
  img = imread("/home/jhermosilla/Proyects backup/C-Party/OpenCV/Images/cover1.jpg");
  if (!img.data) {
    printf("No image data \n");
    return -1;
  }
  cvtColor(img,img, COLOR_BGR2GRAY);
  Mat dst;
  equalizeHist(img, dst);
  histogram = drawHistogram(dst);
  imshow("Output", histogram);
  //imshow("Equ", dst);
  waitKey(0);  
  return 0;
}
