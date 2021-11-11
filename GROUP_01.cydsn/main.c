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
    
    //array dello slave inizializzato a 0
    uint8_t slaveBuffer[SLAVE_BUFFER_SIZE]={0};
    
    // Setting del WHO AM I all'indirizzo 0xBC
    slaveBuffer[2]=0xBC; //who_I_am 
    
    //Setting di tutti i registri dello slave in R/W mode così da poterci scrivere i dati inviati dal bridge control panel
    EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_SIZE-1, slaveBuffer);
    
    
    // Avvio dei seguenti componenti: Mux, ADC, Timer, Isr
    AMux_Start();
    ADC_DelSig_Start();
    Timer_Start();
    isr_ADC_StartEx(Custom_ISR_ADC); //usiamo startex perchè abbiamo scritto il suo codice in interruptRoutines
    
    // inizializzazione delle variabili usate in "interruptRoutines.c" e "main.c"
    counter=0;
    sum_temp=0;
    sum_ldr=0;
    conv_shift=0;
    onetime =1; //variabile flag, è 1 fin quando conv_shift è 0 e quindi non è stata ancora iniziata la conversione
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        /* Place your application code here. */
        
        //se onetime è 1 conv_shift è 0 e il valore del numero di campioni viene letto dal
        //control register 0 dello slave e salvato in conv_shift
        if(onetime==1){
            //maschera per estrarre i bit 2 3 4 e 5 del control register 0 che contengono 
            //informazioni sul numero di campioni che vogliamo usare per la media
            SAMPLE_SIZE = (slaveBuffer[0] & 0b00111100);
            //i bit ottenuti con la maschera vengono spostati verso destra di 2 in modo da
            //poter leggere correttamente il numero di campioni
            SAMPLE_SIZE_shift = SAMPLE_SIZE>>2;
            //il numero di campioni viene salvato nella variabile conv_shift
            conv_shift = SAMPLE_SIZE_shift;
        }
        
        //se conv_shift è diverso da 0, quindi stiamo campionando e facendo la media con un certo
        //numero di campioni leggiamo il valore dei bit 2 3 4 e 5 del control register 0 dello slave
        //e lo salviamo in SAMPLE_SIZE_shift
        if(conv_shift != 0)
        {
            onetime = 0;
            SAMPLE_SIZE = (slaveBuffer[0] & 0b00111100);
            SAMPLE_SIZE_shift = SAMPLE_SIZE>>2; 
        }
        
        //se il nuovo valore di SAMPLE_SIZE_shift è diverso da conv_shift, conv_shift viene aggiornato
        //al nuovo valore
        if(conv_shift !=SAMPLE_SIZE_shift)
        {
            conv_shift = SAMPLE_SIZE_shift;
        }
        
        //se il valore del periodo del timer salvato nel control register 1 dello slave è
        //diverso da quello attuale viene aggiornato
        if ((Timer_ReadPeriod() != slaveBuffer[1]) && (slaveBuffer[1] !=0)){
            Timer_WritePeriod(slaveBuffer[1]);
        }
        
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
