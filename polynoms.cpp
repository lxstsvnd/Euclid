#include"polynoms.hpp"
#include<iostream>
//need to delete elder zeros
Polynom::Polynom(std::vector<mpz_class> coefficients): _coefficients(coefficients){}

std::vector<mpz_class> Polynom::get_coefficients()
{
	return _coefficients;
}

void Polynom::print()
{
	int size=_coefficients.size();
	for(int i = 0; i < size;i++)
	{
		std::cout<<_coefficients[i]<<"x^"<<i<<"+";
	}
}

int Polynom::get_degree()
{
	return _coefficients.size()-1;
}

void Polynom::delete_zeros()
{
	int i = _coefficients.size()-1;
	while(_coefficients[i--]==0)
	{
		if(_coefficients.size()!=1)
			_coefficients.pop_back();
		else
			break;
	}
}

Polynom Polynom::get_derivative()
{
	if(this->get_degree()==0)
		return Polynom(std::vector<mpz_class>{0});
	std::vector<mpz_class> derivative_coefficients;
	for (int i = 1; i < _coefficients.size() ; i++)
	{
		derivative_coefficients.push_back(_coefficients[i]*i);
	}
	return Polynom(derivative_coefficients);
}

Polynom Polynom::delete_elder_coefficient()
{
	std::vector<mpz_class> coefficients=_coefficients;
	coefficients.pop_back();
	return Polynom(coefficients);
}	

Polynom Polynom::remain_elder_coefficient()
{
	std::vector<mpz_class> coefficients(_coefficients.size());
	for(int i = 0;i<_coefficients.size()-1;i++)
		coefficients[i]=0;
	coefficients[coefficients.size()-1]=_coefficients[coefficients.size()-1];
	return Polynom(coefficients);
}

bool operator ==(Polynom lhs, Polynom rhs)
{
	std::vector<mpz_class> lhs_coefficients=lhs.get_coefficients();
	std::vector<mpz_class> rhs_coefficients=rhs.get_coefficients();
	if(lhs.get_degree() != rhs.get_degree())
		return false;
	for( int i = 0; i <= lhs.get_degree() ; i++)
	{
		if(lhs_coefficients[i]!=rhs_coefficients[i])
		{
			return false;
		}
	}
	return true;
}

std::pair<Polynom, Polynom> divide(Polynom Pdivident, Polynom Pdivider)
{
	std::vector<mpz_class> zer{0};
	Polynom Zero(zer);
	std::vector<mpz_class> divident = Pdivident.get_coefficients();
	std::vector<mpz_class> divider = Pdivider.get_coefficients();
	std::vector<mpz_class> remainder=divident;
	std::vector<mpz_class> quotient;
	
	//modifying
	mpz_class h=divider[divider.size()-1];
	mpz_class h_0=h;
	for(int i = 0; i<remainder.size()-divider.size();i++)
		h=h*h_0;
	for(int i = 0; i<remainder.size();i++)
		remainder[i]*=h;
	//deleting zeros from the end
	int i = divident.size()-1;
	while(divident[i--]==0)
		divident.pop_back();

	if(divider.size()==1)
	{
		std::pair<Polynom,Polynom> result{Polynom(std::vector<mpz_class>{0}),Polynom(divident)};
	}
	//need to check if degree of divider is bigger then degree of divident
	while (remainder.size()>=divider.size())
	{
		//deleting elder zero if the exists
		if (remainder.back()==0)
		{
			remainder.pop_back();
			continue;
		}

		std::vector<mpz_class> deduct(remainder.size(),0);
		std::vector<mpz_class> temp_quotient;
		mpz_class q;
		q=remainder.back()/divider.back();
//		std::cout<<"q = "<<q<<std::endl;
		quotient.push_back(q);
		int size_dif=remainder.size()-divider.size();
		for(int i = 0; i < remainder.size() ; i++)
		{
			if(i>=size_dif)
				deduct[i]=q*divider[i-size_dif];
			if(i<size_dif)
				deduct[i]=0;
		}

		//checkign deduction
//		std::cout<<"deduct:"<<std::endl;
//		for (int i = 0; i < deduct.size() ;i++)
//			std::cout<<i<<":"<<deduct[i]<<std::endl;
//
		for(int i = 0; i < remainder.size();i++)
		{
			temp_quotient.push_back(remainder[i]-deduct[i]);
		}
//		std::cout<<"result:"<<std::endl;
//		for (int i = 0;i < temp_quotient.size(); i++)
//			std::cout<<i<<":"<<temp_quotient[i]<<std::endl;
		remainder=temp_quotient;
	}

	//now we need to reverse quotient vector
	std::vector<mpz_class> reversed_quotient;
	for(int i = quotient.size()-1;i>=0;i--)
		reversed_quotient.push_back(quotient[i]);
	quotient=reversed_quotient;
	
//	std::cout<<"REMAINDER"<<std::endl;
//	for(int i =0;i<remainder.size();i++)
//		std::cout<<remainder[i]<<std::endl;
	
	Polynom rem(remainder);
	rem.delete_zeros();
	Polynom quo(quotient);
	quo.delete_zeros();

	std::pair<Polynom, Polynom> result{rem,quo};
	return result;
}

