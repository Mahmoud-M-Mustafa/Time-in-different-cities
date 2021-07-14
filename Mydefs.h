#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "inc/hw_gpio.h"
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/interrupt.h"
#include "semphr.h"
#include "tm4c123gh6pm.h"
#include <stdio.h>
#include <string.h>





void Taskl(void *pvParameters);
void Task2(void *pvParameters);
void Task3(void *pvParameters);