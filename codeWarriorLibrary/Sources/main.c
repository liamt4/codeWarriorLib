#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "swl_led.h"
#include "sci.h"
#include "pit.h"
#include "segs.h"
#include "lcd.h"




unsigned char c;
unsigned char buffer[256] = {0};

void main(void) {
  /* put your own code here */
   SWL_Init();
   PLL_To20MHz();
   sci0_Init(20000000, 9600);
   Segs_Init();
   LCD_Init();



  for(;;) {
    LCD_StringRC(0, 4, "------------");

     //Segs_SayHelp();
  
  /*sci0_txByte('T');
  sci0_txByte('e');
  sci0_txByte('s');
  sci0_txByte('t');
  sci0_txByte('\r'); */
  
  
  
   /* do{
     c = sci0_bread();
     sci0_txStr("You typed a ");
     sci0_txByte(c);
     sci0_txByte('\r'); 
    }while(c != 'q');*/   
  
  /*sci0_txStr("Enter a string: ");
  sci0_rxStr(buffer,256);
  sci0_txStr("You typed: ");
  sci0_txStr(buffer);
  sci0_txStr("\r\r");*/ 
  
  
  /*sci0_txStr("Now you can use r,y,g keys to control lights... ");
	while(1){
	 if(sci0_rxByte(&c)){
	  
           if(c == 'r'){
               SWL_TOG(SWL_RED);
           }else if(c == 'y'){
               SWL_TOG(SWL_YELLOW);
           }else if(c == 'g'){
               SWL_TOG(SWL_GREEN);
           }
      
     	}
	}*/   
	


	    /*while(1){
	     PIT_Sleep(PIT_CH0, 20, 10);
	     SWL_TOG(SWL_RED); 
	    }*/
	  
  } /* loop forever */
  /* please make sure that you never leave main */
}
