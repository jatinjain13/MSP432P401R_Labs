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

void config_interrupts(void);

int main() {

    // Disable the watchdog timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	// Config
	//config_switches();
	//config_leds();
  config_interrupts();
	
	
	TA1CTL &= (uint16_t)(~((1<<5)|(1<<4)));     // Stop timer
	TA1CTL &= (uint16_t)(~(1<<0));              // Clear flag
	TA1CCR0 = (uint16_t)(655);                // 20ms upper limit
	TA1CTL |= (uint16_t)((1<<1));               // Interrupt enable
	TA1CTL |= (uint16_t)((1<<4));               
	TA1CTL |= (uint16_t)((1<<8));               
								
	//NVIc for timer
	NVIC_SetPriority(TA1_N_IRQn,1);
	NVIC_ClearPendingIRQ(TA1_N_IRQn);
	NVIC_EnableIRQ(TA1_N_IRQn);
	
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
    // Configure on a falling edge
	P1->IES |= (uint8_t)((1<<1)|(1<<4));
	// Clear flags
	P1->IFG &= (uint8_t)~((1<<1)|(1<<4));
	// Interrupt pin enabled
	P1->IE |= (uint8_t)((1<<1)|(1<<4));
}

//using TA1 since TA0 is used by buzzer
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
