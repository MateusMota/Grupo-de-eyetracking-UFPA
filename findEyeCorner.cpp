#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <queue>
#include <stdio.h>

#include "constants.h"
#include "helpers.h"

#include "findEyeCorner.h"

using namespace std;
using namespace cv;

Mat *leftCornerKernel;
Mat *rightCornerKernel;

// not constant because stupid opencv type signatures
float kEyeCornerKernel[4][6] = {
  {-1,-1,-1, 1, 1, 1},
  {-1,-1,-1,-1, 1, 1},
  {-1,-1,-1,-1, 0, 3},
  { 1, 1, 1, 1, 1, 1},
};

void createCornerKernels() {
  rightCornerKernel = new Mat(4,6,CV_32F,kEyeCornerKernel);
  leftCornerKernel = new Mat(4,6,CV_32F);
  // flip horizontally
  flip(*rightCornerKernel, *leftCornerKernel, 1);
}

void releaseCornerKernels() {
  delete leftCornerKernel;
  delete rightCornerKernel;
}

// TODO implement these
Mat eyeCornerMap(const Mat &region, bool left, bool left2) {
  Mat cornerMap;

  Size sizeRegion = region.size();
  Range colRange(sizeRegion.width / 4, sizeRegion.width * 3 / 4);
  Range rowRange(sizeRegion.height / 4, sizeRegion.height * 3 / 4);

  Mat miRegion(region, rowRange, colRange);

  filter2D(miRegion, cornerMap, CV_32F,
               (left && !left2) || (!left && !left2) ? *leftCornerKernel : *rightCornerKernel);

  return cornerMap;
}

Point2f findEyeCorner(Mat region, bool left, bool left2) {
  Mat cornerMap = eyeCornerMap(region, left, left2);

  Point maxP;
  minMaxLoc(cornerMap,NULL,NULL,NULL,&maxP);

  Point2f maxP2;
  maxP2 = findSubpixelEyeCorner(cornerMap, maxP);


  return maxP2;
}
Point2f findSubpixelEyeCorner(Mat region, Point maxP) {

  Size sizeRegion = region.size();

  // Matrix dichotomy
  // Not useful, matrix becomes too small

  /*int offsetX = 0;
  if(maxP.x - sizeRegion.width / 4 <= 0) {
    offsetX = 0;
  } else if(maxP.x + sizeRegion.width / 4 >= sizeRegion.width) {
    offsetX = sizeRegion.width / 2 - 1;
  } else {
    offsetX = maxP.x - sizeRegion.width / 4;
  }
  int offsetY = 0;
  if(maxP.y - sizeRegion.height / 4 <= 0) {
    offsetY = 0;
  } else if(maxP.y + sizeRegion.height / 4 >= sizeRegion.height) {
    offsetY = sizeRegion.height / 2 - 1;
  } else {
    offsetY = maxP.y - sizeRegion.height / 4;
  }
  cv::Range colRange(offsetX, offsetX + sizeRegion.width / 2);
  cv::Range rowRange(offsetY, offsetY + sizeRegion.height / 2);

  cv::Mat miRegion(region, rowRange, colRange);


if(left){
    imshow("aa",miRegion);
  } else {
    imshow("aaa",miRegion);
  }*/

  Mat cornerMap(sizeRegion.height * 10, sizeRegion.width * 10, CV_32F);

  resize(region, cornerMap, cornerMap.size(), 0, 0, INTER_CUBIC);

  Point maxP2;
  minMaxLoc(cornerMap, NULL,NULL,NULL,&maxP2);

  return Point2f(sizeRegion.width / 2 + maxP2.x / 10,
                     sizeRegion.height / 2 + maxP2.y / 10);
}

