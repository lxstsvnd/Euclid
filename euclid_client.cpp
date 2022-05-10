#include "parser.hpp"
#include "lexer.hpp"
#include "formulas.hpp"
#include "polynoms.hpp"
#include "tarsky.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

#define LOCAL_PORT 5509
#define LOCAL_ADDR INADDR_ANY

int main()
{
	//подготовка к соединению с сервером
        char message[] = "Hello, World!\n";
        char buffer[sizeof(message)];
        int sock;
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
        send(sock, message, sizeof(message), 0);
        recv(sock, buffer, sizeof(message), 0);
        for(int iter = 0; iter < sizeof(message); ++iter) printf("%c", buffer[iter]);
        close(sock);
}

