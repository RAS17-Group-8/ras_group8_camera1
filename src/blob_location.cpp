
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//for publicing center as multiarray: ,... maybe not all needed
#include <stdio.h>
#include <stdlib.h>
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
#include "opencv2/opencv.hpp"
//to read and write file:
#include <math.h>
#include <fstream>
#include <string>

//random:
#include <cstdlib>

#include<iostream>


using namespace cv;
using namespace std;
//try:
//#include <sensor_msgs/ChannelFloat32>
int once = 1;



static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;
  //ros::Publisher pub_center;
  std::vector<ros::Publisher> pub_center_point;



public:
  ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/camera/rgb/image_raw", 1,
      &ImageConverter::imageCb, this);
    image_pub_ = it_.advertise("/image_converter/output_video_blob",1);

    //pub_center = nh_.advertise<std_msgs::Int32MultiArray>("center_of_object", 1);

    cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }


    int** result_matrix = new int*[255];
    //if(once == 1){
        //Get classification matrix:

        ifstream resultFile;
        resultFile.open("/home/ras18/catkin_ws/src/ras_group8/ras_group8_camera1/classifiers/colors_itr1e4_1e-5_gamma001_newer.dat");

        for(int lin=0;lin<255;lin++)
        {
            result_matrix[lin] = new int[360];

            for(int col=0;col<360;col++)
            {
                //resultFile >> result_matrix.at<int>(lin,col) ;
                 //resultFile >> result_matrix[lin][col];//result_matrix.at<int>(lin,col) ;
                resultFile >> result_matrix[lin][col];
                //cout << result_matrix[lin][col];//result_matrix.at<int>(0,1);
                //cout <<result_matrix.at<int>(0,1) << " ";
            }

            //cout << endl;
        }
        resultFile.close();
        once =0;
    //}
    //ROS_INFO("rows = %i, cols= %i", cv_ptr->image.rows, cv_ptr->image.cols);

    cv::Mat GRAYImage;

    //Transform the colors into grayscale
    cv::cvtColor(cv_ptr->image,GRAYImage,CV_BGR2GRAY);

    // Read image
    Mat im = GRAYImage;

    // Setup SimpleBlobDetector parameters.
    SimpleBlobDetector::Params params;

    // Change thresholds
    params.minThreshold = 5;//10;
    params.maxThreshold = 255;//200;

    // Filter by Area.
    params.filterByArea = true;
    params.minArea = 2500;//500;//200;
    params.maxArea = 100000;//4000;//10000;

    // Filter by Circularity
    params.filterByCircularity = true;
    params.minCircularity = 0;//0.2;
    params.maxCircularity = 1;

    // Filter by Convexity
    params.filterByConvexity = true;
    params.minConvexity = 0.5;//0.8;//0.1;
    params.maxConvexity = 1;

    // Filter by Inertia
    params.filterByInertia = true;
    params.minInertiaRatio = 0.1;//0.01;
    params.maxInertiaRatio = 1;


    // Storage for blobs
    vector<KeyPoint> keypoints;


#if CV_MAJOR_VERSION < 3   // If you are using OpenCV 2

    // Set up detector with params
    SimpleBlobDetector detector(params);

    // Detect blobs
    detector.detect( im, keypoints);
#else

    // Set up detector with params
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

    // Detect blobs
    detector->detect( im, keypoints);
