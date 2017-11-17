#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

//to read and write file:
#include <fstream>
#include <string>

#include <iostream>
#include <iomanip>
#include <ros/console.h>

#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
//#include <array>
using namespace std;

using namespace cv;



int main() {
    int sum = 0;
    float h;
    float s;
    int i=0;
    ifstream Hue;
    ifstream Sat;
    vector< vector<float> > vec;
    vector<float> row;
    vector<float> label_vec;
    vector<int> number_of_element;
    vector<int> number_of_element_total;
    int length_of_dataset = 0;
    int number_of_colors = 9;
    //Mat result_matrix=Mat::zeros(255, 360, CV_8U);
    //std::array<std::array<int, 360>, 255> result_matrix;

    int** result_matrix = new int*[255];
    //int result_matrix[255][360];

/*
    string hue_files[8] = {"/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/orange_H.dat",
                           "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/red_H.dat",
                          //"/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/yellow_H.dat",
                           "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/purple_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/blue_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/dark_green_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/light_green_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/battery_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/trap_H.dat"};

    string sat_files[8] = {"/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/orange_S.dat",
                           "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/red_S.dat",
                          //"/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/yellow_S.dat",
                           "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/purple_S.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/blue_S.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/dark_green_S.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/light_green_S.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/battery_S.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/test_data/trap_S.dat"};
*/

    string hue_files[9] = {"/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/orange_H.dat",
                           "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/red_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/yellow_H.dat",
                           "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/purple_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/blue_smaller_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/dark_green_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/light_green_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/battery_smaller_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/trap_H.dat"};

    string sat_files[9] = {"/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/orange_S.dat",
                           "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/red_S.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/yellow_S.dat",
                           "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/purple_S.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/blue_smaller_S.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/dark_green_S.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/light_green_S.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/battery_smaller_S.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/trap_S.dat"};


    for (int color= 0; color < number_of_colors; ++color){
        Hue.open(hue_files[color].c_str());
        Sat.open(sat_files[color].c_str());

        if (!Hue || !Sat) {
            cout << "Unable to open file";
            exit(1); // terminate with error
        }

        while (Hue >> h) {

            label_vec.push_back(color);
            //sum = sum + h;
            //vec[i][0]=h;
            Sat >> s;
            //vec[i][1] = s;
            row.push_back(h);
            row.push_back(s*255);
            vec.push_back(row);
            row.clear();
            //cout<< vec[i] << endl;
            //cout<< vec[i][0] << " " << vec[i][1] << endl;
            number_of_element.push_back(i);
            i++;

        }

        Sat.close();
        Hue.close();
        //cout << "Sum = " << sum << endl;

        number_of_element_total.insert(number_of_element_total.end(), number_of_element.begin(), number_of_element.end());
        length_of_dataset += vec.size();
        cout << vec.size()<< endl;

    }


    ifstream resultFile;
    resultFile.open("/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/classifiers/result_matrix.dat");

    ROS_INFO("A");
    for(int lin=0;lin<255;lin++)
    {
        result_matrix[lin] = new int[360];

        for(int col=0;col<360;col++)
        {
            //resultFile >> result_matrix.at<int>(lin,col) ;
             //resultFile >> result_matrix[lin][col];//result_matrix.at<int>(lin,col) ;
            resultFile >> result_matrix[lin][col];
            cout << result_matrix[lin][col];//result_matrix.at<int>(0,1);
            //cout <<result_matrix.at<int>(0,1) << " ";
        }

        cout << endl;
    }
    resultFile.close();
    ROS_INFO("B");


    //cout << result_matrix.at<int>(0,1) << endl;



    ROS_INFO("C");
    float trainingData[length_of_dataset][2];
    float labels [length_of_dataset];
    //cout<< "length of dataset  " << length_of_dataset << endl;
    //cout<< "number of element size  " << number_of_element.size() << endl;
    //cout<< "number of element total size  " << number_of_element_total.size() << endl;

    int correct_count_orange = 0;
    int itr_orange = 0;

    int correct_count = 0;
    int itr = 0;
    for (i=0; i<length_of_dataset; ++i){


        //if (label_vec[i] == 0){
            //correct_count_orange += (result_matrix.at<int>((int)trainingData[i][1],(int)trainingData[i][0]) == label_vec[i] );
          //  itr_orange++;
        //}

        //correct_count += (result_matrix.at<int>((int)trainingData[i][1],(int)trainingData[i][0]) == label_vec[i] );
        correct_count += (result_matrix[(int)vec[i][1]][(int)vec[i][0]] == label_vec[i] );

        itr;

    }
    cout << "test error: " << correct_count / itr << endl;

    return 0;
}
