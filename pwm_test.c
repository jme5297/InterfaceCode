/** Program to generate a simple PWM signal using the BBB PRUSS
*   Output is P9_27 and input is P9_28, using PRU EGPIOs
*   by Derek Molloy, for the book Exploring BeagleBone
*/
#include <stdio.h>
#include <stdlib.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <unistd.h>


#define PRU_NUM   0

int main (void)
{
   /*if(getuid()!=0){ // make sure this is ran as root
      printf("You must run this program as root. Exiting.\n");
      exit(EXIT_FAILURE);
   }*/
   // Initialize structure used by prussdrv_pruintc_intc
   tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
   // define variables for user input
   char user_input;
   int percent;
   // loop over user Input
   int counter = 0;
   while(counter < 10) {
     counter = counter + 1;
     // Allocate and initialize memory
     prussdrv_init ();
     prussdrv_open (PRU_EVTOUT_0);
     // Map PRU intrrupts
     prussdrv_pruintc_init(&pruss_intc_initdata);
     printf("Press (F) for forward, (R) for reverse, or (S) to stop: ");
     scanf("%c", &user_input);
     if(&user_input == "F") {
       unsigned int percent = 100;
     }
     else if(&user_input == "R") {
       unsigned int percent = 50;
     }
     else if(&user_input == "S") {
       unsigned int percent = 0;
     }
     prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 0, &percent, 4);
     unsigned int sampletimestep = 10;  //delay factor (624 for 1600 Hz)
     // write it into the next word location in memory (i.e. 4-bytes later)
     prussdrv_pru_write_memory(PRUSS0_PRU0_DATARAM, 1, &sampletimestep, 4);
     // Load and execute binary on PRU
     prussdrv_exec_program (PRU_NUM, "./pwm_test.bin");
     printf("PRU program executed...\n");
     //usleep(5e6);
     //printf("End of usleep...\n");
     // Disable PRU and close memory mappings
     prussdrv_pru_disable(PRU_NUM);
     prussdrv_exit ();
     printf("PRU disabled...\n");
     return EXIT_SUCCESS;
   }
}
