
#include "Uart0.h"

 void initUART(){

		//init UART0
		SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		UARTDisable(UART0_BASE);
		UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
		(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
		UARTFIFOEnable(UART0_BASE);

GPIOPinConfigure(GPIO_PA0_U0RX ); 		
GPIOPinConfigure(GPIO_PA1_U0TX);	    
UARTEnable(UART0_BASE);
GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);


	}

	
	// send strings
void send_text_to_uart(char  *chr){
  
  while(* chr){
    UARTCharPut(UART0_BASE, *(chr++));
  }
  
}

// send single characters 
void send_char_to_uart(char  chr){
  
    UARTCharPut(UART0_BASE, chr);
  
}


//
// Read an integer number from the keyboard and retun to the calling program
//


unsigned int Read_From_Keyboard()
{
unsigned int Total;
unsigned char N;
Total = 0;
while(1)
{
	if (UARTCharsAvail(UART0_BASE))
	{
		N = UARTCharGetNonBlocking(UART0_BASE); //
		send_char_to_uart(N); //
		if(N == '\r') break; //
		N=N- '0'; //
		Total = 10*Total + N; 
	}
}
return Total; //
}




//
// Read time from the keyboard. The time is entered as hh:mm:ss
//
void Read_Time(char buf[])
{
unsigned char c, k = 0;
while(1)
{
		if (UARTCharsAvail(UART0_BASE)) 
		{
			c = UARTCharGetNonBlocking(UART0_BASE); //
			send_char_to_uart(c); //
			if(c == '\r') break; //
			buf[k] = c; //
			k++; //
		}
}
buf[k] = '\0'; //
}



