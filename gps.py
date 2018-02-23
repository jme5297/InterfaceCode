import serial
import Adafruit_BBIO.UART as UART
UART.setup("UART1")
GPS = serial.Serial('/dev/ttyO1', 9600)

while (1):
	while GPS.inWaiting()==0:
		pass
	NMEA=GPS.readline()
	print NMEA
