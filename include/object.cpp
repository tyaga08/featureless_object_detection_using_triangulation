#include "header.h"

class dino_object { 
public:
	vector<float> slope;
	vector<vector<double> > y_point_distance, x_point_distance;
	double largest_distance_centroid=0;
	vector<double> object_x_avg, object_y_avg;
	vector<uint> detected_dino_contour;

	float base_angle, base_axis_angle, base_axis_slope;
	cv::Point2f farthest_point;

	void largest_distance(vector<cv::Point> &contour, cv::Point &contour_centroid);
	void check_distance_centroid(vector<vector<cv::Point> > &contour, vector<cv::Point> &contour_centroid, vector<vector<cv::Point2f> > &triangle_centroid);
	void find_base_angle(cv::Point &contour_centroid);
	void find_base_axis(cv::Point &contour_centroid);
};

// CONDITION FOR CHECKING WHETHER THE MEAN NORMALIZED DISTANCE LIES WITHIN THE RANGE OF THE OBJECT
// NORMALIZE ALL THE DISTANCE FROM TRIANGLE CENTROIDS TO THE X AND Y AXIS OF THE OBJECT
// TAKE THE AVERAGE OF THOSE AND COMPARE WITH THE SET THRESHOLD
void dino_object::check_distance_centroid(vector<vector<cv::Point> > &contour, vector<cv::Point> &contour_centroid, vector<vector<cv::Point2f> > &triangle_centroid) {

	double temp_dist;	
	y_point_distance.resize(triangle_centroid.size());
	x_point_distance.resize(triangle_centroid.size());
	for(uint i=0; i<contour.size(); i++) {
		
		largest_distance(contour.at(i), contour_centroid.at(i));
		find_base_angle(contour_centroid.at(i));
		find_base_axis(contour_centroid.at(i));
		
		double sum=0;

		for(uint j=0; j<triangle_centroid.at(i).size(); j++) {
			float k = 1/base_axis_slope; 
			float x_int = (contour_centroid.at(i).y - triangle_centroid.at(i).at(j).y + k*contour_centroid.at(i).x + triangle_centroid.at(i).at(j).x/k)/(k+1/k);
			float y_int = -k*(x_int) + contour_centroid.at(i).y + k*contour_centroid.at(i).x;
			temp_dist = sqrt(pow(x_int - triangle_centroid.at(i).at(j).x, 2) + pow(y_int - triangle_centroid.at(i).at(j).y, 2));
			y_point_distance.at(i).push_back(temp_dist);
			sum+=y_point_distance.at(i).at(j);
		}
		
		double area = cv::contourArea(contour.at(i), false);
		object_y_avg.push_back(sum/(largest_distance_centroid*triangle_centroid.at(i).size()));

		cout<<"contourArea: -> "<<area<<endl;
		cout<<"largest_distance: -> "<<largest_distance_centroid<<endl;
		cout<<"distance model wrt y: -> "<<object_y_avg.at(i)<<endl;
		cout<<"area model wrt y: -> "<<sum/(area*triangle_centroid.at(i).size())<<endl;

		sum=0;

		for(uint j=0; j<triangle_centroid.at(i).size(); j++) {
			float x_int = (contour_centroid.at(i).y - triangle_centroid.at(i).at(j).y + base_axis_slope*contour_centroid.at(i).x + triangle_centroid.at(i).at(j).x/base_axis_slope)/(base_axis_slope + 1/base_axis_slope);
			float y_int = -base_axis_slope*(x_int) + contour_centroid.at(i).y + base_axis_slope*contour_centroid.at(i).x;
			temp_dist = sqrt(pow(x_int - triangle_centroid.at(i).at(j).x, 2) + pow(y_int - triangle_centroid.at(i).at(j).y, 2));
			x_point_distance.at(i).push_back(temp_dist);
			sum+=x_point_distance.at(i).at(j);
		}
		object_x_avg.push_back(sum/(largest_distance_centroid*triangle_centroid.at(i).size()));

		cout<<"contourArea: -> "<<area<<endl;
		cout<<"largest_distance: -> "<<largest_distance_centroid<<endl;
		cout<<"distance model wrt x: -> "<<object_x_avg.at(i)<<endl;
		cout<<"area model wrt x: -> "<<sum/(area*triangle_centroid.at(i).size())<<endl;
		cout<<"==========================================================================================="<<endl;

		largest_distance_centroid=0;
		if(object_x_avg.at(i)>=0.3 && object_x_avg.at(i)<=0.5 && object_y_avg.at(i)>=0.3 && object_y_avg.at(i)<=0.5)
		detected_dino_contour.push_back(i);
	}
}

// FIND THE LARGEST DISTANCE FROM THE CENTROID OF THE OBJECT. THIS IS USEFUL FOR NORMALIZING THE DISTANNCES. IMPORTANT FOR SCALE INVARIANCE
void dino_object::largest_distance(vector<cv::Point> &contour, cv::Point &contour_centroid) {
	double temp_dist;
	for(uint j=0; j<contour.size(); j++) {
		temp_dist = sqrt(pow(contour.at(j).x - contour_centroid.x, 2) + pow(contour.at(j).y - contour_centroid.y, 2));
		largest_distance_centroid = max(largest_distance_centroid, temp_dist);
		if(largest_distance_centroid == temp_dist)
			farthest_point = contour.at(j);
	}
}

// REQUIRED FOR FINDING THE OBJECT AXES
inline void dino_object::find_base_angle(cv::Point &contour_centroid) {
	base_angle = atan2(contour_centroid.y - farthest_point.y, farthest_point.x - contour_centroid.x);
	cout<<"base_angle: -> "<<base_angle<<endl;
}

void dino_object::find_base_axis(cv::Point &contour_centroid) {
	base_axis_angle = base_angle - 0.726642;//model_base_angle;
	cout<<"base_axis_angle ->"<<base_axis_angle<<endl;
	base_axis_slope = tan(base_axis_angle);
	cout<<"base_axis_slope: -> "<<base_axis_slope<<endl;
}