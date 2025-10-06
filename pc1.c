
#include <lpc17xx.h>

#define LCD_RS (1 << 27) 
#define LCD_EN (1 << 28) 
#define LCD_DATA_MASK (0xF << 23) 

void port_write(void);
void lcd_write(void);
void delay(unsigned int ms);


unsigned long int init_comm[] = {0x3, 0x3, 0x3, 0x2, 0x28, 0x0C, 0x06, 0x01, 0x80};

 unsigned char current_byte;
const unsigned char msg[] = "HELLO LPC1768"; 

int flag1 = 0;
unsigned int i = 0;
unsigned char temp1;

int main() {
    LPC_GPIO0->FIODIR |= LCD_RS | LCD_EN | LCD_DATA_MASK;
    LPC_GPIO0->FIOCLR = LCD_RS | LCD_EN; 

    flag1 = 0; // Command mode
    for (i = 0; i < sizeof(init_comm) / sizeof(init_comm[0]); i++) {
        temp1 = (unsigned char)init_comm[i];
        lcd_write();
        delay(5); 
    }

    flag1 = 1; // Data mode (RS = 1)
    i = 0;
    while (msg[i] != '\0') {
        temp1 = msg[i];
        lcd_write();
        i++;
        delay(1); // Small delay between characters
    }

    while (1); // Infinite loop
}


void port_write(void) {
    LPC_GPIO0->FIOCLR = LCD_DATA_MASK;
    
    LPC_GPIO0->FIOSET = (temp1 & 0x0F) << 23;

    LPC_GPIO0->FIOSET = LCD_EN; 
    delay(1);                   
    LPC_GPIO0->FIOCLR = LCD_EN; 
    delay(1);                  
}


void lcd_write(void) {
    if (flag1 == 0) {
        LPC_GPIO0->FIOCLR = LCD_RS; 
    } else {
        LPC_GPIO0->FIOSET = LCD_RS; 
    }

    temp1 = (temp1 & 0xF0) >> 4; 
    port_write(); 
    
    
    
    current_byte = temp1; 

    temp1 = (current_byte >> 4); 
    port_write(); 
    temp1 = (current_byte & 0x0F); 
    port_write(); 

    delay(2);
}


void delay(unsigned int ms) {
    unsigned int count, j;
    for (count = 0; count < ms; count++) {
        for (j = 0; j < 6000; j++);
    }
}













