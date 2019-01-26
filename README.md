Featureless Object Detection Using Triangulation:

This is an algorithm based on Delaunay Triangulation used for detection of objects with almost neglibile/no features and smooth textures. This algorithm comes handy when the features/keypoints based techniques fail to give results. This method uses the geometric properties of the object.

 









Requirements:
1. OpenCV 3.0
2. CMake 3.5 or above

How to run:
1. Clone the repository and open the terminal in that folder
2. Follow the following terminal command:
	$ cmake .
	$ make
	$ ./image_recognition

To change the images, either add the new image to the ‘images’ folder or choose one from the folder
You could change the image according to your choice in the /src/image_recognition.cpp line number 10:
img1 = cv::imread("images/your_image_with_extension", cv::IMREAD_COLOR);

