 #include "sw_led.h"
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
      PT1AD1 &= led;
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
     return 0;
}