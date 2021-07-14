#include "driverlib/uart.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "tm4c123gh6pm.h"
#define GPIO_PA0_U0RX           0x00000001
#define GPIO_PA1_U0TX           0x00000401

void send_text_to_uart(char  *chr);
void send_char_to_uart(char  chr);
unsigned int Read_From_Keyboard();
void Read_Time(char buf[]);
void initUART();