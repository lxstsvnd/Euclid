#include "lexer.hpp"

namespace Dima
{
	Token::Token(enum TokenType type){mType = type;}
	Token::Token(){;}

	Tokenizer::Tokenizer(const std::string& Line) //строка разбирается в конструкторе
	{
		for(char cCh : Line) //cCh = current character
		{
			if(_cToken.mType == SPECIAL_SEQUENCE)
			{
				switch(cCh)
				{
					case 'd': //дизъюнкция
						_cToken.mText.push_back('d');
						_cToken.mType = DISJOINT;
						break;
					case 'c': //конъюнкция
						_cToken.mText.push_back('c');
						_cToken.mType = CONJUNCT;
						break;
					case 'n': //отрицание
						_cToken.mText.push_back('n');
						_cToken.mType = RESISTANCE;
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
					if(_cToken.mType == WHITESPACE || _cToken.mType == INTEGER_LITERAL)
					{
						_cToken.mType = INTEGER_LITERAL;
						_cToken.mText.push_back(cCh);
					}else{throw std::runtime_error("0-9 is not in integer literal");}
					break;

				case '\\': //выловлено начало спецсимвола
					if(!_cToken.mText.empty()){endToken();}
					_cToken.mType = SPECIAL_SEQUENCE;
					break;

				case '(':
					if(!_cToken.mText.empty()){endToken();}
					_cToken.mType = LBRAC;
					_cToken.mText.push_back('(');
					endToken();
					break;
				case ')':
					if(!_cToken.mText.empty()){endToken();}
					_cToken.mType = RBRAC;
					_cToken.mText.push_back(')');
					endToken();
					break;
				case '+':
					if(!_cToken.mText.empty()){endToken();}
					_cToken.mType = PLUS;
					_cToken.mText.push_back('+');
					endToken();
					break;
				case '*':
					if(!_cToken.mText.empty()){endToken();}
					_cToken.mType = MULTIPLY;
					_cToken.mText.push_back('*');
					endToken();
					break;
				case '^':
					if(!_cToken.mText.empty()){endToken();}
					_cToken.mType = POWER;
					_cToken.mText.push_back('^');
					endToken();
					break;
				case '=':
					if(!_cToken.mText.empty()){endToken();}
					_cToken.mType = EQUAL;
					_cToken.mText.push_back('=');
					endToken();
					break;
				case '>':
					if(!_cToken.mText.empty()){endToken();}
					_cToken.mType = GREATER;
					_cToken.mText.push_back('>');
					endToken();
					break;
				case 'x':
					if(!_cToken.mText.empty()){endToken();}
					_cToken.mType = IDENTIFIER;
					_cToken.mText.push_back('x');
					endToken();
					_cToken.mType = IDENTIFIER;
					break;
				case '-': //x-c -> [x] [+] [-c]
					if(_cToken.mType != WHITESPACE)
					{
						if(!_cToken.mText.empty()){endToken();}
						_cToken.mType = PLUS;
						_cToken.mText.push_back('+');
						endToken();
					}
					_cToken.mType = INTEGER_LITERAL;
					_cToken.mText.push_back('-');
					break;

				case '\n':
				case '\t':
				case ' ':
				case '\r':
				case '\v':
					if(!_cToken.mText.empty()){endToken();}
					break;

				default:
					throw std::runtime_error("unknown symbol!");
					break;
			}
		}
		endToken();
	}

	std::vector<Token> Tokenizer::get_tokens(){return _tokens;}

	void Tokenizer::endToken()
	{
		if(_cToken.mType != WHITESPACE){_tokens.push_back(_cToken);}
		_cToken.mType = WHITESPACE;
		_cToken.mText.erase();
	}

	void Tokenizer::debugPrint()
	{
		for(Token cTok : _tokens){std::cout << "Type: " << sTokenTypeStrings[cTok.mType] << " Name: " << cTok.mText << std::endl;}
	}
}
