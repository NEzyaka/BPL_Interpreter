/****************************************************************************
**
** @file Interprter class implementation
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

#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "interpreter.h"


//getting of code

string Interpreter::readCode(istream& in) //reading code from input stream
{
    getline(in, inputLine);
    return inputLine;
}

short Interpreter::getKeyboardCode() //getting code from keyboard
{
    ofstream out(filename.c_str());
    ifstream in(filename.c_str());

    if(in.is_open())
    {
        cout << "Enter the code:\n";
        do out << readCode(cin) << endl;
        while(readCode(cin) != "");

        out.close();

        if(in.is_open())
        {
            while(!in.eof()) code.push_back(readCode(in));
            in.close();
        }
    }
}

short Interpreter::getFileCode() //getting code from file
{
    try
    {
        ifstream input(filename.c_str());
        if(input.is_open())
        {
            for(inputLine = readCode(input); !input.eof(); inputLine = readCode(input))
                code.push_back(inputLine);
            input.close();
        }
        else throw 1;
    }
    catch(int errCode)
    {
        fileIsEmpty = false;
        cout << READ_ERROR;
        return errCode;
    }
}

void Interpreter::parseCondition(string &line)
{
    string condition = line.substr(IF_BEGIN.size());
    condition = trim(condition);
    condition = condition.substr(condition.find_first_of('(')+1, condition.find_first_of(')')-1);
    condition = trim(condition);

    if(condition.find_first_of('=') == condition.find_last_of('=')-1)
    {
        string varName = condition.substr(0, condition.find_first_of('='));
        varName = trim(varName);

        string varValue = condition.substr(condition.find_first_of('=')+2);
        varValue = trim(varValue);

        Variable tmp_variable = vars[varName];

        if(tmp_variable.getType() == INTEGER)
        {
            if(vars.find(varValue) != vars.end())
            {
                Variable tmp_variable2 = vars[varValue];

                if(tmp_variable.getInteger() == tmp_variable2.getInteger())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
            else
            {
                stringstream convert(varValue);
                int intVal = 0;
                convert >> intVal;
                convert.flush();

                if(tmp_variable.getInteger() == intVal)
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
        }
        else if(tmp_variable.getType() == DOUBLE)
        {
            if(vars.find(varValue) != vars.end())
            {
                Variable tmp_variable2 = vars[varValue];

                if(tmp_variable.getDouble() == tmp_variable2.getDouble())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
            else
            {
                stringstream convert(varValue);
                double doubleVal = 0;
                convert >> doubleVal;
                convert.flush();

                if(tmp_variable.getDouble() == doubleVal)
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
        }
        else if(tmp_variable.getType() == line)
        {
            if(vars.find(varValue) != vars.end())
            {
                Variable tmp_variable2 = vars[varValue];

                if(tmp_variable.getString() == tmp_variable2.getString())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
            else if(tmp_variable.getString() == varValue)
            {
                inNormalIf = true;
                trueCondition = true;
            }
        }
        else if(tmp_variable.getType() == BOOL)
        {
            if(vars.find(varValue) != vars.end())
            {
                Variable tmp_variable2 = vars[varValue];

                if(tmp_variable.getBool() == tmp_variable2.getBool())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
            else
            {
                bool boolValue = false;
                if(strstr(varValue.c_str(), "TRUE") || strstr(varValue.c_str(), "true"))
                    boolValue = true;
                else if(!strstr(varValue.c_str(), "TRUE") || !strstr(varValue.c_str(), "true"))
                {
                    stringstream converter(varValue);
                    converter >> std::boolalpha >> boolValue;
                    converter.flush();
                }

                if(boolValue == tmp_variable.getBool())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
        }
    }
    else if(condition.find_first_of('!') == condition.find_last_of('=')-1)
    {
        string varName = condition.substr(0, condition.find_first_of('!'));
        varName = trim(varName);

        string varValue = condition.substr(condition.find_first_of('!')+2);
        varValue = trim(varValue);

        Variable tmp_variable = vars[varName];

        if(tmp_variable.getType() == INTEGER)
        {
            if(vars.find(varValue) != vars.end())
            {
                Variable tmp_variable2 = vars[varValue];

                if(tmp_variable.getInteger() != tmp_variable2.getInteger())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
            else
            {
                stringstream convert(varValue);
                int intVal = 0;
                convert >> intVal;
                convert.flush();

                if(tmp_variable.getInteger() != intVal)
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
        }
        else if(tmp_variable.getType() == DOUBLE)
        {
            if(vars.find(varValue) != vars.end())
            {
                Variable tmp_variable2 = vars[varValue];

                if(tmp_variable.getDouble() != tmp_variable2.getDouble())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
            else
            {
                stringstream convert(varValue);
                double doubleVal = 0;
                convert >> doubleVal;
                convert.flush();

                if(tmp_variable.getDouble() != doubleVal)
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
        }
        else if(tmp_variable.getType() == line)
        {
            if(vars.find(varValue) != vars.end())
            {
                Variable tmp_variable2 = vars[varValue];

                if(tmp_variable.getString() != tmp_variable2.getString())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
            else if(tmp_variable.getString() != varValue)
            {
                inNormalIf = true;
                trueCondition = true;
            }
        }
        else if(tmp_variable.getType() == BOOL)
        {
            if(vars.find(varValue) != vars.end())
            {
                Variable tmp_variable2 = vars[varValue];

                if(tmp_variable.getBool() != tmp_variable2.getBool())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
            else
            {
                bool boolValue = false;
                if(strstr(varValue.c_str(), "TRUE") || strstr(varValue.c_str(), "true"))
                    boolValue = true;
                else if(!strstr(varValue.c_str(), "TRUE") || !strstr(varValue.c_str(), "true"))
                {
                    stringstream converter(varValue);
                    converter >> std::boolalpha >> boolValue;
                    converter.flush();
                }

                if(tmp_variable.getBool() != boolValue)
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
        }

    }
    else if(condition.find_first_of('<') == condition.find_last_of('=')-1)
    {
        string varName = condition.substr(0, condition.find_first_of('<'));
        varName = trim(varName);

        string varValue = condition.substr(condition.find_first_of('<')+2);
        varValue = trim(varValue);

        Variable tmp_variable = vars[varName];

        if(tmp_variable.getType() == INTEGER)
        {
            if(vars.find(varValue) != vars.end())
            {
                Variable tmp_variable2 = vars[varValue];

                if(tmp_variable.getInteger() <= tmp_variable2.getInteger())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
            else
            {
                stringstream convert(varValue);
                int intVal = 0;
                convert >> intVal;
                convert.flush();

                if(tmp_variable.getInteger() <= intVal)
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
        }
        else if(tmp_variable.getType() == DOUBLE)
        {
            if(vars.find(varValue) != vars.end())
            {
                Variable tmp_variable2 = vars[varValue];

                if(tmp_variable.getDouble() <= tmp_variable2.getDouble())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
            else
            {
                stringstream convert(varValue);
                double doubleVal = 0;
                convert >> doubleVal;
                convert.flush();

                if(tmp_variable.getDouble() <= doubleVal)
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
        }
    }
    else if(condition.find_first_of('>') == condition.find_last_of('=')-1)
    {
        string varName = condition.substr(0, condition.find_first_of('>'));
        varName = trim(varName);

        string varValue = condition.substr(condition.find_first_of('>')+2);
        varValue = trim(varValue);

        Variable tmp_variable = vars[varName];

        if(tmp_variable.getType() == INTEGER)
        {
            if(vars.find(varValue) != vars.end())
            {
                Variable tmp_variable2 = vars[varValue];

                if(tmp_variable.getInteger() >= tmp_variable2.getInteger())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
            else
            {
                stringstream convert(varValue);
                int intVal = 0;
                convert >> intVal;
                convert.flush();

                if(tmp_variable.getInteger() >= intVal)
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
        }
        else if(tmp_variable.getType() == DOUBLE)
        {
            if(vars.find(varValue) != vars.end())
            {
                Variable tmp_variable2 = vars[varValue];

                if(tmp_variable.getDouble() >= tmp_variable2.getDouble())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
            else
            {
                stringstream convert(varValue);
                double doubleVal = 0;
                convert >> doubleVal;
                convert.flush();

                if(tmp_variable.getDouble() >= doubleVal)
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
        }
    }
    else if(condition.find_first_of('<') == condition.find_last_of('<'))
    {
        string varName = condition.substr(0, condition.find_first_of('<'));
        varName = trim(varName);

        string varValue = condition.substr(condition.find_first_of('<')+1);
        varValue = trim(varValue);

        Variable tmp_variable = vars[varName];

        if(tmp_variable.getType() == INTEGER)
        {
            if(vars.find(varValue) != vars.end())
            {
                Variable tmp_variable2 = vars[varValue];

                if(tmp_variable.getInteger() < tmp_variable2.getInteger())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
            else
            {
                stringstream convert(varValue);
                int intVal = 0;
                convert >> intVal;
                convert.flush();

                if(tmp_variable.getInteger() < intVal)
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
        }
        else if(tmp_variable.getType() == DOUBLE)
        {
            if(vars.find(varValue) != vars.end())
            {
                Variable tmp_variable2 = vars[varValue];

                if(tmp_variable.getDouble() < tmp_variable2.getDouble())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
            else
            {
                stringstream convert(varValue);
                double doubleVal = 0;
                convert >> doubleVal;
                convert.flush();

                if(tmp_variable.getDouble() < doubleVal)
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
        }
    }
    else if(condition.find_first_of('>') == condition.find_last_of('>'))
    {
        string varName = condition.substr(0, condition.find_first_of('>'));
        varName = trim(varName);

        string varValue = condition.substr(condition.find_first_of('>')+1);
        varValue = trim(varValue);

        Variable tmp_variable = vars[varName];

        if(tmp_variable.getType() == INTEGER)
        {
            if(vars.find(varValue) != vars.end())
            {
                Variable tmp_variable2 = vars[varValue];

                if(tmp_variable.getInteger() > tmp_variable2.getInteger())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
            else
            {
                stringstream convert(varValue);
                int intVal = 0;
                convert >> intVal;
                convert.flush();

                if(tmp_variable.getInteger() > intVal)
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
        }
        else if(tmp_variable.getType() == DOUBLE)
        {
            if(vars.find(varValue) != vars.end())
            {
                Variable tmp_variable2 = vars[varValue];

                if(tmp_variable.getDouble() > tmp_variable2.getDouble())
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
            else
            {
                stringstream convert(varValue);
                double doubleVal = 0;
                convert >> doubleVal;
                convert.flush();

                if(tmp_variable.getDouble() > doubleVal)
                {
                    inNormalIf = true;
                    trueCondition = true;
                }
            }
        }
    }
}

void Interpreter::parseCycle(string &line)
{
    string condition = line.substr(forCycle.size());
    condition = trim(condition);
    condition = condition.substr(condition.find_first_of('(')+1, condition.find_first_of(')')-1);
    condition = trim(condition);

    if(isNum(condition))
    {
        stringstream convert(condition);
        convert >> forNumber;
    }
}

void Interpreter::declareVar(string &line)
{
    bool varDeclaration = false;
    string varType = "";
    if(line.find(INTEGER) != string::npos) //INTEGER
    {
        varDeclaration = true;
        varType = INTEGER;
    }
    else if(line.find(LINE) != string::npos) //LINE
    {
        varDeclaration = true;
        varType = LINE;
    }
    else if(line.find(BOOL) != string::npos) //BOOL
    {
        varDeclaration = true;
        varType = BOOL;
    }
    else if(line.find(DOUBLE) != string::npos) //DOUBLE
    {
        varDeclaration = true;
        varType = DOUBLE;
    }

    if(varDeclaration)
    {
        Variable tmp_variable;
        tmp_variable.setType(varType);

        size_t equalSignPosition = line.find('=');

        if(equalSignPosition != string::npos)
        {
            string varName = line.substr(varType.size(), equalSignPosition - varType.size());
            varName = trim(varName);
            string value = line.substr(equalSignPosition + 1);
            value = trim(value);

            if(vars.find(value) != vars.end())
            {
                Variable tmp_variable2 = vars[value];
                if(varType == LINE && tmp_variable2.getType() == LINE) //LINE
                    tmp_variable.setString(tmp_variable2.getString());
                else if(varType == INTEGER && tmp_variable2.getType() == INTEGER) //INTEGER
                    tmp_variable.setInteger(tmp_variable2.getInteger());
                else if(varType == DOUBLE && tmp_variable2.getType() == DOUBLE) //DOUBLE
                    tmp_variable.setDouble(tmp_variable2.getDouble());
                else if(varType == BOOL && tmp_variable2.getType() == BOOL) //BOOL
                    tmp_variable.setBool(tmp_variable2.getBool());
            }
            else
            {
                if(varType == LINE) //LINE
                {
                    tmp_variable.setString(value);
                }
                else if(varType == INTEGER) //INTEGER
                {
                    int intVal = 0;

                    stringstream converter(value);
                    converter >> intVal;
                    converter.flush();

                    tmp_variable.setInteger(intVal);
                }
                else if(varType == DOUBLE) //DOUBLE
                {
                    double doubleVal = 0;

                    stringstream converter(value);
                    converter >> doubleVal;
                    converter.flush();

                    tmp_variable.setDouble(doubleVal);
                }
                else if(varType == BOOL) //BOOL
                {
                    bool boolValue = false;
                    if(strstr(value.c_str(), "TRUE") || strstr(value.c_str(), "true"))
                        boolValue = true;
                    else if(!strstr(value.c_str(), "TRUE") || !strstr(value.c_str(), "true"))
                    {
                        stringstream converter(value);
                        converter >> std::boolalpha >> boolValue;
                        converter.flush();
                    }
                    tmp_variable.setBool(boolValue);
                }
            }

            vars.insert(pair<string, Variable> (varName, tmp_variable));
        }
        else //if equal sign not found
        {
            string varName = line.substr(varType.size());
            varName = trim(varName);
            mustInputVars.insert(pair<string, string>(varName, varType));
        }
    }
}

void Interpreter::doBlock(vector<string>block)
{
    for(int i(0); i < block.size(); i++)
    {
        //conditions
        if(block[i].find(IF_BEGIN) != string::npos)
        {
            inIf = true;
            parseCondition(block[i]);
        }
        else if(block[i] == ELSE)
        {
            inNormalIf = false;
            inElse = true;
        }
        else if(block[i] == ENDIF)
        {
            inIf = false;
            inNormalIf = false;
            inElse = false;
        }

        if(inIf)
        {
            if(inNormalIf && trueCondition)
                execute = true;
            else if(inElse && !trueCondition)
                execute = true;
            else execute = false;
        }
        else execute = true;

        if(execute)
        {
            if(block[i].find(singleLineComment) != string::npos) //single-line comment
            {
                size_t commentFind = block[i].find_first_of(singleLineComment);
                block[i] = block[i].substr(0, commentFind);
            }

            if((block[i].find(INTEGER) != string::npos) || (block[i].find(LINE) != string::npos) || (block[i].find(BOOL) != string::npos) || (block[i].find(DOUBLE) != string::npos))
                declareVar(block[i]); //declare var

            //Operators
            if(block[i].find(DUMP_OPERATOR) != string::npos) //DUMP
                if(block[i].find(DUMPVAR_OPERATOR) != string::npos) //DUMPVAR
                    DUMPVAR(block[i]);
                else DUMP(); //DUMP
            else if(block[i].find(PRINT_OPERATOR) != string::npos) //PRINT
                PRINT(block[i]);
            else if(block[i].find(SETVAL_OPERATOR) != string::npos) //SETVAL
                SETVAL(block[i]);
            else if(block[i].find(INPUTVAR_OPERATOR) != string::npos) //INPUTVAR
                INPUTVAR(block[i]);
            else if(block[i].find(ALERT_OPERATOR) != string::npos) //ALERT
                ALERT();
            else if(block[i].find(NEXTLINE_OPERATOR) != string::npos) //NEXTLINE
                NEXTLINE();
            else if(block[i].find(CLEARSCREEN_OPERATOR) != string::npos) //CLEARSCREEN
                CLEARSCREEN();
            else if(block[i].find(COMMAND_OPERATOR) != string::npos) //COMMAND
                COMMAND(block[i]);
            else if(block[i].find(COLOR_OPERATOR) != string::npos) //FONTCOLOR
                FONTCOLOR(block[i]);
            else if(block[i].find(SWAP_OPERATOR) != string::npos) //SWAP
                SWAP(block[i]);
            else if(block[i].find(DELETE_OPERATOR) != string::npos) //DELETE
                DELETE(block[i]);
            else if(block[i].find(SIZE_OPERATOR) != string::npos) //SIZE
                SIZE(block[i]);
            else if(block[i].find(DOBLOCK_OPERATOR) != string::npos) //DOBLOCK
                DOBLOCK(block[i]);
            else if(block[i].find(BLOCKLIST_OPERATOR) != string::npos) //BLOCKLIST
                BLOCKLIST();

            //arithmetic operations with vars
            if(block[i].find(ADD_OPERATOR) != string::npos) //addition
                ADD(block[i]);
            else if(block[i].find(SUBTRACT_OPERATOR) != string::npos) //addition
                SUBTRACT(block[i]);
            else if(block[i].find(MULTIPLY_OPERATOR) != string::npos) //addition
                MULTIPLY(block[i]);
            else if(block[i].find(DIVISE_OPERATOR) != string::npos) //addition
                DIVISE(block[i]);
            else if(block[i].find(INCREMENT_OPERATOR) != string::npos) //addition
                INCREMENT(block[i]);
            else if(block[i].find(DECREMENT_OPERATOR) != string::npos) //addition
                DECREMENT(block[i]);
        }
    }
}

//interpreter

short Interpreter::interpret()
{
    Interpreter::getFileCode(); //getting code

    system("cls");

    switch(code.size())
    {
    case 0:
        break;
    default:
        //information
        cout << "Successful interpreting!\n" << "File: " << filename <<"\nLines in code: " << code.size() << "\nOutput:\n";

        for(int i(0); i < code.size(); i++)
        {
            if(code[i].find(IMPORT_OPERATOR) != string::npos) //IMPORT
                IMPORT(code[i]);

            if(code[i].find(beginBlock) != string::npos)
            {
                blockName = code[i].substr(beginBlock.size());
                blockName = trim(blockName);
                if(!blockName.empty())
                {
                    inBlock = true;
                    blockNames.push_back(blockName);
                }
            }
            else if(code[i] == endBlock)
            {
                inBlock = false;
                currentBlock.clear();
            }

            if(inBlock)
            {
                currentBlock.push_back(code[i]);
                if(blocks.find(blockName) != blocks.end())
                    blocks.erase(blocks.find(blockName));
                blocks.insert(pair<string, vector<string> > (blockName, currentBlock));
            }
        }

        map<string, vector<string> >::const_iterator iter;
        for(iter = blocks.begin(); iter != blocks.end(); iter++)
        {
            if(iter->first == mainBlock)
                doBlock(iter->second);
        }
    }

    return 0;
}
