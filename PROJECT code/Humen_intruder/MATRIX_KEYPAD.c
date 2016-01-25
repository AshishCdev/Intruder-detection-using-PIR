/*
 * Humen_intruder.c
 *
 * Created: 1/16/2016 7:11:57 PM
 *  Author: Ashish kushwaha
 *  Website: arm-tutorials.com
 *  E-mail: ashish_kushwaha@outlook.com
 */
#define F_CPU 11059200
#include <avr/io.h>
#include "Matrix_keypad.h"
#include "lcd.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

char keys [4][4] = {	{'1','2','3','A'},
						{'4','5','6','B'},
						{'7','8','9','C'},
						{'*','0','#','D'}	
					};

char Key_get_char(void){
	DDR(KPAD_PORT)=0b00001111;				//setting direction for output and input
	while((PIN(KPAD_PORT)&0xf0)==0xf0);		//wait until key is pressed
	_delay_ms(30);
	for (char i=0;i<4;i++){					//scanning for rows
		KPAD_PORT=~(1<<i);
		_delay_ms(10);
		for (char j=4;j<8;j++){				//now for columns 
			if (!(PIN(KPAD_PORT)&(1<<j))){	//checking for column key
				while((PIN(KPAD_PORT)&0xf0)!=0xf0);//waiting until key is released
				_delay_ms(10);
				KPAD_PORT=0;PIN(KPAD_PORT)=0xff;//initiating port and pin register for next scan
				return keys[i][j-4];			//returning values from array
			}
		}
	}
}

/* This function makes the string */

char *Key_get_str(char size, char *target){
	target=(char*)malloc(size*sizeof(char));
	if (target==0) return 0;
	else{
	int i=0;
	for(;(i<size&&(target[i-1]!='#'));i++) {
		target[i]=Key_get_char();
		if (target[i]=='*'){
			lcd_bck_spc();
			i-=2;
		}
		else if(target[i]!='#') lcd_putc(target[i]);
		}
	target[i-1]=0;
	return target;
	}
}

