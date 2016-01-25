/*
 * Humen_intruder.c
 *
 * Created: 1/16/2016 7:11:57 PM
 *  Author: Ashish kushwaha
 *  Website: arm-tutorials.com
 * s E-mail: ashish_kushwaha@outlook.com
 */

#ifndef MATRIX_KEYPAD_H_
#define MATRIX_KEYPAD_H_

#define KPAD_PORT PORTB
#define DDR(x) (*(&x - 1))      /* address of data direction register of port x */
#define PIN(x) (*(&x - 2))    /* address of input register of port x          */

#define EN			1
#define DIS			0

extern char Key_get_char(void); 

extern char *Key_get_str(char size, char *target);

#endif /* MATRIX_KEYPAD_H_ */