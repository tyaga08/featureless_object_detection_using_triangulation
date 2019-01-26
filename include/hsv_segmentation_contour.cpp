/*
Purple:
H - 156 to 164
S - 29 to 82

Yellow:
H - 14 to 35
S - 102 to 223

Green:
H - 23 to 69
S - 44 to 84

Pink:
H - 164 to 173
S - 88 to 155

Blue:
H - 53 to 93
S - 87 to 164
*/


#include "header.h"


// DEFINING THE HSV RANGE OF THE COLORS
#define PURPLE_LOW_H 125
#define PURPLE_HIGH_H 170
#define PURPLE_LOW_S 39
#define PURPLE_HIGH_S 255

#define YELLOW_LOW_H 10
#define YELLOW_HIGH_H 77
#define YELLOW_LOW_S 131
#define YELLOW_HIGH_S 255

#define GREEN_LOW_H 40
#define GREEN_HIGH_H 98
#define GREEN_LOW_S 50
#define GREEN_HIGH_S 255

#define PINK_LOW_H 164
#define PINK_HIGH_H 173
#define PINK_LOW_S 88
#define PINK_HIGH_S 155

#define BLUE_LOW_H 53
#define BLUE_HIGH_H 98
#define BLUE_LOW_S 87
#define BLUE_HIGH_S 255

class hsv_segmentation {
	public:
		cv::Mat src_rgb, src_hsv, pink_threshold, purple_threshold, yellow_threshold, green_threshold, blue_threshold, erosion_dilation_temp, erode_kernel, dilate_kernel;
	
	hsv_segmentation(cv::Mat img) {
		src_rgb = img;
		cv::cvtColor(src_rgb, src_hsv, cv::COLOR_BGR2HSV);
		erode_kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5,5));
		dilate_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
	}

	~hsv_segmentation(){
	}

	void get_hsv_threshold();
	
};

// THRESHOLDING THE IMAGE
void hsv_segmentation::get_hsv_threshold() {
	cv::inRange(src_hsv, cv::Scalar(PURPLE_LOW_H, PURPLE_LOW_S, 0), cv::Scalar(PURPLE_HIGH_H, PURPLE_HIGH_S, 255), purple_threshold);
	/*cv::erode(purple_threshold, erosion_dilation_temp, erode_kernel);
	cv::dilate(erosion_dilation_temp, purple_threshold, dilate_kernel);*/
	
	cv::inRange(src_hsv, cv::Scalar(YELLOW_LOW_H, YELLOW_LOW_S, 0), cv::Scalar(YELLOW_HIGH_H, YELLOW_HIGH_S, 255), yellow_threshold);
	/*cv::erode(yellow_threshold, erosion_dilation_temp, erode_kernel);
	cv::dilate(erosion_dilation_temp, yellow_threshold, dilate_kernel);*/
	cv::inRange(src_hsv, cv::Scalar(GREEN_LOW_H, GREEN_LOW_S, 0), cv::Scalar(GREEN_HIGH_H, GREEN_HIGH_S, 255), green_threshold);
	/*cv::erode(green_threshold, erosion_dilation_temp, erode_kernel);
	cv::dilate(erosion_dilation_temp, green_threshold, dilate_kernel);*/

	cv::inRange(src_hsv, cv::Scalar(PINK_LOW_H, PINK_LOW_S, 0), cv::Scalar(PINK_HIGH_H, PINK_HIGH_S, 255), pink_threshold);
	/*cv::erode(pink_threshold, erosion_dilation_temp, erode_kernel);
	cv::dilate(erosion_dilation_temp, pink_threshold, dilate_kernel);*/
	cv::inRange(src_hsv, cv::Scalar(BLUE_LOW_H, BLUE_LOW_S, 0), cv::Scalar(BLUE_HIGH_H, BLUE_HIGH_S, 255), blue_threshold);
	/*cv::erode(blue_threshold, erosion_dilation_temp, erode_kernel);
	cv::dilate(erosion_dilation_temp, blue_threshold, dilate_kernel);*/
}



class image_contours {
	public:
		vector<vector<cv::Point> > contour;
		vector<cv::Vec4i> hierarchy;
		vector<cv::Point> contour_centroid;
		cv::Mat approx_poly;
		double contour_area;

	~image_contours(){
	}

	void get_contour(cv::Mat &threshold_img, cv::Mat &rgb_img);
	void get_centroid(cv::Mat &rgb_img);
	void approximation(cv::Mat &threshold_img);
};

// FINDING CONTOURS IN THE IMAGE
void image_contours::get_contour(cv::Mat &threshold_img, cv::Mat &rgb_img) {
	cv::findContours(threshold_img, contour, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE, cv::Point(0,0));
	

	for(uint i=0; i<contour.size(); i++) {
		contour_area = cv::contourArea(contour.at(i), false);
		if(contour_area < 100) {
			contour.erase(contour.begin()+i);
			i--;
		}
	}
	cout<<"contour.size(): -> "<<contour.size()<<endl;
	get_centroid(rgb_img);
	cout<<"contour_centroid.size(): ->"<<contour_centroid.size()<<endl;
}

// FINDING THE CENTROIDS OF EACH CONTOUR
void image_contours::get_centroid(cv::Mat &rgb_img) {
	for(uint i=0; i<contour.size(); i++) {
		int sumx = 0, sumy = 0;
		for(uint j=0; j<contour.at(i).size(); j++) {
			sumx+=contour.at(i).at(j).x;
			sumy+=contour.at(i).at(j).y;
		}
		contour_centroid.push_back(cv::Point(sumx/contour.at(i).size(), sumy/contour.at(i).size()));
	}
}

