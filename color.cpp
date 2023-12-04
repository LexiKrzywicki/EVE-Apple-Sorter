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

    cv::Mat removeBackground(cv::Mat desiredImage){

        cv::Mat imgBack;

        cv::Scalar backLow(0,150,100);
        cv::Scalar backHigh(35,255,255);

        cv::inRange(desiredImage, backLow, backHigh, imgBack);

        //calculates total number of pixels that are no black - should only be apple
        double totalImagePixels = imgBack.cols * imgBack.rows;
        totalPixels = totalImagePixels - cv::countNonZero(imgBack); 


        //perform bitwise on mask
        cv::Mat finalApple;
        cv::bitwise_and(origImage, origImage, finalApple, imgBack);

        //final image is colored apple & black background
        return finalApple;

    }

    cv::Mat getRed(){
        //HSV values


        cv::Mat thres;
        cv::Mat noBackHSV;
        cv::Mat finalRed;


        cvtColor(noBackImage, noBackHSV, cv::COLOR_BGR2HSV);


        cv::Scalar redLeftLowRange(0,150,100);
        cv::Scalar redLeftHighRange(15,255,255);

        //create mask
        cv::inRange(noBackHSV, redLeftLowRange, redLeftHighRange, thres);

        
        cv::bitwise_or(noBackHSV, noBackHSV, finalRed, thres);

        //counts all non black pixels - should be only red pixels
        redPixels = cv::countNonZero(thres);
        std::cout << "redPixels: " << redPixels << std::endl;
        std::cout << "totalPixels: " << totalPixels << std::endl;
        redImage = finalRed;

        return finalRed;
    }

    double getColorPercent(){

        double percent = (redPixels / totalPixels) *100.00;
        //std::cout << "percent red: " << percent << std::endl;
        return percent;
    }


};

// void getAppleSize(int, appleInfo A, void* );   //originally threshold
// cv::Mat removeBackground(cv::Mat desiredImage, appleInfo A);
// cv::Mat getRed(appleInfo A);
// double getColorPercent(appleInfo A);


int main( int argc, char** argv )
{
    appleInfo apple;
    //get image
    apple.origImage = cv::imread(argv[1], cv::IMREAD_COLOR);

    apple.origImage = apple.origImage(cv::Range(70,400), cv::Range(40, 610));

    

    //convert to HSV
    cvtColor(apple.origImage, appleHSV, cv::COLOR_BGR2HSV);


//THIS IS FOR RED COLOR PERCENT!!!

    apple.noBackImage = apple.removeBackground(appleHSV);


    apple.redImage = apple.getRed();

    double percentage = apple.getColorPercent();
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


// void getAppleSize(int, appleInfo A, void* )
// {

//     //the following appleInfo.middle is changed to appleInfo.image for test originally appleInfo.middle

//  cv::Mat canny_output;

//  Canny(appleInfo.greyImage, canny_output, thresh, thresh*2 );  // used to detect edges
 
//  std::vector<std::vector<cv::Point> > contours;
//  findContours( canny_output, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE );  //find and save contours
 
//  //apply approximations to polygon with accuracy and find bounding rect for every polygon
//  //find min. enclosing circle for every polygon
//  std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
//  std::vector<cv::Rect> boundRect( contours.size() );
//  std::vector<cv::Point2f>centers( contours.size() );
//  std::vector<float>radius( contours.size() );
//  int accuracy = 3;
//  for( size_t i = 0; i < contours.size(); i++ )
//  {
//     cv::approxPolyDP( contours[i], contours_poly[i], accuracy, true );
//     boundRect[i] = cv::boundingRect( contours_poly[i] );
//     minEnclosingCircle( contours_poly[i], centers[i], radius[i] );
//  }

//  drawing = cv::Mat::zeros( canny_output.size(), CV_8UC3 );
//  int prevRadius = 0;
//  int element = 0;
//  for( size_t i = 0; i< contours.size(); i++ )
//  {
//     cv::Scalar color = cv::Scalar( rng.uniform(0, 256), rng.uniform(0,256), rng.uniform(0,256) );
//     cv::drawContours( appleInfo.greyImage, contours_poly, (int)i, color );
//     cv::rectangle( appleInfo.greyImage, boundRect[i].tl(), boundRect[i].br(), color, 1 );
//     cv::circle( appleInfo.greyImage, centers[i], (int)radius[i], color, 2 );

//     if((int)radius[i] > prevRadius){
//         //appleInfo.diameter = (int)radius[i];  //this gets radius which might be bigger then apple
//         prevRadius = radius[i];
//         element = i;
//     }
//  }

//  int x1 = boundRect[element].tl().x;
//  int x2 = boundRect[element].br().x;

// appleInfo.diameter = x2 - x1;


// //  std::cout<< "boundRect: " << x1 << std::endl;
// //  std::cout<< "boundRect: " << x2 << std::endl;
//  //std::cout<< "radius: " << radius[element] << std::endl;

//  std::cout << "diameter: " << appleInfo.diameter << " pixels" << std::endl;
    
//     //cv::imshow( "Contours", imgApple );


// }

