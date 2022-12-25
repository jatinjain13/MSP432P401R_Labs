//Jatinn Jain
// Include peripheral device's register definitions
#include <stdbool.h>
#include "msp.h"
#include "BSP.h"
#include "CortexM.h"
#include "msp432p401r.h"

static uint16_t x = 511;
static uint16_t y = 511;
static uint8_t select = 1;
static uint16_t duty = 0;
static uint8_t reset = 0;
static uint8_t blink = 0;
static int cycle = 500;
	
void config_interrupts(void);
void config_switches(void);
void config_leds(void);

/* 
Configure the switches (P1.1 and P1.4) as inputs, using pull-up internal resistors. 
*/
void config_switches(){	
	// P1.1 and P1.4
	P1->SEL0 &= (uint8_t)(~((1<<1)|(1<<4)));
	P1->SEL1 &= (uint8_t)(~((1<<1)|(1<<4)));

	// Set inputs
	P1->DIR &= (uint8_t)(~((1<<1)|(1<<4)));
	
	// pull resistor 
	P1->REN |= (uint8_t)((1<<1)|(1<<4));
	P1->OUT |= (uint8_t)((1<<1)|(1<<4));
}


void config_leds(){	

	// Set GPIO
	P1->SEL0 &= (uint8_t)(~(1<<0));
	P1->SEL1 &= (uint8_t)(~(1<<0));

	// Set outputs
	P1->DIR |= (uint8_t)(1<<0);
	
	// Set drive strength to default (
	P1->DS &= (uint8_t)(~(1<<0));
	
	// Set off
	P1->OUT &= (uint8_t)(~(1<<0));

    // Disable interrupts
	P1->IE &= (uint8_t)(~(1<<0)); 

  //RGB

	P2->SEL0 &= (uint8_t)(~((1<<0)|(1<<1)|(1<<2)));
	P2->SEL1 &= (uint8_t)(~((1<<0)|(1<<1)|(1<<2)));
	P2->DIR |= (uint8_t)((1<<0)|(1<<1)|(1<<2));
	P2->DS &= (uint8_t)(~((1<<0)|(1<<1)|(1<<2)));
	P2->OUT &= (uint8_t)(~((1<<0)|(1<<1)|(1<<2)));
}

int main() {

    // Disable the watchdog timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	// Config
	//config_switches();
	//config_leds();
  config_interrupts();
	
	
	TA1CTL &= (uint16_t)(~((1<<5)|(1<<4)));     // Stop the timer
	TA1CTL &= (uint16_t)(~(1<<0));              // Clear interrupt flag
	TA1CCR0 = (uint16_t)(655);                // Holds 20ms upper limit
	TA1CTL |= (uint16_t)((1<<1));               // Interrupt enable
	TA1CTL |= (uint16_t)((1<<4));               // Up count mode
	TA1CTL |= (uint16_t)((1<<8));  

	TA2CTL &= (uint16_t)(~((1<<5)|(1<<4)));     // Stop the timer
	TA2CTL &= (uint16_t)(~(1<<0));              // Clear interrupt flag
	TA2CCR0 = (uint16_t)(1638);                // Holds 50ms upper limit 
	TA2CTL |= (uint16_t)((1<<1));               // Interrupt enable
	TA2CTL |= (uint16_t)((1<<4));               // Up count mode
	TA2CTL |= (uint16_t)((1<<8));      
								
	NVIC_SetPriority(TA1_N_IRQn,1);
	NVIC_ClearPendingIRQ(TA1_N_IRQn);
	NVIC_EnableIRQ(TA1_N_IRQn);
	
	NVIC_SetPriority(TA2_N_IRQn,1);
	NVIC_ClearPendingIRQ(TA2_N_IRQn);
	NVIC_EnableIRQ(TA2_N_IRQn);
	
	//Globally enable interrupts in CPU
	__ASM("CPSIE I");
	
	//Joystick and buzzer config
	BSP_Joystick_Init();
	BSP_Buzzer_Init(duty);
	

	
	while(1) {
	__ASM("WFI");

	}
	
	return 0;
}

void config_interrupts(void) {
    // Configure interrupts on falling edge
	P1->IES |= (uint8_t)((1<<1)|(1<<4));
	// Clear interrupt flags
	P1->IFG &= (uint8_t)~((1<<1)|(1<<4));
	// Interrupt pin enabled
	P1->IE |= (uint8_t)((1<<1)|(1<<4));
}

//not needed I initially read the lab wrong
/*void PORT1_IRQHandler(void){
	static volatile uint16_t i = 1000;
	
	
 	if ((P1->IFG & (uint8_t)(1<<4)) != 0) {
		reset =1;
		P1->IFG &= (uint8_t)~(1<<4);
 }
 
}
*/

void TA1_N_IRQHandler(void){
	duty = 512;
	BSP_Joystick_Input(&x, &y, &select);
		//set of if else statements that change the duty cycle
		if (x<341){
			if (y<341){
				//Q6
				TA0CCR0 = 880;
			}
			else if (y<682){
				//Q4
					TA0CCR0 = 980;
			}
			
			else if (y<1023){
			//Q1
					TA0CCR0 = 1109;
			}
			
		}
		
		else if (x<682){
			if (y<341){
				//Q7
					TA0CCR0 = 1280;
			}
			else if (y<682){
				//Q0
			duty = 0;
			}
			
			else if (y<1023){
			//Q2
				TA0CCR0 = 1380;
				
			}
			
		}
		
		else if (x<1023){
			if (y<341){
				//Q8
					TA0CCR0 = 1480;
			}
			else if (y<682){
				//Q5
					TA0CCR0 = 1580;
			}
			
			else if (y<1023){
			//Q3
					TA0CCR0 = 1680;
			}
			
		}
		
		//set buzzer
		BSP_Buzzer_Set(duty);
	
	}

	void TA2_N_IRQHandler(void){
		BSP_Joystick_Input(&x, &y, &select);
		blink+=50; //increase time by 50ms)
		//set of if else statements that change the speed of led changing (cycle)
		if (x<341){
			if (y<341){
				//Q6
				cycle = 750;
			}
			else if (y<682){
				//Q4
					cycle = 500;
			}
			
			else if (y<1023){
			//Q1
					cycle = 250;
			}
			
		}
		
		else if (x<682){
			if (y<341){
				//Q7
			cycle = 750;
			}
			else if (y<682){
				//Q0
				cycle = 250;
			}
			
			else if (y<1023){
			//Q2
				cycle = 250;
				
			}
			
		}
		
		else if (x<1023){
			if (y<341){
				//Q8
				cycle = 750;

			}
			else if (y<682){
				//Q5
				cycle = 500;

			}
			
			else if (y<1023){
			//Q3
					cycle = 500;
			}
			
		}
	
		if (blink%cycle == 0){ //when time is hit toggle
			P2->OUT |= 1;
		}
		

	}
