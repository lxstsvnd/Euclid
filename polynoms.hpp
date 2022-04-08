#pragma once

#include<vector>
#include<tuple>
#include<gmpxx.h>

//В этом заголовочном файле объявляются функции для работы с многочленами
//В многочлене находится вектор коэффициентов типа mpz_class
//mpz_class - длинные числа целой арифметики из пакета GMP
//В векторе коэффициенты располагаются в порядке возрастания степени

namespace Kirill
{
	//Класс дял реализации многочлена
	class Polynom
	{
	private:
		std::vector<mpz_class> _coefficients;		//Вектор коэффициентов
		double _point;					//Точка для вычисления значения многочлена
	public:	
		Polynom();					
		Polynom(std::vector<mpz_class>);		//Конструктор принимает в себя вектор коэффициентов
	
		void add_coefficient(mpz_class coefficient);	//Функция принимает число и добавляет его как верхний коэффициент
		std::vector<mpz_class> get_coefficients();	//Геттер веткора коэффициентов
		Polynom get_derivative();			//Функция возвращает многочлен, являющийся производным от данного
		void print();					//Функция печатает многочлен
		int get_degree();				//Функция возвращает степень многочлена
		void set_point(double);				//Сеттер точки
		double get_point();				//Геттер точки
		void delete_zeros();				//Функция удаляет старшие нули при условии что многочлен не const
		friend bool operator ==(Polynom,Polynom);	//Возвращает true, если многочлены покоэффициентно равны
								//Возвращает false иначе
		friend std::pair<Polynom,Polynom> divide(Polynom,Polynom);
		//Функция выполняет деление многочленов, возвращет std::pair, где
		//Первый элемент пары это остаток
		//Второй это частное
	};
}
