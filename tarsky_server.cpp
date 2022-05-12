#include<iostream>
#include<vector>
#include<set>
#include<algorithm>
#include<math.h>
#include"tarsky_server.hpp"

//В этом файле описаны функции для работы с алгоритмом Тарского

namespace Kirill
{
	//Функция возвращает строки Таблицы тарского для насыщенной системы но оставляет только исходные многочлены
	std::vector<std::vector<int>> get_format_table(std::vector<Polynom> DNF_polynoms, std::vector<int> fd, int sockListener)
	{
		std::vector<std::vector<int>> format_table(DNF_polynoms.size());
		std::vector<Polynom> saturated = full_saturation(DNF_polynoms, fd, sockListener);
		std::vector<std::vector<int>> table = tars_table(saturated);
	//	Нужно отсеять таблицы только для исходных многочленов
		std::vector<int> indexes;
		//Можно сделать быстрее если останется время
		for(int i = 0; i < saturated.size();i++)
		{
			for(int j = 0; j < DNF_polynoms.size();j++)
			{
				if(saturated[i]==DNF_polynoms[j])
				{
					indexes.push_back(i);
				}		
			}
		}
		
		for(int i = 0; i < indexes.size(); i++)
			format_table[i]=table[indexes[i]];

		if(format_table.size()!=DNF_polynoms.size())
			std::cout<<"ERROR,ERROR,ERROR"<<std::endl;

		return format_table;
	}

	//Функция принимает некоторый вектор многочленов, возвращает насыщенную 
	//относительно взятия производной и взятия остатка от деления систему многочленов
	//упорядоченную по возрастанию степени
	std::vector<Polynom> full_saturation(std::vector<Polynom> unsaturated, std::vector<int> fd, int sockListener)
	{
	//Насыщаем относительно взятия остатка и взятия производной, пока насыщение не перестанет добавлять новых многочленов
		int size=unsaturated.size();
		while(1)
		{
			unsaturated=derivation_saturation(unsaturated);
			unsaturated=mod_saturation(unsaturated, fd, sockListener);
			if (unsaturated.size()==size)
				break;
			size=unsaturated.size();
	
		}
			for(int i=0;i<unsaturated.size();i++)
				if(unsaturated[i].get_degree()==0)
					unsaturated.erase(unsaturated.begin()+i);
		degree_sort(unsaturated);
		return unsaturated;
	}

	//Функция принимает некоторый вектор многочленов и возвращает относительно взятия производных систему многочленов
	std::vector<Polynom> derivation_saturation(std::vector<Polynom> unsaturated)
	{
		std::vector<mpz_class> c;
		Polynom C(c);
		std::vector<Polynom> new_polynoms;//Множество новых многочленов
		for (int i =0;i<unsaturated.size();i++)
		{
			C=unsaturated[i];
			while (C.get_degree()>0)//Фиксируем многочлен и все его неконстантные производные добавляем в массив
			{
				C=C.get_derivative();
				C.print();
				if(C.get_degree()>0)
					new_polynoms.push_back(C);
			}
		}
	
	
		for (int i=0;i<new_polynoms.size();i++)
			unsaturated.push_back(new_polynoms[i]);
	
		uniquying(unsaturated);//Оставляем только уникальные многочлены
		return unsaturated;
	}	


	//Принимает матрицу остатков, вектор многочленов и координаты левого верхного и правого нижнего опорного элемента матрицы
	//Возвращает вектор остатков от деления многочленов в заданном промежутке

