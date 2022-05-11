#include "formulas_client.hpp"
#include "polynoms.hpp"
#include "tarsky_client.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#define LOCAL_PORT 9887
#define LOCAL_ADDR INADDR_ANY

int main()
{
        //подготовка к соединению с сервером
        char buffer[1024];
        int sock;
        int bytesRead;
        struct sockaddr_in addr;
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if(sock < 0)
        {
                perror("error while creating socket");
                exit(1);
        }
        addr.sin_family = AF_INET;
        addr.sin_port = htons(LOCAL_PORT);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        //соединение с сервером
        if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        {
                perror("error while connecting");
                exit(2);
        }

        //работа с сервером
	while(true)
	{
        	bytesRead = recv(sock, buffer, 1024, 0);
		if(bytesRead <= 0)
		{
			break;
		}
        	for(int iter = 0; iter < bytesRead; ++iter) printf("%c", buffer[iter]);
	}
        close(sock);
}

