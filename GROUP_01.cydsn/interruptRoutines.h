/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#ifndef __INTERRUPT_ROUTINES_H
    #define __INTERRUPT_ROUTINES_H
    
    #include "cytypes.h"
    #include "stdio.h"
    
    #define SAMPLE_SIZE 5 //numero di campioni di ogni sensore utilizzati per calcolare la media
    #define SLAVE_BUFFER_SIZE 7 //dimensione array dello slave 
    
    CY_ISR_PROTO (Custom_ISR_ADC);
    
    uint8_t slaveBuffer[SLAVE_BUFFER_SIZE]; //array dello slave
    
    // Dichiarazione delle variabili usate in main.c ed interruptRoutines.c
    uint32 counter;
    int16 sum_temp;
    int16 sum_ldr;
    #endif 
/* [] END OF FILE */
