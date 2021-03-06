#include <REGX52.H>

// SKANOWANIE KOLUMN //
/*
unsigned char code tableZeroRotating[] = {0xEF, 0xDF, 0xBF, 0x7F};
#define condition (P2_1 && P2_2 && P2_3) == 0
unsigned char code tableLength = 4;
*/
// SKANOWANIE WIERSZY

unsigned char code tableZeroRotating[] = {0xFD, 0xFB, 0xF7};
#define condition (P2_4 && P2_5 && P2_6 && P2_7) == 0
unsigned char code tableLength = 3;

/////////////////////////////////


unsigned char code tableKeyboard[] = 		{ 0xE7, 0xEB, 0xED,
																					0xD7, 0xDB, 0xDD,
																					0xB7, 0xBB, 0xBD,
																					0x7B, 0x77, 0x7D};
unsigned char code tableTranslation[] = { 0x06, 0x5B, 0x4F,
																					0x66, 0x6D, 0x7D,
																					0x07, 0x7F, 0x6F,
																					0x3F, 0x77, 0x71};

unsigned char leftPos, rightPos;
unsigned char oldValue, newValue;
unsigned char iterator;
unsigned char i;
unsigned char j;
unsigned char old, counter;
																					
																				
void main(void)
{
	leftPos = 0x00;
	rightPos = 0x00;
	oldValue = 0x00;
	newValue = 0x00;
  iterator = 0;
	i = 0;
	old = 0x00;
	counter = 0x00;
	P2 = 0xFF;
	
	IT0 = 1;
  EX0 = 1;
  IT1 = 1;
  EX1 = 1;
	
	TMOD = 0x00;
	ET0 = 1;
	TR0 = 1;
	EA = 1;
	
	SCON = 0x40;  // Tryb 1: 8-bit uart, zmienna predkosc transmisji
	TMOD |= 0x20;  
	PCON = 0x80;  // SMOD = 1: predkosc transmisji x 2
	TH1 = 0xFF;
	TL1 = 0xFF;
	TR1 = 1;
	
	P3_6 = 1;
	P3_7 = 0;
	
	while(1)
	{
		P0_2 = 1;
		P2 = tableZeroRotating[iterator];
		iterator += 1;
		if(iterator == tableLength) iterator = 0;
		while(condition) 
			{			
			P0_2 = 0;
			for(i = 0; i < 10; i++)
				{
					if(P2 == tableKeyboard[i])
					{
						oldValue = newValue;
						newValue = tableTranslation[i];
						SBUF = i;
						while(condition) {;}
					}
					
				}
			for(i = 10; i < 12; i++)
				{
					if(P2 == tableKeyboard[i]) 
					{
						leftPos = oldValue;
						rightPos = newValue;
						SBUF = i;
						while(condition) {;}
					}
				}
			}
	}
}

void but1 (void) interrupt 0
{
	for(i = 0; i<10; i++)
	{
		if(rightPos == tableTranslation[i]) j = i;		
	}
	if(j!=0)
	{
		if(j == 9)
		{
			j -= 1;
			rightPos = tableTranslation[j];
			for(i = 0; i<10; i++)
			{
				if(leftPos == tableTranslation[i]) j = i;		
			}
			if(j!=0)
			{
				j -= 1;
				leftPos = tableTranslation[j];
			}
			else
			{
				j = 9;
				leftPos = tableTranslation[j];
			}
		}
		else
		{
		j -= 1;
		rightPos = tableTranslation[j];
		}
	}
	else
	{
	j = 9;
	rightPos = tableTranslation[j];
	}
}

void but2 (void) interrupt 2
{
	for(i = 0; i<10; i++)
	{
		if(rightPos == tableTranslation[i]) j = i;		
	}
	if(j!=9)
	{
		if(j == 8)
		{
			j += 1;
			rightPos = tableTranslation[j];
			for(i = 0; i<10; i++)
			{
				if(leftPos == tableTranslation[i]) j = i;		
			}
			if(j!=9)
			{
				j += 1;
				leftPos = tableTranslation[j];
			}
			else
			{
				j = 0;
				leftPos = tableTranslation[j];
			}
		}
		else
		{
		j += 1;
		rightPos = tableTranslation[j];
		}
	}
	else
	{
	j = 0;
	rightPos = tableTranslation[j];
	}	
}

void tim (void) interrupt 1
{
	TH0 = 0xFE;
	if(P3_7 == 0)
	{
	P3_6 = 1;
	P3_7 = 1;
	P1 = leftPos;
	P3_6 = 0;
	}
	else
	{
	P3_6 = 1;
	P3_7 = 1;
	P1 = rightPos;
	P3_7 = 0;
	}
}