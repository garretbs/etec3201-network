//Garret Stevens
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <winsock.h>

#define PORT 1101
#define BUF_SIZE 512

struct addr
{
	unsigned long s_addr;
};

struct sockaddr_in
{
	short int 			sin_family;		//AF_INET
	unsigned short int  sin_port; 		//port we want to connect to
	struct addr 		sin_addr;		//address of machine we want to connect to
	unsigned char 		sin_zero[8];	//8 bytes of padding. should be all 0s
};

int sock;
int client_sock;
char buffer[BUF_SIZE];

const char* eight_ball[] = {
							"Outlook not clear",
							"Definitely yes",
							"Ask again later",
							"Please try again",
							"You're gonna die",
							"The gods say no",
							"It may be so",
							"Never"
};

int webpage(){
	char msg[512];
	
	sprintf(msg, "HTTP/1.0 200 OK\r\n");
	sprintf(msg, "%s\r\n", msg);
	sprintf(msg, "%s<html>\n", msg);
	sprintf(msg, "%s<body><p>\n", msg);
	sprintf(msg, "%s%s\n", msg, eight_ball[rand()%8]);
	sprintf(msg, "%s</p></body>\n", msg);
	sprintf(msg, "%s</html>\n\0", msg);
	
	return send(client_sock, msg, strlen(msg), 0);
}

main(){
	WSADATA wsa;
	int startedup = WSAStartup(MAKEWORD(2, 2), &wsa);
	if(startedup != 0){
		printf("WINSOCK ERROR\n");
		return -1;
	}
	
	struct sockaddr_in incoming_addr;
	int addr_size = 16;
	
	int received;
	int closer;
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1){
		printf("SOCKET ERROR: %s\n", strerror(errno));
		return -1;
	}
	
	//create an address structure and fill it in
	struct sockaddr_in serv_address;
	serv_address.sin_family = AF_INET;
	serv_address.sin_port = htons(PORT);
	serv_address.sin_addr.s_addr = 0; //this is the server, so...
	bzero(&(serv_address.sin_zero), 8);
	
	int tippy_tinkletrousers = 1;
	int soccer = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &tippy_tinkletrousers, sizeof(int));
	if(soccer == -1){
		printf("SETSOCKOPT ERROR: %s\n", strerror(errno));
		return -1;
	}
	
	//bind the socket to an unusued port
	int bound = bind(sock, (struct sockaddr*) &serv_address, 16);
	if(bound == -1){
		printf("BIND ERROR: %s\n", strerror(errno));
		return -1;
	}
	
	//listen for incoming connections
	int listening = listen(sock, 16);
	if(listening == -1){
		printf("LISTEN ERROR: %s\n", strerror(errno));
		return -1;
	}
	
	while(1){
		//accept the incoming connection
		client_sock = accept(sock, (struct sockaddr*) &incoming_addr, &addr_size);
		if(client_sock == -1){
			printf("ACCEPT ERROR: %s\n", strerror(errno));
			return -1;
		}
		
		received = recv(client_sock, buffer, BUF_SIZE, 0);
		if(received == 0){
			printf("Socket is blocking\n");
			return 0;
		}else if(received == -1){
			printf("RECEIVE ERROR: %s\n", strerror(errno));
			return -1;
		}
		
		const char* get_ = "GET /";
		const char* quit = "quit";
		
		if(strcmp(strstr(buffer, get_)-buffer+5, quit) == 0){
			closer = close(client_sock);
			if(closer == -1){
				printf("CLOSE ERROR: %s\n", strerror(errno));
			}
			break; //exit the program
		}
		
		if(webpage() == -1){
			printf("SEND ERROR: %s\n", strerror(errno));
			return -1;
		}
		
		closer = close(client_sock);
		if(closer == -1){
			printf("CLOSE ERROR: %s\n", strerror(errno));
		}
	}
	
	//shutdown
	printf("Shutting down...\n");
	int poopsydaisy = closesocket(sock, 2);
	if(poopsydaisy == -1){
		printf("SHUTDOWN ERROR: %s\n", strerror(errno));
		return -1;
	}
	WSACleanup();
	printf("Done\n");
}