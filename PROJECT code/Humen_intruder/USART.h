#ifndef USART_H
#define USART_H

/*
 * USART.h
 *
 * Created: 3/29/2015 2:28:33 AM
 *  Author: Ashish kushwaha
 * Email: Ashish_kushwaha@outlook.com
 */

//line terminator character is LF \n
/* Baud rate Table 

8 Mhz

Rate	UBRR(decimal&hex) error					
300		1666	0x0682		0.0
600		832		0x0340		0.0
1200	416		0x01A0		0.0
2400	207		0x00CF		0.2
4800	103		0x0067		0.2
9600	51		0x0033		0.2
14400	34		0x0022		0.8
19200	25		0x0019		0.2
28800	16		0x0010		2.1
38400	12		0x000C		0.2
57600	8		0x0008		3.7
76800	6		0x0006		7.5
115200	3		0x0003		7.8
230400	1		0x0001		7.8
250000	1		0x0001		0.0

11.0592 Mhz

300		2303	0x08FF		0.0
600		1151	0x047F		0.0
1200	575		0x023F		0.0
2400	287		0x011F		0.0
4800	143		0x008F		0.0
9600	71		0x0047		0.0
14400	47		0x002F		0.0
19200	35		0x0023		0.0
28800	23		0x0017		0.0
38400	17		0x0011		0.0
57600	11		0x000B		0.0
76800	8		0x0008		0.0
115200	5		0x0005		0.0
230400	2		0x0002		0.0
250000	2		0x0002		8.5
*/

extern void serial_init(void);		//change the value of UBBR for desire baud rate

extern void serial_send(char *p);

extern void serial_Erase_all(void);		//flushes the recived data during usart operation

extern unsigned char serial_read_byte(void);

extern unsigned int *serial_read_line(void);

extern int serial_read_int(void);

extern void serial_send_byte(char byte);

extern void serial_snd_int(int);

extern void serial_send_nf(char *p);

extern char *serial_read_str(char size,char *target);
#endif //USART_H