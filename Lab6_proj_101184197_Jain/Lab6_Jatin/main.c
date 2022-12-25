//Jatinn Jain
// Include peripheral device's register definitions
#include <stdbool.h>
#include "msp.h"
#include "BSP.h"
#include "CortexM.h"
#include "msp432p401r.h"



int main() {

	uint16_t duty =0;  //duty cycle
    // Disable the watchdog timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	// Config not needed
	//config_switches();
	//config_leds();
  //config_interrupts();
	
	//Joystick and buzzer config
	BSP_Joystick_Init();
	BSP_Buzzer_Init(duty);
	
	//Globally enable interrupts in CPU
	__ASM("CPSIE I");
	
		uint16_t x = 511; // x,y pos of joystick
		uint16_t y = 511;
		uint8_t select = 0; //if pressed
	
	while(1) {
		//take input from joystick
		BSP_Joystick_Input(&x, &y, &select);
		duty = 512; 
		
		//set of if else statements that change the frequency through the timer TA0CCR0
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
	
	return 0;
}
