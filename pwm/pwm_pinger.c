// Program to test execution of PRUSS code (.p)
// Written by amc6630 on 3/13/2018
/*
*   Before running, please run this command: config-pin P9.27 pruout
*   For compiling both files:
    gcc pwm_pinger.c -o pwm_test -lpthread -lprussdrv
    pasm -b pwm_final.p
*/

#include <stdio.h>
#include <stdlib.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <unistd.h>

#define PRU_NUM   0

int main(void) {
  int delay_period = 10;
  int ping_val = 1;
  int duty_cycle = 5;
  int mode = 1;
  prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0, &ping_val, 4);
  prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 1, &duty_cycle, 4);
  prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 2, &delay_period, 4);
  prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 3, &mode, 4);
  usleep(100000);
  ping_val = 2;
  while(1) {
    ping_val = ping_val + 1;
    if (ping_val > 7) {
      ping_val = 1;
    }
    duty_cycle = duty_cycle + 5;
    if (duty_cycle > 100) {
      duty_cycle = 5;
    }
    prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0, &ping_val, 4);
    prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 1, &duty_cycle, 4);
    usleep(100000);
  }
  return 0;
}
