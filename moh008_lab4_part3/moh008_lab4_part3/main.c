/* Partner : Stephanie scabr006@ucr.edu
 * Lab Section : 021
 * Assignment: Lab4 Exercise 3
 * Created: 2019-10-18 ¿ÀÈÄ 3:32:18
 * Author : Lincoln
 *
 * I acknowledge all content contained herein, excluding template
 * or example code, is my own original work.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/portpins.h>
#include <avr/pgmspace.h>

//FreeRTOS include files
#include "FreeRTOS.h"
#include "task.h"
#include "croutine.h"

enum LED_States {LED_Init,next_led, prev_led} led_state;

void Led_Init(){
	led_state = next_led;
	PORTD = 0x01;
}

void LED_Tick(){
	//Actions
	switch(led_state){
		
		case next_led:
		PORTD = PORTD << 1;
		break;
		
		case prev_led:
		PORTD = PORTD >> 1;
		break;
		
		default:
		break;
	}
	//Transitions
	switch(led_state){
		case next_led:
		if(PORTD == 0x80)
		led_state = prev_led;
		else
		led_state = next_led;
		break;
		
		case prev_led:
		if(PORTD == 0x01)
		led_state = next_led;
		else
		led_state = prev_led;
		break;
		
		default:
		break;
	}
}
void LedSecTask() {
	Led_Init();
	
	for(;;)
	{
		LED_Tick();
		vTaskDelay(100);
	}
}

void StartSecPulse(unsigned portBASE_TYPE Priority) {
	xTaskCreate(LedSecTask,
	(signed portCHAR *)"LedSecTask",
	configMINIMAL_STACK_SIZE,
	NULL,
	Priority,
	NULL );
}

int main(void) {
	DDRA = 0x00; PORTA=0xFF;
	DDRD = 0xFF;
	
	//Start Tasks
	StartSecPulse(1);
	//RunSchedular
	vTaskStartScheduler();
	return 0;
}
