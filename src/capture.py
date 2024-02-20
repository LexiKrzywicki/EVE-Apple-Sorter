import cv2


def capture():
    cap = cv2.VideoCapture(0)  #lenovo

    if not cap.isOpened():
        print("ERROR! Unable to open camera")

    result, image = cap.read()

    cropped = image[110:380, 0:630]

    return cropped

    #cv2.imwrite("image.jpg", cropped)

    # cv2.imshow('Image from Camera', cropped)
    # cv2.waitKey(0)
