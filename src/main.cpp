/*
	Copyright (C) 2003-2006 Cies Breijs <cies AT kde DOT nl>

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public
	License as published by the Free Software Foundation; either
	version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public
	License along with this program; if not, write to the Free
	Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
	Boston, MA 02110-1301, USA.
*/


#include <iostream>

#include <QFile>
#include <QDebug>
#include <QCoreApplication>
#include <QCommandLineParser>

#include "interpreter/interpreter.h"  // for non gui mode
#include "interpreter/echoer.h"


const QString KTURTLE_MAGIC_1_0 = "kturtle-script-v1.0";


int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();

    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("i") << QLatin1String("input"),
                                        ("File or URL to open (in the GUI mode)"), QLatin1String("URL or file")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("d") << QLatin1String("dbus"),
                                        ("Starts KTurtle in D-Bus mode (without a GUI), good for automated unit test scripts")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("t") << QLatin1String("test"),
                                        ("Starts KTurtle in testing mode (without a GUI), directly runs the specified local file"),
                                        QLatin1String("file")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("l") << QLatin1String("lang"),
                                        ("Specifies the localization language by a language code, defaults to \"en_US\" (only works in testing mode)"),
                                        QLatin1String("code")));
    parser.addOption(QCommandLineOption(QStringList() << QLatin1String("p") << QLatin1String("parse"),
                                        ("Translates turtle code to embeddable C++ example strings (for developers only)"),
                                        QLatin1String("file")));

    parser.process(app);

    ///////////////// run without a gui /////////////////
    QString fileString = parser.value("test");
    QFile inputFile(fileString);

    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Could not open input file: " << qPrintable(parser.value("test")) << std::endl;
        std::cerr << "Exitting..." << std::endl;
        return 1;
    }

    QTextStream in(&inputFile);

    // check for our magic identifier
    QString s;
    s = in.readLine();
    if (s == KTURTLE_MAGIC_1_0) {
        s = "";
    }

    Translator::instance()->setLanguage();

    QString script = in.readAll();
    QString firstLine = s + QString("\n");
    script = firstLine + script;

    auto interpreter = new Interpreter(nullptr, true);  // set testing to true
    interpreter->initialize(script);

    (new Echoer())->connectAllSlots(interpreter->getExecuter());

    static const int MAX_ITERATION_STEPS = 2000;
    int i;
    for (i = 0; interpreter->state() != Interpreter::Finished && interpreter->state() != Interpreter::Aborted &&
                interpreter->getErrorList()->isEmpty() && i < MAX_ITERATION_STEPS; i++)
        interpreter->interpret();

    if (!interpreter->getErrorList()->isEmpty()) {
        std::cerr << "ERR> " << interpreter->getErrorStrings().join(", ").toStdString();
        return 1;
    }

    if (i == MAX_ITERATION_STEPS) {
        std::cerr << "ERR> Iterated more than " << MAX_ITERATION_STEPS << " steps. Execution terminated." << std::endl;
        return 1;
    }

    return 0;
}
