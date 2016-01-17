/****************************************************************************
**
** @file Interprter class header
**
** Copyright (C) 2015 Nikita Mironov
** Contact: nekit2002mir@yandex.ru
**
** This file is part of BPL Interpreter source code.
** BPL Interpreter is open-source, cross-platform interpreter for BPL programming language.
**
** BPL Interpreter is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** BPL Interpreter is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with BPL Interpreter. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>
#include <map>
#include <sstream>

#include "variable.h"

using namespace std;

class Interpreter
{
public:
    short interpret(); //interprter
    short getKeyboardCode();
    string filename = ""; //name of file
private:
    bool fileIsEmpty = true;

    /// errors
    const string FILE_IS_EMPTY = "File is empty!\n";
    const string READ_ERROR = "Read error!\n";

    const string PRINT_ERROR = "PRINT operator error!\n";
    const string INPUTVAR_ERROR = "INPUTVAR operator error!\n";
    const string DUMP_ERROR = "DUMP operator error!\n";
    const string DUMPVAR_ERROR = "DUMPVAR operator error!\n";
    const string CMD_ERROR = "COMMAND operator error!\n";
    const string COLOR_ERROR = "FONTCOLOR operator error!\n";
    const string SWAP_ERROR = "SWAP operator error!\n";
    const string DELETE_ERROR = "DUMP operator error!\n";
    const string SIZE_ERROR = "SIZE operator error!\n";
    const string SETVAL_ERROR = "SETVAL operator error!\n";

    struct errorPosition
    {
        unsigned short line = 0;
        unsigned short column = 0;
    };

    /// interpreter errors
    const string ARITHMETIC_ERROR = "Arithmetic operation error!\n";
    const string DECLARATION_ERROR = "Variable declaration error!\n";
    const string CONDITION_ERROR = "Condition error!\n";

    /// operators
    const string PRINT_OPERATOR = "PRINT";
    const string INPUTVAR_OPERATOR = "INPUTVAR";
    const string ALERT_OPERATOR = "ALERT";
    const string NEXTLINE_OPERATOR = "NEXTLINE";
    const string CLEARSCREEN_OPERATOR = "CLEARSCREEN";
    const string DUMP_OPERATOR = "DUMP";
    const string DUMPVAR_OPERATOR = "DUMPVAR";
    const string COMMAND_OPERATOR = "COMMAND";
    const string COLOR_OPERATOR = "FONTCOLOR";
    const string SWAP_OPERATOR = "SWAP";
    const string DELETE_OPERATOR = "DELETE";
    const string SIZE_OPERATOR = "SIZE";
    const string SETVAL_OPERATOR = "SETVAL";
    const string DOBLOCK_OPERATOR = "DOBLOCK";
    const string BLOCKLIST_OPERATOR = "BLOCKLIST";
    const string IMPORT_OPERATOR = "IMPORT";

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

    /// arithmetic
    const string ADD_OPERATOR = "ADD";
    const string SUBTRACT_OPERATOR = "SUBTRACT";
    const string MULTIPLY_OPERATOR = "MULTIPLY";
    const string DIVISE_OPERATOR = "DIVISE";
    const string INCREMENT_OPERATOR = "+";
    const string DECREMENT_OPERATOR = "-";

    void ADD(string line);
    void SUBTRACT(string line);
    void MULTIPLY(string line);
    void DIVISE(string line);
    void INCREMENT(string line);
    void DECREMENT(string line);

    /// variables
    const string INTEGER = "INTEGER";
    const string LINE = "LINE";
    const string BOOL = "BOOL";
    const string DOUBLE = "DOUBLE";
    map<string, Variable> vars;
    map<string, string> mustInputVars;
    void declareVar(string& line);

    /// comments
    const string singleLineComment = "//";

    /// getting of code
    string readCode(istream& in);
    string inputLine;
    vector<string>code;
    short getFileCode();

    /// conditions
    const string IF_BEGIN = "IF";
    const string ELSE = "ELSE";
    const string ENDIF = "ENDIF";
    bool inIf = false;
    bool inNormalIf = false;
    bool inElse = false;
    bool execute = false;
    bool trueCondition = false;
    void parseCondition(string& line);

    /// cycles
    const string forCycle = "FOR";
    const string endForCycle = "ENDFOR";
    bool inFor = false;
    void parseCycle(string& line);
    int forNumber = 0;
    int currentIndex = 0;
    vector<string>forBlock;

    /// blocks
    void doBlock(vector<string>block);
    map<string, vector<string> >blocks;
    vector<string>blockNames;
    bool inBlock = false;
    const string beginBlock = "BEGINBLOCK";
    const string endBlock = "ENDBLOCK";
    string blockName;
    vector<string>currentBlock;
    const string mainBlock = "MAIN";

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
            int buf = 0;
            std::stringstream convert(str);
            convert >> buf;
            return (buf != 0) ? true : false;
        }
    }
};


#endif // INTERPRETER_H
