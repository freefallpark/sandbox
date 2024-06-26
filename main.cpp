#include <opencv2/opencv.hpp>

int main() {
    //Raw Data
    cv::Mat P = (cv::Mat_<float>(3,2) << 0,0,0,1,1,0);
    cv::Mat S = (cv::Mat_<float>(3,2) << 0,0,1,0,0,1);
    std::cout << "Pupil Data: " << std::endl;
    for(int i=0; i<P.rows; i++){
        std::cout << "\t" << P.row(i) << std::endl;
    }
    std::cout << "Screen Data: " << std::endl;
    for(int i=0; i<S.rows; i++){
        std::cout << "\t" << S.row(i) << std::endl;
    }

    //Prepare to Solve for Model: Sx = Px*mxx + Py*myx + Bxx + Byx
    //Create A Matrix
    cv::Mat A = cv::Mat::ones(3,3,CV_32F);
    P.copyTo(A(cv::Rect(0,0,2,3)));

    //Solve for the Model:
    std::cout <<  "Model: " << std::endl;
    cv::Mat cx,cy;
    cv::solve(S.col(0), A, cx, cv::DECOMP_SVD);
    std::cout <<  "\t[mxx, myx, Bx]: " << cx << std::endl;
    cv::solve(S.col(1), A, cy, cv::DECOMP_SVD);
    std::cout <<  "\t[mxy, myy, By: " << cy << std::endl;

    //Test The Model:
    std::cout << "Test Case" << std::endl;
    cv::Mat p = (cv::Mat_<float>(1, 3) << 10,0,1);
    std::cout << "\tPupil Point = " << p(cv::Rect(0,0,2,1)) << std::endl;
    cv::Mat ans = p * cx.t();
    std::cout << "\tScreen Point= [" << ans.at<float>(0) << ", ";
    ans = p * cy.t();
    std::cout << ans.at<float>(0) << "]" << std::endl;
}
