#include <iostream>
#include <stack>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int, char**){
    Mat image;
    stack<CvPoint> pilha;
    CvPoint coordenada, aux;

    image = imread("bolhas.png",CV_LOAD_IMAGE_GRAYSCALE);
    //imshow("image", image);

    if(!image.data)
        cout << "nao abriu bolhas.png" << endl;

    namedWindow("janela",WINDOW_AUTOSIZE);
    imshow("janela", image);
    waitKey();
    for(int i=0;i<256;i++){
        //verificando bolhas nas bordas
        if(image.at<uchar>(i,0)==255 || image.at<uchar>(i,255)==255 || image.at<uchar>(0,i)==255 || image.at<uchar>(255,i)==255){
            if(image.at<uchar>(i,0)==255 || image.at<uchar>(i,255)==255) { //primeira e ultima coluna
                coordenada.x = i;
                coordenada.y = (image.at<uchar>(i, 0) == 255) ? 0 : 255;
            }

            if(image.at<uchar>(0,i)==255 || image.at<uchar>(255,i)==255) { //primeira e ultima linha
                coordenada.x = (image.at<uchar>(0,i)==255)? 0:255;
                coordenada.y = i;
            }

            pilha.push(coordenada);

            while(!pilha.empty()){
                coordenada = pilha.top();
                pilha.pop();

                if(image.at<uchar>(coordenada.x,coordenada.y)==255){
                    //vizinho de esquerda
                    if(coordenada.y-1>=0 && image.at<uchar>(coordenada.x,coordenada.y-1)==255){
                        aux.x=coordenada.x;
                        aux.y=coordenada.y-1;
                        pilha.push(aux);
                    }
                    //vizinho da direita
                    if(coordenada.y+1<=255 && image.at<uchar>(coordenada.x,coordenada.y+1)==255){
                        aux.x=coordenada.x;
                        aux.y=coordenada.y+1;
                        pilha.push(aux);
                    }
                    //vizinho de baixo
                    if(coordenada.x+1<=255 && image.at<uchar>(coordenada.x+1,coordenada.y)==255){
                        aux.x=coordenada.x+1;
                        aux.y=coordenada.y;
                        pilha.push(aux);
                    }
                    //vizinho de cima
                    if(coordenada.x-1>=0 && image.at<uchar>(coordenada.x-1,coordenada.y)==255){
                        aux.x=coordenada.x-1;
                        aux.y=coordenada.y;
                        pilha.push(aux);
                    }
                    image.at<uchar>(coordenada.x,coordenada.y)=0;
                }
            }//fim do while
            cout<<"Aperte uma tecla!"<<endl;
            waitKey();
            imshow("janela", image);
        }
    }
    cout<<"FIM!"<<endl;
    waitKey();

    return 0;
}
