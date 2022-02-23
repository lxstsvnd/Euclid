#include "token.hpp"

Token::Token(std::string token_name){_token_name = token_name;}
Token::Token(){;}
Token::~Token(){_token_name.clear();}
std::string Token::get_name(){return _token_name;}
void Token::set_name(std::string new_name){_token_name = new_name;}
