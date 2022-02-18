/***************************************************************************
	progetto			: "Raspberry speech" con scheda snowboard
    file:				: main.c
    begin               : mer apr 21 10:34:57 CET 2011
    copyright           : (C) 2011 by Giancarlo Martini
    email               : gm@giancarlomartini.it
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "speech.h"
#include "rs232.h"

/* Serial message format: value\n es. 127\n */
#define MAX_LEN_VALUE   4
#define true 1
#define false 0

void empty_serial_buffer(int ttys_descriptor);
void read_serial_value(int ttys_descriptor,char *value);
int get_value(char *device, char *buffer);
#define MAX_VALUE_NUM 10
int main(int argc, char *argv[]) {
	int i;
	// Controlla che ci sia il device sulla riga di comando
	if(argc != 2) {
		puts("Inserire il device da cui si ricevono i dati");
		exit(1);
	}
	char value[10];
	int somma = 0;
	char value_to_speech[100];
	while(1) {
		for(i = 0; i < MAX_VALUE_NUM; i++) {
			// Legge il valore
			if(get_value(argv[1],value) == false) continue;
			somma += atoi(value);
		}
		sprintf(value_to_speech,"%d",somma/MAX_VALUE_NUM);
		printf("Media valori %s\n",value_to_speech);
		speech_value(value);
		somma = 0;
		//sleep(1);
	}
	return EXIT_SUCCESS;

}

int get_value(char *device, char *buffer) {

	// Prova ad aprire il device/porta seriale
	int ttys_descriptor = rs232_open(device,B9600,CS8,PNONE,SB1);
	if(ttys_descriptor == -1) {
		perror("SERIAL ERROR:Port not open!!!\nSerial port is:");
		printf("%s\n",device);
		return false;
	}
	// Elimina eventuali dati parziali
	empty_serial_buffer(ttys_descriptor);
	read_serial_value(ttys_descriptor,buffer);
	rs232_close(ttys_descriptor);
	return true;
}
void empty_serial_buffer(int ttys_descriptor) {
	char buffer[10];
	int bytes;
	//fflush(ttys_descriptor);
	while(1) {
		bytes = rs232_bytes_arrived(ttys_descriptor,2);
		if(bytes <= 0) {
			puts("No data from device");
			continue;
		}
		rs232_read(ttys_descriptor,buffer,1);
		if(buffer[0] != '\n') continue;
		// I prossimi dati sono completi
		break;
	}
}

void read_serial_value(int ttys_descriptor,char *value) {
	int bytes, pos = 0;
	char buffer[10];
	char dummy[1000];
	while(1) {
		bytes = rs232_bytes_arrived(ttys_descriptor,2);
		if(bytes <= 0) {
			puts("No data for value from device");
			continue;
		}

		rs232_read(ttys_descriptor,buffer,1);
		// Controlla se è arrivato il fine valore

		if(buffer[0] == '\n') {
			value[pos] = '\0';
			return;
		}
		if(isdigit(buffer[0]) == 0) continue;
		//printf("->Pos:%i v:%c\n",pos,buffer[0]);
		value[pos] = buffer[0];
		pos++;
		if(pos > MAX_LEN_VALUE) {
			value[pos] = '\0';
			printf("Anomalia in ricezione, valore troppo lungo:%s\nAspetto il successivo",value);
			pos = 0;
			empty_serial_buffer(ttys_descriptor);
		}

	}
}
