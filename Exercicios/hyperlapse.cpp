/*
 * Hyperlapse
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
    char key;
    int cont=0;

    Mat prev_image, next_image, image, mask;
    vector<Point2f > corners[2];
    vector<uchar> status;
    vector<float> err;
    Size winSize(31,31);
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);

    Scalar cor = Scalar(0, 0, 250);

    VideoCapture cap("../pingo.mp4");
//    VideoWriter videoResult;
//    videoResult.open("videoResult.mp4", CV_FOURCC('M','J','P','G'), 15, Size(640,480)); - ERRO

    cap >> prev_image;
    resize(prev_image,prev_image,Size(480,640));
    image = prev_image.clone();
    cvtColor(prev_image, prev_image, CV_BGR2GRAY);

    goodFeaturesToTrack(prev_image, corners[0], 50, 0.01, 20, mask, 3, false, 0.04);

    while(1){
        if(!next_image.empty()) {
            // atualiza prev_image
            prev_image = next_image.clone();
        }
        // pega o segundo frame para comparaçao
        cap >> next_image;
        resize(next_image,next_image,Size(480,640));
        image = next_image.clone();
        cvtColor(next_image, next_image, CV_BGR2GRAY);

        calcOpticalFlowPyrLK(prev_image, next_image, corners[0], corners[1], status, err, winSize,3,
                             termcrit,0, 0.001);

        for (size_t i = 0; i < corners[1].size(); i++) {
            // pinta os pontos em image
            circle(image, corners[1][i], 3, cor, -1, 8);
            // atualiza os pontos
            corners[0][i] = corners[1][i];
        }
        corners[1].resize(0);

        imshow("window", image);

        key = (char) waitKey(10);
        if( key == 27 ) break; // esc pressed!

        cont++;
        if(cont==30){
            goodFeaturesToTrack(next_image, corners[0], 50, 0.01, 20, mask, 3, false, 0.04);
            cont = 0;
        }

    }

    return 0;
}