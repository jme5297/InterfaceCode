import serial
import Adafruit_BBIO.UART as UART
UART.setup("UART4")
ser = serial.Serial('/dev/ttyO4', 9600)
from time import sleep
UPDATE_RT = "$PMTK220,200*2C\r\n"
MEAS_RT = "$PMTK300,200,0,0,0,0*2F\r\n"
BAUD_RT = "$PMTK251,57600*2C\r\n"
ser.write(BAUD_RT)
sleep(1)
ser.baudrate=57600
ser.write(UPDATE_RT)
sleep(1)
ser.write(MEAS_RT)
sleep(1)
ser.flushInput()
ser.flushOutput()
print "Update rates sent!"
