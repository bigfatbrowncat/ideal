/*
 * LexerItem.cpp
 *
 *  Created on: Apr 2, 2012
 *      Author: imizus
 */

#include "Lexer.h"

#include <list>
#include <string>

using namespace std;

/* LexerItem */

void LexerTreeItem::doLexing(list<LexerTreeItem*>& nextIteration)
{
	enum state { ident, oper, number, whitespace, inbraces } curState = whitespace;
	class braces_stack
	{
	private:
		list<Brace> braces;
	public:
		void process(char ch)
		{
			if (ch == '(') braces.push_back(brRound);
			else if (ch == '[') braces.push_back(brSquare);
			else if (ch == '{') braces.push_back(brCurly);
			else if (ch == ')')
			{
				if (braces.back() == brRound)
					braces.pop_back();
				else
					throw ERROR_LEXER_UNEXPECTED_CLOSING_BRACE;
			}
			else if (ch == ']')
			{
				if (braces.back() == brSquare)
					braces.pop_back();
				else
					throw ERROR_LEXER_UNEXPECTED_CLOSING_BRACE;
			}
			else if (ch == '}')
			{
				if (braces.back() == brCurly)
					braces.pop_back();
				else
					throw ERROR_LEXER_UNEXPECTED_CLOSING_BRACE;
			}
		}

		bool thereAreOpened() { return braces.size() > 0; }
	} braces;

	string curItemText;

	unsigned int i = 0;
	while (i < innerText.length())
	{
		braces.process(innerText[i]);

		// Reading the next character
		if (curState == whitespace)
		{
			if (braces.thereAreOpened())
			{
				curState = inbraces;
				curItemText = innerText[i];
			}
			else if (isWhitespace(innerText[i]))
			{
				// do nothing, just skip the whitespace
			}
			else if (isDigit(innerText[i]))
			{
				curState = number;
				curItemText = innerText[i];
			}
			else if (isLetter(innerText[i]) || innerText[i] == '_')
			{
				curState = ident;
				curItemText = innerText[i];
			}
			else if (isOperator(innerText[i]))
			{
				curState = oper;
				curItemText = innerText[i];
			}
			else
				throw ERROR_LEXER_UNEXPECTED_CHAR;
		}
		else if (curState == ident)
		{
			if (braces.thereAreOpened())
			{
				// Braces opened. Saving current item
				LexerTreeItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = inbraces;
				// Adding first brace char to next item
				curItemText = innerText[i];
			}
			else if (isLetter(innerText[i]) || isDigit(innerText[i]) || innerText[i] == '_')
			{
				// Still identifier
				curItemText += innerText[i];
			}
			else if (isOperator(innerText[i]))
			{
				// Changed to operator. Saving current item
				LexerTreeItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = oper;
				// Adding first operator char to next item
				curItemText = innerText[i];
			}
			else if (isWhitespace(innerText[i]))
			{
				// Identifier ended. Saving it
				LexerTreeItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = whitespace;
				// Clearing current item text
				curItemText = "";
			}
			else
				throw ERROR_LEXER_UNEXPECTED_CHAR;
		}
		else if (curState == oper)
		{
			if (braces.thereAreOpened())
			{
				// Braces opened. Saving current item
				LexerTreeItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = inbraces;
				// Adding first brace char to next item
				curItemText = innerText[i];
			}
			else if (isOperator(innerText[i]))
			{
				if (curItemText.length() == 0 || curItemText[curItemText.length() - 1] == innerText[i])
				{
					// The same operator symbol i.e. +, ++, --, ** etc...
					curItemText += innerText[i];
				}
				else
				{
					// The new symbol is still operator, but another one
					// Saving current item
					LexerTreeItem li(curItemText);
					innerItems.push_back(li);
					nextIteration.push_back(&(innerItems.back()));

					// Adding first operator char to next item
					curItemText = innerText[i];
				}
			}
			else if (isLetter(innerText[i]) || innerText[i] == '_')
			{
				// Changed to identifier. Saving current item
				LexerTreeItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = ident;
				// Adding first operator char to next item
				curItemText = innerText[i];
			}
			else if (isDigit(innerText[i]))
			{
				// Changed to number. Saving current item
				LexerTreeItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = number;
				// Adding first operator char to next item
				curItemText = innerText[i];
			}
			else if (isWhitespace(innerText[i]))
			{
				// Identifier ended. Saving it
				LexerTreeItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = whitespace;
				// Clearing current item text
				curItemText = "";
			}
			else
				throw ERROR_LEXER_UNEXPECTED_CHAR;
		}
		else if (curState == number)
		{
			if (braces.thereAreOpened())
			{
				// Braces opened. Saving current item
				LexerTreeItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = inbraces;
				// Adding first brace char to next item
				curItemText = innerText[i];
			}
			else if (isDigit(innerText[i]) || innerText[i] == '.')
			{
				// Still number
				curItemText += innerText[i];
			}
			else if (isOperator(innerText[i]))
			{
				// Changed to operator. Saving current item
				LexerTreeItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = oper;
				// Adding first operator char to next item
				curItemText = innerText[i];
			}
			else if (isWhitespace(innerText[i]))
			{
				// Number ended. Saving it
				LexerTreeItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = whitespace;
				// Clearing current item text
				curItemText = "";
			}
			else
				throw ERROR_LEXER_UNEXPECTED_CHAR;
		}
		else if (curState == inbraces)
		{
			curItemText += innerText[i];

			if (!braces.thereAreOpened())
			{
				// Braces closed. Saving current item
				LexerTreeItem li(curItemText);
				innerItems.push_back(li);
				nextIteration.push_back(&(innerItems.back()));

				curState = whitespace;
				curItemText = "";
			}
		}

		i++;
	}

	// Saving the last lexer item
	if (curItemText != "")
	{
		LexerTreeItem li(curItemText);
		innerItems.push_back(li);
		nextIteration.push_back(&(innerItems.back()));
	}

	// Checking: if we don't have outer braces but we have only one inner item
	//            and it's without braces too, it is our copy, so we remove it.
	if (outerBraces == brNone && innerItems.size() == 1 && innerItems.back().outerBraces == brNone)
	{
		nextIteration.remove(&innerItems.front());
		innerItems.clear();
	}

	if (braces.thereAreOpened())
		throw ERROR_LEXER_MISSING_CLOSING_BRACE;
}

/* Lexer */

void LexerTree::doLexing()
{
	list<LexerTreeItem*> wave;

	if (root != NULL) delete root;
	root = new LexerTreeItem(code);

	root->doLexing(wave);

	do
	{
		list<LexerTreeItem*> newWave;
		for (list<LexerTreeItem*>::const_iterator iter = wave.begin(); iter != wave.end(); iter++)
		{
			list<LexerTreeItem*> wavePart;
			(*iter)->doLexing(wavePart);

			newWave.merge(wavePart);
		}
		wave = newWave;
	}
	while (wave.size() > 0);

}
