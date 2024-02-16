import cv2

cap = cv2.VideoCapture(1)  #lenovo

if not cap.isOpened():
    print("ERROR! Unable to open camera")

result, image = cap.read()

cropped = image[110:380, 0:630]

# cv2.imshow('Image from Camera', cropped)
# cv2.waitKey(0)