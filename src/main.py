import cv2
import numpy as np
import time
import serial

import appleGrade
import inference
import capture


arduino = serial.Serial(port='COM3', baudrate=9600, timeout=.1)
time.sleep(2)

state = "waiting"

while True:
    arduino.reset_input_buffer()
    arduino.reset_output_buffer()
    while state == "waiting":
        arduino.write(b'A')
        if arduino.read() == b'P':
            print("apple found")
            time_start = int(time.time() * 1000)
            image = capture.capture()
            cv2.imwrite("Prediction.jpg", image)
            state = "pytorch"
            #break
        break
    while state == "pytorch":
        print("PYTORCH")
        predictions = inference.inference(image)
        print("predictions: ", predictions)
        
        # if there are defects
        if len(predictions) > 0:
            print("G2 Apple Detected")
            state = "servo"
        else:
            state = "opencv"

    while state == "opencv":
        print("OPENCV")
        apple = appleGrade.AppleInfo(image, image, image, image, image, image, image)
        apple.orig_image = image

        apple.hsv = cv2.cvtColor(apple.orig_image, cv2.COLOR_BGR2HSV)

        apple.no_back_image = apple.remove_background()

        apple.red_image = apple.get_red()
        apple.get_color_percent()
        print("Percent Red = ", apple.percent_red)

        apple.apple_shape = apple.get_shape()
        print("GRADE:", apple.grade())

        time_end = int(time.time() * 1000)

        time_total = time_end - time_start
        print("Time(s) =", time_total)

        cv2.imshow("Original", apple.no_back_image)
        cv2.waitKey(0)
        print("OPENCV COMPLETE")

        state = "servo"

    while state == "servo":
        arduino.write(b'B')
        
        # reads 'D' when the servo is done moving
        if arduino.read() == b'D':
            state = "waiting"
            arduino.reset_input_buffer()
            arduino.reset_output_buffer()
        break

    






    