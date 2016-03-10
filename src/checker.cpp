/****************************************************************************
**
** @file Checker class implementation
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
** Turnip-Runner is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Turnip Runner. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include <cstring>

#include "checker.h"


string Checker::readCode(istream& in) //reading code from input stream
{
    getline(in, inputLine);
    return inputLine;
}

void Checker::declareVar(string &line)
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
                    value = value.substr(value.find_first_of('"')+1);
                    value = value.substr(0, value.find_first_of('"'));
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

void Checker::checkBlock(vector<string>block)
{
    for(auto line = block.begin(); line != block.end(); line++)
    {
            //conditions
            if(line->find(IF_BEGIN) != string::npos)
            {
                inIf = true;
                parseCondition(*line);
            }
            else if(*line == ELSE)
            {
                inNormalIf = false;
                inElse = true;
            }
            else if(*line == ENDIF)
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
                if(line->find(SINGLE_LINE_COMMENT) != string::npos) //single-line comment
                {
                    size_t commentFind = line->find_first_of(SINGLE_LINE_COMMENT);
                    *line = line->substr(0, commentFind);
                }

                if((line->find(INTEGER) != string::npos) || (line->find(LINE) != string::npos) || (line->find(BOOL) != string::npos) || (line->find(DOUBLE) != string::npos))
                    declareVar(*line); //declare var
                else
                    checkOperator(*line);
            }
    }
}

void Checker::checkOperator(string line)
{
        if(line.find(DUMP_OPERATOR) != string::npos) //DUMP
            if(line.find(DUMPVAR_OPERATOR) != string::npos) //DUMPVAR
                DUMPVAR(line);
            else DUMP(); //DUMP
        else if(line.find(PRINT_OPERATOR) != string::npos) //PRINT
            PRINT(line);
        else if(line.find(SETVAL_OPERATOR) != string::npos) //SETVAL
            SETVAL(line);
        else if(line.find(INPUTVAR_OPERATOR) != string::npos) //INPUTVAR
            INPUTVAR(line);
        else if(line.find(ALERT_OPERATOR) != string::npos) //ALERT
            ALERT();
        else if(line.find(NEXTLINE_OPERATOR) != string::npos) //NEXTLINE
            NEXTLINE();
        else if(line.find(CLEARSCREEN_OPERATOR) != string::npos) //CLEARSCREEN
            CLEARSCREEN();
        else if(line.find(COMMAND_OPERATOR) != string::npos) //COMMAND
            COMMAND(line);
        else if(line.find(COLOR_OPERATOR) != string::npos) //FONTCOLOR
            FONTCOLOR(line);
        else if(line.find(SWAP_OPERATOR) != string::npos) //SWAP
            SWAP(line);
        else if(line.find(DELETE_OPERATOR) != string::npos) //DELETE
            DELETE(line);
        else if(line.find(SIZE_OPERATOR) != string::npos) //SIZE
            SIZE(line);
        else if(line.find(DOBLOCK_OPERATOR) != string::npos) //DOBLOCK
            DOBLOCK(line);
        else if(line.find(BLOCKLIST_OPERATOR) != string::npos) //BLOCKLIST
            BLOCKLIST();
        else if(line.find(INVERT_OPERATOR) != string::npos) //INVERT
            INVERT(line);

        //arithmetic operations with vars
        if(line.find(ADD_OPERATOR) != string::npos) //addition
            ADD(line);
        else if(line.find(SUBTRACT_OPERATOR) != string::npos) //subtraction
            SUBTRACT(line);
        else if(line.find(MULTIPLY_OPERATOR) != string::npos) //multiplication
            MULTIPLY(line);
        else if(line.find(DIVISE_OPERATOR) != string::npos) //division
            DIVISE(line);
        else if(line.find(INCREMENT_OPERATOR) != string::npos) //increment
            INCREMENT(line);
        else if(line.find(DECREMENT_OPERATOR) != string::npos) //decrement
            DECREMENT(line);
}

void Checker::makeBlocks(vector<string>code) //create blocks
{
    for(auto line = code.begin(); line != code.end(); line++)
    {
        if(line->find(IMPORT_OPERATOR) != string::npos) //IMPORT
            IMPORT(*line);

        if(line->find(BEGINBLOCK) != string::npos)
        {
            blockName = line->substr(BEGINBLOCK.size()); //getting name of block
            blockName = trim(blockName);
            if(!blockName.empty())
            {
                inBlock = true;
                blockNames.push_back(blockName); //insert current block's name to array
            }
        }
        else if(*line == ENDBLOCK) //close block
        {
            inBlock = false;
            currentBlock.clear();
        }

        if(inBlock)
        {
            currentBlock.push_back(*line); //push current line to the current block
            if(blocks.find(blockName) != blocks.end())
                blocks.erase(blocks.find(blockName));
            blocks.insert(pair<string, vector<string> > (blockName, currentBlock)); //insert current block to array
        }
    }
}

bool Checker::check() //checker
{
    makeBlocks(code);

    auto iter = blocks.find(MAINBLOCK);
    if(iter != blocks.end()) //if MAIN block found
        checkBlock(iter->second);
    else //if MAIN block not found
    {
        isOK = false;
        cout << "MAIN block not found!" << endl;
    }

    return isOK;
}
