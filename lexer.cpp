#include "lexer.hpp"

namespace Dima
{
	Token::Token(enum TokenType type){_mType = type;}
	Token::Token(){;}
	
	void Token::setText(std::string newText){_mText = newText;}
	void Token::setType(enum TokenType newType){_mType = newType;}
	void Token::textPush(char symbol){_mText.push_back(symbol);}
	void Token::textErase(){_mText.erase();}

	const enum TokenType Token::getType() const{return _mType;}
	const std::string Token::getText() const{return _mText;}

	Tokenizer::Tokenizer(const std::string& Line) //строка разбирается в конструкторе
	{
		for(char cCh : Line) //cCh = current character
		{
			if(_cToken.getType() == SPECIAL_SEQUENCE)
			{
				switch(cCh)
				{
					case 'd': //дизъюнкция
						_cToken.textPush('d');
						_cToken.setType(DISJOINT);
						break;
					case 'c': //конъюнкция
						_cToken.textPush('c');
						_cToken.setType(CONJUNCT);
						break;
					case 'n': //отрицание
						_cToken.textPush('n');
						_cToken.setType(RESISTANCE);
						break;
					default: //ошибка по умолчанию
						throw std::runtime_error("uknkown special symbol!");
						break;
				}
				endToken();
				continue;
			}

			switch(cCh)
			{
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9':
					if(_cToken.getType() == WHITESPACE || _cToken.getType() == INTEGER_LITERAL)
					{
						_cToken.setType(INTEGER_LITERAL);
						_cToken.textPush(cCh);
					}else{throw std::runtime_error("0-9 is not in integer literal");}
					break;

				case '\\': //выловлено начало спецсимвола
					if(!_cToken.getText().empty()){endToken();}
					_cToken.setType(SPECIAL_SEQUENCE);
					break;

				case '(':
					if(!_cToken.getText().empty()){endToken();}
					_cToken.setType(LBRAC);
					_cToken.textPush('(');
					endToken();
					break;
				case ')':
					if(!_cToken.getText().empty()){endToken();}
					_cToken.setType(RBRAC);
					_cToken.textPush(')');
					endToken();
					break;
				case '+':
					if(!_cToken.getText().empty()){endToken();}
					_cToken.setType(PLUS);
					_cToken.textPush('+');
					endToken();
					break;
				case '*':
					if(!_cToken.getText().empty()){endToken();}
					_cToken.setType(MULTIPLY);
					_cToken.textPush('*');
					endToken();
					break;
				case '^':
					if(!_cToken.getText().empty()){endToken();}
					_cToken.setType(POWER);
					_cToken.textPush('^');
					endToken();
					break;
				case '=':
					if(!_cToken.getText().empty()){endToken();}
					_cToken.setType(EQUAL);
					_cToken.textPush('=');
					endToken();
					break;
				case '>':
					if(!_cToken.getText().empty()){endToken();}
					_cToken.setType(GREATER);
					_cToken.textPush('>');
					endToken();
					break;
				case 'x':
					if(!_cToken.getText().empty()){endToken();}
					_cToken.setType(IDENTIFIER);
					_cToken.textPush('x');
					endToken();
					_cToken.setType(IDENTIFIER);
					break;
				case '-': //x-c -> [x] [+] [-c]
					if(_cToken.getType() != WHITESPACE)
					{
						if(!_cToken.getText().empty()){endToken();}
						_cToken.setType(PLUS);
						_cToken.textPush('+');
						endToken();
					}
					_cToken.setType(INTEGER_LITERAL);
					_cToken.textPush('-');
					break;

				case '\n':
				case '\t':
				case ' ':
				case '\r':
				case '\v':
					if(!_cToken.getText().empty()){endToken();}
					break;

				default:
					throw std::runtime_error("unknown symbol!");
					break;
			}
		}
		if(_cToken.getType() != IDENTIFIER){endToken();}
	}

	std::vector<Token> Tokenizer::get_tokens(){return _tokens;}

	void Tokenizer::endToken()
	{
		if(_cToken.getType() != WHITESPACE){_tokens.push_back(_cToken);}
		_cToken.setType(WHITESPACE);
		_cToken.textErase();
	}

	void Tokenizer::debugPrint()
	{
		for(Token cTok : _tokens){std::cout << "Type: " << sTokenTypeStrings[cTok.getType()] << " Name: " << cTok.getText() 
			<< std::endl;}
	}
}
