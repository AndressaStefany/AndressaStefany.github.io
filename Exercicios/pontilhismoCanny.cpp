#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;
// passo usado para varrer
// a imagem de referência
#define STEP 2

#define JITTER 2
#define RAIO 3

int main(int argc, char** argv){
    vector<int> yrange;
    vector<int> xrange;

    Mat image, frame, points, border, imageCanny, borrada;
    Vec3b gray;
    int width, height;
    int x, y;

    image= imread("../images/itapaje.jpg",CV_LOAD_IMAGE_COLOR);

    srand(time(0));

    if(!image.data){
        cout << "nao abriu" << argv[1] << endl;
        cout << argv[0] << " imagem.jpg";
        exit(0);
    }
    width=image.size().width;
    height=image.size().height;

    xrange.resize(height/STEP);
    yrange.resize(width/STEP);

    iota(xrange.begin(), xrange.end(), 0);
    iota(yrange.begin(), yrange.end(), 0);

    for(uint i=0; i<xrange.size(); i++){
        xrange[i]= xrange[i]*STEP+STEP/2;
    }

    for(uint i=0; i<yrange.size(); i++){
        yrange[i]= yrange[i]*STEP+STEP/2;
    }

    points = Mat(height, width, CV_8UC3, Scalar(255,255,255));
    //embaralha os elementos de xrange
    random_shuffle(xrange.begin(), xrange.end());

    for(auto i : xrange){
        //embaralha os elementos de yrange
        random_shuffle(yrange.begin(), yrange.end());
        for(auto j : yrange){
            // centro do circulo a
            // ser desenhado
            x = i+rand()%(2*JITTER)-JITTER+1;
            y = j+rand()%(2*JITTER)-JITTER+1;
            gray = image.at<Vec3b>(x,y);
            // traça um circulo
            circle(points,
                   cv::Point(y,x),
                   RAIO,
                   CV_RGB(gray[2],gray[1],gray[0]),
                   -1,
                   CV_AA);
        }
    }
    imwrite("semCanny.jpg", points);

    // convertendo a imagem para tons de cinza
    cvtColor(image, imageCanny, CV_BGR2GRAY);
    // borrando
    GaussianBlur(imageCanny, borrada, Size(3,3), 25, 25);
    // gerando a imagem canny
    Canny(borrada, border, 50, 3*50);
    imwrite("imageCanny.jpg", border);

    // percorrer toda a imagem e verificar
    // os pontos de borda
    for(int i=0; i<height; i++){
        for(int j=0; j<width; j++){
            gray = image.at<Vec3b>(i,j);
            if(border.at<uchar>(i,j)==255){
                circle(points,
                       cv::Point(j,i),
                       1,//raio 1 nas bordas
                       CV_RGB(gray[2],gray[1],gray[0]),
                       -1,
                       CV_AA);
            }
        }
    }
    imwrite("comCanny.jpg", points);

    return 0;
}
