#include "formulas_client.hpp"
#include "polynoms.hpp"
#include "tarsky_client.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <stdexcept>

#define LOCAL_PORT 5533
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
		throw std::runtime_error("Error while creating socket");
        }
        addr.sin_family = AF_INET;
        addr.sin_port = htons(LOCAL_PORT);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        //соединение с сервером
        if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        {
                throw std::runtime_error("Error while connecting");
        }
	std::cout << "Socket is connected to server" << std::endl;

        //работа с сервером
	std::cout << "Client is ready to calculate!" << std::endl;
	Kirill::calculate(sock);	

	//завершение работы
	close(sock);
}

