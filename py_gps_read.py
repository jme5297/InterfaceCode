import serial
import Adafruit_BBIO.UART as UART
UART.setup("UART4")
GPS = serial.Serial('/dev/ttyO4', 57600)

while(1):
	while GPS.inWaiting() == 0:
		pass
	NMEA = GPS.readline()

	if "GPRMC" in NMEA:
		print NMEA
