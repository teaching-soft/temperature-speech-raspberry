/***************************************************************************
                          rs232.h for "la bussola parlante"
                             -------------------
    begin                : mer apr 21 10:34:57 CET 2011
    copyright            : (C) 2011 by Giancarlo Martini and friends
    email                : gm@giancarlomartini.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 

#ifndef _RS232_H_
#define _RS232_H_

#include <termios.h>


/* Ritorna il descrittore di file, se questi == -1 l'apertura della porta   *
 * e' fallita.                                                              *
 * Parametri:
 * ttysPort	Porta da aprire tipo /dev/ttyS0
 * baud		B0, stacca la linea
 *		B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800 *
 *		B2400, B4800, B9600, B19200, B38400, B57600, B115200,      *
 *		B230400, B460800, B500000, B576000, B921600, B1000000,     *
 *		B1152000, B1500000, B2000000, B2500000, B3000000, B3500000 *
 *		B4000000						   *
 * dataBits	CS5, CS6, CS7, CS8
 * parity	PNONE, PEVEN, PODD
 * stopBits	SB1, SB2
 * ************************************************************************/
#define PNONE	0
#define PEVEN	1
#define PODD	2

#define SB1	0
#define SB2	1

int rs232_open(const char *ttysPort,unsigned int baud, unsigned int data_bits, unsigned int parity, unsigned int stop_bits);

/* Controlla quanti bytes sono in attesa di essere letti, attende timeOut  *
 * se non ce ne sono e poi ritorna 0. timeOut deve essere in secondi       *
 * ************************************************************************/
int rs232_bytes_arrived(int ttys_descriptor, int time_out);

/* Controlla quanti bytes sono in attesa di essere letti, senza timeout */
int rs232_buffer_in_lenght(int ttys_descriptor);

/* Legge i bytes arrivati, per un massimo di maxBytes, buffer deve essere  *
 * gi� allocato da chi chiama la funzione. Ritorna il numero di bytes      *
 * letti o -1 se fallisce.                                                 *
 * ************************************************************************/
int rs232_read(int ttys_descriptor,unsigned char *buffer, unsigned int max_bytes);

/* Scrive i bytes in buffer, per un massimo di buf_len. Ritorna il numero  *
 * di bytes scritti o -1 se fallisce.                                      *
 * ************************************************************************/
int rs232_write(int ttys_descriptor, unsigned char * buffer, unsigned int buf_len);
	
/* Chiude il file descriptor                                               */
void rs232_close(int ttys_descriptor);

#endif 
