
/*
 * Humen_intruder.c
 *
 * Created: 1/16/2016 7:11:57 PM
 *  Author: Ashish kushwaha
 *  Website: arm-tutorials.com
 *  E-mail: ashish_kushwaha@outlook.com
 */
#include <avr/io.h>
#include "USART.h"
#include <stdlib.h>

void serial_init(){
		UCSRB=(1<<TXEN)|(1<<RXEN);		
		UCSRC=(1<<UCSZ1)|(1<<UCSZ0)|(1<<URSEL);
		UBRRL=71;                                  ///9600   baudrate at xtal 11.0592Mhz	
}
//This function sends the string and terminates with LF \n
void serial_send(char *p)
{
	serial_Erase_all();
	for (;*p!=0;p++)
	{
		while (!(UCSRA&(1<<UDRE)));
		UDR=*p;
	}
	while (!(UCSRA&(1<<UDRE)));
	UDR='\r';
}

void serial_send_byte(char byte){
	serial_Erase_all();
	while (!(UCSRA&(1<<UDRE)));
	UDR=byte;
}

void serial_Erase_all(void){
	unsigned char trash=0;
	while (UCSRA & (1<<RXC)){ 
	trash=UDR;
	}
}

unsigned char serial_read_byte(void)
{
	while (!(UCSRA&(1<<RXC)));
	return UDR;
}

int serial_read_int(void){
	int getval=0,ret=0,flag=0;
	while(getval!='\n'){
	getval=serial_read_byte();
	if (getval=='-')
	{
		flag=1;
	}
	if ((getval>='0')&&(getval<='9'))
	{
		ret=((ret*10)+(getval-48));
	}
	}
	if (flag==1)ret=(ret*(-1));
	return ret;	
}

void serial_snd_int(int intg){
	char frag[20];int i=0;
	for (;intg;i++)
	{
		frag[i]=intg%10;
		intg/=10;
	}
	i--;
	while(i>=0){
	serial_send_byte(frag[i]+48);
	i--;
	}
}

void serial_send_nf(char *p)
{
	serial_Erase_all();
	for (;*p!=0;p++)
	{
		while (!(UCSRA&(1<<UDRE)));
		UDR=*p;
	}
	while (!(UCSRA&(1<<UDRE)));
}

char *serial_read_str(char size,char *target){
	target=(char*)malloc(size*sizeof(char));
	if (target==0) return 0;
	else{
		int i=0;
		for(;(i<size&&(target[i-1]!='\r'));i++) target[i]=serial_read_byte();
		target[i-1]=0;
		serial_Erase_all();
		return target;
	}
}