	std::vector<Polynom> part_polynom_matrix_calculation(std::vector<std::vector<int>> &Polynom_graph, std::vector<Polynom> unsaturated, int left_up_x, int left_up_y,int right_down_x, int right_down_y, std::vector<int> fd, int sockListener)
	{
        	for(int fdIter = 0; fdIter < fd.size(); ++fdIter)
        	{
			char buffer1[1024];
			char buffer2[1024];
                	//рассылка таблицы по всем клиентам
                        //таблица отправляется по столбцам
 	                for(int columnIter = left_up_x; columnIter < right_down_x; ++columnIter)
                        {
				char buffer[1024];
                        	for(int rowIter = left_up_y; rowIter < right_down_y; ++rowIter)
                                {
                	                std::string tmp = std::to_string(Polynom_graph[columnIter][rowIter]);
                                        send(fd[fdIter], tmp.c_str(), tmp.size(), 0);
					recv(fd[fdIter], buffer, 1024, 0);
                                }
				send(fd[fdIter], "finish\0", 7, 0);
				recv(fd[fdIter], buffer, 1024, 0);
                        }
			send(fd[fdIter], "end\0", 4, 0);
			recv(fd[fdIter], buffer1, 1024, 0);

			std::cout << "table sent" << std::endl;
			//отправка вектора многочленов по клиентам
			//отправляются коэффициенты, многочлен
			//собирается на месте
			for(int polyIter = 0; polyIter < unsaturated.size(); ++polyIter)
			{
				char buffer[1024];
				std::vector<mpz_class> coefs = unsaturated[polyIter].get_coefficients();
				for(int coefIter = 0; coefIter < coefs.size(); ++coefIter)
				{
					std::string tmp = coefs[coefIter].get_str();
					send(fd[fdIter], tmp.c_str(), tmp.size(), 0);
					recv(fd[fdIter], buffer, 1024, 0);
				}
				send(fd[fdIter], "finish\0", 7, 0);
				recv(fd[fdIter], buffer, 1024, 0);
			}
			send(fd[fdIter], "end\0", 4, 0);
			recv(fd[fdIter], buffer2, 1024, 0);

			std::cout << "polynom sent" << std::endl;
                }

		//многочлены собираются обратно и записываются в raw_polynoms
		std::vector<Polynom> raw_polynoms;
		std::vector<mpz_class> coefs;
		std::cout << "start writing polynoms" << std::endl;
		for(int fdIter = 0; fdIter < fd.size(); ++fdIter)
		{
			char message[] = "ready";
			while(true)
			{
				char buffer[1024];
				std::cout << "waiting" << std::endl;
				int bytesRead = recv(fd[fdIter], buffer, 1024, 0);
				if(bytesRead <= 0)
				{
					break;
				}

				if(!strcmp(buffer, "finish"))
				{
					if(!coefs.empty())
					{
						Polynom tmpPoly(coefs);
						raw_polynoms.push_back(tmpPoly);
						coefs.clear();
					}
				}
				if(!strcmp(buffer, "end"))
				{
					if(!coefs.empty())
					{
						Polynom tmpPoly(coefs);
						raw_polynoms.push_back(tmpPoly);
						coefs.clear();
					}
					break;
				}
				else
				{
					std::cout << buffer << std::endl;
					coefs.push_back(mpz_class(buffer));
					std::cout << "written" << std::endl;
				}
				send(fd[fdIter], message, sizeof(message), 0);
			}
			send(fd[fdIter], message, sizeof(message), 0);
		}
		
		std::cout << "polynoms got" << std::endl;

		return raw_polynoms;
	}

