#include "lexer.hpp"
#include "parser.hpp"

int main()
{
	//лексический анализ
	std::string input;
	Tokenizer lexer = Tokenizer();
	lexer.make_tokens(input);
	std::vector<Token> vars = lexer.get_variables();
	std::vector<Token> consts = lexer.get_constants();
	std::vector<Token> lexems = lexer.get_tokens();

	//инициализация терминальных токенов, кроме var и const
	Token lbrac = Token("(");
	Token rbrac = Token(")");
	Token forall = Token("forall");
	Token exists = Token("exists");
	Token reduct = Token("not");
	Token great = Token(">");
	Token equal = Token("=");
	Token geq = Token("geq");
	Token plus = Token("+");
	Token mult = Token("mult");
	Token arrow = Token("rightarrow");
	Token eps = Token("$");

	//объявление нетерминальных токенов
        Token Start = Token();
	Token Form = Token();
	Token Quantize = Token();
	Token Quantor = Token();
	Token Form1 = Token();
	Token Brform = Token();
	Token Imp = Token();
	Token Neg = Token();
	Token Expr = Token();
	Token Expr1 = Token();
	Token Sum = Token();
	Token Mult = Token();
	Token Mult1 = Token();
	Token Var = Token();
	Token Const = Token();
	Token Arrow = Token();
	Token Ord = Token();

	//заполнение полей нетерминальных токенов
	std::vector<Token> buffer;

	Start.set_rule({Form});
	Start.set_connects({forall, exists, lbrac});

	Form.set_rule({Quantize, Form1});
	Form.set_connects({forall, exists, lbrac});

	Quantize.set_rule({Quantor, Form1});
	Quantize.set_rule({eps});
	Quantize.set_connects({{forall, exists}});
	Quantize.set_connects({lbrac});

	Quantor.set_rule({forall, Var});
	Quantor.set_rule({exists, Var});
	Quantor.set_connects({forall});
	Quantor.set_connects({exists});

	Form1.set_rule({lbrac, Brform});
	Form1.set_connects({lbrac});

	Brform.set_rule({Expr});
	Brform.set_rule({Imp});
	Brform.set_rule({Neg});
	for(std::vector<Token>::iterator iter = vars.begin(); iter != vars.end(); ++iter){buffer.push_back(*iter);}
	for(std::vector<Token>::iterator iter = consts.begin(); iter != consts.end(); ++iter){buffer.push_back(*iter);}
	Brform.set_connects(buffer);
	Brform.set_connects({forall, exists, lbrac});
	Brform.set_connects({reduct});
	buffer.clear();

	Imp.set_rule({Form, Arrow, Form, rbrac});
	Imp.set_connects({forall, exists, lbrac});

	Neg.set_rule({reduct, Form, rbrac});
	Neg.set_connects({reduct});

	Expr.set_rule({Expr1, Ord, Expr1, rbrac});
	for(std::vector<Token>::iterator iter = vars.begin(); iter != vars.end(); ++iter){buffer.push_back(*iter);}
	for(std::vector<Token>::iterator iter = consts.begin(); iter != consts.end(); ++iter){buffer.push_back(*iter);}
	Expr.set_connects(buffer);
	buffer.clear();

	Expr1.set_rule({Mult, Sum});
	for(std::vector<Token>::iterator iter = vars.begin(); iter != vars.end(); ++iter){buffer.push_back(*iter);}
	for(std::vector<Token>::iterator iter = consts.begin(); iter != consts.end(); ++iter){buffer.push_back(*iter);}
	Expr1.set_connects(buffer);
	buffer.clear();

	Sum.set_rule({plus, Expr1});
	Sum.set_rule({eps});
	Sum.set_connects({plus});
	Sum.set_connects({rbrac, great, equal, geq});
	
	Mult.set_rule({Var, Mult1});
	Mult.set_rule({Const, Mult1});
	for(std::vector<Token>::iterator iter = vars.begin(); iter != vars.end(); ++iter){buffer.push_back(*iter);}
	Mult.set_connects(buffer);
	buffer.clear();
	for(std::vector<Token>::iterator iter = consts.begin(); iter != consts.end(); ++iter){buffer.push_back(*iter);}
	Mult.set_connects(buffer);
	buffer.clear();

	Mult1.set_rule({mult, Mult});
	Mult1.set_rule({eps});
	Mult1.set_connects({mult});
	Mult1.set_connects({rbrac, plus, great, equal, geq});

	for(std::vector<Token>::iterator iter = vars.begin(); iter != vars.end(); ++iter)
	{
		Var.set_rule({*iter});
		Var.set_connects({*iter});
	}

	for(std::vector<Token>::iterator iter = consts.begin(); iter != consts.end(); ++iter)
	{
		Const.set_rule({*iter});
		Const.set_connects({*iter});
	}

	Arrow.set_rule({arrow});
	Arrow.set_connects({arrow});

	Ord.set_rule({great});
	Ord.set_rule({equal});
	Ord.set_rule({geq});
	Ord.set_connects({great});
	Ord.set_connects({equal});
	Ord.set_connects({geq});

	//синтаксический анализ
	
}
