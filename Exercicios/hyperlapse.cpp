/*
 * Hyperlapse
 */

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

float distancia(CvPoint pt1, CvPoint pt2) {
    int dx = pt2.x - pt1.x;
    int dy = pt2.y - pt1.y;
    return cvSqrt( (float)(dx*dx + dy*dy));
}

CvPoint deslocamento(CvPoint pt1, CvPoint pt2) {
    CvPoint dR;
    dR.x = (int) pt2.x - pt1.x;
    dR.y = (int) pt2.y - pt1.y;

    return dR;
}

CvPoint soma(CvPoint pt1, CvPoint pt2) {
    CvPoint dR;
    dR.x = (int) pt1.x + pt2.x;
    dR.y = (int) pt1.y + pt2.y;

    return dR;
}

CvPoint media(CvPoint pt, int tam) {
    CvPoint dR;
    dR.x = (int) pt.x / tam;
    dR.y = (int) pt.y / tam;

    return dR;
}

int main(int argc, char** argv){
    char key;
    // tamY e tamX eh o tamanho da imagem
    int cont = 0, pad = 40, tamY = 768, tamX = 432;
    // deslocamento e media de deslocamento
    CvPoint desl, mediaDesl;

    CvPoint pt1, pt2;

    // prev_image eh a imagem anterior
    // next_image a imagem seguinte a ser comparada
    // image eh a imagem a ser pintada com os pontos
    // mask eh usada em goodFeaturesToTrack
    // image_cut imagem cortada
    Mat prev_image, next_image, image, mask, image_cut, teste;
    // vetor que guarda as bordas
    vector<Point2f> corners[2], greatCorners[2];
    // status, err, winSize e termcrit usados em calcOpticalFlowPyrLK
    vector<uchar> status;
    vector<float> err;
    Size winSize(31,31);
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);

//    VideoWriter videoResult2;
//    videoResult2.open("videoResult2.avi", CV_FOURCC('M','J','P','G'), 15, Size(tamY,tamX));

    // cor dos pontos
    Scalar cor = Scalar(0, 255, 0);

    VideoCapture cap("../videoCurto2.mp4");
    // pega primeiro frame
    cap >> prev_image;
    resize(prev_image,prev_image,Size(tamY,tamX));

    image = prev_image.clone();

    // converte prev_image para gray color
    cvtColor(prev_image, prev_image, CV_BGR2GRAY);

    // corta a imagem com o pad especificado
    prev_image = prev_image(Rect(pad, pad, tamY-pad*2, tamX-pad*2)).clone();


    while(1){

        if(!next_image.empty()) {
            // atualiza prev_image
            prev_image = next_image.clone();
        }

        // pega pontos de bordas fortes
        // metodo de Shi-Tomasi
        goodFeaturesToTrack(prev_image, corners[0], 300, 0.01, 20, mask, 3, false, 0.04);

        // pega o segundo frame para comparaçao
        // capturar 3 frames acelera o video
        cap >> next_image;
        if (next_image.empty())
            break;
//        cap >> next_image;
//        if (next_image.empty())
//            break;
//        cap >> next_image;
//        if (next_image.empty())
//            break;

        resize(next_image,next_image,Size(tamY,tamX));

        image = next_image.clone();
        image_cut = next_image.clone();
        // corta a imagem com o pad especificado
        next_image = next_image(Rect(pad, pad, tamY-pad*2, tamX-pad*2)).clone();
        // converte next_image para gray color
        cvtColor(next_image, next_image, CV_BGR2GRAY);

        // metodo do Lucas Kanade
        calcOpticalFlowPyrLK(prev_image, next_image, corners[0], corners[1], status, err,
                             winSize, 3, termcrit,0, 0.001);

        // tirando os ruins
        for(size_t i=0; i < status.size(); i++) {
            if(status[i]) {
                greatCorners[0].push_back(corners[0][i]);
                greatCorners[1].push_back(corners[1][i]);
            }
        }
        corners[0].clear();
        corners[1].clear();

        // garante que o desl eh inicialmente zero
        desl.x = 0;
        desl.y = 0;

        for (size_t i = 0; i < greatCorners[1].size(); i++) {
            //soma dos deslocamentos entre os pontos
            desl = soma(deslocamento(greatCorners[0][i],greatCorners[1][i]), desl);
            // pinta os pontos em image
//            circle(image, greatCorners[0][i], 3, cor, -1, 8);
//            circle(image, greatCorners[1][i], 3, Scalar(255, 0, 0), -1, 8);
            // atualiza os pontos
//            greatCorners[0][i] = greatCorners[1][i];
        }
        //media do deslocamento
        mediaDesl = media(desl, greatCorners[1].size());
        cout<<mediaDesl.x<<" , "<<mediaDesl.y<<endl;

        if(mediaDesl.x>pad)
            mediaDesl.x = pad;

        if(mediaDesl.y>pad)
            mediaDesl.y = pad;

        cout<<"pad - mediaDesY: "<<pad - mediaDesl.y<<"    pad - mediaDesX: "<<pad - mediaDesl.x<<endl;
        cout<<"largura: "<<tamY-pad*2<<"    altura: "<<tamX-pad*2<<endl<<endl;

//        teste = image_cut(Rect(pad - mediaDesl.y,pad - mediaDesl.x, tamY-pad*2, tamX-pad*2)).clone();
        teste = image_cut.clone();
        pt1.x = pad-mediaDesl.y;
        pt1.y = pad-mediaDesl.x;

        pt2.x = pad-mediaDesl.y+tamY-pad*2;
        pt2.y = pad-mediaDesl.x+tamX-pad*2;

        rectangle(teste, pt1, pt2, Scalar(255,0,0),1,8,0);

        imshow("window", image);
        imshow("teste", teste);

//        videoResult2 << image;

        key = (char) waitKey(10);
        if( key == 27 ) break; // esc pressed!

//        cont++;
//        if(cont==100){// acha novos pontos de bordas
//            goodFeaturesToTrack(next_image, corners[0], 50, 0.01, 20, mask, 3, false, 0.04);
//            cont = 0;
//        }

    }
//    videoResult2.release();
//    imwrite("blended.jpg",blended);

    return 0;
}