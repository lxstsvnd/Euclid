#include "token.hpp"

Token::Token(std::string name){_name = name;}
Token::Token(){;}
Token::~Token(){;} //Доделать
std::string Token::get_name(){return _name;}
void Token::set_name(std::string new_name){_name = new_name;}
void Token::set_rule(std::vector<Token> rule){_rule.push_back(rule);}
void Token::set_connects(std::vector<Token> connects){_connects.push_back(connects);}
std::vector<std::vector<Token>> Token::get_rules(){return _rule;}
std::vector<std::vector<Token>> Token::get_connects(){return _connects;}
std::vector<Token> Token::check_connects(Token lexem)
{
	for(int iter = 0; iter < _connects.size(); ++iter)
	{
		for(std::vector<Token>::iterator jter = _connects[iter].begin(); jter != _connects[iter].end(); ++jter)
		{
			if((*jter).get_name() == lexem.get_name()){return _rule[iter];}
		}
	}
	return {};
}
