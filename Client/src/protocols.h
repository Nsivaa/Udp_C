/*
 * protocols.h
 *
 *  Created on: 23 nov 2021
 *      Author: Marco
 */

#ifndef PROTOCOLS_H_
#define PROTOCOLS_H_
#if defined WIN32
#include <winsock.h>
#else
#define closesocket close
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define BUFFERSIZE 512
#define PROTOPORT 56700
char input_string[BUFFERSIZE];
struct msgStruct {
	 char message[BUFFERSIZE];
};
struct msgStruct msg;
struct sockaddr_in sad;
struct sockaddr_in cad;
int bytes_rcvd;
int total_bytes_rcvd = 0;
char buf[BUFFERSIZE];

void errorhandler(char *error_message) {
	printf("%s",error_message);
}

void clearwinsock() {
	#if defined WIN32
	WSACleanup();
	#endif
}

#if defined WIN32
WSADATA wsa_data;
#endif
int c_socket;

#endif /* PROTOCOLS_H_ */
