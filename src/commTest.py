import time
import serial

arduino = serial.Serial(port='COM3', baudrate=9600, timeout=.1)
arduino.bytesize = 8
arduino.parity = 'N'

time.sleep(2)

state = "waiting"

while True:
    while state == "waiting":
        if arduino.in_waiting > 0:
            myData = ord(arduino.read())
            if myData < 20:
                print("applefound")
                state = "light"
    while state == "light":
        arduino.write(b'A')
        time.sleep(2)
        arduino.flush()
        state = "waiting"

