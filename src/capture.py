import cv2


def capture():
    cap = cv2.VideoCapture(1, cv2.CAP_DSHOW)  #lenovo

    if not cap.isOpened():
        print("ERROR! Unable to open camera")

    result, image = cap.read()

    cropped = image[115:402, 0:630]

    return cropped
