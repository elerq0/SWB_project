#include <REGX52.H>

#define RS    P2_4
#define RW    P2_5
#define EN    P2_6 

static unsigned char code sign[] = {0x00, 0x1F, 0x0A, 0x0A, 0x0A, 0x0A, 0x1F, 0x00};
unsigned char i,j,k;
char tempI,tempJ;
static unsigned char code row[] = {0x80, 0xC0, 0x90, 0xD0};
static unsigned char code column[] = {0,1,2,3,4,5,6,7,8,9,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
static unsigned char code text[] = {"QWERTYUIOP"};

static bit priority;

void delay()
{
	unsigned int pauss = 0;
	for(pauss = 0;pauss <999; pauss++);
}

void Lcd_CMD (unsigned char value)
 {
   unsigned char temp = 0;
   static bit ready; 
   RS = 0;
   RW = 0;
   temp = value >> 4;
   P2 &= 0xF0;
   P2 |= temp;
   EN = 1;
   EN = 0;
   temp = value & 0x0F;  
   P2 &=0xF0;
   P2 |= temp;
   EN = 1;
   EN = 0;
   P2 |= 0x0F;
   RW = 1;
   RS = 0;
			ready = 1;
      while (ready == 1)
       {
         EN = 1;
         ready = P2_3;
         EN	= 0;
         EN = 1;
         EN	= 0;
       }  
 }
 
static void Lcd_DAT (unsigned char value)
 {
   unsigned char temp = 0;
   static bit ready;
   RS = 1;
   RW = 0;
   temp = value >> 4;
   P2 &= 0xF0;
   P2 |= temp;
   EN = 1;
   EN = 0;
   temp = value & 0x0F;  
   P2 &=0xF0;   
   P2 |= temp;
   EN = 1;
   EN = 0;
   P2 |= 0x0F;
   RW = 1;
   RS = 0;
   ready = 1;
   while (ready == 1)
    {
     EN = 1;
     ready = P2_3;
     EN	= 0;
     EN	= 1;
     EN	= 0;
    }             
 }


void LcdInit(void)
 {
   P2 = 0x83;
   P2_0 = 0;
   EN = 1;
   delay();
   EN = 0;
   delay();
   Lcd_CMD(0x28);  // 4x16   
   Lcd_CMD(0x01);  // clear    
   Lcd_CMD(0x08);  // display off
   Lcd_CMD(0x0C);  // display on
  // Lcd_CMD(0x0F);  // curson com
   Lcd_CMD(0x01);  // clear
   IT0 = 1;
	 IT1 = 1;
	 EX0 = 1;
	 EX1 = 1;
	 EA = 1;
 }
 
 void signCreate(void)
 {
	 Lcd_CMD(0x40);
	 for(i = 0;i < 8; i++) Lcd_DAT(sign[i]);
 }
 
 void buttonSign(void)
 {
	 Lcd_CMD(0x01);
	 delay();
	 Lcd_CMD(row[j] + column[i]);   
   Lcd_DAT(0x00);
 }
 
 void buttonText(void)
 {
	 Lcd_CMD(0x01);
	 delay();
	 tempI = i;
	 tempJ = j;
	 for(k = 0; k <  10; k++)
	 {
		 if((tempI + k) > 15) 
		 {
			tempI = -k;
			tempJ+=1;
			if(tempJ == 4)
				tempJ = 0;
		 }
		 Lcd_CMD(row[tempJ] + column[tempI + k]);
		 Lcd_DAT(text[k]);
	 }
 }
 
 void main(void)
  {
   LcdInit();
	 delay();
	 signCreate();
	 delay();
	 i = 0;
	 j = 0;
	 priority = 0;
	 buttonText();
   while(1) 
	 {
		if(P1_4 == 0)
		{
			priority = 0;
			buttonText();
			while(P1_4 == 0) {;}
		}
		if(P1_7 == 0)
		{
			priority = 1;
			buttonSign();
			while(P1_7 == 0) {;}
		}
	 }
	}
 
 void but1(void) interrupt 0
 {
	 if(i == 0)
	 {
		 i = 16;
		 if(j == 0)
		 {
			 j=4;
		 }
		 j-=1;
	 }
	 i-=1;
	 if(!priority) buttonText();
	 else buttonSign();
 }

 
 void but2(void) interrupt 2
 {
	 i+=1;
	 if(i == 16)
	 {
		 i = 0;
		 j+= 1;
	 }
	 if(j == 4)
	 {
		 j = 0;
	 }
	 if(!priority) buttonText();
	 else buttonSign();
 }