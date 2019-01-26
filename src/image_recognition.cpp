#include "include_files.h"

int main(int argc, char const *argv[])
{
	cv::Mat img, img1, img_rgb, img_thresholded, bgr;

	cv::namedWindow("Image");
	cv::namedWindow("threshold");

	img1 = cv::imread("images/what2.jpeg", cv::IMREAD_COLOR);
	cv::resize(img1, img_rgb, cv::Size(640,360));
	
	// hsv COLOUR SEGMENTATION OF THE OBJECT
	hsv_segmentation *seg = new hsv_segmentation(img_rgb);
	seg->get_hsv_threshold();

	// FINDING CONTOURS AFTER SEGMENTATION
	image_contours *purple = new image_contours();
	purple->get_contour(seg->purple_threshold, img_rgb);

	image_contours *green = new image_contours();
	green->get_contour(seg->green_threshold, img_rgb);

	image_contours *yellow = new image_contours();
	yellow->get_contour(seg->yellow_threshold, img_rgb);	

	// FINDING DELAUNAY TRIANGLES
	delaunay_triangles *purple_triangles = new delaunay_triangles(img_rgb);
	dino_object *purple_dino = new dino_object();

	delaunay_triangles *green_triangles = new delaunay_triangles(img_rgb);
	dino_object *green_dino = new dino_object();

	delaunay_triangles *yellow_triangles = new delaunay_triangles(img_rgb);
	dino_object *yellow_dino = new dino_object();


	// ALGORITHM FOR OBJECT DETECTION
	if(purple->contour.size()) {
		purple_triangles->find_outline(purple->contour, purple->contour_centroid);

		cv::Mat purple_img_temp = cv::Mat::zeros(cv::Size(img_rgb.cols,img_rgb.rows), CV_8UC1);
		for(uint i=0; i<purple_triangles->outline_points.size(); i++) {
			for(uint j=0; j<purple_triangles->outline_points.at(i).size(); j++) {
				purple_img_temp.at<uint8_t>(purple_triangles->outline_points.at(i).at(j).y , purple_triangles->outline_points.at(i).at(j).x) = 255;
			}
		}

		purple_triangles->get_delaunay_triangles(purple_img_temp, seg->purple_threshold);
		
		purple_dino->check_distance_centroid(purple->contour ,purple->contour_centroid, purple_triangles->triangle_centroids);	
	}

	if(green->contour.size()) {
		green_triangles->find_outline(green->contour, green->contour_centroid);		

		cv::Mat green_img_temp = cv::Mat::zeros(cv::Size(img_rgb.cols,img_rgb.rows), CV_8UC1);	
		for(uint i=0; i<green_triangles->outline_points.size(); i++) {
			for(uint j=0; j<green_triangles->outline_points.at(i).size(); j++) {
				green_img_temp.at<uint8_t>(green_triangles->outline_points.at(i).at(j).y , green_triangles->outline_points.at(i).at(j).x) = 255;
			}
		}
		green_triangles->get_delaunay_triangles(green_img_temp, seg->green_threshold);
		green_dino->check_distance_centroid(green->contour ,green->contour_centroid, green_triangles->triangle_centroids);
	}

	if(yellow->contour.size()) {
		yellow_triangles->find_outline(yellow->contour, yellow->contour_centroid);		

		cv::Mat yellow_img_temp = cv::Mat::zeros(cv::Size(img_rgb.cols,img_rgb.rows), CV_8UC1);	
		for(uint i=0; i<yellow_triangles->outline_points.size(); i++) {
			for(uint j=0; j<yellow_triangles->outline_points.at(i).size(); j++) {
				yellow_img_temp.at<uint8_t>(yellow_triangles->outline_points.at(i).at(j).y , yellow_triangles->outline_points.at(i).at(j).x) = 255;
			}
		}
		yellow_triangles->get_delaunay_triangles(yellow_img_temp, seg->yellow_threshold);
		yellow_dino->check_distance_centroid(yellow->contour ,yellow->contour_centroid, yellow_triangles->triangle_centroids);
	}


	for(uint i=0; i<purple_dino->detected_dino_contour.size(); i++) {
	
		cv::drawContours(img_rgb, purple->contour, purple_dino->detected_dino_contour.at(i), cv::Scalar(0,0,255), 2, cv::LINE_AA, purple->hierarchy, 0, cv::Point());
	}

	for(uint i=0; i<green_dino->detected_dino_contour.size(); i++) {
		cv::drawContours(img_rgb, green->contour, green_dino->detected_dino_contour.at(i), cv::Scalar(0,0,255), 2, cv::LINE_AA, green->hierarchy, 0, cv::Point());
	}

	for(uint i=0; i<yellow_dino->detected_dino_contour.size(); i++) {
		cv::drawContours(img_rgb, yellow->contour, yellow_dino->detected_dino_contour.at(i), cv::Scalar(0,0,255), 2, cv::LINE_AA, yellow->hierarchy, 0, cv::Point());
	}

	cout<<" #####################There are total "<<yellow_dino->detected_dino_contour.size()+purple_dino->detected_dino_contour.size()+green_dino->detected_dino_contour.size()<<" brontosaurs!!!!!"<<endl;

	cv::imshow("Image",img_rgb);
	
	cv::waitKey(0);
	delete(seg);
	delete(purple);
	delete(green);
	delete(yellow);
	delete(purple_triangles);
	delete(green_triangles);
	delete(yellow_triangles);
	delete(purple_dino);
	delete(yellow_dino);
	delete(green_dino);

	return 0;
}