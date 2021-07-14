#include "LCD.h"
//****************************************************/   
/*  @Hardware Connentions
*
* [PB0 - PB7]  ->   						  I/O Data Bus.
*  PA5         ->   RS            Register Select.
*  PA6         ->   RW            Read / Write Register.
*  PA7         ->   E             Enable Pin. 
*  
****************************************************/
/*  @important Commands List
*
*    Command  *  Discription         
*  ***********************************
*     0x01    *  Clear display screen
*     0x02    *  Return to home position
*     0x04    *  Shift cursor to left
*     0x06    *  Shift cursor to right
*     0x05    *  Shift display right
*     0x07    *  Shift display left
*     0x08    *  Display off, Cursor off
*     0x0A    *  Display off, Cursor on
*     0x0C    *  Display on , Cursor off
*     0x0E    *  Display on , Cursor on 
*     0x0F    *  Display on , Cursor blinking
*     0x10    *  Shift cursor position to left
*     0x14    *  Shift cursor position to right
*     0x18    *  Shift entire display to left
*     0x1C    *  Shift entire display to right
*     0x20    *  Function set : 4-bit, 1 lines, 5x7 matrix
*     0x28    *  Function set : 4-bit, 2 lines, 5x7 matrix
*     0x30    *  Function set : 8-bit, 1 lines, 5x7 matrix
*     0x38    *  Function set : 8-bit, 2 lines, 5x7 matrix
*     0x80    *  Force cursor to start from 1st line
*     0xC0    *  Force cursor to start from 2nd line
*     0x90    *  Force cursor to start from 3rd line
*     0xD0    *  Force cursor to start from 4th line
*  
****************************************************/

void LCD_init(void)
{
	SYSCTL->RCGCGPIO |= 0x01; /* enable clock to GPIOA */
	SYSCTL->RCGCGPIO |= 0x02; /* enable clock to GPIOB */
	LCD_CTRL->DIR |= 0xE0; /* set PORTA pin 7-5 as output for control */
	LCD_CTRL->DEN |= 0xE0; /* set PORTA pin 7-5 as digital pins */
	LCD_DATA->DIR = 0xFF; /* set all PORTB pins as output for data */
	LCD_DATA->DEN = 0xFF; /* set all PORTB pins as digital pins */
	delayMs(20); /* initialization sequence */
	LCD_command(0x30);
	delayMs(5);
	LCD_command(0x30);
	delayUs(100);
	LCD_command(0x30);
	LCD_command(0x38); /* set 8-bit data, 2-line, 5x7 font */
	LCD_command(0x06); /* move cursor right */
	LCD_command(0x01); /* clear screen, move cursor to home */
	LCD_command(0x0F); /* turn on display, cursor blinking */
}
void LCD_command(unsigned char cmnd)
{
	LCD_CTRL->DATA = 0; /* RS = 0, R/W = 0 */
	LCD_DATA->DATA = cmnd;
	LCD_CTRL->DATA = EN; /* pulse E */
	delayUs(0);
	LCD_CTRL->DATA = 0;
	if (cmnd < 4)
	delayMs(2); /* command 1 and 2 needs up to 1.64ms */
	else
	delayUs(40); /* all others 40 us */
}
void lcd_data(unsigned char data)
{
	LCD_CTRL->DATA = RS; /* RS = 1, R/W = 0 */
	LCD_DATA->DATA = data;
	LCD_CTRL->DATA = EN | RS; /* pulse E */
	delayUs(0);
	LCD_CTRL->DATA = 0;
	delayUs(40);
}


void lcd_text_data(unsigned char *data)
{
		while(*data){
	LCD_CTRL->DATA = RS; /* RS = 1, R/W = 0 */
	LCD_DATA->DATA = *(data++);
	LCD_CTRL->DATA = EN | RS; /* pulse E */
	delayUs(0);
	LCD_CTRL->DATA = 0;
	delayMs(4);
	}
}



void LCD_setcursorRowCol(unsigned int row, unsigned int col)
{
  int command = 0x00;
  if( row == 0)
  {
    command = 0x80;
    while(col <=  0x0F && col >= 0x00)
    {
      LCD_command(command + col);
      break;
    }
  }
    if( row == 1)
  {
    command = 0xC0;
    while(col <=  0x0F && col >= 0x00)
    {
      LCD_command(command + col);
      break;
    }
  }

}

void delayMs(uint32_t n)
{
int i, j;
for(i = 0 ; i < n; i++)
for(j = 0; j < 3180; j++)
{} /* do nothing for 1 ms */
}

/* delay n microseconds (16 MHz CPU clock) */
void delayUs(uint32_t n)
{
int i, j;
for(i = 0 ; i < n; i++)
for(j = 0; j < 3; j++)
{} /* do nothing for 1 us */
}