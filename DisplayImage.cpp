#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>

        // cv::Mat erosion_dst;
        // cv::Mat dilation_dst;
        // int erosion_elem = 0;
        // int erosion_size = 5;
        // int dilation_elem = 0;
        // int dilation_size = 2;
        // int const max_elem = 2;
        // int const max_kernel_size = 21;


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

    void Erosion( int, void* )
    {
        int erosion_type = 2;
        if( erosion_elem == 0 ){ erosion_type = cv::MORPH_RECT; }
        else if( erosion_elem == 1 ){ erosion_type = cv::MORPH_CROSS; }
        else if( erosion_elem == 2) { erosion_type = cv::MORPH_ELLIPSE; }
        cv::Mat element = cv::getStructuringElement( erosion_type,
                            cv::Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                            cv::Point( erosion_size, erosion_size ) );
        cv::erode(noBackImage, erosionImage, element );
        //cv::imshow( "Erosion Demo", erosionImage);
    }

    void Dilation( int, void* )
    {
        int dilation_type = 2;
        if( dilation_elem == 0 ){ dilation_type = cv::MORPH_RECT; }
        else if( dilation_elem == 1 ){ dilation_type = cv::MORPH_CROSS; }
        else if( dilation_elem == 2) { dilation_type = cv::MORPH_ELLIPSE; }
        cv::Mat element = cv::getStructuringElement( dilation_type,
                            cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                            cv::Point( dilation_size, dilation_size ) );
        cv::dilate(noBackImage, dilationImage, element );
        //cv::imshow( "Dilation Demo", dilationImage);
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


    cv::Mat frame;
    //--- INITIALIZE VIDEOCAPTURE
    cv::VideoCapture cap;
    // open the default camera using default API
    // cap.open(0);
    // OR advance usage: select any API backend
    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    // open selected camera using selected API
    cap.open(deviceID, apiID);
    // check if we succeeded
    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }

    cap.read(frame);

    appleInfo apple;

    //get image from commandline
    //apple.origImage = cv::imread(frame, cv::IMREAD_COLOR);

    apple.origImage = frame;

    //crops image
    apple.origImage = apple.origImage(cv::Range(70,400), cv::Range(40, 610));

    // //convert to HSV
    cvtColor(apple.origImage, apple.HSV, cv::COLOR_BGR2HSV);


    apple.noBackImage = apple.removeBackground();

    apple.redImage = apple.getRed();

    double percentage = apple.getColorPercent();
    std::cout << "red percentage = " << percentage << "%" << std::endl;

    apple.appleShape = apple.getShape();

    std::cout << "GRADE: " << apple.grade() << std::endl;

    // //shows images
    // cv::imshow("Original", apple.origImage);
    //cv::imshow("No Background", apple.noBackImage);
    //cv::imshow("Red", apple.redImage);
    //cv::imshow("Shape", apple.appleShape);




    //erosion and dilution
    //cv::namedWindow( "Erosion Demo", cv::WINDOW_AUTOSIZE );
    //cv::namedWindow( "Dilation Demo", cv::WINDOW_AUTOSIZE );
    // auto fun1 = apple.Erosion;
    // auto fun2 = apple.Dilation;
    // cv::createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Erosion Demo", &erosion_elem, max_elem, fun1);
    // cv::createTrackbar( "Kernel size:\n 2n +1", "Erosion Demo", &erosion_size, max_kernel_size, fun1);
    // cv::createTrackbar( "Element:\n 0: Rect \n 1: Cross \n 2: Ellipse", "Dilation Demo", &dilation_elem, max_elem, fun2);
    // cv::createTrackbar( "Kernel size:\n 2n +1", "Dilation Demo", &dilation_size, max_kernel_size, fun2);

    apple.Erosion(  0, 0 );
    apple.Dilation(  0, 0 );

    //cv::waitKey(0);
    
    return 0;
}


