#include<vector>
#include<tuple>
class Polynom
{
private:
	std::vector<double> _coefficients;
public:
	std::vector<double> get_coefficients();
	Polynom(std::vector<double>);
	Polynom get_derivative();
	void print();
	double get_value(double);
	int get_degree();
	friend bool operator==(Polynom,Polynom);
//	std::pair<Polynom,Polynom> ug_divide(Polynom,Polynom);
	std::pair<Polynom,Polynom> divide(Polynom,Polynom);
};

//should modify negative modifier
//should become virtual
class Predicate
{
private:
	Polynom _polynom;
	double _point;
	bool _constant;
	bool _negative;
public:
	Predicate(double constant, double point, Polynom polynom,bool negative);
	Polynom get_polynom();
	double get_point();
	double get_constant();
	virtual	bool calculate()=0;
	double polynom_in_point();
	bool get_negative();
	virtual ~Predicate()=0;
};

class Equality_predicate : public Predicate
{
public:
	Equality_predicate(double constant, double point, Polynom polynom,bool negative);
	bool calculate();
	~Equality_predicate();	
};

class Lesser_predicate : public Predicate
{
public:
	Lesser_predicate(double constant, double point, Polynom polynom, bool negative);
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
