
#include <LPC17xx.h>

unsigned int  i =0 , n = 0;
unsigned int  j =0 ;
unsigned long x = 0x0;
int count  = 0;
unsigned int number  = 0;
unsigned char seven_segment[10] = {0x3F, 0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
void upcount();
void downcount();
int main() {
    	LPC_PINCON -> PINSEL0 = 0x0;
	LPC_PINCON -> PINSEL3 = 0x0;
	LPC_GPIO0 -> FIODIR = 0xFF << 4;
	LPC_GPIO1 -> FIODIR = 0xF << 23;
	
		
	while(1)
	{
		x= LPC_GPIO0 -> FIOPIN & 1 <<21;
		if (x)
		{
			upcount();
		}
		else
		{
			downcount();
		}
		n = number;
		for ( i = 0 ; i< 4; i++){
			LPC_GPIO1 -> FIOPIN =  i<<23;
			LPC_GPIO0 -> FIOPIN = seven_segment[n%10] << 4;
			n = n/10;
			for(j = 0 ; j<500; j++){};
		}
	
		
	}
}
void upcount(){
	count++;
	if (count == 2500) {
		count  = 0 ;
		number = (number+1)%10000;
	}
}
void downcount() {
    count++;
    if (count == 2500) {
        count = 0;
        if (number == 0) {
            number = 9999;
        } 
	else {
            number--;
        }
    }
}