	//Функция принимает некоторый вектор многочленов и возращает относительно взятия остатков систему многочленов
	std::vector<Polynom> mod_saturation(std::vector<Polynom> unsaturated, std::vector<int> fd, int sockListener)
	{
		Polynom C=unsaturated[0];
		std::vector<Polynom> raw_polynoms;
		int size=unsaturated.size();
	
		std::vector<std::vector<int>> Polynom_graph;//Матрица делимостия
		//в i-м столбце и j-м строке находится маркер, если было произведено
		//деление i-го многочлена в массиве на j-й многочлен в массиве
		//с учетом того что степень первого не меньше степени второго
		
		std::vector<int> matrix_null_column(size,0);//столбец из нулей
	
		for(int i=0;i<size;i++)
			Polynom_graph.push_back(matrix_null_column);

		int diff = Polynom_graph.size();
		
		//Непосредственное замыкание
		while (1)
		{
			//деление многочлена на самого себя не имеет смысла
			for(int i=0;i<Polynom_graph.size();i++)
				Polynom_graph[i][i]=1;
	
			//Добавляем многочлены-остатки
			if(Polynom_graph.size() == diff)
			{
				raw_polynoms=part_polynom_matrix_calculation(Polynom_graph,unsaturated, 0,0,Polynom_graph.size(),Polynom_graph.size(), fd, sockListener);
				for(int i = 0; i < raw_polynoms.size(); i++)
				{
					unsaturated.push_back(raw_polynoms[i]);
				}
			}
			else
			{
				raw_polynoms=part_polynom_matrix_calculation(Polynom_graph, unsaturated, Polynom_graph.size()-diff, 0, Polynom_graph.size(), Polynom_graph.size()-diff, fd, sockListener);
				for(int i = 0; i < raw_polynoms.size(); i++)
				{
					unsaturated.push_back(raw_polynoms[i]);
				}
				raw_polynoms=part_polynom_matrix_calculation(Polynom_graph, unsaturated, 0, Polynom_graph.size()-diff, Polynom_graph.size(), Polynom_graph.size(), fd, sockListener);
				for(int i = 0; i < raw_polynoms.size(); i++)
				{
					unsaturated.push_back(raw_polynoms[i]);
				}
			}
			std::cout << "unique" << std::endl;
			uniquying(unsaturated);//оставляем только уникальные
	
			//удаляем константы
			for(int i=0;i<unsaturated.size();i++)
				if(unsaturated[i].get_degree()==0)
					unsaturated.erase(unsaturated.begin()+i);
	
			//Расширяем граф добавляя места для маркеров деления новых многочленов
			diff=unsaturated.size()-size;//разница в размере графа
			std::vector<int> null_column(unsaturated.size(),0);//столбец из нулей
	
			//добавляем нули к уже существующим столбцам
			for(int i=0;i<size;i++)
				for(int j=0;j<diff;j++)
					Polynom_graph[i].push_back(0);
	
			//добавляем нулевые столбцы
			for(int i =0; i<diff;i++)
				Polynom_graph.push_back(null_column);
	
	
			if(size==unsaturated.size())//если после итерации новых многочленов не прибавилось, то выходим из цикла
				break;
			size=unsaturated.size();
		}	
		return unsaturated;
	}
	//функция принимает вектор многочленов и оставляет в нем только уникальные многочлены
	void uniquying(std::vector<Polynom>& unsat)
	{
		std::vector<Polynom> unique;
		for (int i =0;i<unsat.size();i++)
		{
			int k =0;
			for(int j=0;j<unique.size();j++)
			{
				if(unsat[i]==unique[j])
					k++;
			}
			if (k==0)
				unique.push_back(unsat[i]);
		}
		unsat=unique;
	}


	///Функция принимает вектор многочленов и сортирует его по возрастанию степени
	void degree_sort(std::vector<Polynom>& unsorted)
	{
		//обычная сортировка пузырьком
		Polynom temp(std::vector<mpz_class>{0});
		for(int i=0;i<unsorted.size();i++)
		{
			for(int j=0;j<unsorted.size()-i-1;j++)
			{	
				if (unsorted[j].get_degree()>unsorted[j+1].get_degree())
				{
					temp=unsorted[j];
					unsorted[j]=unsorted[j+1];
					unsorted[j+1]=temp;
				}
			}
		}
		
	}
	//Эта функция принимает на вход таблицу знаков и вектор многочленов, печатает таблицу знаков
	void row_print(std::vector<std::vector<int>> t,std::vector<Polynom>);
	
