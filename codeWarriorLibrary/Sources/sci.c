#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "sci.h"



 unsigned int i;

//initialize
unsigned long sci0_Init(unsigned long ulBaudRate, int iRDRF_Interrupt){
     SCI0CR1 = 0b00000000;
     SCI0CR2 = 0b00101100;
     SCI0BD = ulBaudRate/16 / iRDRF_Interrupt;
}



//read a byte with blocking
unsigned char sci0_bread(void){
      while(!(SCI0SR1 & SCI0SR1_RDRF_MASK));
      return SCI0DRL;
      
}



//read a byte without blocking
unsigned char sci0_rxByte(unsigned char * pData){
      if(SCI0SR1 & SCI0SR1_RDRF_MASK){
             *pData = SCI0DRL;
             return 1;
      }else{return 0;}
}



//send a byte
 



//send a string
void sci0_txStr (char const * straddr){
    i = 0;
      while(straddr[i] != 0){
      while((SCI0SR1 &SCI0SR1_TDRE_MASK))
        SCI0DRL = straddr[i++];
      }
}




int sci0_rxStr (char * const sz, int bufferSize)
{

    int count = 0;
    unsigned char c = 0;

    if(sz && bufferSize) {    ///

        while(bufferSize > count) 
        {
          c=sci0_bread();
          if(c!='\r' && c!=0)
              sz[count++]=c;

          else
              break;
        }

        sz[count]=0;
        return count;
    } else return -1;   ///
    
}

/*recieving and sending a string
unsigned char buffer[256] = {0};
  sci0_txStr("Enter a string: ");
  sci0_rxStr(buffer,256);
  sci0_txStr("You typed: ");
  sci0_txStr(buffer);
  sci0_txStr("\r\r"); 
*/

/*turns on a corresponding led if r,y,g are pressed and then 1 and 0 determine on or off
int mode = 0; char c;
c = sci0_bread();
sci0_txByte(c);
switch(c)
{
  case 'r':
    mode = 1;
    break;
  case 'y':
    mode = 2;
    break;
  case 'g':
    mode = 3;
    break;
  case '1':
    switch (mode)
    {
      case '1':
        SWL_ON(SWL_RED);
        break;
      case '2':
        SWL_ON(SWL_YELLOW);
        break;
      case '3':
        SWL_ON(SWL_GREEN);
        break;
    }
    break;
  case '0':
    switch (mode)
    {
      case '1':
        SWL_OFF(SWL_RED);
        break;
      case '2':
        SWL_OFF(SWL_YELLOW);
        break;
      case '3':
        SWL_OFF(SWL_GREEN);
        break;
    }
    break;
  default:
    mode = 0;
    break;   
}
*/