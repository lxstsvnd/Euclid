#include<vector>
#include<tuple>
#include<gmpxx.h>
class Polynom
{
private:
	std::vector<mpz_class> _coefficients;
public:	
	Polynom();
	Polynom(std::vector<mpz_class>);

	std::vector<mpz_class> get_coefficients();
	Polynom get_derivative();
	void print();
	int get_degree();
	void delete_zeros();
	Polynom delete_elder_coefficient();
	Polynom remain_elder_coefficient();
	friend bool operator ==(Polynom,Polynom);
	friend std::pair<Polynom,Polynom> divide(Polynom,Polynom);
};
