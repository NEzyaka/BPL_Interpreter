/****************************************************************************
**
** @file Interpreter class header
**
** Copyright (C) 2015-2016 Nikita Mironov
** Contact: nekit2002mir@yandex.ru
**
** This file is part of Turnip Runner source code.
** Turnip Runner is open-source, cross-platform interpreter for Turnip programming language.
**
** Turnip Runner is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Turnip Runner is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Turnip Runner. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>
#include <map>
#include <sstream>
#include <iostream>

#include "variable.h"
#include "checker.h"

using namespace std;

class Interpreter
{
public:
    void interpret(); //interprter
    void getKeyboardCode(); //getting code from keyboard
    string filename; //name of file
    bool isIDE = false;

private:
    bool fileIsEmpty = true;

    typedef const string error;
    //errors
    error FILE_IS_EMPTY = "File is empty!\n";
    error READ_ERROR = "Read error!\n";


    typedef const string keyword;
    //blocks
    keyword BEGINBLOCK = "beginblock"; //keywords
    keyword ENDBLOCK = "endblock";
    keyword STARTBLOCK = "start";
    map<string, vector<string> >blocks; //arrays
    vector<string>blockNames;
    vector<string>currentBlock;
    vector<string>code;
    void doBlock(vector<string>block); //methods
    void doOperator(string line);
    void makeBlocks(vector<string>code);
    string blockName; //variables
    bool inBlock = false;

    //operators methods
    void PRINT(string& line);
    void INPUTVAR(string& line);
    void ALERT();
    void NEXTLINE();
    void CLEARSCREEN();
    void DUMP();
    void DUMPVAR(string& line);
    void PRINTVAR(string& line);
    void COMMAND(string& line);
    void FONTCOLOR(string& line);
    void SWAP(string& line);
    void DELETE(string& line);
    void SIZE(string& line);
    void SETVAL(string& line);
    void DOBLOCK(string& line);
    void BLOCKLIST();
    void IMPORT(string& line);
    void INVERT(string& line);

    //operators keywords
    keyword PRINT_OPERATOR = "print";
    keyword INPUTVAR_OPERATOR = "inputvar";
    keyword ALERT_OPERATOR = "alert";
    keyword NEXTLINE_OPERATOR = "nextline";
    keyword CLEARSCREEN_OPERATOR = "clearscreen";
    keyword DUMP_OPERATOR = "dump";
    keyword DUMPVAR_OPERATOR = "dumpvar";
    keyword COMMAND_OPERATOR = "command";
    keyword COLOR_OPERATOR = "fontcolor";
    keyword SWAP_OPERATOR = "swap";
    keyword DELETE_OPERATOR = "delete";
    keyword SIZE_OPERATOR = "size";
    keyword SETVAL_OPERATOR = "setval";
    keyword DOBLOCK_OPERATOR = "doblock";
    keyword BLOCKLIST_OPERATOR = "blocklist";
    keyword IMPORT_OPERATOR = "import";
    keyword INVERT_OPERATOR = "invert";
    keyword AND = "and";
    keyword OR = "or";

    //arithmetic operators methods
    void ADD(string& line);
    void SUBTRACT(string& line);
    void MULTIPLY(string& line);
    void DIVISE(string& line);
    void INCREMENT(string& line);
    void DECREMENT(string& line);

    //arithmetic operators keywords
    keyword ADD_OPERATOR = "add";
    keyword SUBTRACT_OPERATOR = "subtract";
    keyword MULTIPLY_OPERATOR = "multiply";
    keyword DIVISE_OPERATOR = "divise";
    keyword INCREMENT_OPERATOR = "+";
    keyword DECREMENT_OPERATOR = "-";

    //comment keyword
    keyword SINGLE_LINE_COMMENT = "@";

    //getting of code
    string readCode(istream& in);
    void getFileCode();
    string inputLine;

    //variables
    keyword INTEGER = "integer"; //keywords
    keyword LINE = "line";
    keyword BOOL = "bool";
    keyword DOUBLE = "double";
    map<string, Variable> vars; //arrays
    map<string, string> mustInputVars;
    void declareVar(string& line); //declaring a var

    //conditions
    keyword IF_BEGIN = "if"; //keywords
    keyword ELSE = "else";
    keyword ENDIF = "endif";
    bool inIf = false; //flags
    bool inNormalIf = false;
    bool inElse = false;
    bool execute = false;
    bool trueCondition = false;
    void parseCondition(string& line); //parsing a condition

    /**
    * Removes leading and trailing whitespace from the string
    * @param str source string
    * @return trimmed string
    * @author coldhamix
    */
    string trim(string& str)
    {
        size_t first = str.find_first_not_of(' ');
        size_t last = str.find_last_not_of(' ');
        return str.substr(first, last - first + 1);
    }

    /**
    * Checks line for numbers
    * @param str source string
    * @return true/false
    * @author NEzyaka
    */
    bool isNum(string& str)
    {
        if(str == "0")
            return true;
        else
        {
            double buf = 0;
            stringstream convert(str);
            convert >> buf;
            return (buf != 0) ? true : false;
        }
    }

    /**
    * Converts number to string
    * @param number
    * @return number to string
    * @author NEzyaka
    */
    template<typename Type> string toString(Type number)
    {
        stringstream convert;
        convert << number;
        return convert.str();
    }

    /**
    * Error message struct
    */
    struct errorException
    {
        errorException(string l, string m)
        {
            line = l;
            message = m;
        }

        /**
        * @param nothing
        * @return formated error message (block:line -> error)
        * @author NEzyaka
        */
        void print()
        {
            cout << line << " -> " << message;
        }
    private:
        string line;
        string message;
    };
};


#endif // INTERPRETER_H
