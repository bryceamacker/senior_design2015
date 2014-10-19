/*********************************************************************
*
* Mississippi State University
*
*********************************************************************
* FileName: photoCellAPI.c
* Dependenies: See INCLUDES setion below
* Proessor: PIC24HJ64GP506A
* Compiler: gcc-xc16
* Company: Mississippi State University/ECE
*
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* MODULE FUNCTION: Functions to help read from photo cells
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author                Date                    Comment
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Steven Calhoun        10/13/2014              SECON 2015
*********************************************************************/

#include "ADCAPI.h"

void adc_init(uint8_t u8_ch0Select, uint16_t u16_ch123SelectMask, uint16_t u16_numTcyMask ) {
    printf("ADC init\n");
    // Turn off ADC while configuring
    AD1CON1bits.ADON = 0;

    // Silmutaneous sampling
    AD1CON1 = ADC_CLK_TMR | ADC_SAMPLE_SIMULTANEOUS;
    AD1CON3 = (u16_numTcyMask & 0x00FF);
    AD1CON2 = ADC_VREF_AVDD_AVSS | ADC_CONVERT_CH0123 | ADC_ALT_BUF_ON;

    AD1CHS0 = ADC_CH0_NEG_SAMPLEA_VREFN | (u8_ch0Select & 0x1F);

    AD1CHS123 = u16_ch123SelectMask;


    // AD1 interrupt
    _AD1IP = 7;
    _AD1IF = 0;
    _AD1IE = 1;

    // Turn on the ADC
    AD1CON1bits.ADON = 1;
}

void _ISR _ADC1Interrupt (void) {
    static uint8_t u8_adcCount = 64;
    uint8_t u8_i;
    uint16_t* au16_adcHWBuff = (uint16_t*) &ADC1BUF0;

    /* If ADC is writing in the 2nd half of the buffer, so determine which
    * half of the buffer is valid (and caused this interrupt), then fetch
    * that half into our local array of ADC results.
    */
    if (AD1CON2 & ADC_ALT_BUF_STATUS_2) {
        for ( u8_i=0; u8_i<8; u8_i++) {
            au16_buffer[u8_i] += au16_adcHWBuff[u8_i];
        } 
    } else {
        for ( u8_i=8; u8_i<16; u8_i++) {
            au16_buffer[u8_i-8] += au16_adcHWBuff[u8_i];  
        }
    }

    _AD1IF = 0;
    SET_SAMP_BIT_ADC1();

    u8_adcCount--;
    if (u8_adcCount == 0) {
        u8_adcCount = 64;
        u8_gotData = 1;
        LED2 = !LED2;

        for ( u8_i=0; u8_i<8; u8_i++) {
            au16_sum[u8_i] = au16_buffer[u8_i];
            au16_buffer[u8_i] = 0;
        }

        printf("\nSum 0: %x\n", au16_sum[0]);
        printf("Sum 1: %x\n", au16_sum[1]);
        printf("Sum 2: %x\n", au16_sum[2]);
        printf("Sum 3: %x\n", au16_sum[3]);
        printf("Sum 4: %x\n", au16_sum[4]);
        printf("Sum 5: %x\n", au16_sum[5]);
        printf("Sum 6: %x\n", au16_sum[6]);
        printf("Sum 7: %x\n", au16_sum[7]);    
    };
}

/*
void _ISR _ADC1Interrupt (void) {
    static uint8_t u8_adcCount = 64;
    uint8_t u8_i;
    uint16_t* au16_adcHWBuff = (uint_t16*) &ADC1BUF0;

    if (AD1CON2bits.BUFS) {
        for (u8_i = 0; u8
    }
}
*/