#include "Mydefs.h"
//  Ain-Shams University   Faculty of Engineering
// CSE 345  Real-Time and Embedded Systems Design - Term Project
//****************************************************/   
/*  @Team
*
* Mahmoud Mohammed Mustafa				17p8110
* Mohamed Akram										17p8070
* Mohamed Ashraf									17p6009
* Ahmed Wael											17p6000 
*  
****************************************************/
// Cities and their time differences from London

char cities[][10] = {"London", "Paris", "Madrid", "Rome", "Athens", "Ankara", "Istanbul",
"Cairo", "Moscow", "Tehran"};

char timediff[] = {0, 1, 1, 1, 2, 2, 2, 2, 3, 4}; // time diffrences compared to london time GMT (Greenwich Mean Time)

unsigned int selection;
QueueHandle_t xQueuel; // Queuel handle
QueueHandle_t xQueue2; // Queue2 handle


int main()
{

	LCD_init(); // Initialize LCD
	initUART(); // Initialize Uart0 

xQueuel = xQueueCreate(1, 8); // Create queue
xQueue2 = xQueueCreate(1, 8); // Create queue

 

// Create Task 1

xTaskCreate((TaskFunction_t)Taskl,"Time Controller",configMINIMAL_STACK_SIZE,NULL,10,NULL);

// Create Task 2

xTaskCreate((TaskFunction_t)Task2,"LCD Controller",configMINIMAL_STACK_SIZE,NULL,10,NULL);

// Create Task 3

xTaskCreate((TaskFunction_t)Task3,"UART Controller",configMINIMAL_STACK_SIZE,NULL,10,NULL);
//
// Start the RTOS scheduler
//
vTaskStartScheduler();
//
// Will never reach here
}



// Task 1 - Time Controller
void Taskl(void *pvParameters)

{
		typedef struct Message
		{
	unsigned char hours;
	unsigned char minutes;
	unsigned char seconds;
	} AMessage ;
	AMessage Tim;
	
xQueueReceive(xQueue2, &Tim, portMAX_DELAY); // Receive initial time

while (1)
{

vTaskDelay(pdMS_TO_TICKS(500)); // Wait for 1 second --> 500 as there is a delay because of the LCD response
Tim.seconds++; // Increment seconds

 
// Adjust time period  to elapse minutes and hours
if(Tim.seconds == 60) 
{
	Tim.seconds = 0; 
	Tim.minutes++; 
	if(Tim.minutes == 60) 
	{
		Tim.minutes = 0;
		Tim.hours++; 
		if(Tim.hours == 24) 
			Tim.hours = 0; 
	}
}
xQueueSend(xQueuel, &Tim, 0); // send to task 2 
}
}

 
 



// Task 2 - LCD Controller
void Task2(void *pvParameters)

{
char Txt[7];
	
	typedef struct Message
	{
		unsigned char hours;

		unsigned char minutes;

		unsigned char seconds;
	} AMessage;

AMessage Tim;

LCD_command(1);  // Clear LCD
selection = 0; // Clear selection

while(1)

{
	xQueueReceive(xQueuel, &Tim, portMAX_DELAY); // Get time  recieve from task 3 Uart
	
  LCD_command(0x01); /* clear screen, move cursor to home */
	LCD_command(0x0F); /* turn on display, cursor blinking */
	LCD_setcursorRowCol(0,0);
	lcd_text_data(cities[selection]);
	LCD_setcursorRowCol(1,0);
	
	Tim.hours = Tim.hours + timediff[selection]; // Hour adjustment
	if(Tim.hours > 23)Tim.hours = Tim.hours - 24; // Tf > 24


	if(Tim.hours < 10) // If < 10
	{

		Txt[1] = (Tim.hours%10)+'0' ;

		Txt[0] = '0';

		Txt[2] = '\0'; // NULL terminator
	}
	else
	{
		
			Txt[0] = (Tim.hours/10)+'0';
			Txt[1] = (Tim.hours%10)+'0' ; 

		Txt[2] = '\0'; // NULL terminator
	}
			lcd_text_data(Txt); // Display hours  

	lcd_data(':'); // Colon                  

	if(Tim.minutes  < 10) // T£ < 10
	{
	Txt[1] = (Tim.minutes%10)+'0' ; // Insert leading 0

	Txt[0] = '0';

	Txt[2] = '\0'; // NULL terminator
	}
		else
	{
		
			Txt[0] = (Tim.minutes/10)+'0';
			Txt[1] = (Tim.minutes%10)+'0' ; 

		Txt[2] = '\0'; // NULL terminator
	}
 lcd_text_data(Txt);// Display minutes


lcd_data(':'); // Colon

if(Tim.seconds < 10) // If < 10
{
	Txt[1] = (Tim.seconds%10)+'0' ; // Insert leading 0

	Txt[0] = '0';

	Txt[2] = '\0'; // NULL terminator

}

		else
	{
		
			Txt[0] = (Tim.seconds/10)+'0';
			Txt[1] = (Tim.seconds%10)+'0' ; 

		Txt[2] = '\0'; // NULL terminator
	}
lcd_text_data(Txt); // Display seconds

}

}
// Task 3 - UART Controller
void Task3(void *pvParameters)

{

	char k, Buffer[10];
	typedef struct Message
	{
	unsigned char hh;
	unsigned char mm;
	unsigned char ss;
	} AMessage;

	AMessage Tim;



	send_text_to_uart("\n\rTime in Different Countries");
	send_text_to_uart("\n\r=======================" );
	send_text_to_uart("\n\rEnter the time in London (hh:mm:ss): ");
		
	Read_Time(Buffer);
		
	Tim.hh = 10*(Buffer[0] - '0') + Buffer[1] - '0'; // Convert to nmbr
	Tim.mm = 10*(Buffer[3] - '0') + Buffer[4] - '0'; // Convert to nmbr
	Tim.ss = 10*(Buffer[6] - '0') + Buffer[7] - '0'; // Convert to nmbr
	
	xQueueSend(xQueue2, &Tim, 0); // Send to Task 1
while (1)
{

send_text_to_uart("\n\r\n\rSelect a City:"); // Heading

	for(k = 0; k < 10; k++) // Display cities

	{

		send_text_to_uart("\n\r"); // New line


		send_char_to_uart((k+'0')); // print numbring
		send_text_to_uart(". ");

		send_text_to_uart(cities[k]); // City names
	}
	send_text_to_uart("\n\rSelection: "); // Selection prompt
	selection = Read_From_Keyboard(); // Read selection

 }
}


void vApplicationStackOverflowHook(xTaskHandle *pxTask, char *pcTaskName)
{
    //
    // This function can not return, so loop forever.  Interrupts are disabled
    // on entry to this function, so no processor interrupts will interrupt
    // this loop.
    //
    while (1)
    {
    }
}

