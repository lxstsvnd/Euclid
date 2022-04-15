#include "parser.hpp"

namespace Dima
{
	Node::Node(int isTerm, enum TokenType type)
	{
		_mType = type;
		_mTerm = isTerm;
		_chPtr.clear();
	}
	Node::Node(int isTerm, std::string text, enum TokenType type)
	{
		_mType = type;
		_mText = std::atoi(_mText.c_str());
		_mTerm = isTerm;
		_chPtr.clear();
	}
	Node::Node(){;}

	void Node::setType(enum TokenType newType){_mType = newType;}
	void Node::setText(std::string newText){_mText = newText;}
	void Node::setMTerm(bool mTerm){_mTerm = mTerm;}
	void Node::setVTerm(bool vTerm){_vTerm = vTerm;}

	enum TokenType Node::getType() const{return _mType;}
	std::string Node::getText() const{return _mText;}
	bool Node::getMTerm() const{return _mTerm;}
	bool Node::getVTerm() const{return _vTerm;}

	std::vector<const Node*> Node::getChildren() const{return _chPtr;}
	void Node::addChild(const Node* addedNode){_chPtr.push_back(addedNode);}

	void parseTree::_getUpperNode(const Node* toNode)
	{
		_upperNode = _root;
		for(const Node* bufNode : toNode->getChildren())
		{
			if(_upperNode != _root) return; //если ссылка на нетерминал получена, функция завершает работу

			if(!bufNode->getMTerm() && bufNode->getChildren().empty())
			{
				_upperNode = (Node*)bufNode;
				std::cout << "curAnalyzed: " << sTokenTypeStrings[_upperNode->getType()] << std::endl;
			}
			else if(!bufNode->getMTerm() && !bufNode->getChildren().empty()){_getUpperNode(bufNode);}
			else if(bufNode->getMTerm() && !bufNode->getVTerm() && bufNode->getType() != EPSILON)
			{
				_upperNode = (Node*)bufNode;
				std::cout << "curAnalyzed: " << sTokenTypeStrings[_upperNode->getType()] << std::endl;
			}
		}
	}

	parseTree::parseTree(const std::vector<Token>& Line)
	{
		_Line = Line;
		_Line.push_back(Token(END)); //указатель на конец строки
		_root = new Node(0, BLACKSPACE); //корень изначально рабочий нетерминал
	}

	parseTree::~parseTree()
	{
		_deleteFromNode(_root);
	}

	void parseTree::_deleteFromNode(const Node* fromNode)
	{
		for(const Node* curNode : fromNode->getChildren()){_deleteFromNode(curNode);}
		delete fromNode;
		fromNode = NULL;
	}

	void parseTree::getUpperNode(){_getUpperNode(_root);}
	void parseTree::_addNode(const Node* addedNode){_upperNode->addChild(addedNode);}
	void parseTree::parse()
	{
		for(const Token cToken : _Line)
		{
			bool j = true;
			while(j)
			{
				getUpperNode(); //обновление указателя на раскрываемый нетерминал
				std::cout << sTokenTypeStrings[_upperNode->getType()] << " " << sTokenTypeStrings[cToken.getType()] 
					<< std::endl;
				switch(_ifMatched(_upperNode, cToken))
				{
					case 0:
						throw std::runtime_error("Syntax error!");
						break;
					case 1:
						break;
					case 2:
						_upperNode->setVTerm(1);
						j = false;
						break;
					case 3:
						std::cout << "SUCCESS!" << std::endl << std::endl;
						j = false;
						break;
				}
			}
		}
	}

