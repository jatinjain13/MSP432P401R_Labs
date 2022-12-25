// Jatin Jain - 101184197
//Bonus 2
// Include peripheral device's register definitions
#include "msp.h"

// Define constants

#define RGB_LED_MASK 0x07

#define RED 0 


// Function prototypes
void config_switches(void);
void config_leds(void);
void config_interrupts(void);

// Variable definitions
static volatile uint8_t currentLED;
static volatile uint8_t paused;

static uint8_t count = 2;
static uint8_t cycle = 0;
static uint8_t reset = 0;

int main() {

	
    // Disable the watchdog timer
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

	// Config
	config_switches();
	config_leds();
  config_interrupts();
	
	TA0CTL &= (uint16_t)(~((1<<5)|(1<<4)));     // Stop the timer
	TA0CTL &= (uint16_t)(~(1<<0));              // Clear interrupt flag
	TA0CCR0 = (uint16_t)(32768);                // Holds the upper limit value
	TA0CTL |= (uint16_t)((1<<1));               // Interrupt enable
	TA0CTL |= (uint16_t)((1<<4));               // Up count mode
	TA0CTL |= (uint16_t)((1<<8));               


								
	// NVIC 
	NVIC_SetPriority(PORT1_IRQn,2);
	NVIC_ClearPendingIRQ(PORT1_IRQn);
	NVIC_EnableIRQ(PORT1_IRQn);
	
	NVIC_SetPriority(TA0_N_IRQn,3);
	NVIC_ClearPendingIRQ(TA0_N_IRQn);
	NVIC_EnableIRQ(TA0_N_IRQn);

	
	//Globally enable interrupts in CPU
	__ASM("CPSIE I");
	
	// Wait for interrupts
	while(1) {
		__ASM("WFI");
	}
	
	return 0;
}

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


void config_interrupts(void) {
    // Configure interrupts on falling edge
	P1->IES |= (uint8_t)((1<<1)|(1<<4));
	// Clear interrupt flags
	P1->IFG &= (uint8_t)~((1<<1)|(1<<4));
	// Interrupt pin enabled
	P1->IE |= (uint8_t)((1<<1)|(1<<4));
}


void PORT1_IRQHandler(void){
	static volatile uint16_t i = 1000;
	//Same as Bonus 1 (See Bonus 1 for comments)
	if ((P1->IFG & (uint8_t)(1<<1)) != 0) {
		if (cycle<=4 && cycle>=0){
		 cycle = 3 ;
		}
		//Clear
		P1->IFG &= (uint8_t)~(1<<1);
 }
	
	//Reset mode is on when switch 2 is presses
 	if ((P1->IFG & (uint8_t)(1<<4)) != 0) {
		reset =1;
		P1->IFG &= (uint8_t)~(1<<4);
 }
 
 
}

void TA0_N_IRQHandler(void){
	
	// Clear 
	TA0CTL &= (uint16_t) (~(1<<0));
	
	if (reset ==0){
		//increase cycle
		cycle++;
		
		//reset
		if (cycle == 9){
			cycle=0;
		}
		
		// change cycle 
		if (cycle<=4 && cycle>=0){
			count = 2;
		}
		else if (cycle <= 5 && cycle>=5){
				count = 3;
		} 
		else if (cycle <= 8 && cycle>=6){
				count = 1;
		} 
		

		P2->OUT &= ~(RGB_LED_MASK);
		P2->OUT |= count;
	}
	
	//When reset mode is on only cycle between off and red
	if (reset > 0 ){
			reset++;
			P2->OUT &= ~(RGB_LED_MASK);
			if (reset%2 == 1){
				P2->OUT |= 1;
			}
		}
}


