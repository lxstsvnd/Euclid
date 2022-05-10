#include "parser.hpp"
#include "lexer.hpp"
#include "formulas.hpp"
#include "polynoms.hpp"
#include "tarsky.hpp"

int main()
{
	std::string text;
	std::getline(std::cin, text);
	Dima::Tokenizer lexer(text);
	Dima::parseTree decision(lexer.get_tokens());
	decision.parse();
	decision.makeDNF();
	Kirill::DNF dnf = decision.getDNF();
	dnf.printPolynoms();
	
	std::cout<<"\n\n"<<std::endl;
	std::cout<<dnf.decide()<<std::endl;
}