	const int parseTree::_ifMatched(Node* nTerm, Token Term)
	{
		std::vector<Node*> bufUncov; //буфер для раскрытого нетерминала
		int flag; //wrong syntax if 0
			  //it was nonterminal if 1
			  //it was terminal if 2
			  //success if 3
		switch(nTerm->getType())
		{
			case END:
				flag = 3;
				switch(Term.getType())
				{
					case END:
						break;
					default:
						_deleteFromNode(_root);
						return 0;
						break;
				}
				break;
			case BLACKSPACE:
				flag = 1;
				switch(Term.getType())
				{
					case LBRAC:
					case IDENTIFIER:
					case INTEGER_LITERAL:
					case RESISTANCE:
						bufUncov.push_back(new Node(0, DNF));
						bufUncov.push_back(new Node(0, END));
						break;
					default:
						_deleteFromNode(_root);
						return 0;
						break;
				}
				break;
			case DNF:
				flag = 1;
				switch(Term.getType())
				{
					case LBRAC:
						bufUncov.push_back(new Node(1, LBRAC));
						bufUncov.push_back(new Node(0, CONJ));
						bufUncov.push_back(new Node(1, RBRAC));
						bufUncov.push_back(new Node(1, DISJOINT));
						bufUncov.push_back(new Node(0, DNF));
						break;
					case IDENTIFIER:
					case INTEGER_LITERAL:
					case RESISTANCE:
						bufUncov.push_back(new Node(0, CONJ));
						break;
					default:
						_deleteFromNode(_root);
						return 0;
						break;
				}
				break;
			case DNF_TMP:
				flag = 1;
				switch(Term.getType())
				{
					case DISJOINT:
						bufUncov.push_back(new Node(1, DISJOINT));
						bufUncov.push_back(new Node(0, DNF));
						break;
					default:
						_deleteFromNode(_root);
						return 0;
						break;
				}
				break;
			case CONJ_TMP:
				flag = 1;
				switch(Term.getType())
				{
					case END:
					case RBRAC:
						bufUncov.push_back(new Node(1, EPSILON));
						break;
					case CONJUNCT:
						bufUncov.push_back(new Node(1, CONJUNCT));
						bufUncov.push_back(new Node(0, CONJ));
						break;
					default:
						_deleteFromNode(_root);
						return 0;
						break;
				}
				break;
			case CONJ:
				flag = 1;
				switch(Term.getType())
				{
					case IDENTIFIER:
					case INTEGER_LITERAL:
					case RESISTANCE:
						bufUncov.push_back(new Node(0, PREDICATE));
						bufUncov.push_back(new Node(0, CONJ_TMP));
						break;
					default:
						_deleteFromNode(_root);
						return 0;
						break;
				}
				break;
			case PREDICATE:
				flag = 1;
				switch(Term.getType())
				{
					case IDENTIFIER:
					case INTEGER_LITERAL:
						bufUncov.push_back(new Node(0, POLYNOM));
						bufUncov.push_back(new Node(0, ORDER));
						bufUncov.push_back(new Node(0, POLYNOM));
						break;
					case RESISTANCE:
						bufUncov.push_back(new Node(1, RESISTANCE));
						bufUncov.push_back(new Node(0, PREDICATE));
						break;
					default:
						_deleteFromNode(_root);
						return 0;
						break;
				}
				break;
			case POLYNOM:
				flag = 1;
				switch(Term.getType())
				{
					case IDENTIFIER:
					case INTEGER_LITERAL:
						bufUncov.push_back(new Node(0, MULT));
						bufUncov.push_back(new Node(0, SUM));
						break;
					default:
						_deleteFromNode(_root);
						return 0;
						break;
				}
				break;
			case ORDER:
				flag = 1;
				switch(Term.getType())
				{
					case GREATER:
						bufUncov.push_back(new Node(1, GREATER));
						break;
					case EQUAL:
						bufUncov.push_back(new Node(1, EQUAL));
						break;
					default:
						_deleteFromNode(_root);
						return 0;
						break;
				}
				break;
			case SUM:
				flag = 1;
				switch(Term.getType())
				{
					case END:
					case CONJUNCT:
					case RBRAC:
					case GREATER:
					case EQUAL:
						bufUncov.push_back(new Node(1, EPSILON));
						break;
					case PLUS:
						bufUncov.push_back(new Node(1, PLUS));
						bufUncov.push_back(new Node(0, POLYNOM));
						break;
					default:
						_deleteFromNode(_root);
						return 0;
						break;
				}
				break;
			case MULT:
				flag = 1;
				switch(Term.getType())
				{
					case INTEGER_LITERAL:
						bufUncov.push_back(new Node(0, CONST));
						bufUncov.push_back(new Node(0, MULT_TMP));
						break;
					case IDENTIFIER:
						bufUncov.push_back(new Node(1, IDENTIFIER));
						bufUncov.push_back(new Node(0, MULT_TMP));
						break;
					default:
						_deleteFromNode(_root);
						return 0;
						break;
				}
				break;
			case MULT_TMP:
				flag = 1;
				switch(Term.getType())
				{
					case END:
					case CONJUNCT:
					case RBRAC:
					case GREATER:
					case EQUAL:
					case PLUS:
						bufUncov.push_back(new Node(1, EPSILON));
						break;
					case MULTIPLY:
						bufUncov.push_back(new Node(1, MULTIPLY));
						bufUncov.push_back(new Node(0, MULT));
						break;
					case IDENTIFIER:
					case CONST:
						bufUncov.push_back(new Node(0, MULT));
						break;
					default:
						_deleteFromNode(_root);
						return 0;
						break;
				}
				break;
			case CONST:
				flag = 1;
				switch(Term.getType())
				{
					case INTEGER_LITERAL:
						bufUncov.push_back(new Node(1, Term.getText(), INTEGER_LITERAL));
						break;
					default:
						_deleteFromNode(_root);
						return 0;
						break;
				}
				break;
			default:
				if(nTerm->getMTerm() && nTerm->getType() == Term.getType()) return 2;
				_deleteFromNode(_root);
				return 0;
				break;
		}

		std::cout << "Rule: ";								//немного косметической работы
		for(int iter = 0; iter < bufUncov.size(); ++iter)				//
		{								 		//
			std::cout << sTokenTypeStrings[bufUncov[iter]->getType()] << " ";	//
		}										//
		std::cout << std::endl << std::endl;						//

		for(int iter = 0; iter < bufUncov.size(); ++iter){_addNode(bufUncov[iter]);} //заполнение потомков нетерминала
		bufUncov.clear();
		return flag;
	}

