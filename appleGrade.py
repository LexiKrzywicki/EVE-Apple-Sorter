import cv2
import numpy as np

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
        img_back = cv2.inRange(self.hsv, np.array([0, 150, 50]), np.array([35, 255, 255]))
        self.total_pixels = cv2.countNonZero(img_back)
        print(self.total_pixels)
        final_apple = cv2.bitwise_and(self.orig_image, self.orig_image, mask=img_back)
        return final_apple

    def get_red(self):
        thres = cv2.inRange(cv2.cvtColor(self.no_back_image, cv2.COLOR_BGR2HSV),
                            np.array([0, 150, 100]), np.array([13, 255, 255]))
        final_red = cv2.bitwise_and(self.no_back_image, self.no_back_image, mask=thres)
        self.red_pixels = cv2.countNonZero(thres)
        self.red_image = cv2.cvtColor(final_red, cv2.COLOR_HSV2BGR)
        print(self.red_pixels)
        return self.red_image

    def get_color_percent(self):
        print(self.red_pixels, self.total_pixels)
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


def main():
    cap = cv2.VideoCapture(2)
    if not cap.isOpened():
        print("ERROR! Unable to open camera")
        return -1

    result, image = cap.read()

    if result:
        cv2.namedWindow("Resized_window", cv2.WINDOW_NORMAL)
        cv2.resizeWindow("Resized_window", 640, 480)
        cv2.imshow("Resized_window", image)
        #cv2.imshow("Image Taken", image[440:1040, 200:1580])
        cv2.waitKey(0)

    apple = AppleInfo(image, image, image, image, image, image, image)
    #apple.orig_image = image[140:440, 10:630]
    apple.orig_image = image

    apple.hsv = cv2.cvtColor(apple.orig_image, cv2.COLOR_BGR2HSV)

    apple.no_back_image = apple.remove_background()

    apple.red_image = apple.get_red()

    apple.get_color_percent()
    print(apple.percent_red)

    apple.apple_shape = apple.get_shape()

    print("GRADE:", apple.grade())

    # cv2.imshow("Original", apple.orig_image)
    # cv2.imshow("No Background", apple.no_back_image)
    # cv2.imshow("Red", apple.red_image)
    # cv2.imshow("Shape", apple.apple_shape)

    apple.erosion()
    apple.dilation()

    #cv2.waitKey(0)


if __name__ == "__main__":
    main()
