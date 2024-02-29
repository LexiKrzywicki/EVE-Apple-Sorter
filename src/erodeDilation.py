from __future__ import print_function
import cv2 as cv
import numpy as np
import argparse
src = None
erosion_size = 0
max_elem = 2
max_kernel_size = 21
title_trackbar_element_shape = 'Element:\n 0: Rect \n 1: Cross \n 2: Ellipse'
title_trackbar_kernel_size = 'Kernel size:\n 2n +1'
title_erosion_window = 'Erosion Demo'
title_dilation_window = 'Dilation Demo'
def main():
    global src
    global imgApple
    src = cv.imread("Prediction.png")
    hsv = cv.cvtColor(src, cv.COLOR_BGR2HSV)
    imgApple = remove_background(hsv)
    # cv.namedWindow(title_erosion_window)
    # cv.createTrackbar(title_trackbar_element_shape, title_erosion_window, 0, max_elem, erosion)
    # cv.createTrackbar(title_trackbar_kernel_size, title_erosion_window, 0, max_kernel_size, erosion)
    # cv.namedWindow(title_dilation_window)
    # cv.createTrackbar(title_trackbar_element_shape, title_dilation_window, 0, max_elem, dilatation)
    # cv.createTrackbar(title_trackbar_kernel_size, title_dilation_window, 0, max_kernel_size, dilatation)
    eroded = erosion(2, imgApple)
    dilated = dilatation(10, eroded)
    eroded2 = erosion(6, dilated)
    dilatation(4, eroded2)
    cv.imshow("original",  imgApple)
    #dilatation(10, eroded)
    cv.waitKey()
    
def remove_background(img):
        #img_back = cv2.inRange(self.hsv, np.array([0, 150, 50]), np.array([35, 255, 255]))
        img_back = cv.inRange(img, np.array([0, 112, 0]), np.array([35, 255, 255]))
        img_back1 = cv.inRange(img, np.array([160, 112, 0]), np.array([180, 255, 255]))
        full = img_back + img_back1
        #print(self.total_pixels)
        final_apple = cv.bitwise_and(src, src, mask=full)
        return final_apple
# optional mapping of values with morphological shapes
def morph_shape(val):
    if val == 0:
        return cv.MORPH_RECT
    elif val == 1:
        return cv.MORPH_CROSS
    elif val == 2:
        return cv.MORPH_ELLIPSE
def erosion(erosionVal, input):
    #erosion_size = cv.getTrackbarPos(title_trackbar_kernel_size, title_erosion_window)
    #erosion_shape = morph_shape(cv.getTrackbarPos(title_trackbar_element_shape, title_erosion_window))
    erosion_shape = 0  #rect
    element = cv.getStructuringElement(erosion_shape, (2 * erosionVal + 1, 2 * erosionVal + 1),
                                       (erosionVal, erosionVal))
    
    erosion_dst = cv.erode(input, element)
    cv.imshow(title_erosion_window, erosion_dst)
    return erosion_dst
def dilatation(dilationVal,  input):
    #dilatation_size = cv.getTrackbarPos(title_trackbar_kernel_size, title_dilation_window)
    #dilation_shape = morph_shape(cv.getTrackbarPos(title_trackbar_element_shape, title_dilation_window))
    dilation_shape = 1 #cross 
    element = cv.getStructuringElement(dilation_shape, (2 * dilationVal + 1, 2 * dilationVal + 1),
                                       (dilationVal, dilationVal))
    dilatation_dst = cv.dilate(input, element)
    cv.imshow(title_dilation_window, dilatation_dst)
    return dilatation_dst
if __name__ == "__main__":
    main()