	std::vector<Kirill::Predicate*> parseTree::_makeConjunct(const Node* fromNode)
	{
		std::vector<Kirill::Predicate*> curPredicates{};
		curPredicates.push_back(_makePredicate(fromNode->getChildren()[0])); //первый потомок - всегда [PREDICATE]
		switch(fromNode->getChildren()[1]->getChildren().size()) //второй потомок - всегда [CONJ_TMP]
		{
			case 1: //[CONJ_TMP] раскрылся в [EPSILON]
				std::cout << "CONJ -> EPSILON" << std::endl;
				break;
			case 2: //[CONJ_TMP] раскрылся в [CONJUNCT][CONJ]
				std::cout << "CONJ -> AND CONJ" << std::endl;
				_conjuncts.push_back(_makeConjunct(fromNode->getChildren()[1]->getChildren()[1]));
				break;
		}
		return curPredicates;
	}

	Kirill::Predicate* parseTree::_makePredicate(const Node* fromNode)
	{
		switch(fromNode->getChildren().size())
		{
			case 2: //если предикат раскрылся в отрицание предиката
				switch(_negotiated) 
				{
					case true:
						_negotiated = false;
						break;
					case false:
						_negotiated = true;
						break;
				}
				return _makePredicate(fromNode->getChildren()[1]);
				break;
			case 3: //если предикат раскрылся в неравенство
				std::cout << "Start making unequality" << std::endl;
				Kirill::Predicate* curPredicate;

				_makePolynom(_curLPolynom, fromNode->getChildren()[0]); //собираются левый и правый полиномы
				_makePolynom(_curRPolynom, fromNode->getChildren()[2]); //

				std::vector<mpz_class> curCoefficients{}; //далее считается разность коэффициентов
				if(_curLPolynom->get_coefficients().size() >= _curRPolynom->get_coefficients().size())
				{
					for(int iter = 0; iter < _curRPolynom->get_coefficients().size(); ++iter)
					{
						curCoefficients.push_back(_curLPolynom->get_coefficients()[iter] 
								-_curRPolynom->get_coefficients()[iter]);
					}
					for(int iter = _curRPolynom->get_coefficients().size(); iter < _curLPolynom->get_coefficients().size()
								; ++iter){curCoefficients.push_back(_curLPolynom->get_coefficients()[iter]);}
				}
				else
				{
					for(int iter = 0; iter < _curLPolynom->get_coefficients().size(); ++iter)
					{
						curCoefficients.push_back(_curLPolynom->get_coefficients()[iter]
								-_curRPolynom->get_coefficients()[iter]);
					}
					for(int iter = _curLPolynom->get_coefficients().size(); iter < _curRPolynom->get_coefficients().size()
							; ++iter){curCoefficients.push_back(0 - _curLPolynom->get_coefficients()[iter]);}
				}

				switch(fromNode->getChildren()[1]->getChildren()[0]->getType()) //инициализируется один из двух предикатов
				{
					case GREATER:
						curPredicate = new Kirill::Greater_predicate(Kirill::Polynom(curCoefficients), _negotiated);
						break;
					case EQUAL:
						curPredicate = new Kirill::Equality_predicate(Kirill::Polynom(curCoefficients), _negotiated);
						break;
				}

				_negotiated = false;
				delete _curLPolynom;
				delete _curRPolynom;
				_curLPolynom = NULL;
				_curRPolynom = NULL;
				std::cout << "Unequality made" << std::endl;
				return curPredicate;
				break;
		}
		throw std::runtime_error("aaa");
	}

