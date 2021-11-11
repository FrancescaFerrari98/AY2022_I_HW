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
    
    #define SLAVE_BUFFER_SIZE 7 //dimensione array dello slave 
    
    CY_ISR_PROTO (Custom_ISR_ADC);
    
    // Dichiarazione delle variabili usate in main.c ed interruptRoutines.c
    uint8_t slaveBuffer[SLAVE_BUFFER_SIZE]; //array dello slave
    
    //variabili utilizzate per salvare il numero di campioni da usare per fare la media
    //presenti nei bit 2 3 4 e 5 del control register 0 dello slave
    uint8_t SAMPLE_SIZE;
    uint8_t SAMPLE_SIZE_shift;
    
    
    int counter; //contatore per determinare il canale da campionare
    
    int16 sum_temp; //somma dei valori di temperatura
    
    int16 sum_ldr; //somma dei valori campionati dall'LDR
    
    int conv_shift; //variabile in cui viene salvato il numero di campioni 
    
    int onetime; //variabile flag
    
#endif 
/* [] END OF FILE */
