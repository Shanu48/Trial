
#include <LPC17xx.h>

unsigned int  i =0 , n = 0;
unsigned int  j =0;
int count  = 0;
unsigned int number  = 0;
unsigned int digit_pins[4] = {23, 24, 25, 26}; 
unsigned char seven_segment[10] = {0x3F, 0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
int main() {
	LPC_PINCON -> PINSEL0 = 0x0;
	LPC_PINCON -> PINSEL3 = 0x0;
	LPC_GPIO0 -> FIODIR = 0xFF << 4;
	LPC_GPIO1 -> FIODIR = 0xF << 23;
		
		
	while(1){
		n = number;
		for (i = 0; i < 4; i++) {
			// Turn off all digits first
			LPC_GPIO1->FIOCLR = (0xF << 23);
        		// Set segments for the current digit
        		LPC_GPIO0->FIOCLR = 0xFF << 4;  // Clear segment pins
        		LPC_GPIO0->FIOSET = seven_segment[n % 10] << 4;
        
        		// Enable current digit
        		LPC_GPIO1->FIOSET = 1 << digit_pins[i];
        
        		n = n / 10;
        
        		// Small delay for multiplexing
        		for (j = 0; j < 5000; j++) {}
		}
		count++;
		if (count == 250) {
			count  = 0 ;
			number = (number+1)%10000;
			
		}
	}
}