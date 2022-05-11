#include "parser.hpp"
#include "lexer.hpp"
#include "formulas.hpp"
#include "polynoms.hpp"
#include "tarsky.hpp"
#include "tests.hpp"

int main()
{
//	std::string text;
//	std::getline(std::cin, text);
//	Dima::Tokenizer lexer(text);
//	lexer.debugPrint();
//	Dima::parseTree decision(lexer.get_tokens());
//	decision.parse();
//	decision.makeDNF();

	//Kirill baluetsa
//	Kirill::DNF dnf = decision.getDNF();
//	dnf.printPolynoms();
//	Kirill::DNF_test_desideability(3,0,3);
	Kirill::test(3,0);	

//	std::cout<<"\n\n"<<std::endl;
//	std::cout<<dnf.decide()<<std::endl;
}
