/****************************************************************************
**
** @file Interprter class operators implementation
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

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <fstream>

#include "interpreter.h"
#include "variable.h"


//operators

void Interpreter::PRINT(string &arg) //PRINT
{
    arg = arg.substr(arg.find_first_of(' '));
    arg = trim(arg);
    if(arg.find('"') != string::npos)
    {
        if(arg.find_first_of('"') != arg.find_last_of('"'))
        {
            string printValue = trim(arg);
            if((printValue.find_first_of('+') == printValue.find_last_of('+')) != string::npos)
            {
                if(printValue.find('+') < printValue.find_first_of('"'))
                {
                    string varName = printValue.substr(0, printValue.find_first_of('+'));
                    varName = trim(varName);
                    if(vars.find(varName) != vars.end())
                    {
                        Variable v = vars[varName];

                        if(v.getType() == str)
                            cout << v.getString();
                        else if(v.getType() == integer)
                            cout << v.getInteger();
                        else if(v.getType() == doubl)
                            cout << v.getDouble();
                        else if(v.getType() == boolean)
                            cout << (v.getBool() == true ? "TRUE" : "FALSE");
                    }

                    printValue = printValue.substr(printValue.find('+')+1);
                    printValue = trim(printValue);
                    printValue = printValue.substr(printValue.find_first_of('"')+1);
                    printValue = printValue.substr(0, printValue.find_first_of('"'));
                    cout << printValue;
                }
                else if(printValue.find('+') > printValue.find_last_of('"'))
                {
                    string varName = printValue.substr(printValue.find('+')+1);
                    varName = trim(varName);

                    printValue = printValue.substr(0, printValue.find('+'));
                    printValue = trim(printValue);
                    printValue = printValue.substr(printValue.find_first_of('"')+1);
                    printValue = printValue.substr(0, printValue.find_first_of('"'));
                    cout << printValue;

                    if(vars.find(varName) != vars.end())
                    {
                        Variable v = vars[varName];

                        if(v.getType() == str)
                            cout << v.getString();
                        else if(v.getType() == integer)
                            cout << v.getInteger();
                        else if(v.getType() == doubl)
                            cout << v.getDouble();
                        else if( v.getType() == boolean)
                            cout << (v.getBool() == true ? "TRUE" : "FALSE");
                    }
                }
            }
            else
            {
                printValue = printValue.substr(printValue.find_first_of('"')+1);
                printValue = printValue.substr(0, printValue.find_first_of('"'));

                if(!printValue.empty())
                    cout << printValue;
            }
        }
        cout << "\n";
    }
    else if(vars.find(trim(arg)) != vars.end())
    {
        Variable v = vars[trim(arg)];

        if(v.getType() == str)
            cout << v.getString() << endl;
        else if(v.getType() == integer)
            cout << v.getInteger() << endl;
        else if(v.getType() == doubl)
            cout << v.getDouble() << endl;
        else if( v.getType() == boolean)
            cout << (v.getBool() == true ? "TRUE" : "FALSE") << endl;
    }
    else if(isNum(arg))
    {
        int buf = 0;
        stringstream convert(arg);
        convert >> buf;
        convert.flush();
        cout << buf;
    }
}

void Interpreter::INPUTVAR(string& arg) //INPUTVAR
{
    string varName = arg.substr(inputOperator.size());
    varName = trim(varName);

    if(!varName.empty())
    {
        if(varName.find(',') != std::string::npos)
        {
            size_t dotPosition = varName.find(',');
            string firstVarName = varName.substr(0, dotPosition);
            firstVarName = trim(firstVarName);

            string secondVarName = varName.substr(dotPosition+2);
            secondVarName = trim(secondVarName);

            if(!firstVarName.empty() && !secondVarName.empty())
            {
                if(mustInputVars.find(firstVarName) != mustInputVars.end() && mustInputVars.find(secondVarName) != mustInputVars.end())
                {
                    //first var
                    string varType = mustInputVars[firstVarName];
                    Variable tmp_variable;
                    tmp_variable.setType(varType);

                    string value = "";
                    cout << "Enter value of '" << firstVarName << "(" << varType << ")': ";

                    getline(cin, value);

                    if(varType == str) //LINE
                    {
                        tmp_variable.setString(value);
                    }
                    else if(varType == integer) //INTEGER
                    {
                        int intVal = 0;

                        stringstream converter(value);
                        converter >> intVal;
                        converter.flush();

                        tmp_variable.setInteger(intVal);
                    }
                    else if(varType == doubl) //DOUBLE
                    {
                        double doubleVal = 0;

                        stringstream converter(value);
                        converter >> doubleVal;
                        converter.flush();

                        tmp_variable.setDouble(doubleVal);
                    }
                    else if(varType == boolean) //BOOL
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

                    mustInputVars.erase(mustInputVars.find(firstVarName));
                    vars.insert(pair<string, Variable> (firstVarName, tmp_variable));

                    //second var
                    varType = mustInputVars[secondVarName];
                    Variable tmp_variable2;
                    tmp_variable2.setType(varType);

                    string value2 = "";
                    cout << "Enter value of '" << secondVarName << "(" << varType << ")': ";

                    getline(cin, value2);

                    if(varType == str) //LINE
                    {
                        tmp_variable2.setString(value2);
                    }
                    else if(varType == integer) //INTEGER
                    {
                        int intVal = 0;

                        stringstream converter(value2);
                        converter >> intVal;
                        converter.flush();

                        tmp_variable2.setInteger(intVal);
                    }
                    else if(varType == doubl) //DOUBLE
                    {
                        double doubleVal = 0;

                        stringstream converter(value2);
                        converter >> doubleVal;
                        converter.flush();

                        tmp_variable2.setDouble(doubleVal);
                    }
                    else if(varType == boolean) //BOOL
                    {
                        bool boolValue = false;
                        if(strstr(value2.c_str(), "TRUE") || strstr(value2.c_str(), "true"))
                            boolValue = true;
                        else if(!strstr(value2.c_str(), "TRUE") || !strstr(value2.c_str(), "true"))
                        {
                            stringstream converter(value);
                            converter >> std::boolalpha >> boolValue;
                            converter.flush();
                        }
                        tmp_variable2.setBool(boolValue);
                    }

                    mustInputVars.erase(mustInputVars.find(secondVarName));
                    vars.insert(pair<string, Variable> (secondVarName, tmp_variable2));
                }
                else if(vars.find(firstVarName) != vars.end() && vars.find(secondVarName) != vars.end())
                {
                    //first var
                    Variable tmp_variable = vars[firstVarName];
                    string value = "";

                    cout << "Enter value of '" << firstVarName << "(" << tmp_variable.getType() << ")': ";
                    getline(cin, value);

                    if(tmp_variable.getType() == str) //LINE
                    {
                        tmp_variable.setString(value);
                    }
                    else if(tmp_variable.getType() == integer) //INTEGER
                    {
                        int intVal = 0;

                        stringstream converter(value);
                        converter >> intVal;
                        converter.flush();

                        tmp_variable.setInteger(intVal);
                    }
                    else if(tmp_variable.getType() == doubl) //DOUBLE
                    {
                        double doubleVal = 0;

                        stringstream converter(value);
                        converter >> doubleVal;
                        converter.flush();

                        tmp_variable.setDouble(doubleVal);
                    }
                    else if(tmp_variable.getType() == boolean) //BOOL
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

                    vars.erase(vars.find(firstVarName));
                    vars.insert(pair<string, Variable> (firstVarName, tmp_variable));

                    //second var
                    Variable tmp_variable2 = vars[secondVarName];
                    string value2 = "";

                    cout << "Enter value of '" << secondVarName << "(" << tmp_variable2.getType() << ")': ";
                    getline(cin, value2);

                    if(tmp_variable2.getType() == str) //LINE
                    {
                        tmp_variable2.setString(value2);
                    }
                    else if(tmp_variable2.getType() == integer) //INTEGER
                    {
                        int intVal = 0;

                        stringstream converter(value2);
                        converter >> intVal;
                        converter.flush();

                        tmp_variable2.setInteger(intVal);
                    }
                    else if(tmp_variable2.getType() == doubl) //DOUBLE
                    {
                        double doubleVal = 0;

                        stringstream converter(value2);
                        converter >> doubleVal;
                        converter.flush();

                        tmp_variable2.setDouble(doubleVal);
                    }
                    else if(tmp_variable2.getType() == boolean) //BOOL
                    {
                        bool boolValue = false;
                        if(strstr(value2.c_str(), "TRUE") || strstr(value2.c_str(), "true"))
                            boolValue = true;
                        else if(!strstr(value2.c_str(), "TRUE") || !strstr(value2.c_str(), "true"))
                        {
                            stringstream converter(value2);
                            converter >> std::boolalpha >> boolValue;
                            converter.flush();
                        }
                        tmp_variable2.setBool(boolValue);
                    }

                    vars.erase(vars.find(secondVarName));
                    vars.insert(pair<string, Variable> (secondVarName, tmp_variable2));
                }
                else if(mustInputVars.find(firstVarName) != mustInputVars.end() && vars.find(secondVarName) != vars.end())
                {
                    //first var
                    string varType = mustInputVars[firstVarName];
                    Variable tmp_variable;
                    tmp_variable.setType(varType);

                    string value = "";
                    cout << "Enter value of '" << firstVarName << "(" << varType << ")': ";

                    getline(cin, value);

                    if(varType == str) //LINE
                    {
                        tmp_variable.setString(value);
                    }
                    else if(varType == integer) //INTEGER
                    {
                        int intVal = 0;

                        stringstream converter(value);
                        converter >> intVal;
                        converter.flush();

                        tmp_variable.setInteger(intVal);
                    }
                    else if(varType == doubl) //DOUBLE
                    {
                        double doubleVal = 0;

                        stringstream converter(value);
                        converter >> doubleVal;
                        converter.flush();

                        tmp_variable.setDouble(doubleVal);
                    }
                    else if(varType == boolean) //BOOL
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

                    mustInputVars.erase(mustInputVars.find(firstVarName));
                    vars.insert(pair<string, Variable> (firstVarName, tmp_variable));

                    //second var
                    Variable tmp_variable2 = vars[secondVarName];
                    string value2 = "";

                    cout << "Enter value of '" << secondVarName << "(" << tmp_variable2.getType() << ")': ";
                    getline(cin, value2);

                    if(tmp_variable2.getType() == str) //LINE
                    {
                        tmp_variable2.setString(value2);
                    }
                    else if(tmp_variable2.getType() == integer) //INTEGER
                    {
                        int intVal = 0;

                        stringstream converter(value2);
                        converter >> intVal;
                        converter.flush();

                        tmp_variable2.setInteger(intVal);
                    }
                    else if(tmp_variable2.getType() == doubl) //DOUBLE
                    {
                        double doubleVal = 0;

                        stringstream converter(value2);
                        converter >> doubleVal;
                        converter.flush();

                        tmp_variable2.setDouble(doubleVal);
                    }
                    else if(tmp_variable2.getType() == boolean) //BOOL
                    {
                        bool boolValue = false;
                        if(strstr(value2.c_str(), "TRUE") || strstr(value2.c_str(), "true"))
                            boolValue = true;
                        else if(!strstr(value2.c_str(), "TRUE") || !strstr(value2.c_str(), "true"))
                        {
                            stringstream converter(value2);
                            converter >> std::boolalpha >> boolValue;
                            converter.flush();
                        }
                        tmp_variable2.setBool(boolValue);
                    }

                    vars.erase(vars.find(secondVarName));
                    vars.insert(pair<string, Variable> (secondVarName, tmp_variable2));
                }
                else if(vars.find(firstVarName) != vars.end() && mustInputVars.find(secondVarName) != mustInputVars.end())
                {
                    //first var
                    Variable tmp_variable = vars[firstVarName];
                    string value = "";

                    cout << "Enter value of '" << firstVarName << "(" << tmp_variable.getType() << ")': ";
                    getline(cin, value);

                    if(tmp_variable.getType() == str) //LINE
                    {
                        tmp_variable.setString(value);
                    }
                    else if(tmp_variable.getType() == integer) //INTEGER
                    {
                        int intVal = 0;

                        stringstream converter(value);
                        converter >> intVal;
                        converter.flush();

                        tmp_variable.setInteger(intVal);
                    }
                    else if(tmp_variable.getType() == doubl) //DOUBLE
                    {
                        double doubleVal = 0;

                        stringstream converter(value);
                        converter >> doubleVal;
                        converter.flush();

                        tmp_variable.setDouble(doubleVal);
                    }
                    else if(tmp_variable.getType() == boolean) //BOOL
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

                    vars.erase(vars.find(firstVarName));
                    vars.insert(pair<string, Variable> (firstVarName, tmp_variable));

                    //second var
                    string varType = mustInputVars[secondVarName];
                    Variable tmp_variable2;
                    tmp_variable2.setType(varType);

                    string value2 = "";
                    cout << "Enter value of '" << secondVarName << "(" << varType << ")': ";

                    getline(cin, value2);

                    if(varType == str) //LINE
                    {
                        tmp_variable2.setString(value2);
                    }
                    else if(varType == integer) //INTEGER
                    {
                        int intVal = 0;

                        stringstream converter(value2);
                        converter >> intVal;
                        converter.flush();

                        tmp_variable2.setInteger(intVal);
                    }
                    else if(varType == doubl) //DOUBLE
                    {
                        double doubleVal = 0;

                        stringstream converter(value2);
                        converter >> doubleVal;
                        converter.flush();

                        tmp_variable2.setDouble(doubleVal);
                    }
                    else if(varType == boolean) //BOOL
                    {
                        bool boolValue = false;
                        if(strstr(value2.c_str(), "TRUE") || strstr(value2.c_str(), "true"))
                            boolValue = true;
                        else if(!strstr(value2.c_str(), "TRUE") || !strstr(value2.c_str(), "true"))
                        {
                            stringstream converter(value);
                            converter >> std::boolalpha >> boolValue;
                            converter.flush();
                        }
                        tmp_variable2.setBool(boolValue);
                    }

                    mustInputVars.erase(mustInputVars.find(secondVarName));
                    vars.insert(pair<string, Variable> (secondVarName, tmp_variable2));
                }
            }
        }
        else
        {
            if(mustInputVars.find(varName) != mustInputVars.end()) //inputing value of variable without value
            {
                string varType = mustInputVars[varName];
                Variable tmp_variable;
                tmp_variable.setType(varType);

                string value = "";
                cout << "Enter value of '" << varName << "(" << varType << ")': ";

                getline(cin, value);

                if(varType == str) //LINE
                {
                    tmp_variable.setString(value);
                }
                else if(varType == integer) //INTEGER
                {
                    int intVal = 0;

                    stringstream converter(value);
                    converter >> intVal;
                    converter.flush();

                    tmp_variable.setInteger(intVal);
                }
                else if(varType == doubl) //DOUBLE
                {
                    double doubleVal = 0;

                    stringstream converter(value);
                    converter >> doubleVal;
                    converter.flush();

                    tmp_variable.setDouble(doubleVal);
                }
                else if(varType == boolean) //BOOL
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

                mustInputVars.erase(mustInputVars.find(varName));
                vars.insert(pair<string, Variable> (varName, tmp_variable));
            }
            else if(vars.find(varName) != vars.end()) //inputing new value of variable
            {
                Variable tmp_variable = vars[varName];

                string value = "";
                cout << "Enter value of '" << varName << "(" << tmp_variable.getType() << ")': ";

                getline(cin, value);

                if(tmp_variable.getType() == str) //LINE
                {
                    tmp_variable.setString(value);
                }
                else if(tmp_variable.getType() == integer) //INTEGER
                {
                    int intVal = 0;

                    stringstream converter(value);
                    converter >> intVal;
                    converter.flush();

                    tmp_variable.setInteger(intVal);
                }
                else if(tmp_variable.getType() == doubl) //DOUBLE
                {
                    double doubleVal = 0;

                    stringstream converter(value);
                    converter >> doubleVal;
                    converter.flush();

                    tmp_variable.setDouble(doubleVal);
                }
                else if(tmp_variable.getType() == boolean) //BOOL
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

                vars.erase(vars.find(varName));
                vars.insert(pair<string, Variable> (varName, tmp_variable));
            }
        }
    }
}

void Interpreter::ALERT() //ALERT
{
    cout << "\a";    //ALERT
}

void Interpreter::NEXTLINE() //NEXTLINE
{
    cout << "\n";    //NEXTLINE
}

void Interpreter::CLEARSCREEN() //CLEARSCREEN
{
    system("cls");
    cout << "Successful interpreting!\n" << "File: " << filename <<"\nLines in code: " << code.size() << "\nOutput:" << endl;
}

void Interpreter::DUMP() //DUMP
{
    map<string, Variable>::const_iterator iter;
    int ix = 1;
    for(iter = vars.begin(); iter != vars.end(); iter++, ix++)
    {
        cout << ix << ": " << iter->first << " -> ";

        Variable v = iter->second;

        if(v.getType() == str)
            cout << v.getString() << " [LINE, length: " << v.getString().size() << " symbols, size: " << sizeof(v.getString()) << " bytes]" << endl;
        else if(v.getType() == integer)
            cout << v.getInteger() << " [INTEGER, size: " << sizeof(v.getInteger()) << " bytes]" << endl;
        else if(v.getType() == doubl)
            cout << v.getDouble() << " [DOUBLE, size: " << sizeof(v.getDouble()) << " bytes]" << endl;
        else if(v.getType() == boolean)
            cout << (v.getBool() == true ? "TRUE" : "FALSE") << " [BOOL, size: " << sizeof(v.getBool()) << " bytes]" << endl;
    }
}

void Interpreter::DUMPVAR(string& arg) //DUMPVAR
{
    string varName = arg.substr(dumpVarOperator.size());
    varName = trim(varName);

    if(!varName.empty())
    {
        if(strstr(varName.c_str(), ","))
        {
            Variable tmp_variable;
            Variable tmp_variable2;

            size_t dotPosition = varName.find(',');
            string firstVarName = varName.substr(0, dotPosition);
            firstVarName = trim(firstVarName);

            string secondVarName = varName.substr(dotPosition+2);
            secondVarName = trim(secondVarName);

            stringstream convert(secondVarName);
            double plusVal = 0;
            convert >> plusVal;
            convert.flush();

            if(plusVal == 0)
            {
                if(!firstVarName.empty())
                    tmp_variable = vars[firstVarName];
                if(!secondVarName.empty())
                    tmp_variable2 = vars[secondVarName];

                if(tmp_variable.getType() == str)
                    cout << firstVarName << " -> " << tmp_variable.getString() << " [LINE, length: " << tmp_variable.getString().size() << " symbols, size: " << sizeof(tmp_variable.getString()) << " bytes]" << endl;
                else if(tmp_variable.getType() == integer)
                    cout << firstVarName << " -> " << tmp_variable.getInteger() << " [INTEGER, size: " << sizeof(tmp_variable.getInteger()) << " bytes]" << endl;
                else if(tmp_variable.getType() == doubl)
                    cout << firstVarName << " -> " << tmp_variable.getDouble() << " [DOUBLE, size: " << sizeof(tmp_variable.getDouble()) << " bytes]" << endl;
                else if(tmp_variable.getType() == boolean)
                    cout << firstVarName << " -> " << (tmp_variable.getBool() == true ? "TRUE" : "FALSE") << " [BOOL, size: " << sizeof(tmp_variable.getBool()) << " bytes]" << endl;


                if(tmp_variable2.getType() == str)
                    cout << secondVarName << " -> " << tmp_variable2.getString() << " [LINE, length: " << tmp_variable2.getString().size() << " symbols, size: " << sizeof(tmp_variable2.getString()) << " bytes]" << endl;
                else if(tmp_variable2.getType() == integer)
                    cout << secondVarName << " -> " << tmp_variable2.getInteger() << " [INTEGER, size: " << sizeof(tmp_variable2.getInteger()) << " bytes]" << endl;
                else if(tmp_variable2.getType() == doubl)
                    cout << secondVarName << " -> " << tmp_variable2.getDouble() << " [DOUBLE, size: " << sizeof(tmp_variable2.getDouble()) << " bytes]" << endl;
                else if(tmp_variable2.getType() == boolean)
                    cout << secondVarName << " -> " << (tmp_variable2.getBool() == true ? "TRUE" : "FALSE") << " [BOOL, size: " << sizeof(tmp_variable2.getBool()) << " bytes]" << endl;
            }
        }
        else
        {
            Variable v = vars[varName];

            if(v.getType() == str)
                cout << varName << " -> " << v.getString() << " [LINE, length: " << v.getString().size() << " symbols, size: " << sizeof(v.getString()) << " bytes]" << endl;
            else if(v.getType() == integer)
                cout << varName << " -> " << v.getInteger() << " [INTEGER, size: " << sizeof(v.getInteger()) << " bytes]" << endl;
            else if(v.getType() == doubl)
                cout << varName << " -> " << v.getDouble() << " [DOUBLE, size: " << sizeof(v.getDouble()) << " bytes]" << endl;
            else if(v.getType() == boolean)
                cout << varName << " -> " << (v.getBool() == true ? "TRUE" : "FALSE") << " [BOOL, size: " << sizeof(v.getBool()) << " bytes]" << endl;
        }
    }
}

void Interpreter::COMMAND(string& arg) //COMMAND
{
    string val = arg.substr(commandOperator.size());
    if(!val.empty())
    {
        string argValue = "start " + val;
        if(trim(argValue) != "start")
        {
            argValue = trim(argValue);
            system(argValue.c_str());
        }
    }
}

void Interpreter::FONTCOLOR(string& arg) //FONTCOLOR
{
    string colorValue = arg.substr(colorOperator.size());
    colorValue = trim(colorValue);

    if(!colorValue.empty())
    {
        string arg = "color ";
        if(colorValue == "BLACK" || colorValue == "black")
            arg.append("0");
        else if(colorValue == "BLUE" || colorValue == "blue")
            arg.append("1");
        else if(colorValue == "GREEN" || colorValue == "green")
            arg.append("2");
        else if(colorValue == "CYAN" || colorValue == "cyan")
            arg.append("3");
        else if(colorValue == "RED" || colorValue == "red")
            arg.append("4");
        else if(colorValue == "PURPLE" || colorValue == "purple")
            arg.append("5");
        else if(colorValue == "YELLOW" || colorValue == "yellow")
            arg.append("6");
        else if(colorValue == "WHITE" || colorValue == "white")
            arg.append("7");
        else if(colorValue == "GRAY" || colorValue == "gray")
            arg.append("8");
        else if(colorValue == "LIGHTGRAY" || colorValue == "lightgray")
            arg.append("9");
        else if(colorValue == "LIGHTGREEN" || colorValue == "lightgreen")
            arg.append("a");
        else if(colorValue == "LIGHTBLUE" || colorValue == "lightblue")
            arg.append("b");
        else if(colorValue == "LIGHTRED" || colorValue == "lightred")
            arg.append("c");
        else if(colorValue == "LIGHTPURPLE" || colorValue == "lightpurple")
            arg.append("d");
        else if(colorValue == "LIGHTYELLOW" || colorValue == "lightyellow")
            arg.append("e");
        else if(colorValue == "LIGHTWHITE" || colorValue == "lightwhite")
            arg.append("f");

        system(arg.c_str());
    }
}

void Interpreter::SWAP(string& arg) //SWAP
{
    string varNames = arg.substr(swapOperator.size());
    varNames = trim(varNames);

    if(!varNames.empty())
    {
        size_t dotPosition = varNames.find(',');
        string firstVarName = varNames.substr(0, dotPosition);
        firstVarName = trim(firstVarName);

        string secondVarName = varNames.substr(dotPosition+2);
        secondVarName = trim(secondVarName);

        if(!firstVarName.empty() && !secondVarName.empty())
        {
            Variable tmp_variable = vars[firstVarName];
            Variable tmp_variable2 = vars[secondVarName];

            if(tmp_variable.getType() == integer && tmp_variable2.getType() == integer)
            {
                int firstVal = tmp_variable.getInteger();
                int secondVal = tmp_variable2.getInteger();

                tmp_variable.setInteger(secondVal);
                tmp_variable2.setInteger(firstVal);
            }
            else if(tmp_variable.getType() == doubl && tmp_variable2.getType() == doubl)
            {
                double firstVal = tmp_variable.getDouble();
                double secondVal = tmp_variable2.getDouble();

                tmp_variable.setDouble(secondVal);
                tmp_variable2.setDouble(firstVal);
            }
            else if(tmp_variable.getType() == str && tmp_variable2.getType() == str)
            {
                string firstVal = tmp_variable.getString();
                string secondVal = tmp_variable2.getString();

                tmp_variable.setString(secondVal);
                tmp_variable2.setString(firstVal);
            }
            else if(tmp_variable.getType() == boolean && tmp_variable2.getType() == boolean)
            {
                bool firstVal = tmp_variable.getBool();
                bool secondVal = tmp_variable2.getBool();

                tmp_variable.setBool(secondVal);
                tmp_variable2.setBool(firstVal);
            }

            vars.erase(vars.find(firstVarName));
            vars.erase(vars.find(secondVarName));

            vars.insert(pair<string, Variable> (firstVarName, tmp_variable));
            vars.insert(pair<string, Variable> (secondVarName, tmp_variable2));
        }
    }
}

void Interpreter::DELETE(string& arg) //DELETE
{
    string varName = arg.substr(deleteOperator.size());
    varName = trim(varName);

    if(!varName.empty())
    {
        if(strstr(varName.c_str(), ","))
        {
            Variable tmp_variable;
            Variable tmp_variable2;

            size_t dotPosition = varName.find(',');
            string firstVarName = varName.substr(0, dotPosition);
            firstVarName = trim(firstVarName);

            string secondVarName = varName.substr(dotPosition+2);
            secondVarName = trim(secondVarName);

            if(!firstVarName.empty())
                if(vars.find(firstVarName) != vars.end())
                    vars.erase(vars.find(firstVarName));
                else if(mustInputVars.find(firstVarName) != mustInputVars.end())
                    mustInputVars.erase(mustInputVars.find(firstVarName));
            if(!secondVarName.empty())
                if(vars.find(secondVarName) != vars.end())
                    vars.erase(vars.find(secondVarName));
                else if(mustInputVars.find(secondVarName) != mustInputVars.end())
                    mustInputVars.erase(mustInputVars.find(secondVarName));
        }
        else
        {
            if(!varName.empty())
                if(vars.find(varName) != vars.end())
                    vars.erase(vars.find(varName));
                else if(mustInputVars.find(varName) != mustInputVars.end())
                    mustInputVars.erase(mustInputVars.find(varName));
        }
    }
}

void Interpreter::SIZE(string& arg) //SIZE
{
    string varName = arg.substr(sizeOperator.size());
    varName = trim(varName);

    if(!varName.empty())
    {
        if(vars.find(varName) != vars.end())
        {
            Variable tmp_variable = vars[varName];

            cout << "\n";
            if(tmp_variable.getType() == integer)
                cout << sizeof(tmp_variable.getInteger()) << " bytes";
            else if(tmp_variable.getType() == doubl)
                cout << sizeof(tmp_variable.getDouble()) << " bytes";
            else if(tmp_variable.getType() == str)
                cout << sizeof(tmp_variable.getString()) << " bytes";
            else if(tmp_variable.getType() == boolean)
                cout << sizeof(tmp_variable.getBool()) << " bytes";
        }
    }
}

void Interpreter::SETVAL(string& arg) //SETVAL
{
    string line = arg.substr(setValOperator.size());
    line = trim(line);

    if(!line.empty())
    {
        size_t spacePosition = line.find_last_of(' ');

        string varName = line.substr(0, spacePosition);
        varName = trim(varName);

        string varVal = line.substr(spacePosition);
        varVal = trim(varVal);

        if(mustInputVars.find(varName) != mustInputVars.end())
        {
            string varType = mustInputVars[varName];
            Variable tmp_variable;
            tmp_variable.setType(varType);

            if(varType == str) //LINE
            {
                tmp_variable.setString(varVal);
            }
            else if(varType == integer) //INTEGER
            {
                int intVal = 0;

                stringstream converter(varVal);
                converter >> intVal;
                converter.flush();

                tmp_variable.setInteger(intVal);
            }
            else if(varType == doubl) //DOUBLE
            {
                double doubleVal = 0;

                stringstream converter(varVal);
                converter >> doubleVal;
                converter.flush();

                tmp_variable.setDouble(doubleVal);
            }
            else if(varType == boolean) //BOOL
            {
                bool boolValue = false;
                if(strstr(varVal.c_str(), "TRUE") || strstr(varVal.c_str(), "true"))
                    boolValue = true;
                else if(!strstr(varVal.c_str(), "TRUE") || !strstr(varVal.c_str(), "true"))
                {
                    stringstream converter(varVal);
                    converter >> std::boolalpha >> boolValue;
                    converter.flush();
                }
                tmp_variable.setBool(boolValue);
            }

            mustInputVars.erase(mustInputVars.find(varName));
            vars.insert(pair<string, Variable> (varName, tmp_variable));
        }
        else if(vars.find(varName) != vars.end())
        {
            Variable tmp_variable = vars[varName];

            if(tmp_variable.getType() == str) //LINE
            {
                tmp_variable.setString(varVal);
            }
            else if(tmp_variable.getType() == integer) //INTEGER
            {
                int intVal = 0;

                stringstream converter(varVal);
                converter >> intVal;
                converter.flush();

                tmp_variable.setInteger(intVal);
            }
            else if(tmp_variable.getType() == doubl) //DOUBLE
            {
                double doubleVal = 0;

                stringstream converter(varVal);
                converter >> doubleVal;
                converter.flush();

                tmp_variable.setDouble(doubleVal);
            }
            else if(tmp_variable.getType() == boolean) //BOOL
            {
                bool boolValue = false;
                if(strstr(varVal.c_str(), "TRUE") || strstr(varVal.c_str(), "true"))
                    boolValue = true;
                else if(!strstr(varVal.c_str(), "TRUE") || !strstr(varVal.c_str(), "true"))
                {
                    stringstream converter(varVal);
                    converter >> std::boolalpha >> boolValue;
                    converter.flush();
                }
                tmp_variable.setBool(boolValue);
            }

            vars.erase(vars.find(varName));
            vars.insert(pair<string, Variable> (varName, tmp_variable));
        }
    }
}

void Interpreter::DOBLOCK(string& arg) //DOBLOCK
{
    arg = arg.substr(arg.find_first_of(' '));
    arg = trim(arg);

    map<string, vector<string> >::const_iterator iter;
    for(iter = blocks.begin(); iter != blocks.end(); iter++)
    {
        if(iter->first == arg)
            doBlock(iter->second);
    }
}

void Interpreter::BLOCKLIST()
{
    map<string, vector<string> >::const_iterator iter;
    int ix = 1;
    for(iter = blocks.begin(); iter != blocks.end(); iter++, ix++)
    {
        switch(iter->second.size()-1)
        {
        case 1:
            cout << ix << ": " << iter->first << " [1 line]" << endl;
            break;
        default:
            cout << ix << ": " << iter->first << " [" << iter->second.size()-1  << " lines]" << endl;
        }
    }
}

void Interpreter::IMPORT(string& file)
{
    file = file.substr(file.find_first_of(' '));
    file = trim(file);

    ifstream input(file.c_str());
    if(input.is_open())
    {
        for(inputLine = readCode(input); !input.eof(); inputLine = readCode(input))
            code.push_back(inputLine);
        input.close();
    }
}


//arithmetic operations with vars

void Interpreter::add(string &line) //addition
{
    Variable tmp_variable;
    Variable tmp_variable2;

    size_t plusPosition = line.find('+');
    string firstVarName = line.substr(0, plusPosition);
    firstVarName = trim(firstVarName);

    string secondVarName = line.substr(plusPosition+2);
    secondVarName = trim(secondVarName);

    stringstream convert(secondVarName);
    double plusVal = 0;
    convert >> plusVal;
    convert.flush();

    if(plusVal == 0) //variable + variable
    {
        if(firstVarName == secondVarName)
        {
            tmp_variable = vars[firstVarName];

            if(tmp_variable.getType() == integer)
                tmp_variable.setInteger(tmp_variable.getInteger() + tmp_variable.getInteger());
            else if(tmp_variable.getType() == doubl)
                tmp_variable.setDouble(tmp_variable.getDouble() + tmp_variable.getDouble());
        }
        else
        {
            tmp_variable = vars[firstVarName];
            tmp_variable2 = vars[secondVarName];

            if(tmp_variable.getType() == integer && tmp_variable2.getType() == integer)
                tmp_variable.setInteger(tmp_variable.getInteger() + tmp_variable2.getInteger());
            else if(tmp_variable.getType() == doubl && tmp_variable2.getType() == doubl)
                tmp_variable.setDouble(tmp_variable.getDouble() + tmp_variable2.getDouble());
            /*else if(tmp_variable.getType() == integer && tmp_variable2.getType() == doubl)
                tmp_variable.setInteger((double)tmp_variable.getInteger() + tmp_variable2.getDouble());
            else if(tmp_variable.getType() == doubl && tmp_variable2.getType() == integer)
                tmp_variable.setInteger(tmp_variable.getInteger() + (double)tmp_variable2.getInteger());*/
        }
    }
    else //variable + number
    {
        tmp_variable = vars[firstVarName];

        if(tmp_variable.getType() == integer)
            tmp_variable.setInteger(tmp_variable.getInteger() + plusVal);
        else if(tmp_variable.getType() == doubl)
            tmp_variable.setDouble(tmp_variable.getDouble() + plusVal);
    }

    vars.erase(vars.find(firstVarName));
    vars.insert(pair<string, Variable> (firstVarName, tmp_variable));
}

