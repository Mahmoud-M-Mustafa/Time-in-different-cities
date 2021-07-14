#include "TM4C123GH6PM.h"
#include <stdint.h>
#define LCD_DATA GPIOB   // port B for data
#define LCD_CTRL GPIOA	//  port A  for control
#define RS 0x20 /* PORTA BIT5 mask */
#define RW 0x40 /* PORTA BIT6 mask */
#define EN 0x80 /* PORTA BIT7 mask */


void LCD_command(unsigned char cmnd);
void delayMs(uint32_t n);
void delayUs(uint32_t n);
void LCD_setcursorRowCol(unsigned int row, unsigned int col);