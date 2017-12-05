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
#include "std_msgs/Float32MultiArray.h"
#include "opencv2/opencv.hpp"

//for ras evidence:
#include <ras_msgs/RAS_Evidence.h>
#include <sensor_msgs/image_encodings.h>

//for writing to rosbag:
#include<rosbag/bag.h>

using namespace cv;
using namespace std;
//try:
//#include <sensor_msgs/ChannelFloat32>



//static const std::string OPENCV_WINDOW = "Image window";
ras_msgs::RAS_Evidence evidence;

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  
  ros::Publisher evidence_pub_;



  //ros::Subscriber color_vec_pub_, shape_vec_pub;

  //ros::Publisher pub_center;
    int id;
  float xpos;
  float ypos;


public:
  ImageConverter(int inid=0,float x=0, float y=0)
  //ImageConverter()
    : it_(nh_)
  {
    // Subscrive to input video feed and publish output video feed
    image_sub_ = it_.subscribe("/camera/rgb/image_raw", 1,
      &ImageConverter::imageCb, this);
    evidence_pub_ = nh_.advertise<ras_msgs::RAS_Evidence>("/evidence",1);
    id=inid;
    xpos=x;
    ypos=y;

    //pub_center = nh_.advertise<std_msgs::Int32MultiArray>("center_of_object", 1);

    //cv::namedWindow(OPENCV_WINDOW);
  }

  ~ImageConverter()
  {
    //cv::destroyWindow(OPENCV_WINDOW);
  }

  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {
      cout<<"beginning function"<<endl;
/*
 *
    During the final contest, record a rosbag with your group number but only record the /evidence topic which contains the evidence messages, e.g. :

rosbag record -O dd2425_ht17_Gx_phaseY /evidence*/

      rosbag::Bag bag;



       string str;
   if (id==0) str=evidence.an_object;
   else if (id==1) str=evidence.green_cube;
   else if (id==2) str=evidence.blue_cube;
   else if (id==3) str=evidence.yellow_cube;
   else if (id==4) str=evidence.red_hollow_cube;
   else if (id==5) str=evidence.green_hollow_cube;
   else if (id==6) str=evidence.red_ball;
   else if (id==7) str=evidence.yellow_ball;
   else if (id==8) str=evidence.blue_triangle;
   else if (id==9) str=evidence.red_cylinder;
   else if (id==10) str=evidence.green_cylinder;
   else if (id==11) str=evidence.purple_cross;
   else if (id==12) str=evidence.orange_cross;
   else if (id==13) str=evidence.patric;
   else if (id==14) str=evidence.purple_star;


    evidence.stamp = ros::Time::now();

    evidence.group_number=8;
    evidence.image_evidence = *msg;//(cv_ptr->image).makeShared();
    evidence.object_id=str;

       evidence.object_location.header.stamp=ros::Time::now();
   evidence.object_location.header.frame_id="map";
   evidence.object_location.child_frame_id="map";
   evidence.object_location.transform.translation.x=xpos;
   evidence.object_location.transform.translation.y=ypos;
   evidence.object_location.transform.rotation.x=0;
   evidence.object_location.transform.rotation.y=0;
   evidence.object_location.transform.rotation.z=0;
   evidence.object_location.transform.rotation.w=1;


    evidence_pub_.publish(evidence);


    cout<<"in function"<<endl;
    bag.open("/home/ras18/bagfiles/dd2425_ht17_G8_phasetest.bag", rosbag::bagmode::Write);
    bag.write("/evidence", ros::Time::now(), evidence);
    bag.close();


    //ros::spinOnce();
  }


};



int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic(1,2,3);
  //ros::spinOnce();


  ROS_INFO("What's up");
   ros::Rate loop_rate(2);
   for (int i=0;i<5;i++) {
       ros::spinOnce();
       loop_rate.sleep();

   }

  //ros::spin();
  return 0;
}
