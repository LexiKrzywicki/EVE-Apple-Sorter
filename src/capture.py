import cv2

cap = cv2.VideoCapture(1)  #lenovo

if not cap.isOpened():
    print("ERROR! Unable to open camera")

result, image = cap.read()