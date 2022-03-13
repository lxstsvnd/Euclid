#include<vector>
#include<tuple>
#include"Formulas.hpp"
#include<iostream>
#include<math.h>

Polynom::Polynom(std::vector<double> coefficients): _coefficients(coefficients){}

std::vector<double> Polynom::get_coefficients()
{
	return _coefficients;
}

void Polynom::print()
{
	int size=_coefficients.size();
	for (int i = 0; i < size-1;i++)
		std::cout<<_coefficients[i]<<"x^"<<i<<"+";
	std::cout<<_coefficients[size-1]<<"x^"<<size-1<<std::endl;
}

double Polynom::get_value(double point_x)
{
	double sum=0;
	double monom=1;
	for (auto coefficient : _coefficients)
	{
		sum+=coefficient*monom;
		monom*=point_x;
	}
	return sum;
}	

Polynom Polynom::get_derivative()
{
	if (this->get_degree()==0)
		return Polynom(std::vector<double>{0});
	std::vector<double> derivative_coefficients;
	for (int i =1; i<_coefficients.size(); i++)
	{
		derivative_coefficients.push_back(_coefficients[i]*i);
	}
	return Polynom(derivative_coefficients);
}


int Polynom::get_degree()
{
	return _coefficients.size()-1;
}

bool operator ==(Polynom lhs, Polynom rhs)
{
	std::vector<double> lhs_coefficients=lhs.get_coefficients();
	std::vector<double> rhs_coefficients=rhs.get_coefficients();
	if (lhs.get_degree() != rhs.get_degree())
		return false;
	for(int i =0 ; i<=lhs.get_degree();i++)
	{
		if(lhs_coefficients[i]!=rhs_coefficients[i])
		{
			return false;
		}
	}
	return true;
}

std::pair<Polynom,Polynom> Polynom::divide(Polynom Pdivident,Polynom Pdivider)
{
	std::vector<double> divident = Pdivident.get_coefficients();
	std::vector<double> divider = Pdivider.get_coefficients();
	std::vector<double> remainder=divident;
	std::vector<double> quotient;

	if (divider.size()==1)
	{
 		std::pair<Polynom, Polynom> result{Polynom(std::vector<double>{0}), Polynom(divident)	};
		return result;
	}

	//check if degree of divider is bigger then degree of divident
	if (Pdivider.get_degree() > Pdivident.get_degree())
		std::cout<<"degree of divider is bigger then degree of divident"<<std::endl;

	std::cout<<remainder.size()<<" "<<divider.size()<<std::endl;
	while (remainder.size()>=divider.size())
	{
		//deleting elder zeros if they exists		
		if (remainder.back()==0)
		{
			remainder.pop_back();
			continue;
		}

		std::vector<double> deduct(remainder.size(),0);
		std::vector<double> temp_quotient;
		double q = remainder.back()/divider.back();

		//adding coefficient into remainder
		std::cout<<"q="<<remainder.back()<<"/"<<divider.back()<<std::endl;
		quotient.push_back(q);

		int size_dif=remainder.size()-divider.size();

		//calculating deduction vector
		for(int i=0;i<remainder.size();i++)
		{
			if (i>=size_dif)
				deduct[i]=q*divider[i-size_dif];
			if (i<size_dif)
				deduct[i]=0;
		}

		//checking deduction 
		std::cout<<"deduct:"<<std::endl;
		for (int i =0;i<deduct.size();i++)
			std::cout<<i<<":"<<deduct[i]<<std::endl;

		//deduction itself
		std::cout<<"resultnig:"<<std::endl;
		std::cout<<remainder.size()<<std::endl;
		for(int i =0; i<remainder.size();i++)
		{
			temp_quotient.push_back(remainder[i]-deduct[i]);
			std::cout<<i<<":"<<temp_quotient[i]<<std::endl;
		}
		
		//looping
		remainder=temp_quotient;
	}
	//need to reverse quotient vector
	std::vector<double> reversed_quotient;
//	quotient.pop_back();
	for (int i=quotient.size()-1;i>=0;i--)
		reversed_quotient.push_back(quotient[i]);
	//popping additional zero from quotient


	quotient=reversed_quotient;
	//making coefficients round
	for (int i =0;i<quotient.size();i++)
		quotient[i]=floor(quotient[i]*1000)/1000;
	for (int i =0;i<remainder.size();i++)
		remainder[i]=floor(remainder[i]*1000)/1000;
	std::pair<Polynom, Polynom> result{Polynom(remainder),Polynom(quotient)};

	return result;
}

Predicate::Predicate(double constant, double point, Polynom polynom, bool negative):_constant(constant),_point(point),_polynom(polynom),_negative(negative){}
Predicate::~Predicate(){}
Polynom Predicate::get_polynom()
{
	return _polynom;
}
double Predicate::get_point()
{
	return _point;
}
double Predicate::get_constant()
{
	return _constant;
}
double Predicate::polynom_in_point()
{
	return this->get_polynom().get_value(this->get_point());
}

bool Predicate::get_negative()
{
	return _negative;
}

Equality_predicate::Equality_predicate(double constant, double point, Polynom polynom,bool negative) : Predicate(constant,point,polynom,negative){}
Equality_predicate::~Equality_predicate(){}

bool Equality_predicate::calculate()
{
	bool negative=this->get_negative();
	if (this->get_constant()==this->polynom_in_point())
		return true*(!negative);
	return !(true*(!negative));
}

Lesser_predicate::Lesser_predicate(double constant, double point, Polynom polynom, bool negative) : Predicate(constant,point,polynom,negative){}
Lesser_predicate::~Lesser_predicate(){}

bool Lesser_predicate::calculate()
{
	bool negative=this->get_negative();
	//if polynom leq then constant
	if (this->polynom_in_point()>=this->get_constant())
		return true*(!negative);
	return !(false*(!negative));
}

Conjunct::Conjunct(std::vector<Predicate*> predicats): _predicats(predicats) {}

bool Conjunct::calculate()
{
	for (auto predicate : _predicats)
	{
		if (predicate->calculate()==false)
			return false;
	}
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
	for (auto  predicate: _conjuncts)
		if (predicate.calculate()==true)
			return true;
	return false;
}
void DNF::add_conjunct(Conjunct conjunct)
{
	_conjuncts.push_back(conjunct);
}



