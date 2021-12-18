/*
 ============================================================================
 Name        : Client.c
 Author      : Mark
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */



#include "protocols.h"

int main(int argc, char **argv) {
	char ipport[50];
	char opz;
	char ip[30];
	char serverip[30];
	char servername[30];
	struct hostent *host= NULL;
	char name[40];
	int port;
	struct in_addr* ina;
	setvbuf(stdout, NULL, _IONBF, 0);
	int result = WSAStartup(MAKEWORD(2 ,2), &wsa_data);
	if (result != 0) {
		printf ("error at WSAStartup\n");
		return -1;
	}
	c_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (c_socket < 0) {
		errorhandler("socket creation failed.\n");
		closesocket(c_socket);
		clearwinsock();
		return -1;
	}
	while(1){
		/*
		 * Standard IP is localhost
		 */
		if(argc!=2 ){
			if(argc>2){
				printf("wrong argument format\n");
			}
			do{
				printf("Would you like to use standard IP address and port? Y/N \n");
				opz=getchar();
				fflush(stdin);
			}
			while(opz!='Y' && opz!='y' && opz!='N' && opz!='n' );
			/*
			 * User can input different IP and port than localhost
			 */
			if(opz=='N' || opz=='n')
			{
				do  {
					printf("Input ip and port in the following format : [HOSTNAME]:[PORT]\n");
					gets(ipport);
					printf("\n");
					if(strchr(ipport,':')!= NULL){
						strcpy(name,strtok(ipport, ":"));
						host = gethostbyname(name);
						if(host==NULL)
							printf("gethostbyname() failed.\n");
					}
					else{
						printf("wrong format\n");
					}
				}while(host == NULL);
				ina = (struct in_addr*) host->h_addr_list[0];
				port=atoi(strtok(NULL, ":"));
				fflush(stdin);
				strcpy(ip,inet_ntoa(*ina));
				printf("sending to : ip:%s, port: %d\n", ip, port);
			}
			else
				/*
				 * localhost
				 */
			{
				strcpy(ip,"127.0.0.1");
				port=PROTOPORT;
				printf("\n");
			}
		}
		else{
			strcpy(ipport, argv[1]);
			strcpy(name,strtok(ipport, ":"));
			host = gethostbyname(name);
			if (host == NULL) {
				printf( "gethostbyname() failed.\n");
				do  {
					printf("Input ip and port in the following format : [HOSTNAME]:[PORT]\n");
					gets(ipport);
					printf("\n");
					if(strchr(ipport,':')!= NULL){
						strcpy(name,strtok(ipport, ":"));
						host = gethostbyname(name);
						if(host==NULL)
							printf("gethostbyname() failed.\n");
					}
					else{
						printf("wrong format\n");
					}
				}while(host == NULL);
				ina = (struct in_addr*) host->h_addr_list[0];
				port=atoi(strtok(NULL, ":"));
				fflush(stdin);
				strcpy(ip,inet_ntoa(*ina));
				printf("sending to: ip:%s, port: %d\n", ip, port);
			}
			else {
				ina = (struct in_addr*) host->h_addr_list[0];
				port=atoi(strtok(NULL, ":"));
				fflush(stdin);
				strcpy(ip,inet_ntoa(*ina));
				printf("sending to: ip:%s, port: %d\n", ip, port);
			}
		}
		memset(&sad, 0, sizeof(sad));
		sad.sin_family = PF_INET;
		sad.sin_addr.s_addr = inet_addr(ip);
		sad.sin_port = htons(port);
		printf("Input your operands in this format: [operator] [number] [number]\n");
		printf("The valid operators are: x, +, - and /\n");
		printf("Input = to quit\n");
		do{
			printf("\nWaiting for an input\n");
			printf("Input :");
			gets(msg.message);
			if(msg.message[0]=='='){
				printf("closing...");
				return 0;
			}
			/*
			 * Sending operation to Server
		     */

			if (sendto(c_socket, (void *)&msg, sizeof(msg), 0, (struct sockaddr*)&sad,  sizeof(sad))!= sizeof(msg)){
				errorhandler("sendto() sent a different number of bytes than expected");
				closesocket(c_socket);
				clearwinsock();
				return -1;
			}

			/*
			 * Receiving result from server
			 */
			int sadsize=(sizeof(sad));
			recvfrom(c_socket, (void *)&msg, BUFFERSIZE, 0, (struct sockaddr*)&sad, &sadsize);
			host=gethostbyaddr((char*)&sad.sin_addr.s_addr,4,AF_INET);
			strcpy(servername, host->h_name);
			struct in_addr* ina = (struct in_addr*) host->h_addr_list[0];
			strcpy(serverip,inet_ntoa(*ina));
			printf("result  '%s' received by server %s, ip %s\n", msg.message, servername, serverip);

			/*
			 * Connection is interrupted when user inputs '='
			 */
			if(msg.message[0]=='='){
				printf("closing...");
				return 0;}
		}
		while(1);

		if(msg.message[0]=='='){
			printf("closing...");
			return 0;}
	}
	closesocket(c_socket);
	clearwinsock();
	system("pause");
	return(0);
}




