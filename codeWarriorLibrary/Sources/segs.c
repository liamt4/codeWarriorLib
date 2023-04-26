#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "segs.h"

//Pulse Port A LSB low then high to latch new value
#define Segs_WLATCH PORTA &= (~0x01); PORTA |= 0x01;

//Set the mode bit low
#define Segs_ML PORTA &= (~0x02);

//Set the mode line low
#define Segs_MH PORTA |= 0x02;

//setup ports to speak with 7-segs
void Segs_Init(void) {
  //Standard GPIO setup (no inputs in this case)
  //Set control pins high to keep device from loading
  PORTA |= 0x03l;//0b00000011
  
  //Set direction of two lsb bits on Port A to output
  DDRA |= 0x03;
  
  //Set all bits on Port B to output
  DDRB |= 0xff;
  
  //After initialization, turn all segs off (if we don't do this,
  //previous latched data will stay on the display until we replace it.)
  Segs_Clear();
}

//control segs manually
void Segs_Custom(unsigned char Addr, unsigned char Value) {
  Addr &= 0x07;
  Addr |= 0b01111000;
  PORTB = Addr;
  Segs_MH 
  Segs_WLATCH  
  PORTB = Value;
  Segs_ML
  Segs_WLATCH
 
}

//show HEX decoding(changes all of display to HEX, w/wo dp)
void Segs_Normal(unsigned char Addr, unsigned char Value, Segs_DPOption dp){
 //This will display a particular value, 0 to F in hex, on the specified
 //digit(0 is top left, 7 is bottom right)
 
 //I'm going to use the Addr byte, which should already contain the three-bit
 //address in the 3 LSB bits (where we want it) to construct the command
 Addr &= 0x07;//Clear all bits but the existing 0-7 address
 
 //Add the rest of the command(Bank A, Normal, Decode, Hexadecimal, NoData following.)
 Addr |= 0b01011000;//Three zeros on right will have the value of addr from call
 
 //Do we want the decimal point? Adjust value if so
 if(dp) Value &= (~0x80);//Turn dp bit on by setting a 0(0 means dp ON)
 else Value |= 0x80;//Turn dp bit off by setting a 1 0b10000000
 
 //Send the command to the Driver/Decoder
 PORTB = Addr;
 Segs_MH //This is a command
 Segs_WLATCH//Strobe R/W to load command
 
 //Send the data(number between 0 and F)
 PORTB = Value;
 Segs_ML//This is data
 Segs_WLATCH//Load data  
}

// deprecated after term 1202
// go code B (changes all of display to code b)
void Segs_CodeB (unsigned char Addr, unsigned char Value){
  
}

// show a 16-bit value on the upper or lower display
void Segs_16H (unsigned long Value, Segs_LineOption Line){
     unsigned char Addr;
     unsigned int v1 = (Value & 0xF000)>>12;
     unsigned int v2 = (Value & 0x0F00)>>8;
     unsigned int v3 = (Value & 0x00F0)>>4;
     unsigned int v4 = (Value & 0x000F);
     Addr = Addr + (Line * 4);
     Segs_Normal(Addr, v1, Segs_DP_OFF);
     Segs_Normal(Addr+1, v2, Segs_DP_OFF);
     Segs_Normal(Addr+2, v3, Segs_DP_OFF);
     Segs_Normal(Addr+3, v4, Segs_DP_OFF);
}

// show a decimal value on the first or second line of the 7-segs
void Segs_16D (unsigned int Value, Segs_LineOption Line){
     unsigned char Addr;
     unsigned int v1 = Value/1000;
     unsigned int temp = Value%1000;
     unsigned int v2 = Value/100;
     unsigned int v3 = 0;
     unsigned int v4 = 0;
     temp = temp%100;
     v3 = temp/10;
     v4 = temp%10;
     Addr = Addr + (Line * 4);
     Segs_Normal(Addr, v1, Segs_DP_OFF);
     Segs_Normal(Addr+1, v2, Segs_DP_OFF);
     Segs_Normal(Addr+2, v3, Segs_DP_OFF);
     Segs_Normal(Addr+3, v4, Segs_DP_OFF);
}

// show the 8-bit value starting on the digit as addr (0-6)
void Segs_8H (unsigned char Addr, unsigned char Value){
     int v1 = (Value & 0x00F0)>>4;
     int v2 = (Value & 0x000F);
     Segs_Normal(Addr, v1, Segs_DP_OFF);
     Segs_Normal(Addr+1, v2, Segs_DP_OFF);
}

void Segs_8D(unsigned int Value, Segs_LineOption Line){
     unsigned char Addr;
     unsigned int temp = Value%1000;
     unsigned int v1 = temp/10;
     unsigned int v2 = temp%10;
     temp = temp%100;
     Addr = Addr + (Line * 4);
     Segs_Normal(Addr, v1, Segs_DP_OFF);
     Segs_Normal(Addr+1, v2, Segs_DP_OFF);
}

// clear the display
void Segs_Clear(void){
    unsigned int i;
     for(i = 0; i<=7; i++){
        Segs_Custom(i, 0b10000000);
     }
}

// say Err on the appropriate line
void Segs_SayErr (Segs_LineOption Line){
   Segs_Custom(Line*4, 0b11001111);
   Segs_Custom(Line*4+1, 0b10001100);
   Segs_Custom(Line*4+2, 0b10001100);
}

// deprecated after term 1202
// say 'help' as best the 7-segs can show
void Segs_SayHelp (void){
   Segs_Custom(0, 0b11011110);
   Segs_Custom(1, 0b11001111);
   Segs_Custom(2, 0b10001011);
   Segs_Custom(3, 0b11101110);
}


