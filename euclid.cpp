#include "lexer.hpp"
#include "parser.hpp"
#include "iostream"

int main()
{
	//лексический анализ
	std::string input;
	std::cin >> input;
	Tokenizer lexer = Tokenizer();
	lexer.make_tokens(input);
	std::vector<Token> vars = lexer.get_variables();
	std::vector<Token> consts = lexer.get_constants();
	std::vector<Token> lexems = lexer.get_tokens();

	//инициализация терминальных токенов, кроме var и const
	Token* lbrac = new Token("(");
	Token* rbrac = new Token(")");
	Token* forall = new Token("forall");
	Token* exists = new Token("exists");
	Token* reduct = new Token("not");
	Token* great = new Token(">");
	Token* equal = new Token("=");
	Token* geq = new Token("geq");
	Token* plus = new Token("+");
	Token* mult = new Token("mult");
	Token* arrow = new Token("rightarrow");
	Token* eps = new Token("$");

	//объявление нетерминальных токенов
        Token* Start = new Token("START");
	Token* Form = new Token("FORM");
	Token* Quantize = new Token("QUANTIZE");
	Token* Quantor = new Token("QUANTOR");
	Token* Form1 = new Token("FORM1");
	Token* Brform = new Token("BRFORM");
	Token* Imp = new Token("IMP");
	Token* Neg = new Token("NEG");
	Token* Expr = new Token("EXPR");
	Token* Expr1 = new Token("EXPR1");
	Token* Sum = new Token("SUM");
	Token* Mult = new Token("MULT");
	Token* Mult1 = new Token("MULT1");
	Token* Var = new Token("VAR");
	Token* Const = new Token("CONST");
	Token* Arrow = new Token("ARROW");
	Token* Ord = new Token("ORD");

	//заполнение полей нетерминальных токенов
	std::vector<Token*> buffer;

	Start->set_rule({Form});
	Start->set_connects({forall, exists, lbrac});

	Form->set_rule({Quantize, Form1});
	Form->set_connects({forall, exists, lbrac});

	Quantize->set_rule({Quantor, Quantize});
	Quantize->set_rule({eps});
	Quantize->set_connects({{forall, exists}});
	Quantize->set_connects({lbrac});

	Quantor->set_rule({forall, Var});
	Quantor->set_rule({exists, Var});
	Quantor->set_connects({forall});
	Quantor->set_connects({exists});

	Form1->set_rule({lbrac, Brform});
	Form1->set_connects({lbrac});

	Brform->set_rule({Expr});
	Brform->set_rule({Imp});
	Brform->set_rule({Neg});
	for(int iter = 0; iter < vars.size(); ++iter){buffer.push_back(&vars[iter]);}
	for(int iter = 0; iter < consts.size(); ++iter){buffer.push_back(&consts[iter]);}
	Brform->set_connects(buffer);
	Brform->set_connects({forall, exists, lbrac});
	Brform->set_connects({reduct});
	buffer.clear();

	Imp->set_rule({Form, Arrow, Form, rbrac});
	Imp->set_connects({forall, exists, lbrac});

	Neg->set_rule({reduct, Form, rbrac});
	Neg->set_connects({reduct});

	Expr->set_rule({Expr1, Ord, Expr1, rbrac});
	for(int iter = 0; iter < vars.size(); ++iter){buffer.push_back(&vars[iter]);}
	for(int iter = 0; iter < consts.size(); ++iter){buffer.push_back(&consts[iter]);}
	Expr->set_connects(buffer);
	buffer.clear();

	Expr1->set_rule({Mult, Sum});
	for(int iter = 0; iter < vars.size(); ++iter){buffer.push_back(&vars[iter]);}
	for(int iter = 0; iter < consts.size(); ++iter){buffer.push_back(&consts[iter]);}
	Expr1->set_connects(buffer);
	buffer.clear();

	Sum->set_rule({plus, Expr1});
	Sum->set_rule({eps});
	Sum->set_connects({plus});
	Sum->set_connects({rbrac, great, equal, geq});
	
	Mult->set_rule({Var, Mult1});
	Mult->set_rule({Const, Mult1});
	for(int iter = 0; iter < vars.size(); ++iter){buffer.push_back(&vars[iter]);}
	Mult->set_connects(buffer);
	buffer.clear();
	for(int iter = 0; iter < consts.size(); ++iter){buffer.push_back(&consts[iter]);}
	Mult->set_connects(buffer);
	buffer.clear();

	Mult1->set_rule({mult, Mult});
	Mult1->set_rule({eps});
	Mult1->set_connects({mult});
	Mult1->set_connects({rbrac, plus, great, equal, geq});

	for(int iter = 0; iter < vars.size(); ++iter)
	{
		Var->set_rule({&vars[iter]});
		Var->set_connects({&vars[iter]});
	}

	for(int iter = 0; iter < consts.size(); ++iter)
	{
		Const->set_rule({&consts[iter]});
		Const->set_connects({&consts[iter]});
	}

	Arrow->set_rule({arrow});
	Arrow->set_connects({arrow});

	Ord->set_rule({great});
	Ord->set_rule({equal});
	Ord->set_rule({geq});
	Ord->set_connects({great});
	Ord->set_connects({equal});
	Ord->set_connects({geq});

	//синтаксический анализ
	Parser analyzer = Parser(lexems, Start);
	bool if_parsed = analyzer.parse();
	printf("%d\n", if_parsed);
}
