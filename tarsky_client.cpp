#include<iostream>
#include<vector>
#include<set>
#include<algorithm>
#include<math.h>
#include <cstring>
#include"tarsky_client.hpp"

//В этом файле описаны функции для работы с алгоритмом Тарского на клиентском устройстве

namespace Kirill
{
	std::vector<Polynom> part_polynom_matrix_calculation(std::vector<std::vector<int>> &Polynom_graph, std::vector<Polynom> unsaturated, int sock);

	void calculate(int sock)
	{
		//для записи полученных от сервера объектов нужны буферы
		char buffer[1024];
		std::vector<Polynom> unsaturated;

		//пока вычисление не будет доведено до конца,
		//клиент повторяет цикл вычислений
		while(true)
		{
			//получение таблицы от сервера
			std::vector<std::vector<int>> table;
			std::vector<int> column;
			while(true)
			{
				memset(buffer, 0, 1024);
				int bytesRead = recv(sock, buffer, 1024, 0);
				if(bytesRead <= 0)
				{
					break;
				}

				if(!strcmp(buffer, "finish"))
				{
					if(!column.empty())
					{
						table.push_back(column);
						column.clear();
					}
				}
				else if(!strcmp(buffer, "end"))
				{
					if(!column.empty())
					{
						table.push_back(column);
						column.clear();
					}
					break;
				}
				else
				{
					column.push_back(std::atoi(buffer));
				}
				send(sock, "ready\0", 6, 0);
			}
//			std::cout << "got table" << std::endl;
			send(sock, "ready\0", 6, 0);

			//получение вектора многочленов от сервера
			std::vector<mpz_class> coefs;
			while(true)
			{
				memset(buffer, 0, 1024);
				int bytesRead = recv(sock, buffer, 1024, 0);
				if(bytesRead <= 0)
				{
					break;
				}

				if(!strcmp(buffer, "finish"))
				{
					if(!coefs.empty())
					{
						Polynom tmpPoly(coefs);
						unsaturated.push_back(tmpPoly);
						coefs.clear();
					}
				}
				else if(!strcmp(buffer, "end"))
				{
					if(!coefs.empty())
					{
						Polynom tmpPoly(coefs);
						unsaturated.push_back(tmpPoly);
						coefs.clear();
					}
					break;
				}
				else
				{
					coefs.push_back(mpz_class(buffer));
				}
				send(sock, "ready\0", 6, 0);
			}
//			std::cout << "got polynoms" << std::endl;

			//запуск насыщения системы
			std::vector<Polynom> saturated = part_polynom_matrix_calculation(table, unsaturated, sock);

			//вектор многочленов возвращается на сервер
			send(sock, "ready\0", 6, 0); //говорит серверу, что все посчитано
			recv(sock, buffer, 1024, 0); //ждет, пока сервер начнет его слушать
			for(int polyIter = 0; polyIter < saturated.size(); ++polyIter)
			{
				std::vector<mpz_class> coefs = saturated[polyIter].get_coefficients();
				for(int coefIter = 0; coefIter < coefs.size(); ++coefIter)
				{
					std::string tmp = coefs[coefIter].get_str();
					send(sock, tmp.c_str(), tmp.size(), 0);
					recv(sock, buffer, 1024, 0);
				}
				send(sock, "finish\0", 7, 0);
				recv(sock, buffer, 1024, 0);
			}
			send(sock, "end\0", 4, 0);
			int bytesRead = recv(sock, buffer, 1024, 0);
//			std::cout << "sent" << std::endl;
		}
	}

	//Принимает матрицу остатков, вектор многочленов и координаты левого верхного и правого нижнего опорного элемента матрицы
	//Возвращает вектор остатков от деления многочленов в заданном промежутке
	std::vector<Polynom> part_polynom_matrix_calculation(std::vector<std::vector<int>> &Polynom_graph, std::vector<Polynom> unsaturated, int sock)
	{
		std::vector<Polynom> raw_polynoms;
		Polynom C;
	
		//непосредственное насыщение
		for(int i=0;i<Polynom_graph.size();i++)
		{
			for(int j =0;j<Polynom_graph[i].size();j++)
			{
				if(Polynom_graph[i][j]==0)//если мы еще не пытались делить i многочлен на j
				{	
					if (unsaturated[i].get_degree()>=unsaturated[j].get_degree())
					{
						//Если степень первого больше степени второго, то поставим пометки в массиве
						Polynom_graph[i][j]=1;
	
						C=divide(unsaturated[i],unsaturated[j]).first;
						if(C.get_degree()>0)
						{
							raw_polynoms.push_back(C);
							std::vector<mpz_class> negative_c=C.get_coefficients();
							for(int k=0;k<negative_c.size();k++)
							{
								negative_c[k]=-negative_c[k];
							}
							raw_polynoms.push_back(Polynom(negative_c));
						}
					}
					else
					{
						Polynom_graph[i][j]=-1;
						C=divide(unsaturated[j],unsaturated[i]).first;
						if(C.get_degree()>0)
						{
							raw_polynoms.push_back(C);
							std::vector<mpz_class> negative_c=C.get_coefficients();
							for(int k=0;k<negative_c.size();k++)
							{
								negative_c[k]=-negative_c[k];
							}
							raw_polynoms.push_back(Polynom(negative_c));
						}
					}
				}			
			}
		}
		return raw_polynoms;

	}
}