void Interpreter::subtr(string &line) //subtraction
{
    Variable tmp_variable;
    Variable tmp_variable2;

    size_t minusPosition = line.find('-');
    string firstVarName = line.substr(0, minusPosition);
    firstVarName = trim(firstVarName);

    string secondVarName = line.substr(minusPosition+2);
    secondVarName = trim(secondVarName);

    stringstream convert(secondVarName);
    double subtrVal = 0;
    convert >> subtrVal;
    convert.flush();

    if(subtrVal == 0) //variable - variable
    {
        if(firstVarName == secondVarName)
        {
            tmp_variable = vars[firstVarName];

            if(tmp_variable.getType() == integer)
                tmp_variable.setInteger(tmp_variable.getInteger() - tmp_variable.getInteger());
            else if(tmp_variable.getType() == doubl)
                tmp_variable.setDouble(tmp_variable.getDouble() - tmp_variable.getDouble());
        }
        else
        {
            tmp_variable = vars[firstVarName];
            tmp_variable2 = vars[secondVarName];

            if(tmp_variable.getType() == integer && tmp_variable2.getType() == integer)
                tmp_variable.setInteger(tmp_variable.getInteger() - tmp_variable2.getInteger());
            else if(tmp_variable.getType() == doubl && tmp_variable2.getType() == doubl)
                tmp_variable.setDouble(tmp_variable.getDouble() - tmp_variable2.getDouble());
            /*else if(tmp_variable.getType() == integer && tmp_variable2.getType() == doubl)
                tmp_variable.setInteger((double)tmp_variable.getInteger() - tmp_variable2.getDouble());
            else if(tmp_variable.getType() == doubl && tmp_variable2.getType() == integer)
                tmp_variable.setInteger(tmp_variable.getInteger() - (double)tmp_variable2.getInteger());*/
        }
    }
    else //variable - number
    {
        tmp_variable = vars[firstVarName];

        if(tmp_variable.getType() == integer)
            tmp_variable.setInteger(tmp_variable.getInteger() - subtrVal);
        else if(tmp_variable.getType() == doubl)
            tmp_variable.setDouble(tmp_variable.getDouble() - subtrVal);
    }

    vars.erase(vars.find(firstVarName));
    vars.insert(pair<string, Variable> (firstVarName, tmp_variable));
}

