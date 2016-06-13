#include <winsock2.h>
#include "stdafx.h"
#include <windows.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <conio.h>

int getsocket()
{
	int hsock;
	int * p_int ;
	hsock = socket(AF_INET, SOCK_STREAM, 0);
	if(hsock == -1){
		printf("Error initializing socket %d\n",WSAGetLastError());
		return -1;
	}
	
	p_int = (int*)malloc(sizeof(int));
	*p_int = 1;
	if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
		(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
		printf("Error setting options %d\n", WSAGetLastError());
		free(p_int);
		return -1;
	}
	free(p_int);

	return hsock;
}

void socket_client()
{

	//The port and address you want to connect to
	int host_port= 1102;
	char* host_name="127.0.0.1";

	//Initialize socket support WINDOWS ONLY!
	unsigned short wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 2, 2 );
 	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 || ( LOBYTE( wsaData.wVersion ) != 2 ||
		    HIBYTE( wsaData.wVersion ) != 2 )) {
	    fprintf(stderr, "Could not find sock dll %d\n",WSAGetLastError());
		goto FINISH;
	}

	//Initialize sockets and set any options

	//Connect to the server
	struct sockaddr_in my_addr;

	my_addr.sin_family = AF_INET ;
	my_addr.sin_port = htons(host_port);
	
	memset(&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = inet_addr(host_name);

	//if( connect( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == SOCKET_ERROR ){
	//	fprintf(stderr, "Error connecting socket %d\n", WSAGetLastError());
	//	goto FINISH;
	//}

	//Now lets do the client related stuff
	char buffer[1024];
	int buffer_len = 1024;
	int bytecount;
	int c;

	while(true) {

		int hsock = getsocket();
		//add error checking on hsock...
		if( connect(hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == SOCKET_ERROR ){
			fprintf(stderr, "Error connecting socket %d\n", WSAGetLastError());
			goto FINISH;
		}

		memset(buffer, '\0', buffer_len);

		FILE *fp;
		int m, n, i, j, b = 0;
		char ch;

		fp = fopen("input.txt", "r+");

		while ((ch = fgetc(fp)) != EOF)
		{
			if (ch == ',' || ch == '\n')
				ch = fgetc(fp);
			buffer[b++] = ch;
		}

		fclose(fp);

		if ((bytecount = send(hsock, buffer, strlen(buffer), 0)) == SOCKET_ERROR){
			fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
		}
		printf("Sent bytes %d\n", bytecount);
		printf("Sent message: %s\n", buffer);


		if ((bytecount = recv(hsock, buffer, buffer_len, 0)) == SOCKET_ERROR){
			fprintf(stderr, "Error receiving client data %d\n", WSAGetLastError());
				
		}
		printf("Recieved bytes %d\nReceived string \"%s\"\n", bytecount, buffer);

		char nl = '\n';
		char comma = ',';

		fp = fopen("input.txt", "r+");
		fseek(fp, 1, SEEK_END);
		for (i = 0; buffer[i] != '\0'; i++)
		{
			fwrite(&buffer[i++], sizeof(char), 1, fp);
			fwrite(&comma, sizeof(char), 1, fp);
			fwrite(&buffer[i], sizeof(char), 1, fp);
			fwrite(&nl, sizeof(char), 1, fp);
		}

		fclose(fp);

		closesocket(hsock);
	}

	//closesocket(hsock);
FINISH:
;
}