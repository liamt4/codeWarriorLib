 #include "swl_led.h"
 #include "derivative.h"
 
 
 
 void SWL_Init (void){
    //Set initial state for PT1AD1 (Switches and LED GPIO)
    PT1AD1 &= 0b00011111;
    //Set data direction (1 for out, 0 for input)
    DDR1AD1 = 0b11100000;
    //Enable buttons as inputs (0 for out, 1 for input)
    ATD1DIEN1 |= 0b00011111;

 }
 
 void SWL_ON (SWL_LEDColour led){
      PT1AD1 |= led;
 }
 
void SWL_OFF (SWL_LEDColour led){
      PT1AD1 &= ~led;
}

void SWL_TOG (SWL_LEDColour led){
      PT1AD1 ^= led;
}
int SWL_CHECKON (SWL_LEDColour led){
      return (PT1AD1 & led);
}

// is a specific switch being pushed (T/F (1/0))
int SWL_Pushed (SWL_SwitchPos pos){
      
     return (PT1AD1 & pos);
}

// is any switch being pushed (T/F (1/0))
int SWL_Any (void){
    if(PT1AD1 & SWL_ANY)
      return 1;
     else return 0;
}

//Set clock speed to 20MHz
void PLL_To20MHz (void)
{
 // PLLCLK = 2 x OSCCLK x ([SYNR + 1] / [REFDV + 1])
 // PLLCLK = 2 x 16Mhz x ([4 + 1] / [3 + 1])
 // 5/4 (1.25) * 16Mhz * 2 = 40MHz
 // bus is PLLCLOCK / 2, 40MHz / 2 = 20MHz 2.3.2.1 + 2.3.2.2
 SYNR = 4;
 REFDV = 3;
 // we could go faster, but we want to remain stable!
 CLKSEL_PSTP = 1; // 2.3.2.6 (pseudo stop, oscillator runs in stop)

 PLLCTL = 0b11111111; // 2.3.2.7
 // monitor enable (clock is self-monitored)
 // PLL turned on
 // automatic acquisition/tracking mode
 // fast wakeup from full stop

 // can't switch to PLLCLK if (lock=0 and auto=1) - must wait for lock
 while (!CRGFLG_LOCK)
 ; // could, but shouldn't block for long

 // now that we are locked, use PLLCLK/2 for bus (20MHz)
 CLKSEL_PLLSEL = 1; // 2.3.2.6
}
