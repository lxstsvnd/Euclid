#include "token.hpp"
#include <vector>
#include <string>
#include <cctype>

class Tokenizer
{
	private:
		std::vector<Token> _tokens;
		std::vector<Token> _variables; //имена переменных (нужно для анализатора)
		std::vector<Token> _constants; //значения констант (нужно для анализатора)
	public:
		Tokenizer();
		~Tokenizer();
		void make_tokens(std::string input); //лексический анализ строки
		std::vector<Token> get_variables();
		std::vector<Token> get_constants();
		std::vector<Token> get_tokens();
		void add_variable(std::string symbol);
		void add_constant(std::string constant);
		void add_token(std::string token_name);
		bool check_special(std::string special); //проверка строки на соответствие спецсимволу
		bool isother(char symbol); //проверка символа на скобку, арифметический оператор или оператор порядка
};
