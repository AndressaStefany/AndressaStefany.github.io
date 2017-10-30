/*
 * Metodo do Lucas Kanade usando OpenCV
 */

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv){
    VideoCapture video;
    Mat cap1, cap2, mask, image;
    vector<Point2f > corners[2];
    vector<uchar> status;
    vector<float> err;
    Size winSize(31,31);
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);
    Scalar cor;
    char key;
    int width, height;

    video.open(0); // abre a conexao
    if(!video.isOpened()){
        cout << "cameras indisponiveis";
        return -1;
    }
    // pega primeiro frame
    video >> cap1;
    // image e a imagem que sera pintada
    // e exibida
    image = cap1.clone();
    // transforma cap1 para gray
    cvtColor(cap1, cap1, CV_BGR2GRAY);

    // cor dos pontos
    cor = Scalar(250, 0, 250);

    // encontra os pontos usando Shi-Tomasi
    // image, corners, maxCorners, qualityLevel, minDistance, mask, blockSize, useHarrisDetector, k
    goodFeaturesToTrack(cap1, corners[0], 50, 0.01, 20, mask, 3, false, 0.04);

    while(1) {
        // verifica se eh o primeiro loop
        if(!cap2.empty()) {//ver se precisa dessa largura e altura
            // pega largura e altura da imagem
            width = cap1.size().width;
            height = cap1.size().height;

            // zera cap1
            cap1 = Mat::zeros(height, width, CV_32F);
            // atualiza cp1
            cap1 = cap2.clone();
        }
        // pega o segundo frame
        // para comparaçao
        video >> cap2;

        // atualiza image
        image = cap2.clone();
        // converte para gray
        cvtColor(cap2, cap2, CV_BGR2GRAY);

        // acha os pontos usando Lucas Kanade
        // (prev_image, next_image, prev_ponts, next_ponts, status, erro, winSize,
        // maxLevel, criteria, flags)
        calcOpticalFlowPyrLK(cap1, cap2, corners[0], corners[1], status, err, winSize,3,
                             termcrit,0, 0.001);

        for (size_t i = 0; i < corners[1].size(); i++) {
            // pinta os pontos em image
            circle(image, corners[1][i], 3, cor, -1, 8);
            // atualiza os pontos
            corners[0][i] = corners[1][i];
        }
        // zera o tamanho de corners[1]
        corners[1].resize(0);
        // mostra image
        imshow("window", image);

        key = (char) waitKey(10);
        if( key == 27 ) break; // esc pressed!
    }
    return 0;
}