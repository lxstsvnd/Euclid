#include "parser.hpp"

namespace dnf_parser
{
	Node::Node(int isTerm, enum TokenType type)
	{
		mType = type;
		mTerm = isTerm;
		_chPtr.clear();
	}
	Node::Node(){;}
	std::vector<const Node*> Node::getChildren() const{return _chPtr;}
	void Node::addChild(const Node* addedNode){_chPtr.push_back(addedNode);}

	void parseTree::_getUpperNode(const Node* toNode)
	{
		_upperNode = _root;
		for(const Node* bufNode : toNode->getChildren())
		{
			if(_upperNode != _root) return;
			//std::cout << sTokenTypeStrings[bufNode->mType] << " ";
			if(!bufNode->mTerm && bufNode->getChildren().empty())
			{
				_upperNode = (Node*)bufNode;
				std::cout << "curAnalyzed: " << sTokenTypeStrings[_upperNode->mType] << std::endl;
				//return;
			}else if(!bufNode->mTerm && !bufNode->getChildren().empty())
			{
				_getUpperNode(bufNode);
				//return;
			}
		}
		//std::cout << std::endl;
		return;
	}

	parseTree::parseTree(const std::vector<Token>& Line)
	{
		_Line = Line;
		_Line.push_back(Token(END)); //добавляем указатель на конец строки
		_root = new Node(0, BLACKSPACE); //корень изначально рабочий нетерминал
	}
	void parseTree::getUpperNode(){_getUpperNode(_root);}
	void parseTree::_addNode(const Node* addedNode){_upperNode->addChild(addedNode);}
	void parseTree::parse()
	{
		for(const Token cToken : _Line)
		{
			bool j = true;
			while(j == true)
			{
				getUpperNode();
				std::cout << sTokenTypeStrings[_upperNode->mType] << " " << sTokenTypeStrings[cToken.mType] << std::endl;
				int flag = _ifMatched(_upperNode, cToken);
				switch(flag)
				{
					case 0:
						throw std::runtime_error("Syntax error!");
						break;
					case 1:
						break;
					case 2:
						j = false;
						break;
					case 3:
						std::cout << "SUCCESS!" << std::endl;
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
		switch(nTerm->mType)
		{
			case END:
				flag = 3;
				switch(Term.mType)
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
				switch(Term.mType)
				{
					case LBRAC:
					case IDENTIFIER:
					case INTEGER_LITERAL:
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
				switch(Term.mType)
				{
					case LBRAC:
					case IDENTIFIER:
					case INTEGER_LITERAL:
						bufUncov.push_back(new Node(0, CONJ));
						bufUncov.push_back(new Node(0, DNF_TMP));
						break;
					default:
						return 0;
						break;
				}
				break;
			case DNF_TMP:
				flag = 1;
				switch(Term.mType)
				{
					case END:
						bufUncov.push_back(new Node(1, EPSILON));
						break;
					case DISJOINT:
						bufUncov.push_back(new Node(1, DISJOINT));
						bufUncov.push_back(new Node(0, DNF));
						break;
					default:
						return 0;
						break;
				}
				break;
			case CONJ:
				flag = 1;
				switch(Term.mType)
				{
					case LBRAC:
						bufUncov.push_back(new Node(1, LBRAC));
						bufUncov.push_back(new Node(0, PREDICATE));
						bufUncov.push_back(new Node(1, CONJUNCT));
						bufUncov.push_back(new Node(0, CONJ));
						bufUncov.push_back(new Node(1, RBRAC));
						break;
					case IDENTIFIER:
					case INTEGER_LITERAL:
						bufUncov.push_back(new Node(0, PREDICATE));
						break;
					default:
						return 0;
						break;
				}
				break;
			case PREDICATE:
				flag = 1;
				switch(Term.mType)
				{
					case IDENTIFIER:
					case INTEGER_LITERAL:
						bufUncov.push_back(new Node(0, POLYNOM));
						bufUncov.push_back(new Node(0, ORDER));
						bufUncov.push_back(new Node(0, POLYNOM));
						break;
					default:
						return 0;
						break;
				}
				break;
			case POLYNOM:
				flag = 1;
				switch(Term.mType)
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
				switch(Term.mType)
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
				switch(Term.mType)
				{
					case END:
					case DISJOINT:
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
				switch(Term.mType)
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
				switch(Term.mType)
				{
					case END:
					case DISJOINT:
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
					default:
						return 0;
						break;
				}
				break;
			case CONST:
				flag = 1;
				switch(Term.mType)
				{
					case INTEGER_LITERAL:
						bufUncov.push_back(new Node(1, INTEGER_LITERAL));
						break;
					default:
						return 0;
						break;
				}
				break;
			default:
				if(nTerm->mTerm && nTerm->mType == Term.mType) return 2;
				return 0;
				break;
		}
		std::cout << "Rule: ";
		for(int iter = 0; iter < bufUncov.size(); ++iter)
		{
			std::cout << sTokenTypeStrings[bufUncov[iter]->mType] << " "; 
		}
		std::cout << std::endl << std::endl;
		if(flag == 3) return 3;
		for(int iter = 0; iter < bufUncov.size(); ++iter){_addNode(bufUncov[iter]);}
		if(bufUncov[0]->mTerm && bufUncov[0]->mType != EPSILON)
		{
			if(bufUncov[0]->mType == Term.mType) flag = 2;
			else return 0;
		}
		bufUncov.clear();
		return flag;
	}
}

