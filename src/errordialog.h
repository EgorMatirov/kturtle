/*
	Copyright (C) 2003-2006 Cies Breijs <cies AT kde DOT nl>

	This program is free software; you can redistribute it and/or
	modify it under the terms of version 2 of the GNU General Public
	License as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public
	License along with this program; if not, write to the Free
	Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
	Boston, MA 02110-1301, USA.
*/

#ifndef _ERRORDIALOG_H_
#define _ERRORDIALOG_H_

#include <QTableWidget>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLabel>

#include "interpreter/errormsg.h"


class ErrorDialog : public KDialog
{
	Q_OBJECT

	public:
		ErrorDialog(ErrorList* _errorList, QWidget* parent = 0);

	signals:
		void currentlySelectedError(int, int, int, int);

	private slots:
		void selectedErrorChangedProxy();

	private:
		ErrorList      *errorList;
		QTableWidget   *errorTable;
		QVBoxLayout    *baseLayout;
		QLabel         *label;
		QSpacerItem    *spacer;
};

#endif  // _ERRORDIALOG_H_

