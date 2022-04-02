#include<iostream>
#include<vector>
#include<set>
#include<algorithm>
#include<math.h>
#include"Tarskiy.hpp"

// our main object is vector<vector<<int>> Polynon_graph;
// if unsaturated is already sorted by deg(P)
// matrix will be upper-triangle


std::vector<Polynom> full_saturation(std::vector<Polynom> unsaturated)
{
	int size=unsaturated.size();
	while(1)
	{
		std::cout<<"im alive"<<std::endl;
		unsaturated=derivation_saturation(unsaturated);
		unsaturated=mod_saturation(unsaturated);
//		unsaturated=drop_elder_coefficient_saturation(unsaturated);
//		unsaturated=elder_coefficient_saturation(unsaturated);
		std::cout<<"mod saturation ends here"<<std::endl;
		std::cout<<": unsat.size="<<unsaturated.size()<<std::endl;
		std::cout<<": old_size="<<size<<std::endl;
		if (unsaturated.size()==size)
			break;
		size=unsaturated.size();

	}
		for(int i=0;i<unsaturated.size();i++)
			if(unsaturated[i].get_degree()==0)
				unsaturated.erase(unsaturated.begin()+i);
	
	return unsaturated;
}

//need to fix get_derivative
std::vector<Polynom> derivation_saturation(std::vector<Polynom> unsaturated)
{
	std::cout<<"derivating"<<std::endl;
	std::vector<mpz_class> c;
	Polynom C(c);
	std::vector<Polynom> new_polynoms;
	for (int i =0;i<unsaturated.size();i++)
	{
		C=unsaturated[i];
		while (C.get_degree()>0)
		{
			C=C.get_derivative();
			C.print();
			if(C.get_degree()>0)
				new_polynoms.push_back(C);
		}
	}

	//adding new polynoms into unsaturated
	for (int i=0;i<new_polynoms.size();i++)
		unsaturated.push_back(new_polynoms[i]);

	uniquying(unsaturated);
	std::cout<<"derivation ended"<<std::endl;
	return unsaturated;
}	

std::vector<Polynom> elder_coefficient_saturation(std::vector<Polynom> unsaturated)
{
	std::cout<<"elder_coefficient sarutaion"<<std::endl;
	std::vector<mpz_class> c;
	Polynom C(c);
	std::vector<Polynom> new_polynoms;
	for(int i=0;i<unsaturated.size();i++)
	{
		C=unsaturated[i];
		C=C.remain_elder_coefficient();
		C.print();
		if(C.get_degree()>0)
			new_polynoms.push_back(C);
	}
	for (int i=0;i<new_polynoms.size();i++)
		unsaturated.push_back(new_polynoms[i]);
	uniquying(unsaturated);
	std::cout<<"elder coefficients sarutaion ended"<<std::endl;
	return unsaturated;
}

std::vector<Polynom> drop_elder_coefficient_saturation(std::vector<Polynom> unsaturated)
{

	std::cout<<"dropping elder_coefficient"<<std::endl;
	std::vector<mpz_class> c;
	Polynom C(c);
	std::vector<Polynom> new_polynoms;
	for(int i=0;i<unsaturated.size();i++)
	{
		C=unsaturated[i];
		while(C.get_degree()>0)
		{
			C=C.delete_elder_coefficient();
			C.print();
			if(C.get_degree()>0)
				new_polynoms.push_back(C);
		}
	}
	for(int i = 0; i< new_polynoms.size();i++)
		unsaturated.push_back(new_polynoms[i]);
	uniquying(unsaturated);
	std::cout<<"elder coefficient droppng ended"<<std::endl;
	return unsaturated;
}

