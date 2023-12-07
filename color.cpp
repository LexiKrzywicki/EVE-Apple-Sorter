#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>

int thresh = 100;  //originally 75
cv::RNG rng(12345);
cv::Mat appleHSV;
cv::Mat drawing;
cv::Mat imgApple;

class appleInfo{
    public:

        cv::Mat origImage;
        cv::Mat HSV;
        cv::Mat noBackImage;  //without background
        cv::Mat redImage;  //red only parts of the apple
        cv::Mat appleShape;

        double totalPixels;
        double redPixels;
        double percentRed;

    cv::Mat removeBackground(){

        cv::Mat imgBack;
        cv::Mat imgRight;
        cv::Mat combine;

        cv::Scalar backLow(0,150,50);
        cv::Scalar backHigh(35,255,255);

        cv::inRange(HSV, backLow, backHigh, imgBack);

        //calculates total number of pixels that are not black - should only be apple
        totalPixels = cv::countNonZero(imgBack); 

        //perform bitwise on mask
        cv::Mat finalApple;
        cv::bitwise_and(origImage, origImage, finalApple, imgBack);

        //final image is colored apple & black background
        return finalApple;

    }

    cv::Mat getRed(){

        cv::Mat thres;
        cv::Mat noBackHSV;
        cv::Mat finalRed;
        cv::Mat finalRedBGR;  

        cvtColor(noBackImage, noBackHSV, cv::COLOR_BGR2HSV);

        cv::Scalar redLeftLowRange(0,150,100);
        cv::Scalar redLeftHighRange(13,255,255);

        //create mask
        cv::inRange(noBackHSV, redLeftLowRange, redLeftHighRange, thres);

        cv::bitwise_and(noBackHSV, noBackHSV, finalRed, thres);

        //counts all non black pixels - should be only red pixels
        redPixels = cv::countNonZero(thres);

        //coverts image to BGR color for showing
        cvtColor(finalRed, finalRedBGR, cv::COLOR_HSV2BGR);

        redImage = finalRedBGR;

        return finalRedBGR;
    }
    
    //calculates percent red
    double getColorPercent(){

        percentRed = (redPixels / totalPixels) *100.00;
        //std::cout << "percent red: " << percent << std::endl;
        return percentRed;
    }

    cv::Mat getShape(){

        cv::Mat noBackGray;
        cv::Mat noBackColor;
        cv::Mat binaryThresh;

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;

        //convert noBack image to gray
        cv::cvtColor(noBackImage, noBackColor, cv::COLOR_HSV2BGR);
        cv::cvtColor(noBackColor, noBackGray, cv::COLOR_BGR2GRAY);

        cv::threshold(noBackGray, binaryThresh, 100, 255, cv::THRESH_BINARY);

        cv::findContours(binaryThresh, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

        cv::Mat image_copy = noBackImage.clone();

        cv::drawContours(image_copy, contours, -1, cv::Scalar(0,255,0), 2);

        return image_copy;
    }

    //determines apple grade from percent red
    std::string grade(){

        std::string strGrade;

        if(percentRed > 60){
            strGrade = "G1";
        }
        else if(percentRed > 45){
            strGrade = "G2";
        }
        else{
            strGrade = "Cider";
        }

        return strGrade;
    }


};


int main( int argc, char** argv )
{
    appleInfo apple;

    //get image from commandline
    apple.origImage = cv::imread(argv[1], cv::IMREAD_COLOR);

    //convert to HSV
    cvtColor(apple.origImage, apple.HSV, cv::COLOR_BGR2HSV);


    apple.noBackImage = apple.removeBackground();

    apple.redImage = apple.getRed();

    double percentage = apple.getColorPercent();
    std::cout << "red percentage = " << percentage << "%" << std::endl;

    apple.appleShape = apple.getShape();

    std::cout << "GRADE: " << apple.grade() << std::endl;

    //shows images
    cv::imshow("Original", apple.origImage);
    cv::imshow("No Background", apple.noBackImage);
    cv::imshow("Red", apple.redImage);
    cv::imshow("Shape", apple.appleShape);

    cv::waitKey(0);
    
    return 0;
}


