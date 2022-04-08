#include"polynoms.hpp"
#include<iostream>
//В этом файле описываются функции для работы с многочленами
//В многочлене находится вектор коэффициентов типа mpz_class
//mpz_class - длинные числа целой арифметики из пакета GMP
//В векторе коэффициенты располагаются в порядке возрастания степени

namespace Kirill
{
	//Конструктор многочлена - получает на вход вектор элементов mpz_class
	Polynom::Polynom(std::vector<mpz_class> coefficients): _coefficients(coefficients){}
	Polynom::Polynom(){;}
	
	//Функция возвращает вектор коэффициентов многочлена
	std::vector<mpz_class> Polynom::get_coefficients()
	{
		return _coefficients;
	}

	//Функция печатает многочлен в формате 1x^0+2x^2+3x^3+...
	void Polynom::print()
	{
		int size=_coefficients.size();
		for(int i = 0; i < size ; i++)
		{
			std::cout<<_coefficients[i]<<"x^"<<i<<"+";
		}
	}
	
	//Возвращает степень многочлена
	int Polynom::get_degree()
	{
		return _coefficients.size()-1;
	}

	void Polynom::set_point(double point)
	{
		_point=point;
	}

	double Polynom::get_point()
	{
		return _point;
	}

	//Удаляет старшие нулевые коэффициенты, если многочлен это не константа
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

	//Возвращает производную многочлена
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

	//Возвращает true, если многочлены покоэффициентно равны, false иначе
	bool operator ==(Polynom lhs, Polynom rhs)
	{
		std::vector<mpz_class> lhs_coefficients=lhs.get_coefficients();
		std::vector<mpz_class> rhs_coefficients=rhs.get_coefficients();
		if(lhs.get_degree() != rhs.get_degree())//если их степени не равны то дальше проверять нет смысла
			return false;
		for( int i = 0; i <= lhs.get_degree() ; i++)//покоэффициентное сравнение
			if(lhs_coefficients[i]!=rhs_coefficients[i])
				return false;
		return true;
	}

	//Функция модифицированного деления многочленов, замкнутого для многочленов в целых коэффициентов
	//Работает по алгоритму школьного деления в столбик
	//Принимает два многочлена - первый делимое, второе делитель, степень первого не меньше степени второго
	//Возвращает два многочлена - первый остаток, второй частное
	std::pair<Polynom, Polynom> divide(Polynom Pdivident, Polynom Pdivider)
	{
		std::vector<mpz_class> divident = Pdivident.get_coefficients();//Вектор делимого
		std::vector<mpz_class> divider = Pdivider.get_coefficients();//Вектор делителя
		std::vector<mpz_class> remainder=divident;//Вектор остатка
		std::vector<mpz_class> quotient;//Вектор частного
	
		mpz_class h=divider[divider.size()-1];//коэффициент для модифицированного деления
		mpz_class h_0=h;
	
		for(int i = 0; i<remainder.size()-divider.size();i++)
			h=h*h_0;
		for(int i = 0; i<remainder.size();i++)
			remainder[i]*=h;

		//Удаляем нули с конца
		int i = divident.size()-1;
		while(divident[i--]==0)
			divident.pop_back();
		//Если делитель это константа, то остаток это ноль
		if(divider.size()==1)
			std::pair<Polynom,Polynom> result{Polynom(std::vector<mpz_class>{0}),Polynom(divident)};
	
		while (remainder.size()>=divider.size())
		{
			//Каждую итерацию чистим коэффициенты с конца
			if (remainder.back()==0)
			{
				remainder.pop_back();
				continue;
			}
	
			std::vector<mpz_class> deduct(remainder.size(),0);//Вектор вычитаемого
			std::vector<mpz_class> temp_quotient;//Вектор в котором храним результат вычитания из остатка вычитаемого
			mpz_class q;//Коэффициент частного
			q=remainder.back()/divider.back();
			quotient.push_back(q);
			int size_dif=remainder.size()-divider.size();
			for(int i = 0; i < remainder.size() ; i++)//Рассчитываем вычитаемое
			{
				if(i>=size_dif)
					deduct[i]=q*divider[i-size_dif];
				if(i<size_dif)
					deduct[i]=0;
			}
	
			for(int i = 0; i < remainder.size();i++)//Вычитаем из остатка вычитаемое
				temp_quotient.push_back(remainder[i]-deduct[i]);
			remainder=temp_quotient;
		}
	
		//Нужно перевернуть вектор остатков
		std::vector<mpz_class> reversed_quotient;
		for(int i = quotient.size()-1;i>=0;i--)
			reversed_quotient.push_back(quotient[i]);
		quotient=reversed_quotient;
		
		Polynom rem(remainder);
		rem.delete_zeros();
		Polynom quo(quotient);
		quo.delete_zeros();
	
		std::pair<Polynom, Polynom> result{rem,quo};
		return result;
	}
}
