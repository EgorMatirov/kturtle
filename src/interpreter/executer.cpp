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


// This file is originally written by Walter Scheppers, but allmost
// every aspect of it is slightly changed by Cies Breijs.


#include <math.h>
#include <unistd.h>  // for usleep();

#include <QTimer>  // for wait

#include <QtDebug>

#include <klocale.h>

#include "executer.h"

// this function is used in executer and canvas:
#define ROUND2INT(x) ( (x) >= 0 ? (int)( (x) + .5 ) : (int)( (x) - .5 ) )


void Executer::initialize(TreeNode* tree, ErrorList* _errorList)
{
	rootNode       = tree;
	newScope       = rootNode;
	currentNode    = rootNode;
	finished       = !currentNode->hasChildren();  // set finished to false unless the tree is empty
	errorList      = _errorList;

	breaking       = false;
	returning      = false;
	waiting        = false;
	returnValue    = 0;
	
	executeCurrent = false;

	functionTable.clear();
	globalVariableTable.clear();

	while (!functionStack.isEmpty()) delete functionStack.pop().variableTable;
}


void Executer::execute()
{
	if (finished || waiting) return;
	qDebug() << "Executer::execute() -- main execute loop called";

	if (executeCurrent) {
		// executeCurrent is used to make shure the currentNode will be executed
		// this way the tree will not be walked before the execution...
		qDebug() << ">> jumping to tree node, and executing: " << currentNode->token()->look();
		executeCurrent = false;
		execute(currentNode);
		return;
	} else if (returning) {
		delete functionStack.top().variableTable;
		currentNode = functionStack.pop().function;
		qDebug() << ">> returning to: " << currentNode->token()->look();
		currentNode->setValue(returnValue);
// 		returnValue = 0;      // DONT DO THIS... the the functionCall node that get recalled now handle it...
// 		returning   = false;
		execute(currentNode);
		return;
	} else {
		if (newScope == 0) {  // same scope
			qDebug() << ">> to next sibling";
			
			TreeNode* currentParent = currentNode->parent();
			currentNode = currentNode->nextSibling();
		
			if (currentNode == 0) {  // running off sibling list
				qDebug() << ">> to parent";
				currentNode = currentParent;
				if (currentNode == rootNode) {
					qDebug() << ">> hit root";
					finished = true;
					return;
				}
				
				qDebug() << ">> executing: " << currentNode->token()->look();
				execute(currentNode);
				return;
			}
		} else {  // entering new scope
			qDebug() << ">> to the new scope";
			currentNode = newScope;
			newScope = 0;
			
			// skip ahead to frist child (if any) else we will not get into the scope
			if (currentNode->hasChildren()) currentNode = currentNode->firstChild();
		}
	}
	
	qDebug() << ">> to top";
	while (currentNode->hasChildren() && currentNode->token()->type() != Token::Scope)
		currentNode = currentNode->firstChild();
	qDebug() << ">> executing: " << currentNode->token()->look();
	execute(currentNode);
}


