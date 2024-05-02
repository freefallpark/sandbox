#include <opencv2/opencv.hpp>

std::vector<cv::Point3f> spherePoints(float r, cv::Point3f center){
    //Create points along the x and y axes:
    int pointDensity = 5;
    std::vector<cv::Point3f> xyz;
    for(int i=1;i<pointDensity;i++){
        float p = (float)i/(float)pointDensity;
        xyz.emplace_back(    p*r,       0, 0);
        xyz.emplace_back(    -p*r,      0, 0);
        xyz.emplace_back(    0,       p*r, 0);
        xyz.emplace_back(    0,      -p*r, 0);
    }

    //Create 3d Points with equation of a circle :
    for(auto & i : xyz){
        // From Equation of Sphere: z = sqrt(r^2 - x^2 - y^2)
        i.z = (float)sqrt(pow(r,2)- pow(i.x,2)-pow(i.y,2));
        i+=center;
    }

    return xyz;
}

int main() {
    //Generate Fake xyz, data
    std::vector<cv::Point3f> points = spherePoints(10, cv::Point3f(0,0,0));

    //regionPlot Points
    cv::Mat img = cv::Mat(720,1280,CV_32FC3);
    //region create Camera Matrix
    cv::Mat camMat = cv::Mat::eye(3,3, CV_32F)*20;
    camMat.at<float>(0,0) = 800;        //fx
    camMat.at<float>(1,1) = 800;        //fy
    camMat.at<float>(0,2) = 1280.0/2.0; //cx
    camMat.at<float>(1,2) = 720.0/2.0;  //cy
    //endregion
    //region Create Distortion Coeff
    std::vector<float> distCoeff = {0,0,0,0,0,0,0,0};
    //endregion
    std::vector<cv::Point2f> pts2d;
    cv::Vec3f rVec(0.78,0.78,0),tVec(0,0,75);
    cv::projectPoints(points,rVec,tVec,camMat,distCoeff,pts2d);
    std::vector<cv::KeyPoint> kp;
    for(auto p:pts2d){
        cv::circle(img,p,3,cv::Scalar(255,255,255),-1);
    }
    //endregion

    //region Fit Sphere to calculate best fit center of rotation
    cv::Mat f((int)points.size(),1,CV_32F);
    cv::Mat A((int)points.size(),4,CV_32F);
    cv::Mat c(f);
    for(int i=0;i<points.size();i++){
        //Populate f vector
        auto val = (float)(pow(points[i].x, 2) + pow(points[i].y, 2) + pow(points[i].z, 2));
        f.at<float>(i,0) = val;
        //Populate A matrix
        cv::Mat a = (cv::Mat_<float>(1,4) << 2*points[i].x,2*points[i].y,2*points[i].z,1);
        a.copyTo(A(cv::Rect(0,i,4,1)));
    }
    cv::solve(A,f,c, cv::DECOMP_SVD);
    std::cout << "{x0, y0, z0, r}\n" << c.t() << std::endl;
    //endregion

    //region Plot Center of Best Fit Sphere
    std::vector<cv::Point2f> imgPts;
    cv::projectPoints(c(cv::Rect(0,0,1,3)),rVec,tVec,camMat,distCoeff,imgPts);
    cv::drawMarker(img,imgPts[0],cv::Scalar(255,0,0));
    //endregion

    //todo: Note The vector from the constellation origin to the center of the sphere written in the constellation frame
    // should be the translation portion of the transformation matrix to go from constellation to doctor.
    //  I still need to think about this problem more I think...

    //todo: give an instruction to look left and right we need to determine how much rotation about constellation x is from constellation xy plane to doctor xy plane;

    cv::imshow("3dPoints:",img);
    cv::waitKey(0);
};
