#include <lpc17xx.h>

#define LCD_RS 0x08000000 
#define LCD_EN 0x10000000 
#define LCD_DATA_MASK 0x07800000

#define SWITCH_PIN (1 << 13)  // P2.13

unsigned long temp2 = 0;
unsigned char flag1 = 0; // 0 = command, 1 = data
unsigned char flag2 = 0; 
unsigned char temp1;

unsigned long init_comm[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80};

unsigned char msg[] = "HELLO LPC1768";
unsigned int i = 0;

void lcd_write(void);
void port_write(void);
void delay_lcd(unsigned long r1);
void lcd_display_message(void);

int main() {
    SystemInit();
    SystemCoreClockUpdate();

    // Set LCD pins (P0.23 to P0.26 and P0.27, P0.28 for RS and EN) as output
    LPC_GPIO0->FIODIR |= LCD_RS | LCD_EN | LCD_DATA_MASK;
    LPC_GPIO0->FIOCLR = LCD_RS | LCD_EN | LCD_DATA_MASK;

    // Configure P2.13 as input for switch
    LPC_GPIO2->FIODIR &= ~SWITCH_PIN;  // Clear direction bit for input
    LPC_PINCON->PINMODE4 &= ~(3 << 26); // Enable pull-up/pull-down resistors on P2.13 (pinmode bits 26-27)
    LPC_PINCON->PINMODE4 |= (2 << 26);  // Set pull-up resistor for P2.13 (10 = pull-up)

    while(1) {
        // Poll switch (active low, pressed = 0)
        if ((LPC_GPIO2->FIOPIN & SWITCH_PIN) == 0) {
            // Debounce delay
            delay_lcd(300000);
            if ((LPC_GPIO2->FIOPIN & SWITCH_PIN) == 0) { // Confirm pressed
                lcd_display_message();

                // Wait for switch release to avoid multiple triggers
                while ((LPC_GPIO2->FIOPIN & SWITCH_PIN) == 0);
                delay_lcd(300000);  // Debounce delay after release
            }
        }
    }
}

void lcd_display_message(void) {
    flag1 = 0; // command mode
    for (i = 0; i < sizeof(init_comm) / sizeof(init_comm[0]); i++) {
        temp1 = (unsigned char)init_comm[i];
        lcd_write();
        delay_lcd(50000);
    }

    flag1 = 1; // data mode
    i = 0;
    while (msg[i] != '\0') {
        temp1 = msg[i];
        lcd_write();
        i++;
        delay_lcd(50000);
    }
}

void lcd_write(void) {
    // flag2 decides if you send two nibbles or only one
    flag2 = (flag1 == 1) ? 0 : ((temp1 == 0x30) || (temp1 == 0x20)) ? 1 : 0;

    temp2 = temp1 & 0xF0;
    temp2 = temp2 << 19;  // shift high nibble to bits 23-26
    port_write();

    if (flag2 == 0) {
        temp2 = temp1 & 0x0F;
        temp2 = temp2 << 23; // shift low nibble to bits 23-26
        port_write();
    }
}

void port_write(void) {
    LPC_GPIO0->FIOPIN = temp2; // Set data lines

    if (flag1 == 0) 
        LPC_GPIO0->FIOCLR = LCD_RS;  // RS = 0 for command
    else
        LPC_GPIO0->FIOSET = LCD_RS;  // RS = 1 for data

    LPC_GPIO0->FIOSET = LCD_EN;   // EN = 1
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = LCD_EN;   // EN = 0
    delay_lcd(3000000);
}

void delay_lcd(unsigned long r1) {
    volatile unsigned long r;
    for (r = 0; r < r1; r++);
}



