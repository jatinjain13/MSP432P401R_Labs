//Jatin Jain 101184197

#include "msp.h"

// Define constants
#define DELAY_VALUE 5000
#define RED_LED 0
#define RGB_LED 1

//which led is currently selected
static uint8_t selected_led;

int main(void) {
	
	selected_led = RED_LED;
	
	// Stop watchdog timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

		
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
	
	//NVIC configuration
	NVIC_SetPriority(PORT1_IRQn,2);
	NVIC_ClearPendingIRQ(PORT1_IRQn);
	NVIC_EnableIRQ(PORT1_IRQn);

	//Globally enable interrupts 
	__ASM("CPSIE I");
	
	// Create infinite loop
	while(1) {
		__ASM("WFI");
	}
}

void PORT1_IRQHandler(void){
	
	// DEBOUNCE
	static uint16_t i = DELAY_VALUE;

		while (i--) {
				
			
		}
  	
	//SWITCH 1 PRESSED
	if((P1->IFG & (uint8_t)(1<<1))){
		// Clearing the interrupt flag.
		P1->IFG &= (uint8_t)~(1<<1);
		
		
		selected_led = (selected_led + 1) % 2;
	} else if((P1->IFG & (uint8_t)(1<<4))){
		// Clear interrupt flag.
		P1->IFG &= (uint8_t)~(1<<4);
		
		 // If selected is red
		if (selected_led == RED_LED){
      // toggle
			P1->OUT ^= (uint8_t)((1<<0));
		} else {
      // otherwise for RGB clear port then increment counter to next state and increase
			uint8_t count = (P2->OUT & 0x07);
			count++;
			P2->OUT &= ~(0x07);
			P2->OUT |= count & 0x07;
		}
	}
}
