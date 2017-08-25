#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

bool confere(int width, int height, int x, int y){
    if(x>=0 && y>=0 && x<width && y<height)
        return true; // pertence
    else//Não pertence
        return false;
}

int main(int, char**){
    Mat image, negativeImage;
    int x1, y1, width, height;
    bool pertence;
    CvPoint p1, p2;

    image= imread("../biel.png",CV_LOAD_IMAGE_GRAYSCALE);
    width = image.size().width;
    height = image.size().height;

    if(!image.data) {
        cout << "nao abriu biel.png" << endl;
        return 0;
    }

    cout<<"Informe P1: "<<endl;
    cin >> x1 >> y1;

    pertence = confere(width,height,x1,y1);

    if(pertence) {
        p1.x = x1;
        p1.y = y1;
        cout<< "p1 " << p1.x <<p1.y<<endl<<endl;
    } else {
        cout << "A coordenada não pertence à imagem!"<< endl;
        return 0;
    }

    cout<<"Informe P2: "<<endl;
    cin >> x1 >> y1;

    pertence = confere(width,height,x1,y1);

    if(pertence) {
        p2.x = x1;
        p2.y = y1;
        cout<< "p2 " << p2.x <<p2.y<<endl<<endl;
    } else {
        cout << "A coordenada não pertence à imagem!"<< endl;
        return 0;
    }

    imshow("janela", image);
    waitKey();

    for(int i=p1.x;i<p2.x;i++) {
        for (int j = p1.y; j<p2.y; j++)
            image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);

    }

    imshow("janela", image);
    waitKey();

    return 0;
}