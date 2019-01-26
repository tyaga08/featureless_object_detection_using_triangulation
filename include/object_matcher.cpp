#include "header.h"

// cv::Point2f point_map;

struct map_cvpoint2f
{
	bool operator()(cv::Point2f const& lhs, cv::Point2f const& rhs) const
	{
		return lhs.x == rhs.x ? lhs.y < rhs.y : lhs.x < rhs.x;
	}
	
};

class object_matcher {
public:
	vector<vector<cv::Point2f> > triangle_centroids;
	vector<vector<cv::Vec4f> > centroid_line_list;
	vector<vector<float> > skeletal_slopes, slope_derivative;
	vector<vector<uint> > centroid_line_index;

	map<cv::Point2f, int, map_cvpoint2f> centroid_line_map;
	map<cv::Point2f, int, map_cvpoint2f>::iterator map_itr;

	int line_count;

	float slope;
	bool check = true;
	uint index_min = 0, index_sec_min = 0, index;

	cv::Point2f current, previous;
	float dist, min_dist, sec_min_dist;

	object_matcher(vector<vector<cv::Vec6f> > &triangle_list) {
		triangle_centroids.resize(triangle_list.size());
		skeletal_slopes.resize(triangle_list.size());
		centroid_line_list.resize(triangle_list.size());
		centroid_line_index.resize(triangle_list.size());
	}

	void get_triangle_centroids(vector<vector<cv::Vec6f> > &triangle_list, cv::Mat &img);
	void get_skeletal_slopes(cv::Mat &img, vector<cv::Point> &contour_centroid); 
};

void object_matcher::get_triangle_centroids(vector<vector<cv::Vec6f> > &triangle_list, cv::Mat &img) {
	for(uint i=0; i<triangle_list.size(); i++) {
		for(uint j=0; j<triangle_list.at(i).size(); j++) {
			triangle_centroids.at(i).push_back(cv::Point2f((triangle_list.at(i).at(j)[0]+triangle_list.at(i).at(j)[2]+triangle_list.at(i).at(j)[4])/3, (triangle_list.at(i).at(j)[1]+triangle_list.at(i).at(j)[3]+triangle_list.at(i).at(j)[5])/3));
			img.at<float>(triangle_centroids.at(i).at(j).y, triangle_centroids.at(i).at(j).x) = 255;
		}
	}
}



















