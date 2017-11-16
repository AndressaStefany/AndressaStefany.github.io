/*
 * Hough Lines
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    VideoCapture cap("../videoCurto2.mp4");
    Mat image1, image2, imageCanny1, imageCanny2, otherImage1, otherImage2;
    vector<Vec2f> lines1, lines2;

    cap>>image1;
    cap>>image2;
    cap>>image2;
    cap>>image2;
    cap>>image2;
    cap>>image2;
    cap>>image2;
    cap>>image2;
    cap>>image2;
//    Mat image = imread("../images/resultTfoto.png");
    resize(image1, image1, Size(640,480));
    resize(image2, image2, Size(640,480));

//    imshow("imagem1", image1);
//    imshow("imagem2", image2);

    Canny(image1, imageCanny1, 50, 200, 3);
    Canny(image2, imageCanny2, 50, 200, 3);
    cvtColor(imageCanny1, otherImage1, CV_GRAY2BGR);
    cvtColor(imageCanny2, otherImage2, CV_GRAY2BGR);

    HoughLines(imageCanny1, lines1, 1, CV_PI/180, 150, 0, 0 );
    HoughLines(imageCanny2, lines2, 1, CV_PI/180, 150, 0, 0 );

    // draw lines
    for( size_t i = 0; i < lines1.size(); i++ )
    {
        float rho = lines1[i][0], theta = lines1[i][1];
        cout<<"rho: "<<rho<<" theta: "<<theta<<endl;
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        line( otherImage1, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
    }
//    for( size_t i = 0; i < lines2.size(); i++ )
//    {
//        float rho = lines2[i][0], theta = lines2[i][1];
//        Point pt1, pt2;
//        double a = cos(theta), b = sin(theta);
//        double x0 = a*rho, y0 = b*rho;
//        pt1.x = cvRound(x0 + 1000*(-b));
//        pt1.y = cvRound(y0 + 1000*(a));
//        pt2.x = cvRound(x0 - 1000*(-b));
//        pt2.y = cvRound(y0 - 1000*(a));
//        line( otherImage2, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
//    }

    imshow("imagem21", otherImage1);
//    imshow("imagem22", otherImage2);

    waitKey();

    return 0;
}
