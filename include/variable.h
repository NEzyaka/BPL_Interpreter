/****************************************************************************
**
** @file Variable class header & implementation
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

#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

class Variable
{
public:
    void setType(std::string type) { varType = type; } //setting type
    std::string getType() { return varType; } //getting type

    void setInteger(int i) { integerVal = i; } //setting INTEGER value
    int getInteger() { return integerVal; } //getting INTEGER value

    void setString(std::string s) { lineVal = s; } //setting LINE value
    std::string getString() { return lineVal; } //getting LINE value

    void setBool(bool b) { boolVal = b; } //setting BOOL value
    bool getBool() { return boolVal; } //getting BOOL value

    void setDouble(double d) { doubleVal = d; } //setting DOUBLE value
    double getDouble() { return doubleVal; } //getting DOUBLE value

    Variable() {}
    virtual ~Variable() {}
private:
    std::string varType; //type
    int integerVal; //INTEGER value
    std::string lineVal; //LINE value
    bool boolVal; //BOOL value
    double doubleVal; //DOUBLE value

};

#endif // VARIABLE_H