	void parseTree::_makePolynomSpecial(const Node* fromNode, int _coef)
	{
		int coef = _coef; //вспомогательный коэффициент
		switch(fromNode->getChildren()[0]->getChildren()[0]->getType()) //первый всегда [MULT]
		{
			case IDENTIFIER:
				++degree;
				break;
			case CONST:
				coef = std::atoi(fromNode->getChildren()[0]->getChildren()[0]->getChildren()[0]->getText().c_str());
				break;
		}

		switch(fromNode->getChildren()[0]->getChildren()[1]->getChildren()[0]->getType())
		{
			case EPSILON:
				break;
			case MULT:
				_makePolynomSpecial(fromNode->getChildren()[0]->getChildren()[1]->getChildren()[0], coef);
				break;
			case MULTIPLY:
				_makePolynomSpecial(fromNode->getChildren()[0]->getChildren()[1]->getChildren()[1], coef);
				break;
		}

		_coefficients.push_back(std::pair<mpz_class, int>{coef, degree});
		coef = 1;
		degree = 0;
	}

	void parseTree::_makePolynom(Kirill::Polynom* toPolynom, const Node* fromNode) //вводится всегда в порядке убывания степени
	{									       //[^] пока не реализован
		int curPos{0}; //для отслеживания текущей позиции в векторе коэффициентов
		std::cout << "Start making polynom" << std::endl;
		_makePolynomSpecial(fromNode, 1);

		switch(fromNode->getChildren()[1]->getChildren()[0]->getType())
		{
			case PLUS:
				_makePolynom(toPolynom, fromNode->getChildren()[1]->getChildren()[1]);
				break;
			case EPSILON:
				break;
		}

		std::vector<mpz_class> actualCoefs; //вектор коэффициентов в приемлемом для конструктора виде
		std::sort(_coefficients.begin(), _coefficients.end(), 
				[](std::pair<mpz_class, int> &a, std::pair<mpz_class, int> &b){return std::get<1>(a) < std::get<1>(b);});
		for(std::pair<mpz_class, int> curPair : _coefficients)
		{
			while(curPos < std::get<1>(curPair))
			{
				actualCoefs.push_back(0);
				++curPos;
			}
			actualCoefs.push_back(std::get<0>(curPair));
			++curPos;
		}
		toPolynom = new Kirill::Polynom(actualCoefs);
		std::cout << "Polynom made" << std::endl;
		_coefficients.clear();
	}

	void parseTree::makeConjuncts(){_getConjuncts(_root);}
	void parseTree::_getConjuncts(const Node* fromNode) //[BLACK] всегда раскрывается в [DNF] [END]
	{
		switch(fromNode->getChildren()[0]->getChildren().size())
		{
			case 1: //[DNF] раскрылся в [CONJ]
				std::cout << "In a CONJ" << std::endl;
				_conjuncts.push_back(Kirill::Conjunct(_makeConjunct(fromNode->getChildren()[0]->getChildren()[0])));
				break;
			case 5: //[DNF] раскрылся в [(][CONJ][)][DISJOINT][DNF]
				_conjuncts.push_back(Kirill::Conjunct(_makeConjunct(fromNode->getChildren()[0]->getChildren()[1])));
				_getConjuncts(fromNode->getChildren()[0]->getChildren()[4]);
				break;
		}
	}
	
	std::vector<Kirill::Conjunct> parseTree::getConjuncts(){return _conjuncts;}
}

