#include "header.h"

int lowH = 0, lowS = 0, lowV = 0;
int highH = 359, highS = 255, highV = 255;


// Call back function for the trackbar
static void lowH_thresh_trackbar(int, void *)
{
    lowH = min(highH-1, lowH);
    cv::setTrackbarPos("Low H", "Color trackbar", lowH);
}

static void highH_thresh_trackbar(int, void *)
{
    highH = max(highH, lowH+1);
    cv::setTrackbarPos("High H", "Color trackbar", highH);
}

static void lowS_thresh_trackbar(int, void *)
{
    lowS = min(highS-1, lowS);
    cv::setTrackbarPos("Low S", "Color trackbar", lowS);
}

static void highS_thresh_trackbar(int, void *)
{
    highS = max(highS, lowS+1);
    cv::setTrackbarPos("High S", "Color trackbar", highS);
}

static void lowV_thresh_trackbar(int, void *)
{
    lowV = min(highV-1, lowV);
    cv::setTrackbarPos("Low V", "Color trackbar", lowV);
}

static void highV_thresh_trackbar(int, void *)
{
    highV = max(highV, lowV+1);
    cv::setTrackbarPos("High V", "Color trackbar", highV);
}


cv::Mat trackbar_function(cv::Mat &img) {
	cv::Mat src_rgb, src_hsv, src_threshold;
	src_rgb = img;
	cv::namedWindow("Color trackbar");
	
	

		// create trackbar for thresholding values
		cv::createTrackbar("Low H", "Color trackbar", &lowH, 359, lowH_thresh_trackbar);
		cv::createTrackbar("High H", "Color trackbar", &highH, 359, highH_thresh_trackbar);

		cv::createTrackbar("Low S", "Color trackbar", &lowS, 255, lowS_thresh_trackbar);
		cv::createTrackbar("High S", "Color trackbar", &highS, 255, highS_thresh_trackbar);

		cv::createTrackbar("Low V", "Color trackbar", &lowV, 255, lowV_thresh_trackbar);
		cv::createTrackbar("High V", "Color trackbar", &highV, 255, highV_thresh_trackbar);
		
	while(1) {
		cv::cvtColor(src_rgb, src_hsv, cv::COLOR_BGR2HSV);	
		cv::inRange(src_hsv, cv::Scalar(lowH, lowS, lowV), cv::Scalar(highH, highS, highV), src_threshold);

		cv::imshow("Color trackbar",src_threshold);

		char key = (char) cv::waitKey(30);
        if (key == 'q' || key == 27) {
            break;
        }
	}

	return src_threshold;
}