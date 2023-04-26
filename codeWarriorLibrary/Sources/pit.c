#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "pit.h"

int PIT_Init(PIT_Channel chan, PIT_Interrupt intState, 
            unsigned long ulBusRate, unsigned long ulInterval_us){
    unsigned long prescale = 1;
    unsigned long tickCount = 0;
    
    PITMUX &= ~chan;
    
    tickCount = (ulBusRate * ulInterval_us);
    
    if(tickCount > 65536ul){
     prescale = (tickCount / 65536ul) + 1;
     tickCount /= prescale; 
    }
    
    switch(chan){
      case PIT_CH0:
          PITMTLD0 = prescale -1;
          PITLD0 = tickCount -1;
          break;
      case PIT_CH1:
          PITMTLD0 = prescale -1;
          PITLD1 = tickCount -1;
          break;
      case PIT_CH2:
          PITMTLD0 = prescale -1;
          PITLD2 = tickCount -1;
          break;
      case PIT_CH3:
          PITMTLD0 = prescale -1;
          PITLD3 = tickCount -1;
          break;
    }
    
    //Now set up interrupts
    if(intState)
        PITINTE |= chan;
    else
        PITINTE &= ~chan;
    
    //Enable the channel
    PITCE |= chan;
    
    //Finally, Enable the PIT module
    PITCFLMT |= PITCFLMT_PITE_MASK | PITCFLMT_PITFRZ_MASK;
    
    //Force load it (in case the timer is already running)
    PITFLT = chan;
    
    //Clear any current flag
    PITTF =  chan;
    
    return 0;
  }
  
  //Blocking
  int PIT_Sleep(PIT_Channel chan, unsigned long ulBusRate, unsigned int ms){
      //Set up the timer to go off when I am done waiting
      PIT_Init(chan, 0, ulBusRate, ms*1000);
      while(!(PITTF & chan)); //Wait until the flag appears
      PITTF = chan;//"clear" flag by writing a 1 
  }