/*
 * Scheduler.h
 *
 * Created: 2022-07-05 오전 9:41:10
 *  Author: Sumin.Hwang
 */ 

#ifndef SCHEDULER_H_

#include <i2c_simple_master.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <atmel_start.h>
#include <avr/io.h>
#include <util/delay.h>


#define SCHEDULER_H_

#define	UART0_BUFF_SIZE		128

char	uart0_rx_buff   [UART0_BUFF_SIZE];
char    SA_buff         [UART0_BUFF_SIZE];
char    W_buff          [UART0_BUFF_SIZE];
char    R_buff          [UART0_BUFF_SIZE];

char	uart0_tx_buff   [UART0_BUFF_SIZE];

int     uart0_rx_len;
int 	uart0_tx_len;

volatile int sq;

void THCV241_A_not_use_sublink_mainlink_1lane(void);

void THCV241A_for_sublink_set(void);

void UART_TX_STR();

#endif /* SCHEDULER_H_ */
