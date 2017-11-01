/*
 * Hyperlapse
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

float distancia(CvPoint pt1, CvPoint pt2) {
    int dx = pt2.x - pt1.x;
    int dy = pt2.y - pt1.y;
    return cvSqrt( (float)(dx*dx + dy*dy));
}

CvPoint deslocamento(CvPoint pt1, CvPoint pt2) {
    CvPoint dR;
    dR.x = (int) pt1.x - pt2.x;
    dR.y = (int) pt1.y - pt2.y;

    return dR;
}

CvPoint soma(CvPoint pt1, CvPoint pt2) {
    CvPoint dR;
    dR.x = (int) pt1.x + pt2.x;
    dR.y = (int) pt1.y + pt2.y;

    return dR;
}

CvPoint media(CvPoint pt, int tam) {
    CvPoint dR;
    dR.x = (int) pt.x / tam;
    dR.y = (int) pt.y / tam;

    return dR;
}

int main(int argc, char** argv){
    char key;
    int cont=0;
    CvPoint desl, mediaDesl;
    Rect teste;

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
        //nova imagem
        image_cut = next_image.clone();

        calcOpticalFlowPyrLK(prev_image, next_image, corners[0], corners[1], status, err, winSize,3,
                             termcrit,0, 0.001);

        desl.x = 0;
        desl.y = 0;

        for (size_t i = 0; i < corners[1].size(); i++) {
            //soma dos deslocamentos entre os pontos
            desl = soma(deslocamento(corners[0][i],corners[1][i]), desl);
            // pinta os pontos em image
            circle(image, corners[1][i], 3, cor, -1, 8);
            // atualiza os pontos
            corners[0][i] = corners[1][i];
        }
        //media do deslocamento
        mediaDesl = media(desl, corners[1].size());

        //ERROR AQUI
        teste =  Rect(corners[1][0].x - mediaDesl.x, corners[1][0].y - mediaDesl.y, 668,332);

        image_cut = image_cut(teste);

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