std::vector<Polynom> mod_saturation(std::vector<Polynom> unsaturated)
{
	Polynom C=unsaturated[0];
	//we adding new polynoms in raw_polynom before modifying the main matrix
	std::vector<Polynom> raw_polynoms;
	int size=unsaturated.size();
	//zero matrix size*size

	std::vector<std::vector<int>> Polynom_graph;
	std::vector<int> null_column(size,0);

	for(int i=0;i<size;i++)
		Polynom_graph.push_back(null_column);
	
	//making closur over unsaturated system
	//
	//additional checking for trying to divide double on another double
	while (1)
	{
		std::cout<<"New iteration"<<std::endl;
		std::cout<<"Graph size is "<<unsaturated.size()<<std::endl;
		for(int i=0;i<Polynom_graph.size();i++)
			Polynom_graph[i][i]=1;
			
		std::cout<<"Polynoms are"<<std::endl;

		std::cout<<"Comparing polynoms"<<std::endl;
		std::cout<<unsaturated.size()<<std::endl;
		std::cout<<Polynom_graph.size()<<std::endl;	


		for(int i=0;i<Polynom_graph.size();i++)
		{
			std::cout<<"i :"<<i<<std::endl;
			for(int j =0;j<Polynom_graph.size();j++)
			{
				std::cout<<"j :"<<j<<std::endl;
				if(Polynom_graph[i][j]==0)
				{
					if (unsaturated[i].get_degree()>=unsaturated[j].get_degree())
					{
						if(unsaturated[i].get_degree()==0)
						{
							Polynom_graph[i][j]=-2;
							Polynom_graph[j][i]=-2;
							continue;
						}
						Polynom_graph[i][j]=1;
						Polynom_graph[j][i]=-1;
						std::cout<<"dividing polynom:"<<std::endl;
						unsaturated[i].print();
						std::cout<<"on polynom:"<<std::endl;
						unsaturated[j].print();
						std::cout<<"the result is :"<<std::endl;
						C=divide(unsaturated[i],unsaturated[j]).first;
						if(C.get_degree()>0)
							raw_polynoms.push_back(C);
						C.print();
//						if(unsaturated[i].get_degree()==unsaturated[j].get_degree())
//							C=C.divide(unsaturated[j],unsaturated[i]).first;
					}
					else
					{
						Polynom_graph[i][j]=-1;
						Polynom_graph[j][i]=1;
						C=divide(unsaturated[j],unsaturated[i]).first;
						if(C.get_degree()>0)
							raw_polynoms.push_back(C);
					}
				}			
			}
		}

		std::cout<<"Pushing back polynoms..."<<std::endl;

		for(int i =0;i<raw_polynoms.size();i++)
			unsaturated.push_back(raw_polynoms[i]);
		std::cout<<"POlynoms pushed."<<std::endl;
		std::cout<<"Uniquying..."<<std::endl;
//need to fix unification
		uniquying(unsaturated);
		std::cout<<"Uniquyed"<<std::endl;
//deleting bag constants
		for(int i=0;i<unsaturated.size();i++)
			if(unsaturated[i].get_degree()==0)
				unsaturated.erase(unsaturated.begin()+i);
		std::cout<<"difference is"<<unsaturated.size()-size<<std::endl;
		std::cout<<":"<<std::endl;
//deb
		for(int i =0;i<unsaturated.size();i++)
		{
			std::cout<<" "<<std::endl;
			std::cout<<i<<" : ";	
			unsaturated[i].print();
			std::cout<<" "<<std::endl;
		}
//deb

		//changing old Graph, adding new zeros
		double diff=unsaturated.size()-size;
		//column of zeros
		std::vector<int> null_column(unsaturated.size(),0);

		//adding zeros to already existed columns
		for(int i=0;i<size;i++)
			for(int j=0;j<diff;j++)
				Polynom_graph[i].push_back(0);

		//adding new columns
		for(int i =0; i<diff;i++)
			Polynom_graph.push_back(null_column);


		if(size==unsaturated.size())
			break;
		size=unsaturated.size();
		std::cout<<std::endl;
		std::cout<<std::endl;
	}	
	return unsaturated;
}
//only unqieu polynoms remains
void uniquying(std::vector<Polynom>& unsat)
{
	std::vector<Polynom> unique;
	for (int i =0;i<unsat.size();i++)
	{
//		if(unsat[i].get_degree()==1)
//			continue;
		std::cout<<unsat.size()<<std::endl;
		std::cout<<i<<std::endl;
		int k =0;
		for(int j=0;j<unique.size();j++)
		{
			if(unsat[i]==unique[j])
				k++;
		}
		if (k==0)
			unique.push_back(unsat[i]);
	}
	unsat=unique;
}

void range(std::vector<Polynom>& unranged)
{
	std::vector<Polynom> ranged;
	//should find maximum
	Polynom Max=unranged[0];
	for(int i =1; i<unranged.size();i++)
			if (unranged[i].get_degree()>Max.get_degree())
				Max=unranged[i];
	for(int i =0;i<Max.get_degree()+1;i++)
		for(int j=0;j<unranged.size();j++)
			if(unranged[j].get_degree()==i)
				ranged.push_back(unranged[i]);
	unranged=ranged;
}

