#include<vector>
#include<tuple>
#include"Formulas.hpp"
#include<iostream>
#include<math.h>

Predicate::Predicate(Polynom polynom, bool negative):_polynom(polynom),_negative(negative){}
Predicate::~Predicate(){}

Polynom Predicate::get_polynom()
{
	return _polynom;
}

bool Predicate::get_negative()
{
	return _negative;
}

bool Predicate::get_value()
{
	return _value;
}

void Predicate::set_value(bool value)
{
	_value=value;
}

Equality_predicate::Equality_predicate(Polynom polynom,bool negative) : Predicate(polynom,negative){}
Equality_predicate::~Equality_predicate(){}

Lesser_predicate::Lesser_predicate(Polynom polynom, bool negative) : Predicate(polynom,negative){}
Lesser_predicate::~Lesser_predicate(){}

Conjunct::Conjunct(std::vector<Predicate*> predicats): _predicats(predicats) {}

bool Conjunct::calculate()
{
	for (auto predicate : _predicats)
		if (predicate->get_value()==false)
			return false;
	return true;
}

void Conjunct::add_predicate(Predicate * predicate)
{
	_predicats.push_back(predicate);
}

DNF::DNF(std::vector<Conjunct> conjuncts) : _conjuncts(conjuncts){}
bool DNF::calculate()
{
	bool result = false;
	for (auto  conjunct: _conjuncts)
		if (conjunct.calculate()==true)
			return true;
	return false;
}
void DNF::add_conjunct(Conjunct conjunct)
{
	_conjuncts.push_back(conjunct);
}