void Interpreter::multipl(string &line) //mulriplication
{
    Variable tmp_variable;
    Variable tmp_variable2;

    size_t umnozhPosition = line.find('*');
    string firstVarName = line.substr(0, umnozhPosition);
    firstVarName = trim(firstVarName);

    string secondVarName = line.substr(umnozhPosition+2);
    secondVarName = trim(secondVarName);

    stringstream convert(secondVarName);
    double multiplVal = 0;
    convert >> multiplVal;
    convert.flush();

    if(multiplVal == 0) //variable * variable
    {
        if(firstVarName == secondVarName)
        {
            tmp_variable = vars[firstVarName];

            if(tmp_variable.getType() == integer)
                tmp_variable.setInteger(tmp_variable.getInteger() * tmp_variable.getInteger());
            else if(tmp_variable.getType() == doubl)
                tmp_variable.setDouble(tmp_variable.getDouble() * tmp_variable.getDouble());
        }
        else
        {
            tmp_variable = vars[firstVarName];
            tmp_variable2 = vars[secondVarName];

            if(tmp_variable.getType() == integer && tmp_variable2.getType() == integer)
                tmp_variable.setInteger(tmp_variable.getInteger() * tmp_variable2.getInteger());
            else if(tmp_variable.getType() == doubl && tmp_variable2.getType() == doubl)
                tmp_variable.setDouble(tmp_variable.getDouble() * tmp_variable2.getDouble());
            /*else if(tmp_variable.getType() == integer && tmp_variable2.getType() == doubl)
                tmp_variable.setInteger((double)tmp_variable.getInteger() * tmp_variable2.getDouble());
            else if(tmp_variable.getType() == doubl && tmp_variable2.getType() == integer)
                tmp_variable.setInteger(tmp_variable.getInteger() * (double)tmp_variable2.getInteger());*/
        }
    }
    else //variable * number
    {
        tmp_variable = vars[firstVarName];

        if(tmp_variable.getType() == integer)
            tmp_variable.setInteger(tmp_variable.getInteger() * multiplVal);
        else if(tmp_variable.getType() == doubl)
            tmp_variable.setDouble(tmp_variable.getDouble() * multiplVal);
    }

    vars.erase(vars.find(firstVarName));
    vars.insert(pair<string, Variable> (firstVarName, tmp_variable));
}

