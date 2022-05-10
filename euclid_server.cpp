#include "parser.hpp"
#include "lexer.hpp"
#include "formulas.hpp"
#include "polynoms.hpp"
#include "tarsky.hpp"

#include <unistd.h>
#include <sys/types.h>	
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>

#define LOCAL_PORT 5522
#define LOCAL_ADDR INADDR_ANY
#define SLEEP_TIMEOUT 5

int main()
{
	//подготовка к работе сервера
        int sockListener;
	char message[] = "Hello, World!\n";
        char buffer[sizeof(message)];
        struct sockaddr_in addr;
        sockListener = socket(AF_INET, SOCK_STREAM, 0);
        if(sockListener < 0)
        {
                throw std::runtime_error("Error while building listener");
        }
	fcntl(sockListener, F_SETFL, O_NONBLOCK);

        //заполнение полей адреса и привязка слушающего сокета
        addr.sin_family = AF_INET;
        addr.sin_port = htons(LOCAL_PORT);
        addr.sin_addr.s_addr = htonl(LOCAL_ADDR);
        if (bind(sockListener, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        {
                throw std::runtime_error("Error while binding listener");
        }

	//сессия инициализации клиентов:
	//в дочернем процессе запускается таймер,
	//в это время родительский процесс устанаваливает соединения
	std::vector<int> fd;
	listen(sockListener, 1);
	pid_t pid = fork();
	switch(pid)
	{
		case -1:
			throw std::runtime_error("Error while forking");
		case 0:
			sleep(SLEEP_TIMEOUT);
			std::cout << "exited" << std::endl;
			_exit(0);
		default:
			while(true)
			{
				//ожидание завершения таймера
				int status;
				waitpid(pid, &status, WNOHANG);
				std::cout << status << std::endl;
				std::cout << WIFEXITED(status) << std::endl;
				if(WIFEXITED(status))
				{
					break;
				}
				std::cout << "accepting" << std::endl;

				//подключение клиентов
				int sockReceive = accept(sockListener, NULL, NULL);
				std::cout << sockReceive << std::endl;
				//клиент добавляется в вектор клиентов
				if(sockReceive >= 0)
				{
					fd.push_back(sockReceive);
				}
			}
	}

	std::cout << fd.size() << std::endl;
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

	//организация распределенных вычислений
//	while(true)
//	{
//		for(int iter = 0; iter < fd.size(); ++iter)
//		{
//			send(fd[iter], message, sizeof(message), 0);
//		}
//	}
}
