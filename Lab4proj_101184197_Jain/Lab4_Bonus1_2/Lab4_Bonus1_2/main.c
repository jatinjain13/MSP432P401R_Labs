//Jatin Jain 101184197
//BONUS 1 and 2

#include "msp.h"

// Define constants
#define DELAY_VALUE 5000
#define RED_LED 0
#define RGB_LED 1

//which led is currently selected
static uint8_t selected_led;
static int counter;
static int holder;


void config_LED_BUTTON(void){
	//Configure switches active low
	P1->SEL0 &= (uint8_t)(~((1<<1)|(1<<4)));
	P1->SEL1 &= (uint8_t)(~((1<<1)|(1<<4)));
	P1->DIR &= (uint8_t)(~((1<<1)|(1<<4)));
	P1->REN |= (uint8_t)((1<<1)|(1<<4));
	P1->OUT |= (uint8_t)((1<<1)|(1<<4));
	
	
		
	// Configure red led
	P1->SEL0 &= (uint8_t)(~(1<<0));
	P1->SEL1 &= (uint8_t)(~(1<<0));
	P1->DIR |= (uint8_t)(1<<0);
	P1->DS &= (uint8_t)(~(1<<0));
	P1->OUT &= (uint8_t)(~(1<<0));
	

	
	
	// Configure RGB LED 
	P2->SEL0 &= (uint8_t)(~((1<<0)|(1<<1)|(1<<2)));
	P2->SEL1 &= (uint8_t)(~((1<<0)|(1<<1)|(1<<2)));
	P2->DIR |= (uint8_t)((1<<0)|(1<<1)|(1<<2));
	P2->DS &= (uint8_t)(~((1<<0)|(1<<1)|(1<<2)));
	P2->OUT &= (uint8_t)(~((1<<0)|(1<<1)|(1<<2)));
	
  // Configure interrupts edge
	P1->IES |= (uint8_t)((1<<1)|(1<<4));
	
	// Clear flags
	P1->IFG &= (uint8_t)~((1<<1)|(1<<4));
	
	// enable interrupt
	P1->IE |= (uint8_t)((1<<1)|(1<<4));


}

void PORT1_IRQHandler(void){
	// DEBOUNCE
	static uint16_t i = DELAY_VALUE;

		while (i--) {
			
			__ASM volatile(""); //used to pause 		
			
		}
		
		
	if((P1->IFG & (uint8_t)(1<<1)) && (P1->IFG & (uint8_t)(1<<4))){
		// Clearing the interrupt flag
		counter =0; //reset counter
		P1->IFG &= (uint8_t)~(1<<1);
		P1->IFG &= (uint8_t)~(1<<4);
		
	}
	
	//SWITCH 1 PRESSED
	else if((P1->IFG & (uint8_t)(1<<1))){
		// Clearing the interrupt flag.
		P1->IFG &= (uint8_t)~(1<<1);
		//increment counter
		counter++;


	} else if((P1->IFG & (uint8_t)(1<<4))){
		// Clear flag
		P1->IFG &= (uint8_t)~(1<<4);
		//Store contents of p2 out in holder
		
		while (counter--){ //toggle red led number of times counter is pressed
			while (i--) {			
			}
			P1->OUT ^= (uint8_t)(1<<0);
			static uint16_t i = DELAY_VALUE;
			while (i--) {			
			}
			
			P1->OUT ^= (uint8_t)(1<<0);
			i = DELAY_VALUE;
			while (i--) {			
			}
		}
		
/* Not needed 
		holder = (P2->OUT & 0x07);
		//make counter == holder
		counter += holder;
		//clear first 3 bits
		P2->OUT &= ~(0x07);
		//increment by counter and wrap around if first 3 bits are high
		P2->OUT |= counter & 0x07;
		*/
		counter = 0;
		
	
	}
}


int main(void) {
	
	selected_led = RED_LED;
	
	// Stop watchdog timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	config_LED_BUTTON();
	
	//NVIC configuration
	NVIC_SetPriority(PORT1_IRQn,2);
	NVIC_ClearPendingIRQ(PORT1_IRQn); //INSTEAD OF MANUAL CLEARING
	NVIC_EnableIRQ(PORT1_IRQn);

	//Globally enable interrupts 
	__ASM("CPSIE I");

	// Create infinite loop
	while(1) {
		__ASM("WFI");
	}
}
