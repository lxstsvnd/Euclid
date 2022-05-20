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
	std::vector<Polynom> part_polynom_matrix_calculation(std::vector<Polynom> unsaturated, int left_up_x, int left_up_y, int right_down_x, int right_down_y, int sock);

	void calculate(int sock)
	{
		//для записи полученных от сервера объектов нужны буферы
		char buffer[100];
		std::vector<Polynom> unsaturated;

		//пока вычисление не будет доведено до конца,
		//клиент повторяет цикл вычислений
		while(true)
		{

			//получение пределов таблицы от сервера
			int table[4];
			for(int tableIter = 0; tableIter < 4; ++tableIter)
			{
				memset(buffer, 0, 100);
				recv(sock, buffer, 100, 0);
				table[tableIter] = std::atoi(buffer);
				send(sock, "ready\0", 6, 0);
			}

			//получение вектора многочленов от сервера
			std::vector<mpz_class> coefs;
			while(true)
			{
				memset(buffer, 0, 100);
				int bytesRead = recv(sock, buffer, 100, 0);
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

			//запуск насыщения системы
			std::vector<Polynom> saturated = part_polynom_matrix_calculation(unsaturated, table[0], table[1], table[2], table[3], sock);

			//вектор многочленов возвращается на сервер
			send(sock, "ready\0", 6, 0); //говорит серверу, что все посчитано
			recv(sock, buffer, 100, 0); //ждет, пока сервер начнет его слушать
			for(int polyIter = 0; polyIter < saturated.size(); ++polyIter)
			{
				std::vector<mpz_class> coefs = saturated[polyIter].get_coefficients();
				for(int coefIter = 0; coefIter < coefs.size(); ++coefIter)
				{
					std::string tmp = coefs[coefIter].get_str();
					send(sock, tmp.c_str(), tmp.size(), 0);
					recv(sock, buffer, 100, 0);
				}
				send(sock, "finish\0", 7, 0);
				recv(sock, buffer, 100, 0);
			}
			send(sock, "end\0", 4, 0);
			int bytesRead = recv(sock, buffer, 100, 0);
		}
	}

	//Принимает матрицу остатков, вектор многочленов и координаты левого верхного и правого нижнего опорного элемента матрицы
	//Возвращает вектор остатков от деления многочленов в заданном промежутке
	std::vector<Polynom> part_polynom_matrix_calculation(std::vector<Polynom> unsaturated, int left_up_x, int left_up_y, int right_down_x, int right_down_y, int sock)
	{
		std::vector<Polynom> raw_polynoms;
		Polynom C;
	
		//непосредственное насыщение
		for(int i=left_up_x;i<right_down_x;i++)
		{
			for(int j =left_up_y;j<right_down_y;j++)
			{
				//считаем только верхнюю треугольную матрицу,
				//понятно, что номер столбца больше номера строки
				if(i > j)//если мы еще не пытались делить i многочлен на j
				{	
					if (unsaturated[i].get_degree()>=unsaturated[j].get_degree())
					{
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
