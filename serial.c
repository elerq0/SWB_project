#include <REGX52.H>                    
volatile unsigned char newValue, oldValue;

unsigned char code tableTranslation[] = { 0x06, 0x5B, 0x4F,
																					0x66, 0x6D, 0x7D,
																					0x07, 0x7F, 0x6F,
																					0x3F, 0x77, 0x71};


void SIR_SerialPort(void) interrupt 4
  {
  	if (RI == 1)
	  {
			oldValue = newValue;
	  	newValue = SBUF;
	    RI = 0;
	  }
  }
	
void main(void){
  SCON = 0x50;
	TMOD = 0x20;
	PCON = 0x80;
	TH1 = 0xFF;
	TL1 = 0xFF;
	TR1 = 1;
	IE |= 0x90;
	
	oldValue = 0;
	newValue = 0;
	P2 = 0x00;
	P2_0 = 1;
	P2_1 = 0;
  while(1)
	{
	P0 = tableTranslation[newValue];
	}
}

void lele(void)
{
	
	P0 = 0x00;
	P0 = tableTranslation[oldValue];
	P2_0 = !P2_0;
	P2_1 = !P2_1;
	P0 = 0x00;
	
	P2_0 = !P2_0;
	P2_1 = !P2_1;
	
}