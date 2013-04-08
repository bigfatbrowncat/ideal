/*
 * LexerItem.h
 *
 *  Created on: Apr 2, 2012
 *      Author: imizus
 */

#ifndef LEXERITEM_H_
#define LEXERITEM_H_

#include <list>
#include <string>

using namespace std;

#define ERROR_LEXER_UNEXPECTED_CHAR					1
#define ERROR_LEXER_UNEXPECTED_CLOSING_BRACE		2
#define ERROR_LEXER_MISSING_CLOSING_BRACE			3

enum brace { BR_NONE, BR_ROUND, BR_SQUARE, BR_CURLY };

class LexerTreeItem
{
private:
	string innerText;
	list<LexerTreeItem> innerItems;
	brace outerBraces;

	bool isDigit(char ch)
	{
		return (ch >= '0' && ch <= '9');
	}

	bool isDot(char ch)
	{
		return ch == '.';
	}

	bool isLetter(char ch)
	{
		return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
	}

	bool isOperator(char ch)
	{
		return (ch == '+') || (ch == '-') || (ch == '*') || (ch == '/') || (ch == '^');
	}

	bool isWhitespace(char ch)
	{
		return (ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n');
	}

	void extractBraces()
	{
		outerBraces = BR_NONE;
		if (innerText.length() > 1)
		{
			if (innerText[0] == '(' && innerText[innerText.length() - 1] == ')')
			{
				outerBraces = BR_ROUND;
			}
			else if (innerText[0] == '[' && innerText[innerText.length() - 1] == ']')
			{
				outerBraces = BR_SQUARE;
			}
			else if (innerText[0] == '{' && innerText[innerText.length() - 1] == '}')
			{
				outerBraces = BR_CURLY;
			}
		}

		if (outerBraces != BR_NONE)
		{
			innerText = innerText.substr(1, innerText.length() - 2);
		}
		this->innerText = innerText;

	}
public:
	LexerTreeItem(string innerText) : innerText(innerText)
	{
		extractBraces();
	}

	const string& getInnerText() const { return innerText; }
	const list<LexerTreeItem>& getInnerItems() const { return innerItems; }
	brace getOuterBraces() const { return outerBraces; }

	void doLexing(list<LexerTreeItem*>& nextIteration);
};

class LexerTree
{
private:
	LexerTreeItem* root;
	string code;
	void doLexing();
public:
	LexerTree(string code) : root(NULL), code(code)
	{
		doLexing();
	}
	~LexerTree()
	{
		if (root != NULL) delete root;
	}

	const LexerTreeItem& getRoot() const { return *root; }
};

#endif /* LEXERITEM_H_ */