	//Эта функция ставит знаки на крайних интервалах, соответствующих +inf и -inf
	void inf_point(std::vector<std::vector<int>> &tars_table, std::vector<Polynom> &polynoms, int p);

	//Эта функция заполняет знаки в точках нового многочлена
	void set_sign_from_rem(std::vector<std::vector<int>> &tars_table, std::vector<Polynom> &polynoms,int p, int i);
	//Эта функция заполняет знаки в промежутках нового многочлена
	void set_sign_from_neighbor(std::vector<std::vector<int>> &tars_table, std::vector<Polynom> &polynoms, int p,int i);

	void add_new_roots(std::vector<std::vector<int>> &t, int i, std::vector<int> new_roots);

	void add_new_roots_last_row(std::vector<std::vector<int>> &t, std::vector<int> new_roots,int p);


	//Функция принимает отсортированный вектор многочленов без констант и возвращет таблицу знаков системы многочленов из вектора
	std::vector<std::vector<int>> tars_table(std::vector<Polynom> polynoms)
	{
		std::cout<<"tarskiy table initiated"<<std::endl;
		std::vector<std::vector<int>> t;//сама таблица;
		//столбцам соответствую многочлены, строкам - точки и интервалы вещественной прямой
	
		//вытащим первый многочлен(линейный) и сделаем для него столбец
		std::vector<int> first_column(3);//первый столбец
		std::vector<mpz_class> first_p_coef=polynoms[0].get_coefficients();
		mpz_class sign_mpz = first_p_coef[1]/abs(first_p_coef[1]);//нужно узнать знак при старшем коэффициенте
	
		int sign = sign_mpz.get_d();//приведение знака из целочисленного GMP в int
		first_column[0]=sign*(-1);//заполнение первых строк таблицы
		first_column[1]=0;
		first_column[2]=sign;
	
		t.push_back(first_column);//добавляем строку в таблицу
		row_print(t,polynoms);//печатаем первую строчку
	
		//Основной цикл для заполнения таблицы
		for(int p=1;p<polynoms.size();p++)
		{
			Polynom new_polynom = polynoms[p];
			std::vector<mpz_class> new_polynom_coef=new_polynom.get_coefficients();
			int p_size=new_polynom_coef.size();
	
			std::cout<<std::endl<<std::endl<<"NEW ITERATION"<<std::endl;
			std::cout<<"adding polynom : ";
			new_polynom.print();
			std::cout<<std::endl;
	
			std::vector<int> new_roots;//здесь будут хранится индексы корней многочлена
			std::vector<int> new_roots_temp;
			inf_point(t,polynoms,p);//Добавляем знаки в крайних промежутках на бесконечности
	
			//итерация по всем нечетным позициям, на них находятся корни многочлена
			for(int i=1;i<t[0].size();i+=2)
				set_sign_from_rem(t,polynoms,p,i);//Добавляем знаки по остаткам многочлена
	
	
			//остается заполнить четные ячейки - они отображают знаки на интервалах
			if(p!=1)					//самый первый многочлен смотреть не надо
				for(int i=1;i<t[0].size()-3;i+=2)	//итерируемся по интервалам
					set_sign_from_neighbor(t,polynoms,p,i);	//Добавляем знаки по соседям
	
	
	
			//Теперь нужно найти где у нового многочлена происходит смена знака -1|+1 или +1|-1
			//И добавить две новые ячейки между сменой знака, одна для нового корня 0, другая
			//для знака на интервале
			
			//1.Найдем места, где происходит смена знака в новом многочлене
			for(int i = 1;i<t[0].size();i++)
				if(t[p][i]==t[p][i-1]*(-1))
					new_roots.push_back(i-1);
			
			//2.Создадим новые строки во всех многочленах
			new_roots_temp=new_roots;	//копируем строку новых корней
			//Итерация по всем многочленам в таблице кроме последнего
			for(int i=0;i<t.size()-1;i++)
				add_new_roots(t,i,new_roots);
	
	
			//check if iteration was correct
			new_roots=new_roots_temp;
			std::cout<<"new roots are";
			if(new_roots.size()==0)
				std::cout<<"there is no new roots"<<std::endl;
			for(int i=0;i<new_roots.size();i++)
				std::cout<<new_roots[i]<<" ";
			std::cout<<std::endl;
			std::cout<<"table is now"<<std::endl;
	
			//3.Теперь изменение столбца последнего многочлена
			add_new_roots_last_row(t,new_roots,p);
	
			row_print(t,polynoms);//распечатаем таблицу под конец итерации
		}
		return t;
	}
	void add_new_roots_last_row(std::vector<std::vector<int>> &t, std::vector<int> new_roots, int p)
	{
		for(int j=0;j<new_roots.size();j++)
		{
			auto it=t[p].begin();
			it += new_roots[j]+1;
			int prev=t[p][new_roots[j]];
			int next=t[p][new_roots[j]+1];
			if((new_roots[j] % 2)==0)		//Случай интервал-точка => 0|+
				it=t[p].insert(it,{0,next});
			if((new_roots[j] % 2)==1)		//Случай точка-интервал => +|0
				it=t[p].insert(it,{prev,0});
			for(int k=j+1;k<new_roots.size();k++)
				new_roots[k]+=2;
		}
	}
	void add_new_roots(std::vector<std::vector<int>> &t, int i, std::vector<int> new_roots)
	{
		//Итерация по найденным корням
		for(int j=0;j<new_roots.size();j++)
		{
			auto it=t[i].begin();
			it += new_roots[j]+1;
			int prev=t[i][new_roots[j]];//Коэффициент слева
			int next=t[i][new_roots[j]+1];//Коэффициент справа
	
			if(prev!=0)		//если слева от корня стоит знак, то скопируем его
				it=t[i].insert(it,{prev,prev});
			if(prev==0)		//если слева от корня ноль, то
			{
				if(next!=0)	//если справа стоит знак, то скопируем его
					it=t[i].insert(it,{next,next});
				if(next==0)	//если справа стоит ноль, все заполним нулями
					it=t[i].insert(it,{0,0});
			}
								//было добавлено две новые строки
			for(int k=j+1;k<new_roots.size();k++)	//значит нужно сдвинуть коэффициенты на 2
				new_roots[k]+=2;
		}
	
	}

