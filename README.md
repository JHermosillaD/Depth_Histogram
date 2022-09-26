# Depth map Histogram visualization :robot:

> OpenCV implementation to display the histogram of depth maps.

## About
Depth maps have a single channel and a 16-bit depth. Therefore, they can represent 2<sup>16</sup> different intensity values. Unlike the classic grayscale images that ranges from 0 to 255, i.e have a 8-bit depth. The intesity values can represent the distance in millimeters, centimeters, etc. And they can be recorded by a rgbd camera or similar devices. 

The program receives a depth map as the input and displays the histogram of the image.
<p float="left">
  <img src="results/src_img.png" width="200" />
  <img src="results/dpt_histo.png" width="455" /> 
</p>

Then it performs an histogram equalization, which is reflected in the change of the dynamic range of the image.
<p float="left">
  <img src="results/equal_img.png" width="200" />
  <img src="results/dpt_histo_equal.png" width="455" /> 
</p>

Finally, it performs a threshold filtering by the average to generate a binary image.
<p float="left">
  <img src="results/filter_img.png" width="200" />
</p>

Tested using OpenCV 4.2.0 and Ubuntu 20.04