void Interpreter::div(string &line) //division
{
    Variable tmp_variable;
    Variable tmp_variable2;

    size_t delPosition = line.find('/');
    string firstVarName = line.substr(0, delPosition);
    firstVarName = trim(firstVarName);

    string secondVarName = line.substr(delPosition+2);
    secondVarName = trim(secondVarName);

    stringstream convert(secondVarName);
    double divVal = 0;
    convert >> divVal;
    convert.flush();

    if(divVal == 0) //variable / variable
    {
        if(firstVarName == secondVarName)
        {
            tmp_variable = vars[firstVarName];

            if(tmp_variable.getType() == integer)
                tmp_variable.setInteger(tmp_variable.getInteger() / tmp_variable.getInteger());
            else if(tmp_variable.getType() == doubl)
                tmp_variable.setDouble(tmp_variable.getDouble() / tmp_variable.getDouble());
        }
        else
        {
            tmp_variable = vars[firstVarName];
            tmp_variable2 = vars[secondVarName];

            if(tmp_variable.getType() == integer && tmp_variable2.getType() == integer)
                tmp_variable.setInteger(tmp_variable.getInteger() / tmp_variable2.getInteger());
            else if(tmp_variable.getType() == doubl && tmp_variable2.getType() == doubl)
                tmp_variable.setDouble(tmp_variable.getDouble() / tmp_variable2.getDouble());
            /*else if(tmp_variable.getType() == integer && tmp_variable2.getType() == doubl)
                tmp_variable.setInteger((double)tmp_variable.getInteger() / tmp_variable2.getDouble());
            else if(tmp_variable.getType() == doubl && tmp_variable2.getType() == integer)
                tmp_variable.setInteger(tmp_variable.getInteger() / (double)tmp_variable2.getInteger());*/
        }
    }
    else //variable / number
    {
        if(tmp_variable.getType() == integer)
            if((int)divVal == tmp_variable.getInteger()) tmp_variable.setInteger(1);
            else tmp_variable.setInteger(tmp_variable.getInteger() / divVal);
        else if(tmp_variable.getType() == doubl)
            if(divVal == tmp_variable.getDouble()) tmp_variable.setDouble(1);
            else tmp_variable.setDouble(tmp_variable.getDouble() / divVal);
    }

    vars.erase(vars.find(firstVarName));
    vars.insert(pair<string, Variable> (firstVarName, tmp_variable));
}

void Interpreter::increment(string &line) //increment
{
    Variable tmp_variable;

    size_t incrementPosition = line.find("++");
    string varName = line.substr(0, incrementPosition);
    varName = trim(varName);

    tmp_variable = vars[varName];

    if(tmp_variable.getType() == integer)
        tmp_variable.setInteger(tmp_variable.getInteger()+1);
    else if(tmp_variable.getType() == doubl)
        tmp_variable.setDouble(tmp_variable.getDouble()+1);

    vars.erase(vars.find(varName));
    vars.insert(pair<string, Variable> (varName, tmp_variable));
}

void Interpreter::decrement(string &line) //decrement
{
    Variable tmp_variable;

    size_t incrementPosition = line.find("--");
    string varName = line.substr(0, incrementPosition);
    varName = trim(varName);

    tmp_variable = vars[varName];

    if(tmp_variable.getType() == integer)
        tmp_variable.setInteger(tmp_variable.getInteger()-1);
    else if(tmp_variable.getType() == doubl)
        tmp_variable.setDouble(tmp_variable.getDouble()-1);

    vars.erase(vars.find(varName));
    vars.insert(pair<string, Variable> (varName, tmp_variable));
}
