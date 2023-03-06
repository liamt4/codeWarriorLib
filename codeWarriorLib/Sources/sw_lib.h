
#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

// Switch and indicator LED library
// Revision History:
// May 9th 2014 - Initial Port to C - Simon Walker
// June 2018 - Minor change to AnyButton, typedef on enum added
// Oct 2018 - Added enum switch function, separated debounce/non-debounce versions
// Dec 2020 - Simplified function names
// Sep 2021 - Debounce option added to Transition

// enum to describe LED indicator positions
// tag values represent actual bit positions
typedef enum LEDColour 
{
  SWL_RED     = 0b10000000,
  SWL_YELLOW  = 0b01000000,
  SWL_GREEN   = 0b00100000
} SWL_LEDColour;

// enum to describe mc switch positions
// tag values represent actual bit positions
typedef enum SwitchPos 
{
  SWL_CTR  = 0b00000001,
  SWL_RIGHT= 0b00000010,
  SWL_DOWN = 0b00000100,
  SWL_LEFT = 0b00001000,
  SWL_UP   = 0b00010000 
} SWL_SwitchPos;

// option to add debounce in switch state checks
typedef enum DebounceOption
{
  SWL_DebOff,
  SWL_DebOn
} DebounceOption;

// init - must be called to init port PT1AD1
void SWL_Init (void);

///////////////////////////////////////////////////////////////
// LED functions
void SWL_ON (SWL_LEDColour led);
void SWL_OFF (SWL_LEDColour led);
void SWL_TOG (SWL_LEDColour led);
int SWL_CHECKON (SWL_LEDColour led);

///////////////////////////////////////////////////////////////
// special LED functions (does not apply to CNT courses)
//void SWL_AUX_ON (void);
//void SWL_AUX_OFF(void);

///////////////////////////////////////////////////////////////
// switch functions

// is a specific switch being pushed (T/F (1/0))
int SWL_Pushed (SWL_SwitchPos pos);

// is any switch being pushed (T/F (1/0))
int SWL_Any (void);
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
// you won't implement these function initially
// after we talk about some switch related issues, you will be
//  implementing these functions - possibly in CMPE2250

// by switch name
//int SWL_PushedDeb (SWL_SwitchPos pos);

// look for transition by switch name (compares against previous call)
//int SWL_Transition (SWL_SwitchPos pos, DebounceOption deb);
///////////////////////////////////////////////////////////////


