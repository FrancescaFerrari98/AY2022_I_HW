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
#include "project.h"
#include "stdio.h"
#include "interruptRoutines.h"
#define LED_ON 1
#define LED_OFF 0

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    // Avvio del componente slave "EZI2C"
    EZI2C_Start();
    
    // inserimento del periodo del timer nello slave buffer all'indirizzo 0x=1 
    slaveBuffer[1]=Timer_ReadPeriod();
    // Setting del WHO AM I all'indirizzo 0xBC
    slaveBuffer[2]=0xBC; //who_I_am 
    
    // Setting dei bit (Average samples bits, Status bits) del CONTROL REGISTER 0
    slaveBuffer[0] = 0b10111;
    
    // Avvio dei seguenti componenti: Mux, ADC, Timer, Isr
    AMux_Start();
    ADC_DelSig_Start();
    Timer_Start();
    isr_ADC_StartEx(Custom_ISR_ADC); //usiamo startex perchè abbiamo scritto il suo codice in interruptRoutines
    
    // inizializzazione delle variabili usate in "interruptRoutines.c"
    counter=0;
    sum_temp=0;
    sum_ldr=0;

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
         // ciclo di controllo per accensione del led integrato (pin 2[1])
        // Se i bit 0 e 1 del CONTROL REGISTER 0 (i.e Status bit 0 e Status bit 1) sono settati a 1 il led deve risultare acceso
        if ((slaveBuffer[0] & 00000011)==1)
        {
            Pin_LED_Write(LED_ON);
        }
        else 
        {
            Pin_LED_Write(LED_OFF);
        }
    }
}

/* [] END OF FILE */
