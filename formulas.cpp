#include<vector>
#include<tuple>
#include"formulas.hpp"
#include<iostream>
#include<math.h>

namespace Kirill
{
	//Конструктор предиката - получает многочлен и negative=1, если перед предикатом стоит отрицание
	//и negative=0, если перед предикатом не стоит отрицания
	Predicate::Predicate(Polynom polynom, bool negative):_polynom(polynom),_negative(negative){}
	Predicate::Predicate(){;}
	Predicate::~Predicate(){}

	//Возвращает многочлен, находящийся в предикате 
	Polynom Predicate::get_polynom()
	{
		return _polynom;
	}

	//Возвращает да, если перед предикатом стоит отрицание, нет иначе
	bool Predicate::get_negative()
	{
		return _negative;
	}
	
	//Возвращает да, если предикат принимает истину, нет иначе
	bool Predicate::get_value()
	{
		return _value;
	}

	//Задает, является ли предикат истинным или нет
	void Predicate::set_value(bool value)
	{
		_value=value;
	}

	//Конструктор для предиката равенства
	Equality_predicate::Equality_predicate(Polynom polynom,bool negative) : Predicate(polynom,negative){}
	//Деструктор для предиката равенства
	Equality_predicate::~Equality_predicate(){}
	bool Equality_predicate::calculate(){return true;}
	
	//Конструктор для предиката сравнения
	Greater_predicate::Greater_predicate(Polynom polynom, bool negative) : Predicate(polynom,negative){}
	//Деструктор для предиката сравенния
	Greater_predicate::~Greater_predicate(){}
	bool Greater_predicate::calculate(){return true;}

	//Конструктор конъюнкта - получает вектор, состоящий из предикатов
	Conjunct::Conjunct(std::vector<Predicate*> predicats): _predicats(predicats) {}
	Conjunct::Conjunct(){;}
	
	//Вычисляет значение конъюнкта в зависимости от значения предикатов внутри
	bool Conjunct::calculate()
	{
		for (auto predicate : _predicats)
			if (predicate->get_value()==false)
				return false;
		return true;
	}

	//Добавляет предикат в конъюнкт
	void Conjunct::add_predicate(Predicate * predicate)
	{
		_predicats.push_back(predicate);
	}

	//Конструктор ДНФ - получает вектор конъюнктов
	DNF::DNF(std::vector<Conjunct> conjuncts) : _conjuncts(conjuncts){}
	
	//Вычисляет значение ДНФ в зависимости от значения конъюнктов внутри
	bool DNF::calculate()
	{
		bool result = false;
		for (auto  conjunct: _conjuncts)
			if (conjunct.calculate()==true)
				return true;
		return false;
	}

	//Добавляет коъюнкт в предикат
	void DNF::add_conjunct(Conjunct conjunct)
	{
		_conjuncts.push_back(conjunct);
	}
}
