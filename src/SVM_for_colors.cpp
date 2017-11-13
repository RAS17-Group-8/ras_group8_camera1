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

    string hue_files[7] = {"/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/orange_H.dat",
                           "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/red_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/yellow_H.dat",
                           "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/purple_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/blue_smaller_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/dark_green_H.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/light_green_H.dat"};

    string sat_files[7] = {"/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/orange_S.dat",
                           "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/red_S.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/yellow_S.dat",
                           "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/purple_S.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/blue_smaller_S.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/dark_green_S.dat",
                          "/home/ras28/catkin_ws/src/ras_group8/ras_group8_camera1/pcl_data_for_color_classification/light_green_S.dat"};



    for (int color= 0; color < 7; ++color){
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
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, (int)1e5, 1e-5);
    params.gamma = 0.001;
    //params.degree = 3;


    // Data for visual representation
    int width = 360, height = 255;
    Mat image = Mat::zeros(height, width, CV_8UC3);

    // Train the SVM
    CvSVM SVM;
    SVM.train(trainingDataMat, labelsMat, Mat(), Mat(), params);
    Vec3b green(0,255,0), blue (255,0,0), red (0,0,255), orange (0, 100,200), yellow (0,255,255), purple (255, 100, 180), light_green(150,255,150);

    // Show the decision regions given by the SVM
    for (int i = 0; i < image.rows; ++i)
        for (int j = 0; j < image.cols; ++j)
        {
            Mat sampleMat = (Mat_<float>(1,2) << j,i);
            float response = SVM.predict(sampleMat);

            if (response == 0)
                image.at<Vec3b>(i,j)  = orange;
            else if (response == 1)
                 image.at<Vec3b>(i,j)  = red;
            else if (response == 2)
                 image.at<Vec3b>(i,j)  = yellow;
            else if (response == 3)
                 image.at<Vec3b>(i,j)  = purple;
            else if (response == 4)
                 image.at<Vec3b>(i,j)  = blue;
            else if (response == 5)
                 image.at<Vec3b>(i,j)  = green;
            else if (response == 6)
                 image.at<Vec3b>(i,j)  = light_green;



        }

    /*
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
