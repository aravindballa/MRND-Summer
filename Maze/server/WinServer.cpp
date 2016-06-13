#include "stdafx.h"
#include <winsock2.h>
#include <windows.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
struct node
{
	char msg[128];
	int msg_id;
	node *next;
}*flist,*alist,*printid;

struct bufserv{
	
		int userId;
		int forumId;
		int msgId;
		int commentId;
		int choice;
		char *forumname;
		char msg[128];
}buf1;

bool flag=true;
int mid = 0;
int count1 =0;
char *Data[100];
int count=1;
int values[100];
DWORD WINAPI SocketHandler(void*);
void replyto_client(char *buf, int *csock);

void socket_server() {

	//The port you want the server to listen on
	int host_port= 1102;

	unsigned short wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 2, 2 );
 	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 || ( LOBYTE( wsaData.wVersion ) != 2 ||
		    HIBYTE( wsaData.wVersion ) != 2 )) {
	    fprintf(stderr, "No sock dll %d\n",WSAGetLastError());
		goto FINISH;
	}

	//Initialize sockets and set options
	int hsock;
	int * p_int ;
	hsock = socket(AF_INET, SOCK_STREAM, 0);
	if(hsock == -1){
		printf("Error initializing socket %d\n",WSAGetLastError());
		goto FINISH;
	}
	
	//Enabling the reuse of address and enable sending of keep alive messages
	p_int = (int*)malloc(sizeof(int));
	*p_int = 1;
	if( (setsockopt(hsock, SOL_SOCKET, SO_REUSEADDR, (char*)p_int, sizeof(int)) == -1 )||
		(setsockopt(hsock, SOL_SOCKET, SO_KEEPALIVE, (char*)p_int, sizeof(int)) == -1 ) ){
		printf("Error setting options %d\n", WSAGetLastError());
		free(p_int);
		goto FINISH;
	}
	free(p_int);

	//Bind and listen
	struct sockaddr_in my_addr;
	my_addr.sin_family = AF_INET ;
	my_addr.sin_port = htons(host_port);
	
	memset(&(my_addr.sin_zero), 0, 8);
	my_addr.sin_addr.s_addr = INADDR_ANY ;
	
	/* if you get error in bind 
	make sure nothing else is listening on that port */
	if( bind( hsock, (struct sockaddr*)&my_addr, sizeof(my_addr)) == -1 ){
		fprintf(stderr,"Error binding to socket %d\n",WSAGetLastError());
		goto FINISH;
	}
	if(listen( hsock, 10) == -1 ){
		fprintf(stderr, "Error listening %d\n",WSAGetLastError());
		goto FINISH;
	}
	
	//Now lets do the actual server stuff

	int* csock;
	sockaddr_in sadr;
	int	addr_size = sizeof(SOCKADDR);
	
	
		printf("waiting for a connection\n");
		csock = (int*)malloc(sizeof(int));
		while (true)
		if ((*csock = accept(hsock, (SOCKADDR*)&sadr, &addr_size)) != INVALID_SOCKET){
			printf("Received connection from %s\n", inet_ntoa(sadr.sin_addr));
			CreateThread(0, 0, &SocketHandler, (void*)csock, 0, 0);
		}
		else{
			fprintf(stderr, "Error accepting %d\n", WSAGetLastError());
		}
	

FINISH:
;
}



