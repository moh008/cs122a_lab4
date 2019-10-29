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


enum D0_State {D0_off,D0_on} D0_state;

void D0_Init(){
	D0_state = D0_off;
}

void D0_Tick(){
	//Actions
	switch(D0_state){
		case D0_off:
		PORTD = PORTD & 0xFE;
		break;
		
		case D0_on:
		PORTD = PORTD | 0x01;
		break;
		
		default:
		break;
	}
	//Transitions
	switch(D0_state){
		case D0_off:
		D0_state = D0_on;
		break;
		case D0_on:
		D0_state = D0_off;
		break;
		
		default:
		break;
	}
}

enum D2_State {D2_off,D2_on} D2_state;

void D2_Init(){
	D2_state = D2_off;
}

void D2_Tick(){
	//Actions
	static unsigned char cnt1 = 0x00;
	switch(D2_state){
		case D2_off:
		PORTD = PORTD & 0xFB;
		break;
		
		case D2_on:
		PORTD = PORTD | 0x04;
		break;
		
		default:
		break;
	}
	//Transitions
	switch(D2_state){
		case D2_off:
		if(cnt1 == 0x02)
		{
			D2_state = D2_on;
			cnt1 = 0x00;
		}
		else
		{
			cnt1++;
		}
		break;
		
		case D2_on:
		if(cnt1 == 0x02)
		{
			D2_state = D2_off;
			cnt1 = 0x00;
		}
		else
		{
			cnt1++;
		}
		break;
		
		default:
		break;
	}
}

enum D4_State {D4_off,D4_on} D4_state;

void D4_Init(){
	D4_state = D4_off;
}

void D4_Tick(){
	static unsigned char cnt2 = 0x00;
	//Actions
	switch(D4_state){
		case D4_off:
		PORTD = PORTD & 0xEF;
		break;
		
		case D4_on:
		PORTD = PORTD | 0x10;
		break;
		
		default:
		break;
	}
	//Transitions
	switch(D4_state){
		case D4_off:
		if(cnt2 == 0x05)
		{
			D4_state = D4_on;
			cnt2 = 0x00;
		}
		else
		{
			cnt2++;
		}
		break;
		
		case D4_on:
		if(cnt2 == 0x05)
		{
			D4_state = D4_off;
			cnt2 = 0x00;
		}
		else
		{
			cnt2++;
		}
		break;
		
		default:
		break;
	}
}


void LedSecTask() {
	D0_Init();
	D2_Init();
	D4_Init();
	
	for(;;)
	{
		D0_Tick();
		D2_Tick();
		D4_Tick();
		vTaskDelay(500);
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