#endif

    // Draw detected blobs as red circles.
    // DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
    // the size of the circle corresponds to the size of blob

    Mat im_with_keypoints;
    drawKeypoints( im, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
    // Show blobs
    cv::imshow(OPENCV_WINDOW, im_with_keypoints);



    // Get HSV image and manipulate it:
    cv::Mat HSVImage;
    //Transform the colors into HSV
    cv::cvtColor(cv_ptr->image,HSVImage,CV_BGR2HSV);


    int rows = HSVImage.rows;
    int cols = HSVImage.cols;
    //remove noise
    cv::Mat str_el = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    morphologyEx(HSVImage, HSVImage, cv::MORPH_OPEN, str_el);
    morphologyEx(HSVImage, HSVImage, cv::MORPH_CLOSE, str_el);


    int xrand;
    int yrand;

    int H;
    int S;

    int orange_sat_limit = 0.7*255;
    int orange_hue_limit = 90;

    int green_sat_limit = 0.5*255;
    int green_hue_limit = 210;
    int number_of_color_votes = 0;
    float color_votes[9] = {0.0};

    std_msgs::Int32MultiArray center_array;

    //cout << "keypoints size:" << keypoints.size() << endl;

    for (int i = 0; i < keypoints.size(); ++i)
    {
        std::string topicName = "/blob/center" + boost::lexical_cast<std::string>(i);

        ros::Publisher pub1 = nh_.advertise<std_msgs::Int32MultiArray>(topicName, 1);

        pub_center_point.push_back(pub1);
    }



    int xpos;
    int ypos;
    int iteration = 0;


    float PI = 3.1416;
    float dy_from_middle = 188.7; //mm
    int h_Im = rows;
    int w_Im = cols;
    float a = 231; //mm
    float C = 144.82*PI/180;
    float C1 = PI - C;
    float d_floor_y;

    float angle;

    int h_rel;
    int w_rel;

    float w_floor;
    float w_real;

    for(std::vector<cv::KeyPoint>::iterator blobIterator = keypoints.begin(); blobIterator != keypoints.end(); blobIterator++){
       std::cout << "size of blob is: " << blobIterator->size << std::endl;
       std::cout << "point is at: " << blobIterator->pt.x << " " << blobIterator->pt.y << std::endl;

       xpos = blobIterator->pt.x;
       ypos = blobIterator->pt.y;
       center_array.data.clear();

       center_array.data.push_back(xpos);//blobIterator->pt.x);
       center_array.data.push_back(ypos);//blobIterator->pt.y);



       //manipulate blob here:
       //FOR GETTING LOCATION:

       //if x horizontal, y vertical..?
       h_rel = xpos - h_Im/2;
       w_rel = ypos - w_Im/2;
       if(h_rel<0){
           angle = h_rel*23.8/(h_Im*2) * PI/180;
           d_floor_y = dy_from_middle+ sin(angle)*231/sin(PI/180*(180-144.82)-angle);
       }
       else{
           angle = -h_rel*23.8/(h_Im*2) * PI/180;
           d_floor_y = dy_from_middle+ sin(angle)*231/sin(PI/180*(180-35.18)-angle);
       }

       w_floor = 352/660 * d_floor_y;
       w_real = w_rel / (w_Im/2) * w_floor;


       cout << "floor x: " << d_floor_y << "y: " << w_real<< endl;


       // FOR GETTING COLOR:
       for (int i=0; i<50; i++){
           //get random pixel locations within blob:
           //ROS_INFO("1");
           xrand = rand() % (int)(blobIterator->size / 1.4142) + blobIterator->pt.x - blobIterator->size / 2; //just get inside circle
           yrand = rand() % (int)(blobIterator->size / 1.4142) + blobIterator->pt.y - blobIterator->size / 2;
           if (xrand < 0) xrand = 0;
           if (yrand< 0 ) yrand = 0;
           if (xrand > rows ) xrand = rows;
           if (yrand > cols ) yrand = cols;
           //find H and S values in these locations:
           Vec3b HSVPoint = HSVImage.at<Vec3b>(Point(xrand, yrand));

           //ROS_INFO("2");
           H = HSVPoint.val[0]*2;
           S = HSVPoint.val[1];
           //ROS_INFO("3");

           //cout << "H: " << H << " S: " << S << endl;
           //For removing H and S values with high ambiguity:
           if(S>orange_sat_limit || (S>green_sat_limit && H>orange_hue_limit) || H>green_hue_limit){
               //number_of_color_votes++;
               color_votes[result_matrix[(int)S-1][(int)H-1]]++;
           }
           //ROS_INFO("4");
       }
       for(int itr=0; itr<9;itr++) color_votes[itr] /= 50;
       std::cout << "orange:" << color_votes[0] << " red:" << color_votes[1] << " yellow:" << color_votes[2]
                    << " purple:" << color_votes[3] << " blue:" << color_votes[4] << " dark_green:" << color_votes[5]
                       <<" light green:" << color_votes[6] <<" battery:" << color_votes[7] << " trap:" << color_votes[8]
                         << endl;

       pub_center_point[iteration].publish(center_array);

       iteration++;
    }



    //for getting color values in one point:
    /*
    Vec3b colour = drawing.at<Vec3b>(Point(x, y));
    if(colour.val[0]==255 && colour.val[1]==255 && colour.val[2]==255)*/

    //points within: (x or y) +- (<=blobsize/ (2*1.4142)) ,


    //imshow("keypoints", im_with_keypoints );
    waitKey(3);


    //from old program:
    // Update GUI Window
    //cv::imshow(OPENCV_WINDOW, ThreshImage);
    //cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    //cv::waitKey(3);    // Output modified video stream


    //image_pub_.publish(cv_ptr->toImageMsg());

  }
};



int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  //Slower loop rate perhaps when training:
  ros::Rate loop_rate(2);
  for (;;) {
      ros::spinOnce();
      loop_rate.sleep();
  }
  ros::spin();
  return 0;
}
