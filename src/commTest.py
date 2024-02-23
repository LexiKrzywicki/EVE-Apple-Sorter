import time
import serial

arduino = serial.Serial(port='COM3', baudrate=9600, timeout=.1)
arduino.bytesize = 8
arduino.parity = 'N'

time.sleep(2)
once = True

state = "waiting"

while True:
    arduino.reset_input_buffer()
    arduino.reset_output_buffer()
    while state == "waiting":
        arduino.write(b'A')
        if arduino.read() == b'P':
            state = "pytorch"
            break
        break
    while state == "pytorch":
        #print("IN PYTROCH")

        #run pytorch script
        #if bruise detected move servo.
        state = "servo"
        break

        #if no surface defect run openCV then run servo


    while state == "servo":
        arduino.write(b'B')
        
        if arduino.read() == b'D':
            state = "waiting"
            arduino.reset_input_buffer()
            arduino.reset_output_buffer()
        break
            

