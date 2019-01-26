#include "header.h"

#define point_count 500 //NUMBER OF SAMPLE POINTS REQUIRED ON THE CONTOUR (VERTICES OF DELAUNAY TRIANGLES)

class delaunay_triangles {
public:
	vector<vector<cv::Point> > copy_contour, outline_points, assorted_outline_points;
	float xp, yp, theta, x_cent, y_cent;
	float prev_dist, dist, min_dist, current_slope, previous_slope;
	cv::Point current, previous, previous_in_line;
	int csize;

	vector<cv::Subdiv2D> del;
	cv::Rect window;
	vector<vector<cv::Vec6f> > triangle_list; // VECTOR CONTAINING THE VECTOR OF VERTICES OF DELAUNAY TRIANGLES
	vector<cv::Point> triangulated_points = vector<cv::Point>(3);
	bool animate = false;

	vector<vector<cv::Point2f> > triangle_centroids; // CENTROIDS OF DELAUNAY TRIANGLES

	delaunay_triangles(cv::Mat img) {
		window = cv::Rect(0, 0, img.cols, img.rows);
	}

	~delaunay_triangles() {
	}

	void find_outline(vector<vector<cv::Point> > &contour, vector<cv::Point> contour_centroid);
	void get_delaunay_triangles(cv::Mat img, cv::Mat threshold_image);
	void draw_triangles(cv::Mat img, cv::Mat threshold_image, uint index);
	void check_midpoint(cv::Mat img, uint index);
	void get_triangle_centroids(cv::Mat img);

};

// GETTING THE SAMPLE POINTS ACCORDING TO THE NUMBER SET ABOVE
void delaunay_triangles::find_outline(vector<vector<cv::Point> > &contour, vector<cv::Point> contour_centroid) {
	copy_contour = contour;
	outline_points.resize(copy_contour.size());

	for(uint i=0; i<copy_contour.size();i++) {
		csize = (int)copy_contour.at(i).size()/point_count;
		int pcount = 0;
		outline_points.at(i).push_back(copy_contour.at(i).at(0));
		for(uint j=0; j<copy_contour.at(i).size();j++) {
			if(pcount!=csize) {
				pcount++;
			}
			else {
				outline_points.at(i).push_back(copy_contour.at(i).at(j));
				pcount = 0;
			}
		}
		outline_points.at(i).push_back(contour_centroid.at(i));
	}
}	

// FIND THE ALL THE DELAUNAY TRIANGLES
void delaunay_triangles::get_delaunay_triangles(cv::Mat img, cv::Mat threshold_image) {
	del.resize(outline_points.size());
	triangle_list.resize(del.size());
	for(uint i=0; i<outline_points.size(); i++) {
		del.at(i) = cv::Subdiv2D(window);
		for(uint j=0; j<outline_points.at(i).size(); j++) {
			cv::Point2f temp_point = cv::Point2f(outline_points.at(i).at(j).x, outline_points.at(i).at(j).y);
			del.at(i).insert(temp_point);
			
			if(animate) {
				draw_triangles(img, threshold_image, i);
				cv::waitKey(100);
			}
		}

		if(!animate) draw_triangles(img, threshold_image, i);
	}
	cout<<"triangle_list-vector size: "<<triangle_list.size()<<endl;
	get_triangle_centroids(img);
}

