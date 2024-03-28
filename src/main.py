import cv2
import numpy as np
import time
import serial

import appleGrade
import inference
import capture


arduino = serial.Serial(port='COM5', baudrate=9600, timeout=.1)
time.sleep(2)

state = "waiting"
grade = 0

while True:
    arduino.reset_input_buffer()
    arduino.reset_output_buffer()
    while state == "waiting":
        arduino.write(b'A')
        print("WROTE A")
        if arduino.read() == b'Z':
            graded = 0
            print("apple found")
            time_start = int(time.time() * 1000)
            time.sleep(1)
            image = capture.capture()
            imagePy = image.copy()
            state = "pytorch"
        break
    while state == "pytorch":
        predictions, boxes = inference.inference(imagePy)
        #print("predictions: ", predictions)
        print("surface defects: ", len(boxes))
        if len(boxes) > 0:        
            print("G2 Apple Detected")
            grade = 2
            state = "visionServo"
        else:
            grade = 1
            state = "opencv"

    while state == "opencv":
        print("running opencv")
        apple = appleGrade.AppleInfo(image, image, image, image, image, image, image)
        apple.orig_image = image
        #cv2.imwrite("OpenCVimage.jpg", apple.orig_image)

        apple.hsv = cv2.cvtColor(apple.orig_image, cv2.COLOR_BGR2HSV)

        apple.no_back_image = apple.remove_background()

        #cv2.imshow("no back", apple.no_back_image)
        #cv2.waitKey(0)

        apple.red_image = apple.get_red()
        #cv2.write --the red image
        apple.get_color_percent()
        print("Percent Red = ", apple.percent_red)

        if apple.percent_red > 60:
            grade = 1
            print("GRADE 1")

        else:
            # apple.dilation()
            # apple.erosion()
            # apple.apple_shape = apple.get_shape()
            grade = 2
            print("GRADE 2")

        #print("OPENCV COMPLETE")
        
        state = "visionServo"

    while state == "visionServo":
        arduino.write(b'B')
        #print("WROTE BBBBBB")
        
        # reads 'Y' when the servo is done moving
        if arduino.read() == b'Y':
            time_end = int(time.time() * 1000)

            time_total = time_end - time_start
            #print("Time(s) =", time_total)
            state = "outtake"
        break


    while state == "outtake":
        if grade == 1:
            arduino.write(b'C')
            if arduino.read() == b'W':
                state = "checkLift"
            break
        else:
            state = "checkLift"

    while state == "checkLift":
        arduino.write(b'D')
        if arduino.read() == b'U':
            state = "waiting"
            print("ready for next apple")
            arduino.reset_input_buffer()
            arduino.reset_output_buffer()
    
    # while state == "outtake":
    #     if grade == 1:
    #         arduino.write(b'C')
    #         if arduino.read() == b'W':
    #             state = "waiting"
    #             print("ready for next apple")
    #             arduino.reset_input_buffer()
    #             arduino.reset_output_buffer()
    #         break
    #     if grade == 2:
    #         arduino.write(b'D')
    #         if arduino.read() == b'U':
    #             state = "waiting"
    #             print("ready for next apple")
    #             arduino.reset_input_buffer()
    #             arduino.reset_output_buffer()
    #         break


    