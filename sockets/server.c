//Garret Stevens
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define PORT 1101

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

main(){
	//open a socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
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
	
	//accept the incoming connection
	struct sockaddr_in incoming_addr;
	int addr_size = 16;
	int acceptance = accept(sock, (struct sockaddr*) &incoming_addr, &addr_size);
	if(acceptance == -1){
		printf("ACCEPT ERROR: %s\n", strerror(errno));
		return -1;
	}
	
	//receive the message that comes in and print
	int buf_size = 64;
	char buffer[buf_size];
	int received = recv(acceptance, buffer, buf_size, 0);
	if(received == 0){
		printf("Socket is blocking\n");
		return 0;
	}else if(received == -1){
		printf("RECEIVE ERROR: %s\n", strerror(errno));
		return -1;
	}
	printf("Message received: %s\n", buffer);
	
	//shutdown
	printf("Shutting down...\n");
	int defecation = shutdown(sock, 2);
	if(defecation == -1){
		printf("SHUTDOWN ERROR: %s\n", strerror(errno));
		return -1;
	}
	printf("Done\n");
}