void Executer::execute(TreeNode* node)
{
	if (finished) return;

	// this method executes one node at the time

	// if (currentNode->token()->type() != Token::Scope) qDebug() << "1234567890!!!!!";

	switch (node->token()->type()) {

//BEGIN GENERATED executer_switch_cpp CODE

/* The code between the line that start with "//BEGIN GENERATED" and "//END GENERATED"
 * is generated by "generate.rb" according to the definitions specified in
 * "definitions.rb". Please make all changes in the "definitions.rb" file, since all
 * all change you make here will be overwritten the next time "generate.rb" is run.
 * Thanks for looking at the code!
 */

		case Token::Unknown             : executeUnknown(node);             break;
		case Token::Root                : executeRoot(node);                break;
		case Token::Scope               : executeScope(node);               break;
		case Token::Variable            : executeVariable(node);            break;
		case Token::FunctionCall        : executeFunctionCall(node);        break;
		case Token::String              : /* a constant; do nothing */      break;
		case Token::Number              : /* a constant; do nothing */      break;
		case Token::True                : /* a constant; do nothing */      break;
		case Token::False               : /* a constant; do nothing */      break;
		case Token::Exit                : executeExit(node);                break;
		case Token::If                  : executeIf(node);                  break;
		case Token::Else                : executeElse(node);                break;
		case Token::Repeat              : executeRepeat(node);              break;
		case Token::While               : executeWhile(node);               break;
		case Token::For                 : executeFor(node);                 break;
		case Token::ForTo               : executeForTo(node);               break;
		case Token::Break               : executeBreak(node);               break;
		case Token::Return              : executeReturn(node);              break;
		case Token::Wait                : executeWait(node);                break;
		case Token::And                 : executeAnd(node);                 break;
		case Token::Or                  : executeOr(node);                  break;
		case Token::Not                 : executeNot(node);                 break;
		case Token::Equals              : executeEquals(node);              break;
		case Token::NotEquals           : executeNotEquals(node);           break;
		case Token::GreaterThan         : executeGreaterThan(node);         break;
		case Token::LessThan            : executeLessThan(node);            break;
		case Token::GreaterOrEquals     : executeGreaterOrEquals(node);     break;
		case Token::LessOrEquals        : executeLessOrEquals(node);        break;
		case Token::Addition            : executeAddition(node);            break;
		case Token::Substracton         : executeSubstracton(node);         break;
		case Token::Multiplication      : executeMultiplication(node);      break;
		case Token::Division            : executeDivision(node);            break;
		case Token::Power               : executePower(node);               break;
		case Token::Assign              : executeAssign(node);              break;
		case Token::Learn               : executeLearn(node);               break;
		case Token::ArgumentList        : executeArgumentList(node);        break;
		case Token::Reset               : executeReset(node);               break;
		case Token::Clear               : executeClear(node);               break;
		case Token::Center              : executeCenter(node);              break;
		case Token::Go                  : executeGo(node);                  break;
		case Token::GoX                 : executeGoX(node);                 break;
		case Token::GoY                 : executeGoY(node);                 break;
		case Token::Forward             : executeForward(node);             break;
		case Token::Backward            : executeBackward(node);            break;
		case Token::Direction           : executeDirection(node);           break;
		case Token::TurnLeft            : executeTurnLeft(node);            break;
		case Token::TurnRight           : executeTurnRight(node);           break;
		case Token::PenWidth            : executePenWidth(node);            break;
		case Token::PenUp               : executePenUp(node);               break;
		case Token::PenDown             : executePenDown(node);             break;
		case Token::PenColor            : executePenColor(node);            break;
		case Token::CanvasColor         : executeCanvasColor(node);         break;
		case Token::CanvasSize          : executeCanvasSize(node);          break;
		case Token::SpriteShow          : executeSpriteShow(node);          break;
		case Token::SpriteHide          : executeSpriteHide(node);          break;
		case Token::Print               : executePrint(node);               break;
		case Token::FontSize            : executeFontSize(node);            break;
		case Token::Random              : executeRandom(node);              break;

//END GENERATED executer_switch_cpp CODE

		default:
			qDebug() << "No method to execute this tree node, THIS SHOULDN'T HAPPEN!";
			break;

	}

}


VariableTable* Executer::currentVariableTable()
{
	if (functionStack.isEmpty())
		return &globalVariableTable;
	else
		return functionStack.top().variableTable;
}




