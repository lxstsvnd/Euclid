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
	lexer.debugPrint();
	Dima::parseTree decision(lexer.get_tokens());
	decision.parse();
	decision.makeDNF();
	Kirill::DNF dnf = decision.getDNF();
	dnf.printPolynoms();
}
