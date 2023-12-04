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
        cv::Mat noBackImage;  //without background
        cv::Mat redImage;  //red only parts of the apple
        //cv::Mat middle;  
        cv::Mat greyImage;  //grey image used for color thresholding
        double totalPixels;
        double redPixels;
        int diameter;


};

void getAppleSize(int, appleInfo A, void* );   //originally threshold
cv::Mat removeBackground(cv::Mat desiredImage, appleInfo A);
cv::Mat getRed(appleInfo A);
double getColorPercent(appleInfo A);


int main( int argc, char** argv )
{
    appleInfo apple;
    //get image
    apple.origImage = cv::imread(argv[1], cv::IMREAD_COLOR);

    apple.origImage = apple.origImage(cv::Range(70,400), cv::Range(40, 610));

    

    //convert to HSV
    cvtColor(apple.origImage, appleHSV, cv::COLOR_BGR2HSV);


//THIS IS FOR RED COLOR PERCENT!!!

    apple.noBackImage = removeBackground(appleHSV, apple);


    apple.redImage = getRed(apple);

    double percentage = getColorPercent(apple);
    std::cout << "red percentage = " << percentage << "%" << std::endl;


//THIS IS FOR BOUNDING BOX
    //convert image to grayscale and blue to reduce noise
    //cv::Mat appleGray;
    //cvtColor(apple.noBackImage, apple.greyImage, cv::COLOR_BGR2GRAY );
    //blur(apple.greyImage, apple.greyImage, cv::Size(3,3) );

    //below code if to easily change thresh and see result
        //const char* source_window = "Source";
        //cv::namedWindow( source_window );
        //cv::imshow( source_window, apple.middle );
        //const intdiameter max_thresh = 255;
        //cv::createTrackbar( "Canny thresh:", source_window, &thresh, max_thresh, getAppleSize );
    
    //getAppleSize(0, apple, 0);



    cv::imshow("Original", apple.origImage);
    cv::imshow("No Background", apple.noBackImage);
    cv::imshow("Red", apple.redImage);
    //cv::imshow("Gray Image", apple.greyImage);


    cv::waitKey(0);
    
    return 0;
}


void getAppleSize(int, appleInfo A, void* )
{

    //the following a.middle is changed to A.image for test originally a.middle

 cv::Mat canny_output;

 Canny(A.greyImage, canny_output, thresh, thresh*2 );  // used to detect edges
 
 std::vector<std::vector<cv::Point> > contours;
 findContours( canny_output, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );  //find and save contours
 
 //apply approximations to polygon with accuracy and find bounding rect for every polygon
 //find min. enclosing circle for every polygon
 std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
 std::vector<cv::Rect> boundRect( contours.size() );
 std::vector<cv::Point2f>centers( contours.size() );
 std::vector<float>radius( contours.size() );
 int accuracy = 3;
 for( size_t i = 0; i < contours.size(); i++ )
 {
    cv::approxPolyDP( contours[i], contours_poly[i], accuracy, true );
    boundRect[i] = cv::boundingRect( contours_poly[i] );
    minEnclosingCircle( contours_poly[i], centers[i], radius[i] );
 }

 drawing = cv::Mat::zeros( canny_output.size(), CV_8UC3 );
 int prevRadius = 0;
 int element = 0;
 for( size_t i = 0; i< contours.size(); i++ )
 {
    cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
    cv::drawContours( A.greyImage, contours_poly, (int)i, color );
    cv::rectangle( A.greyImage, boundRect[i].tl(), boundRect[i].br(), color, 1 );
    cv::circle( A.greyImage, centers[i], (int)radius[i], color, 2 );

    if((int)radius[i] > prevRadius){
        //A.diameter = (int)radius[i];  //this gets radius which might be bigger then apple
        prevRadius = radius[i];
        element = i;
    }
 }

 int x1 = boundRect[element].tl().x;
 int x2 = boundRect[element].br().x;

A.diameter = x2 - x1;


//  std::cout<< "boundRect: " << x1 << std::endl;
//  std::cout<< "boundRect: " << x2 << std::endl;
 //std::cout<< "radius: " << radius[element] << std::endl;

 std::cout << "diameter: " << A.diameter << " pixels" << std::endl;
    
    //cv::imshow( "Contours", imgApple );


}

cv::Mat removeBackground(cv::Mat desiredImage, appleInfo A){

    cv::Mat imgBack;

    cv::Scalar backLow(0,150,100);
    cv::Scalar backHigh(35,255,255);

    cv::inRange(desiredImage, backLow, backHigh, imgBack);

    //imgBack is image with black representing apple

    //to get the size of the apple. cannot do this with finalApple becuas finalApple does not have numerica color values for countNonZero to work.
    //bitwising with Original Image produces nonHSV image
    double totalImagePixels = imgBack.cols * imgBack.rows;
    A.totalPixels = totalImagePixels - cv::countNonZero(imgBack); 
    std::cout << "size of apple: " << A.totalPixels << std::endl;


    //perform bitwise on mask
    cv::Mat finalApple;
    cv::Mat notMask;
    //cv::bitwise_not(imgBack, desiredImage);
    cv::bitwise_and(A.origImage, A.origImage, finalApple, imgBack);
    //final image is colored apple & black background
    return finalApple;

}

cv::Mat getRed(appleInfo A){
    //HSV values


    cv::Mat leftThres;
    cv::Mat noBackThres;
    cv::Mat finalApple;
    cvtColor(A.noBackImage, noBackThres, cv::COLOR_BGR2HSV);


    cv::Scalar redLeftLowRange(0,150,100);
    cv::Scalar redLeftHighRange(15,255,255);

    //create mask
    cv::inRange(noBackThres, redLeftLowRange, redLeftHighRange, leftThres);

    
    cv::bitwise_or(noBackThres, noBackThres, finalApple, leftThres);

    A.redPixels = cv::countNonZero(leftThres);
    std::cout << "redPixels: " << A.redPixels << std::endl;
    std::cout << "totalPixels: " << A.totalPixels << std::endl;
    A.redImage = finalApple;

    return finalApple;
}

double getColorPercent(appleInfo A){

    double percent = (A.redPixels / A.totalPixels) *100.00;
    //std::cout << "percent red: " << percent << std::endl;
    return percent;
}