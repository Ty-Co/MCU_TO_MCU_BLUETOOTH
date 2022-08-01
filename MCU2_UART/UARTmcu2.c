//	Tylor Cooks
// 	UART MCU2
//	10/20/2021
//	CECS447
// 	YOUTUBE VIDEO: https://www.youtube.com/watch?v=w_MFGEgZQ6w&ab_channel=b


#include "PLL.h"
#include "UART.h"
#include "tm4c123gh6pm.h"

#define RED 		0x02
#define GREEN  	0x08
#define BLUE 		0x04
#define PURPLE 	0x06
#define WHITE 	0x0E
#define DARK 		0x00

void OutCRLFB(void);
void PortF_Init(void);
void mode2(void);
void mode3(void);
void Delay(void);
void OutCRLF(void);


unsigned char CharNum;
unsigned char Choice = '0';

unsigned long Num = 0;
unsigned long Sent = 1;
unsigned long mode = 0;
unsigned char ColorLetter[]={'r','g','b','p','w','d'};
unsigned int 	Colors[] = {RED, GREEN, BLUE, PURPLE, WHITE, DARK};
char MessageRecieved[20];
char Message[20];
//---------------------OutCRLF---------------------
// Output a CR,LF to UART to go to a new line
// Input: none
// Output: none

//debug code
int main(void){
 
  PLL_Init();
	UART_InitB();
  PortF_Init();

 
  while(1){
		
		Choice = UART_InCharB();	
		if(Choice == '2'){
			mode2();
			CharNum ='0';}
		else if(Choice == '3'){
			mode3();
		
		}



  }
}

void mode2(){
	
	while(CharNum != '!'){
		CharNum = UART_InCharB();
		Num = CharNum - '0';
			Sent = 0;
			while(Sent == 0 && CharNum !='!'){
					GPIO_PORTF_DATA_R = Colors[Num];
			}
	}
}

void mode3(){
	
	UART_InStringB(Message,19);
	UART_OutStringB(Message);

}


void OutCRLFB(void){
  UART_OutCharB(CR);
  UART_OutCharB(LF);
}

void PortF_Init(){
	SYSCTL_RCGC2_R |= 0x20; 
	// Port F_Buttons
		
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   	// unlock PortF PF0  
	GPIO_PORTF_CR_R |= 0x1F;         		// allow changes to PF4-0 :11111->0x1F     
  GPIO_PORTF_AMSEL_R &= ~0x1F;        // disable analog function
  GPIO_PORTF_PCTL_R &= ~0x000FFFFF; 	// GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R &= ~0x11;          // PF4,PF0 input   
  GPIO_PORTF_DIR_R |= 0x0E;          	// PF3,PF2,PF1 output   
	GPIO_PORTF_AFSEL_R &= ~0x1F;        // no alternate function
  GPIO_PORTF_PUR_R |= 0x11;          	// enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R |= 0x1F;          	// enable digital pins PF4-PF0  

	GPIO_PORTF_IS_R 			&= ~0x11;       // PF4,0 is edge-sensitive
	GPIO_PORTF_IBE_R 			&= ~0x11;       // PF4,0 is not both edges
	GPIO_PORTF_IEV_R 			&= ~0x11;    		// PF4,0 falling edge event
	GPIO_PORTF_ICR_R 			|= 0x11;      	// clear flag4,0
	GPIO_PORTF_IM_R 			|= 0x11;      	// arm interrupt on PF4,0
	NVIC_PRI7_R 					|= (NVIC_PRI7_R&0xFF1FFFFF)|0x0020000; //priority 1
	NVIC_EN0_R 						|= 0x40000000;      // enable interrupt 30 in NVIC	

}

void GPIOPortF_Handler(void){
	Delay();
	if(GPIO_PORTF_RIS_R&0x01){		//SW2
		CharNum=Num + '0';
		UART_OutCharB(CharNum);
		Sent= 1;}
	if(GPIO_PORTF_RIS_R&0x10){	//SW1
		Num++;
		Num = Num%6;}
		
	GPIO_PORTF_ICR_R|=0x11;
	
}



void Delay(){
	unsigned long volatile time;
  time = 500000;  // 0.1sec
  while(time){
		time--;
  }
}
