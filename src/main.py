import cv2
import numpy as np
import time
import serial
import os
import subprocess

import appleGrade
import inference
import capture

arduino = serial.Serial(port='COM3', baudrate=9600, timeout=.1)
time.sleep(2)

state = "waiting"


def state_machine(state):
    match state:
        case "waiting":
            #print("WAITING")
            if arduino.in_waiting > 0:
                myData = ord(arduino.read())
                if myData < 15:
                    state = "capture"
        case "capture":
            print("CAPTURE")
            if len(inference.inference()) > 0:
                state = "opencv"
            else:
                print("G1 Apple")


if __name__ == "__main__":
   # stuff only to run when not called via 'import' here
    while True:
        if arduino.in_waiting > 0: 
            myData = ord(arduino.read())
            print(myData)
            if myData < 14:
                print("applefound")
                image = capture.capture()
                if len(inference.inference(image)) > 0:
                    print("GOTTEM")
                    exit()
        


#if apple detected by ultrasonic take picture of apple
#run ML first
#run OpenCV next

# if arduino.in_waiting > 0:
#     print("apple detected")
#     myData = ord(arduino.read())
#     print(myData)
    # time_start = int(time.time() * 1000)

    # #if no surface defect, then G1 apple
    # #captures image in inference.py
    # if len(inference.pred_classes) > 0:
    #     print("Apple is G2")
    # else:
    #     print("NOW RUNNING OPENCV")
    #     image = capture.cropped
    #     apple = appleGrade.AppleInfo(image, image, image, image, image, image, image)
    #     apple.orig_image = image

    #     apple.hsv = cv2.cvtColor(apple.orig_image, cv2.COLOR_BGR2HSV)

    #     apple.no_back_image = apple.remove_background()

    #     apple.red_image = apple.get_red()
    #     apple.get_color_percent()
    #     print("Percent Red = ", apple.percent_red)

    #     apple.apple_shape = apple.get_shape()
    #     print("GRADE:", apple.grade())

    #     time_end = int(time.time() * 1000)

    #     time_total = time_end - time_start
    #     print("Time(s) =", time_total)

    #     cv2.imshow("Original", apple.no_back_image)
    #     cv2.waitKey(0)
    #     print("OPENCV COMPLETE")


    






    