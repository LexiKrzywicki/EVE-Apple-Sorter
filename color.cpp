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
        
        double totalPixels;
        double redPixels;
        double percentRed;

    cv::Mat removeBackground(){

        cv::Mat imgBack;

        cv::Scalar backLow(0,150,100);
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

    //crops image
    apple.origImage = apple.origImage(cv::Range(70,400), cv::Range(40, 610));

    //convert to HSV
    cvtColor(apple.origImage, apple.HSV, cv::COLOR_BGR2HSV);


    apple.noBackImage = apple.removeBackground();

    apple.redImage = apple.getRed();

    double percentage = apple.getColorPercent();
    std::cout << "red percentage = " << percentage << "%" << std::endl;

    std::cout << "GRADE: " << apple.grade() << std::endl;

    //shows images
    cv::imshow("Original", apple.origImage);
    cv::imshow("No Background", apple.noBackImage);
    cv::imshow("Red", apple.redImage);

    cv::waitKey(0);
    
    return 0;
}


