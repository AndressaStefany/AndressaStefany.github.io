#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat rotate(Mat image, double angle) {
    Mat dst;
    Point2f pt(image.cols/2., image.rows/2.);
    Mat r = getRotationMatrix2D(pt, angle, 1.0); //rotação
    warpAffine(image, dst, r, Size(image.cols, image.rows)); //rotacionando

    return dst;
}

Mat translation(Mat image, double dx, double dy) {
    Mat dst;

    Mat deslocamento = (Mat_<double>(2,3) << 1, 0, dx, 0, 1, dy);
    //translação
    warpAffine(image, dst, deslocamento, image.size());

    return dst;
}

int main(int argc, char** argv) {
    Mat image;
    image= imread("../images/pingo.png",CV_LOAD_IMAGE_GRAYSCALE);

    image = translation(image,100,300);
    image = rotate(image, 10);

    imshow("janela1", image);
    waitKey();

    return 0;
}