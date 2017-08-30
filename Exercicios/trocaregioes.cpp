#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
    Mat image, otherImage;
    int width, height, aux1, aux2;

    image= imread("biel.png",CV_LOAD_IMAGE_GRAYSCALE);
    width = image.size().width;
    height = image.size().height;

    if(!image.data) {
        cout << "nao abriu biel.png" << endl;
        return 0;
    }

    imshow("janela", image);
    waitKey();

    for(int i = 0;i<height/2;i++) {
        for (int j = 0; j<width/2; j++) {
            aux1 = image.at<uchar>(height/2 + i, width/2 + j);
            aux2 = image.at<uchar>(i, j);
            image.at<uchar>(i, j) = aux1; // segundo quadrante
            image.at<uchar>(height/2 + i, width/2 + j) = aux2; //quarto quadrante

            aux1 = image.at<uchar>(height/2 + i, j);
            aux2 = image.at<uchar>(i, width/2 + j);
            image.at<uchar>(i, width/2 + j) = aux1; // primeiro quadrante
            image.at<uchar>(height/2 + i, j) = aux2; // terceiro quadrante
        }
    }

    imshow("janela", image);
    waitKey();

    return 0;
}