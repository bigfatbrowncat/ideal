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

enum Brace { brNone, brRound, brSquare, brCurly };

class LexerTreeItem
{
private:
	string innerText;
	list<LexerTreeItem*> innerItems;
	Brace outerBraces;

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
		return (ch == '+') || (ch == '-') || (ch == '*') || (ch == '/') || (ch == '^') || (ch == '=') || (ch == ';');
	}

	bool isWhitespace(char ch)
	{
		return (ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n');
	}

	// This function is used for lexing double-char operators such as '+=', '-=', '*=', '/='
	bool isValidWithEq(char ch)
	{
		return (ch == '+') || (ch == '-') || (ch == '*') || (ch == '/');
	}

	void extractBraces()
	{
		outerBraces = brNone;
		if (innerText.length() > 1)
		{
			if (innerText[0] == '(' && innerText[innerText.length() - 1] == ')')
			{
				outerBraces = brRound;
			}
			else if (innerText[0] == '[' && innerText[innerText.length() - 1] == ']')
			{
				outerBraces = brSquare;
			}
			else if (innerText[0] == '{' && innerText[innerText.length() - 1] == '}')
			{
				outerBraces = brCurly;
			}
		}

		if (outerBraces != brNone)
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
	const list<LexerTreeItem*>& getInnerItems() const { return innerItems; }
	Brace getOuterBraces() const { return outerBraces; }

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
