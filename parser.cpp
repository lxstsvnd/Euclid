#include "parser.hpp"

namespace dnf_parser
{
	Node::Node(int isTerm, enum TokenType type)
	{
		mType = type;
		mTerm = isTerm;
	}
	Node::Node(){;}
	std::vector<const Node*> Node::getChildren() const{return _chPtr;}
	void Node::addChild(const Node& addedNode){_chPtr.push_back(&addedNode);}

	void parseTree::_getUpperNode(const Node* toNode)
	{
		_upperNode = _root;
		for(const Node* bufNode : toNode->getChildren())
		{
			if(!bufNode->mTerm && bufNode->getChildren().empty())
			{
				printf("found!\n");
				_upperNode = (Node*)bufNode;
				return;
			}else if(!bufNode->mTerm && !bufNode->getChildren().empty())
			{
				printf("anyway!\n");
				_getUpperNode(bufNode);
				return;
			}
		}
		return;
	}

	parseTree::parseTree(const std::vector<Token>& Line)
	{
		_Line = Line;
		_Line.push_back(Token(END)); //добавляем указатель на конец строки
		_root = new Node(0, BLACKSPACE); //корень изначально рабочий нетерминал
	}
	void parseTree::getUpperNode()
	{
		printf("getting upper!\n");
		_getUpperNode(_root);
	}
	void parseTree::_addNode(const Node& addedNode){_upperNode->addChild(addedNode);}
	void parseTree::parse()
	{
		printf("In parse!\n");
		for(const Token cToken : _Line)
		{
			getUpperNode();
			const int flag = _ifMatched(_upperNode, cToken);
			if(!flag){throw std::runtime_error("Syntax error!");}
		}
		printf("SUCCESS!\n");
	}

	bool parseTree::_ifMatched(Node* nTerm, Token Term)
	{
		printf("In match!\n");
		std::cout << _upperNode->mType << std::endl;
		std::vector<Node> bufUncov; //буффер для раскрытого нетерминала
		switch(nTerm->mType)
		{
			case BLACKSPACE:
				switch(Term.mType)
				{
					case LBRAC:
					case IDENTIFIER:
					case INTEGER_LITERAL:
						bufUncov.push_back(Node(0, DNF));
						bufUncov.push_back(Node(1, END));
						break;
					default:
						return false;
						break;
				}
				break;
			case DNF:
				switch(Term.mType)
				{
					case LBRAC:
					case IDENTIFIER:
					case INTEGER_LITERAL:
						bufUncov.push_back(Node(0, CONJ));
						bufUncov.push_back(Node(0, DNF_TMP));
						break;
					default:
						return false;
						break;
				}
				break;
			case DNF_TMP:
				switch(Term.mType)
				{
					case END:
						break;
					case DISJOINT:
						bufUncov.push_back(Node(1, DISJOINT));
						bufUncov.push_back(Node(0, DNF));
						break;
					default:
						return false;
						break;
				}
				break;
			case CONJ:
				switch(Term.mType)
				{
					case LBRAC:
						bufUncov.push_back(Node(1, LBRAC));
						bufUncov.push_back(Node(0, PREDICATE));
						bufUncov.push_back(Node(1, CONJUNCT));
						bufUncov.push_back(Node(0, CONJ));
						bufUncov.push_back(Node(1, RBRAC));
						break;
					case IDENTIFIER:
					case INTEGER_LITERAL:
						bufUncov.push_back(Node(0, PREDICATE));
						break;
					default:
						return false;
						break;
				}
				break;
			case PREDICATE:
				switch(Term.mType)
				{
					case IDENTIFIER:
					case INTEGER_LITERAL:
						bufUncov.push_back(Node(0, POLYNOM));
						bufUncov.push_back(Node(0, ORDER));
						bufUncov.push_back(Node(0, POLYNOM));
						break;
					default:
						return false;
						break;
				}
				break;
			case POLYNOM:
				switch(Term.mType)
				{
					case IDENTIFIER:
					case INTEGER_LITERAL:
						bufUncov.push_back(Node(0, MULT));
						bufUncov.push_back(Node(0, SUM));
						break;
					default:
						return false;
						break;
				}
				break;
			case ORDER:
				switch(Term.mType)
				{
					case GREATER:
						bufUncov.push_back(Node(1, GREATER));
						break;
					case EQUAL:
						bufUncov.push_back(Node(1, EQUAL));
						break;
					default:
						return false;
						break;
				}
				break;
			case SUM:
				switch(Term.mType)
				{
					case END:
					case DISJOINT:
					case CONJUNCT:
					case RBRAC:
					case GREATER:
					case EQUAL:
						break;
					case PLUS:
						bufUncov.push_back(Node(1, PLUS));
						bufUncov.push_back(Node(0, POLYNOM));
						break;
					default:
						return false;
						break;
				}
				break;
			case MULT:
				switch(Term.mType)
				{
					case INTEGER_LITERAL:
						bufUncov.push_back(Node(0, CONST));
						bufUncov.push_back(Node(0, MULT_TMP));
						break;
					case IDENTIFIER:
						bufUncov.push_back(Node(1, IDENTIFIER));
						bufUncov.push_back(Node(0, MULT_TMP));
						break;
					default:
						return false;
						break;
				}
				break;
			case MULT_TMP:
				switch(Term.mType)
				{
					case END:
					case DISJOINT:
					case CONJUNCT:
					case RBRAC:
					case GREATER:
					case EQUAL:
					case PLUS:
						break;
					case MULTIPLY:
						bufUncov.push_back(Node(1, MULTIPLY));
						bufUncov.push_back(Node(0, MULT));
					default:
						return false;
						break;
				}
				break;
			case CONST:
				switch(Term.mType)
				{
					case INTEGER_LITERAL:
						bufUncov.push_back(Node(1, INTEGER_LITERAL));
					default:
						return false;
						break;
				}
				break;
			default:
				return false;
				break;
		}
		for(int iter = 0; iter < bufUncov.size(); ++iter){_addNode(bufUncov[iter]);}
		bufUncov.clear();
		return true;
	}
}