void object_matcher::get_skeletal_slopes(cv::Mat &img, vector<cv::Point> &contour_centroid) {
	for(uint i=0; i<triangle_centroids.size(); i++) {
		previous = triangle_centroids.at(i).at(0);

		for(uint j=0; j<triangle_centroids.at(i).size(); j++) {
			current = triangle_centroids.at(i).at(j);
			min_dist = 100;
			for(uint k=0; k<triangle_centroids.at(i).size(); k++) {
				if(k!=j) {
					dist = sqrt(pow(triangle_centroids.at(i).at(k).x - current.x,2)+pow(triangle_centroids.at(i).at(k).y - current.y,2));
					
					min_dist = min(min_dist,dist);
					if(min_dist == dist) {
						// for(uint a=0;a<centroid_line_index.at(i).size(); a++) {
						// 	if(centroid_line_index.at(i).at(a)==j*10+k || centroid_line_index.at(i).at(a)==k*10+j) {
						// 		check=false;
						// 		break;
						// 	}
						// }

						if(check==true) {
							index = k;
						}

						else check = true;
						index = k;
					}
				}
			}
				cv::Vec4f temp_point = {current.x, current.y, triangle_centroids.at(i).at(index).x, triangle_centroids.at(i).at(index).y};
				centroid_line_list.at(i).push_back(temp_point);
				centroid_line_index.at(i).push_back(j*10+index);
		}
	}

	/*for(uint i=0; i<triangle_centroids.size(); i++) {
		for(uint j=0; j<triangle_centroids.at(i).size(); j++) {
			// centroid_line_map.insert(pair<cv::Point2f, int>(triangle_centroids.at(i).at(j),0));
			centroid_line_map[triangle_centroids.at(i).at(j)] = 0;
		}

		for(uint j=0; j<triangle_centroids.at(i).size(); j++) {
			current = triangle_centroids.at(i).at(j);
			min_dist = 100;
			sec_min_dist = 200;

			if(centroid_line_map[current] < 2) {
				for(uint k=0; k<triangle_centroids.at(i).size();k++) {
					if(k!=j)
					dist = sqrt(pow(triangle_centroids.at(i).at(k).x - current.x,2)+pow(triangle_centroids.at(i).at(k).y - current.y,2));
					
					if(dist<=min_dist && centroid_line_map.at(triangle_centroids.at(i).at(k))<=2) {
						sec_min_dist = min_dist;
						min_dist = dist;
						index_sec_min = index_min;
						index_min = k;
					}

					else if(dist<sec_min_dist && centroid_line_map.at(triangle_centroids.at(i).at(k))<=2) {
						sec_min_dist = dist;
						index_sec_min = k;	
					}
				}

				cv::Vec4f temp_point = {current.x, current.y, triangle_centroids.at(i).at(index_min).x, triangle_centroids.at(i).at(index_min).y};
				centroid_line_list.at(i).push_back(temp_point);
				centroid_line_map.at(current)+=1;
				centroid_line_map.at(triangle_centroids.at(i).at(index_min))+=1;
				if(centroid_line_map.at(current)<2) {
				temp_point = {current.x, current.y, triangle_centroids.at(i).at(index_sec_min).x, triangle_centroids.at(i).at(index_sec_min).y};
				centroid_line_list.at(i).push_back(temp_point);
				centroid_line_map.at(current)+=1;
				centroid_line_map.at(triangle_centroids.at(i).at(index_sec_min))+=1;
				}
			}
		}
		cout<<"MAP of points"<<endl;
		for(map_itr = centroid_line_map.begin(); map_itr!= centroid_line_map.end(); map_itr++) {
			cout<<map_itr->first<<"			"<<map_itr->second<<endl;
		}
		centroid_line_map.clear();
	}*/
	cv::Mat new_img = cv::Mat::zeros(cv::Size(img.cols,img.rows), CV_32FC1);
	for(uint i=0; i<centroid_line_list.size(); i++) {
		cout<<"centroid_line_list size --> "<<centroid_line_list.at(i).size()<<endl;
		for(uint j=0; j<centroid_line_list.at(i).size();j++) {
			cv::line(new_img, cv::Point2f(centroid_line_list.at(i).at(j)[0],centroid_line_list.at(i).at(j)[1]), cv::Point2f(centroid_line_list.at(i).at(j)[2], centroid_line_list.at(i).at(j)[3]), cv::Scalar(255), 1, cv::LINE_AA, 0);
			cv::imshow("wtf", new_img);
			// cv::waitKey(100);
		}
	}
	cout<<"The skeletal slopes:	---->"<<endl;
	/*for(uint i=0; i<centroid_line_list.size(); i++) {
		for(uint j=0; j<centroid_line_list.at(i).size();j++) {
			slope = (centroid_line_list.at(i).at(j)[1] - centroid_line_list.at(i).at(j)[3])/(centroid_line_list.at(i).at(j)[0] - centroid_line_list.at(i).at(j)[2]);
			skeletal_slopes.at(i).push_back(slope);
			// cout<<skeletal_slopes.at(i).at(j)<<endl;
		}

		// sort(skeletal_slopes.at(i).begin(), skeletal_slopes.at(i).end());
*/
	for(uint i=0; i<triangle_centroids.size(); i++) {
		for(uint j=0; j<centroid_line_list.at(i).size(); j++) {
			slope = atan2(triangle_centroids.at(i).at(j).y - contour_centroid.at(i).y, triangle_centroids.at(i).at(j).x - contour_centroid.at(i).x);
			skeletal_slopes.at(i).push_back(slope*180/pi);
			// cout<<skeletal_slopes.at(i).at(j)<<endl;
		}

		// sort(skeletal_slopes.at(i).begin(), skeletal_slopes.at(i).end());

/*		remove duplicates from sorted array
		for(uint j=1; j<skeletal_slopes.at(i).size(); j++) {
			if(skeletal_slopes.at(i).at(j)==skeletal_slopes.at(i).at(j-1)) {
				skeletal_slopes.at(i).erase(skeletal_slopes.at(i).begin() + j);
				j--;
			}
		}*/
		for(uint j=0; j<skeletal_slopes.at(i).size();j++) {
			cout<<skeletal_slopes.at(i).at(j)<<endl;
		}
		cout<<skeletal_slopes.at(i).size()<<endl;
	}

	slope_derivative.resize(skeletal_slopes.size());

	for(uint i=0; i<skeletal_slopes.size(); i++) {
		for(uint j=1; j<skeletal_slopes.at(i).size(); j++) {
			slope_derivative.at(i).push_back(round((skeletal_slopes.at(i).at(j) - skeletal_slopes.at(i).at(j-1))));
		}

		for(uint j=0; j<slope_derivative.at(i).size();j++) {
			cout<<slope_derivative.at(i).at(j)<<endl;
		}
	}

	plt::plot(slope_derivative.at(0));
	plt::show();
}