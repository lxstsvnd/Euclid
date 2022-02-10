#include "lexer.hpp"

int main()
{
	//лексический анализ
	std::string input;
	Tokenizer lexer = Tokenizer();
	lexer.make_tokens(input);
	std::vector<char> variables = lexer.get_variables();
	std::vector<int> constants = lexer.get_constants();
	std::vector<Token> lexems = lexer.get_tokens();
}
