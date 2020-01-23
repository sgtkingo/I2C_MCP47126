/*
 * File:   22LC64.c
 * Author: kater
 *
 * Created on 11. ?íjna 2019, 17:52
 */

#include "PICF18LF46K22_ConfigSFR.h"
#include "MCP4726_DA.h"


void InitDevice(){
    OSCCON=0b01111100; //osc setting, 16 MHz, internal by FOSCH
    //LEDs on port for show result data
    ANSELD=0;
    TRISD=0;
    
    Init_MCP4726();
}

void TestDevice(){
    //01111011
    //char cmd=CreateCommand_MCP4726(CMD_ALL,VR_VREF_BF,PD_1k,Gx2);
    //Write_ALL_MCP4726(cmd, 0x1FAA);
    //Set_VoltageAsData_MCP4726(0x11AA);
    //Write_DAC12b_MCP4726(PD_1k,0x11FF);
    //Write_DAC8b_MCP4726(PD_1k, 0x55);
    return;
}

void ClearDevice(){
    PORTD=0x00;
}


void main(void) {
    InitDevice();
    TestDevice();
    ClearDevice();
    
    unsigned int DATA_VOLTAGE=DA_MAX;
    
    for(unsigned int i=DATA_VOLTAGE;i>0;i--){
        Set_VoltageAsData_MCP4726(i);
        NOP();
        delay_ms(10);
    }
    /*ReadFrom_MCP4726();
    delay_ms(1);
    
    for(int i=0;i<EEPROM_MEM_SIZE_4726;i++){
        PORTD=EEPROM_MEM_4726[i];
        __delay_ms(250);
    }*/
    PORTD=0xFF;
    
    while(1){
        asm("NOP");
    }
}

/*void interrupt IRS(void){
}*/
