// Program to initalize execution of PRUSS code (.p)
// Written by amc6630 on 3/11/2018

#include <stdio.h>
#include <stdlib.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>

#define PRU_NUM   0

int main (void)
{
   tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
   printf("Hello...\n");
   prussdrv_init ();
   printf("Trying to open...\n");
   prussdrv_open (PRU_EVTOUT_0);
   printf("Initializing...\n");
   prussdrv_pruintc_init(&pruss_intc_initdata);
   printf("Executing...\n");
   prussdrv_exec_program (PRU_NUM, "./pwm_final.bin");
   printf("Executed!\n");
}
