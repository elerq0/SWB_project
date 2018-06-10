#include <REGX52.H>                    
volatile unsigned char newValue, oldValue, leftValue, rightValue, temp;

unsigned char code tableTranslation[] = { 0x06, 0x5B, 0x4F,
																					0x66, 0x6D, 0x7D,
																					0x07, 0x7F, 0x6F,
																					0x3F, 0x77, 0x71,
																					0x00};


void SIR_SerialPort(void) interrupt 4
  {
  	if (RI == 1)
	  {
			temp = SBUF;
			if((temp == 10) || (temp == 11))
			{
				leftValue = oldValue;
				rightValue = newValue;				
			}
			else
			{
			oldValue = newValue;
	  	newValue = temp;
			}
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
	
	ET0 = 1;
	TR0 = 1;
	EA = 1;
	
	leftValue = 12;
	rightValue = 12;
	oldValue = 12;
	newValue = 12;
	P2 = 0x00;
	P2_0 = 1;
	P2_1 = 0;
  while(1)
	{
	PCON |= 0x01;
	}
}

void time(void) interrupt 1
{
	TH0 = 0xFE;
	if(P2_1 == 0)
	{
	P2_0 = 1;
	P2_1 = 1;
	P0 = tableTranslation[leftValue];
	P2_0 = 0;
	}
	else
	{
	P2_0 = 1;
	P2_1 = 1;
	P0 = tableTranslation[rightValue];
	P2_1 = 0;
	}
}