bool Executer::checkParameterQuantity(TreeNode* node, uint quantity, int errorCode)
{
// 	qDebug() << "Executer::checkParameterQuantity";
	uint nodeSize = node->childCount();

	if (quantity == 0) {
		if (nodeSize == 0) return true;  // thats easy!
		i18n("Internationlalized string with an %1", QString("argument"));
		addError(i18n("The %1 command accepts no parameters.", node->token()->look()), *node->token(), errorCode);
		return false;
	}
	
// 	// CHECK THIS OUT LATER
// 	if (nodeSize != 0) // when all parameters are forgotten the parser puts a Unknown/tokEOL param, catch this:
// 		if (node->firstChild()->getToken().type == tokEOL) nodeSize = 0;
	
	if (nodeSize != quantity) {
		if (nodeSize < quantity) {
			addError(i18n("The %1 command was called with %2 but needs 1 parameter.", node->token()->look(), nodeSize), *node->token(), errorCode);
		} else {
			addError(i18n("The %1 command was called with %2 but only accepts 1 parameter.", node->token()->look(), nodeSize), *node->token(), errorCode);
		}
		return false;
	}
	return true; // if all tests passed
}


bool Executer::checkParameterType(TreeNode* node, int valueType, int errorCode)
{
// 	qDebug() << "Executer::checkParameterType";
	uint quantity = node->childCount();
	TreeNode* currentChild = node->firstChild();
	while (currentChild != 0) {
		if (currentChild->value()->type() != valueType) {
			switch (valueType) {
				case Value::String:
					if (quantity == 1)
						addError(i18n("The %1 command only accepts a string as its parameter.", node->token()->look()), *node->token(), errorCode);
					else
						addError(i18n("The %1 command only accepts strings as its parameters.", node->token()->look()), *node->token(), errorCode);
					break;
				
				case Value::Number:
					if (quantity == 1)
						addError(i18n("The %1 command only accepts a number as its parameter.", node->token()->look()), *node->token(), errorCode);
					else
						addError(i18n("The %1 command only accepts numbers as its parameters.", node->token()->look()), *node->token(), errorCode);
					break;
				
				case Value::Bool:
					if (quantity == 1)
						addError(i18n("The %1 command only accepts an answer as its parameter.", node->token()->look()), *node->token(), errorCode);
					else
						addError(i18n("The %1 command only accepts answers as its parameters.", node->token()->look()), *node->token(), errorCode);
					break;
			}
			
			return false;
		}

		currentChild = node->nextChild();
	}
	return true; // if all tests passed
}



//BEGIN GENERATED executer_cpp CODE

/* The code between the line that start with "//BEGIN GENERATED" and "//END GENERATED"
 * is generated by "generate.rb" according to the definitions specified in
 * "definitions.rb". Please make all changes in the "definitions.rb" file, since all
 * all change you make here will be overwritten the next time "generate.rb" is run.
 * Thanks for looking at the code!
 */

