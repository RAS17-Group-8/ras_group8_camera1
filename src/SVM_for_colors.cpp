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
    Mat result_matrix=Mat::zeros(255, 360, CV_8U);

    string hue_files[9] = {"/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/orange_H.dat",
                           "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/red_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/yellow_H.dat",
                           "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/purple_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/blue_smaller_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/dark_green_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/light_green_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/battery_smaller_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/trap_H.dat"  };

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

    float trainingData[length_of_dataset][2];
    float labels [length_of_dataset];
    //cout<< "length of dataset  " << length_of_dataset << endl;
    //cout<< "number of element size  " << number_of_element.size() << endl;
    //cout<< "number of element total size  " << number_of_element_total.size() << endl;

    random_shuffle(number_of_element_total.begin(), number_of_element_total.end());

    for (i=0; i<length_of_dataset; ++i){
        trainingData[i][0] = vec[number_of_element_total[i]][0];
        trainingData[i][1] = vec[number_of_element_total[i]][1];
        labels[i] = label_vec[number_of_element_total[i]];

        //cout<< trainingData[i][0] << " " << trainingData[i][1] << endl;
    }


    Mat labelsMat(length_of_dataset, 1, CV_32FC1, labels);
    Mat trainingDataMat(length_of_dataset, 2, CV_32FC1, trainingData);


    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.C           = 1;
    params.kernel_type = CvSVM::RBF;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, (int)10, 1e-5);
    params.gamma = 0.001;
    //params.degree = 3;


    // Data for visual representation
    int width = 360, height = 255;
    Mat image = Mat::zeros(height, width, CV_8UC3);

    // Train the SVM
    CvSVM SVM;
    SVM.train(trainingDataMat, labelsMat, Mat(), Mat(), params);
    Vec3b green(0,255,0), blue (255,0,0), red (0,0,255), orange (0, 100,200), yellow (0,255,255), purple (255, 100, 180), light_green(150,255,150), battery(190,190,190), trap(120,120,120);


    cout <<"imrows: " << image.rows << endl;
    cout <<"imcols: " << image.cols << endl;

/*
    // Show the decision regions given by the SVM
    for (int i = 0; i < image.rows; i++)
        for (int j = 0; j < image.cols; j++)
        {
            Mat sampleMat = (Mat_<float>(1,2) << j,i);
            int response = SVM.predict(sampleMat);

            result_matrix.at<int>(i,j) = response;
            if (response == 0){
                image.at<Vec3b>(i,j)  = orange;
                //result_matrix.at<int>(i,j) = 0;
               }
            else if (response == 1){
                 image.at<Vec3b>(i,j)  = red;
                 //result_matrix.at<int>(i,j) = 1;
                }
            else if (response == 2){
                 image.at<Vec3b>(i,j)  = yellow;
                 //result_matrix.at<int>(i,j) = 2;
                }
            else if (response == 3){
                 image.at<Vec3b>(i,j)  = purple;
                 //result_matrix.at<int>(i,j) = 3;
                }
            else if (response == 4){
                 image.at<Vec3b>(i,j)  = blue;
                 //result_matrix.at<int>(i,j) = 4;
                }
            else if (response == 5){
                 image.at<Vec3b>(i,j)  = green;
                 //result_matrix.at<int>(i,j) = 5;
                }
            else if (response == 6){
                 image.at<Vec3b>(i,j)  = light_green;
                 //result_matrix.at<int>(i,j) = 6;
                }
            /*
            else if (response == 7){
                 image.at<Vec3b>(i,j)  = battery;
                 //result_matrix.at<int>(i,j) = 7;
                }
            else if (response == 8){
                 image.at<Vec3b>(i,j)  = trap;
                 //result_matrix.at<int>(i,j) = 8;
                }*/

     //   }




    ofstream resultFile;
    resultFile.open("/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/classifiers/result_matrix.dat");

    ROS_INFO("A");
    for(int lin=0;lin<255;lin++)
    {
        for(int col=0;col<360;col++)
        {
             resultFile << result_matrix.at<int>(lin,col) << " " ;
        }
        resultFile << std::endl ;
    }
    resultFile.close();
    ROS_INFO("B");




    // Show the training data
    int thickness = -1;
    int lineType = 8;
   //int trainingData_int =  trainingData.astype(int);
    for (i=0; i<length_of_dataset; ++i){

        //circle( image, Point((int)trainingData[i][0],  (int)trainingData[i][1]), 1, Scalar(  0,   0,   0), 0.1, lineType);

        if (labels[i] == 0)
            circle( image, Point((int)trainingData[i][0],  (int)trainingData[i][1]), 1, (Scalar)orange, thickness, lineType);
        else if (labels[i] == 1)
            circle( image, Point((int)trainingData[i][0],  (int)trainingData[i][1]), 1, (Scalar)red, thickness, lineType);
        else if (labels[i] == 2)
            circle( image, Point((int)trainingData[i][0],  (int)trainingData[i][1]), 1, (Scalar)yellow, thickness, lineType);
        else if (labels[i] == 3)
            circle( image, Point((int)trainingData[i][0],  (int)trainingData[i][1]), 1, (Scalar)purple, thickness, lineType);
        else if (labels[i] == 4)
            circle( image, Point((int)trainingData[i][0],  (int)trainingData[i][1]), 1, (Scalar)blue, thickness, lineType);
        else if (labels[i] == 5)
            circle( image, Point((int)trainingData[i][0],  (int)trainingData[i][1]), 1, (Scalar)green, thickness, lineType);
        else if (labels[i] == 6)
            circle( image, Point((int)trainingData[i][0],  (int)trainingData[i][1]), 1, (Scalar)light_green, thickness, lineType);
        else if (labels[i] == 7)
            circle( image, Point((int)trainingData[i][0],  (int)trainingData[i][1]), 1, (Scalar)battery, thickness, lineType);
        else if (labels[i] == 8)
            circle( image, Point((int)trainingData[i][0],  (int)trainingData[i][1]), 1, (Scalar)trap, thickness, lineType);

    }


    //For finding region to cut out from decision:
    Vec3b white(255,255,255);

    int orange_sat_limit = 0.7*255;
    int orange_hue_limit = 90;

    int green_sat_limit = 0.5*255;
    int green_hue_limit = 210;


    for(int hue = 0; hue< orange_hue_limit; hue++){
        image.at<Vec3b>(orange_sat_limit,hue)  = white;
    }
    for(int hue = orange_hue_limit; hue< green_hue_limit; hue++){
        image.at<Vec3b>(green_sat_limit,hue)  = white;
    }
    for(int sat = green_sat_limit; sat< orange_sat_limit; sat++){
        image.at<Vec3b>(sat,orange_hue_limit)  = white;
    }
    for(int sat = 0; sat< green_sat_limit; sat++){
        image.at<Vec3b>(sat,green_hue_limit)  = white;
    }



    /*circle( image, Point(501,  10), 5, Scalar(  0,   0,   0), thickness, lineType);
    circle( image, Point(255,  10), 5, Scalar(255, 255, 255), thickness, lineType);
    circle( image, Point(501, 255), 5, Scalar(255, 255, 255), thickness, lineType);
    circle( image, Point( 10, 501), 5, Scalar(255, 255, 255), thickness, lineType);

    // Show support vectors
    thickness = 2;
    lineType  = 8;
    int c     = SVM.get_support_vector_count();

    for (int i = 0; i < c; ++i)
    {
        const float* v = SVM.get_support_vector(i);
        circle( image,  Point( (int) v[0], (int) v[1]),   6,  Scalar(128, 128, 128), thickness, lineType);
    }
    */

    imwrite("/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/result.png", image);        // save the image

    imshow("SVM Simple Example", image); // show it to the user


    return 0;
}

