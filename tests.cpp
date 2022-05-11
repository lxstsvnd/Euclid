#include"tests.hpp"
#include"formulas.hpp"
#include<time.h>
#include<gmpxx.h>
#include<iostream>
#include<fstream>
#include"polynoms.hpp"

namespace Kirill
{
	//Генериурует случайный многочлен с коэффициентами от 1 до 10
	Polynom generate_polynom(int size, int seed)
	{
		std::vector<mpz_class> coefficients(size);
		srand(time(NULL)+seed);
		for(int i = 0; i < size; i++)
			coefficients[i]=rand()%10+1;
		return Polynom(coefficients);
	}

	//Генерирует ДНФ, в каждом конъюнкте одно равенство нулю со случайным многочленом
	DNF generate_DNF(int size, int seed, int polynom_size)
	{
		DNF dnf;
		std::vector<Predicate*> predicats(size);
		std::cout<<"Generating polynoms"<<std::endl;
		Polynom random_polynom;

		std::vector<Equality_predicate> temp_predicats(size);

		for(int i = 0; i < size; i++)
		{
			random_polynom=generate_polynom(polynom_size,seed+i);
			random_polynom.print();
			temp_predicats[i]=Equality_predicate(random_polynom,0);
		}

		for(int i = 0; i < size; i++)
			predicats[i]=&(temp_predicats[i]);


		std::cout<<"Collecting polynoms into DNF"<<std::endl;

		Conjunct conjunct;
		std::vector<Predicate*> predicate_ptr;

		for(int i = 0; i < size; i++)
		{
			predicate_ptr=std::vector<Predicate*>{predicats[i]};
			conjunct = Conjunct(predicate_ptr);
			dnf.add_conjunct(conjunct);
		}
		std::cout<<"returning dnf"<<std::endl;
		dnf.printPolynoms();
		return dnf;
	}

	//Генерирует случайную ДНФ заданного размера с многочленом заданного размера
	//Проверяет сколько времени она будет решаться
	//Пишет это в специальный файл
	void DNF_test_desideability(int size, int seed, int polynom_size)
	{
		std::cout<<"testing on random polynom"<<std::endl;
		std::cout<<"##########################"<<std::endl;
	
		DNF dnf;
		std::vector<Predicate*> predicats(size);
		std::cout<<"Generating polynoms"<<std::endl;
		Polynom random_polynom;

		std::vector<Equality_predicate> temp_predicats(size);

		for(int i = 0; i < size; i++)
		{
			random_polynom=generate_polynom(polynom_size,seed+i);
			random_polynom.print();
			temp_predicats[i]=Equality_predicate(random_polynom,0);
		}

		for(int i = 0; i < size; i++)
			predicats[i]=&(temp_predicats[i]);


		std::cout<<"Collecting polynoms into DNF"<<std::endl;

		Conjunct conjunct;
		std::vector<Predicate*> predicate_ptr;

		for(int i = 0; i < size; i++)
		{
			predicate_ptr=std::vector<Predicate*>{predicats[i]};
			conjunct = Conjunct(predicate_ptr);
			dnf.add_conjunct(conjunct);
		}
		std::cout<<"returning dnf\n"<<std::endl;
		dnf.printPolynoms();
		std::cout<<"#############\n"<<std::endl;


		int start_time=clock();
		std::cout<<"Trying to decide DNF"<<std::endl;
		dnf.printPolynoms();		
		dnf.decide();
		std::cout<<"DNF decided"<<std::endl;
		int time = clock()-start_time;
		std::ofstream fout("output.csv",std::ios::binary|std::ios::app);
//		std::ofstream fout("output.csv");
		fout<<polynom_size<<" , "<<(double)time/CLOCKS_PER_SEC<<std::endl;
		std::cout<<size<<" , "<<polynom_size<<" , "<<(double)time/CLOCKS_PER_SEC<<std::endl;
		fout.close();
	}

	void test(int size, int seed)
	{
		for(int i = 2; i < 6; i++)
		{
			for(int k = 0; k < 10; k++)
			{
				DNF_test_desideability(size,seed,i);
			}
		}
	}
}
