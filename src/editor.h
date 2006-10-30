/*
	Copyright (C) 2003-2006 Cies Breijs <cies # kde ! nl>

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

#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <cmath>

#include <QFrame>
#include <QPixmap>
#include <QTextCursor>
#include <QTextEdit>

#include <kurl.h>

#include "highlighter.h"

class QTextEdit;
class QHBoxLayout;
class QPaintEvent;

static const QColor LINE_HIGHLIGHT_COLOR(239, 247, 255);
static const QColor WORD_HIGHLIGHT_COLOR(255, 255, 156);
static const QColor ERROR_HIGHLIGHT_COLOR(255, 200, 200);
static const int EXTRA_SATURATION = 30;  // used for drawing the highlighted background
static const int EDITOR_MARGIN = 2;  // some margin that can't be set to zero, yet painters should know it
static const int CURSOR_RECT_MARGIN = 5;  // another margin that cannot be traced
static const int LINENUMBER_SPACING = 2;  // sets the margin for the line numbers


//BEGIN LineNumbers class

class LineNumbers : public QWidget
{
	Q_OBJECT

	public:
		LineNumbers(QWidget *parent, QTextEdit *te) : QWidget(parent), editor(te), maxWidth(0) {}
		~LineNumbers() {}
	
		void setFont(const QFont& f) { QWidget::setFont(f); }

		void setWidth(int w) {
			if (w == maxWidth) return;  // save some cpu cycles
			maxWidth = w;
			QString s("");
			for (; w > 0; w--) s += "0";
			setFixedWidth(fontMetrics().width(s) + 2*LINENUMBER_SPACING);
		}

		void paintEvent(QPaintEvent*) {
			QAbstractTextDocumentLayout* layout = editor->document()->documentLayout();
			int contentsY = editor->verticalScrollBar()->value();
			qreal pageBottom = contentsY + editor->viewport()->height();
			const QFontMetrics fm = fontMetrics();
			const int ascent = fontMetrics().ascent() + 1;  // height = ascent + descent + 1
			int lineCount = 1;
			QPainter painter(this);
			for (QTextBlock block = editor->document()->begin(); block.isValid(); block = block.next(), ++lineCount) {
				const QRectF boundingRect = layout->blockBoundingRect(block);
				QPointF position = boundingRect.topLeft();
				if (position.y() + boundingRect.height() < contentsY) continue;
				if (position.y() > pageBottom) break;
				const QString txt = QString::number(lineCount);
				painter.drawText(width() - fm.width(txt) - LINENUMBER_SPACING, qRound(position.y()) - contentsY + ascent, txt);
			}
			painter.end();
		}

	private:
		QTextEdit *editor;
		int        maxWidth;
};

//END class LineNumbers


//BEGIN QTextEditor sub-class

class TextEdit : public QTextEdit
{
	Q_OBJECT

	public:
		explicit TextEdit(QWidget* parent = 0)
			: QTextEdit(parent), currentWordRect(QRect()), currentErrorRect(QRect()) {}

		void markCurrentWord(int startRow, int startCol, int endRow, int endCol) {
			currentWordRect = coordsToRect(startRow, startCol, endRow, endCol);
			viewport()->update();
		}

		void removeCurrentWordMark() {
			currentWordRect = QRect();
			viewport()->update();
		}

		void markCurrentError(int startRow, int startCol, int endRow, int endCol) {
			currentErrorRect = coordsToRect(startRow, startCol, endRow, endCol);
			viewport()->update();
		}

		void removeCurrentErrorMark() {
			currentErrorRect = QRect();
			viewport()->update();
		}

		void highlightCurrentLine() { viewport()->update(); }

		QRect currentLineRect() {
			// this method is also used for highlighting the background of the numbers
			QTextCursor cursor = textCursor();
			cursor.movePosition(QTextCursor::StartOfBlock);
			QRect rect = cursorRect(cursor);
			cursor.movePosition(QTextCursor::EndOfBlock);
			rect |= cursorRect(cursor);  // get the bounding rectangle of both rects
			rect.setX(0);
			rect.setWidth(viewport()->width());
			return rect;
		}

	protected:
		void paintEvent(QPaintEvent *event) {
			QPainter painter(viewport());
			painter.fillRect(currentLineRect(), QBrush(LINE_HIGHLIGHT_COLOR));
			// TODO actually a number of rectangles has to be painted for each line the
			// the word/error is on. so an array of QRects has to be stored in
			// currentWordRect and currentErrorRect.
			if (!currentWordRect.isNull())  painter.fillRect(currentWordRect,  QBrush(WORD_HIGHLIGHT_COLOR));
			if (!currentErrorRect.isNull()) painter.fillRect(currentErrorRect, QBrush(ERROR_HIGHLIGHT_COLOR));
			painter.end();
			QTextEdit::paintEvent(event);
		}

	private:
		QRect coordsToRect(int startRow, int startCol, int endRow, int endCol) {
			QTextCursor cursor(document());
			cursor.movePosition(QTextCursor::Start,         QTextCursor::MoveAnchor);
			cursor.movePosition(QTextCursor::NextBlock,     QTextCursor::MoveAnchor, startRow - 1);
			cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, startCol - 1);
			QRect rect = cursorRect(cursor).adjusted(CURSOR_RECT_MARGIN, 0, 0, 0);
			cursor.movePosition(QTextCursor::NextBlock,     QTextCursor::MoveAnchor, endRow - startRow);
			cursor.movePosition(QTextCursor::StartOfBlock,  QTextCursor::MoveAnchor);
			cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, endCol - 1);
			rect |= cursorRect(cursor).adjusted(0, 0, -CURSOR_RECT_MARGIN, 0);;  // get the bounding rectangle of both rects
			return rect;
		}

		QRect currentWordRect, currentErrorRect;
};

//END QTextEditor sub-class



class Editor : public QFrame
{
	Q_OBJECT

	public:
		Editor(QWidget *parent = 0);
		~Editor();

		QTextEdit* view() const { return editor; }
		QTextDocument* document() const { return editor->document(); }

		const KUrl& currentUrl() { return m_currentUrl; }
		void setCurrentUrl(const KUrl& url = KUrl());

		bool maybeSave();

		bool isModified() { return editor->document()->isModified(); }
		QString content() { return editor->document()->toPlainText(); }

		Token* currentToken(const QString& text, int cursorIndex) { return highlighter->formatType(text, cursorIndex); }

		void removeMarkings() {
			editor->removeCurrentWordMark();
			editor->removeCurrentErrorMark();
		}


	public slots:
		bool newFile();
		bool openFile(const KUrl &url = KUrl());
		void openExample(const QString& example, const QString& exampleName);
		bool saveFile(const KUrl &url = KUrl());
		bool saveFileAs();
		void toggleLineNumbers(bool b) { numbers->setVisible(b); }
		void setModified(bool);
		void setInsertMode(bool b);

		void markCurrentWord(int startRow, int startCol, int endRow, int endCol) {
			editor->markCurrentWord(startRow, startCol, endRow, endCol);
		}
		void markCurrentError(int startRow, int startCol, int endRow, int endCol) {
			editor->markCurrentError(startRow, startCol, endRow, endCol);
		}


	signals:
		void currentUrlChanged(const KUrl&);
		void modificationChanged(bool);
		void cursorPositionChanged(int row, int col, const QString& line);


	protected slots:
		void textChanged(int pos, int added, int removed);
		void cursorPositionChanged();

	protected:
		void paintEvent(QPaintEvent *event);


	private slots:
		void highlightCurrentLine() { this->update(); }

	private:
		void setContent(const QString&);

		TextEdit    *editor;  // TODO why pointers?
		Highlighter *highlighter;
		LineNumbers *numbers;
		QHBoxLayout *box;  // TODO is this relly needed?
		KUrl         m_currentUrl;
		QColor       highlightedLineBackgroundColor;  // the bg color of the current line's line number space
		int          currentLine;
};


#endif  // _EDITOR_H_
