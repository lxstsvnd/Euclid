#include "parser.hpp"
#include "lexer.hpp"
#include "formulas_server.hpp"
#include "polynoms.hpp"
#include "tarsky_server.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>

#define LOCAL_PORT 5534
#define LOCAL_ADDR INADDR_ANY
#define CONNECT_TIMEOUT 10

int main(int argc, char* argv[])
{
        //подготовка к работе сервера
	int CLIENTS_NUM;
	if (argc == 1)
	{
		throw std::runtime_error("Error: that's not a single-computer version");
	}
	CLIENTS_NUM = std::atoi(argv[1]);

	int sockListener;
        char message[] = "Hello, World!\n";
        char buffer[sizeof(message)];
        struct sockaddr_in addr;
        sockListener = socket(AF_INET, SOCK_STREAM, 0);
        if(sockListener < 0)
        {
                throw std::runtime_error("Error while building listener");
        }

        //заполнение полей адреса и привязка неблокирующего слушающего сокета
        addr.sin_family = AF_INET;
        addr.sin_port = htons(LOCAL_PORT);
        addr.sin_addr.s_addr = htonl(LOCAL_ADDR);
        if (bind(sockListener, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        {
                throw std::runtime_error("Error while binding listener");
        }

        //сессия инициализации клиентов:
        //до наступления момента CONNECT_TIMEOUT
        //сокет пытается принять клиентов
        std::vector<int> fd;
        listen(sockListener, 1);
        for(int clientCounter = 0; clientCounter < CLIENTS_NUM; ++clientCounter)
        {
                int sockReceive = accept(sockListener, NULL, NULL);
                if(sockReceive < 0)
                {
                        throw std::runtime_error("Error while receiving");
                }
		std::cout << "client " << clientCounter + 1 << " joined!" << std::endl;
		fd.push_back(sockReceive);
        }

        std::cout << "Server is ready to work" << std::endl;
        //токенизация и синтаксический анализ ввода
        std::string text;
        std::getline(std::cin, text);
        Dima::Tokenizer lexer(text);
        lexer.debugPrint();
        Dima::parseTree decision(lexer.get_tokens());
        decision.parse();

        //извлечение ДНФ из дерева разбора
        decision.makeDNF();
        Kirill::DNF dnf = decision.getDNF();
        dnf.printPolynoms();

	//вычисления
	std::cout<<"\n\n"<<std::endl;
	int start_time=clock();
	std::cout<<dnf.decide(fd, sockListener)<<std::endl;
	int time = clock()-start_time;
	std::cout<< "time: " << (double)time / CLOCKS_PER_SEC << std::endl;

	//завершение работы сервера
	for(int iter = 0; iter < fd.size(); ++iter)
	{
		close(fd[iter]);
	}
	close(sockListener);
	std::cout << "Server is shutting down" << std::endl;
}


