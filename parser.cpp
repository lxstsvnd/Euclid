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
		_mText = text;
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
//				std::cout << "curAnalyzed: " << sTokenTypeStrings[_upperNode->getType()] << std::endl;
			}
			else if(!bufNode->getMTerm() && !bufNode->getChildren().empty()){_getUpperNode(bufNode);}
			else if(bufNode->getMTerm() && !bufNode->getVTerm() && bufNode->getType() != EPSILON)
			{
				_upperNode = (Node*)bufNode;
//				std::cout << "curAnalyzed: " << sTokenTypeStrings[_upperNode->getType()] << std::endl;
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

	void parseTree::_merge(std::vector<std::pair<mpz_class, int>> a, std::vector<std::pair<mpz_class, int>> b)
	{
		int iter = 0; //итератор по a
		int jter = 0; //иетартор по b
		while(iter < a.size() && jter < b.size())
		{
			if(std::get<1>(a[iter]) < std::get<1>(b[jter]))
			{
				_resultCoefsTMP.push_back(a[iter]);
				++iter;
			}
			else if(std::get<1>(a[iter]) > std::get<1>(b[jter]))
			{
				std::pair<mpz_class, int> tmp(-std::get<0>(b[jter]), std::get<1>(b[jter]));
				_resultCoefsTMP.push_back(tmp);
				++jter;
			}
			else
			{
				std::pair<mpz_class, int> tmp(-std::get<0>(b[jter]) + std::get<0>(a[iter]), std::get<1>(b[jter]));
				_resultCoefsTMP.push_back(tmp);
				++iter;
				++jter;
			}
		}
		if(iter == a.size()) while(jter < b.size())
		{
			std::pair<mpz_class, int> tmp(-std::get<0>(b[jter]), std::get<1>(b[jter]));
			_resultCoefsTMP.push_back(tmp);
			++jter;
		}
		else while(iter < a.size())
		{
			_resultCoefsTMP.push_back(a[iter]);
			++iter;
		}
		//теперь нужно добавить нули, если соответствующая степень отсутствует
		jter = 0; //текущая степень в векторе
		for(iter = 0; iter <= std::get<1>(_resultCoefsTMP[_resultCoefsTMP.size() - 1]); ++iter)
		{
			if(jter > _resultCoefsTMP.size()){break;}
			while(iter < std::get<1>(_resultCoefsTMP[jter]))
			{
				_resultCoefs.push_back(0);
				++iter;
			}
			_resultCoefs.push_back(std::get<0>(_resultCoefsTMP[jter]));
			++jter;
		}
	}

	void parseTree::parse()
	{
		for(const Token cToken : _Line)
		{
			bool j = true;
			while(j)
			{
				getUpperNode(); //обновление указателя на раскрываемый нетерминал
//				std::cout << sTokenTypeStrings[_upperNode->getType()] << " " << sTokenTypeStrings[cToken.getType()] 
//					<< std::endl;
				switch(_ifMatched(_upperNode, cToken))
				{
					case 0:
						_deleteFromNode(_root);
						throw std::runtime_error("Syntax error!");
						break;
					case 1:
						break;
					case 2:
						_upperNode->setVTerm(1);
						j = false;
						break;
					case 3:
//						std::cout << "SUCCESS!" << std::endl << std::endl;
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
						return 0;
						break;
				}
				break;
			default:
				if(nTerm->getMTerm() && nTerm->getType() == Term.getType()) return 2;
				return 0;
				break;
		}

//		std::cout << "Rule: ";								//немного косметической работы
//		for(int iter = 0; iter < bufUncov.size(); ++iter)				//
//		{								 		//
//			std::cout << sTokenTypeStrings[bufUncov[iter]->getType()] << " ";	//
//		}										//
//		std::cout << std::endl << std::endl;						//
//
		for(int iter = 0; iter < bufUncov.size(); ++iter){_addNode(bufUncov[iter]);} //заполнение потомков нетерминала
		bufUncov.clear();
		return flag;
	}

	void parseTree::_makePredicate(const Node* fromNode)
	{
//		std::cout << "Trying to make predicate: " << sTokenTypeStrings[fromNode->getType()] << std::endl;
		if(!fromNode->getMTerm() && fromNode->getChildren()[0]->getType() == RESISTANCE)
		{
			_negFlag = true;
			_makePredicate(fromNode->getChildren()[1]);
		}
		else if(!fromNode->getMTerm()){for(const Node* toNode : fromNode->getChildren()){_makePredicate(toNode);}}
		else
		{
//			std::cout << "We are in leaf: " << sTokenTypeStrings[fromNode->getType()] << std::endl;
			switch(fromNode->getType())
			{
				case INTEGER_LITERAL:
//					std::cout << "It's integer: " << std::stoi(fromNode->getText()) << std::endl;
					_curCoefficient = std::stoi(fromNode->getText());
					break;
				case IDENTIFIER:
//					std::cout << "It's x" << std::endl;
					++_degree;
					break;
				case PLUS:
//					std::cout << "It's +" << std::endl;
					if(!_leftFlag){_leftPolynomCoefs.push_back(std::make_pair(_curCoefficient, _degree));}
					else{_rightPolynomCoefs.push_back(std::make_pair(_curCoefficient, _degree));}
					_degree = 0;
					_curCoefficient = 1;
					break;
				case GREATER:
				case EQUAL:
//					std::cout << "Left completed" << std::endl;
					_leftPolynomCoefs.push_back(std::make_pair(_curCoefficient, _degree));
					_degree = 0;
					_curCoefficient = 1;
					_leftFlag = true;
					break;
				default:
					break;
			}
		}

	}

	void parseTree::_makeConjunct(const Node* fromNode)
	{
//		std::cout << "In _makeConjunct: " << sTokenTypeStrings[fromNode->getType()] << std::endl;
		switch(fromNode->getType())
		{
			case PREDICATE:
				switch(fromNode->getChildren().size())
				{
					case 2:
						_negFlag = true;
						_makeConjunct(fromNode->getChildren()[1]);
						break;
					case 3:
//						std::cout << "Predicate has been caught" << std::endl;
						_makePredicate(fromNode);
						_rightPolynomCoefs.push_back(std::make_pair(_curCoefficient, _degree));
						_curCoefficient = 1;
						_degree = 0;
						for(int iter = 0; iter < _rightPolynomCoefs.size(); ++iter)
						{
//							std::cout << "!" << std::get<0>(_rightPolynomCoefs[iter]) << "!" << std::endl;
						}

//						std::cout << "Predicate made" << std::endl; //эта и следующая - косметика
						for(std::pair<mpz_class, int> curPair : _leftPolynomCoefs)
						{
//							std::cout << std::get<0>(curPair) << " " << std::get<1>(curPair) << std::endl;
						}
						for(std::pair<mpz_class, int> curPair : _rightPolynomCoefs)
						{
//							std::cout << std::get<0>(curPair) << " " << std::get<1>(curPair) << std::endl;
						}

						std::sort(_leftPolynomCoefs.begin(), _leftPolynomCoefs.end(),
								[](std::pair<mpz_class, int> &s1, std::pair<mpz_class, int> &s2){
								return std::get<1>(s1) < std::get<1>(s2);});
						std::sort(_rightPolynomCoefs.begin(), _rightPolynomCoefs.end(),
								[](std::pair<mpz_class, int> &s1, std::pair<mpz_class, int> &s2){
								return std::get<1>(s1) < std::get<1>(s2);});
//						std::cout << "Start merging" << std::endl;
						_merge(_leftPolynomCoefs, _rightPolynomCoefs);
					
						for(std::pair<mpz_class, int> curPair : _resultCoefsTMP)
						{
//							std::cout << std::get<0>(curPair) << " " << std::get<1>(curPair) << std::endl;
						}
//						std::cout << "After merging" << std::endl;
						for(mpz_class cur : _resultCoefs)
						{
//							std::cout << cur << std::endl;
						}
//						std::cout << std::endl;
		
						switch(fromNode->getChildren()[1]->getChildren()[0]->getType()) //проверка предиката [>] или [=]
						{
							case GREATER:
//								std::cout << "Greater predicate" << std::endl;
								_conjunct.add_predicate(new Kirill::Greater_predicate(
											Kirill::Polynom(_resultCoefs), _negFlag));
								_leftPolynomCoefs.clear();
								_rightPolynomCoefs.clear();
								_resultCoefsTMP.clear();
								_resultCoefs.clear();
								_negFlag = false;
								_leftFlag = false;
								break;
							case EQUAL:
//								std::cout << "Equal predicate" << std::endl;
								_conjunct.add_predicate(new Kirill::Equality_predicate(Kirill::Polynom(
												_resultCoefs), _negFlag));
								_leftPolynomCoefs.clear();
								_rightPolynomCoefs.clear();
								_resultCoefsTMP.clear();
								_resultCoefs.clear();
								_negFlag = false;
								_leftFlag = false;
								break;
						}
						break;
				}
				break;
			default:
				for(const Node* toNode : fromNode->getChildren()){_makeConjunct(toNode);}
		}
	}

	void parseTree::_makeDNF(const Node* fromNode)
	{
//		std::cout << "Making DNF: " << sTokenTypeStrings[fromNode->getType()] << std::endl;
		switch(fromNode->getType())
		{
			case CONJ:
				_makeConjunct(fromNode); //конъюнкт выгружается в поле из private
//				std::cout << "Try to add conjunct" << std::endl;
				_dnf.add_conjunct(_conjunct); //конъюнкт добавляется в _dnf
//				std::cout << "Conjunct has been added" << std::endl;
				_conjunct.clear(); //_conjunct очищается
				break;
			default:
				for(const Node* toNode : fromNode->getChildren()){_makeDNF(toNode);}
		}
	}

	void parseTree::makeDNF()
	{
		_makeDNF(_root);
//		std::cout << std::endl;
	}
	Kirill::DNF parseTree::getDNF(){return _dnf;}
}
