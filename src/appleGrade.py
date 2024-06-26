import cv2
import numpy as np
import time


class AppleInfo:
    def __init__(self, orig_image, hsv, back_image, red_image, erosion_image, dilation_image, apple_shape):
        self.orig_image = None
        self.hsv = None
        self.no_back_image = None
        self.red_image = None
        self.erosion_image = None
        self.dilation_image = None
        self.apple_shape = None
        self.total_pixels = 0
        self.red_pixels = 0
        self.percent_red = 0

        # for erosion and dilation
        self.erosion_elem = 1
        self.erosion_size = 5
        self.dilation_elem = 1
        self.dilation_size = 2
        self.max_elem = 2
        self.max_kernel_size = 21

    def remove_background(self):
        #img_back = cv2.inRange(self.hsv, np.array([0, 150, 50]), np.array([35, 255, 255]))
        img_back = cv2.inRange(self.hsv, np.array([0, 112, 0]), np.array([40, 255, 255]))
        img_back1 = cv2.inRange(self.hsv, np.array([160, 112, 0]), np.array([180, 255, 255]))
        full = img_back + img_back1
        self.total_pixels = cv2.countNonZero(full)
        #print(self.total_pixels)
        final_apple = cv2.bitwise_and(self.orig_image, self.orig_image, mask=full)
        return final_apple

    def get_red(self):
        thres = cv2.inRange(cv2.cvtColor(self.no_back_image, cv2.COLOR_BGR2HSV),
                            np.array([0, 150, 100]), np.array([10, 255, 255]))
        final_red = cv2.bitwise_and(self.no_back_image, self.no_back_image, mask=thres)
        self.red_pixels = cv2.countNonZero(thres)
        self.red_image = cv2.cvtColor(final_red, cv2.COLOR_HSV2BGR)
        #self.red_image = cv2.cvtColor(self.red_image, cv2.COLOR_BGR2HSV)
        #print(self.red_pixels)
        return self.red_image

    def get_color_percent(self):
        #print(self.red_pixels, self.total_pixels)
        self.percent_red = (self.red_pixels / self.total_pixels) * 100.00
        return self.percent_red

    def get_shape(self):
        no_back_gray = cv2.cvtColor(self.no_back_image, cv2.COLOR_HSV2BGR)
        no_back_gray = cv2.cvtColor(no_back_gray, cv2.COLOR_BGR2GRAY)
        _, binary_thresh = cv2.threshold(no_back_gray, 100, 255, cv2.THRESH_BINARY)
        contours, _ = cv2.findContours(binary_thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)
        image_copy = self.no_back_image.copy()
        cv2.drawContours(image_copy, contours, -1, (0, 255, 0), 2)
        return image_copy
    
    def erosion(self, erosionVal, input):
        erosion_shape = 0  #MORPH_RECT
        element = cv2.getStructuringElement(erosion_shape, (2 * erosionVal + 1, 2 * erosionVal + 1),
                                        (erosionVal, erosionVal))
        self.erosion_image = cv2.erode(input, element)
        # cv2.imshow(title_erosion_window, erosion_dst)
        return self.erosion_image

    def dilatation(self, dilationVal,  input):
        #dilatation_size = cv.getTrackbarPos(title_trackbar_kernel_size, title_dilation_window)
        #dilation_shape = morph_shape(cv.getTrackbarPos(title_trackbar_element_shape, title_dilation_window))
        dilation_shape = 1 #cross 
        element = cv2.getStructuringElement(dilation_shape, (2 * dilationVal + 1, 2 * dilationVal + 1),
                                        (dilationVal, dilationVal))
        self.dilation_image = cv2.dilate(input, element)
        return self.dilation_image

    def grade(self):
        if self.percent_red > 60:
            return "G1"
        elif self.percent_red > 45:
            return "G2"
        else:
            return "Cider"


#uncomment when running only appleGrade.py
# def main():

#     # time_start = int(time.time() * 1000)

#     # # cap = cv2.VideoCapture(2)

#     # result, image = cap.read()

#     image = cv2.imread("Image2.png")


#     time_capture = int(time.time() * 1000)

#     apple = AppleInfo(image, image, image, image, image, image, image)

#     apple.orig_image = image

#     apple.hsv = cv2.cvtColor(apple.orig_image, cv2.COLOR_BGR2HSV)

#     apple.no_back_image = apple.remove_background()

#     kernel = np.ones((5, 5), np.uint8) 
#     img_erosion = cv2.erode(apple.no_back_image, kernel, iterations=1) 
#     img_dilation = cv2.dilate(img_erosion, kernel, iterations=1)

#     apple.red_image = apple.get_red()

#     apple.get_color_percent()
#     print("Percent Red = ", apple.percent_red)


#     #apple.erosion()
#     cv2.imshow("original", apple.orig_image)
#     # cv2.imshow("erosion", img_erosion)
#     # cv2.imshow("dilation", img_dilation)
#     cv2.imshow("no back image", apple.no_back_image)
#     cv2.imshow("red percentage", apple.red_image)
#     #cv2.imshow("eroded", apple.erosion_image)

#     cv2.waitKey(0)
#     cv2.destroyAllWindows()
#     #apple.dilation()


#     # apple.apple_shape = apple.get_shape()

#     # print("GRADE:", apple.grade())

#     # time_end = int(time.time() * 1000)
#     # time_cap = time_capture - time_start

#     # time_total = time_end - time_start

#     # print("total time (ms): ", time_total)
#     # print("capture time (ms): ", time_cap)

#     # cv2.waitKey(0)


# if __name__ == "__main__":
#     main()
