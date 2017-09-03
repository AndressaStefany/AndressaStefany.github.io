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

    image= imread("../images/pingo.png",CV_LOAD_IMAGE_GRAYSCALE);
    width = image.size().width;
    height = image.size().height;

    if(!image.data) {
        cout << "nao abriu pingo.png" << endl;
        return 0;
    }

    cout<<"Informe P1: ";
    cin >> x1 >> y1;

    pertence = confere(width,height,x1,y1);

    if(pertence) {
        p1.x = x1;
        p1.y = y1;
    } else {
        cout << "A coordenada não pertence à imagem!"<< endl;
        return 0;
    }

    cout<<endl<<"Informe P2: ";
    cin >> x1 >> y1;
    cout<<endl;

    pertence = confere(width,height,x1,y1);

    if(pertence) {
        p2.x = x1;
        p2.y = y1;
    } else {
        cout << "A coordenada não pertence à imagem!"<< endl;
        return 0;
    }

    imshow("janela1", image);

    for(int i=p1.x;i<p2.x;i++) {
        for (int j = p1.y; j<p2.y; j++)
            image.at<uchar>(i, j) = 255 - image.at<uchar>(i, j);

    }

    imshow("janela2", image);
    waitKey();

    return 0;
}