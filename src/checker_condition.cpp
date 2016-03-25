/****************************************************************************
**
** @file Checker class conditions parsing implementation
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

#include "checker.h"


void Checker::parseCondition(string &line)
{
    try
    {
        string condition = line.substr(IF_BEGIN.size());
        condition = trim(condition);
        condition = condition.substr(condition.find_first_of('(')+1, condition.find_last_of(')')-1);
        condition = trim(condition);

        if(condition.find(AND) != string::npos) //two conditions with AND
        {
            string firstCondition = condition.substr(condition.find_first_of('(')+1, condition.find_first_of(')')-1);
            string secondCondition = condition.substr(condition.find_last_of('(')+1, condition.find_last_of(')')-1);
            secondCondition = secondCondition.substr(0, secondCondition.size()-1);

            //first condition
            if(firstCondition.find_first_of('=') == firstCondition.find_last_of('=')-1)
            {
                string varName = firstCondition.substr(0, firstCondition.find_first_of('='));
                varName = trim(varName);

                string varValue = firstCondition.substr(firstCondition.find_first_of('=')+2);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                    else if(tmp_variable.getType() == LINE)
                    {
                        if(vars.find(varValue) == vars.end())
                        {
                            if(varValue.find('"') != string::npos)
                            {
                                if(varValue.find_first_of('"') == varValue.find_last_of('"'))
                                {
                                    isOK = false;

                                    if(varValue.find("\"") == varValue.size()-1)
                                        errors.push_back(errorException(line, "Expected '\"' at begin of message '" + line.substr(0, line.size()-1) + "'"));
                                    else if(varValue.find("\"") == 0)
                                        errors.push_back(errorException(line, "Expected '\"' at end of message '" + varValue.substr(1) + "'"));
                                }
                            }
                            else
                            {
                                isOK = false;

                                if(vars.find(varValue) == vars.end())
                                    errors.push_back(errorException(line, "Undeclared variable '" + varValue + "' or expected \" at ends of message"));

                            }
                        }
                    }
                    else if(tmp_variable.getType() == BOOL)
                    {
                        if(vars.find(varValue) == vars.end())
                        {
                            if(varValue != "TRUE" && varValue != "FALSE" && varValue != "true" && varValue != "false")
                            {
                                isOK = false;
                                errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                            }
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            else if(firstCondition.find_first_of('!') == firstCondition.find_last_of('=')-1)
            {
                string varName = firstCondition.substr(0, firstCondition.find_first_of('!'));
                varName = trim(varName);

                string varValue = firstCondition.substr(firstCondition.find_first_of('!')+2);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                    else if(tmp_variable.getType() == LINE)
                    {
                        if(vars.find(varValue) == vars.end())
                        {
                            if(varValue.find('"') != string::npos)
                            {
                                if(varValue.find_first_of('"') == varValue.find_last_of('"'))
                                {
                                    isOK = false;

                                    if(varValue.find("\"") == varValue.size()-1)
                                        errors.push_back(errorException(line, "Expected '\"' at begin of message '" + line.substr(0, line.size()-1) + "'"));
                                    else if(varValue.find("\"") == 0)
                                        errors.push_back(errorException(line, "Expected '\"' at end of message '" + varValue.substr(1) + "'"));
                                }
                            }
                            else
                            {
                                isOK = false;

                                if(vars.find(varValue) == vars.end())
                                    errors.push_back(errorException(line, "Undeclared variable '" + varValue + "' or expected \" at ends of message"));

                            }
                        }
                    }
                    else if(tmp_variable.getType() == BOOL)
                    {
                        if(vars.find(varValue) == vars.end())
                        {
                            if(varValue != "TRUE" && varValue != "FALSE" && varValue != "true" && varValue != "false")
                            {
                                isOK = false;
                                errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                            }
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            else if(firstCondition.find_first_of('<') == firstCondition.find_last_of('=')-1)
            {
                string varName = firstCondition.substr(0, firstCondition.find_first_of('<'));
                varName = trim(varName);

                string varValue = firstCondition.substr(firstCondition.find_first_of('<')+2);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            else if(firstCondition.find_first_of('>') == firstCondition.find_last_of('=')-1)
            {
                string varName = firstCondition.substr(0, firstCondition.find_first_of('>'));
                varName = trim(varName);

                string varValue = firstCondition.substr(firstCondition.find_first_of('>')+2);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            else if(firstCondition.find_first_of('<') == firstCondition.find_last_of('<'))
            {
                string varName = firstCondition.substr(0, firstCondition.find_first_of('<'));
                varName = trim(varName);

                string varValue = firstCondition.substr(firstCondition.find_first_of('<')+1);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            if(firstCondition.find_first_of('>') == firstCondition.find_last_of('>')) //works only without else
            {
                string varName = firstCondition.substr(0, firstCondition.find_first_of('>'));
                varName = trim(varName);

                string varValue = firstCondition.substr(firstCondition.find_first_of('>')+1);
                varValue = trim(varValue);

                Variable tmp_variable = vars[varName];

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }

            //second condition
            if(secondCondition.find_first_of('=') == secondCondition.find_last_of('=')-1)
            {
                string varName = secondCondition.substr(0, secondCondition.find_first_of('='));
                varName = trim(varName);

                string varValue = secondCondition.substr(secondCondition.find_first_of('=')+2);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                    else if(tmp_variable.getType() == LINE)
                    {
                        if(vars.find(varValue) == vars.end())
                        {
                            if(varValue.find('"') != string::npos)
                            {
                                if(varValue.find_first_of('"') == varValue.find_last_of('"'))
                                {
                                    isOK = false;

                                    if(varValue.find("\"") == varValue.size()-1)
                                        errors.push_back(errorException(line, "Expected '\"' at begin of message '" + line.substr(0, line.size()-1) + "'"));
                                    else if(varValue.find("\"") == 0)
                                        errors.push_back(errorException(line, "Expected '\"' at end of message '" + varValue.substr(1) + "'"));
                                }
                            }
                            else
                            {
                                isOK = false;

                                if(vars.find(varValue) == vars.end())
                                    errors.push_back(errorException(line, "Undeclared variable '" + varValue + "' or expected \" at ends of message"));

                            }
                        }
                    }
                    else if(tmp_variable.getType() == BOOL)
                    {
                        if(vars.find(varValue) == vars.end())
                        {
                            if(varValue != "TRUE" && varValue != "FALSE" && varValue != "true" && varValue != "false")
                            {
                                isOK = false;
                                errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                            }
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            else if(secondCondition.find_first_of('!') == secondCondition.find_last_of('=')-1)
            {
                string varName = secondCondition.substr(0, secondCondition.find_first_of('!'));
                varName = trim(varName);

                string varValue = secondCondition.substr(secondCondition.find_first_of('!')+2);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                    else if(tmp_variable.getType() == LINE)
                    {
                        if(vars.find(varValue) == vars.end())
                        {
                            if(varValue.find('"') != string::npos)
                            {
                                if(varValue.find_first_of('"') == varValue.find_last_of('"'))
                                {
                                    isOK = false;

                                    if(varValue.find("\"") == varValue.size()-1)
                                        errors.push_back(errorException(line, "Expected '\"' at begin of message '" + line.substr(0, line.size()-1) + "'"));
                                    else if(varValue.find("\"") == 0)
                                        errors.push_back(errorException(line, "Expected '\"' at end of message '" + varValue.substr(1) + "'"));
                                }
                            }
                            else
                            {
                                isOK = false;

                                if(vars.find(varValue) == vars.end())
                                    errors.push_back(errorException(line, "Undeclared variable '" + varValue + "' or expected \" at ends of message"));

                            }
                        }
                    }
                    else if(tmp_variable.getType() == BOOL)
                    {
                        if(vars.find(varValue) == vars.end())
                        {
                            if(varValue != "TRUE" && varValue != "FALSE" && varValue != "true" && varValue != "false")
                            {
                                isOK = false;
                                errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                            }
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            else if(secondCondition.find_first_of('<') == secondCondition.find_last_of('=')-1)
            {
                string varName = secondCondition.substr(0, secondCondition.find_first_of('<'));
                varName = trim(varName);

                string varValue = secondCondition.substr(secondCondition.find_first_of('<')+2);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            else if(secondCondition.find_first_of('>') == secondCondition.find_last_of('=')-1)
            {
                string varName = secondCondition.substr(0, secondCondition.find_first_of('>'));
                varName = trim(varName);

                string varValue = secondCondition.substr(secondCondition.find_first_of('>')+2);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            else if(secondCondition.find_first_of('<') == secondCondition.find_last_of('<'))
            {
                string varName = secondCondition.substr(0, secondCondition.find_first_of('<'));
                varName = trim(varName);

                string varValue = secondCondition.substr(secondCondition.find_first_of('<')+1);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            if(secondCondition.find_first_of('>') == secondCondition.find_last_of('>')) //works only without else
            {
                string varName = secondCondition.substr(0, secondCondition.find_first_of('>'));
                varName = trim(varName);

                string varValue = secondCondition.substr(secondCondition.find_first_of('>')+1);
                varValue = trim(varValue);

                Variable tmp_variable = vars[varName];

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
        }
        else if(condition.find(OR) != string::npos)
        {
            string firstCondition = condition.substr(condition.find_first_of('(')+1, condition.find_first_of(')')-1);
            string secondCondition = condition.substr(condition.find_last_of('(')+1, condition.find_last_of(')')-1);
            secondCondition = secondCondition.substr(0, secondCondition.size()-1);

            //first condition
            if(firstCondition.find_first_of('=') == firstCondition.find_last_of('=')-1)
            {
                string varName = firstCondition.substr(0, firstCondition.find_first_of('='));
                varName = trim(varName);

                string varValue = firstCondition.substr(firstCondition.find_first_of('=')+2);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                    else if(tmp_variable.getType() == LINE)
                    {
                        if(vars.find(varValue) == vars.end())
                        {
                            if(varValue.find('"') != string::npos)
                            {
                                if(varValue.find_first_of('"') == varValue.find_last_of('"'))
                                {
                                    isOK = false;

                                    if(varValue.find("\"") == varValue.size()-1)
                                        errors.push_back(errorException(line, "Expected '\"' at begin of message '" + line.substr(0, line.size()-1) + "'"));
                                    else if(varValue.find("\"") == 0)
                                        errors.push_back(errorException(line, "Expected '\"' at end of message '" + varValue.substr(1) + "'"));
                                }
                            }
                            else
                            {
                                isOK = false;

                                if(vars.find(varValue) == vars.end())
                                    errors.push_back(errorException(line, "Undeclared variable '" + varValue + "' or expected \" at ends of message"));

                            }
                        }
                    }
                    else if(tmp_variable.getType() == BOOL)
                    {
                        if(vars.find(varValue) == vars.end())
                        {
                            if(varValue != "TRUE" && varValue != "FALSE" && varValue != "true" && varValue != "false")
                            {
                                isOK = false;
                                errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                            }
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            else if(firstCondition.find_first_of('!') == firstCondition.find_last_of('=')-1)
            {
                string varName = firstCondition.substr(0, firstCondition.find_first_of('!'));
                varName = trim(varName);

                string varValue = firstCondition.substr(firstCondition.find_first_of('!')+2);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                    else if(tmp_variable.getType() == LINE)
                    {
                        if(vars.find(varValue) == vars.end())
                        {
                            if(varValue.find('"') != string::npos)
                            {
                                if(varValue.find_first_of('"') == varValue.find_last_of('"'))
                                {
                                    isOK = false;

                                    if(varValue.find("\"") == varValue.size()-1)
                                        errors.push_back(errorException(line, "Expected '\"' at begin of message '" + line.substr(0, line.size()-1) + "'"));
                                    else if(varValue.find("\"") == 0)
                                        errors.push_back(errorException(line, "Expected '\"' at end of message '" + varValue.substr(1) + "'"));
                                }
                            }
                            else
                            {
                                isOK = false;

                                if(vars.find(varValue) == vars.end())
                                    errors.push_back(errorException(line, "Undeclared variable '" + varValue + "' or expected \" at ends of message"));

                            }
                        }
                    }
                    else if(tmp_variable.getType() == BOOL)
                    {
                        if(vars.find(varValue) == vars.end())
                        {
                            if(varValue != "TRUE" && varValue != "FALSE" && varValue != "true" && varValue != "false")
                            {
                                isOK = false;
                                errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                            }
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            else if(firstCondition.find_first_of('<') == firstCondition.find_last_of('=')-1)
            {
                string varName = firstCondition.substr(0, firstCondition.find_first_of('<'));
                varName = trim(varName);

                string varValue = firstCondition.substr(firstCondition.find_first_of('<')+2);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            else if(firstCondition.find_first_of('>') == firstCondition.find_last_of('=')-1)
            {
                string varName = firstCondition.substr(0, firstCondition.find_first_of('>'));
                varName = trim(varName);

                string varValue = firstCondition.substr(firstCondition.find_first_of('>')+2);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            else if(firstCondition.find_first_of('<') == firstCondition.find_last_of('<'))
            {
                string varName = firstCondition.substr(0, firstCondition.find_first_of('<'));
                varName = trim(varName);

                string varValue = firstCondition.substr(firstCondition.find_first_of('<')+1);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            if(firstCondition.find_first_of('>') == firstCondition.find_last_of('>')) //works only without else
            {
                string varName = firstCondition.substr(0, firstCondition.find_first_of('>'));
                varName = trim(varName);

                string varValue = firstCondition.substr(firstCondition.find_first_of('>')+1);
                varValue = trim(varValue);

                Variable tmp_variable = vars[varName];

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }

            //second condition
            if(secondCondition.find_first_of('=') == secondCondition.find_last_of('=')-1)
            {
                string varName = secondCondition.substr(0, secondCondition.find_first_of('='));
                varName = trim(varName);

                string varValue = secondCondition.substr(secondCondition.find_first_of('=')+2);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                    else if(tmp_variable.getType() == LINE)
                    {
                        if(vars.find(varValue) == vars.end())
                        {
                            if(varValue.find('"') != string::npos)
                            {
                                if(varValue.find_first_of('"') == varValue.find_last_of('"'))
                                {
                                    isOK = false;

                                    if(varValue.find("\"") == varValue.size()-1)
                                        errors.push_back(errorException(line, "Expected '\"' at begin of message '" + line.substr(0, line.size()-1) + "'"));
                                    else if(varValue.find("\"") == 0)
                                        errors.push_back(errorException(line, "Expected '\"' at end of message '" + varValue.substr(1) + "'"));
                                }
                            }
                            else
                            {
                                isOK = false;

                                if(vars.find(varValue) == vars.end())
                                    errors.push_back(errorException(line, "Undeclared variable '" + varValue + "' or expected \" at ends of message"));

                            }
                        }
                    }
                    else if(tmp_variable.getType() == BOOL)
                    {
                        if(vars.find(varValue) == vars.end())
                        {
                            if(varValue != "TRUE" && varValue != "FALSE" && varValue != "true" && varValue != "false")
                            {
                                isOK = false;
                                errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                            }
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            else if(secondCondition.find_first_of('!') == secondCondition.find_last_of('=')-1)
            {
                string varName = secondCondition.substr(0, secondCondition.find_first_of('!'));
                varName = trim(varName);

                string varValue = secondCondition.substr(secondCondition.find_first_of('!')+2);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                    else if(tmp_variable.getType() == LINE)
                    {
                        if(vars.find(varValue) == vars.end())
                        {
                            if(varValue.find('"') != string::npos)
                            {
                                if(varValue.find_first_of('"') == varValue.find_last_of('"'))
                                {
                                    isOK = false;

                                    if(varValue.find("\"") == varValue.size()-1)
                                        errors.push_back(errorException(line, "Expected '\"' at begin of message '" + line.substr(0, line.size()-1) + "'"));
                                    else if(varValue.find("\"") == 0)
                                        errors.push_back(errorException(line, "Expected '\"' at end of message '" + varValue.substr(1) + "'"));
                                }
                            }
                            else
                            {
                                isOK = false;

                                if(vars.find(varValue) == vars.end())
                                    errors.push_back(errorException(line, "Undeclared variable '" + varValue + "' or expected \" at ends of message"));

                            }
                        }
                    }
                    else if(tmp_variable.getType() == BOOL)
                    {
                        if(vars.find(varValue) == vars.end())
                        {
                            if(varValue != "TRUE" && varValue != "FALSE" && varValue != "true" && varValue != "false")
                            {
                                isOK = false;
                                errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                            }
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            else if(secondCondition.find_first_of('<') == secondCondition.find_last_of('=')-1)
            {
                string varName = secondCondition.substr(0, secondCondition.find_first_of('<'));
                varName = trim(varName);

                string varValue = secondCondition.substr(secondCondition.find_first_of('<')+2);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            else if(secondCondition.find_first_of('>') == secondCondition.find_last_of('=')-1)
            {
                string varName = secondCondition.substr(0, secondCondition.find_first_of('>'));
                varName = trim(varName);

                string varValue = secondCondition.substr(secondCondition.find_first_of('>')+2);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            else if(secondCondition.find_first_of('<') == secondCondition.find_last_of('<'))
            {
                string varName = secondCondition.substr(0, secondCondition.find_first_of('<'));
                varName = trim(varName);

                string varValue = secondCondition.substr(secondCondition.find_first_of('<')+1);
                varValue = trim(varValue);

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
            if(secondCondition.find_first_of('>') == secondCondition.find_last_of('>')) //works only without else
            {
                string varName = secondCondition.substr(0, secondCondition.find_first_of('>'));
                varName = trim(varName);

                string varValue = secondCondition.substr(secondCondition.find_first_of('>')+1);
                varValue = trim(varValue);

                Variable tmp_variable = vars[varName];

                if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                {
                    Variable tmp_variable = vars[varName];

                    if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                    {
                        if(vars.find(varValue) == vars.end() && !isNum(varValue))
                        {
                            isOK = false;
                            errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                        }
                    }
                }
                else if(vars.find(varName) == vars.end())
                {
                    isOK = false;
                    errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                }
            }
        }
        else
        {
            if(line != ENDIF)
            {
                if(condition.find_first_of('=') == condition.find_last_of('=')-1)
                {
                    string varName = condition.substr(0, condition.find_first_of('='));
                    varName = trim(varName);

                    string varValue = condition.substr(condition.find_first_of('=')+2);
                    varValue = trim(varValue);

                    if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                    {
                        Variable tmp_variable = vars[varName];

                        if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                        {
                            if(vars.find(varValue) == vars.end() && !isNum(varValue))
                            {
                                isOK = false;
                                errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                            }
                        }
                        else if(tmp_variable.getType() == LINE)
                        {
                            if(vars.find(varValue) == vars.end())
                            {
                                if(varValue.find('"') != string::npos)
                                {
                                    if(varValue.find_first_of('"') == varValue.find_last_of('"'))
                                    {
                                        isOK = false;

                                        if(varValue.find("\"") == varValue.size()-1)
                                            errors.push_back(errorException(line, "Expected '\"' at begin of message '" + line.substr(0, line.size()-1) + "'"));
                                        else if(varValue.find("\"") == 0)
                                            errors.push_back(errorException(line, "Expected '\"' at end of message '" + varValue.substr(1) + "'"));
                                    }
                                }
                                else
                                {
                                    isOK = false;

                                    if(vars.find(varValue) == vars.end())
                                        errors.push_back(errorException(line, "Undeclared variable '" + varValue + "' or expected \" at ends of message"));

                                }
                            }
                        }
                        else if(tmp_variable.getType() == BOOL)
                        {
                            if(vars.find(varValue) == vars.end())
                            {
                                if(varValue != "TRUE" && varValue != "FALSE" && varValue != "true" && varValue != "false")
                                {
                                    isOK = false;
                                    errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                                }
                            }
                        }
                    }
                    else if(vars.find(varName) == vars.end() && mustInputVars.find(varName) == mustInputVars.end())
                    {
                        isOK = false;
                        errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                    }
                }
                else if(condition.find_first_of('!') == condition.find_last_of('=')-1)
                {
                    string varName = condition.substr(0, condition.find_first_of('!'));
                    varName = trim(varName);

                    string varValue = condition.substr(condition.find_first_of('!')+2);
                    varValue = trim(varValue);

                    if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                    {
                        Variable tmp_variable = vars[varName];

                        if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                        {
                            if(vars.find(varValue) == vars.end() && !isNum(varValue))
                            {
                                isOK = false;
                                errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                            }
                        }
                        else if(tmp_variable.getType() == LINE)
                        {
                            if(vars.find(varValue) == vars.end())
                            {
                                if(varValue.find('"') != string::npos)
                                {
                                    if(varValue.find_first_of('"') == varValue.find_last_of('"'))
                                    {
                                        isOK = false;

                                        if(varValue.find("\"") == varValue.size()-1)
                                            errors.push_back(errorException(line, "Expected '\"' at begin of message '" + line.substr(0, line.size()-1) + "'"));
                                        else if(varValue.find("\"") == 0)
                                            errors.push_back(errorException(line, "Expected '\"' at end of message '" + varValue.substr(1) + "'"));
                                    }
                                }
                                else
                                {
                                    isOK = false;

                                    if(vars.find(varValue) == vars.end())
                                        errors.push_back(errorException(line, "Undeclared variable '" + varValue + "' or expected \" at ends of message"));

                                }
                            }
                        }
                        else if(tmp_variable.getType() == BOOL)
                        {
                            if(vars.find(varValue) == vars.end())
                            {
                                if(varValue != "TRUE" && varValue != "FALSE" && varValue != "true" && varValue != "false")
                                {
                                    isOK = false;
                                    errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                                }
                            }
                        }
                    }
                    else if(vars.find(varName) == vars.end())
                    {
                        isOK = false;
                        errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                    }
                }
                else if(condition.find_first_of('<') == condition.find_last_of('=')-1)
                {
                    string varName = condition.substr(0, condition.find_first_of('<'));
                    varName = trim(varName);

                    string varValue = condition.substr(condition.find_first_of('<')+2);
                    varValue = trim(varValue);

                    if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                    {
                        Variable tmp_variable = vars[varName];

                        if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                        {
                            if(vars.find(varValue) == vars.end() && !isNum(varValue))
                            {
                                isOK = false;
                                errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                            }
                        }
                    }
                    else if(vars.find(varName) == vars.end())
                    {
                        isOK = false;
                        errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                    }
                }
                else if(condition.find_first_of('>') == condition.find_last_of('=')-1)
                {
                    string varName = condition.substr(0, condition.find_first_of('>'));
                    varName = trim(varName);

                    string varValue = condition.substr(condition.find_first_of('>')+2);
                    varValue = trim(varValue);

                    if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                    {
                        Variable tmp_variable = vars[varName];

                        if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                        {
                            if(vars.find(varValue) == vars.end() && !isNum(varValue))
                            {
                                isOK = false;
                                errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                            }
                        }
                    }
                    else if(vars.find(varName) == vars.end())
                    {
                        isOK = false;
                        errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                    }
                }
                else if(condition.find_first_of('<') == condition.find_last_of('<'))
                {
                    string varName = condition.substr(0, condition.find_first_of('<'));
                    varName = trim(varName);

                    string varValue = condition.substr(condition.find_first_of('<')+1);
                    varValue = trim(varValue);

                    if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                    {
                        Variable tmp_variable = vars[varName];

                        if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                        {
                            if(vars.find(varValue) == vars.end() && !isNum(varValue))
                            {
                                isOK = false;
                                errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                            }
                        }
                    }
                    else if(vars.find(varName) == vars.end())
                    {
                        isOK = false;
                        errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                    }
                }
                if(condition.find_first_of('>') == condition.find_last_of('>')) //works only without else
                {
                    string varName = condition.substr(0, condition.find_first_of('>'));
                    varName = trim(varName);

                    string varValue = condition.substr(condition.find_first_of('>')+1);
                    varValue = trim(varValue);

                    Variable tmp_variable = vars[varName];

                    if(vars.find(varName) != vars.end() && mustInputVars.find(varName) != mustInputVars.end())
                    {
                        Variable tmp_variable = vars[varName];

                        if(tmp_variable.getType() == INTEGER || tmp_variable.getType() == INTEGER)
                        {
                            if(vars.find(varValue) == vars.end() && !isNum(varValue))
                            {
                                isOK = false;
                                errors.push_back(errorException(line, "Undeclared variable '" + varValue + "'"));
                            }
                        }
                    }
                    else if(vars.find(varName) == vars.end())
                    {
                        isOK = false;
                        errors.push_back(errorException(line, "Undeclared variable '" + varName + "'"));
                    }
                }
            }
        }
    }
    catch(std::out_of_range)
    {
        isOK = false;
        errors.push_back(errorException(line, "Invalid condition"));
    }
}
