#include <iostream>
#include <stack>
#include <list>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void myseedfill(Mat image, int cor, int searchcor, stack<CvPoint> pilha){
    CvPoint coordenada, aux;

    while(!pilha.empty()){
        coordenada = pilha.top();
        pilha.pop();

        //vizinho de esquerda
        if(coordenada.y-1>=0 && image.at<uchar>(coordenada.x,coordenada.y-1)==searchcor){
            aux.x=coordenada.x;
            aux.y=coordenada.y-1;
            pilha.push(aux);
        }
        //vizinho da direita
        if(coordenada.y+1<image.size().width && image.at<uchar>(coordenada.x,coordenada.y+1)==searchcor){
            aux.x=coordenada.x;
            aux.y=coordenada.y+1;
            pilha.push(aux);
        }
        //vizinho de baixo
        if(coordenada.x+1<image.size().height && image.at<uchar>(coordenada.x+1,coordenada.y)==searchcor){
            aux.x=coordenada.x+1;
            aux.y=coordenada.y;
            pilha.push(aux);
        }
        //vizinho de cima
        if(coordenada.x-1>=0 && image.at<uchar>(coordenada.x-1,coordenada.y)==searchcor){
            aux.x=coordenada.x-1;
            aux.y=coordenada.y;
            pilha.push(aux);
        }
        image.at<uchar>(coordenada.x,coordenada.y) = cor;

    }//fim do while
}

int main(int, char**){
    Mat image;
    stack<CvPoint> pilha;
    CvPoint coordenada;
    int cont = 50, buracos = 0, total, width, height;

    image = imread("../images/bolhas.png",CV_LOAD_IMAGE_GRAYSCALE);
    width = image.size().width;
    height = image.size().height;

    if(!image.data)
        cout << "nao abriu bolhas.png" << endl;

    namedWindow("janela1",WINDOW_AUTOSIZE);
    imshow("janela1", image);

    //verificando bolhas nas bordas
    for(int i=0;i<width;i++){
        if(image.at<uchar>(i,0)==255 || image.at<uchar>(i,height-1)==255 || image.at<uchar>(0,i)==255
           || image.at<uchar>(width-1,i)==255){

            if(image.at<uchar>(i,0)==255 || image.at<uchar>(i,height-1)==255) { //primeira e ultima coluna
                coordenada.x = i;
                coordenada.y = (image.at<uchar>(i, 0) == 255) ? 0 : height-1;
            }

            if(image.at<uchar>(0,i)==255 || image.at<uchar>(width-1,i)==255) { //primeira e ultima linha
                coordenada.x = (image.at<uchar>(0,i)==255)? 0 : width-1;
                coordenada.y = i;
            }

            pilha.push(coordenada);

            myseedfill(image, 0, 255, pilha);
        }
    }

    waitKey();
    namedWindow("janela2",WINDOW_AUTOSIZE);
    imshow("janela2", image);

    //pinta as bolhas do centro
    for(int l=0;l<height;l++){
        for(int c=0;c<width;c++){
            if(image.at<uchar>(l,c)==255){
                coordenada.x = l;
                coordenada.y = c;

                pilha.push(coordenada);

                cont++;

                myseedfill(image, 200, 255, pilha);
            }
        }
    }

    waitKey();
    namedWindow("janela3",WINDOW_AUTOSIZE);
    imshow("janela3", image);

    total = cont-50;

    //pinta o fundo
    coordenada.x = 0;
    coordenada.y = 0;

    pilha.push(coordenada);

    myseedfill(image, 255, 0, pilha);

    waitKey();
    namedWindow("janela4",WINDOW_AUTOSIZE);
    imshow("janela4", image);

    //conta as bolhas com buraco
    for(int l=0;l<height;l++){
        for(int c=0;c<width;c++){

            if(image.at<uchar>(l,c)==0) {//procura o preto

                coordenada.x = l;
                coordenada.y = c;

                pilha.push(coordenada);

                myseedfill(image, 50, 0, pilha);

                //eh uma nova bolha?
                if(image.at<uchar>(l, c-1) == 200){
                    buracos++;
                    myseedfill(image, 100, 0, pilha);
                }

            }
        }
    }


    cout<<endl<<endl<<"Tem o total de "<<total<<" bolhas."<<endl<<"Sendo "
        <<buracos<<" bolhas com buracos e "<<total-buracos<<" bolhas sem buracos."
        <<endl<<endl<<"FIM!"<<endl;
    waitKey();

    return 0;
}
