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

#include "photoCellAPI.h"

void  photo_cell_init() {  
    CONFIG_RB0_AS_ANALOG();
    CONFIG_RB1_AS_ANALOG();
    CONFIG_RB2_AS_ANALOG();
    CONFIG_RB3_AS_ANALOG();

    adc_init(0);
}


int16_t read_photo_transistor(photoTrans color) {
    switch (color) {
        case YELLOW_TRANS:
            return adc_read(1);
            break;
        case BLUE_TRANS:
            return adc_read(0);
            break;
        case RED_TRANS:
            return adc_read(3);
            break;
        case GREEN_TRANS:
            return adc_read(2);
            break;
        default: 
            return 0;
            break;
    }
}

void photo_trans_print() {
    while (1)  {
        printf("\nYellow %i", read_photo_transistor(YELLOW_TRANS));
        printf("    Blue %i", read_photo_transistor(BLUE_TRANS));
        printf("    Red %i", read_photo_transistor(RED_TRANS));
        printf("    Green %i", read_photo_transistor(GREEN_TRANS));
        doHeartbeat();
    } 
}