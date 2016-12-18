 /* speech.h
 * author: 5 ELE
 * date: 12/01/2016
 * description: main.c
 */
#include <stdlib.h>
#include <stdio.h>
#include "speech.h"
#include "rs232.h"

/* Serial message format: value\n es. 127\n */
#define MAX_LEN_VALUE   4
void empty_serial_buffer(int ttys_descriptor);
void read_serial_value(int ttys_descriptor,char *value);


int main(int argc, char *argv[])
{
    // Controlla che ci sia il device sulla riga di comando
    if(argc != 2){
        puts("Inserire il device da cui si ricevono i dati");
        exit(1);
    }
   // Prova ad aprire il device/porta seriale 
   int ttys_descriptor = rs232_open(argv[1],B9600,CS8,PNONE,SB1);
    if(ttys_descriptor == -1){
		perror("SERIAL ERROR:Port not open!!!\nSerial port is:");
		printf("%s\n",argv[1]);
		exit(-1);
	}
    // Elimina eventuali dati parziali
    empty_serial_buffer(ttys_descriptor);
    // Legge il primo valore
    char value[10];
    while(1) {
        read_serial_value(ttys_descriptor,value);
        speech_value(value);
    }
    return EXIT_SUCCESS;
    
}

void empty_serial_buffer(int ttys_descriptor)
{
    char buffer[10];
    int bytes;
    while(1) {
        bytes = rs232_bytes_arrived(ttys_descriptor,2);
        if(bytes <= 0){
            puts("No data from device");
            continue;
        }
        rs232_read(ttys_descriptor,buffer,1);
        if(buffer[0] != '\n') continue;
        // I prossimi dati sono completi
        break;
    }
}

void read_serial_value(int ttys_descriptor,char *value)
{
    int bytes, pos = 0;char buffer[10];
    while(1) {
        bytes = rs232_bytes_arrived(ttys_descriptor,2);
        if(bytes <= 0){
            puts("No data for value from device");
            continue;
        }
        
        rs232_read(ttys_descriptor,buffer,1);
        // Controlla se è arrivato il fine valore
        if(buffer[0] == '\n'){
            value[pos] = '\0';
            return;
        }
        value[pos++] = buffer[0];
        if(pos >= MAX_LEN_VALUE){
            puts("Anomalia in ricezione, valore troppo lungo\nAspetto il successivo");
            pos = 0;
            empty_serial_buffer(ttys_descriptor);
        }
    }    
}
