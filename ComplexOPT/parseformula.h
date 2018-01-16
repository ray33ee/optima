#ifndef PARSEFORMULA_H
#define PARSEFORMULA_H

#include <exception>
#include <QString>

#include <QList>
#include <QString>
#include <QStack>

#include "temp.h"

namespace parseFormula
{
    /* Exception thrown when a left bracket is missing */
    class MissingLeftBracket;

    /* Exception thrown when a right bracket is missing */
    class MissingRightBracket;

    /* Exception thrown when an invalid character is found in string */
    class InvalidCharacterDetected;

    /* List of functions */
    static const char* functions[] = { "log", "neg", "conj", "sqrt", "ln", "exp", "sinh", "cosh", "tanh", "sin", "cos", "tan", "asinh", "acosh", "atanh", "asin", "acos", "atan", "inv", "mod", "arg" };

    void removeWhitespace(QString &);

    bool isValid(const QString &);

    bool isNum(QString);

    bool isFunction(const QString &);

    int getPrecedence(const QString &);

    bool isUnaryNegative(QList<QString> & outputQueue, QStack<QString> &opStack, QString &prevToken);

    void sendToken(const QString &, const QList<QString> & outputQueue, QStack<QString> &opStack, QString &prev);

    void processString(QString);

}

#endif // PARSEFORMULA_H
