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

    //reading errors
    const string FILE_IS_EMPTY = "File is empty!\n";
    const string READ_ERROR = "Read error!\n";

    //operators errors
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

    //interpreter errors
    const string ARITHMETIC_ERROR = "Arithmetic operation error!\n";
    const string DECLARATION_ERROR = "Variable declaration error!\n";
    const string CONDITION_ERROR = "Condition error!\n";

    //operators
    const string printOperator = "PRINT";
    const string inputOperator = "INPUTVAR";
    const string alertOperator = "ALERT";
    const string endlOperator = "NEXTLINE";
    const string clsOperator = "CLEARSCREEN";
    const string dumpOperator = "DUMP";
    const string dumpVarOperator = "DUMPVAR";
    const string commandOperator = "COMMAND";
    const string colorOperator = "FONTCOLOR";
    const string swapOperator = "SWAP";
    const string deleteOperator = "DELETE";
    const string sizeOperator = "SIZE";
    const string setValOperator = "SETVAL";
    const string doBlockOperator = "DOBLOCK";
    const string blockListOperator = "BLOCKLIST";
    const string importOperator = "IMPORT";

    void PRINT(string& arg);
    void INPUTVAR(string& arg);
    void ALERT();
    void NEXTLINE();
    void CLEARSCREEN();
    void DUMP();
    void DUMPVAR(string& arg);
    void PRINTVAR(string& arg);
    void COMMAND(string& arg);
    void FONTCOLOR(string& arg);
    void SWAP(string& arg);
    void DELETE(string& arg);
    void SIZE(string& arg);
    void SETVAL(string& arg);
    void DOBLOCK(string& arg);
    void BLOCKLIST();
    void IMPORT(string& file);

    //arithmetic
    const string plusOperator = "+=";
    const string minusOperator = "-=";
    const string multiplOperator = "*=";
    const string divOperator = "/=";
    const string incrementOperator = "++";
    const string decrementOperator = "--";

    void add(string& line);
    void subtr(string& line);
    void multipl(string& line);
    void div(string& line);
    void increment(string& line);
    void decrement(string& line);

    //variables
    const string integer = "INTEGER";
    const string str = "LINE";
    const string boolean = "BOOL";
    const string doubl = "DOUBLE";
    map<string, Variable> vars;
    map<string, string> mustInputVars;
    void declareVar(string& line);

    //comments
    const string singleLineComment = "//";

    //getting of code
    string readCode(istream& in);
    string inputLine;
    vector<string>code;
    short getFileCode();

    //conditions
    const string ifCondition = "IF";
    const string elseCondition = "ELSE";
    const string endIfCondition = "ENDIF";
    bool inIf = false;
    bool inNormalIf = false;
    bool inElse = false;
    bool execute = false;
    bool trueCondition = false;
    void parseCondition(string& line);

    //cycles
    const string forCycle = "FOR";
    const string endForCycle = "ENDFOR";
    bool inFor = false;
    void parseCycle(string& line);
    int forNumber = 0;
    int currentIndex = 0;
    vector<string>forBlock;

    //blocks
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
