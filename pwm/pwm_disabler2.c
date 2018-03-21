// Program to disable execution of PRUSS code (.p)
// Written by amc6630 on 3/11/2018

#include <stdio.h>
#include <stdlib.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#include <unistd.h>

#define PRU_NUM   1

int main (void)
{
  tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
  prussdrv_init ();
  prussdrv_open (PRU_EVTOUT_0);
  prussdrv_pruintc_init(&pruss_intc_initdata);
  prussdrv_exec_program (PRU_NUM, "./pru2.bin");
  unsigned int mode = 0;
  prussdrv_pru_write_memory(PRUSS0_PRU1_DATARAM, 3, &mode, 4);
 }
