#include<vector>
#include<tuple>
#include"Tarskiy.hpp"

class Predicate
{
private:
	Polynom _polynom;
	bool _value;
	bool _negative;
public:
	Predicate(Polynom polynom,bool negative);
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
	bool calculate();
	~Equality_predicate();	
};

class Lesser_predicate : public Predicate
{
public:
	Lesser_predicate(Polynom polynom, bool negative);
	bool calculate();
	~Lesser_predicate();
};

class Conjunct
{
private:
	std::vector<Predicate*> _predicats;
public:
	Conjunct(std::vector<Predicate*> predicats);
	bool calculate();
	void add_predicate(Predicate *);
};

class DNF
{
private:
	std::vector<Conjunct> _conjuncts;
public:
	DNF(std::vector<Conjunct> conjuncts);
	bool calculate();
	void add_conjunct(Conjunct);
};
