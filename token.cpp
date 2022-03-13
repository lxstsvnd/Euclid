#include "token.hpp"

Token::Token(std::string name){_name = name;}
Token::Token(){;}
Token::~Token(){;} //Доделать
std::string Token::get_name(){return _name;}
void Token::set_name(std::string new_name){_name = new_name;}
void Token::set_rule(std::vector<Token*> rule){_rule.push_back(rule);}
void Token::set_connects(std::vector<Token*> connects){_connects.push_back(connects);}
std::vector<std::vector<Token*>> Token::get_rules(){return _rule;}
std::vector<std::vector<Token*>> Token::get_connects(){return _connects;}
std::vector<Token*> Token::check_connects(Token lexem)
{
	for(int iter = 0; iter < _connects.size(); ++iter)
	{
		for(int jter = 0; jter < _connects[iter].size(); ++jter)
		{
			if((*_connects[iter][jter]).get_name() == lexem.get_name()){return _rule[iter];}
		}
	}
	return {};
}
bool Token::check_name()
{
	if(_name == "(" || _name == ")" || _name == "forall" || _name == "exists" || _name == "not" || _name == ">"
			|| _name == "=" || _name == "geq" || _name == "+" || _name == "*"
			|| _name == "rightarrow" || _name == "$") return 0;
	return 1;
}