void degree_sort(std::vector<Polynom>& unsorted)
{
	//ordinary bubble sort
	Polynom temp(std::vector<mpz_class>{0});
	for(int i=0;i<unsorted.size();i++)
	{
		for(int j=0;j<unsorted.size()-i-1;j++)
		{	
			if (unsorted[j].get_degree()>unsorted[j+1].get_degree())
			{
				temp=unsorted[j];
				unsorted[j]=unsorted[j+1];
				unsorted[j+1]=temp;
			}
		}
	}
		
}
//maybe optimizing for first and second polynom order
std::vector<std::vector<int>> tars_table(std::vector<Polynom> polynoms)
{
	std::cout<<"tarskiy table initiated"<<std::endl;
	//-1000 in table is empty marker
	std::vector<std::vector<int>> t;//table
	//sorting polynoms by degree
	std::cout<<"end of the world"<<std::endl;
//	degree_sort(polynoms);
	//first iteration for linear polynom
	//std::cout<<"END OF THE WORLD"<<std::endl;
	std::vector<int> first_column(3);
	std::vector<mpz_class> first_p_coef=polynoms[0].get_coefficients();
	mpz_class sign_mpz = first_p_coef[1]/abs(first_p_coef[1]);
	int sign = sign_mpz.get_ui();

	//std::cout<<"END OF THE WORLD"<<std::endl;
	first_column[0]=sign*(-1);
	first_column[1]=0;
	first_column[2]=sign;
	t.push_back(first_column);
	
	//main inductive cycle
	for(int p=1;p<polynoms.size();p++)
	{
		std::vector<int> new_roots;//places for new roots
		//adding new column to table with -inf and +inf point
		std::vector<int> new_column(t[0].size());
		for(int i=0;i<t[0].size();i++)
			new_column[i]=-1000;
		std::vector<mpz_class> p_coef=polynoms[p].get_coefficients();
		mpz_class sign_mpz=first_p_coef[1]/abs(first_p_coef[1]);
		if((polynoms[p].get_degree() % 2)==0)//if even
		{
			new_column[0]=sign_mpz.get_ui();
			new_column[t[0].size()-1]=sign_mpz.get_ui();
		}
		if((polynoms[p].get_degree() % 2)==1)
		{
			new_column[0]=sign_mpz.get_ui()*(-1);
			new_column[t[0].size()-1]=sign_mpz.get_ui();
		}

		t.push_back(new_column);

		//iteration on roots
		for(int i=1;i<t[0].size();i+=2)
		{
			int sign=-1000;
			int l=0;
			//need to find polynom with zero on i
			for(int j=0;j<t.size();j++)
			{
				if(t[j][i]==0)
				{
					std::cout<<"found at polynom number "<<j<<std::endl;
					l=j;
					break;		
				}
			}
			//l is number of polynom which have 0 on i place
			Polynom rem = divide(polynoms[i],polynoms[l]).first;
			std::cout<<"remainder is ";
			rem.print();
			std::cout<<"remainder's degree is "<<rem.get_degree()<<std::endl;
			std::cout<<std::endl;
			//if reminder is actually a polynom, not a constant
			if(rem.get_degree()>0)
			{
			//need to find rem polynom in l and copy sign
				for(int j=0;j<t.size();j++)
					if(polynoms[j]==rem)
					{	
						std::cout<<"dfound"<<std::endl;
						t[p][i]=t[j][i];
					}
			}
			//case if reminder is a constant
			if(rem.get_degree()==0)
			{
				mpz_class mpz_sign=rem.get_coefficients()[0]/abs(rem.get_coefficients()[0]);
				t[p][i]=mpz_sign.get_ui();
			}
		}

		//signing empty cells
		//iteration on intervals
		//not sure about thins
		if(p!=1)//need to make this thing better
			for(int i=1;i<t[0].size()-3;i+=2)
			{
				t[p][i+1]=t[p][i];	
			}
		
		//adding new roots
		//1.finding new roots
		//need to iterate through last line and collect occasiong -1|+1 in new_roots vector
		for(int i = 1;i<t[0].size();i++)
			if(t[p][i]==t[p][i-1]*(-1))
				new_roots.push_back(i-1);

		std::cout<<"new roots are";
		if(new_roots.size()==0)
			std::cout<<"there is no new roots"<<std::endl;
		for(int i=0;i<new_roots.size();i++)
			std::cout<<new_roots[i]<<" ";
		std::cout<<std::endl;
	//}
	}
	return t;
}
