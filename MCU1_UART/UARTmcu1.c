// TYCO
// 	UART MCU1
//	10/20/2021
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


//---------------------OutCRLF---------------------
// Output a CR,LF to UART to go to a new line
// Input: none
// Output: none
void Menu_Display(void);
void mode1(void);
void mode2(void);
void mode3(void);
void OutCRLF(void);
void OutCRLFB(void);
void PortF_Init(void);
void PortD_Init(void);
void Delay(void);


int Send = 0;
int Recieve = 0;
unsigned long UartFlag = 0;
unsigned int Index = 0;
unsigned long NumChoice=0;

unsigned char Choice = '0';
unsigned char CharIndex = '0';
unsigned char Light;
unsigned char Temp = DARK;

char MessagePC[20];  
char MessageMCU2[20];

unsigned char ColorLetter[]={'r','g','b','p','w','d'};
unsigned int 	Colors[] = {RED, GREEN, BLUE, PURPLE, WHITE, DARK};

//debug code
int main(void){

  PLL_Init();
  UART_InitA(); 
	UART_InitB();
  PortF_Init();
	PortD_Init();
	

	//OutCRLF(); newline 
	
  while(1){
		
		if(Choice == '0')
			Menu_Display();
		else if(Choice == '1')
			mode1();
		else if(Choice =='2')
			mode2();
		else if(Choice =='3')
			mode3();

		
  }
}

void Menu_Display(){
		OutCRLF();
		OutCRLF();
    UART_OutString("WELOME to CECS 447 Project 2 - UART");
		OutCRLF();
		UART_OutString("Please choose a communication mode(type 1 or 2 or 3):");
		OutCRLF();
		UART_OutString("     1. PC <--> MCU_1 only");
		OutCRLF();
		UART_OutString("     2. MCU_1 <--> MCU_2 LED Control");
		OutCRLF();
		UART_OutString("     3. PC <--> MCU_1 <--> MCU_2 Messenger");
		OutCRLF();
		UART_OutString("Enter: ");
		NumChoice = UART_InUDec();
		Choice = NumChoice + '0';
		OutCRLF();
		UART_OutCharB(Choice);
}

void mode1(){
	
	UART_OutString("MODE 1");
	OutCRLF();
	
	UART_OutString("'r'= red, 'g'= green, 'b'= blue, 'p'= purple, 'w'= white, 'd'= dark");
	OutCRLF();
	
	UART_OutString("Enter: "); 
	
	Temp = UART_InChar(&UartFlag);
	UART_OutChar(Temp);
	while(Light != CR){
	Light = UART_InChar(&UartFlag);}
	Light = Temp;
	OutCRLF();
	

	if(Light == 'r'){
		GPIO_PORTF_DATA_R = RED;
		UART_OutString("red LED is on.");
		OutCRLF();}
	else if(Light == 'g'){
		GPIO_PORTF_DATA_R = GREEN;
		UART_OutString("green LED is on.");
		OutCRLF();}
	else if(Light == 'b'){
		GPIO_PORTF_DATA_R = BLUE;
		UART_OutString("blue LED is on.");
		OutCRLF();}
	else if(Light == 'p'){
		GPIO_PORTF_DATA_R = PURPLE;
		UART_OutString("purple LED is on.");
		OutCRLF();}
	else if(Light == 'w'){
		GPIO_PORTF_DATA_R = WHITE;
		UART_OutString("white LED is on.");
		OutCRLF();}
	else if(Light == 'd' || Light == 0){
		GPIO_PORTF_DATA_R = DARK;
		UART_OutString("no LED is on.");
		OutCRLF();}
		OutCRLF();
	

}

void mode2(){
	Index = 0;
	while(Choice == '2'){
		
		while(Recieve == 0){
		GPIO_PORTF_DATA_R = Colors[Index];}
		Index = UART_InCharB(&UartFlag) -'0';
		Recieve = 0;
	
	}
}

void mode3(){
	
	UART_OutString("In String To PC: ");
	UART_InString(MessagePC,19);
	UART_OutString(MessagePC);
	OutCRLF();
	UART_OutStringB(MessagePC);
	OutCRLF();
	UART_InStringB(MessageMCU2,19);
	OutCRLF();
	UART_OutString(MessageMCU2);

}
	
void OutCRLF(void){
  UART_OutChar(CR);
  UART_OutChar(LF);
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
		if(GPIO_PORTF_RIS_R&0x01){			//SW2
			UART_OutCharB(Index+'0');
			Recieve = 1;
		}
		if(GPIO_PORTF_RIS_R&0x10){	//SW1
			Index++;
			Index = Index%6;
			}

	GPIO_PORTF_ICR_R|=0x11;
	
}


void PortD_Init(){
	SYSCTL_RCGC2_R |= 0x08; 
	// Port D_Buttons
	GPIO_PORTD_LOCK_R 		 = 0x4C4F434B;  // 2) unlock PortD  	
	GPIO_PORTD_AMSEL_R 		&= ~0x01;  			// disable analog functionality on PD0
	GPIO_PORTD_PCTL_R 		&= ~0x0000000F; // configure PD0 as GPIO
	GPIO_PORTD_DIR_R 			&= ~0x01;    		// make 0 INPUT (built-in button)
	GPIO_PORTD_AFSEL_R 		&= ~0x01;  			// disable alt funct on PD0
	GPIO_PORTD_CR_R 			|= 0x01; 			  // allow changes to PD0
	GPIO_PORTD_PUR_R 			|= 0x01;     		// enable weak pull-up on PF0
	GPIO_PORTD_DEN_R 		  |= 0x01;     		// enable digital I/O on PFD
	
		// Interrupt 
	GPIO_PORTD_IS_R 			&= ~0x01;       // PD0-3 is edge-sensitive
	GPIO_PORTD_IBE_R 			&= ~0x01;       // PD0-3 is not both edges
	GPIO_PORTD_IEV_R 			&= ~0x01;    		// PD0-3 falling edge event
	GPIO_PORTD_ICR_R 			|= 0x01;      	// clear flag PD0-3
	GPIO_PORTD_IM_R 			|= 0x01;      	// arm interrupt on PD0-3
	NVIC_PRI0_R 					|= (NVIC_PRI0_R&0x1FFFFFF)|0x6000000; //priority 3
	NVIC_EN0_R 						|= 0x00000008;      // enable interrupt 3 in NVIC
	
}

void GPIOPortD_Handler(void){	
		
		if(GPIO_PORTD_RIS_R&0x01){
			Delay();
			UartFlag = 1;
			Recieve = 1;
			Light = CR;
			if(Choice == '2')
				UART_OutCharB('!');
			Choice = '0';
			}
		GPIO_PORTD_ICR_R |= 0x01;
}


void Delay(){
	unsigned long volatile time;
  time = 500000;  // 0.1sec
  while(time){
		time--;
  }
}
