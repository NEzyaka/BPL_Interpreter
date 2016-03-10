/****************************************************************************
**
** @file Interpreter class conditions parsing implementation
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

#include <cstring>

#include "interpreter.h"


void Interpreter::parseCondition(string &line)
{
    string condition = line.substr(IF_BEGIN.size());
    condition = trim(condition);
    condition = condition.substr(condition.find_first_of('(')+1, condition.find_last_of(')')-1);
    condition = trim(condition);
    if(condition.find(AND) != string::npos)
    {
        string firstCondition = condition.substr(condition.find_first_of('(')+1, condition.find_first_of(')')-1);
        string secondCondition = condition.substr(condition.find_last_of('(')+1, condition.find_last_of(')')-1);
        secondCondition = secondCondition.substr(0, secondCondition.size()-1);
        bool firstIsTrue = false;
        bool secondIsTrue = false;

        if(firstCondition.find_first_of('=') == firstCondition.find_last_of('=')-1)
        {
            string varName = firstCondition.substr(0, firstCondition.find_first_of('='));
            varName = trim(varName);

            string varValue = firstCondition.substr(firstCondition.find_first_of('=')+2);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() == tmp_variable2.getInteger())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() == intVal)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() == tmp_variable2.getDouble())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() == doubleVal)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == LINE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getString() == tmp_variable2.getString())
                        firstIsTrue = true;
                }
                else
                {
                    varValue = varValue.substr(varValue.find_first_of('"')+1);
                    varValue = varValue.substr(0, varValue.find_first_of('"'));

                    if(tmp_variable.getString() == varValue)
                        firstIsTrue = true;
                }

            }
            else if(tmp_variable.getType() == BOOL)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getBool() == tmp_variable2.getBool())
                        firstIsTrue = true;
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
                        firstIsTrue = true;
                }
            }
        }
        else if(firstCondition.find_first_of('!') == firstCondition.find_last_of('=')-1)
        {
            string varName = firstCondition.substr(0, firstCondition.find_first_of('!'));
            varName = trim(varName);

            string varValue = firstCondition.substr(firstCondition.find_first_of('!')+2);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() != tmp_variable2.getInteger())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() != intVal)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() != tmp_variable2.getDouble())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() != doubleVal)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == LINE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getString() != tmp_variable2.getString())
                        firstIsTrue = true;
                }
                else
                {
                    varValue = varValue.substr(varValue.find_first_of('"')+1);
                    varValue = varValue.substr(0, varValue.find_first_of('"'));

                    if(tmp_variable.getString() != varValue)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == BOOL)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getBool() != tmp_variable2.getBool())
                        firstIsTrue = true;
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

                    if(boolValue != tmp_variable.getBool())
                        firstIsTrue = true;
                }
            }
        }
        else if(firstCondition.find_first_of('<') == firstCondition.find_last_of('=')-1)
        {
            string varName = firstCondition.substr(0, firstCondition.find_first_of('<'));
            varName = trim(varName);

            string varValue = firstCondition.substr(firstCondition.find_first_of('<')+2);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() <= tmp_variable2.getInteger())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() <= intVal)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() <= tmp_variable2.getDouble())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() <= doubleVal)
                        firstIsTrue = true;
                }
            }
        }
        else if(firstCondition.find_first_of('>') == firstCondition.find_last_of('=')-1)
        {
            string varName = firstCondition.substr(0, firstCondition.find_first_of('>'));
            varName = trim(varName);

            string varValue = firstCondition.substr(firstCondition.find_first_of('>')+2);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() >= tmp_variable2.getInteger())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() >= intVal)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() >= tmp_variable2.getDouble())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() >= doubleVal)
                        firstIsTrue = true;
                }
            }
        }
        else if(firstCondition.find_first_of('<') == firstCondition.find_last_of('<'))
        {
            string varName = firstCondition.substr(0, firstCondition.find_first_of('<'));
            varName = trim(varName);

            string varValue = firstCondition.substr(firstCondition.find_first_of('<')+1);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() < tmp_variable2.getInteger())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() < intVal)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() < tmp_variable2.getDouble())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() < doubleVal)
                        firstIsTrue = true;
                }
            }
        }
        if(firstCondition.find_first_of('>') == firstCondition.find_last_of('>')) //works only without else
        {
            string varName = firstCondition.substr(0, firstCondition.find_first_of('>'));
            varName = trim(varName);

            string varValue = firstCondition.substr(firstCondition.find_first_of('>')+1);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() > tmp_variable2.getInteger())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() > intVal)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() > tmp_variable2.getDouble())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() > doubleVal)
                        firstIsTrue = true;
                }
            }
        }


        if(secondCondition.find_first_of('=') == secondCondition.find_last_of('=')-1)
        {
            string varName = secondCondition.substr(0, secondCondition.find_first_of('='));
            varName = trim(varName);

            string varValue = secondCondition.substr(secondCondition.find_first_of('=')+2);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() == tmp_variable2.getInteger())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() == intVal)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() == tmp_variable2.getDouble())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() == doubleVal)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == line)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getString() == tmp_variable2.getString())
                        secondIsTrue = true;
                }
                else
                {
                    varValue = varValue.substr(varValue.find_first_of('"')+1);
                    varValue = varValue.substr(0, varValue.find_first_of('"'));

                    if(tmp_variable.getString() == varValue)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == BOOL)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getBool() == tmp_variable2.getBool())
                        secondIsTrue = true;
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
                        secondIsTrue = true;
                }
            }
        }
        else if(secondCondition.find_first_of('!') == secondCondition.find_last_of('=')-1)
        {
            string varName = secondCondition.substr(0, secondCondition.find_first_of('!'));
            varName = trim(varName);

            string varValue = secondCondition.substr(secondCondition.find_first_of('!')+2);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() != tmp_variable2.getInteger())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() != intVal)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() != tmp_variable2.getDouble())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() != doubleVal)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == LINE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getString() != tmp_variable2.getString())
                        secondIsTrue = true;
                }
                else
                {
                    varValue = varValue.substr(varValue.find_first_of('"')+1);
                    varValue = varValue.substr(0, varValue.find_first_of('"'));

                    if(tmp_variable.getString() != varValue)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == BOOL)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getBool() != tmp_variable2.getBool())
                        secondIsTrue = true;
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

                    if(boolValue != tmp_variable.getBool())
                        secondIsTrue = true;
                }
            }
        }
        else if(secondCondition.find_first_of('<') == secondCondition.find_last_of('=')-1)
        {
            string varName = secondCondition.substr(0, secondCondition.find_first_of('<'));
            varName = trim(varName);

            string varValue = secondCondition.substr(secondCondition.find_first_of('<')+2);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() <= tmp_variable2.getInteger())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() <= intVal)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() <= tmp_variable2.getDouble())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() <= doubleVal)
                        secondIsTrue = true;
                }
            }
        }
        else if(secondCondition.find_first_of('>') == secondCondition.find_last_of('=')-1)
        {
            string varName = secondCondition.substr(0, secondCondition.find_first_of('>'));
            varName = trim(varName);

            string varValue = secondCondition.substr(secondCondition.find_first_of('>')+2);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() >= tmp_variable2.getInteger())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() >= intVal)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() >= tmp_variable2.getDouble())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() >= doubleVal)
                        secondIsTrue = true;
                }
            }
        }
        else if(secondCondition.find_first_of('<') == secondCondition.find_last_of('<'))
        {
            string varName = secondCondition.substr(0, secondCondition.find_first_of('<'));
            varName = trim(varName);

            string varValue = secondCondition.substr(secondCondition.find_first_of('<')+1);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() < tmp_variable2.getInteger())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() < intVal)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() < tmp_variable2.getDouble())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() < doubleVal)
                        secondIsTrue = true;
                }
            }
        }
        if(secondCondition.find_first_of('>') == secondCondition.find_last_of('>')) //works only without else
        {
            string varName = secondCondition.substr(0, secondCondition.find_first_of('>'));
            varName = trim(varName);

            string varValue = secondCondition.substr(secondCondition.find_first_of('>')+1);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() > tmp_variable2.getInteger())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() > intVal)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() > tmp_variable2.getDouble())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() > doubleVal)
                        secondIsTrue = true;
                }
            }
        }

        if(firstIsTrue && secondIsTrue)
        {
            inNormalIf = true;
            trueCondition = true;
        }
    }
    else if(condition.find(OR) != string::npos)
    {
        string firstCondition = condition.substr(condition.find_first_of('(')+1, condition.find_first_of(')')-1);
        string secondCondition = condition.substr(condition.find_last_of('(')+1, condition.find_last_of(')')-1);
        secondCondition = secondCondition.substr(0, secondCondition.size()-1);
        bool firstIsTrue = false;
        bool secondIsTrue = false;

        if(firstCondition.find_first_of('=') == firstCondition.find_last_of('=')-1)
        {
            string varName = firstCondition.substr(0, firstCondition.find_first_of('='));
            varName = trim(varName);

            string varValue = firstCondition.substr(firstCondition.find_first_of('=')+2);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() == tmp_variable2.getInteger())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() == intVal)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() == tmp_variable2.getDouble())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() == doubleVal)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == LINE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getString() == tmp_variable2.getString())
                        firstIsTrue = true;
                }
                else
                {
                    varValue = varValue.substr(varValue.find_first_of('"')+1);
                    varValue = varValue.substr(0, varValue.find_first_of('"'));

                    if(tmp_variable.getString() == varValue)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == BOOL)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getBool() == tmp_variable2.getBool())
                        firstIsTrue = true;
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
                        firstIsTrue = true;
                }
            }
        }
        else if(firstCondition.find_first_of('!') == firstCondition.find_last_of('=')-1)
        {
            string varName = firstCondition.substr(0, firstCondition.find_first_of('!'));
            varName = trim(varName);

            string varValue = firstCondition.substr(firstCondition.find_first_of('!')+2);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() != tmp_variable2.getInteger())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() != intVal)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() != tmp_variable2.getDouble())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() != doubleVal)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == LINE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getString() != tmp_variable2.getString())
                        firstIsTrue = true;
                }
                else
                {
                    varValue = varValue.substr(varValue.find_first_of('"')+1);
                    varValue = varValue.substr(0, varValue.find_first_of('"'));

                    if(tmp_variable.getString() != varValue)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == BOOL)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getBool() != tmp_variable2.getBool())
                        firstIsTrue = true;
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

                    if(boolValue != tmp_variable.getBool())
                        firstIsTrue = true;
                }
            }
        }
        else if(firstCondition.find_first_of('<') == firstCondition.find_last_of('=')-1)
        {
            string varName = firstCondition.substr(0, firstCondition.find_first_of('<'));
            varName = trim(varName);

            string varValue = firstCondition.substr(firstCondition.find_first_of('<')+2);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() <= tmp_variable2.getInteger())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() <= intVal)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() <= tmp_variable2.getDouble())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() <= doubleVal)
                        firstIsTrue = true;
                }
            }
        }
        else if(firstCondition.find_first_of('>') == firstCondition.find_last_of('=')-1)
        {
            string varName = firstCondition.substr(0, firstCondition.find_first_of('>'));
            varName = trim(varName);

            string varValue = firstCondition.substr(firstCondition.find_first_of('>')+2);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() >= tmp_variable2.getInteger())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() >= intVal)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() >= tmp_variable2.getDouble())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() >= doubleVal)
                        firstIsTrue = true;
                }
            }
        }
        else if(firstCondition.find_first_of('<') == firstCondition.find_last_of('<'))
        {
            string varName = firstCondition.substr(0, firstCondition.find_first_of('<'));
            varName = trim(varName);

            string varValue = firstCondition.substr(firstCondition.find_first_of('<')+1);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() < tmp_variable2.getInteger())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() < intVal)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() < tmp_variable2.getDouble())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() < doubleVal)
                        firstIsTrue = true;
                }
            }
        }
        if(firstCondition.find_first_of('>') == firstCondition.find_last_of('>')) //works only without else
        {
            string varName = firstCondition.substr(0, firstCondition.find_first_of('>'));
            varName = trim(varName);

            string varValue = firstCondition.substr(firstCondition.find_first_of('>')+1);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() > tmp_variable2.getInteger())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() > intVal)
                        firstIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() > tmp_variable2.getDouble())
                        firstIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() > doubleVal)
                        firstIsTrue = true;
                }
            }
        }


        if(secondCondition.find_first_of('=') == secondCondition.find_last_of('=')-1)
        {
            string varName = secondCondition.substr(0, secondCondition.find_first_of('='));
            varName = trim(varName);

            string varValue = secondCondition.substr(secondCondition.find_first_of('=')+2);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() == tmp_variable2.getInteger())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() == intVal)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() == tmp_variable2.getDouble())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() == doubleVal)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == LINE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getString() == tmp_variable2.getString())
                        secondIsTrue = true;
                }
                else
                {
                    varValue = varValue.substr(varValue.find_first_of('"')+1);
                    varValue = varValue.substr(0, varValue.find_first_of('"'));

                    if(tmp_variable.getString() == varValue)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == BOOL)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getBool() == tmp_variable2.getBool())
                        secondIsTrue = true;
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
                        secondIsTrue = true;
                }
            }
        }
        else if(secondCondition.find_first_of('!') == secondCondition.find_last_of('=')-1)
        {
            string varName = secondCondition.substr(0, secondCondition.find_first_of('!'));
            varName = trim(varName);

            string varValue = secondCondition.substr(secondCondition.find_first_of('!')+2);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() != tmp_variable2.getInteger())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() != intVal)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() != tmp_variable2.getDouble())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() != doubleVal)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == LINE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getString() != tmp_variable2.getString())
                        secondIsTrue = true;
                }
                else
                {
                    varValue = varValue.substr(varValue.find_first_of('"')+1);
                    varValue = varValue.substr(0, varValue.find_first_of('"'));

                    if(tmp_variable.getString() != varValue)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == BOOL)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getBool() != tmp_variable2.getBool())
                        secondIsTrue = true;
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

                    if(boolValue != tmp_variable.getBool())
                        secondIsTrue = true;
                }
            }
        }
        else if(secondCondition.find_first_of('<') == secondCondition.find_last_of('=')-1)
        {
            string varName = secondCondition.substr(0, secondCondition.find_first_of('<'));
            varName = trim(varName);

            string varValue = secondCondition.substr(secondCondition.find_first_of('<')+2);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() <= tmp_variable2.getInteger())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() <= intVal)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() <= tmp_variable2.getDouble())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() <= doubleVal)
                        secondIsTrue = true;
                }
            }
        }
        else if(secondCondition.find_first_of('>') == secondCondition.find_last_of('=')-1)
        {
            string varName = secondCondition.substr(0, secondCondition.find_first_of('>'));
            varName = trim(varName);

            string varValue = secondCondition.substr(secondCondition.find_first_of('>')+2);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() >= tmp_variable2.getInteger())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() >= intVal)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() >= tmp_variable2.getDouble())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() >= doubleVal)
                        secondIsTrue = true;
                }
            }
        }
        else if(secondCondition.find_first_of('<') == secondCondition.find_last_of('<'))
        {
            string varName = secondCondition.substr(0, secondCondition.find_first_of('<'));
            varName = trim(varName);

            string varValue = secondCondition.substr(secondCondition.find_first_of('<')+1);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() < tmp_variable2.getInteger())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() < intVal)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() < tmp_variable2.getDouble())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() < doubleVal)
                        secondIsTrue = true;
                }
            }
        }
        if(secondCondition.find_first_of('>') == secondCondition.find_last_of('>')) //works only without else
        {
            string varName = secondCondition.substr(0, secondCondition.find_first_of('>'));
            varName = trim(varName);

            string varValue = secondCondition.substr(secondCondition.find_first_of('>')+1);
            varValue = trim(varValue);

            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == INTEGER)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getInteger() > tmp_variable2.getInteger())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    int intVal = 0;
                    convert >> intVal;
                    convert.flush();

                    if(tmp_variable.getInteger() > intVal)
                        secondIsTrue = true;
                }
            }
            else if(tmp_variable.getType() == DOUBLE)
            {
                if(vars.find(varValue) != vars.end())
                {
                    Variable tmp_variable2 = vars[varValue];

                    if(tmp_variable.getDouble() > tmp_variable2.getDouble())
                        secondIsTrue = true;
                }
                else
                {
                    stringstream convert(varValue);
                    double doubleVal = 0;
                    convert >> doubleVal;
                    convert.flush();

                    if(tmp_variable.getDouble() > doubleVal)
                        secondIsTrue = true;
                }
            }
        }

        if(firstIsTrue || secondIsTrue)
        {
            inNormalIf = true;
            trueCondition = true;
        }
    }
    else
    {
        if(condition != ENDIF)
        {
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
                else if(tmp_variable.getType() == LINE)
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
                    else
                    {
                        varValue = varValue.substr(varValue.find_first_of('"')+1);
                        varValue = varValue.substr(0, varValue.find_first_of('"'));

                        if(tmp_variable.getString() == varValue)
                        {
                            inNormalIf = true;
                            trueCondition = true;
                        }
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
                else if(tmp_variable.getType() == LINE)
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
                    else
                    {
                        varValue = varValue.substr(varValue.find_first_of('"')+1);
                        varValue = varValue.substr(0, varValue.find_first_of('"'));

                        if(tmp_variable.getString() != varValue)
                        {
                            inNormalIf = true;
                            trueCondition = true;
                        }
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
            if(condition.find_first_of('>') == condition.find_last_of('>')) //works only without else
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
    }
}