int SolveMaze(int **maze, int m, int n, int x, int y, int x2, int y2, int **solution, int *path)
{
	static int mz = 0;

	if (x == x2 && y == y2)
	{
		solution[x][y] = 1;
		return 1;
	}

	if (x >= 0 && x < m && y >= 0 && y < n && maze[x][y] == 1)
	{
		solution[x][y] = 1;
		
		if (SolveMaze(maze, m, n, x + 1, y, x2, y2, solution, path))
		{
			printf("%d,%d ", x, y);
			path[mz++] = x; path[mz++] = y;
			return 1;
		}
		if (SolveMaze(maze, m, n, x, y + 1, x2, y2, solution, path))
		{
			printf("%d,%d ", x, y);
			path[mz++] = x; path[mz++] = y;
			return 1;
		}
		if (SolveMaze(maze, m, n, x - 1, y, x2, y2, solution, path))
		{
			printf("%d,%d ", x, y);
			path[mz++] = x; path[mz++] = y;
			return 1;
		}
		if (SolveMaze(maze, m, n, x, y - 1, x2, y2, solution, path))
		{
			printf("%d,%d ", x, y);
			path[mz++] = x; path[mz++] = y;
			return 1;
		}

		solution[x][y] = 0;
		return 0;
	}

	return 0;
}

int path_exists(int **maze, int m, int n, int x1, int y1, int x2, int y2, int *path)
{
	int i, j;
	//error checking


	int **solution = (int**)malloc(m*sizeof(int *));
	for (i = 0; i < m; i++)
		solution[i] = (int*)malloc(n*sizeof(int));



	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			solution[i][j] = 0;


	if (SolveMaze(maze, m, n, x1, y1, x2, y2, solution, path))
	{
		printf("Path exists\n");
		
		for (i = 0; i < m*n; i++)
			printf("%d ", path[i]);
		return 1;
	}

	printf("Path does not exist.\n");
	return 0;
}

void process_input(char *recvbuf, int recv_buf_cnt, int* csock) 
{
	char replybuf[1024]={'\0'};
	printf("client: ");
	printf("%s\n",recvbuf);

	int i, j;
	int rec_i = 0, rep_i = 0;
	int m = recvbuf[rec_i++] - '0', n = recvbuf[rec_i++] - '0';

	int **maze = (int**) malloc(m*sizeof(int *));
	for (i = 0; i < m; i++)
		maze[i] = (int*) malloc(n*sizeof(int));

	int *path = (int*)malloc(m*n*sizeof(int));
	for (i = 0; i < m*n; i++)
		path[i] = -1;

	for (i = 0; i < m; i++)
		for (j = 0; j < n; j++)
			maze[i][j] = recvbuf[rec_i++] - '0';

	for (i = 0; i < m; i++)
	{
		for (j = 0; j < n; j++)
			printf("%d ", maze[i][j]);
		printf("\n");
	}

	int x1 = recvbuf[rec_i++] - '0', y1 = recvbuf[rec_i++] - '0';
	int x2 = recvbuf[rec_i++] - '0', y2 = recvbuf[rec_i++] - '0';

	//process input
	if (path_exists(maze, m, n, x1, y1, x2, y2, path))
	{
		for (i = 0; i < m*n; i++)
			if (path[i] != -1)
				replybuf[i] = path[i] + '0';
			else
				replybuf[i] = '\0';
	}

	//strcpy(replybuf, "thanks.\n");
	replyto_client(replybuf, csock);
	replybuf[0] = '\0';
}

void replyto_client(char *buf, int *csock) {
	int bytecount;
	
	if((bytecount = send(*csock, buf, strlen(buf), 0))==SOCKET_ERROR){
		fprintf(stderr, "Error sending data %d\n", WSAGetLastError());
		free (csock);
	}
	printf("replied to client: %s\n",buf);
}

DWORD WINAPI SocketHandler(void* lp){
    int *csock = (int*)lp;

	char recvbuf[1024];
	int recvbuf_len = 1024;
	int recv_byte_cnt;
	//while (true){
		memset(recvbuf, 0, recvbuf_len);
		if ((recv_byte_cnt = recv(*csock, recvbuf, recvbuf_len, 0)) == SOCKET_ERROR)
		{
			fprintf(stderr, "Error receiving data %d\n", WSAGetLastError());
			free(csock);
			return 0;
		}
		
	//}
	//printf("Received bytes %d\nReceived string \"%s\"\n", recv_byte_cnt, recvbuf);
	process_input(recvbuf, recv_byte_cnt, csock);


    return 0;
}