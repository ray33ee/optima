#include "prefix.h"
#include <QCoreApplication>
#include <QDebug>

void prefix::SetDefault()
{
    itsOperations.append("-");
    itsOperations.append("+");
    itsOperations.append("*");
    itsOperations.append("/");
    itsOperations.append("^");
    itsOperations.append("log");

    itsOperations.append("neg");
    itsOperations.append("conj");
    itsOperations.append("sqrt");
    itsOperations.append("ln");
    itsOperations.append("exp");

    itsOperations.append("sinh");
    itsOperations.append("cosh");
    itsOperations.append("tanh");
    itsOperations.append("sin");
    itsOperations.append("cos");
    itsOperations.append("tan");
    itsOperations.append("asinh");
    itsOperations.append("acosh");
    itsOperations.append("atanh");
    itsOperations.append("asin");
    itsOperations.append("acos");
    itsOperations.append("atan");
    itsOperations.append("inv");
    itsOperations.append("arg");
    itsOperations.append("mod");

    itsOperations.append("re");
    itsOperations.append("im");
}

bool prefix::PushPrefixToken(const QString &token)
{
    Token push;
    bool isNumeric;

    if (itsOperations.indexOf(token) != -1)
        push = { 2, Complex(itsOperations.indexOf(token), 0) };

    if (token == "z")
        push = { 1, Complex(0, 0) };

    token.toDouble(&isNumeric);
    if (isNumeric)
        push = { 3, Complex(token.toDouble(), 0) };

    if (token == "e")
        push = { 3, Complex(2.718281828, 0)};
    if (token == "i")
        push = { 3, Complex(0, 1)};
    if (token == "pi")
        push = { 3, Complex(3.141592654, 0)};


    itsRPN.append(push);

    return 1;

}


bool prefix::SetPrefix(const QString &string)
{
    QString strToken;
    bool error = 0;

    for (int cnt = 0; cnt < string.size(); cnt++)
    {
        if (string.at(cnt) == ' ')
        {
            error |= PushPrefixToken(strToken.toLower()); //turn the string to lower case to ensure function is NOT case sensitive
            strToken = "";
        }
        else
        {
            strToken.append(string.at(cnt));
        }

    }
    error |= PushPrefixToken(strToken); //Very last token

    return error;
}

bool prefix::SetPrefix(const QList<QString> &string)
{
    bool error = 0;
    for (int cnt = 0; cnt < string.size(); cnt++)
        error |= PushPrefixToken(string.at(cnt));
    return error;
}










