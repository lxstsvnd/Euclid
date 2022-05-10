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

#define LOCAL_PORT 5509
#define LOCAL_ADDR INADDR_ANY
#define TIMEOUT_SEC 10
#define TIMEOUT_USEC 0

int main()
{
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

	//подготовка к работе сервера
	int sockListener;
        char buffer[1024];
        int bytesRead;
        struct sockaddr_in addr;
        sockListener = socket(AF_INET, SOCK_STREAM, 0);
        if(sockListener < 0)
        {
                throw std::runtime_error("Error while building listener");
        }

	//заполнение полей адреса и привязка слушающего сокета
        addr.sin_family = AF_INET;
        addr.sin_port = htons(LOCAL_PORT);
        addr.sin_addr.s_addr = htonl(LOCAL_ADDR);
        if (bind(sockListener, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        {
                throw std::runtime_error("Error while binding listener");
        }

	//работа сервера
        listen(sockListener, 1);
        while(true)
	{
		//сессия инициализации клиентов:
		//в дочернем процессе запущен таймер,
		//в это время родительский процесс ждет соединения
		std::vector<int> fd;
		pid_t pid = fork();
		switch(pid)
		{
			case -1:
				throw std::runtime_error("Error while forking");
			case 0:
				sleep(5);
				_exit(0);
			default:
				while(true)
				{
					//ожидание завершения таймера
					waitpid(pid, &status, WHOHANG);
					if(WIFEXITED(status))
					{
						break;
					}

					//подключение клиентов
					int sockRecieve = accept(sockListener, NULL, NULL);
					if(sockReceive < 0)
					{
						wait(&status);
						throw std::runtime_error("Error while accepting client");
					}
					fd.push_back(sockReceive);
				}
		}

		//рассылка данных по подключенным клиентам
	}
}
