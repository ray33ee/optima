#ifndef PARSEFORMULA_H
#define PARSEFORMULA_H

#include <exception>
#include <QString>

#include <QList>
#include <QVector>
#include <QString>
#include <QStack>

#include "qdebug.h"
#include <complex>

using Complex = std::complex<double>;

struct Token
{
    int type;
    Complex data;

    Token(int t = 0, Complex c = Complex()):type(t), data(c) {}

};

struct TokenList
{
    Token* formula;
    int count;

    TokenList(Token* f = nullptr, int c = 0): formula(f), count(c) {}

};

namespace parseFormula
{
    /* Exception thrown when a left bracket is missing */
    class MissingLeftBracket;

    /* Exception thrown when a right bracket is missing */
    class MissingRightBracket;

    /* Exception thrown when an invalid character is found in string */
    class InvalidCharacterDetected;

    /* Exception thrown when unknown token is found */
    class UnknownTokenType;

    /* List of functions */
    static const char* functions[] = { "log", "neg", "conj", "sqrt", "ln", "exp", "sinh", "cosh", "tanh", "sin", "cos", "tan", "asinh", "acosh", "atanh", "asin", "acos", "atan", "inv", "mod", "arg" };

    /* Number of non-operator functions */
    static const int size = 21;

    bool isNum(const QString &);

    bool isFunction(const QString &);

    int getPrecedence(const QString &);

    double getIndex(const QString &);

    bool isUnaryNegative(const QList<QString> &outputQueue, const QStack<QString> &opStack, const QString &prevToken);

    void sendToken(const QString &, const QList<QString> & outputQueue, QStack<QString> &opStack, QString &prev);

    QVector<Token> processString(QString);

}

#endif // PARSEFORMULA_H

