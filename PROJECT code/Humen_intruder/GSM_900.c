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
#include "GSM_900.h"
#include "lcd.h"
#include "USART.h"
#include <string.h>
#include <stdlib.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include "lcd.h"


void WDT_on (void){
	WDTCR |= 0b00001111; //setting watch dog timer for 2.2 sec
}

void WDT_off(void)
{
	/* Write logical one to WDTOE and WDE */
	WDTCR |= (1<<WDTOE) | (1<<WDE);
	/* Turn off WDT */
	WDTCR = 0x00;
}

void gsm_ini(void){
		serial_send("AT");
		char *f;WDT_on();
		f=gsm_res_btwn_c('\n','\r',f,10);
		if (!strcmp(f,"OK")){
			WDT_off();
			lcd_clrscr();free(f);lcd_puts("DONE");
			_delay_ms(500);
			lcd_clrscr();
			gsm_echo_dis();
			}
		else while(1);
}
void gsm_reg(void){
	char *k;
	anim_on();
	lcd_clrscr();
	lcd_puts("Registering to\nthe network");
	here:serial_send("AT+CREG?");
	k=gsm_res_btwn_n(9,3,k,10);
	if (!strcmp(k,"0,1")){
		lcd_clrscr();
		lcd_puts("Registered");
		anim_done();
		free(k);
		_delay_ms(500);
	}
	else {
		_delay_ms(1000);
		goto here;
	}
	
}
void gsm_echo_dis(void){
	char *o;
	serial_send("ATE0");
	o=gsm_res_btwn_c('\n','\r',o,10);
	WDT_on();
	if(!strcmp(o,"OK")){WDT_off();lcd_puts("Echo disabled");_delay_ms(700);free(o);}
	else while (1);
}
void gsm_sim_status(void){
	char *p;
	serial_send("AT+CSPN?");
	p=gsm_res_btwn_n(2,5,p,10);
	if (!strcmp(p,"ERROR")){
		lcd_clrscr();
		lcd_puts("Insert SIM card \nand restart");while(1);
	}
	else {p=gsm_res_btwn_c(34,34,p,15);lcd_clrscr();lcd_puts(p);_delay_ms(700);free(p);}
}
char *gsm_res_btwn_c(char start,char terminate,char *target,char size){
	char dump;
	do{
		dump=serial_read_byte();
	} while(dump!=start);
	target=(char*)malloc(size*sizeof(char));
	if (target==0) return 0;
	else{
		int i=0;
		for(;(i<size&&(target[i-1]!=terminate));i++) target[i]=serial_read_byte();
		target[i-1]=0;
		serial_Erase_all();
		return target;
	}
}
char *gsm_res_btwn_n(char after,char till,char *target,char size){
	char dump;
	for (;after;--after)
	{
		dump=serial_read_byte();
	}
	target=(char*)malloc(size*sizeof(char));
	if (target==0) return 0;
	char i;
	for(i=0;i<till;i++){
		target[i]=serial_read_byte();
	}
	target[i]=0;
	serial_Erase_all();
	return target;
}

void gsm_sig_q(void){
	char *h,inte=0;
	serial_send("AT+CSQ");
	h=gsm_res_btwn_n(8,2,h,20);
	inte=extract_int(h);
	lcd_gotoxy(15,0);
	if ((inte>=0)&&(inte<99))lcd_put_cus((inte/6)+4);
	else lcd_putc('X');
	free(h);
}
_Bool gsm_send_message(char *message){
	char *j,*k;
	serial_send("AT+CMGF=1");
	j=gsm_res_btwn_c('\n','\r',j,10);
	if (!strcmp(j,"OK")){
		free(j);
		serial_send_nf("AT+CMGS=");
		serial_send_byte(34);
		serial_send_nf("+91");
		uint8_t numb[10];
		eeprom_read_block (( void *) numb , ( const void *) eeprom_star, 11) ;
		serial_send_nf(numb);
		serial_send_byte(34);
		serial_send_byte('\r');
		_delay_ms(1000);
		serial_send_nf(message);
		UDR=26;
		k=gsm_res_btwn_n(2,5,k,10);
		if (!strcmp(k,"ERROR")){
			lcd_clrscr();
			lcd_puts("Message sending\nfailed");
			free(k);
			return 0;
		}
		else {free(k); lcd_puts("massage sent\nsuccessfully"); return 1;}
	}
	else{free(j);return 0;}
}