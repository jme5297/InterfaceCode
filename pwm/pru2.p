// PRUSS program to output a simple PWM signal at fixed sample rate of 100
// Written by amc6630 on 3/11/2018
// Output is r30.8: P8.27

// ============ REGISTER MAPPINGS ============
// r0 - general register used for loading other registers or as a counter
// r1 - DUTY_CYCLE HIGH (0 to 1000)
// r2 - DELAY (best if set for f = 1600 Hz)
// r3 - DUTY_CYCLE LOW (1000 minus HIGH)
// r4 - counter variable used in the PWM high and low loops
// r5 - MODE (0 for IDLE, 1 for ACTIVE)
// r6 - ENDLOOP (if == 0, ends PWM loop and checks for ping from C code)
// r7 - PING_VAL_OLD (integer that varies from 1 to 7)
// r8 - PING_VAL_NEW (integer that varies from 1 to 7)
// r9 - CHECKLOOP counter variable (used if PING_VAL_OLD == PING_VAL_NEW)
// ===========================================


// ========= MANDATORY INPUT ORDER FROM C CODE =========
// 1. PING_VAL (has to be sent at f = 10 Hz from C-code)
// 2. DUTY_CYCLE
// 3. DELAY
// 4. MODE
// =====================================================


.origin 0                // offset of start of program in PRU memory
.entrypoint SETUP        // program entry point used by the debugger

SETUP:
  MOV r5, 0              // Set MODE to IDLE (0)
  CLR r30.t8             // Set output to low when in IDLE mode

IDLE:
  MOV r0, 0x0000000C     // Load the memory location of MODE
  LBBO r5, r0, 0, 4      // Store value of MODE on r5
  QBEQ IDLE, r5, 0       // If r5 == 0, go to IDLE and start over

ACTIVE:                  // executed if r5 != 0
  MOV	r0, 0x00000000	   // load the memory location of PING_VAL
  LBBO	r7, r0, 0, 4	   // load PING_VAL as PING_VAL_OLD into r7
  MOV r6, 1000           // Set ENDLOOP to 1000 iterations

START:                   // Sets up PWM loop if MODE != 0 (not in IDLE mode)
  SUB r6, r6, 1          // decrement ENDLOOP
  MOV	r0, 0x00000004	   // load the memory location of DUTY_CYCLE
  LBBO	r1, r0, 0, 4	   // load DUTY_CYCLE into r1
  MOV	r0, 0x00000008	   // load the memory location of DELAY
  LBBO	r2, r0, 0, 4	   // load DELAY into r2
  MOV	r3, 1000		       // load 1000 into r3
  SUB	r3, r3, r1	       // subtract r1 (high) away from 1000

MAINLOOP:
  // ------ HIGH cycle ------
  MOV r4, r1                  // start counter at number of steps HIGH
  SET r30.t8                  // set output to high
SIGNAL_HIGH:
  MOV r0, r2                  // load r2 from above
DELAY_HIGH:
  SUB r0, r0, 1               // decrement delay loop counter
  QBNE	DELAY_HIGH, r0, 0     // repeat until step delay is done
	SUB	r4, r4, 1               // the signal was high for a step
	QBNE	SIGNAL_HIGH, r4, 0    // repeat until signal high steps are done

  // ------ LOW cycle ------
	MOV	r4, r3		              // number of steps low loaded
	CLR	r30.t8	                // set the output P9_27 low
SIGNAL_LOW:
	MOV	r0, r2		              // the delay step length - load r2 above
DELAY_LOW:
	SUB	r0, r0, 1               // decrement loop counter
	QBNE	DELAY_LOW, r0, 0      // repeat until step delay is done
	SUB	r4, r4, 1               // the signal was low for a step
	QBNE	SIGNAL_LOW, r4, 0     // repeat until signal low % is done
  // ====== end of 1 HIGH-LOW cycle ======
  QBNE START, r6, 0          // complete ENDLOOP number of full HIGH-LOW cycles

  // ====== end of ENDLOOP number of cycles ======
  MOV	r0, 0x00000000	        // load the memory location of PING_VAL
  LBBO	r8, r0, 0, 4	        // load PING_VAL as PING_VAL_NEW into r8
  QBNE ACTIVE, r7, r8         // go to ACTIVE if PING_VAL_NEW != PING_VAL_OLD
  MOV r9, 500                 // set CHECKLOOP to 500 iterations

CHECKLOOP:                    // mini-loop to verify end of C code execution
  SUB r9, r9, 1               // decrement CHECKLOOP variable
  QBNE CHECKLOOP, r9, 0       // loop over CHECKLOOP
  MOV	r0, 0x00000000	        // load the memory location of PING_VAL
  LBBO	r8, r0, 0, 4	        // load PING_VAL as PING_VAL_NEW into r8
  QBNE ACTIVE, r7, r8         // go to ACTIVE if PING_VAL_NEW != PING_VAL_OLD
  QBEQ END, r7, r8            // go to END if PING_VAL_NEW == PING_VAL_OLD

END:
  QBA SETUP                   // go to SETUP if code reaches this point
