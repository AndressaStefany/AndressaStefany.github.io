#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;



void mostraHist(Mat image, bool equalize){
    vector<Mat> planes; //armazenar as componentes
    int nbins = 64; //tamanho do vetor utilizado para armazenar os histogramas
    Mat histR, histG, histB; //guardar os histogramas
    float range[] = {0, 256};
    const float *histrange = { range };
    bool uniform = true;
    bool acummulate = false;

    int histw = nbins, histh = nbins/2;
    Mat histImgR(histh, histw, CV_8UC3, Scalar(0,0,0));
    Mat histImgG(histh, histw, CV_8UC3, Scalar(0,0,0));
    Mat histImgB(histh, histw, CV_8UC3, Scalar(0,0,0));

    split (image, planes); //separa os planos
    if(equalize) //equaliza
    {
        equalizeHist(planes[0], planes[0]);
        equalizeHist(planes[1], planes[1]);
        equalizeHist(planes[2], planes[2]);

        merge(planes, image);//junta os planos equalizados
    }
    //os histogramas são calculados
    calcHist(&planes[0], 1, 0, Mat(), histR, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[1], 1, 0, Mat(), histG, 1,
             &nbins, &histrange,
             uniform, acummulate);
    calcHist(&planes[2], 1, 0, Mat(), histB, 1,
             &nbins, &histrange,
             uniform, acummulate);
    //normalizaçao
    normalize(histR, histR, 0, histImgR.rows, NORM_MINMAX, -1, Mat());
    normalize(histG, histG, 0, histImgG.rows, NORM_MINMAX, -1, Mat());
    normalize(histB, histB, 0, histImgB.rows, NORM_MINMAX, -1, Mat());

    histImgR.setTo(Scalar(0));
    histImgG.setTo(Scalar(0));
    histImgB.setTo(Scalar(0));

    for(int i=0; i<nbins; i++){//gera os desenhos os histogramas
        line(histImgR,
             Point(i, histh),
             Point(i, histh-cvRound(histR.at<float>(i))),
             Scalar(0, 0, 255), 1, 8, 0);
        line(histImgG,
             Point(i, histh),
             Point(i, histh-cvRound(histG.at<float>(i))),
             Scalar(0, 255, 0), 1, 8, 0);
        line(histImgB,
             Point(i, histh),
             Point(i, histh-cvRound(histB.at<float>(i))),
             Scalar(255, 0, 0), 1, 8, 0);
    }
    //imagens dos histogramas são copiadas para a imagem
    histImgR.copyTo(image(Rect(0, 0       ,nbins, histh)));
    histImgG.copyTo(image(Rect(0, histh   ,nbins, histh)));
    histImgB.copyTo(image(Rect(0, 2*histh ,nbins, histh)));
}

int main(int argc, char** argv){
    Mat image;
    int width, height;
    VideoCapture cap; //pegar as imagens

    cap.open(0);//abre a conexao

    if(!cap.isOpened()){
        cout << "cameras indisponiveis";
        return -1;
    }

    width  = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    cout << "largura = " << width << endl;
    cout << "altura  = " << height << endl;

    while(1){
        cap >> image;//captura a imagem

        mostraHist(image, false);

        imshow("image", image);

        mostraHist(image, true);

        imshow("imageEqualized", image);

        if(waitKey(30) >= 0) break;
    }
    return 0;
}
