#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
    Mat imageAnt, image;
    int width, height;
    VideoCapture cap; //pegar as imagens
    vector<Mat> planes; //armazenar as componentes
    Mat histR, histRef; //guardar os histogramas
    int nbins = 64; //tamanho do vetor utilizado para armazenar os histogramas
    float range[] = {0, 256};
    const float *histrange = { range };
    bool uniform = true;
    bool acummulate = false;

    cap.open(0); //abre a conexao

    if(!cap.isOpened()){
        cout << "cameras indisponiveis";
        return -1;
    }

    width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    cout << "largura = " << width << endl;
    cout << "altura  = " << height << endl;

    int histw = nbins, histh = nbins/2;
    Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));

    while(1){
        cap >> image; //captura a imagem
        split (image, planes);//separa os planos

        //o histograma e calculado
        calcHist(&planes[0], 1, 0, Mat(), histR, 1,
                 &nbins, &histrange,
                 uniform, acummulate);
        //normalizaçao
        normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());

        //verifica histRef e compara os histogramas
        //utilizando o metodo Chi-Square
        if(histRef.data && compareHist(histR, histRef, CV_COMP_CHISQR)>100){
            cout<<"Ha algo estranho!"<<endl;
        }

        histImgR.setTo(Scalar(0));

        for(int i=0; i<nbins; i++){//gera o desenho o histograma
            line(histImgR,
                 Point(i, histh),
                 Point(i, histh-cvRound(histR.at<float>(i))),
                 Scalar(0, 0, 255), 1, 8, 0);
        }
        //imagen do histograma e copiada para a imagem
        histImgR.copyTo(image(Rect(0, 0,nbins, histh)));

        histRef = histR.clone(); //guarda o ultimo histograma

        imshow("image", image);
        if(waitKey(30) >= 0) break;
    }
    return 0;
}
