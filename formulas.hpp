#pragma once

#include<vector>
#include<tuple>
#include"tarsky.hpp"
//should modify negative modifier
//should become virtual

namespace Kirill
{
	class Predicate
	{
	private:
		Polynom _polynom;
		bool _value{0};
		bool _negative;
	public:
		Predicate(Polynom polynom,bool negative);
		Predicate();
		Polynom get_polynom();
		virtual	bool calculate()=0;
		double polynom_in_point();
		bool get_value();
		void set_value(bool);
		bool get_negative();
		virtual ~Predicate()=0;
	};
	
	class Equality_predicate : public Predicate
	{
	public:
		Equality_predicate(Polynom polynom,bool negative);
		bool calculate() override;
		~Equality_predicate();	
	};

	class Greater_predicate : public Predicate
	{
	public:
		Greater_predicate(Polynom polynom, bool negative);
		bool calculate() override;
		~Greater_predicate();
	};

	class Conjunct
	{
	private:
		std::vector<Predicate*> _predicats;
	public:
		Conjunct(std::vector<Predicate*> predicats);
		Conjunct();
		void clear();
		bool calculate();
		void add_predicate(Predicate *);
		std::vector<Predicate*> getPredicates(); //для Димасика
	};

	class DNF
	{
	private:
		std::vector<Conjunct> _conjuncts;
	public:
		DNF(std::vector<Conjunct> conjuncts);
		DNF();
		bool calculate();
		void add_conjunct(Conjunct);
		void printPolynoms(); //для Димасика
	};
}
