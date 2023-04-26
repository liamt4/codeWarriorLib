#include <hidef.h>      /* common defines and macros */
#include "derivative.h" /* derivative-specific definitions */

#include "lcd.h"
#include "pit.h"

/////////////////////////////////////////////////////////////////////////////
// Globals and Constants
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Function Implementations
/////////////////////////////////////////////////////////////////////////////

// Set up the Hitachi
// NOTE:  Assumes PIT already set up!
void LCD_Init(void)
{
    PTH = 0x38; // Function Set = 8 bit i.f., 2 line, 5 * 7

    LCD_EDown;  // not enabled
    LCD_RWDown; // writing
    LCD_RSDown; // command

    DDRK |= (0x07); // set port K0:2 as outputs

    // all pins now are low
    // e could have been running a little high
    // cause delay to allow LCD to get it together
    // spec says 40ms after Vcc > 2.7V, but seems to need much longer
    PIT_Sleep(PIT_CH0, 20, 100);
    // once
    LCD_EUp;
    LCD_EDown;
    PIT_Sleep(PIT_CH0, 20, 15);

    // twice
    LCD_EUp;
    LCD_EDown;
    PIT_Sleep(PIT_CH0, 20, 1);

    // busy flag valid now
    // 3rd time, after busy has completed (init)
    LCD_Inst(0x38);
    LCD_Inst(0x0c); // Disp Control = Disp On, no cursor, no blink
    LCD_Inst(0x01); // Clear Disp
    LCD_Inst(0x06); // Entry Mode Set = inc + no disp shift
}

void LCD_Busy(void) // Block until LCD is accepting input
{
    byte inVal = 0; // Buffer for input
    // Uncomment below for debugging, convert busy flag detection
    //  to simple delay loop.
    //{PIT_Sleep(PIT_CH0, 20, LCD_INST_DELAY_DEFAULT); return;}

    LCD_RSDown; // RS = 0 Instruction
    LCD_RWUp;   // RW = 1 Reading from

    do // Read current state
    {
        LCD_EUp;
        LCD_MicroDelay;
        inVal = PTH;
        LCD_EDown;
    } while (inVal & 0x80);
}

// Execute a command on the Hitachi, preceded by fixed delay in ms
// Useful for debugging to see if the LCD lagging is the issue by adding delay.
void LCD_Inst_Delay(byte val, byte ms)
{
    PIT_Sleep(PIT_CH0, 20, ms); // Wait specified duration

    LCD_RWDown; // Writing
    LCD_RSDown; // Command

    PTH = val; // Put command on port H

    // Strobe Enable line to execute
    LCD_EUp;
    LCD_MicroDelay;
    LCD_EDown;
}

// Execute a command on the Hitachi paying attention to busy flag
void LCD_Inst(byte val)
{
    LCD_Busy(); // Wait until device is ready
    LCD_RWDown; // Writing
    LCD_RSDown; // Command

    PTH = val; // Put command on port H

    // Strobe Enable line to execute
    LCD_EUp;
    LCD_MicroDelay;
    LCD_EDown;
}

// Pass data to the Hitachi
void LCD_Data(byte val)
{
    LCD_Busy(); // Wait until device is ready
    LCD_RWDown; // Writing
    LCD_RSUp;   // Data

    PTH = val; // Put data on the h port

    // Strobe the enable to latch data
    LCD_EUp;
    LCD_MicroDelay;
    LCD_EDown;
}

// Move to specified location (actual index)
void LCD_Addr(byte addr)
{
    LCD_Inst(eLCD_Set_DDAddr_PFX | addr);
}

// Go to specified location (inferred from col,row) 0,0 is top left
void LCD_AddrRC(byte row, byte col)
{
    byte addr = 0;
    // Sanitize input using modulus
    row %= LCD_ROWS;
    col %= LCD_COLS;
    
    // Start with offset to first cell on that row
    switch (row)
    {
    case 0:
        addr = 0x00;
        break;
    case 1:
        addr = 0x40;
        break;
    case 2:
        addr = 0x14;
        break;
    case 3:
        addr = 0x54;
        break;
    }
    // Add the column number to the offset to get the cell
    addr += col;
    LCD_Addr(addr);
}

void LCD_String(char szaddr[])
{
    int curr = 0; // Position counter
    while (szaddr[curr] && curr < 20)
        LCD_Data(szaddr[curr++]);
}

void LCD_StringRC(byte row, byte col, char szaddr[])
{
    LCD_AddrRC(row, col);
    LCD_String(szaddr);
}

void LCD_DispControl(unsigned char curon, unsigned char blinkon)
{
    LCD_Inst(eLCD_Display_PFX | eLCD_DP_Display_On | curon ? eLCD_DP_Cursor_Visible : 0 | blinkon ? eLCD_DP_Cursor_Blink
                                                                                                  : 0);
}

void LCD_Clear(void)
{
    LCD_Inst(eLCD_Clear);
}

void LCD_Home(void)
{
    LCD_Inst(eLCD_Home);
}

// Shift count spaces in direction (true is left)
// Pause is number of ms between each shift
void LCD_Shift(byte direction, byte count, byte pause)
{ // Turn on shifting
    int i = 0;
    LCD_Inst(eLCD_Entry_Mode_PFX | eLCD_EM_LtoR | eLCD_EM_Shift_Enabled);

    // Shift
    for (i = 0; i < count; ++i)
    {
        LCD_Inst(eLCD_CursorShift_PFX | eLCD_CS_Shift_Display | direction ? eLCD_CS_RtoL : 0);
        TMR_Sleep(pause);
    }
    // Turn off shifting
    LCD_Inst(eLCD_Entry_Mode_PFX | eLCD_EM_LtoR | ~eLCD_EM_Shift_Enabled);
}