#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include <iostream>

int thresh = 100;
cv::RNG rng(12345);
cv::Mat appleHSV;

class appleInfo{
    public:

    cv::Mat image;
    double totalPixels;
    double redPixels;
    
};

void thresh_callback(int, void* );
appleInfo removeBackground(cv::Mat desiredImage, cv::Mat OriginalImage, appleInfo A);
appleInfo getRed(cv::Mat desiredImage, cv::Mat OriginalImage, appleInfo A);
double getColorPercent(appleInfo A);


int main( int argc, char** argv )
{
    appleInfo apple;
    //get image
    cv::Mat imgApple = cv::imread(argv[1], cv::IMREAD_COLOR);
    

    //convert to HSV
    //cvtColor(imgApple, appleHSV, cv::COLOR_BGR2HSV);


    cvtColor( imgApple, appleHSV, cv::COLOR_BGR2GRAY );
    blur( appleHSV, appleHSV, cv::Size(3,3) );

    //thresh 75 for image1
    const char* source_window = "Source";
    cv::namedWindow( source_window );
    cv::imshow( source_window, imgApple );
    const int max_thresh = 255;
    cv::createTrackbar( "Canny thresh:", source_window, &thresh, max_thresh, thresh_callback );
    thresh_callback(0, 0);
    cv::waitKey();


    /*
    apple = removeBackground(appleHSV, imgApple, apple);
    std::cout << "Main: totalPixels" << apple.totalPixels << std::endl;

    apple = getRed(appleHSV, imgApple, apple);
    std::cout << "Main: redPixels" << apple.redPixels << std::endl;

    double percentage = getColorPercent(apple);
    std::cout << "percent of red present" << percentage << std::endl;

    cv::imshow("Original", imgApple);
    //cv::imshow("Threshold Image", imgBack);
    cv::imshow("Final", apple.image);
    cv::waitKey(0);
    */

    return 0;
}


void thresh_callback(int, void* )
{
 cv::Mat canny_output;
 Canny( appleHSV, canny_output, thresh, thresh*2 );
 std::vector<std::vector<cv::Point> > contours;
 findContours( canny_output, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );
 std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
 std::vector<cv::Rect> boundRect( contours.size() );
 std::vector<cv::Point2f>centers( contours.size() );
 std::vector<float>radius( contours.size() );
 for( size_t i = 0; i < contours.size(); i++ )
 {
    cv::approxPolyDP( contours[i], contours_poly[i], 3, true );
    boundRect[i] = cv::boundingRect( contours_poly[i] );
    minEnclosingCircle( contours_poly[i], centers[i], radius[i] );
 }
 cv::Mat drawing = cv::Mat::zeros( canny_output.size(), CV_8UC3 );
 for( size_t i = 0; i< contours.size(); i++ )
 {
    cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
    cv::drawContours( drawing, contours_poly, (int)i, color );
    cv::rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2 );
    cv::circle( drawing, centers[i], (int)radius[i], color, 2 );
 }
 cv::imshow( "Contours", drawing );
}



appleInfo removeBackground(cv::Mat desiredImage, cv::Mat OriginalImage, appleInfo A){

    cv::Scalar backLeftLowRange(20, 0, 20);
    cv::Scalar backLeftHighRange(25, 100, 255);
    cv::Scalar backRightLowRange(50, 0, 20);
    cv::Scalar backRightHighRange(150, 255, 255);

    cv::Mat imgBack;
    inRange(desiredImage, backLeftLowRange, backLeftHighRange, imgBack);
    //inRange(desiredImage, backCenterLowRange, backCenterHighRange, imgBack);
    inRange(desiredImage, backRightLowRange, backRightHighRange, imgBack);

    //imgBack is image with black representing apple

    //to get the size of the apple. cannot do this with finalApple becuas finalApple does not have numerica color values for countNonZero to work.
    //bitwising with OriginalImage produces nonHSV image
    int totalImagePixels = imgBack.cols * imgBack.rows;
    A.totalPixels = totalImagePixels - cv::countNonZero(imgBack); 
    std::cout << "size of apple: " << A.totalPixels << std::endl;


    //perform bitwise on mask
    cv::Mat finalApple;
    cv::Mat notMask;
    cv::bitwise_not(imgBack, notMask);
    cv::bitwise_and(OriginalImage, OriginalImage, finalApple, notMask);
    //final image is colored apple & black background
    A.image = finalApple;
    return (A);

}

appleInfo getRed(cv::Mat desiredImage, cv::Mat OriginalImage, appleInfo A){
    //HSV values
    cv::Scalar redLeftLowRange(0, 100, 20);
    cv::Scalar redLeftHighRange(25, 255, 255);
    cv::Scalar redRightLowRange(160, 100, 20);
    cv::Scalar redRightHighRange(179, 255, 255);

    cv::Mat imgThres;
    cv::Mat finalApple;

    //create mask
    inRange(desiredImage, redLeftLowRange, redLeftHighRange, imgThres);
    inRange(desiredImage, redRightLowRange, redRightHighRange, imgThres);

    //imgThres is a black and white image wheere white represents the red parts

    A.redPixels = cv::countNonZero(imgThres);
    std::cout << "redPixels: " << A.redPixels << std::endl;
    std::cout << "totalPixels: " << A.totalPixels << std::endl;


    //perform bitwise on mask
    cv::bitwise_and(OriginalImage, OriginalImage, finalApple, imgThres);
    A.image = finalApple;

    return (A);
}

double getColorPercent(appleInfo A){

    double percent = (A.redPixels / A.totalPixels) *100.00;
    std::cout << "percent red: " << percent << std::endl;
    return percent;
}

