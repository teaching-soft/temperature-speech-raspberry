/* speech.c
 * author: 5 TIEN
 * date: 2/03/2013
 * description: speech functions
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include "speech.h"


#define FESTIVAL_DEFAULT_SERVER_HOST "localhost"
#define FESTIVAL_DEFAULT_SERVER_PORT 1314
static int festival_socket_open(const char *host, int port);

// espeak flite festivall
// espeak   -vit+m/f1 "Ciao come stai"
/* echo "come stai" | festival --tts --pipe --language italian
*/
//#define SPEECH_PROGRAM	"/usr/bin/espeak"
#define SPEECH_PROGRAM	"festival"
void call_speech_engine(char *text_to_speech);
static int festival_socket_open(const char *host, int port);
// ------------------------------------------------------
void speech_value(char *text_to_speech)
{
	char buffer[200];
	// Open the socket only 1 time
	static int socket = -1;
	int n_byte_write;
	if(socket == -1) socket = festival_socket_open(FESTIVAL_DEFAULT_SERVER_HOST, FESTIVAL_DEFAULT_SERVER_PORT);
	if(socket == -1)
	{
		puts("Festival not found, I try to start server: festival --server --language italian");
		system ("festival --server --language italian &");
		sleep(2);
		socket = festival_socket_open(FESTIVAL_DEFAULT_SERVER_HOST, FESTIVAL_DEFAULT_SERVER_PORT);
		// Try next time
		if(socket == -1) return;
	}
	//	Set female voice
	strcpy(buffer,"(set! voice_default 'voice_lp_diphone)");
	n_byte_write = write(socket,buffer,strlen(buffer));
	if (n_byte_write < 0) puts("I can't send cmd to festival");
	//--
	sprintf(buffer,"(SayText \"%s\")",text_to_speech);
	n_byte_write = write(socket,buffer,strlen(buffer));
	if (n_byte_write < 0)  if (n_byte_write < 0) puts("I can't send message to speak");
}
// ---------------------------------------------------------------------
static int festival_socket_open(const char *host, int port)
{
    /* Return an FD to a remote server */
    struct sockaddr_in serv_addr;
    struct hostent *serverhost;
    int fd;

    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (fd < 0)
    {
		fprintf(stderr,"festival_client: can't get socket\n");
		return -1;
    }
    memset(&serv_addr, 0, sizeof(serv_addr));
    if ((serv_addr.sin_addr.s_addr = inet_addr(host)) == -1)
    {
		/* its a name rather than an ipnum */
		serverhost = gethostbyname(host);
		if (serverhost == (struct hostent *)0)
		{
			fprintf(stderr,"festival_client: gethostbyname failed\n");
			return -1;
		}
		memmove(&serv_addr.sin_addr,serverhost->h_addr, serverhost->h_length);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (connect(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) != 0)
    {
		fprintf(stderr,"festival_client: connect to server failed\n");
		return -1;
    }

    return fd;
}
/*
 * 	if(pid == 0)
	{
		questi lasciano dei defunc con espeak
		sprintf(buffer,"\"%s\"",text_to_speech);
		execl(SPEECH_PROGRAM, SPEECH_PROGRAM,buffer, NULL);
		exit(0);

		// echo "come stai" | festival --tts --pipe --language italian
		sprintf(buffer,"echo \"%s\" | %s --tts --pipe --language italian ",text_to_speech,SPEECH_PROGRAM);
		//execl(SPEECH_PROGRAM, SPEECH_PROGRAM,buffer, NULL);
		system (buffer);
		perror("Error in call_speech_engine");
	}
	//puts(text_to_speech);
	*/