	void inf_point(std::vector<std::vector<int>> &t, std::vector<Polynom> &polynoms, int p)
	{
		Polynom new_polynom = polynoms[p];
		std::vector<mpz_class> new_polynom_coef=new_polynom.get_coefficients();
		int p_size=new_polynom_coef.size();
	
		std::vector<int> new_column(t[0].size());//создаем новый столбец
			for(int i=0;i<t[0].size();i++)//заполняем столбец маркерами в виде числа -1000
				new_column[i]=-1000;
	
		//заполнение крайних точек столбца-заменяем на функцию
		mpz_class last_element=new_polynom_coef[p_size-1];
		int sign_last_element;
		if(last_element > 0)
			sign_last_element = 1;
		if(last_element < 0)
			sign_last_element = -1;
		if(last_element == 0)
			sign_last_element = 0;
	
		if((new_polynom.get_degree() % 2)==0)		//если степень четная, то знаки на -inf и +inf равны знаку коэффициента
		{
			new_column[0]=sign_last_element;
			new_column[t[0].size()-1]=sign_last_element;
		}
		if((new_polynom.get_degree() % 2)==1)		//если степень нечетная, то на -inf знак меняется
		{				     		// а на +inf остается
			new_column[0]=sign_last_element*(-1);
			new_column[t[0].size()-1]=sign_last_element;
		}
	
		t.push_back(new_column);//добавляем многочлен в таблицу
	}

