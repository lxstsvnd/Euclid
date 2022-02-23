#include "lexer.hpp"
#include "parser.hpp"

int main()
{
	//лексический анализ
	std::string input;
	Tokenizer lexer = Tokenizer();
	lexer.make_tokens(input);
	std::vector<char> variables = lexer.get_variables();
	std::vector<int> constants = lexer.get_constants();
	std::vector<Token> lexems = lexer.get_tokens();

	//инициализация терминальных токенов, кроме var и const
	Token l_brac = Token("(");
	Token r_brac = Token(")");
	Token forall = Token("forall");
	Token exists = Token("exists");
	Token reduct = Token("not");
	Token great = Token(">");
	Token equal = Token("=");
	Token geq = Token("geq");
	Token plus = Token("+");
	Token mult = Token("mult");
	Token arrow = Token("rightarrow");

	//Объявляение нетерминальных токенов
	Analyzer_token Start = Analyzer_token();
	Analyzer_token Form = Analyzer_token();
	Analyzer_token Quantize = Analyzer_token();
	Analyzer_token Quantor = Analyzer_token();
	Analyzer_token Form1 = Analyzer_token();
	Analyzer_token Brform = Analyzer_token();
	Analyzer_token Imp = Analyzer_token();
	Analyzer_token Neg = Analyzer_token();
	Analyzer_token Expr = Analyzer_token();
	Analyzer_token Expr1 = Analyzer_token();
	Analyzer_token Sum = Analyzer_token();
	Analyzer_token Mult = Analyzer_token();
	Analyzer_token Mult1 = Analyzer_token();
	Analyzer_token Var = Analyzer_token();
	Analyzer_token Const = Analyzer_token();
	Analyzer_token Arrow = Analyzer_token();
	Analyzer_token Ord = Analyzer_token();

	//заполнение полей нетерминальных токенов
	Start.set_rule();
}
