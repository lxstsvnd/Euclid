#include<iostream>
#include<vector>
#include<set>
#include<algorithm>
#include<math.h>
#include"Tarskiy.hpp"

//В этом файле описаны функции для работы с алгоритмом Тарского


//Функция принимает некоторый вектор многочленов, возвращает насыщенную 
//относительно взятия производной и взятия остатка от деления систему многочленов
std::vector<Polynom> full_saturation(std::vector<Polynom> unsaturated)
{
//Насыщаем относительно взятия остатка и взятия производной, пока насыщение не перестанет добавлять новых многочленов
	int size=unsaturated.size();
	while(1)
	{
		unsaturated=derivation_saturation(unsaturated);
		unsaturated=mod_saturation(unsaturated);
		if (unsaturated.size()==size)
			break;
		size=unsaturated.size();

	}
		for(int i=0;i<unsaturated.size();i++)
			if(unsaturated[i].get_degree()==0)
				unsaturated.erase(unsaturated.begin()+i);
	
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

//Функция принимает некоторый вектор многочленов и возращает относительно взятия остатков систему многочленов
std::vector<Polynom> mod_saturation(std::vector<Polynom> unsaturated)
{
	Polynom C=unsaturated[0];
	std::vector<Polynom> raw_polynoms;
	int size=unsaturated.size();

	std::vector<std::vector<int>> Polynom_graph;//Матрица делимостия
	//в i-м столбце и j-м строке находится маркер, если было произведено
	//деление i-го многочлена в массиве на j-й многочлен в массиве
	//с учетом того что степень первого не меньше степени второго
	
	std::vector<int> null_column(size,0);//столбец из нулей

	for(int i=0;i<size;i++)
		Polynom_graph.push_back(null_column);
	
	//Непосредственное замыкание
	while (1)
	{
		//деление многочлена на самого себя не имеет смысла
		for(int i=0;i<Polynom_graph.size();i++)
			Polynom_graph[i][i]=1;
			
		//непосредственное насыщение
		for(int i=0;i<Polynom_graph.size();i++)
		{
			for(int j =0;j<Polynom_graph.size();j++)
			{
				if(Polynom_graph[i][j]==0)//если мы еще не пытались делить i многочлен на j
				{	
					if (unsaturated[i].get_degree()>=unsaturated[j].get_degree())
					{
//						if(unsaturated[i].get_degree()==0)
//						{
//							Polynom_graph[i][j]=-2;
//							Polynom_graph[j][i]=-2;
//							continue;
//						}
//						//Если степень первого больше степени второго, то поставим пометки в массиве
						Polynom_graph[i][j]=1;
						Polynom_graph[j][i]=-1;

						C=divide(unsaturated[i],unsaturated[j]).first;
						if(C.get_degree()>0)
						{
							raw_polynoms.push_back(C);
							//так-же нужно добавить отрицание многочлена для деления j на i
							std::vector<mpz_class> negative_c=C.get_coefficients();
							for(int k=0;k<negative_c.size();k++)
								negative_c[k]=-negative_c[k];
							raw_polynoms.push_back(Polynom(negative_c));

						}
//						if(unsaturated[i].get_degree()==unsaturated[j].get_degree())
//							C=C.divide(unsaturated[j],unsaturated[i]).first;
					}
					else
					{
						Polynom_graph[i][j]=-1;
						Polynom_graph[j][i]=1;
						C=divide(unsaturated[j],unsaturated[i]).first;
						if(C.get_degree()>0)
						{
							raw_polynoms.push_back(C);
							//так-же нужно добавить отрицание многочлена для деления i на j
							std::vector<mpz_class> negative_c=C.get_coefficients();
							for(int k=0;k<negative_c.size();k++)
								negative_c[k]=-negative_c[k];
							raw_polynoms.push_back(Polynom(negative_c));
						}
					}
				}			
			}
		}

		for(int i =0;i<raw_polynoms.size();i++)
			unsaturated.push_back(raw_polynoms[i]);
		uniquying(unsaturated);//оставляем только уникальные
		std::cout<<"Uniquyed"<<std::endl;

		//удаляем константы
		for(int i=0;i<unsaturated.size();i++)
			if(unsaturated[i].get_degree()==0)
				unsaturated.erase(unsaturated.begin()+i);

		//Расширяем граф добавляя места для маркеров деления новых многочленов
		double diff=unsaturated.size()-size;//разница в размере графа
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
//кажется эту функцию надо удалить
//void range(std::vector<Polynom>& unranged)
//{
//	std::vector<Polynom> ranged;
//
//	Polynom Max=unranged[0];
//	for(int i =1; i<unranged.size();i++)
//			if (unranged[i].get_degree()>Max.get_degree())
//				Max=unranged[i];
//	for(int i =0;i<Max.get_degree()+1;i++)
//		for(int j=0;j<unranged.size();j++)
//			if(unranged[j].get_degree()==i)
//				ranged.push_back(unranged[i]);
//	unranged=ranged;
//}
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

//Функция принимает отсортированный вектор многочленов без констант и возвращет таблицу знаков системы многочленов из вектора
std::vector<std::vector<int>> tars_table(std::vector<Polynom> polynoms)
{
	std::cout<<"tarskiy table initiated"<<std::endl;
	std::vector<std::vector<int>> t;//сама таблица;
	//столбцам соответствую многочлены, строкам - точки вещественной прямой

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
		std::cout<<std::endl<<std::endl<<"NEW ITERATION"<<std::endl;
		std::cout<<"adding polynom : ";
		polynoms[p].print();
		std::cout<<std::endl;

		std::vector<int> new_roots;//здесь будут хранится индексы корней многочлена
		std::vector<int> new_roots_temp;
		std::vector<int> new_column(t[0].size());//создаем новый столбец
		for(int i=0;i<t[0].size();i++)//заполняем столбец маркерами в виде числа -1000
			new_column[i]=-1000;

		//заполнение крайних точек столбца
		std::vector<mpz_class> p_coef=polynoms[p].get_coefficients();
		mpz_class sign_mpz=p_coef[p_coef.size()-1]/abs(p_coef[p_coef.size()-1]);//нужно узнать знак при старшец коэффициенте
		if((polynoms[p].get_degree() % 2)==0)		//если степень четная, то знаки на -inf и +inf равны знаку коэффициента
		{
			new_column[0]=sign_mpz.get_d();
			new_column[t[0].size()-1]=sign_mpz.get_d();
		}
		if((polynoms[p].get_degree() % 2)==1)		//если степень нечетная, то на -inf знак меняется
		{				     		// а на +inf остается
			new_column[0]=sign_mpz.get_d()*(-1);
			new_column[t[0].size()-1]=sign_mpz.get_d();
		}

		t.push_back(new_column);//добавляем многочлен в таблицу

		//итерация по всем нечетным позициям, на них находятся корни многочлена
		for(int i=1;i<t[0].size();i+=2)
		{
			int sign=-1000;
			int l=0;
			std::vector<mpz_class> divider_coefficients{-1000};
			mpz_class divider_elder_coefficient=0;
			mpz_class divider_coeff_sign=-1000;
			mpz_class divider_coeff_sign_old=-1000;
			//нужно найти столбец многочлена где в i-й позиции стоит нуль
			while(t[l][i]!=0)
			{
				l++;
			}
			//Нужно получить знак старшего коэффициента многочлена, возведенного в степень
			divider_coefficients=polynoms[l].get_coefficients();
			divider_elder_coefficient=divider_coefficients[divider_coefficients.size()-1];
			divider_coeff_sign=divider_elder_coefficient/abs(divider_elder_coefficient);
			divider_coeff_sign_old=divider_coeff_sign;
			//непосредственно возведение в степень
			for(int j=0;j<polynoms[p].get_degree()-polynoms[l].get_degree();j++)
				divider_coeff_sign*=divider_coeff_sign_old;

			//Найдем какой вклад от знака получает старший коэффициент
			std::vector<mpz_class> divider = polynoms[l].get_coefficients();
			mpz_class h0 = divider[divider.size()-1];//Старший коэффициент
			double h=h0.get_d();
			if(h==0)
				std::cout<<"ERRORERRORERROEERROE"<<std::endl;
			if(h>0)
			{
				std::cout<<"ZEROED"<<std::endl;
				h=1;
			}
			if(h<0)
			{
				std::cout<<"ERIUJGTEJGITEJG"<<std::endl;
//				std::cout<<"D of degrees is "<< (polynoms[p].get_degree()-polynoms[l].get_degree())%2<<std::endl;
				if((polynoms[p].get_degree()-polynoms[l].get_degree())%2==0)
				{
					std::cout<<"First"<<std::endl;
					h=-1;
				}
				if((polynoms[p].get_degree()-polynoms[l].get_degree())%2==1)
				{
					std::cout<<"Second"<<std::endl;
					h=1;
				}
				if(polynoms[p].get_degree()==polynoms[l].get_degree())
				{
					std::cout<<"Third"<<std::endl;
					h=-1;
				}
			}
			std::cout<<"H IS "<<h<<std::endl;
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
					mpz_class modified_elder_coeff_sign=remnant_coeff_sign*divider_coeff_sign;
					t[p][i]=(int)modified_elder_coeff_sign.get_d();
				}
			}
			std::cout<<std::endl;
		}

		//остается заполнить четные ячейки - они отображают знаки на интервалах
		if(p!=1)					//самый первый многочлен смотреть не надо
			for(int i=1;i<t[0].size()-3;i+=2)	//итерируемся по интервалам
			{					//заполняем знаки интервала
				if(t[p][i]==-1000)		//в зависимости от знаков соседей
				{
					t[p][i+1]=-2000;
					continue;
				}
				if(t[p][i]!=0)
				{
					t[p][i+1]=t[p][i];
					continue;
				}
				if(t[p][i]==0)
				{
					t[p][i+1]=t[p][i+2];
					continue;
				}
				if(t[p][i+2]==0)
				{
					t[p][i+1]=0;
					continue;
				}
			}

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
			//смещаем корни обратно, чтобы обработать следующий многочлен
			new_roots=new_roots_temp;
			
		}
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

		row_print(t,polynoms);//распечатаем таблицу под конец итерации
	}
	return t;
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