	void set_sign_from_rem(std::vector<std::vector<int>> &t, std::vector<Polynom> &polynoms,int p, int i)
	{
		int l = 0;
		while(t[l][i]!=0)
		{
			l++;
		}
	
		//Найдем какой вклад от знака получает старший коэффициент
		std::vector<mpz_class> divider = polynoms[l].get_coefficients();
		mpz_class h0 = divider[divider.size()-1];//Старший коэффициент
		double h=h0.get_d();
		if(h>0)
		{
			h=1;
		}
		if(h<0)
		{
			if((polynoms[p].get_degree()-polynoms[l].get_degree())%2==0)
				h=-1;
			if((polynoms[p].get_degree()-polynoms[l].get_degree())%2==1)
				h=1;
		}
		//печатаем многочлены
		polynoms[p].print();
		std::cout<<std::endl;
		polynoms[l].print();
		//находим остаток от деления многочленов
		Polynom rem = divide(polynoms[p],polynoms[l]).first;
		std::cout<<std::endl;
		std::cout<<"remainder is ";
		rem.print();
		std::cout<<std::endl;
		std::cout<<"remainder's degree is "<<rem.get_degree()<<std::endl;
	
		//если остаток это не константа, то ищем остаток в насыщенной системе
		if(rem.get_degree()>0)
		{
			for(int j=0;j<t.size();j++)	//итерация по всем многочленам
				if(polynoms[j]==rem)	//если новый многочлен совпадает с остатком
				{	
					std::cout<<"remainder of polynom found in the table"<<std::endl;
					std::cout<<"sign is "<<t[j][i]<<std::endl;
					t[p][i]=h*t[j][i];//копируем знак из остатка, нужно учесть знак при делении
				}
		}
		//если остаток это константа
		if(rem.get_degree()==0)
		{
			if(rem.get_coefficients()[0]==0)//если остаток ноль, то точка-корень многочлена
			{
				std::cout<<"Remainder is 0, point is root of polynom"<<std::endl;
				t[p][i]=0;
			}
			if(rem.get_coefficients()[0]!=0)//если остаток ненулевое число, то копируем его знак
			{
				std::cout<<"Remainder is "<<rem.get_coefficients()[0]<<std::endl;
				mpz_class remnant_coeff_sign=rem.get_coefficients()[0]/abs(rem.get_coefficients()[0]);
				mpz_class modified_elder_coeff_sign=remnant_coeff_sign*h;
				t[p][i]=(int)modified_elder_coeff_sign.get_d();
			}
		}
		std::cout<<std::endl;
	}

	//Может вызвать ошибки
	void set_sign_from_neighbor(std::vector<std::vector<int>> &t, std::vector<Polynom> &polynoms, int p,int i)
	{
		//заполняем знаки интервала в зависимости от знаков соседей
		if(t[p][i]==-1000)//для отлова ошибок	
		{
			t[p][i+1]=-2000;
//			return 0;
		}
		if(t[p][i]!=0)
		{
			t[p][i+1]=t[p][i];
//			return 0;
		}
		if(t[p][i]==0)
		{
			t[p][i+1]=t[p][i+2];
//			return 0;
		}
		if((t[p][i]==0)&&(t[p][i+2]==0))
		{
			t[p][i+1]=0;
//			return 0;
		}

	}

	void row_print(std::vector<std::vector<int>> t,std::vector<Polynom> polynoms)
	{
		std::cout<<"Printing talbe..."<<std::endl;
		for(int j=0;j<t.size();j++)
		{
			std::cout<<"__________"<<std::endl;
			std::cout<<"["<<j<<"] = ";
			polynoms[j].print();
			std::cout<<" |";
			for(int i=0;i<t[j].size();i++)
				std::cout<<t[j][i]<<" |";
			std::cout<<std::endl;
			std::cout<<"__________"<<std::endl;
		}
		std::cout<<"Table ended..."<<std::endl;
	}
}
