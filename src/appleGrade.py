import cv2
import numpy as np
import time
#import RPi.GPIO as GPIO


# GPIO.setmode(GPIO.BCM)

# GPIO_TRIGGER = 18
# GPIO_ECHO = 24

# GPIO.setup(GPIO_TRIGGER, GPIO.OUT)
# GPIO.setup(GPIO_ECHO, GPIO.IN)

# def distance():
#     # set Trigger to HIGH
#     GPIO.output(GPIO_TRIGGER, True)
 
#     # set Trigger after 0.01ms to LOW
#     time.sleep(0.00001)
#     GPIO.output(GPIO_TRIGGER, False)
 
#     StartTime = time.time()
#     StopTime = time.time()
 
#     # save StartTime
#     while GPIO.input(GPIO_ECHO) == 0:
#         StartTime = time.time()
 
#     # save time of arrival
#     while GPIO.input(GPIO_ECHO) == 1:
#         StopTime = time.time()
 
#     # time difference between start and arrival
#     TimeElapsed = StopTime - StartTime
#     # multiply with the sonic speed (34300 cm/s)
#     # and divide by 2, because there and back
#     distance = (TimeElapsed * 34300) / 2
 
#     return distance

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
        self.erosion_elem = 0
        self.erosion_size = 5
        self.dilation_elem = 0
        self.dilation_size = 2
        self.max_elem = 2
        self.max_kernel_size = 21

    def remove_background(self):
        #img_back = cv2.inRange(self.hsv, np.array([0, 150, 50]), np.array([35, 255, 255]))
        img_back = cv2.inRange(self.hsv, np.array([0, 112, 0]), np.array([35, 255, 255]))
        img_back1 = cv2.inRange(self.hsv, np.array([160, 112, 0]), np.array([180, 255, 255]))
        full = img_back + img_back1
        self.total_pixels = cv2.countNonZero(full)
        #print(self.total_pixels)
        final_apple = cv2.bitwise_and(self.orig_image, self.orig_image, mask=full)
        return final_apple

    def get_red(self):
        thres = cv2.inRange(cv2.cvtColor(self.no_back_image, cv2.COLOR_BGR2HSV),
                            np.array([0, 150, 100]), np.array([13, 255, 255]))
        final_red = cv2.bitwise_and(self.no_back_image, self.no_back_image, mask=thres)
        self.red_pixels = cv2.countNonZero(thres)
        self.red_image = cv2.cvtColor(final_red, cv2.COLOR_HSV2BGR)
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

    def erosion(self):
        erosion_type = [cv2.MORPH_RECT, cv2.MORPH_CROSS, cv2.MORPH_ELLIPSE][self.erosion_elem]
        element = cv2.getStructuringElement(erosion_type,
                                            (2 * self.erosion_size + 1, 2 * self.erosion_size + 1),
                                            (self.erosion_size, self.erosion_size))
        cv2.erode(self.no_back_image, self.erosion_image, element)

    def dilation(self):
        dilation_type = [cv2.MORPH_RECT, cv2.MORPH_CROSS, cv2.MORPH_ELLIPSE][self.dilation_elem]
        element = cv2.getStructuringElement(dilation_type,
                                            (2 * self.dilation_size + 1, 2 * self.dilation_size + 1),
                                            (self.dilation_size, self.dilation_size))
        cv2.dilate(self.no_back_image, self.dilation_image, element)

    def grade(self):
        if self.percent_red > 60:
            return "G1"
        elif self.percent_red > 45:
            return "G2"
        else:
            return "Cider"


# def main():

#     # while distance() > 15.0:
#     #     print("Measured Distance = %.1f cm" % distance())
#     #     time.sleep(2)
    
#     # print("found_apple")
    
#     time.sleep(3) #gives time for apple to roll to position

#     time_start = int(time.time() * 1000)

#     # cap = cv2.VideoCapture(2)
#     cap = cv2.VideoCapture(0) #for raspi
 
#     # cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
#     # cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
#     if not cap.isOpened():
#         print("ERROR! Unable to open camera")
#         return -1

#     result, image = cap.read()

#     time_capture = int(time.time() * 1000)

#     apple = AppleInfo(image, image, image, image, image, image, image)

#     apple.orig_image = image

#     apple.hsv = cv2.cvtColor(apple.orig_image, cv2.COLOR_BGR2HSV)

#     apple.no_back_image = apple.remove_background()

#     apple.red_image = apple.get_red()

#     apple.get_color_percent()
#     print("Percent Red = ", apple.percent_red)

#     apple.erosion()
#     apple.dilation()

#     apple.apple_shape = apple.get_shape()

#     print("GRADE:", apple.grade())

#     time_end = int(time.time() * 1000)
#     time_cap = time_capture - time_start

#     time_total = time_end - time_start

#     print("total time (ms): ", time_total)
#     print("capture time (ms): ", time_cap)

#     # cv2.namedWindow("Original", cv2.WINDOW_NORMAL)
#     # cv2.resizeWindow("Original", 640, 480)
#     # cv2.imshow("Original", apple.orig_image)
#     # cv2.namedWindow("No Background", cv2.WINDOW_NORMAL)
#     # cv2.resizeWindow("No Background", 640, 480)
#     # cv2.imshow("No Background", apple.no_back_image)
#     # cv2.namedWindow("Red", cv2.WINDOW_NORMAL)
#     # cv2.resizeWindow("Red", 640, 480)
#     # cv2.imshow("Red", apple.red_image)
#     # cv2.namedWindow("Shape", cv2.WINDOW_NORMAL)
#     # cv2.resizeWindow("Shape", 640, 480)
#     # cv2.imshow("Shape", apple.apple_shape)

#     # cv2.waitKey(0)


# if __name__ == "__main__":
#     main()
