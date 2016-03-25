/****************************************************************************
**
** @file main source
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

#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <sstream>

#include "interpreter.h"

using namespace std;

void version() //show information about version
{
    cout << "Turnip Runner 16.03 Preview\n"
         "Copyright (C) 2015-2016 Nikita Mironov\n"
         "\n"
         "This program comes with ABSOLUTELY NO WARRANTY.\n"
         "This is free software, and you are welcome to redistribute it\n"
         "under certain conditions.\n"
         "\n"
         "Start this program with command prompt argument '-license'\n"
         "or see <http://www.gnu.org/licenses/gpl> for show terms of GNU GPL v3.\n"
         "\n"
         "For more info see <https://github.com/NEzyaka/Turnip-Runner>\n";
}

void showLicense() //show terms of GNU GPL v3
{
#ifdef _WIN32
    system("start COPYING.txt");
#else
    system("COPYING.txt");
#endif //_WIN32
}

int main(int argc, char** argv)
{
#ifdef _WIN32
    system("title Turnip Runner 16.03 Preview");
#endif // _WIN32

    setlocale(0, ".65001"); //UTF-8 support

    Interpreter interpreter;
    bool isIDE = false;

    if(argc >= 2) //one ore more arguments
    {
        stringstream convert(argv[1]);
        string arg = convert.str();

        interpreter.filename = arg;

        if(argc == 3) //two arguments
        {
            stringstream convert(argv[2]);
            string subArg = convert.str();

            if(subArg == "-edit") //editing of file
            {
                interpreter.getKeyboardCode();
                return 0;
            }
            else if(subArg == "-IDE")
            {
                isIDE = true;
                interpreter.isIDE = true;
                interpreter.interpret();
            }
        }
        else if(argc == 2) //one argument
        {
            if(arg == "-ver") //show information about version
                version();
            else if(arg == "-license") //show terms of GNU GPL v3
                showLicense();
            else interpreter.interpret(); //interpret a file
        }
    }
    else //no arguments
    {
        cout << "Enter the path of file: ";
        getline(cin, interpreter.filename);
        interpreter.interpret();
    }

    if(!isIDE)
    {
        cout << endl << "Press any key to exit...";
        getch();
    }

    return 0;
}

