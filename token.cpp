#include "token.hpp"

Token::Token(std::string name){_name = name;}
Token::Token(){;}
Token::~Token(){;} //Доделать
std::string Token::get_name(){return _name;}
void Token::set_name(std::string new_name){_name = new_name;}
void Token::set_rule(std::vector<Token> rule){_rule.push_back(rule);}
void Token::set_connects(std::vector<Token> connects){_connects.push_back(connects);}
