#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>

using namespace cv;
using namespace std;

int alfa_slider = 1, alfa_slider_max = 100;
int h_slider, h_max;//altura
int t_slider, t_max;//tamanho da janela de foco

double tcima, tbaixo;

double tiltshift(int x, int a, int b, int c) {
    if(c>0)
        return 0.5*(tanh((x-a)/c)-tanh((x-b)/c));
}

void on_trackbar_blend(int, void*){
//    alfa = (double) alfa_slider/alfa_slider_max ;
//    addWeighted( image1, alfa, imageTop, 1-alfa, 0.0, blended);
//    imshow("addweighted", blended);
}

void on_trackbar(int, void*){
    tcima = h_slider-t_slider;//tamanho de cima
    tbaixo = h_slider+t_slider;//tamanho de cima
}

int main(int argvc, char** argv){
    Mat image, imageBor;

    image = imread("../images/cidade.jpg");
    imageBor = image.clone();
/*    imageBor = Mat::zeros(image.size(), CV_8UC3); //para teste*/

    Mat blended = Mat::zeros(image.size(), CV_8UC3);

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
    createTrackbar("Alpha:", "window", &alfa_slider, alfa_slider_max, on_trackbar_blend);
    on_trackbar_blend(alfa_slider, 0);

    while(1){
        //imagem resultante
        for(int i=0; i<image.size().height; i++)
            addWeighted(image.row(i),tiltshift(i,tcima,tbaixo,alfa_slider),
                        imageBor.row(i),1-tiltshift(i,tcima,tbaixo,alfa_slider),0,blended.row(i));

        imshow("window",blended);

        if(waitKey(30)==0) break;
    }

    return 0;
}