void Executer::executeUnknown(TreeNode* node) {
	qDebug() << "Executer::executeUnknown()";
	if (node->parent()->token()->type() == Token::Learn) {
		currentNode = node->parent();
		executeCurrent = true;
		return;
	}
}
void Executer::executeRoot(TreeNode* node) {
	qDebug() << "Executer::executeRoot()";
	node = node; // stop the warnings // this is a stud
}
void Executer::executeScope(TreeNode* node) {
	qDebug() << "Executer::executeScope()";
	// catch loops, they need to be managed...
	int parentTokenType = node->parent()->token()->type();
	if (parentTokenType == Token::If     ||
	    parentTokenType == Token::Repeat ||
	    parentTokenType == Token::While  ||
//	    parentTokenType == Token::ForIn  ||
	    parentTokenType == Token::ForTo) {
		currentNode = node->parent();
		executeCurrent = true;
		return;
	}
	newScope = node;
}
void Executer::executeVariable(TreeNode* node) {
	qDebug() << "Executer::executeVariable()";
	// no need to look up when assigning (in a for loop statement)
	if (node->parent()->token()->type() == Token::Assign ||
	    node->parent()->token()->type() == Token::ForTo)
		return;

	if (!functionStack.isEmpty() && 
	    functionStack.top().variableTable->contains(node->token()->look())) {
		qDebug() << "exists locally";
		node->setValue( (*functionStack.top().variableTable)[node->token()->look()] );
	} else if (globalVariableTable.contains(node->token()->look())) {
		qDebug() << "exists globally";
		node->setValue(globalVariableTable[node->token()->look()]);
	} else {
		addError(i18n("The variable '%1' was used without first beeing assigned to a value", node->token()->look()), *node->token(), 0);
	}
}
void Executer::executeFunctionCall(TreeNode* node) {
	qDebug() << "Executer::executeFunctionCall()";
	if (returning) {  // if the function is already executed and returns now
		returnValue = 0;
		returning = false;
		qDebug() << "==> functionReturned!";
		return;
	}

	if (!functionTable.contains(node->token()->look())) {
		addError(i18n("An unknown function with named '%1' was called", node->token()->look()), *node->token(), 0);
		return;
	}
	
	CalledFunction c;
	c.function      = node;
	c.variableTable = new VariableTable();
	functionStack.push(c);
	qDebug() << "==> functionCalled!";
	
	TreeNode* learnNode = functionTable[node->token()->look()];

	// if the parameter numbers are not equal...
	if (node->childCount() != learnNode->child(1)->childCount()) {
		addError(
			i18n("The function '%1' was called with %2, while it should should be called with %3",
				node->token()->look(),
				i18np("1 parameter", "%n parameters", node->childCount()),
				i18np("1 parameter", "%n parameters", learnNode->child(1)->childCount())
			),
			*node->token(), 0);
		return;
	}
		
	for (uint i = 0; i < node->childCount(); i++) {
		functionStack.top().variableTable->insert(learnNode->child(1)->child(i)->token()->look(), node->child(i)->value());
		qDebug() << "inserted variable " << learnNode->child(1)->child(i)->token()->look() << " on function stack";
	}
	newScope = learnNode->child(2);
}
void Executer::executeExit(TreeNode* node) {
	qDebug() << "Executer::executeExit()";
	node = node; // stop the warnings
	finished = true;
}
void Executer::executeIf(TreeNode* node) {
	qDebug() << "Executer::executeIf()";
	QString id = QString("__%1_%2").arg(node->token()->look()).arg((long)node);
	if (currentVariableTable()->contains(id)) {
		currentVariableTable()->remove(id);
		return;
	}
	currentVariableTable()->insert(id, Value()); // store a empty Value just to know we executed once
	if (node->child(0)->value()->boolean()) {
		newScope = node->child(1);
	} else {
		if (node->childCount() >= 3)
			newScope = node->child(2); // execute the else part
	}
}
void Executer::executeElse(TreeNode* node) {
	qDebug() << "Executer::executeElse()";
	execute(node->child(0));  // execute the scope, that's all...
}
void Executer::executeRepeat(TreeNode* node) {
	qDebug() << "Executer::executeRepeat()";
	breaking = false;
	QString id = QString("__%1_%2").arg(node->token()->look()).arg((long)node);
	// the iteration state is stored on the variable table
	if (currentVariableTable()->contains(id)) {
		int currentCount = ROUND2INT((*currentVariableTable())[id].number());
		if (currentCount > 0) {
			(*currentVariableTable())[id].setNumber(currentCount - 1);
		} else {
			currentVariableTable()->remove(id);
			return;
		}
	} else {
		currentVariableTable()->insert(id, Value((double)(ROUND2INT(node->child(0)->value()->number()) - 1)));
	}
	newScope = node->child(1);
}
void Executer::executeWhile(TreeNode* node) {
	qDebug() << "Executer::executeWhile()";
	// first time this gets called the expression is allready executed
	// after one iteration the expression is not automatically re-executed.
	// so we do the following on every call to executeWhile:
	//     exec scope, exec expression, exec scope, exec expresion, ...

	QString id = QString("__%1_%2").arg(node->token()->look()).arg((long)node);
	if (currentVariableTable()->contains(id)) {
		newScope = node; // re-execute the expression
		currentVariableTable()->remove(id);
		return;
	}
	currentVariableTable()->insert(id, Value()); // store a empty Value just to know we executed once

	if (node->child(0)->value()->boolean())
		newScope = node->child(1); // (re-)execute the scope
	else
		currentVariableTable()->remove(id); // clean-up, keep currenNode on currentNode so the next sibling we be run next
}
void Executer::executeFor(TreeNode* node) {
	qDebug() << "Executer::executeFor()";
	qCritical("Executer::executeFor(): should have been translated to Token::ForTo by the parser");
	node = node; // stop the warnings
}
void Executer::executeForTo(TreeNode* node) {
	qDebug() << "Executer::executeForTo()";
	// first time this gets called the expressions are allready executed
	// after one iteration the expression is not re-executed.
	// so we do: exec scope, exec expressions, exec scope, exec expresions, ...

	bool firstIteration = false;
	if (functionStack.isEmpty() || functionStack.top().function != node) {
		// if this for loop is called for the first time...
		CalledFunction c;
		c.function      = node;
		c.variableTable = new VariableTable();
		functionStack.push(c);

		currentVariableTable()->insert(node->child(0)->token()->look(), Value(node->child(1)->value()->number()));
		firstIteration = true;
	}

	QString id = QString("__%1_%2").arg(node->token()->look()).arg((long)node);

	if (currentVariableTable()->contains(id)) {
		newScope = node; // re-execute the expressions
		currentVariableTable()->remove(id);
		return;
	}
	currentVariableTable()->insert(id, Value()); // store a empty Value just to know we executed once

	double currentCount   = (*currentVariableTable())[node->child(0)->token()->look()].number();
	double startCondition = node->child(1)->value()->number();
	double endCondition   = node->child(2)->value()->number();
	double step           = node->child(3)->value()->number();

	if ((startCondition < endCondition && currentCount + step < endCondition) ||
	    (startCondition > endCondition && currentCount + step > endCondition)) {
		if (!firstIteration)
			(*currentVariableTable())[node->child(0)->token()->look()].setNumber(currentCount + step);
		newScope = node->child(4); // (re-)execute the scope
	} else {
		// cleaning up after last iteration...
		delete functionStack.top().variableTable;
		functionStack.pop();
	}
}
void Executer::executeBreak(TreeNode* node) {
	qDebug() << "Executer::executeBreak()";
	if (!checkParameterQuantity(node, 0, 20000+Token::Break*100+90)) return;
}
void Executer::executeReturn(TreeNode* node) {
	qDebug() << "Executer::executeReturn()";
	returnValue = node->child(0)->value();
	returning   = true;
}
void Executer::executeWait(TreeNode* node) {
	qDebug() << "Executer::executeWait()";
	waiting = true;
	QTimer::singleShot(1000*((int)node->child(0)->value()->number()), this, SLOT(stopWaiting()));
}
void Executer::executeAnd(TreeNode* node) {
	qDebug() << "Executer::executeAnd()";
	// TODO: maybe add some error handling here...
	node->value()->setBool(node->child(0)->value()->boolean() && node->child(1)->value()->boolean());
}
void Executer::executeOr(TreeNode* node) {
	qDebug() << "Executer::executeOr()";
	// TODO: maybe add some error handling here...
	node->value()->setBool(node->child(0)->value()->boolean() || node->child(1)->value()->boolean());
}
void Executer::executeNot(TreeNode* node) {
	qDebug() << "Executer::executeNot()";
	// TODO: maybe add some error handling here...
	node->value()->setBool(!node->child(0)->value()->boolean());
}
void Executer::executeEquals(TreeNode* node) {
	qDebug() << "Executer::executeEquals()";
	node->value()->setBool(*node->child(0)->value() == node->child(1)->value());
}
void Executer::executeNotEquals(TreeNode* node) {
	qDebug() << "Executer::executeNotEquals()";
	node->value()->setBool(*node->child(0)->value() != node->child(1)->value());
}
void Executer::executeGreaterThan(TreeNode* node) {
	qDebug() << "Executer::executeGreaterThan()";
	node->value()->setBool(*node->child(0)->value() > node->child(1)->value());
}
void Executer::executeLessThan(TreeNode* node) {
	qDebug() << "Executer::executeLessThan()";
	node->value()->setBool(*node->child(0)->value() < node->child(1)->value());
}
void Executer::executeGreaterOrEquals(TreeNode* node) {
	qDebug() << "Executer::executeGreaterOrEquals()";
	node->value()->setBool(*node->child(0)->value() >= node->child(1)->value());
}
void Executer::executeLessOrEquals(TreeNode* node) {
	qDebug() << "Executer::executeLessOrEquals()";
	node->value()->setBool(*node->child(0)->value() <= node->child(1)->value());
}
void Executer::executeAddition(TreeNode* node) {
	qDebug() << "Executer::executeAddition()";
	if (node->child(0)->value()->type() == Value::Number && node->child(1)->value()->type() == Value::Number) {
		node->value()->setNumber(node->child(0)->value()->number() + node->child(1)->value()->number());
	} else {
		node->value()->setString(node->child(0)->value()->string().append(node->child(1)->value()->string()));
	}
}
void Executer::executeSubstracton(TreeNode* node) {
	qDebug() << "Executer::executeSubstracton()";
	if (node->child(0)->value()->type() == Value::Number && node->child(1)->value()->type() == Value::Number) {
		node->value()->setNumber(node->child(0)->value()->number() - node->child(1)->value()->number());
	} else {
		if (node->child(0)->value()->type() != Value::Number)
			addError(i18n("You tried to substract from a non-number, '%1'", node->child(0)->token()->look()), *node->token(), 0);
		if (node->child(1)->value()->type() != Value::Number)
			addError(i18n("You tried to substract a non-number, '%1'", node->child(1)->token()->look()), *node->token(), 0);
	}
}
void Executer::executeMultiplication(TreeNode* node) {
	qDebug() << "Executer::executeMultiplication()";
	if (node->child(0)->value()->type() == Value::Number && node->child(1)->value()->type() == Value::Number) {
		node->value()->setNumber(node->child(0)->value()->number() * node->child(1)->value()->number());
	} else {
		if (node->child(0)->value()->type() != Value::Number)
			addError(i18n("You tried to multiplicate a non-number, '%1'", node->child(0)->token()->look()), *node->token(), 0);
		if (node->child(1)->value()->type() != Value::Number)
			addError(i18n("You tried to multiplicate by a non-number, '%1'", node->child(1)->token()->look()), *node->token(), 0);
	}
}
void Executer::executeDivision(TreeNode* node) {
	qDebug() << "Executer::executeDivision()";
	if (node->child(0)->value()->type() == Value::Number && node->child(1)->value()->type() == Value::Number) {
		node->value()->setNumber(node->child(0)->value()->number() / node->child(1)->value()->number());
	} else {
		if (node->child(0)->value()->type() != Value::Number)
			addError(i18n("You tried to divide a non-number, '%1'", node->child(0)->token()->look()), *node->token(), 0);
		if (node->child(1)->value()->type() != Value::Number)
			addError(i18n("You tried to divide by a non-number, '%1'", node->child(1)->token()->look()), *node->token(), 0);
	}
}
void Executer::executePower(TreeNode* node) {
	qDebug() << "Executer::executePower()";
	if (node->child(0)->value()->type() == Value::Number && node->child(1)->value()->type() == Value::Number) {
		node->value()->setNumber(pow(node->child(0)->value()->number(), node->child(1)->value()->number()));
	} else {
		if (node->child(0)->value()->type() != Value::Number)
			addError(i18n("You tried to raise a non-number to a power, '%1'", node->child(0)->token()->look()), *node->token(), 0);
		if (node->child(1)->value()->type() != Value::Number)
			addError(i18n("You tried to raise the power of a non-number, '%1'", node->child(1)->token()->look()), *node->token(), 0);
	}
}
void Executer::executeAssign(TreeNode* node) {
	qDebug() << "Executer::executeAssign()";
	if (!functionStack.isEmpty() && functionStack.top().variableTable->contains(node->token()->look())) {
		qDebug() << "exists locally";
		functionStack.top().variableTable->insert(node->child(0)->token()->look(), node->child(1)->value());
		return;
	}
	// insterts unless already exists then replaces
	globalVariableTable.insert(node->child(0)->token()->look(), node->child(1)->value());
}
void Executer::executeLearn(TreeNode* node) {
	qDebug() << "Executer::executeLearn()";
	functionTable.insert(node->child(0)->token()->look(), node);
}
void Executer::executeArgumentList(TreeNode* node) {
	qDebug() << "Executer::executeArgumentList()";
	node = node; // stop the warnings // this is a stud
}
void Executer::executeReset(TreeNode* node) {
	qDebug() << "Executer::executeReset()";
	if (!checkParameterQuantity(node, 0, 20000+Token::Reset*100+90)) return;
	emit reset();
}
void Executer::executeClear(TreeNode* node) {
	qDebug() << "Executer::executeClear()";
	if (!checkParameterQuantity(node, 0, 20000+Token::Clear*100+90)) return;
	emit clear();
}
void Executer::executeCenter(TreeNode* node) {
	qDebug() << "Executer::executeCenter()";
	if (!checkParameterQuantity(node, 0, 20000+Token::Center*100+90)) return;
	emit center();
}
void Executer::executeGo(TreeNode* node) {
	qDebug() << "Executer::executeGo()";
	if (!checkParameterQuantity(node, 2, 20000+Token::Go*100+90) ||
		!checkParameterType(node, Value::Number, 20000+Token::Go*100+91)) return;
	emit go(node->child(0)->value()->number(), node->child(1)->value()->number());
}
void Executer::executeGoX(TreeNode* node) {
	qDebug() << "Executer::executeGoX()";
	if (!checkParameterQuantity(node, 1, 20000+Token::GoX*100+90) ||
		!checkParameterType(node, Value::Number, 20000+Token::GoX*100+91)) return;
	emit goX(node->child(0)->value()->number());
}
void Executer::executeGoY(TreeNode* node) {
	qDebug() << "Executer::executeGoY()";
	if (!checkParameterQuantity(node, 1, 20000+Token::GoY*100+90) ||
		!checkParameterType(node, Value::Number, 20000+Token::GoY*100+91)) return;
	emit goY(node->child(0)->value()->number());
}
void Executer::executeForward(TreeNode* node) {
	qDebug() << "Executer::executeForward()";
	if (!checkParameterQuantity(node, 1, 20000+Token::Forward*100+90) ||
		!checkParameterType(node, Value::Number, 20000+Token::Forward*100+91)) return;
	emit forward(node->child(0)->value()->number());
}
void Executer::executeBackward(TreeNode* node) {
	qDebug() << "Executer::executeBackward()";
	if (!checkParameterQuantity(node, 1, 20000+Token::Backward*100+90) ||
		!checkParameterType(node, Value::Number, 20000+Token::Backward*100+91)) return;
	emit backward(node->child(0)->value()->number());
}
void Executer::executeDirection(TreeNode* node) {
	qDebug() << "Executer::executeDirection()";
	if (!checkParameterQuantity(node, 1, 20000+Token::Direction*100+90) ||
		!checkParameterType(node, Value::Number, 20000+Token::Direction*100+91)) return;
	emit direction(node->child(0)->value()->number());
}
void Executer::executeTurnLeft(TreeNode* node) {
	qDebug() << "Executer::executeTurnLeft()";
	if (!checkParameterQuantity(node, 1, 20000+Token::TurnLeft*100+90) ||
		!checkParameterType(node, Value::Number, 20000+Token::TurnLeft*100+91)) return;
	emit turnLeft(node->child(0)->value()->number());
}
void Executer::executeTurnRight(TreeNode* node) {
	qDebug() << "Executer::executeTurnRight()";
	if (!checkParameterQuantity(node, 1, 20000+Token::TurnRight*100+90) ||
		!checkParameterType(node, Value::Number, 20000+Token::TurnRight*100+91)) return;
	emit turnRight(node->child(0)->value()->number());
}
void Executer::executePenWidth(TreeNode* node) {
	qDebug() << "Executer::executePenWidth()";
	if (!checkParameterQuantity(node, 1, 20000+Token::PenWidth*100+90) ||
		!checkParameterType(node, Value::Number, 20000+Token::PenWidth*100+91)) return;
	emit penWidth(node->child(0)->value()->number());
}
void Executer::executePenUp(TreeNode* node) {
	qDebug() << "Executer::executePenUp()";
	if (!checkParameterQuantity(node, 0, 20000+Token::PenUp*100+90)) return;
	emit penUp();
}
void Executer::executePenDown(TreeNode* node) {
	qDebug() << "Executer::executePenDown()";
	if (!checkParameterQuantity(node, 0, 20000+Token::PenDown*100+90)) return;
	emit penDown();
}
void Executer::executePenColor(TreeNode* node) {
	qDebug() << "Executer::executePenColor()";
	if (!checkParameterQuantity(node, 3, 20000+Token::PenColor*100+90) ||
		!checkParameterType(node, Value::Number, 20000+Token::PenColor*100+91)) return;
	emit penColor(node->child(0)->value()->number(), node->child(1)->value()->number(), node->child(2)->value()->number());
}
void Executer::executeCanvasColor(TreeNode* node) {
	qDebug() << "Executer::executeCanvasColor()";
	if (!checkParameterQuantity(node, 3, 20000+Token::CanvasColor*100+90) ||
		!checkParameterType(node, Value::Number, 20000+Token::CanvasColor*100+91)) return;
	emit canvasColor(node->child(0)->value()->number(), node->child(1)->value()->number(), node->child(2)->value()->number());
}
void Executer::executeCanvasSize(TreeNode* node) {
	qDebug() << "Executer::executeCanvasSize()";
	if (!checkParameterQuantity(node, 2, 20000+Token::CanvasSize*100+90) ||
		!checkParameterType(node, Value::Number, 20000+Token::CanvasSize*100+91)) return;
	emit canvasSize(node->child(0)->value()->number(), node->child(1)->value()->number());
}
void Executer::executeSpriteShow(TreeNode* node) {
	qDebug() << "Executer::executeSpriteShow()";
	if (!checkParameterQuantity(node, 0, 20000+Token::SpriteShow*100+90)) return;
	emit spriteShow();
}
void Executer::executeSpriteHide(TreeNode* node) {
	qDebug() << "Executer::executeSpriteHide()";
	if (!checkParameterQuantity(node, 0, 20000+Token::SpriteHide*100+90)) return;
	emit spriteHide();
}
void Executer::executePrint(TreeNode* node) {
	qDebug() << "Executer::executePrint()";
	if (!checkParameterQuantity(node, 1, 20000+Token::Print*100+90))
		return;
	qDebug() << "Printing: '" << node->child(0)->value()->string() << "'";
	emit print(node->child(0)->value()->string());
}
void Executer::executeFontSize(TreeNode* node) {
	qDebug() << "Executer::executeFontSize()";
	if (!checkParameterQuantity(node, 1, 20000+Token::FontSize*100+90) ||
		!checkParameterType(node, Value::Number, 20000+Token::FontSize*100+91)) return;
	emit fontSize(node->child(0)->value()->number());
}
void Executer::executeRandom(TreeNode* node) {
	qDebug() << "Executer::executeRandom()";
	if (!checkParameterQuantity(node, 2, 20000+Token::Random*100+90) ||
		!checkParameterType(node, Value::Number, 20000+Token::Random*100+91)) return;
}

//END GENERATED executer_cpp CODE


#include "executer.moc"