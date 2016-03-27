/****************************************************************************
**
** @file Checker class operators implementation
**
** Copyright (C) 2015-2016 Nikita Mironov
** Contact: nekit2002mir@yandex.ru
**
** This file is part of Turnip Runner source code.
** Turnip Runner is open-source, cross-platform Checker for Turnip programming language.
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

#include <cstring>
#include <fstream>

#include "checker.h"
#include "variable.h"


//operators

void Checker::PRINT(string& line) //PRINT
{
    try
    {
        string printValue = line.substr(line.find_first_of(' '));
        printValue = trim(printValue);
        if(printValue.find('"') != string::npos) //if argument contains message
        {
            if(printValue.find_first_of('"') == printValue.find_last_of('"')) //if argument contains only one "
            {
                isOK = false;

                if(printValue.find("\"") == printValue.size()-1)
                    errors.push_back(errorException(line, "Expected '\"' at begin of message '" + line.substr(0, line.size()-1) + "'"));
                else if(printValue.find("\"") == 0)
                    errors.push_back(errorException(line, "Expected '\"' at end of message '" + printValue.substr(1) + "'"));
            }
        }
        else //if argument contains only variable's name
        {
            isOK = false;

            if(vars.find(trim(printValue)) == vars.end())
                errors.push_back(errorException(line, "Undeclared variable '" + trim(printValue) + "' or expected \" at ends of message"));

        }
    }
    catch(std::out_of_range) //catch std::out_of range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of PRINT operator"));
    }
}

void Checker::INPUTVAR(string& line) //INPUTVAR
{
    try
    {
        string varName = line.substr(INPUTVAR_OPERATOR.size()); //getting variable's name
        varName = trim(varName);

        if(!varName.empty())
        {
            if(varName.find(AND) != std::string::npos) //if argument contains AND (2 variables)
            {
                size_t andPosition = varName.find(AND);
                string firstVarName = varName.substr(0, andPosition); //getting first variable's name
                firstVarName = trim(firstVarName);

                if(firstVarName.find('"') != string::npos)
                {
                    if(firstVarName.find_first_of('"') != firstVarName.find_last_of('"'))  //if argument contains custom invitation
                    {
                        firstVarName = firstVarName.substr(firstVarName.find_last_of('"')+1);
                        firstVarName = trim(firstVarName);
                    }
                    else if(firstVarName.find_first_of('"') == firstVarName.find_last_of('"')) //if argument contains only one "
                    {
                        isOK = false;

                        if(firstVarName.substr(0, firstVarName.find_last_of(' ')).find("\"") == firstVarName.substr(0, firstVarName.find_last_of(' ')).size()-1)
                            errors.push_back(errorException(line, "Expected '\"' at begin of invitation '" + firstVarName.substr(0, firstVarName.find_last_of(' ')-1) + "'"));
                        else if(firstVarName.find("\"") == 0)
                            errors.push_back(errorException(line, "Expected '\"' at end of invitation '" + firstVarName.substr(1, firstVarName.find_last_of(' ')-1) + "'"));
                    }
                }

                string secondVarName = varName.substr(andPosition+3); //getting second variable's name
                secondVarName = trim(secondVarName);

                if(secondVarName.find('"') != string::npos)
                {
                    if(secondVarName.find_first_of('"') != secondVarName.find_last_of('"'))  //if argument contains custom invitation
                    {
                        secondVarName = secondVarName.substr(secondVarName.find_last_of('"')+1);
                        secondVarName = trim(secondVarName);
                    }
                    else if(secondVarName.find_first_of('"') == secondVarName.find_last_of('"')) //if argument contains only one "
                    {
                        isOK = false;

                        if(secondVarName.substr(0, secondVarName.find_last_of(' ')).find("\"") == secondVarName.substr(0, secondVarName.find_last_of(' ')).size()-1)
                            errors.push_back(errorException(line, "Expected '\"' at begin of invitation '" + secondVarName.substr(0, secondVarName.find_last_of(' ')-1) + "'"));
                        else if(secondVarName.find("\"") == 0)
                            errors.push_back(errorException(line, "Expected '\"' at end of invitation '" + secondVarName.substr(1, secondVarName.find_last_of(' ')-1) + "'"));
                    }
                }

                if(!firstVarName.empty() && !secondVarName.empty())
                {
                    if((mustInputVars.find(firstVarName) == mustInputVars.end()) || (mustInputVars.find(secondVarName) == mustInputVars.end()) || (vars.find(firstVarName) == vars.end()) || (vars.find(secondVarName) == vars.end()))
                    {
                        if((mustInputVars.find(firstVarName) == mustInputVars.end()) && (vars.find(firstVarName) == vars.end())) //if first variable is undeclared
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + firstVarName + "'"));
                        }
                        if((mustInputVars.find(secondVarName) == mustInputVars.end()) && (vars.find(secondVarName) == vars.end())) //if second variable is undeclared
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + secondVarName + "'"));
                        }
                    }
                }
            }
            else //only one variable
            {
                if(varName.find_first_of('"') != varName.find_last_of('"'))  //if argument contains custom invitation
                {
                    varName = varName.substr(varName.find_last_of('"')+1);
                    varName = trim(varName);
                }

                if((mustInputVars.find(varName) == mustInputVars.end()) && (vars.find(varName) == vars.end())) //if variable is undeclared
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
        }
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of INPUTVAR operator"));
    }
}

void Checker::ALERT() //ALERT
{

}

void Checker::NEXTLINE() //NEXTLINE
{

}

void Checker::CLEARSCREEN() //CLEARSCREEN
{

}

void Checker::DUMP() //DUMP
{

}

void Checker::DUMPVAR(string& line) //DUMPVAR
{
    try
    {
        string varName = line.substr(DUMPVAR_OPERATOR.size()); //getting variable's name
        varName = trim(varName);

        if(!varName.empty())
        {
            if(varName.find(AND) != std::string::npos) //if atgument contains (2 variables)
            {
                size_t andPosition = varName.find(AND);
                string firstVarName = varName.substr(0, andPosition); //getting first variable's name
                firstVarName = trim(firstVarName);

                string secondVarName = varName.substr(andPosition+3); //getting second variable's name
                secondVarName = trim(secondVarName);

                if(!firstVarName.empty() && !secondVarName.empty())
                {
                    if((vars.find(firstVarName) == vars.end()) || (vars.find(secondVarName) == vars.end()))
                    {
                        isOK = false;

                        if(vars.find(firstVarName) == vars.end()) //if first variable is undeclared
                            errors.push_back(errorException(line, "Undeclared variable '" + firstVarName + "'"));
                        if(vars.find(secondVarName) == vars.end()) //if second variable is undeclared
                            errors.push_back(errorException(line, "Undeclared variable '" + secondVarName + "'"));
                    }
                }
            }
            else //only one variable
            {
                if(!varName.empty())
                {
                    if(vars.find(varName) == vars.end()) //if variable is undeclared
                    {
                        isOK = false;
                        errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                    }
                }
            }
        }
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of INPUTVAR operator"));
    }
}

void Checker::COMMAND(string& line) //COMMAND
{
    try
    {
        string val = line.substr(COMMAND_OPERATOR.size()); //getting command prompt argument
        val = trim(val);
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of COMMAND operator"));
    }
}

void Checker::FONTCOLOR(string& line) //FONTCOLOR
{
    try
    {
        string colorValue = line.substr(COLOR_OPERATOR.size()); //getting font color
        colorValue = trim(colorValue);

        if(!colorValue.empty())
        {
            if(colorValue != "black" && colorValue != "blue" &&
                    colorValue != "green" && colorValue != "cyan" &&
                    colorValue != "red" && colorValue != "purple" &&
                    colorValue != "yellow" && colorValue != "white" &&
                    colorValue != "gray" && colorValue != "lightgray" &&
                    colorValue != "lightgreen" && colorValue != "lightblue" &&
                    colorValue != "lightred" && colorValue != "lightpurple" &&
                    colorValue != "lightyellow" && colorValue != "lightwhite")
            {
                //if font color is invalid
                isOK = false;
                errors.push_back(errorException(line, "Invalid color: '" + colorValue + "'"));
            }
        }
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of FONTCOLOR operator"));
    }
}

void Checker::SWAP(string& line) //SWAP
{
    try
    {
        if(!line.empty())
        {
            size_t operatorPosition = line.find(SWAP_OPERATOR);
            string firstVarName = line.substr(0, operatorPosition); //getting first variable's name
            firstVarName = trim(firstVarName);

            string secondVarName = line.substr(firstVarName.size()+SWAP_OPERATOR.size()+1); //getting second variable's name
            secondVarName = trim(secondVarName);

            if(!firstVarName.empty() && !secondVarName.empty())
            {
                if((vars.find(firstVarName) != vars.end()) && (vars.find(secondVarName) != vars.end())) //if variables are declared and have a value
                {
                    Variable tmp_variable = vars[firstVarName];
                    Variable tmp_variable2 = vars[secondVarName];

                    if(tmp_variable.getType() != tmp_variable2.getType()) //if types of variables do not match
                    {
                        isOK = false;
                        errors.push_back(errorException(line, "Types of variables '" + firstVarName + "' and '" + secondVarName + "' do not match"));
                    }
                }
                else //if variables are undeclared and haven't got a value
                {
                    isOK = false;

                    if(vars.find(firstVarName) == vars.end())
                        errors.push_back(errorException(line, "Undeclared variable '" + firstVarName + "'"));
                    if(vars.find(secondVarName) == vars.end())
                        errors.push_back(errorException(line, "Undeclared variable '" + secondVarName + "'"));
                }
            }
        }
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of SWAP operator"));
    }
}

void Checker::DELETE(string& line) //DELETE
{
    try
    {
        string varName = line.substr(DELETE_OPERATOR.size()); //getting variable's name
        varName = trim(varName);

        if(!varName.empty())
        {
            if(varName.find(AND) != std::string::npos) //if argument contains AND (2 variables)
            {
                size_t andPosition = varName.find(AND);
                string firstVarName = varName.substr(0, andPosition); //getting first variable's name
                firstVarName = trim(firstVarName);

                string secondVarName = varName.substr(andPosition+3); //second variable's name
                secondVarName = trim(secondVarName);

                if(!firstVarName.empty() && !secondVarName.empty())
                {
                    if((vars.find(firstVarName) == vars.end()) && (mustInputVars.find(firstVarName) == mustInputVars.end())) //if first variable is undeclared
                    {
                        isOK = false;
                        errors.push_back(errorException(line, "Undeclared variable '" + firstVarName + "'"));
                    }
                    if((vars.find(secondVarName) == vars.end()) && (mustInputVars.find(secondVarName) == mustInputVars.end())) //if second variable is undeclared
                    {
                        isOK = false;
                        errors.push_back(errorException(line, "Undeclared variable '" + secondVarName + "'"));
                    }
                }
            }
            else //only one variable
            {
                if(!varName.empty())
                {
                    if((vars.find(varName) == vars.end()) && (mustInputVars.find(varName) == mustInputVars.end())) // if variable is undeclared
                    {
                        isOK = false;
                        errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                    }
                }
            }
        }
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of DELETE operator"));
    }
}

void Checker::SIZE(string& line) //SIZE
{
    try
    {
        string varName = line.substr(SIZE_OPERATOR.size()); //getting variable's name
        varName = trim(varName);

        if(!varName.empty())
        {
            if(vars.find(varName) == vars.end()) //if variable is undeclared
            {
                isOK = false;
                errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
            }
        }
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of SIZE operator"));
    }
}

void Checker::SETVAL(string& line) //SETVAL
{
    try
    {
        size_t operatorPosition = line.find(SETVAL_OPERATOR);
        string varName = line.substr(0, operatorPosition); //getting variable's name
        varName = trim(varName);

        string varVal = line.substr(varName.size()+SETVAL_OPERATOR.size()+1); //getting new variable's value
        varVal = trim(varVal);

        if((vars.find(varName) == vars.end()) && (mustInputVars.find(varName) == mustInputVars.end())) //if variable is undeclared
        {
            isOK = false;
            errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
        }
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of SETVAL operator"));
    }
}

void Checker::DOBLOCK(string& line) //DOBLOCK
{
    try
    {
        string blockName = line.substr(line.find_first_of(' ')); //getting vlock's name
        blockName = trim(blockName);

        if(blocks.find(blockName) == blocks.end()) //if block is undeclared
        {
            isOK = false;
            errors.push_back(errorException(line, "Undeclared block '" + blockName + "'"));
        }
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of DOBLOCK operator"));
    }
}

void Checker::BLOCKLIST() //BLOCKLIST
{

}

void Checker::IMPORT(string& file) //IMPORT
{
    try
    {
        string fileName = file.substr(file.find_first_of(' ')); //getting file's path
        fileName = trim(fileName);

        ifstream input(fileName.c_str());
        if(!input.is_open()) //if file doesn't exists
        {
            isOK = false;
            errors.push_back(errorException(file, "File '" + fileName + "' does not exist"));
        }
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(file, "Invalid argument of IMPORT operator"));
    }
}

void Checker::INVERT(string& line) //INVERT
{
    try
    {
        string varName = line.substr(INVERT_OPERATOR.size()); //getting variable's name
        varName = trim(varName);

        if(vars.find(varName) == vars.end()) //if variable is undeclared
        {
            isOK = false;
            errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
        }
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of INVERT operator"));
    }
}


//arithmetic operations with vars

void Checker::ADD(string& line) //addition
{
    try
    {
        size_t operatorPosition = line.find(ADD_OPERATOR);
        string firstVarName = line.substr(0, operatorPosition); //getting first variable's name
        firstVarName = trim(firstVarName);

        string secondVarName = line.substr(firstVarName.size()+ADD_OPERATOR.size()+1); //getting second variable's name
        secondVarName = trim(secondVarName);

        if((vars.find(firstVarName) != vars.end()) && (vars.find(secondVarName) != vars.end())) //if first variable is declared and have a value and second operand is declared variable with a value
        {
            if(firstVarName == secondVarName)
            {
                Variable tmp_variable = vars[firstVarName];

                if(tmp_variable.getType() != INTEGER && tmp_variable.getType() != DOUBLE) //if type of variable isn't INTEGER or DOUBLE
                {
                    isOK = false;
                    errors.push_back(errorException(line, "You may not to do arithmetic operations with type '" + tmp_variable.getType() + "'"));
                }
            }
            else
            {
                Variable tmp_variable = vars[firstVarName];
                Variable tmp_variable2 = vars[secondVarName];

                if(tmp_variable.getType() != tmp_variable2.getType()) //if types of variables do not match
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Types of variables '" + firstVarName + "' and '" + secondVarName + "' do not match"));
                }
            }
        }
        else if(isNum(secondVarName)) //if second operand is a number
        {
            stringstream convert(secondVarName);
            double plusVal = 0;
            convert >> plusVal;
            convert.flush();

            Variable tmp_variable = vars[firstVarName];

            if(tmp_variable.getType() != INTEGER && tmp_variable.getType() != DOUBLE) //if type of variable isn't INTEGER or DOUBLE
            {
                isOK = false;
                errors.push_back(errorException(line, "You may not to do arithmetic operations with type '" + tmp_variable.getType() + "'"));
            }
        }
        else //something else
        {
            isOK = false;

            if(vars.find(firstVarName) == vars.end()) //if first variable is undeclared
                errors.push_back(errorException(line, "Undeclared variable '" + firstVarName + "'"));
            if(vars.find(secondVarName) == vars.end()) //if second variable is undeclared
                errors.push_back(errorException(line, "Undeclared variable '" + secondVarName + "'"));
        }
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of ADD operator"));
    }
}

void Checker::SUBTRACT(string& line) //subtraction
{
    try
    {
        size_t operatorPosition = line.find(SUBTRACT_OPERATOR);
        string firstVarName = line.substr(0, operatorPosition); //getting second variable's name
        firstVarName = trim(firstVarName);

        string secondVarName = line.substr(firstVarName.size()+SUBTRACT_OPERATOR.size()+1); //getting second variable's name
        secondVarName = trim(secondVarName);

        if((vars.find(firstVarName) != vars.end()) && (vars.find(secondVarName) != vars.end())) //if first variable is declared and have a value and second operand is declared variable with a value
        {
            if(firstVarName == secondVarName)
            {
                Variable tmp_variable = vars[firstVarName];

                if(tmp_variable.getType() != INTEGER && tmp_variable.getType() != DOUBLE) //if type of variable isn't INTEGER or double
                {
                    isOK = false;
                    errors.push_back(errorException(line, "You may not to do arithmetic operations with type '" + tmp_variable.getType() + "'"));
                }
            }
            else
            {
                Variable tmp_variable = vars[firstVarName];
                Variable tmp_variable2 = vars[secondVarName];

                if(tmp_variable.getType() != tmp_variable2.getType()) //if types of variables do not match
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Types of variables '" + firstVarName + "' and '" + secondVarName + "' do not match"));
                }
            }
        }
        else if(isNum(secondVarName)) //if second operand is number
        {
            stringstream convert(secondVarName);
            double plusVal = 0;
            convert >> plusVal;
            convert.flush();

            Variable tmp_variable = vars[firstVarName];

            if(tmp_variable.getType() != INTEGER && tmp_variable.getType() != DOUBLE)  //if type of variable isn't INTEGER or double
            {
                isOK = false;
                errors.push_back(errorException(line, "You may not to do arithmetic operations with type '" + tmp_variable.getType() + "'"));
            }
        }
        else //something else
        {
            isOK = false;

            if(vars.find(firstVarName) == vars.end()) //if first variable is undeclared
                errors.push_back(errorException(line, "Undeclared variable '" + firstVarName + "'"));
            if(vars.find(secondVarName) == vars.end()) //if second variable is undeclared
                errors.push_back(errorException(line, "Undeclared variable '" + secondVarName + "'"));
        }
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of SUBTRACT operator"));
    }
}

void Checker::MULTIPLY(string& line) //multiplication
{
    try
    {
        size_t operatorPosition = line.find(MULTIPLY_OPERATOR);
        string firstVarName = line.substr(0, operatorPosition); //getting first variable's name
        firstVarName = trim(firstVarName);

        string secondVarName = line.substr(firstVarName.size()+MULTIPLY_OPERATOR.size()+1); //getting second variable's name
        secondVarName = trim(secondVarName);

        if((vars.find(firstVarName) != vars.end()) && (vars.find(secondVarName) != vars.end())) //if first variable is declared and have a value and second operand is declared variable with a value
        {
            if(firstVarName == secondVarName)
            {
                Variable tmp_variable = vars[firstVarName];

                if(tmp_variable.getType() != INTEGER && tmp_variable.getType() != DOUBLE) //if type of variable isn't INTEGER or double
                {
                    isOK = false;
                    errors.push_back(errorException(line, "You may not to do arithmetic operations with type '" + tmp_variable.getType() + "'"));
                }
            }
            else
            {
                Variable tmp_variable = vars[firstVarName];
                Variable tmp_variable2 = vars[secondVarName];

                if(tmp_variable.getType() != tmp_variable2.getType()) //if types of variables do not match
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Types of variables '" + firstVarName + "' and '" + secondVarName + "' do not match"));
                }
            }
        }
        else if(isNum(secondVarName)) //if second operand is number
        {
            stringstream convert(secondVarName);
            double plusVal = 0;
            convert >> plusVal;
            convert.flush();

            Variable tmp_variable = vars[firstVarName];

            if(tmp_variable.getType() != INTEGER && tmp_variable.getType() != DOUBLE) //if type of variable isn't INTEGER or double
            {
                isOK = false;
                errors.push_back(errorException(line, "You may not to do arithmetic operations with type '" + tmp_variable.getType() + "'"));
            }
        }
        else //something else
        {
            isOK = false;

            if(vars.find(firstVarName) == vars.end()) //if first variable is undeclared
                errors.push_back(errorException(line, "Undeclared variable '" + firstVarName + "'"));
            if(vars.find(secondVarName) == vars.end()) //if second variable is undeclared
                errors.push_back(errorException(line, "Undeclared variable '" + secondVarName + "'"));
        }
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of MULTIPLY operator"));
    }
}

void Checker::DIVISE(string& line) //division
{
    try
    {
        Variable tmp_variable;
        Variable tmp_variable2;

        size_t operatorPosition = line.find(DIVISE_OPERATOR);
        string firstVarName = line.substr(0, operatorPosition); //getting first variable's name
        firstVarName = trim(firstVarName);

        string secondVarName = line.substr(firstVarName.size()+DIVISE_OPERATOR.size()+1); //getting second variable's name
        secondVarName = trim(secondVarName);

        if((vars.find(firstVarName) != vars.end()) && (vars.find(secondVarName) != vars.end())) //if first variable is declared and have a value and second operand is declared variable with a value
        {
            if(firstVarName == secondVarName)
            {
                Variable tmp_variable = vars[firstVarName];

                if(tmp_variable.getType() != INTEGER && tmp_variable.getType() != DOUBLE) //if type of variable isn't INTEGER or double
                {
                    isOK = false;
                    errors.push_back(errorException(line, "You may not to do arithmetic operations with type '" + tmp_variable.getType() + "'"));
                }
            }
            else
            {
                Variable tmp_variable = vars[firstVarName];
                Variable tmp_variable2 = vars[secondVarName];

                if(tmp_variable.getType() != tmp_variable2.getType()) //if types of variables do not match
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Types of variables '" + firstVarName + "' and '" + secondVarName + "' do not match"));
                }
            }
        }
        else if(isNum(secondVarName)) //if second operand is number
        {
            stringstream convert(secondVarName);
            double plusVal = 0;
            convert >> plusVal;
            convert.flush();

            Variable tmp_variable = vars[firstVarName];

            if(tmp_variable.getType() != INTEGER && tmp_variable.getType() != DOUBLE) //if type of variable isn't INTEGER or double
            {
                isOK = false;
                errors.push_back(errorException(line, "You may not to do arithmetic operations with type '" + tmp_variable.getType() + "'"));
            }
        }
        else //something else
        {
            isOK = false;

            if(vars.find(firstVarName) == vars.end()) //if first variable is undeclared
                errors.push_back(errorException(line, "Undeclared variable '" + firstVarName + "'"));
            if(vars.find(secondVarName) == vars.end()) //if second variable is undeclared
                errors.push_back(errorException(line, "Undeclared variable '" + secondVarName + "'"));
        }
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of DIVISE operator"));
    }
}

void Checker::INCREMENT(string& line) //increment
{
    try
    {
        size_t operatorPosition = line.find(INCREMENT_OPERATOR);
        string varName = line.substr(0, operatorPosition); //getting variable's name
        varName = trim(varName);

        if(vars.find(varName) != vars.end()) //if variable is declared and have a value
        {
            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() != INTEGER && tmp_variable.getType() != DOUBLE) //if type of variable isn't INTEGER or double
            {
                isOK = false;
                errors.push_back(errorException(line, "You may not to do arithmetic operations with type '" + tmp_variable.getType() + "'"));
            }

        }
        else if(vars.find(varName) == vars.end()) //if variable is undeclared
        {
            isOK = false;

            if(vars.find(varName) == vars.end()) //if variable is undeclared
                errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
        }
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of increment operator"));
    }
}

void Checker::DECREMENT(string& line) //decrement
{
    try
    {
        size_t operatorPosition = line.find(DECREMENT_OPERATOR);
        string varName = line.substr(0, operatorPosition); //getting variable's name
        varName = trim(varName);

        if(vars.find(varName) != vars.end())  //if variable is declared and have a value
        {
            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() != INTEGER && tmp_variable.getType() != DOUBLE) //if type of variable isn't INTEGER or double
            {
                isOK = false;
                errors.push_back(errorException(line, "You may not to do arithmetic operations with type '" + tmp_variable.getType() + "'"));
            }

        }
        else if(vars.find(varName) == vars.end()) //if variable is undeclared
        {
            isOK = false;

            if(vars.find(varName) == vars.end()) //if variable is undeclared
                errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
        }
    }
    catch(std::out_of_range) //catch std::out_of_range
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid argument of decrement operator"));
    }
}
