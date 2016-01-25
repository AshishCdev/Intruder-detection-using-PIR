/*
 * Humen_intruder.c
 *
 * Created: 1/16/2016 7:11:57 PM
 *  Author: Ashish kushwaha
 *  Website: arm-tutorials.com
 *  E-mail: ashish_kushwaha@outlook.com
 */

#ifndef GSM_900_H_
#define GSM_900_H_

extern void gsm_ini(void);

extern char *gsm_res_btwn_c(char start,char terminate,char *target,char size); //this function takes the response between starting and terminating char
extern char *gsm_res_btwn_n(char after,char till,char *target,char size);//this function takes the response after after'th char and size of till char
extern void gsm_echo_dis(void);            //this function disables the echo from the gsm module
extern void gsm_sim_status(void);		   //for getting the name of sim
extern void gsm_sig_q(void);			   //measuring the quality of signal straingth
extern _Bool gsm_send_message(char *message); //send the message to the number saved in eeprom started by eeprom_star

#define eeprom_star 20


#endif /* GSM_900_H_ */