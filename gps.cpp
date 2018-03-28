#include <stdio.h>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <stdlib.h>
#include <string.h>
unsigned char rx_buffer[80];
void process();
void data_GR(char * buffer, int loc);
double lat, lon, cog;
int main(){

	int uart0_filestream = -1;


	uart0_filestream = open("/dev/ttyO4", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}

	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);

	if (uart0_filestream != -1)
	{
    while(1){

			char temp = uart0_filestream;

			int i = 0;
			if(temp != '\n' && i < 80){
				rx_buffer[i] = temp;
				i++;
			}
			else{
				rx_buffer[i] = '\0';
				i=0;
				process();
			}
  		}
    }


  //----- CLOSE THE UART -----
//	close(uart0_filestream);

  return 0;
}
void process(){
char field[20];
data_GR(field, 0);
if (strcmp(field, "$GPRMC") == 0){
	data_GR(field, 3);
		lat = strtod(field,NULL);
	data_GR(field, 5);
		lon = strtod(field,NULL);
	data_GR(field, 8);
		cog =  strtod(field,NULL);
}
}
void data_GR(char * buffer, int loc){
int sentencePos =0;
int fieldPos = 0;
int commacount =0;
while (sentencePos < 80){
if(rx_buffer[sentencePos] == ','){
	commacount ++;
	sentencePos ++;
}
if(commacount == loc){
buffer[fieldPos] = rx_buffer[sentencePos];
fieldPos++;
}
sentencePos++;
}
buffer[fieldPos] = '\0';
}
