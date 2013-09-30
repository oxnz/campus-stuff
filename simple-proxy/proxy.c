/**
 * File			: proxy.c
 * Abstract		: a simple proxy program
 * Version		: 0.1
 * Author		: 0xnz
 * Last-update	: 2013-09-30 18:42:42
 * Copying		: Copyright (c) 2013 oxnz, All rights reserved.
 */

#include<stdio.h>  
#include<string.h>  
#include<poll.h>  

#include<sys/types.h>  
#include<sys/socket.h>  
#include<arpa/inet.h>  
#include<unistd.h>  

#define ADDR "127.0.0.1"  
#define PORT 8080  
#define PORT_APACHE 8088  
#define SIZE 1024  

int main()  
{  
	int ret=0;  
	int maxsocket;  
	int apa,ser,cli;  
	char buf[SIZE]={0};  
	char apabuf[SIZE]={0};  
	struct pollfd fds[5]={0};  
	struct sockaddr_in apaaddr,seraddr,cliaddr;  
	socklen_t clilen=sizeof(cliaddr);  

	apaaddr.sin_family=AF_INET;  
	apaaddr.sin_addr.s_addr=inet_addr(ADDR);  
	apaaddr.sin_port=htons(PORT_APACHE);  

	seraddr.sin_family=AF_INET;  
	seraddr.sin_addr.s_addr=inet_addr(ADDR);  
	seraddr.sin_port=htons(PORT);  

	apa=socket(AF_INET,SOCK_STREAM,0);  
	ser=socket(AF_INET,SOCK_STREAM,0);  
	bind(ser,(struct sockaddr*)&seraddr,sizeof(seraddr));  
	fds[0].fd=ser;  
	fds[0].events=POLLIN;  
	listen(fds[0].fd,5);  
	while(1)  
	{  
		memset(buf,0,SIZE);  
		memset(apabuf,0,SIZE);  
		ret=poll(fds,sizeof(fds)/sizeof(fds[0]),-1);  
		if(ret<0)  
		{  
			printf("poll error\n");  
			break;  
		}  
		if(fds[0].revents&POLLIN)  
		{  
			cli=accept(fds[0].fd,(struct sockaddr*)&cliaddr,&clilen);  
			recv(cli,buf,SIZE,0);  
			printf("get from %s:%d-- %s\n",inet_ntoa(cliaddr.sin_addr),  
					ntohs(cliaddr.sin_port),buf);  
			connect(apa,(struct sockaddr*)&apaaddr,sizeof(apaaddr));  
			send(apa,buf,SIZE,0);  
			recv(apa,apabuf,SIZE,0);  
			send(cli,apabuf,SIZE,0);  
		}  
	}  
	return 0;  
}  
