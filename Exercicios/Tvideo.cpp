#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

int alfa_slider = 25, alfa_slider_max = 100;// default 25
int h_slider = 257, h_max;//altura, default 257
int t_slider = 122, t_max;//tamanho da janela de foco, default 122

double tcima, tbaixo;

double tiltshift(int x, double a, double b, double c) {
    if(c>0)
        return (double)0.5*(tanh((x-a)/c)-tanh((x-b)/c));
    else
        return 0;
}

void on_trackbar(int, void*){
    tcima = h_slider-t_slider;//tamanho de cima
    tbaixo = h_slider+t_slider;//tamanho de cima
}

int main(int argvc, char** argv){
    Mat image, imageBor, blended, imageHSV;
    VideoCapture cap("../images/video.avi");
    VideoWriter videoResult;
    videoResult.open("videoResult.avi", CV_FOURCC('M','J','P','G'), 15, Size(640,480));

    cap >> image;//frame
    resize(image,image,Size(640,480));
    imageBor = image.clone();
//    imageBor = Mat::zeros(image.size(), CV_8UC3); //para teste

    //Aplicando o filtro gaussiano
    for(int i=0; i<10; i++)
        GaussianBlur(imageBor, imageBor, Size(3,3),0,0);

    namedWindow("window", 1);
    //altura maxima
    h_max = image.size().height;
    createTrackbar("Altura:", "window", &h_slider, h_max, on_trackbar);
    on_trackbar(h_slider, 0);

    //tamanho maximo da janela de foco
    t_max = image.size().height;
    createTrackbar("Largura:", "window", &t_slider, t_max, on_trackbar);
    on_trackbar(t_slider, 0);

    //Nivel do desfoque
    createTrackbar("Alpha:", "window", &alfa_slider, alfa_slider_max);

    while (1){

        //convertendo para HSV
        cvtColor(image, imageHSV, CV_BGR2HSV);
        vector<Mat> planes;
        split(imageHSV, planes);

        planes[1]*=3;//mudando saturaçao

        merge(planes, imageHSV);//juntando os planos
        cvtColor(imageHSV,image,CV_HSV2BGR);

        imageBor = image.clone();
        blended = Mat::zeros(image.size(), CV_8UC3);

        //Aplicando o filtro gaussiano
        for(int i=0; i<10; i++)
            GaussianBlur(imageBor, imageBor, Size(3,3),0,0);

        //imagem resultante
        for(int i=0; i<image.size().height; i++)
            addWeighted(image.row(i),tiltshift(i,tcima,tbaixo,alfa_slider),
                        imageBor.row(i),1-tiltshift(i,tcima,tbaixo,alfa_slider),0,blended.row(i));

        videoResult << blended;
        imshow("window",blended);

        for(int i=0; i<=4; i++)
            cap >> image;

        if(image.empty())
            break;
        resize(image,image,Size(640,480));

        if(waitKey(30)==27) break;
    }
    videoResult.release();
    imwrite("blended.jpg",blended);

    return 0;
}