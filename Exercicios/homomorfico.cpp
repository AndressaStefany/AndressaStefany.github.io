#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

using namespace cv;
using namespace std;

//int gl = 3, gh= 13, d0 = 41, c= 5;
int gl, gh, d0, c;
int gl_max=100, gh_max=100, d0_max=100, c_max=100;

Mat padded, filter, complexImage;
Mat image, imagegray, tmp;
Mat_<float> realInput, zeros;
vector<Mat> planos;

char key;
int dft_M, dft_N;

// troca os quadrantes da imagem da DFT
void deslocaDFT(Mat& image ){
    Mat tmp2, A, B, C, D;

    // se a imagem tiver tamanho impar, recorta a regiao para
    // evitar cópias de tamanho desigual
    image = image(Rect(0, 0, image.cols & -2, image.rows & -2));
    int cx = image.cols/2;
    int cy = image.rows/2;

    // reorganiza os quadrantes da transformada
    // A B   ->  D C
    // C D       B A
    A = image(Rect(0, 0, cx, cy));
    B = image(Rect(cx, 0, cx, cy));
    C = image(Rect(0, cy, cx, cy));
    D = image(Rect(cx, cy, cx, cy));

    // A <-> D
    A.copyTo(tmp2);  D.copyTo(A);  tmp2.copyTo(D);

    // C <-> B
    C.copyTo(tmp2);  B.copyTo(C);  tmp2.copyTo(B);
}

void filterHomo(int, void*){
    // gh, gl e d0 variam de 0 a 10
    double gh_calc = gh/10.0;
    double gl_calc = gl/10.0;
    double d0_calc = d0/10.0;
    double c_calc= c/1000.0; //de 0 a 0.1

    double d;

    // prepara o filtro homomorfico
    for(int i=0; i<dft_M; i++){
        for(int j=0; j<dft_N; j++){
            d = (i-dft_M/2)*(i-dft_M/2)+(j-dft_N/2)*(j-dft_N/2);// d²
            tmp.at<float> (i,j) = (gh_calc-gl_calc)*(1-exp(-c_calc*d/(d0_calc*d0_calc)))+gl_calc;
        }
    }
    Mat comps[]= {tmp, tmp};
    imshow("filtro",tmp);
    merge(comps, 2, filter);
}

int main(int , char**){
    //cria janela e sliders
    namedWindow("filtrada", CV_WINDOW_AUTOSIZE);
    createTrackbar("Gamma H:", "filtrada", &gh, gh_max, filterHomo);
    createTrackbar("Gamma L:", "filtrada", &gl, gl_max, filterHomo);
    createTrackbar("D0:", "filtrada", &d0, d0_max,filterHomo);
    createTrackbar("C:", "filtrada", &c, c_max, filterHomo);

    // ler imagem para filtragem
    image = imread("../images/cents.jpg", CV_LOAD_IMAGE_GRAYSCALE);

    // identifica os tamanhos otimos para
    // calculo do FFT
    dft_M = getOptimalDFTSize(image.rows);
    dft_N = getOptimalDFTSize(image.cols);

    // realiza o padding da imagem
    copyMakeBorder(image, padded, 0,
                   dft_M - image.rows, 0,
                   dft_N - image.cols,
                   BORDER_CONSTANT, Scalar::all(0));

    // parte imaginaria da matriz complexa (preenchida com zeros)
    zeros = Mat_<float>::zeros(padded.size());

    // prepara a matriz complexa para ser preenchida
    complexImage = Mat(padded.size(), CV_32FC2, Scalar(0));

    // a função de transferência (filtro frequencial) deve ter o
    // mesmo tamanho e tipo da matriz complexa
    filter = complexImage.clone();
    // cria uma matriz temporária para criar as componentes real
    // e imaginaria do filtro ideal
    tmp = Mat(dft_M, dft_N, CV_32F);

    for(;;){
        imagegray = image.clone();
        imshow("original", imagegray);

        // realiza o padding da imagem
        copyMakeBorder(imagegray, padded, 0,
                       dft_M - image.rows, 0,
                       dft_N - image.cols,
                       BORDER_CONSTANT, Scalar::all(0));

        // limpa o array de matrizes que vao compor a
        // imagem complexa
        planos.clear();
        // cria a compoente real
        realInput = Mat_<float>(padded);
        //evitar log(0)
        realInput += Scalar::all(1);
        // aplicando o log na parte real
        log(realInput,realInput);
        // insere as duas componentes no array de matrizes
        planos.push_back(realInput);
        planos.push_back(zeros);

        // combina o array de matrizes em uma unica
        // componente complexa
        merge(planos, complexImage);

        // calcula o dft
        dft(complexImage, complexImage);

        // realiza a troca de quadrantes
        deslocaDFT(complexImage);

        // aplica o filtro frequencial
        mulSpectrums(complexImage,filter,complexImage,0);

        // troca novamente os quadrantes
        deslocaDFT(complexImage);

        // calcula a DFT inversa
        idft(complexImage, complexImage);

        // limpa o array de planos
        planos.clear();

        // separa as partes real e imaginaria da
        // imagem filtrada
        split(complexImage, planos);

        // exponencial
//        planos[0] -= Scalar::all(1);
//        exp(planos[0], planos[0]);

        // normaliza a parte real para exibicao
        normalize(planos[0], planos[0], 0, 1, CV_MINMAX);
        imshow("filtrada", planos[0]);

        key = (char) waitKey(10);
        if( key == 27 ) break; // esc pressed!
    }
    // salvar img filtrada e filtro
    Mat final;
    planos[0].convertTo(final, CV_8UC1, 255.0);
    imwrite("filtrada.png", final);
    // mostra parte real do filtro
    vector<Mat> pFiltro;
    split(filter,pFiltro);
    normalize(pFiltro[0], pFiltro[0], 0, 1, CV_MINMAX);

    pFiltro[0].convertTo(final, CV_8UC1, 255.0);
    imwrite("filtro.png", final);

    waitKey(0);

    return 0;
}
