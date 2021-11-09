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
#include "interruptRoutines.h"
#include "project.h"

int32 value_digit;
int16 sum_temp;
int16 sum_ldr;
int16 avg_temp;
int16 avg_ldr;
int16 temp_degrees;

CY_ISR (Custom_ISR_ADC){
    
    // Funzione necessaria per abbassare la linea dell'interrupt
    Timer_ReadStatusRegister();
    
    /* Ciclo per campionare alternativamente il sensore di temperatura ed il Light Dependent Resistor:
       Si è utilizzato un contatore cosicchè quando lo stesso assume valori pari si campiona il sensore di temperatura
       contrariamente quando il contatore assume valori dispari si campiona l'LDR. Counter è stato inizializzato a 0 nel main.
       questo ciclo prosegue fino a quando il contatore non supera 2*SAMPLE_SIZE*/
    if(counter%2==0 && counter<2*SAMPLE_SIZE){
        AMux_Select(counter%2);
        ADC_DelSig_StartConvert();
        value_digit = ADC_DelSig_Read32(); 
        if(value_digit <0) value_digit=0;
        if(value_digit >65535) value_digit=65535;
        // Funzione per fare la somma del valore dei 5 campioni di temperatura
        sum_temp= sum_temp + ADC_DelSig_CountsTo_mVolts(value_digit);
        counter++;
        // Si è deciso di spegnere l'adc durante lo switch da un canale all'altro del mux così da evitare valori erronei dati dall'ADC
        ADC_DelSig_StopConvert();
        
    }else if(counter%2==1 && counter<2*SAMPLE_SIZE){
        AMux_Select(counter%2);
        ADC_DelSig_StartConvert();
        value_digit = ADC_DelSig_Read32(); 
        if(value_digit <0) value_digit=0;
        if(value_digit >65535) value_digit=65535;
        // funzione per fare la somma del valore dei 5 campioni del LDR
        sum_ldr= sum_ldr + ADC_DelSig_CountsTo_mVolts(value_digit);
        counter++;
        ADC_DelSig_StopConvert();
        
    }else if(counter>=2*SAMPLE_SIZE){
        // quando il contatore supera 2*SAMPLE_SIZE sono stati ottenuti 5 campioni per entrambi i sensori.
        // Si calcolano quindi le medie (sui 5 campioni ottenuti) per entrambi i sensori.
        avg_temp=sum_temp/SAMPLE_SIZE;
        avg_ldr=sum_ldr/SAMPLE_SIZE;
        
        /* Funzione per stabilire quali dei registri presenti nel Buffer dello slave possono essere messi come R/W o solo come R
           In questo caso, all'inizio, era necessario settare tutti registri dello Slave Buffer come R/W*/ 
        EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, SLAVE_BUFFER_SIZE-1 ,slaveBuffer);
        
        /* Divisione del dato calcolato precedentemente (16bit) in due variabili da 8 bit in modo tale da salvarli nello slave buffer
           nelle posizioni 3 e 4 si sono salavati rispettivamente l'MSB e l'LSB del dato di temperatura*/ 
        slaveBuffer[3]=avg_temp>>8;
        slaveBuffer[4]=avg_temp & 0xFF;
        
        // in ugual modo si è fatto con l'LDR riempiendo le posizioni 5 e 6 dello slave buffer
        slaveBuffer[5]=avg_ldr>>8;
        slaveBuffer[6]=avg_ldr & 0xFF;
        
        // Una volta scritti i valori di interesse all'interno dello slave buffer si decide di lasciare come possibili R/W solo i due CONTROL REGISTER (per questo compare 2 nella funzione)
        EZI2C_SetBuffer1(SLAVE_BUFFER_SIZE, 2 ,slaveBuffer);
        
        
        sum_temp=0;
        sum_ldr=0;
        counter=0;
    }
 }  
/* [] END OF FILE */
