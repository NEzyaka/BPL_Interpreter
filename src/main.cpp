/****************************************************************************
**
** @file main source
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
#include <cstdlib>
#include <cstdio>
#include <sstream>

#include "interpreter.h"

void version() //show information about version/build
{
    std::cout << "BPL Interpreter Beta\n 16.01 Preview\n"
              "Copyright (C) 2015 Nikita Mironov\n"
              "\n"
              "This program comes with ABSOLUTELY NO WARRANTY.\n"
              "This is free software, and you are welcome to redistribute it\n"
              "under certain conditions.\n"
              "\n"
              "Start this program with command prompt argument '-license'\n"
              "or see <http://www.gnu.org/licenses/gpl> for show terms of GNU GPL v3.\n";
}

void showLicense() //show terms of GNU GPL v3
{
    system("start COPYING.txt");
}

int main(int argc, char** argv)
{
    system("title BPL Interpreter 16.01 Preview");

    Interpreter* interpreter = new Interpreter;

    if(argc >= 2)
    {
        std::string* arg = new std::string("");
        std::stringstream* convert = new std::stringstream(argv[1]);
        *convert >> *arg;
        delete convert;

        interpreter->filename = *arg;

        if(argc == 3)
        {
            std::string* arg = new std::string("");
            std::stringstream* convert = new std::stringstream(argv[2]);
            *convert >> *arg;

            if(*arg == "-edit") //editing of file
            {
                interpreter->getKeyboardCode();
                return 0;
            }

            delete arg;
            delete convert;
        }

        if(*arg == "-ver")
            version();
        else if(*arg == "-license")
            showLicense();

        else interpreter->interpret();
        delete arg;
    }
    else
    {
        std::cout << "Enter the path of file: ";
        std::getline(std::cin, interpreter->filename);
        interpreter->interpret();
    }

    delete interpreter;
    getchar();

    return 0;
}

