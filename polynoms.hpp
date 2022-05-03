#pragma once

#include<vector>
#include<tuple>
#include<gmpxx.h>

namespace Kirill
{
	class Polynom
	{
	private:
		std::vector<mpz_class> _coefficients;
	public:	
		Polynom();
		Polynom(std::vector<mpz_class>);
	
		void add_coefficient(mpz_class coefficient);
		std::vector<mpz_class> get_coefficients();
		Polynom get_derivative();
		void print();
		int get_degree();
		void delete_zeros();
		friend bool operator ==(Polynom,Polynom);
		friend std::pair<Polynom,Polynom> divide(Polynom,Polynom);
	};
}
