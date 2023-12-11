#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>

class appleInfo{
    public:

        cv::Mat origImage;
        cv::Mat HSV;
        cv::Mat noBackImage;  //without background
        cv::Mat redImage;  //red only parts of the apple
        cv::Mat erosionImage;
        cv::Mat dilationImage;
        cv::Mat appleShape;

        std::pair<cv::Mat, double> apple0;
        std::pair<cv::Mat, double> apple1;
        std::pair<cv::Mat, double> apple2;


        cv::Mat leftApple;
        cv::Mat middleApple;
        cv::Mat rightApple;

        double totalPixels;
        double redPixels;
        double percentRed;



        //for erosion and dilation
        int erosion_elem = 0;
        int erosion_size = 2;
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

        cv::Mat noBackHSV;
        cv::Mat thresLeft;
        cv::Mat thresRight;
        cv::Mat thresFull;
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
        cv::dilate(erosionImage, dilationImage, element );
        return dilationImage;
    }

    //to get individual apples
    cv::Mat newCroppedImg(cv::Rect rectShape){
        cv::Mat cropped;
        cv::Mat cloned;
        cloned = noBackImage.clone();

        int x = rectShape.x;
        int y = rectShape.y;

        int width = rectShape.width;
        int height = rectShape.height;

        cv::Rect myROI(x, y, width+1, height+1);
      
        cropped = noBackImage(myROI);

        return cropped;
    }

    double appleRatio(cv::Mat desiredApple){
        double width = desiredApple.size().width;
        double height = desiredApple.size().height;

        cv::Mat desiredGray;
        cvtColor(desiredApple, desiredGray, cv::COLOR_BGR2GRAY);

        double totalArea = width*height;
        double nonZero = cv::countNonZero(desiredGray);

        return (nonZero/totalArea)*100.00;
    }



    cv::Mat seperate(){

        cv::Mat noBackGray;
        cv::Mat binaryThresh;

        int thresh = 100;
        cv::RNG rng(12345);

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;

        //convert image to gray
        cv::cvtColor(dilationImage, noBackGray, cv::COLOR_BGR2GRAY);

        cv::threshold(noBackGray, binaryThresh, 100, 255, cv::THRESH_BINARY);

        cv::Mat canny_output;
        cv::Canny( noBackGray, canny_output, thresh, thresh*2 );

        cv::findContours(binaryThresh, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

        std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
        std::vector<cv::Rect> boundRect( contours.size() );
        std::vector<cv::Rect> rectArea(contours.size());

        cv::Mat drawing = cv::Mat::zeros( canny_output.size(), CV_8UC3 );

        appleShape = noBackImage.clone();

        //gets a list of rectangles based on contours and draws them on image
        for( size_t i = 0; i < contours.size(); i++ )
        {
            cv::approxPolyDP( contours[i], contours_poly[i], 3, true );
            boundRect[i] = cv::boundingRect( contours_poly[i] );
            cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
            rectangle(appleShape, boundRect[i].tl(), boundRect[i].br(), color, 2 );
            if(boundRect[i].area() > rectArea[0].area()){
                rectArea[2] = rectArea[1];
                rectArea[1] = rectArea[0];
                rectArea[0] = boundRect[i];
            }
            else if(boundRect[i].area() > rectArea[1].area() && boundRect[i].area() != rectArea[0].area()){
                rectArea[2] = rectArea[1];
                rectArea[1] = boundRect[i];
            }
            else if(boundRect[i].area( ) > rectArea[2].area() && boundRect[i].area() != rectArea[1].area()){
                rectArea[2] = boundRect[i];
            }
        } 

        apple0.first = newCroppedImg(rectArea[0]);
        apple0.second = appleRatio(apple0.first);

        std::get<0>(apple1) = newCroppedImg(rectArea[1]);
        apple1.second = appleRatio(apple1.first);

        std::get<0>(apple2) = newCroppedImg(rectArea[2]); 
        apple2.second = appleRatio(apple1.first);

        return appleShape;
    }


    //determines apple grade from percent red
    std::string grade(){

        std::string strGrade = "Cider";

        if(percentRed > 60){
            if(apple0.second > 78.4 && apple1.second > 78.4 && apple2.second >78.4){
                strGrade = "G1";
            }
            else{
                strGrade = "G2";
                std::cout << "bad ratios" << std::endl;
            }

        }
        else if(percentRed > 45){
            strGrade = "G2";
            std::cout << "red percentage not high enough" << std::endl;
        }
        else{
            strGrade = "Cider";
            std::cout << "low red percentage" << std::endl;
        }

        std::cout << "GRADE: " << apple.grade() << std::endl;
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

    apple.erosionImage = apple.Erosion(0,0);
    apple.dilationImage = apple.Dilation(0,0);

    apple.appleShape = apple.seperate();

    //shows images
    //cv::imshow("Original", apple.origImage);
    // cv::imshow("No Background", apple.noBackImage);
    // cv::imshow("Red", apple.redImage);
    // cv::imshow("Erosion", apple.erosionImage);
    // cv::imshow("Dilation", apple.dilationImage);
    cv::imshow("Apple0", apple.apple0.first);
    cv::imshow("Apple1", apple.apple1.first);
    cv::imshow("Apple2", apple.apple2.first);
    cv::imshow("Shape", apple.appleShape);

    cv::waitKey(0);
    
    return 0;
}


