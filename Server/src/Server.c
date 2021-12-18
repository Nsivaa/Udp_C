
#include "protocols.h"
#define NO_ERROR 0

/*
 * addition between integers
 */
int add(int n1, int n2){
	int risultato=n1+n2;
	return risultato;
}

/*
 * multiplication between integers
 */
int mult(int n1, int n2){
	int risultato=n1*n2;
	return risultato;
}

/*
 * subtraction between integers
 */

int sub(int n1, int n2){
	int risultato=n1-n2;
	return risultato;
}

/*
 * division between reals
 */

float division(float n1, float n2){
	if(n2==0){return NAN;}
	float risultato=n1/n2;
	return risultato;
}
/*
 *Main function of the application
 */

int main(int argc, char *argv[]) {
	int my_socket;
	int client_len;
	struct hostent *remoteHost;
	char clientname[30];
	char clientip[30];
	setvbuf(stdout, NULL, _IONBF, 0);
	int result = WSAStartup(MAKEWORD(2,2), &wsa_data);
	if (result != NO_ERROR) {
		printf("Error at WSAStartup()\n");
		return 0;
	}
	my_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	sad.sin_family = PF_INET;
	/*
	 * Standard IP and port
	 */
	sad.sin_addr.s_addr = inet_addr( LOCALHOST );
	sad.sin_port = htons( PROTOPORT );
	/*
	 * Socket is closed if bind() fails
	 */
	if (bind(my_socket, (struct sockaddr*) &sad, sizeof(sad)) <0) {
		errorhandler("bind() failed.\n");
		closesocket(my_socket);
		return -1;
	}

	printf("Server started\n");

	while(1)
	{


		client_len = sizeof(cad);
		int bytes_rcvd;
		/*
		 * Receiving operation from Client
		 */
		bytes_rcvd=recvfrom(my_socket,(void *)&msg, BUFFERSIZE, 0, (struct sockaddr*)&cad, &client_len );
		msg.message[bytes_rcvd] = '\0';
		remoteHost=gethostbyaddr((char*)&sad.sin_addr.s_addr,4,AF_INET);
		strcpy(clientname, remoteHost->h_name);
		struct in_addr* ina = (struct in_addr*) remoteHost->h_addr_list[0];
		strcpy(clientip,inet_ntoa(*ina));

		printf("Operation  '%s' requested by client %s, ip %s\n", msg.message, clientname, clientip);
		/*
		 * Character matrix that will contain client's input
		 */
		char token[2][BUFFERSIZE];
		/*
		 * String that will contain computation's result
		 */
		char ris[BUFFERSIZE];
		strcpy(ris,msg.message);
		int n1, n2;
		int asol;
		float sol;

		/*
		 * Client's input is tokenized
		 */
		strcpy(token[0],strtok(msg.message, " "));

		/*
		 * Controls on the validity of input
		 */
		if(token[0][0]=='+' || token[0][0]=='-' || token[0][0]=='/' || token[0][0]=='x')
		{
			strcpy(token[1],strtok(NULL, " ") );
			strcpy(token[2],strtok(NULL, " ") );
			if(token[0][0]=='/' && atoi(token[2])==0)
			{ strcpy(ris,"Operation invalid"); }
			else{
				n1=atoi(token[1]);
				n2=atoi(token[2]);
				if(token[0][0]=='+'){
					asol=add(n1, n2 );
					sprintf(ris, "%d", asol);
				}
				if(token[0][0]=='-'){
					asol=sub(n1, n2 );
					sprintf(ris, "%d", asol);
				}
				if(token[0][0]=='/'){
					sol=division(n1, n2 );
					sprintf(ris, "%.2f", sol);
				}
				if(token[0][0]=='x'){
					asol=mult(n1, n2 );
					sprintf(ris, "%d", asol);
				}
				if(isnan(sol)){
					strcpy(ris,"Can't divide by 0");
				}

			}
		}
		else{
			strcpy(ris,"Operation invalid");
		}
		/*
		 * Concatenate tokens in correct order
		 */
		strcpy(msg.message, "");
		strcat(msg.message, token[1]);
		strcat(msg.message, " ");
		strcat(msg.message, token[0]);
		strcat(msg.message, " ");
		strcat(msg.message, token[2]);
		strcat(msg.message, " ");
		strcat(msg.message, "=");
		strcat(msg.message, " ");
		strcat(msg.message, ris);


		/*
		 * Sending result to Client
		 */
		if (sendto(my_socket, (void *)&msg, sizeof(msg), 0, (struct sockaddr*)&cad, client_len )!=sizeof(msg)){
			errorhandler("sendto() sent a different number of bytes than expected");
			closesocket(c_socket);
			clearwinsock();
			return -1;
		}


	}

	closesocket(my_socket);
	clearwinsock();
	return 0;
}





