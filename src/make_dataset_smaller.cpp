#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

//to read and write file:
#include <fstream>
#include <string>

#include <iostream>
#include <iomanip>

#include <algorithm>
using namespace std;

using namespace cv;



template <typename T>
string ToString(T val)
{
    stringstream stream;
    stream << val;
    return stream.str();
}

int main() {
    float h;
    float s;
    int i=0;
    ifstream Hue;
    ifstream Sat;

    ofstream Hfile;
    Hfile.open("/home/ras18/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/orange_H.dat");
    ofstream Sfile;
    Sfile.open("/home/ras18/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/orange_S.dat");


    Hue.open("/home/ras18/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/old data/orange_big_H.dat");
    Sat.open("/home/ras18/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/old data/orange_big_S.dat");



    if (!Hue || !Sat) {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }

    while (Hue >> h) {

        Sat >> s;

        //if ( i % 3 != 0 /*&& i< 1200*/){

        if(h < 40 ){
            Hfile << ToString(h) << endl;
            Sfile << ToString(s) << endl;

        }

        /*
        if(i>1200){
            Hfile << ToString(h) << endl;
            Sfile << ToString(s) << endl;
        }
*/
        i++;
    }

    //close files for training data:
    Hfile.close();
    Sfile.close();

    Sat.close();
    Hue.close();

    return 0;
}