/*
int main()
{
    // Data for visual representation
    int width = 512, height = 512;
    Mat image = Mat::zeros(height, width, CV_8UC3);

    // Set up training data
    float labels[4] = {1.0, -1.0, -1.0, -1.0};
    Mat labelsMat(4, 1, CV_32FC1, labels);

    float trainingData[4][2] = { {501, 10}, {255, 10}, {501, 255}, {10, 501} };
    Mat trainingDataMat(4, 2, CV_32FC1, trainingData);

    // Set up SVM's parameters
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::LINEAR;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

    // Train the SVM
    CvSVM SVM;
    SVM.train(trainingDataMat, labelsMat, Mat(), Mat(), params);

    Vec3b green(0,255,0), blue (255,0,0);
    // Show the decision regions given by the SVM
    for (int i = 0; i < image.rows; ++i)
        for (int j = 0; j < image.cols; ++j)
        {
            Mat sampleMat = (Mat_<float>(1,2) << j,i);
            float response = SVM.predict(sampleMat);

            if (response == 1)
                image.at<Vec3b>(i,j)  = green;
            else if (response == -1)
                 image.at<Vec3b>(i,j)  = blue;
        }

    // Show the training data
    int thickness = -1;
    int lineType = 8;
    circle( image, Point(501,  10), 5, Scalar(  0,   0,   0), thickness, lineType);
    circle( image, Point(255,  10), 5, Scalar(255, 255, 255), thickness, lineType);
    circle( image, Point(501, 255), 5, Scalar(255, 255, 255), thickness, lineType);
    circle( image, Point( 10, 501), 5, Scalar(255, 255, 255), thickness, lineType);

    // Show support vectors
    thickness = 2;
    lineType  = 8;
    int c     = SVM.get_support_vector_count();

    for (int i = 0; i < c; ++i)
    {
        const float* v = SVM.get_support_vector(i);
        circle( image,  Point( (int) v[0], (int) v[1]),   6,  Scalar(128, 128, 128), thickness, lineType);
    }

    imwrite("result.png", image);        // save the image

    imshow("SVM Simple Example", image); // show it to the user
    waitKey(0);

}*/
