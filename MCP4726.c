/*
 * File:   22LC64.c
 * Author: kater
 *
 * Created on 11. ?íjna 2019, 17:52
 */

#include "PICF18LF46K22_ConfigSFR.h"
#include "MCP4726_DA.h"


#define AD_GODONE   ADCON0bits.GO_DONE 
#define AD_ENABLE   ADCON0bits.ADON

#define SAMPLE_N 1024

unsigned int getSample();
unsigned int calcSample(unsigned int sample);
void setProgress();
void Init_AD();

unsigned int getSample(){
    AD_GODONE=1;
    NOP();
    while(AD_GODONE);
    NOP();
    return (ADRESH<<8 | ADRESL);
}

unsigned int calcSample(unsigned int sample){
    unsigned int result=0x0000;
    float devider10b=(float)sample/1024;
    float result12b=4096*devider10b;
    
    result=(unsigned int)result12b;
    return result;
}

void setProgress(){
    NOP();
    LATD<<=1;
    delay_NOP(8);
    LATD|=1;
    NOP();
}

void Init_AD(){
    INTCONbits.GIE=1;
    INTCONbits.PEIE=1;

    //Init AD IN
    ANSELAbits.ANSA3=1;
    TRISAbits.RA3=1;
    
    
    ADCON0=0b00001100; //AN3 (RA3) chanel, Disable AD
    ADCON1=0b00000000; //Vref + to Vdd, Vref - to Vss
    ADCON2=0b10101101; //ADFM=1 , ACQT 12 TAD , ADCS Fosc/16 (TAD=1us)

}

void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    //LEDs on port for show result data
    ANSELD=0;
    TRISD=0;
    
    Init_MCP4726();
    Init_AD();
    delay_ms(10);
}

void TestDevice(){
    return;
}

void ClearDevice(){
    ADRESH=ADRESL=0x00;
    PORTD=0x00;
}


void main(void) {
    InitDevice();
    TestDevice();
    ClearDevice();
    
    AD_ENABLE=1;
    
    char devider=SAMPLE_N/8;
    unsigned int DATA_VOLTAGE=0x03FF;
    unsigned int SAMPLE=0;
    /*Set_VoltageAsData_MCP4726(DATA_VOLTAGE);
    delay_ms(1000);*/
    
    for(unsigned int i=SAMPLE_N;i>0;i--){
        
        if( i % devider == 0)setProgress();
        SAMPLE=getSample();
        delay_ms(1);
        Set_VoltageAsData_MCP4726(calcSample(SAMPLE));
        NOP();
        delay_ms(10);
    }
    PORTD=0xFF;
    AD_ENABLE=0;
    
    while(1){
        asm("NOP");
    }
}

/*void interrupt IRS(void){
}*/
