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
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = inet_addr("192.168.7.2");
	bzero(&(address.sin_zero), 8);
	
	//connect to the receive program
	int conn = connect(sock, (struct sockaddr *) &address, sizeof(struct sockaddr));
	if(conn == -1){
		printf("CONNECTION ERROR: %s\n", strerror(errno));
		return -1;
	}
	
	//send the message
	char* buffer = "Watson can you hear me?\0";
	int sent = send(sock, buffer, strlen(buffer), 0);
	if(sent == -1){
		printf("SEND ERROR: %s\n", strerror(errno));
		return -1;
	}
	printf("Sent %i bytes\n", sent);
	printf("Shutting down...\n");
	int shitdown = shutdown(sock, 2);
	if(shitdown == -1){
		printf("SHUTDOWN ERROR: %s\n", strerror(errno));
		return -1;
	}
	printf("Done\n");
}
