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
	Equality_predicate::Equality_predicate(){}
	//Деструктор для предиката равенства
	Equality_predicate::~Equality_predicate(){}
	bool Equality_predicate::calculate(std::vector<int>& column, std::vector<Polynom> &DNF_polynoms)
	{
	//Ищем индекс под которым многочлен предиката лежит в векторе
		int index;	
		for(int i = 0; i < column.size(); i++)
			if(DNF_polynoms[i]==this->get_polynom())
				index = i;

	//Пишем ответ
		bool answer;
		if(column[index]==0)
			answer=true;
		else
			answer=false;

		return (answer!=this->get_negative());

	}
	
	//Конструктор для предиката сравнения
	Greater_predicate::Greater_predicate(Polynom polynom, bool negative) : Predicate(polynom,negative){}
	Greater_predicate::Greater_predicate(){}
	//Деструктор для предиката сравенния
	Greater_predicate::~Greater_predicate(){}
	bool Greater_predicate::calculate(std::vector<int>& column, std::vector<Polynom> &DNF_polynoms)
	{
	//Ищем индекс под которым многочлен предиката лежит в векторе
		int index;	
		for(int i = 0; i < column.size(); i++)
			if(DNF_polynoms[i]==this->get_polynom())
				index = i;
		
	//Пишем ответ
		bool answer;
		if(column[index]>0)
			answer=true;
		else
			answer=false;

		//debug line
//		for(auto polynom : DNF_polynoms)
//			polynom.print();
		for(auto i : column)
			std::cout<<i<<" ";
		DNF_polynoms[0].print();
		this->get_polynom().print();
		std::cout<<"Index is"<<index<<std::endl;
		std::cout<<"answer is "<<answer<<std::endl;
		std::cout<<"negative is"<<this->get_negative()<<std::endl;
		std::cout<<"#######################"<<std::endl;
		//debug line

		return (answer!=this->get_negative());
	}

	//Конструктор конъюнкта - получает вектор, состоящий из предикатов
	Conjunct::Conjunct(std::vector<Predicate*> predicats): _predicats(predicats) {}
	Conjunct::Conjunct(){;}
	void Conjunct::clear(){_predicats.clear();}
	
	//Вычисляет значение конъюнкта в зависимости от значения предикатов внутри
	bool Conjunct::calculate(std::vector<int>& column, std::vector<Polynom> &DNF_polynoms)
	{
		for (auto predicate : _predicats)
			if (predicate->calculate(column, DNF_polynoms)==false)
				return false;
		return true;
	}

	//Добавляет предикат в конъюнкт
	void Conjunct::add_predicate(Predicate * predicate)
	{
		_predicats.push_back(predicate);
	}

	std::vector<Predicate*> Conjunct::getPredicates(){return _predicats;}
	Conjunct::~Conjunct(){}


	//Конструктор ДНФ - получает вектор конъюнктов
	DNF::DNF(std::vector<Conjunct> conjuncts) : _conjuncts(conjuncts){}
	DNF::DNF(){;}

	//Вычисляет значение ДНФ в зависимости от значения конъюнктов внутри
	bool DNF::calculate(std::vector<int>& column, std::vector<Polynom> &DNF_polynoms)
	{
		bool result = false;
		for (auto  conjunct: _conjuncts)
			if (conjunct.calculate(column, DNF_polynoms)==true)
				return true;
		return false;
	}

	bool DNF::decide()
	{
		std::vector<Polynom> DNF_polynoms;	//Здесь лежат все многочлены из ДНФ
		std::vector<std::vector<int>> table;	//Сюда будет записана таблица Тарского

		std::cout<<"alive"<<std::endl;

		this->printPolynoms();

		for(Conjunct curConjunct : _conjuncts)
		{
			std::cout<<"alive1"<<std::endl;
			for(Predicate* curPredicate : curConjunct.getPredicates())
			{
				curPredicate->get_polynom().print();
				std::cout<<"alive2"<<std::endl;
				DNF_polynoms.push_back(curPredicate->get_polynom());
			}
		}

		std::cout<<"alive"<<std::endl;



		uniquying(DNF_polynoms);		//Оставим только уникальные многочлены
		degree_sort(DNF_polynoms);		//Отсортируем многочлены в соотв. с таблицей
		table=get_format_table(DNF_polynoms);	
		int table_wideness=table[0].size();	//Ширина таблицы

		//debug line
		std::cout<<"Printing extracted table:"<<std::endl;
		for(auto polynom_string : table)
		{
			for(auto integer : polynom_string)
				std::cout<<integer<<" ";
			std::cout<<std::endl;
		}	
		std::cout<<"Table printed"<<std::endl;
		//debug line

		//Итерация по всем точкам и промежуткам
		for(int i = 0; i < table_wideness; i++)
		{
			//Получаем срез знаков для данной точки
			std::vector<int> column(table.size());
			for(int j = 0; j < table.size(); j++)
				column[j]=table[j][i];
			//Вычислим значение ДНФ для этого вектора
			if(this->calculate(column,DNF_polynoms)==true)
				return true;
		}
		return false;
	}
	//Добавляет коъюнкт в предикат
	void DNF::add_conjunct(Conjunct conjunct)
	{
		_conjuncts.push_back(conjunct);
	}

	//Печатает многочлены
	void DNF::printPolynoms()
	{
		for(Conjunct curConjunct : _conjuncts)
		{
			for(Predicate* curPredicate : curConjunct.getPredicates())
			{
				curPredicate->get_polynom().print();
			}
		}
	}
	DNF::~DNF(){}
}
