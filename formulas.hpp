#pragma once

#include<vector>
#include<tuple>
#include"tarsky.hpp"

//В этом заголовочном файле лежит реализация классов для работы с логикой
//1.Класс предиката
//2.Его потомки для выражения предиката равенсвта и предиката больше
//3.Класс конъюнкта
//4.Класс ДНФ

namespace Kirill
{
	class Predicate	//Виртуальный класс для описания логических предикатов, сами логические предикаты реализуются наследниками
	{		//Внутри используется класс Polynom для описания многочленов
	private:
		Polynom _polynom;	//Многочлен хранится в виде класса многочлен
		bool _value;		//Логическое значение предиката
		bool _negative;		//Логическое значение равное 1 если перед предикатом стоит отрицание, 0 иначе
	public:
		Predicate(Polynom polynom,bool negative);	//Конструктор получает на вход многочлен и флаг отрицания
		Predicate();
		Polynom get_polynom();		//Функция возвращает многочлен из предиката
		virtual	bool calculate()=0;	//Функция вычисляет логическое значение предиката
		double polynom_in_point();	//Функция возвращает значение многочлена в точке; надо перенести в класс многочленов
		bool get_value();		//Геттер логического значения предиката
		void set_value(bool);		//Сеттер логического значения предиката
		bool get_negative();		//Геттер флага отрицания
		virtual ~Predicate()=0;		
	};
	
	//Потомок класса предикатов, используется для предикатов сравнения
	class Equality_predicate : public Predicate
	{
	public:
		Equality_predicate(Polynom polynom, bool negative);
		bool calculate() override;//пока не реализована
		~Equality_predicate();	
	};

	//Потомок класса предикатов, используется для предиката неравенства
	class Greater_predicate : public Predicate
	{
	public:
		Greater_predicate(Polynom polynom, bool negative);
		bool calculate() override;//пока не реализована
		~Greater_predicate();
	};

	//Класс реализует логическую конъюнкцию предикатов
	class Conjunct
	{
	private:
		//Класс хранит в себе вектор указателей на предикаты
		std::vector<Predicate*> _predicats;
	public:
		Conjunct(std::vector<Predicate*> predicats);	//Конструктор принимает вектор укзателей на предикаты
		Conjunct();			
		bool calculate();				//Функция, вычисляющая значения конъюнкта
		void add_predicate(Predicate *);		//Функция принимает указатель на предикат 
								//и добавляет его в вектор
	};

	//Класс реализует логическую работу ДНФ
	class DNF
	{
	private:
		//Класс хранит в себе вектор конъюнктов
		std::vector<Conjunct> _conjuncts; 
	public:
		DNF(std::vector<Conjunct> conjuncts);	//Конструктор принимает вектор конъюнктов
		bool calculate();			//Функция, вычисляющая значение ДНФ
		void add_conjunct(Conjunct);		//Функция принимает указатель на предикат
							//и добавляет его в в вектор
	};
}
