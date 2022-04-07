#include "lexer.hpp"
#include "parser.hpp"
#include "formulas.hpp"

int main()
{
	std::string text;
	std::getline(std::cin, text);
	dnf_parser::Tokenizer lexer(text);
	lexer.debugPrint();
	dnf_parser::parseTree decision(lexer.get_tokens());
	decision.parse();
}
