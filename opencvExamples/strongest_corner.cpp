/*
 * Usa metodo de Shi-Tomasi para detecçao
 * de bordas usando cv::goodFeaturesToTrack()
 * https://docs.opencv.org/3.2.0/dd/d1a/group__imgproc__feature.html#ga1d6bb77486c8f92d79c8793ad995d541
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
    vector<Point2f > corners;
    Mat mask;

    Mat image;

    image = imread("../images/itapaje.jpg", CV_LOAD_IMAGE_GRAYSCALE);

    // image, corners, maxCorners, qualityLevel, minDistance, mask, blockSize, useHarrisDetector, k
    goodFeaturesToTrack( image, corners, 50, 0.01, 20, mask, 3, false, 0.04 );

    for( size_t i = 0; i < corners.size(); i++ )
    {// pinta os cantos de bordas forte
        // image, center, radius, color, thickness=1, lineType=8, shift=0
        circle( image, corners[i], 3, Scalar( 255. ), -1 );
    }
    // mostra
    imshow("window", image);

    waitKey(0);

    return 0;
}