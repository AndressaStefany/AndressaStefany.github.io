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
    char key;
    Mat prev_image, next_image, image, mask, last_T, final;
    vector<Point2f> corners[2], greatCorners[3];

    vector<uchar> status;
    vector<float> err;
    Size winSize(31,31);
    TermCriteria termcrit(TermCriteria::COUNT|TermCriteria::EPS,20,0.03);

    VideoCapture cap("../hipo.mp4");
    cap >> prev_image;
    cvtColor(prev_image, prev_image, CV_BGR2GRAY);
    resize(prev_image,prev_image,Size(700,700));
    int cont=0;
    while (1) {
        if(!next_image.empty()) {
            // atualiza prev_image
            prev_image = next_image.clone();
        }
        cap >> next_image;
        if (next_image.empty())
            break;
        cvtColor(next_image, next_image, CV_BGR2GRAY);
        resize(next_image,next_image,Size(700,700));

        goodFeaturesToTrack(prev_image, corners[0], 300, 0.01, 20, mask, 3, false, 0.04);
        calcOpticalFlowPyrLK(prev_image, next_image, corners[0], corners[1], status, err,
                             winSize, 3, termcrit,0, 0.001);

        for(size_t i=0; i < status.size(); i++) {
            if(status[i]) {
                greatCorners[0].push_back(corners[0][i]);
                greatCorners[1].push_back(corners[1][i]);

                greatCorners[2].push_back(Point(corners[0][i].x-corners[1][i].x,corners[0][i].y-corners[1][i].y));

                cout<<"greatCorners: ("<<corners[0][i].x-corners[1][i].x<<" , "
                    <<corners[0][i].y-corners[1][i].y<<" )"<<endl;
            }
        }
        corners[0].clear();
        corners[1].clear();

        Mat T = estimateRigidTransform(greatCorners[0], greatCorners[1], false);
        if(T.data == NULL) {
            last_T.copyTo(T);
        }
        T.copyTo(last_T);

        double dx = T.at<double>(0,2);
        double dy = T.at<double>(1,2);
        double da = atan2(T.at<double>(1,0), T.at<double>(0,0));

        cout<<"dx: "<<dx<<" dy: "<<dy<<" da: "<<da<<endl;

        final = rotate(next_image, da);
        final = translation(final, dx, dy);

        imshow("original", next_image);
        imshow("window", final);

        key = (char) waitKey(10);
        if( key == 27 ) break; // esc pressed!

        cont=cont+5;
    }
    return 0;
}