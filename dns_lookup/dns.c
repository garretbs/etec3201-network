//Garret Stevens
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>

main(int argc, char** argv){
	if(argc != 2){
		printf("Invalid number of arguments\n");
		return -1;
	}
	
	struct hostent* carlos;
	carlos = gethostbyname(argv[1]);
	if(!carlos){
		printf("DNS information not found\n");
		return -1;
	}
	printf("Official name: %s\n", carlos->h_name);
	
	int i = 0;
	while(carlos->h_aliases[i]) printf("Alias %i: %s\n", i, carlos->h_aliases[i++]);
	
	i = 0;
	while(carlos->h_addr_list[i] != NULL) printf("Address %i: %s\n", i, inet_ntoa(*(struct sockaddr_in*) carlos->h_addr_list[i++]));
}