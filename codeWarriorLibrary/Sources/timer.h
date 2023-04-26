 typedef enum PIT_Channel 
{
  PIT_CH0 = PITMUX_PMUX0_MASK,
  PIT_CH1 = PITMUX_PMUX1_MASK,
  PIT_CH2 = PITMUX_PMUX2_MASK,
  PIT_CH3 = PITMUX_PMUX3_MASK
} PIT_Channel;

typedef enum PIT_MicroTimer 
{
PIT_MT0,
PIT_MT1
} PIT_MicroTimer;

typedef enum PIT_Interrupt 
{
  PIT_Int_Disabled = 0,
  PIT_Int_Enabled = 1
} PIT_Interrupt;


void PIT_InitChannel(PIT_Channel ch, PIT_MicroTimer mt, PIT_Interrupt ie);

void PIT_Set1msDelay(PIT_Channel ch);
int PIT_SetXmsEvent(PIT_Channel ch, PIT_MicroTimer mt, unsigned int  ms); 

void PIT_Start(); 

int PIT_Init(PIT_Channel chan, PIT_Interrupt intState, unsigned long ulBusRate, unsigned long ulInterval_us);

//int PIT_Sleep(PIT_Channel chan, unsigned long ulBusRate, unsigned int ms);
void PIT_Sleep(PIT_Channel ch, unsigned int ms);