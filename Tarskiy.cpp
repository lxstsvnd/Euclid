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
						{
							raw_polynoms.push_back(C);
							//also should add -C for j/i
							std::vector<mpz_class> negative_c=C.get_coefficients();
							for(int k=0;k<negative_c.size();k++)
								negative_c[k]=-negative_c[k];
							raw_polynoms.push_back(Polynom(negative_c));

						}
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

void row_print(std::vector<std::vector<int>> t, std::vector<Polynom> polynoms);
//maybe optimizing for first and second polynom order
std::vector<std::vector<int>> tars_table(std::vector<Polynom> polynoms)
{
	std::cout<<"tarskiy table initiated"<<std::endl;
	//-1000 in table is empty marker
	std::vector<std::vector<int>> t;//table
	//sorting polynoms by degree
	std::cout<<"end of the world"<<std::endl;
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
	row_print(t,polynoms);
	//main inductive cycle
	for(int p=1;p<polynoms.size();p++)
	{
		std::cout<<std::endl<<std::endl<<"NEW ITERATION"<<std::endl;
		std::cout<<"adding polynom ";
		polynoms[p].print();
		std::cout<<std::endl;

		std::vector<int> new_roots;//places for new roots
		std::vector<int> new_roots_temp;
		//adding new column to table with -inf and +inf point
		std::vector<int> new_column(t[0].size());
		for(int i=0;i<t[0].size();i++)
			new_column[i]=-1000;
		std::vector<mpz_class> p_coef=polynoms[p].get_coefficients();
		mpz_class sign_mpz=p_coef[p_coef.size()-1]/abs(p_coef[p_coef.size()-1]);
//		std::cout<<"sign of "<<p<<"polynom is "<<sign_mpz.get_d()<<std::endl;
//		std::cout<<"deg of polynom "<<p<<" is "<<polynoms[p].get_degree()<<std::endl; 
		if((polynoms[p].get_degree() % 2)==0)//if not even
		{
			new_column[0]=sign_mpz.get_d();
			new_column[t[0].size()-1]=sign_mpz.get_d();
		}
		if((polynoms[p].get_degree() % 2)==1)//if even
		{
			new_column[0]=sign_mpz.get_d()*(-1);
			new_column[t[0].size()-1]=sign_mpz.get_d();
		}

		t.push_back(new_column);
		std::cout<<"t size is "<<t.size()<<std::endl;

		//iteration on roots
		for(int i=1;i<t[0].size();i+=2)
		{
			int sign=-1000;
			int l=0;
			std::vector<mpz_class> divider_coefficients{-1000};
			mpz_class divider_elder_coefficient=0;
			mpz_class divider_coeff_sign=-1000;
			mpz_class divider_coeff_sign_old=-1000;
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
			//getting sign of powered elder coefficient
			divider_coefficients=polynoms[l].get_coefficients();
			divider_elder_coefficient=divider_coefficients[divider_coefficients.size()-1];
			divider_coeff_sign=divider_elder_coefficient/abs(divider_elder_coefficient);
			divider_coeff_sign_old=divider_coeff_sign;
			for(int j=0;j<polynoms[p].get_degree()-polynoms[l].get_degree();j++)
				divider_coeff_sign*=divider_coeff_sign_old;

			//printing polynoms
			polynoms[p].print();
			std::cout<<std::endl;
			polynoms[l].print();
			//l is number of polynom which have 0 on i place
			Polynom rem = divide(polynoms[p],polynoms[l]).first;
			std::cout<<"remainder is ";
			rem.print();
			std::cout<<std::endl;
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
				if(rem.get_coefficients()[0]==0)
					t[p][i]=0;
				if(rem.get_coefficients()[0]!=0)
				{
					mpz_class remnant_coeff_sign=rem.get_coefficients()[0]/abs(rem.get_coefficients()[0]);
					mpz_class modified_elder_coeff_sign=remnant_coeff_sign*divider_coeff_sign;
					t[p][i]=(int)modified_elder_coeff_sign.get_d();
				}
			}
		}

		//signing empty cells
		//iteration on intervals
		//not sure about thins
		if(p!=1)//need to make this thing better
			for(int i=1;i<t[0].size()-3;i+=2)
			{
				if(t[p][i]==-1000)
				{
					t[p][i+1]=-2000;
					continue;
				}
				if(t[p][i]!=0)
				{
					t[p][i+1]=t[p][i];
					continue;
				}
				if(t[p][i]==0)
				{
					t[p][i+1]=t[p][i+2];
					continue;
				}
				if(t[p][i+2]==0)
				{
					t[p][i+1]=0;
					continue;
				}
			}

		//adding new roots
		//1.finding new roots
		//need to iterate through last line and collect occasiong -1|+1 in new_roots vector
		for(int i = 1;i<t[0].size();i++)
			if(t[p][i]==t[p][i-1]*(-1))
				new_roots.push_back(i-1);
		//lets look on last row
		std::cout<<"Last row is";
		for(int i = 0;i < t[p].size();i++)
			std::cout<<t[p][i]<<"|";
		std::cout<<std::endl;
		std::cout<<"new roots are";
		if(new_roots.size()==0)
			std::cout<<"there is no new roots"<<std::endl;
		for(int i=0;i<new_roots.size();i++)
			std::cout<<new_roots[i]<<" ";
		std::cout<<std::endl;
		

		
		//2.making new columns
		new_roots_temp=new_roots;
		//iteration through all previous polynoms
		for(int i=0;i<t.size()-1;i++)
		{
			//iterating through new roots
			for(int j=0;j<new_roots.size();j++)
			{
				auto it=t[i].begin();
				it += new_roots[j]+1;
				int prev=t[i][new_roots[j]];
				int next=t[i][new_roots[j]+1];
				if(prev!=0)//if left is not 0
					it=t[i].insert(it,{prev,prev});
				if(prev==0)//if left is 0
				{
					if(next!=0)//if left is 0 but right is not
						it=t[i].insert(it,{next,next});
					if(next==0)//if both left and right 0
						it=t[i].insert(it,{0,0});
				}
				for(int k=j+1;k<new_roots.size();k++)
					new_roots[k]+=2;
			}
			//shifting remain roots
			new_roots=new_roots_temp;
			
		}
		//check if iteration was correct
		new_roots=new_roots_temp;
		std::cout<<"previous polynoms redacted, now work with new polynom"<<std::endl;
		std::cout<<"new roots are";
		if(new_roots.size()==0)
			std::cout<<"there is no new roots"<<std::endl;
		for(int i=0;i<new_roots.size();i++)
			std::cout<<new_roots[i]<<" ";
		std::cout<<std::endl;
		std::cout<<"table is now"<<std::endl;
		row_print(t,polynoms);
		//iteration through new polynom
		for(int j=0;j<new_roots.size();j++)
		{
			auto it=t[p].begin();
			it += new_roots[j]+1;
			int prev=t[p][new_roots[j]];
			int next=t[p][new_roots[j]+1];
			if((new_roots[j] % 2)==0)//conflict on interval-point => 0|+
				it=t[p].insert(it,{0,next});
			if((new_roots[j] % 2)==1)//conflict on point-interval=> +|0
				it=t[p].insert(it,{prev,0});
			for(int k=j+1;k<new_roots.size();k++)
				new_roots[k]+=2;
		}

		row_print(t,polynoms);
	}
	return t;
}

void row_print(std::vector<std::vector<int>> t, std::vector<Polynom> polynoms)
{
	std::cout<<"Printing talbe..."<<std::endl;
	for(int j=0;j<t.size();j++)
	{
		std::cout<<"__________"<<std::endl;
		std::cout<<"["<<j<<"] = ";
		polynoms[j].print();
		std::cout<<" |";
		for(int i=0;i<t[j].size();i++)
			std::cout<<t[j][i]<<" |";
		std::cout<<std::endl;
		std::cout<<"__________"<<std::endl;
	}
	std::cout<<"Table ended..."<<std::endl;
}