/*
// GPT modified correct code for simple LCD display
#include <lpc17xx.h>

#define LCD_RS 0x08000000 
#define LCD_EN 0x10000000 
#define LCD_DATA_MASK 0x07800000

unsigned long temp2 = 0;
unsigned char flag1 = 0; // 0 = command, 1 = data
unsigned char flag2 = 0; 
unsigned char temp1;

unsigned long init_comm[] = {0x30, 0x30, 0x30, 0x20, 0x28, 0x0C, 0x06, 0x01, 0x80};

unsigned char msg[] = "HELLO LPC1768";
unsigned int i = 0;

void lcd_write(void);
void port_write(void);
void delay_lcd(unsigned long r1);

int main() {
    SystemInit();
    SystemCoreClockUpdate();

    LPC_GPIO0->FIODIR |= LCD_RS | LCD_EN | LCD_DATA_MASK;
    LPC_GPIO0->FIOCLR = LCD_RS | LCD_EN | LCD_DATA_MASK;

    flag1 = 0; // command mode
    for (i = 0; i < sizeof(init_comm) / sizeof(init_comm[0]); i++) {
        temp1 = (unsigned char)init_comm[i];
        lcd_write();
        delay_lcd(50000);
    }

    flag1 = 1; // data mode
    i = 0;
    while (msg[i] != '\0') {
        temp1 = msg[i];
        lcd_write();
        i++;
        delay_lcd(50000);
    }

    while (1); // infinite loop
}

void lcd_write(void) {
    // flag2 decides if you send two nibbles or only one
    flag2 = (flag1 == 1) ? 0 : ((temp1 == 0x30) || (temp1 == 0x20)) ? 1 : 0;

    temp2 = temp1 & 0xF0;
    temp2 = temp2 << 19;  // shift high nibble to bits 23-26
    port_write();

    if (flag2 == 0) {
        temp2 = temp1 & 0x0F;
        temp2 = temp2 << 23; // shift low nibble to bits 23-26
        port_write();
    }
}

void port_write(void) {
    LPC_GPIO0->FIOPIN = temp2; // Set data lines

    if (flag1 == 0) 
        LPC_GPIO0->FIOCLR = LCD_RS;  // RS = 0 for command
    else
        LPC_GPIO0->FIOSET = LCD_RS;  // RS = 1 for data

    LPC_GPIO0->FIOSET = LCD_EN;   // EN = 1
    delay_lcd(25);
    LPC_GPIO0->FIOCLR = LCD_EN;   // EN = 0
    delay_lcd(3000000);
}

void delay_lcd(unsigned long r1) {
    volatile unsigned long r;
    for (r = 0; r < r1; r++);
}

*/
/*
//Class code for LCD display


#include <lpc17xx.h>

#define LCD_RS 0x08000000 
#define LCD_EN 0x10000000 
#define LCD_DATA_MASK 0x07800000

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
        for (j = 0; j < 6000000; j++);
    }
}

*/

/*
#include <lpc17xx.h>
#define RS_CTRL 0x08000000 //P0.27, 1<<27
#define EN_CTRL 0x10000000 //P0.28, 1<<28
#define DT_CTRL 0x07800000 //P0.23 to P0.26 data lines, F<<23
unsigned long int temp1=0, temp2=0,i,j ;
unsigned char flag1 =0, flag2 =0;
unsigned char msg[] = {"Hello"};
void lcd_write(void);
void port_write(void);
void delay_lcd(unsigned long);
unsigned long int init_command[] = {0x30,0x30,0x30,0x20,0x28,0x0c,0x06,0x01,0x80};
int main(void)
{
SystemInit();
SystemCoreClockUpdate();
LPC_GPIO0->FIODIR = DT_CTRL | RS_CTRL | EN_CTRL;
flag1 =0;
for (i=0; i<9;i++)
{
temp1 = init_command[i];
lcd_write();
}
flag1 =1; //DATA MODE
i =0;
while (msg[i] != '\0')
{
temp1 = msg[i];
lcd_write();
i+= 1;
}
while(1);
}
void lcd_write(void)
{
flag2 = (flag1 == 1) ? 0 :((temp1 == 0x30) || (temp1 == 0x20)) ? 1 : 0;
temp2 = temp1 & 0xf0;
temp2 = temp2 << 19;
port_write();
if (flag2==0)
{
temp2 = temp1 & 0x0f; //26-4+1
temp2 = temp2 << 23;
port_write();
}
}
void port_write(void)
{
LPC_GPIO0->FIOPIN = temp2;
if (flag1 == 0)
LPC_GPIO0->FIOCLR = RS_CTRL;
else
LPC_GPIO0->FIOSET = RS_CTRL;
LPC_GPIO0->FIOSET = EN_CTRL;
delay_lcd(25);
LPC_GPIO0->FIOCLR = EN_CTRL;
delay_lcd(3000000);
}
void delay_lcd(unsigned long r1)
{
unsigned long r;
for(r=0;r<r1;r++);
return;
}
*/











