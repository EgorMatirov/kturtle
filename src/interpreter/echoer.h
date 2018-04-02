/*
	Copyright (C) 2003-2006 Cies Breijs <cies AT kde DOT nl>
	Copyright (C) 2018 Mikhail Ivchenko <ematirov@gmail.com>

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


#ifndef _ECHOER_H_
#define _ECHOER_H_


#include "executer.h"

#include <QDebug>
#include <cmath>
#include <iostream>
#include <iomanip>


const double DegToRad = M_PI / 180.0;
/**
 * @short Echoes signals to a QTextStream
 *
 * Just echos the signals it gets.
 * Useful for creating UnitTests.
 *
 * @author Cies Breijs
 */
class Echoer : public QObject
{
	Q_OBJECT

	public:
		/**
		 * Default Constructor
		 */
        explicit Echoer(QObject* parent = 0) { setParent(parent); }

		/**
		 * Default Destructor
		 */
		virtual ~Echoer() = default;

        /**
		 * Connects all its slots to the signals of the Executer
		 */
		void connectAllSlots(Executer* executer) {
			connect(executer, SIGNAL(reset()),
				SLOT(reset()));
			connect(executer, SIGNAL(clear()),
				SLOT(clear()));
			connect(executer, SIGNAL(center()),
				SLOT(center()));
			connect(executer, SIGNAL(go(double, double)),
				SLOT(go(double, double)));
			connect(executer, SIGNAL(goX(double)),
				SLOT(goX(double)));
			connect(executer, SIGNAL(goY(double)),
				SLOT(goY(double)));
			connect(executer, SIGNAL(forward(double)),
				SLOT(forward(double)));
			connect(executer, SIGNAL(backward(double)),
				SLOT(backward(double)));
			connect(executer, SIGNAL(direction(double)),
				SLOT(direction(double)));
			connect(executer, SIGNAL(turnLeft(double)),
				SLOT(turnLeft(double)));
			connect(executer, SIGNAL(turnRight(double)),
				SLOT(turnRight(double)));
			connect(executer, SIGNAL(penWidth(double)),
				SLOT(penWidth(double)));
			connect(executer, SIGNAL(penUp()),
				SLOT(penUp()));
			connect(executer, SIGNAL(penDown()),
				SLOT(penDown()));
			connect(executer, SIGNAL(penColor(double, double, double)),
				SLOT(penColor(double, double, double)));
			connect(executer, SIGNAL(canvasColor(double, double, double)),
				SLOT(canvasColor(double, double, double)));
			connect(executer, SIGNAL(canvasSize(double, double)),
				SLOT(canvasSize(double, double)));
			connect(executer, SIGNAL(spriteShow()),
				SLOT(spriteShow()));
			connect(executer, SIGNAL(spriteHide()),
				SLOT(spriteHide()));
			connect(executer, SIGNAL(print(const QString&)),
				SLOT(print(const QString&)));
			connect(executer, SIGNAL(fontSize(double)),
				SLOT(fontSize(double)));
		}

	double x = 0;
	double y = 0;
	double angle = 0;
	bool isPenUp = false;


	public slots:
		void reset() { }
		void clear() { }
		void center() {  }
		void go(double arg0, double arg1) {
            if(!isPenUp)
            {
                std::cout << std::fixed << std::setw(3) << x << " " << y << " " << arg0 << " " << arg1 << std::endl;
            }
            x = arg0; y = arg1;
        }
		void goX(double arg0) { go(arg0, y); }
		void goY(double arg0) { go(x, arg0); }
		void forward(double arg0) {
			double x2 = x + (arg0 * std::sin(angle * DegToRad));
			double y2 = y - (arg0 * std::cos(angle * DegToRad));
			go(x2, y2);
		}
		void backward(double arg0) {
			double x2 = x - ( arg0 * std::sin(angle * DegToRad) );
			double y2 = y + ( arg0 * std::cos(angle * DegToRad) );
			go(x2, y2);

		}
		void direction(double arg0) { angle = arg0; }
		void turnLeft(double arg0) { angle = angle - arg0; }
		void turnRight(double arg0) { angle = angle + arg0; }
		void penWidth(double arg0) { }
		void penUp() { isPenUp = true; }
		void penDown() { isPenUp = false; }
		void penColor(double arg0, double arg1, double arg2) {  }
		void canvasColor(double arg0, double arg1, double arg2) { }
		void canvasSize(double arg0, double arg1) { }
		void spriteShow() { }
		void spriteHide() { }
		void print(const QString& arg0) { }
		void fontSize(double arg0) {}

};

#endif  // _ECHOER_H_


