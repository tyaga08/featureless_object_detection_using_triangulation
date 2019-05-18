# Featureless Object Detection Using Triangulation:

This is an algorithm based on Delaunay Triangulation used for detection of objects with almost neglibile/no features and smooth textures. This algorithm comes handy when the features/keypoints based techniques fail to give results. This method uses the geometric properties of the object.

![1](https://user-images.githubusercontent.com/11211895/51793813-a9d9a400-2194-11e9-8975-92a89c704051.png)

![2](https://user-images.githubusercontent.com/11211895/51793815-b3fba280-2194-11e9-9847-ed7f11522a27.png)

![3](https://user-images.githubusercontent.com/11211895/51793816-bfe76480-2194-11e9-8a1c-f222d16f31e9.png)

![4](https://user-images.githubusercontent.com/11211895/51793821-c7a70900-2194-11e9-9fed-30f983ea9cf0.png)


### Requirements:
1. OpenCV 3.0
2. CMake 3.5 or above

### How to run:  
>git clone https://github.com/tyaga08/featureless_object_detection_using_triangulation.git  
mkdir build  
cd build  
cmake .  
make  
./image_recognition  
  
To change the images, either add the new image to the ‘images’ folder or choose one from the folder.
You could change the image according to your choice in the /src/image_recognition.cpp line number 10:  
img1 = cv::imread("images/your_image_with_extension", cv::IMREAD_COLOR);
