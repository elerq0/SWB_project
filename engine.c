#include <REGX52.H>
#define IN1 P2_1
#define IN2 P2_2
#define IN3 P2_5
#define IN4 P2_6
#define EN1 P2_3
#define EN2 P2_4

volatile unsigned char timer1 = 0;
volatile unsigned char timer2 = 0;

void main(void)
{
	EX0 = 1;
	IT0 = 1;
	EX1 = 1;
	IT1 = 1;
	EA  = 1;
	
	IN1 = 0;
	IN2 = 1;
	EN1 = 1;
	EN2 = 1;
	IN3 = 1;
	IN4 = 0;
	
	P1_7 = 0;
	
	while(1)
	{
	PCON |= 0x01;
	}
}

void but(void) interrupt 0
	 {
		 timer1 += 1;
	   if(timer1 > 2)
		 {
			 EN1 = !EN1;
			 IN1 = !IN1;
			 IN2 = !IN2;
			 EN1 = !EN1;
			 timer1 = 0;			 
		 }
	 }
	 
void but2(void) interrupt 2
	 {
	   timer2 += 1;
		 if(timer2 > 2)
		 {
			 P1_7 = !P1_7;
			 timer2 = 0;
		 }
	 }