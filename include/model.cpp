#include "header.h"

class dino_model /*: public hsv_segmentation, public image_contours, public delaunay_triangles*/ {

public:
	// vector<cv::Point2f> quad14, quad23, quad12, quad34;
	vector<float> slope;
	vector<vector<double> > y_point_distance, x_point_distance;
	double largest_distance_centroid=0;
	double model_x_avg, model_y_avg;

	float base_angle;
	cv::Point2f farthest_point;

void largest_distance(vector<cv::Point> &contour, cv::Point &contour_centroid);
void check_distance_centroid(vector<vector<cv::Point> > &contour, vector<cv::Point> &contour_centroid, vector<vector<cv::Point2f> > &triangle_centroid);
void find_base_angle(cv::Point &contour_centroid);

};


void dino_model::check_distance_centroid(vector<vector<cv::Point> > &contour, vector<cv::Point> &contour_centroid, vector<vector<cv::Point2f> > &triangle_centroid) {
	
	y_point_distance.resize(triangle_centroid.size());
	x_point_distance.resize(triangle_centroid.size());
	for(uint i=0; i<contour.size(); i++) {
		
		largest_distance(contour.at(i), contour_centroid.at(i));
		find_base_angle(contour_centroid.at(i));
		
		double sum=0;

		for(uint j=0; j<triangle_centroid.at(i).size(); j++) {
			y_point_distance.at(i).push_back(abs(triangle_centroid.at(i).at(j).y - contour_centroid.at(i).y));
			sum+=y_point_distance.at(i).at(j);
		}
		
		double area = cv::contourArea(contour.at(i), false);
		model_y_avg = sum/(largest_distance_centroid*triangle_centroid.at(i).size());

		cout<<"contourArea: -> "<<area<<endl;
		cout<<"largest_distance: -> "<<largest_distance_centroid<<endl;
		cout<<"distance model wrt y: -> "<<model_y_avg<<endl;
		cout<<"area model wrt y: -> "<<sum/(area*triangle_centroid.at(i).size())<<endl;

		sum=0;

		for(uint j=0; j<triangle_centroid.at(i).size(); j++) {
			x_point_distance.at(i).push_back(abs(triangle_centroid.at(i).at(j).x - contour_centroid.at(i).x));
			sum+=x_point_distance.at(i).at(j);
		}
		model_x_avg = sum/(largest_distance_centroid*triangle_centroid.at(i).size());

		cout<<"contourArea: -> "<<area<<endl;
		cout<<"largest_distance: -> "<<largest_distance_centroid<<endl;
		cout<<"distance model wrt x: -> "<<model_x_avg<<endl;
		cout<<"area model wrt x: -> "<<sum/(area*triangle_centroid.at(i).size())<<endl;
		cout<<"base_angle: -> "<<base_angle<<endl;

		cout<<"farthest_point: ->"<<farthest_point.x<<" , "<<farthest_point.y<<endl;
		largest_distance_centroid = 0;
	}

}

void dino_model::largest_distance(vector<cv::Point> &contour, cv::Point &contour_centroid) {
	double temp_dist;
	for(uint j=0; j<contour.size(); j++) {
		temp_dist = sqrt(pow(contour.at(j).x - contour_centroid.x, 2) + pow(contour.at(j).y - contour_centroid.y, 2));
		largest_distance_centroid = max(largest_distance_centroid, temp_dist);
		if(largest_distance_centroid == temp_dist)
			farthest_point = contour.at(j);
	}
}

inline void dino_model::find_base_angle(cv::Point &contour_centroid) {
	cout<<endl<<contour_centroid.y <<" - "<< farthest_point.y<<endl;
	cout<<endl<<farthest_point.x<< " - " <<contour_centroid.x<<endl;
	base_angle = atan2(contour_centroid.y - farthest_point.y, farthest_point.x - contour_centroid.x);
	cout<<"base_angle: ->"<<base_angle<<endl;
}