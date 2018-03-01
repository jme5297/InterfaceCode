/*
*   Program to generate a simple PWM signal using the BBB PRUSS
*   Output is P9_27, using PRU Enhanced GPIOs
*   by amc6630 onn 3/1/2018

*   Before running, please run this command: config-pin P9.27 pruout

*   For compiling both files:
    gcc pwm_test.c -o pwm_test -lpthread -lprussdrv
    pasm -b pwm_test.p
*/

#include <stdio.h>
#include <stdlib.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#define PRU_NUM   0

int main (void)
{
   // Initialize structure used by prussdrv_pruintc_intc
   tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;

   // duty cycle variable
   unsigned int init_duty_cycle = 100;
   printf("PRU program will be executed...\n");

   int on = 1;
   while (on == 1) {
     printf("Iterating...");
     // Decremental loop
     while(init_duty_cycle > 1){
       prussdrv_init ();
       prussdrv_open (PRU_EVTOUT_0);

       // Map PRU intrrupts
       prussdrv_pruintc_init(&pruss_intc_initdata);

       init_duty_cycle = init_duty_cycle - 1;
       // write duty cycle on PRU memory
       prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0, &init_duty_cycle, 4);

       unsigned int sampletimestep = 10;  //delay factor (10 default, 624 for 1600 Hz)
       // write it into the next word location in memory (i.e. 4-bytes later)
       prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 1, &sampletimestep, 4);

       // Load and execute binary on PRU
       prussdrv_exec_program (PRU_NUM, "./pwm_test.bin");

       // Wait for event completion from PRU
       int n = prussdrv_pru_wait_event (PRU_EVTOUT_0);
       //printf("PRU program completed, event number %d.\n", n);
   }

     // Incremental loop
     while(init_duty_cycle < 100){
       prussdrv_init ();
       prussdrv_open (PRU_EVTOUT_0);

       // Map PRU intrrupts
       prussdrv_pruintc_init(&pruss_intc_initdata);

       init_duty_cycle = init_duty_cycle + 1;
       // write duty cycle on PRU memory
       prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0, &init_duty_cycle, 4);

       unsigned int sampletimestep = 10;  //delay factor (10 default, 624 for 1600 Hz)
       // write it into the next word location in memory (i.e. 4-bytes later)
       prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 1, &sampletimestep, 4);

       // Load and execute binary on PRU
       prussdrv_exec_program (PRU_NUM, "./pwm_test.bin");

       // Wait for event completion from PRU
       int n = prussdrv_pru_wait_event (PRU_EVTOUT_0);
       //printf("PRU program completed, event number %d.\n", n);
     }
   }

   // Disable PRU and close memory mappings
   prussdrv_pru_disable(PRU_NUM);
   prussdrv_exit ();
   return EXIT_SUCCESS;

}
