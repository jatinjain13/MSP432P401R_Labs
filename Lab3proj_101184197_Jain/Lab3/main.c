//Jatin Jain 101184197
#include "msp.h"



#define RED_LED 0
#define RGB_LED 1




static uint8_t selected_led;
	
int main(void) {
	
	//Watchdog timer disable
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	
	
	
//Configure P1.1 P1.4 as an inputs
	P1->SEL0 &= (uint8_t)(~((1<<1)|(1<<4))); //GPIO
	P1->SEL1 &= (uint8_t)(~((1<<1)|(1<<4)));
	P1->DIR &= (uint8_t)(~((1<<1)|(1<<4))); //Input
	P1->REN |= (uint8_t)((1<<1)|(1<<4)); //Pull up
	P1->OUT |= (uint8_t)((1<<1)|(1<<4));
	
	
		
	// Configure Red LED P1.0 as output
	P1->SEL0 &= (uint8_t)(~(1<<0)); //GPIO
	P1->SEL1 &= (uint8_t)(~(1<<0));
	P1->DIR |= (uint8_t)(1<<0); //Output
	P1->DS &= (uint8_t)(~(1<<0));  //Low
	P1->OUT &= (uint8_t)(~(1<<0)); //Drive pins low
	

	
	
	// Configure RGB LED P2.1 P2.0 and P2.2 as active low output
	P2->SEL0 &= (uint8_t)(~((1<<0)|(1<<1)|(1<<2)));
	P2->SEL1 &= (uint8_t)(~((1<<0)|(1<<1)|(1<<2)));
	P2->DIR |= (uint8_t)((1<<0)|(1<<1)|(1<<2));
	P2->DS &= (uint8_t)(~((1<<0)|(1<<1)|(1<<2)));
	P2->OUT &= (uint8_t)(~((1<<0)|(1<<1)|(1<<2)));
	

	// tracks which led is currently selected
	selected_led =RED_LED;
	
//Program Loop
	while(1) {
		static uint8_t switch1; //tracks switch being on or off
    static uint8_t switch2;

		if (!(P1->IN & (uint8_t)(1<<1)) && !switch1) { //Switch 1 is pressed
			uint16_t i = 1000;
			
			//Debounce
			while (i==0){
				i--;
			}
			
			switch1 = 1; // Switch 1 ON
			//Switch selected LED
			if (selected_led == RED_LED){
					selected_led = RGB_LED;
			}
			else if (selected_led == RGB_LED){
				selected_led = RED_LED;
			}	
				
				//Turn off switch 1 when not currently pressed
		} else if (P1->IN & (1<<1)) {
			switch1 = 0;
		}
		
	//Switch 2 is pressed
		if (!(P1->IN & (uint8_t)(1<<4)) && !switch2) { 
			uint16_t i = 1000;
			
			//Debouncing
			while (i==0){
				i--;
			}
			
			switch2 = 1; //switch 2 is pressed
			if (selected_led == RED_LED) {
				P1->OUT ^= (uint8_t)(1<<0); //Toggle red LED
			} else if (selected_led == RGB_LED) {
				uint8_t led_state = (P2->OUT); //LED state variable holds value in P2->OUT
				led_state++; //Increase it to move to next state
				P2->OUT &= ~(0x7); //Clear
				P2->OUT |= (led_state & 0x7); //If state is 1000 (state 8) clear
			}
			
		} else if (P1->IN & (1<<4)) { //turn off switch 2
				switch2 = 0;
			}
	}
	return 0;
}


