#include <opencv2/opencv.hpp>

int main() {
    //region create fake Raw Data
    cv::Mat P = (cv::Mat_<float>(3,2) << 0,0,
                                         0,10,
                                         5,0);
    cv::Mat S = (cv::Mat_<float>(3,2) << 0,0,
                                         10,0,
                                         0,5);
    std::cout << "Pupil Data: " << std::endl;
    for(int i=0; i<P.rows; i++){
        std::cout << "\t" << P.row(i) << std::endl;
    }
    std::cout << "Screen Data: " << std::endl;
    for(int i=0; i<S.rows; i++){
        std::cout << "\t" << S.row(i) << std::endl;
    }
    //endregion

    //region Solve for Pupil to Screen Mapping
    //Prepare to Solve for Model: Sx = Px*mxx + Py*myx + Bxx + Byx
    //Create f vector:
    cv::Mat f  = S.col(0);
    std::cout << "f: \n" << f << std::endl;
    //Create A Matrix
    cv::Mat A = cv::Mat::ones(3,3,CV_32F);
    P.copyTo(A(cv::Rect(0,0,2,3)));
    std::cout << "A: \n" << A << std::endl;

    //Solve for the Model:
    std::cout <<  "Model: " << std::endl;
    cv::Mat cx,cy;
    cv::solve(A, S.col(0), cx, cv::DECOMP_SVD);
    std::cout <<  "\t[mxx, myx, Bx]: " << cx << std::endl;
    cv::solve(A, S.col(1), cy, cv::DECOMP_SVD);
    std::cout <<  "\t[mxy, myy, By]: " << cy << std::endl;
    //endregion

    //region Map from Pupil to Screen coordinates:
    std::cout << "Test Case" << std::endl;
    cv::Mat p = (cv::Mat_<float>(1, 3) << 10,0,1);
    std::cout << "\tPupil Point = " << p(cv::Rect(0,0,2,1)) << std::endl;
    cv::Mat Sx = p * cx;
    std::cout << "\tScreen Point= [" << Sx.at<float>(0) << ", ";
    cv::Mat Sy = p * cy;
    std::cout << Sy.at<float>(0) << "]" << std::endl;
    //endregion
}
