                         uC/OS-II, The Real-Time Kernel
                                
                           Port for the TI MSP430

                                Version 1.00

                                READ ME FILE


******************************************************************************
                                 C COMPILER
******************************************************************************

This port assumes the:

    IAR Embedded Workbench for TI MSP430 v1.26A


******************************************************************************
                              ACKNOWLEDGEMENTS
******************************************************************************

    This port was originally done by Mr. Jian Chen (yenger@hotmail.com).


******************************************************************************
                                  CONTACTS
******************************************************************************

    If you find a problem (i.e. bug) in uC/OS-II or this port, do not hesitate  
to report the problem to either:

1) Through the INTERNET:

   My INTERNET address is:   Jean.Labrosse@Micrium.com


2) By writing to me at:

   Micrium, Inc.
   949 Crestview Circle
   Weston, FL 33327
   U.S.A.

   c/o: Jean J. Labrosse


3) By calling Micrium, Inc.:

   (954) 217-2036
   (954) 217-2037, FAX

  
   Make sure that the problem you are  reporting is in  uC/OS-II and not your
application.    If you in fact discover a bug in uC/OS-II, check the  uC/OS-II 
web site for a list of known bugs and their fixes:

    www.uCOS-II.com

   If you are  using  an  older  version,  you should  first  get the lastest
version.  


******************************************************************************
                                   NOTES
******************************************************************************

1) For the port, I assumed that you would be using the Watchdog Timer as the
   interrupt source for the TICK ISRs.

2) The port has been tested using the C-Spy simulator for both task level and 
   ISR level context switching using the C-Spy simulator.  The watchdog timer 
   interrupts were set at 50,000 cycles.

3) To save on the stack space for each task, this port makes use of a separate
   interrupt stack.  In other words, when an interrupt occurs, the current task's
   registers are pushed onto its stack and then the stack pointer is changed to 
   use the interrupt stack pointer.  This means that you will not need to account
   for ISR nesting in the calculation of your task stacks.


Jean J. Labrosse
