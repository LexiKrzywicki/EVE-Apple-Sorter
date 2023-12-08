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
        cv::Mat erosionImage;
        cv::Mat dilationImage;
        cv::Mat appleShape;

        double totalPixels;
        double redPixels;
        double percentRed;



        //for erosion and dilation
        int erosion_elem = 0;
        int erosion_size = 5;
        int dilation_elem = 0;
        int dilation_size = 2;
        int const max_elem = 2;
        int const max_kernel_size = 21;


    cv::Mat removeBackground(){

        cv::Mat maskLeft;
        cv::Mat maskRight;
        cv::Mat fullMask;
        
        cv::Mat imgRight;
        cv::Mat combine;

        cv::Scalar backLow(0,0,0);
        cv::Scalar backHigh(80,255,255);

        cv::Scalar backBottom(150, 0, 0);
        cv::Scalar backTop(179, 255, 255);

        cv::inRange(HSV, backLow, backHigh, maskLeft);
        cv::inRange(HSV, backBottom, backTop, maskRight);

        //combine the upper and lower ranges
        fullMask = maskLeft + maskRight;

        //calculates total number of pixels that are not black - should only be apple
        totalPixels = cv::countNonZero(fullMask); 

        //perform bitwise on mask
        cv::Mat finalApple;
        cv::bitwise_and(origImage, origImage, finalApple, fullMask);

        //final image is colored apple & black background
        return finalApple;

    }

    cv::Mat getRed(){

        cv::Mat thresLeft;
        cv::Mat thresRight;
        cv::Mat thresFull;
        cv::Mat noBackHSV;
        cv::Mat finalRed;
        cv::Mat finalRedBGR;  

        cvtColor(noBackImage, noBackHSV, cv::COLOR_BGR2HSV);

        cv::Scalar redLeftLowRange(0,1,0);
        cv::Scalar redLeftHighRange(25,255,255);

        cv::Scalar redRightLowRange(155, 0, 0);
        cv::Scalar redRightHighRange(179, 255, 255);

        //create mask
        cv::inRange(noBackHSV, redLeftLowRange, redLeftHighRange, thresLeft);
        cv::inRange(noBackHSV, redRightLowRange, redRightHighRange, thresRight);

        thresFull = thresLeft + thresRight;

        cv::bitwise_and(noBackHSV, noBackHSV, finalRed, thresFull);

        //counts all non black pixels - should be only red pixels
        redPixels = cv::countNonZero(thresFull);

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

    cv::Mat Erosion( int, void* )
    {
        int erosion_type = 2;
        if( erosion_elem == 0 ){ erosion_type = cv::MORPH_RECT; }
        else if( erosion_elem == 1 ){ erosion_type = cv::MORPH_CROSS; }
        else if( erosion_elem == 2) { erosion_type = cv::MORPH_ELLIPSE; }
        cv::Mat element = cv::getStructuringElement( erosion_type,
                            cv::Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                            cv::Point( erosion_size, erosion_size ) );
        cv::erode(noBackImage, erosionImage, element );
        return erosionImage;
    }

    cv::Mat Dilation( int, void* )
    {
        int dilation_type = 2;
        if( dilation_elem == 0 ){ dilation_type = cv::MORPH_RECT; }
        else if( dilation_elem == 1 ){ dilation_type = cv::MORPH_CROSS; }
        else if( dilation_elem == 2) { dilation_type = cv::MORPH_ELLIPSE; }
        cv::Mat element = cv::getStructuringElement( dilation_type,
                            cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                            cv::Point( dilation_size, dilation_size ) );
        cv::dilate(noBackImage, dilationImage, element );
        return dilationImage;
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

        appleShape = noBackImage.clone();

        cv::drawContours(appleShape, contours, -1, cv::Scalar(0,255,0), 2);

        return appleShape;
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

    apple.erosionImage = apple.Erosion(  0, 0 );
    apple.dilationImage = apple.Dilation(  0, 0 );

    apple.appleShape = apple.getShape();

    std::cout << "GRADE: " << apple.grade() << std::endl;

    //shows images
    cv::imshow("Original", apple.origImage);
    cv::imshow("No Background", apple.noBackImage);
    cv::imshow("Red", apple.redImage);
    cv::imshow("Erosion", apple.erosionImage);
    cv::imshow("Dilation", apple.dilationImage);
    cv::imshow("Shape", apple.appleShape);

    cv::waitKey(0);
    
    return 0;
}


