#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */


#ifndef PITH
#define PITH

typedef enum PIT_ChannelTypedef_ {
 PIT_CH0 = 0b00000001,
 PIT_CH1 = 0b00000010,
 PIT_CH2 = 0b00000100,
 PIT_CH3 = 0b00001000 
}PIT_Channel;

typedef enum PIT_InterruptTypede_ {
 PIT_Int_Disabled = 0,
 PIT_Int_Enabled = 1 
}PIT_Interrupt;

int PIT_Init(PIT_Channel chan, PIT_Interrupt intState, unsigned long ulBusRate, unsigned long ulInterval_us);

int PIT_Sleep(PIT_Channel chan, unsigned long ulBusRate, unsigned int ms); 
#endif