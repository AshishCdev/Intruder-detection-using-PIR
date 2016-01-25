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
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lcd.h"
#include "Matrix_keypad.h"
#include "GSM_900.h"
#include "USART.h"
#define std_by 0
#define time_out 1

char time_count=0;_Bool mode_p=0;
/* Timer 0 interrupt initialization for animation*/
void anim_on(void){ 
TCNT0=0;		//complete 255 value overflow
TCCR0|=(1<<2)|(1<3);  //starting timer 0 for analog sampling with 1024 prescaler
TIMSK|=(1<<TOIE0);// Enabling Timer interrupt 
sei();
}

/* Function for triggering Timer 1 interrupt for network refresh and timeout*/
void thread_on(_Bool mode){
	mode_p =mode;
		TCCR1A=0b00000000; 
	if(!mode)TCCR1B=0b00000100;
	else 	 TCCR1B=0b00000101;
		TIMSK|=(1<<TOIE1);
		sei();
}

void anim_done(void){
	TIMSK&=~(1<<TOIE0);
	lcd_gotoxy(15,1);
	lcd_put_cus(3);
}

void setup_ini(void){
	lcd_init(LCD_DISP_ON); //initializing the lcd module
	lcd_bck_on();			
	lcd_puts("   system\ninitializing");
	anim_on();
	DDRA=0b11000000;
	_delay_ms(1000);
	serial_init(); //initializing the UART module with 9600 baud at XTAL 11059200 
	gsm_ini();
	anim_done();
}

/*function for extracting int from string*/
int extract_int (char *str){
	int ret=0;char i=0;
	do{
		if ((str[i]>='0')&&(str[i]<='9')){
			ret=((ret*10)+(str[i]-48));
		}
		i++;
	}while(str[i]!='\0');
	return ret;
}
/*function for writing the phone no into the eeprom of the microcontroller*/
void eeprom_update(){
	lcd_clrscr();
	lcd_puts("Type new number\n");
	char *key_str1,*key_str2;
		key_str1=Key_get_str(15,key_str1);
		lcd_clrscr();
		lcd_puts("again\n");
		key_str2=Key_get_str(15,key_str2);
		if(!strcmp(key_str1,key_str2)){
			free(key_str2);
			eeprom_update_block (( const void *) key_str1, ( void *) eeprom_star, 10);
			eeprom_update_byte(( uint8_t *) (eeprom_star+10),'\0');
			lcd_clrscr();lcd_puts("number changed \nsuccessfully");
			free(key_str1);
		}
		else {lcd_clrscr();lcd_puts("numbers not same\nchange not saved");_delay_ms(1000);}
	return ;
}

/*main loop for idle time*/
void main_loop(){
	char s=0;
here1:gsm_sim_status();
	thread_on(std_by);
	lcd_gotoxy(0,1);lcd_puts("# For change num");_delay_ms(1000);lcd_bck_off();
	while(((PIN(KPAD_PORT)&0xf0)==0xf0)&&(!(PINA&0b00000001))); //wait for the sensor signal or keypad
	TIMSK&=~(1<<TOIE1);
	if ((PIN(KPAD_PORT)&0xf0)!=0xf0){ //if keypad is pressed then update the phone no in eeprom
	s=Key_get_char();
	lcd_bck_on();
	if (s=='#'){
		eeprom_update(); 
	  }
	}
	else{                         // send the message to that no. 
		lcd_clrscr();
		anim_on();
		lcd_bck_on();
		lcd_puts("sending massage");
		anim_done();
		gsm_send_message("Intruder detected in your room");
	}
	 _delay_ms(700);
	 goto here1;
}

int main(void){
	DDRA=0b11111110;
	DDR(KPAD_PORT)=0b00001111;
	setup_ini();
	gsm_sim_status();
	gsm_reg();
	lcd_clrscr();
	main_loop();
	return 0;
}

ISR(TIMER0_OVF_vect){
	time_count++;
	if ((time_count==8)||(time_count==16)||(time_count==24)){
		lcd_gotoxy(15,1);
		lcd_put_cus((time_count/8)-1);
	}
	if (time_count>24) time_count=0;
}

ISR(TIMER1_OVF_vect){
	if (mode_p)
	{
	lcd_clrscr();
	lcd_puts("   TIMEOUT");
	_delay_ms(700);
	lcd_clrscr();
	main_loop();
	}
	else {
		gsm_sig_q();
	}
}
