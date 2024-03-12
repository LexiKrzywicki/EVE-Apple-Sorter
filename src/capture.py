import cv2
import sys

cap = cv2.VideoCapture(1, cv2.CAP_MSMF)  #lenovo

def capture():
    #cap = cv2.VideoCapture(1, cv2.CAP_DSHOW)
    #cap = cv2.VideoCapture(1)

    if not cap.isOpened():
        print("ERROR! Unable to open camera")

    result, image = cap.read()

    if result:
        cropped = image[120:410, 0:630]
        cv2.imwrite("Image.png", cropped)

    else:
        sys.exit("no image captured")

    return cropped
