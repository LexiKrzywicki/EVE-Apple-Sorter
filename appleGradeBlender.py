import cv2
import numpy as np
import sys

class AppleInfo:
    def __init__(self, orig_image, hsv, back_image, red_image, erosion_image, dilation_image, apple_shape):
        self.orig_image = orig_image
        self.hsv = hsv
        self.no_back_image = back_image
        self.red_image = red_image
        self.erosion_image = erosion_image
        self.dilation_image = dilation_image
        self.apple_shape = apple_shape
        self.total_pixels = 0
        self.red_pixels = 0
        self.percent_red = 0

        # for erosion and dilation
        self.erosion_elem = 0
        self.erosion_size = 2
        self.dilation_elem = 0
        self.dilation_size = 2
        self.max_elem = 2
        self.max_kernel_size = 21

        self.apple0 = None
        self.apple1 = None
        self.apple2 = None

    def remove_background(self):

        low_range = np.array([0, 0, 0])
        high_range = np.array([180, 210, 230])

        full_mask = cv2.inRange(self.hsv, low_range, high_range)

        self.total_pixels = cv2.countNonZero(full_mask)
        print(self.total_pixels)
        final_apple = cv2.bitwise_and(self.orig_image, self.orig_image, mask=full_mask)
        return final_apple

    def get_red(self):
        no_back_hsv = cv2.cvtColor(self.no_back_image, cv2.COLOR_BGR2HSV)

        back_low = np.array([0, 0, 0])
        back_high = np.array([30, 255, 255])

        back_bottom = np.array([140, 0, 0])
        back_top = np.array([179, 255, 255])

        mask_left = cv2.inRange(no_back_hsv, back_low, back_high)
        mask_right = cv2.inRange(no_back_hsv, back_bottom, back_top)
        full_mask = mask_left + mask_right

        final_red = cv2.bitwise_and(no_back_hsv, no_back_hsv, mask=full_mask)
        self.red_pixels = cv2.countNonZero(final_red)
        print(self.red_pixels)
        self.red_image = cv2.cvtColor(final_red, cv2.COLOR_HSV2BGR)
        return self.red_image

    def get_color_percent(self):
        print(self.total_pixels)
        print(self.red_pixels)
        self.percent_red = (self.red_pixels / self.total_pixels) * 100.00
        return self.percent_red

    def erosion(self):
        erosion_type = [cv2.MORPH_RECT, cv2.MORPH_CROSS, cv2.MORPH_ELLIPSE][self.erosion_elem]
        element = cv2.getStructuringElement(erosion_type,
                                            (2 * self.erosion_size + 1, 2 * self.erosion_size + 1),
                                            (self.erosion_size, self.erosion_size))
        self.erosion_image = cv2.erode(self.no_back_image, element)
        return self.erosion_image

    def dilation(self):
        dilation_type = [cv2.MORPH_RECT, cv2.MORPH_CROSS, cv2.MORPH_ELLIPSE][self.dilation_elem]
        element = cv2.getStructuringElement(dilation_type,
                                            (2 * self.dilation_size + 1, 2 * self.dilation_size + 1),
                                            (self.dilation_size, self.dilation_size))
        self.dilation_image = cv2.dilate(self.erosion_image, element)
        return self.dilation_image

    def new_cropped_img(self, rect_shape):
        x, y, width, height = rect_shape
        my_roi = (x, y, width + 1, height + 1)
        cropped = self.no_back_image[y:y + height + 1, x:x + width + 1]
        return cropped

    def apple_ratio(self, desired_apple):
        width, height, _ = desired_apple.shape
        desired_gray = cv2.cvtColor(desired_apple, cv2.COLOR_BGR2GRAY)
        total_area = width * height
        non_zero = cv2.countNonZero(desired_gray)
        return (non_zero / total_area) * 100.00

    def separate(self):
        no_back_gray = cv2.cvtColor(self.dilation_image, cv2.COLOR_BGR2GRAY)
        _, binary_thresh = cv2.threshold(no_back_gray, 100, 255, cv2.THRESH_BINARY)
        contours, _ = cv2.findContours(binary_thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_NONE)

        contours_poly = [cv2.approxPolyDP(contour, 3, True) for contour in contours]
        bound_rect = [cv2.boundingRect(poly) for poly in contours_poly]
        rect_area = sorted(bound_rect, key=lambda x: x[2] * x[3], reverse=True)[:3]

        drawing = np.zeros_like(self.dilation_image)
        self.apple_shape = self.no_back_image.copy()

        for i, (poly, rect) in enumerate(zip(contours_poly, bound_rect)):
            color = tuple(np.random.randint(0, 256, 3).tolist())
            cv2.rectangle(self.apple_shape, rect[0:2], (rect[0] + rect[2], rect[1] + rect[3]), color, 2)

            if i == 0:
                self.apple0 = (self.new_cropped_img(rect), self.apple_ratio(self.new_cropped_img(rect)))
            elif i == 1:
                self.apple1 = (self.new_cropped_img(rect), self.apple_ratio(self.new_cropped_img(rect)))
            elif i == 2:
                self.apple2 = (self.new_cropped_img(rect), self.apple_ratio(self.new_cropped_img(rect)))

        return self.apple_shape

    def grade(self):
        str_grade = "Cider"

        if self.percent_red > 60:
            if all(apple[1] > 78.4 for apple in [self.apple0, self.apple1, self.apple2]):
                str_grade = "G1"
            else:
                str_grade = "G2"
                print("bad ratios")
        elif self.percent_red > 45:
            str_grade = "G2"
            print("red percentage not high enough")
        else:
            print("low red percentage")

        print("GRADE:", str_grade)
        return str_grade


def main():
    image = cv2.imread(sys.argv[1], cv2.IMREAD_COLOR)
    apple = AppleInfo(image, image, image, image, image, image, image)

    # convert to HSV
    apple.hsv = cv2.cvtColor(apple.orig_image, cv2.COLOR_BGR2HSV)

    apple.no_back_image = apple.remove_background()

    apple.red_image = apple.get_red()

    apple.get_color_percent()
    print(apple.percent_red)

    apple.erosion_image = apple.erosion()
    apple.dilation_image = apple.dilation()

    apple.apple_shape = apple.separate()

    print(apple.grade())

    # shows images
    cv2.imshow("Original", apple.orig_image)
    cv2.imshow("No Background", apple.no_back_image)
    cv2.imshow("Red", apple.red_image)
    cv2.imshow("Erosion", apple.erosion_image)
    cv2.imshow("Dilation", apple.dilation_image)
    cv2.imshow("Apple0", apple.apple0[0])
    cv2.imshow("Apple1", apple.apple1[0])
    cv2.imshow("Apple2", apple.apple2[0])
    cv2.imshow("Shape", apple.apple_shape)

    cv2.waitKey(0)


if __name__ == "__main__":
    main()
