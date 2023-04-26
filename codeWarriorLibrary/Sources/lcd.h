#pragma once
#ifndef LCDH
#define LCDH

/////////////////////////////////////////////////////////////////////////////
// Processor:     MC9S12XDP512
// Bus Speed:     20 MHz (Requires Active PLL)
// Author:        AJ Armstrong
// Details:       library for Hitachi LCD Display controller via GPIO
// Revision History :
// Created : 12 Mar 2021
// 12 Mar 2021  - Initial implementation
//
/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////
// 8-Bit interface init on LCD
// LCD is wired to PTH for data, PK0:2 for control :
// 2     1     0     PTK
// A     R/W*  E     LCD
////////////////////////////////////////////////////

////////////////////////////////////////////////////
// LCD Address Scheme (HEX):
// 00 01 ... 12 13
// 40 41 ... 52 53
// 14 15 ... 26 27
// 54 55 ... 66 67
////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// Macros and Constants
/////////////////////////////////////////////////////////////////////////////
#define LCD_RWUp DDRH = 0; PORTK |= 2;
#define LCD_RWDown PORTK &= (~2); DDRH = 0xFF;

//This is borderline for timing.  If problems, add microdelay
#define LCD_EUp PORTK |= 1;
#define LCD_EDown PORTK &= (~1);
#define LCD_RSUp PORTK  |= 4;
#define LCD_RSDown PORTK  &= (~4);
#define LCD_MicroDelay { char __x = 1; while(--__x); }

// Magic number aggregation
#define LCD_COLS 20 //Number of letters per on screen
#define LCD_ROWS 4 //Number of displayed lines (TODO: Automate detection)
#define LCD_INST_DELAY_DEFAULT 10 //ms, delay used when busy flag unavailable
/////////////////////////////////////////////////////////////////////////////
// Types
/////////////////////////////////////////////////////////////////////////////

typedef enum eLCD_Commands
  {
    eLCD_Setup = 0x38, //Function set: DataLength 1 byte; 4 Lines, font 0
    eLCD_Off = 0x0C,
    eLCD_Clear = 0x01,
    eLCD_Home = 0x02,
    //
    eLCD_Entry_Mode_PFX = 0x04,
    eLCD_EM_LtoR = 0x02,
    eLCD_EM_Shift_Enabled = 0x01,
    //
    eLCD_Display_PFX = 0x08,
    eLCD_DP_Display_On = 0x04,
    eLCD_DP_Cursor_Visible = 0x02,
    eLCD_DP_Cursor_Blink = 0x01,
    //
    eLCD_CursorShift_PFX = 0x10,
    eLCD_CS_Shift_Display = 0x08,
    eLCD_CS_RtoL = 0x04,

    eLCD_Set_DDAddr_PFX = 0x80, //Or with address cell (7 bits)
    eLCD_Set_CGAddr_PFX = 0x40 //Or with address cell (6 bits)
  } eLCD_Commands;

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned long int ulong;

/////////////////////////////////////////////////////////////////////////////
// Library Prototypes
/////////////////////////////////////////////////////////////////////////////

void LCD_Init (void);
void LCD_Busy(void); //Block until LCD is accepting input
void LCD_Inst_Delay (byte val, byte ms);
void LCD_Inst (byte  val);
void LCD_Data (byte val);
void LCD_Addr (byte addr);
void LCD_AddrRC (byte row, byte col);
void LCD_String (char const * straddr);
void LCD_StringRC (byte row, byte col, char szaddr[]);
void LCD_DispControl (unsigned char curon, unsigned char blinkon);
void LCD_Clear (void);
void LCD_Home (void);
void LCD_Shift (byte direction, byte count, byte pause);

#endif