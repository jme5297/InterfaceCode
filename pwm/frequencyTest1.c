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
#include <pthread.h>

#define PRU_NUM   0

// Duty Cycle value (updates every user call)
int duty_cycle = 1;
int delay_val = 2000;
int mode = 1;
double initFreq = 50;

void *pruFunc(void *vargp) {
  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
  prussdrv_init ();
  prussdrv_open (PRU_EVTOUT_0);
  prussdrv_pruintc_init(&pruss_intc_initdata);
  prussdrv_exec_program (PRU_NUM, "./pru1.bin");
  delay_val = (int)(1.0/((5e-9)*2.0*1000.0*(double)initFreq) - 1);
  prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 1, &duty_cycle, 4);
  prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 2, &delay_val, 4);
//  prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 2, &newFreq, 4);
  prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 3, &mode, 4);
  usleep(100000);
  while(1) {
    if(duty_cycle <= 0){ duty_cycle = 1; }
    if(duty_cycle >= 1000){ duty_cycle = 999; }
    prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 1, &duty_cycle, 4);
    prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 2, &delay_val, 4);
//    prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 2, &newFreq, 4);
    usleep(10000);
  }
  return 0;
}


int main(void){

  int newDutyCycle;
  int newFreq;
  char input[10];
  printf("Duty cycle, 1 to 999 [-1=cancel]: ");
  fgets(input, sizeof(input), stdin);
  sscanf(input, "%d", &newDutyCycle);
  printf("Frequency: ");
  fgets(input, sizeof(input), stdin);
  sscanf(input, "%d", &newFreq);

  pthread_t pruThread;
  pthread_create(&pruThread, NULL, pruFunc, NULL);

  while(newDutyCycle != -1){

    duty_cycle = newDutyCycle;
    delay_val = (int)(1.0/((5e-9)*2.0*1000.0*(double)newFreq) - 1);
    printf("Delay Period: %d\n\n", delay_val);

    printf("Duty cycle, 1 to 999 [-1=cancel]: ");
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%d", &newDutyCycle);
    printf("Frequency: ");
    fgets(input, sizeof(input), stdin);
    sscanf(input, "%d", &newFreq);
  }

  pthread_cancel(pruThread);

  exit(0);
}
