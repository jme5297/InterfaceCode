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
int duty_cycle = 5;

void *pruFunc(void *vargp) {
  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
  prussdrv_init ();
  prussdrv_open (PRU_EVTOUT_0);
  prussdrv_pruintc_init(&pruss_intc_initdata);
  prussdrv_exec_program (PRU_NUM, "./pwm_final.bin");
  int delay_period = 624;
  int ping_val = 1;
  int mode = 1;
  prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0, &ping_val, 4);
  prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 1, &duty_cycle, 4);
  prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 2, &delay_period, 4);
  prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 3, &mode, 4);
  usleep(100000);
  while(1) {
    if(duty_cycle <= 0){ duty_cycle = 1; }
    if(duty_cycle >= 100){ duty_cycle = 99; }
    prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 1, &duty_cycle, 4);
    usleep(10000);
  }
  return 0;
}


int main(void){

  printf("Hello!\n");
  
  pthread_t pruThread;
  printf("Creating thread...\n");
  pthread_create(&pruThread, NULL, pruFunc, NULL);
  printf("Joining thread...\n");
  //pthread_join(pruThread, NULL);
  
  printf("Thread created!\n");

  int newDutyCycle;
  char input[10];
  printf("Enter a new duty cycle [-1=cancel]: ");
  fgets(input, sizeof(input), stdin);
  sscanf(input, "%d", &newDutyCycle);
  //newDutyCycle = (int)input;
  while(newDutyCycle != -1){
    
    duty_cycle = newDutyCycle;
    printf("Enter [-1=cancel]: ");
    
    fgets(input, sizeof(input), stdin);
    //newDutyCycle = (int)input;
    sscanf(input, "%d", &newDutyCycle);
  }

  pthread_cancel(pruThread);

  exit(0);

}

