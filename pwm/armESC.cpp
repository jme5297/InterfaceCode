#include <unistd.h>
#include <iostream>
#include <fstream>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#define PRU_NUM0 0

unsigned int dc0;
unsigned int dp0;
double dutyCycle_speed;
unsigned int modeOn = 1;

void WriteDutyCycle(double dc){
		dc0 = static_cast<unsigned int>(dc * 1000.0);
		if(dc0 == static_cast<unsigned int>(1000.0)){ dc0 -= 1; }
		if(dc0 == 0){ dc = 1; }
		prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 1, &dc0, 4);
}

int main(){

  dutyCycle_speed = 0.150;
	dc0 = static_cast<unsigned int>(dutyCycle_speed * 1000.0);
	dp0 = static_cast<unsigned int>(999);

  std::cout << "Initializing PRU...\n";
  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
	prussdrv_init();
	prussdrv_open(PRU_EVTOUT_0);
	prussdrv_pruintc_init(&pruss_intc_initdata);
	prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 1, &dc0, 4);
	prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 2, &dp0, 4);
	prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 3, &modeOn, 4);
	prussdrv_exec_program(PRU_NUM0, "./pru1.bin");
  usleep(500000);

  std::cout << "Calibrating forward...\n";
  dutyCycle_speed = 0.155;
  WriteDutyCycle(dutyCycle_speed);
  usleep(500000);

  std::cout << "Calibrating to zero...\n";
  dutyCycle_speed = 0.150;
  WriteDutyCycle(dutyCycle_speed);
  usleep(500000);

  std::cout << "ESC IS ARMED!\n";
  return 0;

}