// DRAW THE DELAUNAY TRIANGLES
void delaunay_triangles::draw_triangles(cv::Mat del_img, cv::Mat threshold_image, uint index) {
	del.at(index).getTriangleList(triangle_list.at(index));
	check_midpoint(threshold_image, index);
	for(uint i=0; i<triangle_list.at(index).size(); i++) {
		triangulated_points.at(0) = cv::Point(cvRound(triangle_list.at(index).at(i)[0]), cvRound(triangle_list.at(index).at(i)[1]));
		triangulated_points.at(1) = cv::Point(cvRound(triangle_list.at(index).at(i)[2]), cvRound(triangle_list.at(index).at(i)[3]));
		triangulated_points.at(2) = cv::Point(cvRound(triangle_list.at(index).at(i)[4]), cvRound(triangle_list.at(index).at(i)[5]));

		if(window.contains(triangulated_points.at(0)) && window.contains(triangulated_points.at(1)) && window.contains(triangulated_points.at(2))) {
			cv::line(del_img, triangulated_points.at(0), triangulated_points.at(1), cv::Scalar(255), 1, cv::LINE_AA, 0);
			cv::line(del_img, triangulated_points.at(1), triangulated_points.at(2), cv::Scalar(255), 1, cv::LINE_AA, 0);
			cv::line(del_img, triangulated_points.at(2), triangulated_points.at(0), cv::Scalar(255), 1, cv::LINE_AA, 0);
		}
		// cv::imshow("delaunay_triangles", del_img);
	}
	cout<<"Number of triangles: "<<triangle_list.at(index).size()<<endl;	
}

// CHECK WHETHER THE SIDES OF THE TRIANGLES ARE ON OR INSIDE THE CONTOUR. THIS FUNCTION DELETES ALL THE TRIANGLES WHOSE SIDES LIE OUTSIDE THE CONTOUR
void delaunay_triangles::check_midpoint(cv::Mat img, uint index) {
	cout<<"checking"<<endl;
	cv::Point mid_point;
	bool delete_triangle = false;
	for(int i=0;i<triangle_list.at(index).size(); ++i) {
		
		int count=0;
		for(uint m=0; m<=4; m+=2) {
			if(triangle_list.at(index).at(i)[m]<=0 || triangle_list.at(index).at(i)[m]>=640) {
				triangle_list.at(index).erase(triangle_list.at(index).begin()+i);
				i--;
				delete_triangle = true;
				break;
			}
		}
		if(delete_triangle == true) {
			delete_triangle = false;
			continue;
		}

		for(uint m=1; m<=5; m+=2) {
			if(triangle_list.at(index).at(i)[m]<=0 || triangle_list.at(index).at(i)[m]>=480) {
				triangle_list.at(index).erase(triangle_list.at(index).begin()+i);
				i--;	
				delete_triangle = true;
				break;
			}
		}
		if(delete_triangle == true) {
			delete_triangle = false;
			continue;
		}
		mid_point = cv::Point(cvRound((triangle_list.at(index).at(i)[0] + triangle_list.at(index).at(i)[2])/2),cvRound((triangle_list.at(index).at(i)[1] + triangle_list.at(index).at(i)[3])/2));
		if(img.at<uchar>(mid_point.y, mid_point.x)==0) count++;
		
		mid_point = cv::Point(cvRound((triangle_list.at(index).at(i)[2] + triangle_list.at(index).at(i)[4])/2),cvRound((triangle_list.at(index).at(i)[3] + triangle_list.at(index).at(i)[5])/2));
		if(img.at<uchar>(mid_point.y, mid_point.x)==0) {
			count++;
		}
		
		mid_point = cv::Point(cvRound((triangle_list.at(index).at(i)[4] + triangle_list.at(index).at(i)[0])/2),cvRound((triangle_list.at(index).at(i)[5] + triangle_list.at(index).at(i)[1])/2));
		if(img.at<uchar>(mid_point.y, mid_point.x)==0) count++;
		
		if(count>=2) {
			triangle_list.at(index).erase(triangle_list.at(index).begin()+i);
			i--;
		}
		contin:;

	}
}

// GET TRIANGLE CENTROIDS
void delaunay_triangles::get_triangle_centroids(cv::Mat img) {
	triangle_centroids.resize(triangle_list.size());
	for(uint i=0; i<triangle_list.size(); i++) {
		for(uint j=0; j<triangle_list.at(i).size(); j++) {
			triangle_centroids.at(i).push_back(cv::Point2f((triangle_list.at(i).at(j)[0]+triangle_list.at(i).at(j)[2]+triangle_list.at(i).at(j)[4])/3, (triangle_list.at(i).at(j)[1]+triangle_list.at(i).at(j)[3]+triangle_list.at(i).at(j)[5])/3));
		}
	}
}