#include <unistd.h>
#include <iostream>
#include <fstream>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#define PRU_NUM1 1

std::ofstream tReader;
std::string gpio_steer = "/sys/class/gpio/gpio" + std::to_string(49) + "/value";
std::string gpio_payload = "/sys/class/gpio/gpio" + std::to_string(117) + "/value";
unsigned int dc1;
unsigned int dp1;
double dutyCycle_str;
unsigned int modeOn = 1;
unsigned int modeOff = 0;

void WriteDutyCycle(double dc){
		dc1 = static_cast<unsigned int>(dc * 1000.0);
		if(dc1 == static_cast<unsigned int>(1000.0)){ dc1 -= 1; }
		if(dc1 == 0){ dc = 1; }
		prussdrv_pru_write_memory(PRUSS0_PRU1_DATARAM, 1, &dc1, 4);
}

int main(){

	std::cout << "Activating steering servo switch...\n";
	tReader.open(gpio_payload);
	tReader << 0;
	tReader.close();
	tReader.open(gpio_steer);
	tReader << 1;
	tReader.close();

	dutyCycle_str = 0.074;
	dc1 = static_cast<unsigned int>(dutyCycle_str * 1000.0);
	dp1 = static_cast<unsigned int>(1999);

  std::cout << "Initializing steering servo...\n";
  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
	prussdrv_init();
	prussdrv_open(PRU_EVTOUT_1);
	prussdrv_pruintc_init(&pruss_intc_initdata);
	prussdrv_pru_write_memory(PRUSS0_PRU1_DATARAM, 1, &dc1, 4);
	prussdrv_pru_write_memory(PRUSS0_PRU1_DATARAM, 2, &dp1, 4);
	prussdrv_pru_write_memory(PRUSS0_PRU1_DATARAM, 3, &modeOn, 4);
	prussdrv_exec_program(PRU_NUM1, "./pru2.bin");
  usleep(1000000);

	std::cout << "Right turn...\n";
  dutyCycle_str = 0.058;
  WriteDutyCycle(dutyCycle_str);
  usleep(1000000);

  std::cout << "Left turn...\n";
  dutyCycle_str = 0.088;
  WriteDutyCycle(dutyCycle_str);
  usleep(1000000);

	std::cout << "Straight...\n";
  dutyCycle_str = 0.074;
  WriteDutyCycle(dutyCycle_str);
  usleep(1000000);

  std::cout << "Steering servo test complete.\n";

	tReader.open(gpio_steer);
	tReader << 0;
	tReader.close();
	prussdrv_pru_write_memory(PRUSS0_PRU1_DATARAM, 3, &modeOff, 4);
	prussdrv_exec_program(PRU_NUM1, "./pru2.bin");

  return 0;

}
