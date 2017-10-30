/*
 * Hyperlapse
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
    char key;
    int cont=0,normal[2];
    double sumX, sumY;

    Mat prev_image, next_image, image, mask, image_cut;
    vector<Point2f> corners[2];
    vector<uchar> status;
    vector<float> err;
    Size winSize(31,31);
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);

//    VideoWriter videoResult2;
//    videoResult2.open("videoResult2.avi", CV_FOURCC('M','J','P','G'), 15, Size(768,432));

    Scalar cor = Scalar(0, 255, 0);

    VideoCapture cap("../videoCurto2.mp4");

    cap >> prev_image;
    resize(prev_image,prev_image,Size(768,432));
    image = prev_image.clone();
    cvtColor(prev_image, prev_image, CV_BGR2GRAY);

    //nova imagem
//    image_cut = next_image.clone();
//    resize(image_cut,image_cut,Size(758,422));
//    cvtColor(image_cut, image_cut, CV_BGR2GRAY);

    goodFeaturesToTrack(prev_image, corners[0], 50, 0.01, 20, mask, 3, false, 0.04);

    while(1){
        if(!next_image.empty()) {
            // atualiza prev_image
            prev_image = next_image.clone();
        }
        // pega o segundo frame para comparaçao
        cap >> next_image;
        if (next_image.empty())
            break;
        cap >> next_image;
        if (next_image.empty())
            break;
        cap >> next_image;
        if (next_image.empty())
            break;

        resize(next_image,next_image,Size(768,432));
        image = next_image.clone();
        cvtColor(next_image, next_image, CV_BGR2GRAY);

        calcOpticalFlowPyrLK(prev_image, next_image, corners[0], corners[1], status, err, winSize,3,
                             termcrit,0, 0.001);

        sumX = 0;
        sumY = 0;
//        circle(image, Point2f(0,0), 3, Scalar(0, 255, 0), -1, 8);
//        cout<<corners[0].size()<<"    "<<corners[1].size()<<endl;
        for (size_t i = 0; i < corners[1].size(); i++) {
            //media da distancia entre os pontos
            sumX += (abs(corners[0][i].x-corners[1][i].x)/2);
            sumY += (abs(corners[0][i].y-corners[1][i].y)/2);

            // pinta os pontos em image
            circle(image, corners[1][i], 3, cor, -1, 8);
            // atualiza os pontos
            corners[0][i] = corners[1][i];

        }
        //verificar os pontos perdidos (?)
        normal[0] = (int) sumX/corners[0].size();
        normal[1] = (int) sumY/corners[0].size();

//        cout<<normal[0]<<" , "<<normal[1]<<endl;

        corners[1].resize(0);

        imshow("window", image);

//        videoResult2 << image;

        key = (char) waitKey(10);
        if( key == 27 ) break; // esc pressed!

        cont++;
        if(cont==100){// acha novos pontos de bordas
            goodFeaturesToTrack(next_image, corners[0], 50, 0.01, 20, mask, 3, false, 0.04);
            cont = 0;
        }

    }
//    videoResult2.release();
//    imwrite("blended.jpg",blended);

    return 0;
}