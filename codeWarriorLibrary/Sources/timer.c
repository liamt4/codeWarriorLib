#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */
#include "sw_led.h"   
#include "pll.h"
#include "sci.h"
#include "pit.h"

unsigned int timertick;
int thiscounter = 0;

void PIT_InitChannel(PIT_Channel ch, PIT_MicroTimer mt, PIT_Interrupt ie) 
{
    switch(mt) 
    {
      case PIT_MT0:
        PITMUX &= ~ch;
        break;
      case PIT_MT1:
        PITMUX |= ch;
        break;
    }
    
    switch(ch) //disables the interuupts for the selected channel 
    {
      case PIT_CH0:
        PITCE |= PITCE_PCE0_MASK;   
        break;
      case PIT_CH1:
        PITCE |= PITCE_PCE1_MASK;
        break;
      case PIT_CH2:
        PITCE |= PITCE_PCE2_MASK;
        break;
      case PIT_CH3:
        PITCE |= PITCE_PCE3_MASK;
        break;
    }  
    
}

//the final touch without this one nothing works, so be sure to put this in the main
void PIT_Start() 
{
  PITCFLMT |= PITCFLMT_PITE_MASK | PITCFLMT_PITFRZ_MASK;
}


//non blocking flag with a certain time defined from the user
int PIT_SetXmsEvent(PIT_Channel ch, PIT_MicroTimer mt, unsigned int  ms) 
{
  
  if(ms > 800){ms=500;}
  
  timertick = 80*ms; 
  
  switch(mt) 
  {
    case PIT_MT0:
      PITMTLD0 = 250-1;
      break;
    case PIT_MT1:
      PITMTLD1 = 250-1;
      break;
  }
  
  switch(ch) 
  {
    case PIT_CH0:
      PITLD0 = timertick-1;
      break;
    case PIT_CH1:
      PITLD1 = timertick-1;
      break;
    case PIT_CH2:
      PITLD2 = timertick-1;
      break;
    case PIT_CH3:
      PITLD3 = timertick-1;
      break;
  }
  
   
}


//sets the channel to the microtimer 1 and the calculations are for 1ms
void PIT_Set1msDelay(PIT_Channel ch) 
{
  PITMUX |= ch;     //setting 
  PITMTLD1 = 250-1; //set the microtimer 1 to 250 tiks
  
  switch(ch) 
  {
    case PIT_CH0:
      PITLD0 = 80-1;
      break;
    case PIT_CH1:
      PITLD1 = 80-1;
      break;
    case PIT_CH2:
      PITLD2 = 80-1;
      break;
    case PIT_CH3:
      PITLD3 = 80-1;
      break;
  }
  
  PITCE |= ch;  //enables the cannel
  
  PITFLT = ch; //force loads the channel
  
  PITTF = ch;   //makes the flag equal to the channel
  
}


void PIT_Sleep(PIT_Channel ch, unsigned int ms) 
{
   PIT_Set1msDelay(ch);

   while(thiscounter<ms) 
   {
      if(PITTF & ch) 
      {
        thiscounter++;
        PITTF = ch;
      }
   }
   thiscounter = 0;
}