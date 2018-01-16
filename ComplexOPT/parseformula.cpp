#include "parseformula.h"

#include "qdebug.h"

namespace parseFormula
{
    /* Exception thrown when a left bracket is missing */
    class MissingLeftBracket : public std::exception
    {
        virtual const char* what() const throw()
        {
            return "ParseFormula: Missing left bracket in formula";
        }
    } MLB;

    /* Exception thrown when a right bracket is missing */
    class MissingRightBracket : public std::exception
    {
        virtual const char* what() const throw()
        {
            return "ParseFormula: Missing right bracket in formula";
        }
    } MRB;

    /* Exception thrown when an invalid character is found in string */
    class InvalidCharacterDetected : public std::exception
    {
        virtual const char* what() const throw()
        {
            return "ParseFormula: Invalid character found in formula";
        }
    } ICD;


    void removeWhitespace(QString& formula)
    {
        for (int i = 0; i < formula.size(); ++i)
            if (formula[i] == ' ' || formula[i] == '\n' || formula[i] == '\t')
                formula.remove(i, 1);
    }

    bool isValid(const QString &formula)
    {
        return true;
    }

    bool isNum(QString str)
    {
        bool res;
        str.toDouble(&res);
        return res;
    }

    bool isOp(const QString &token)
    {
        return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
    }

    bool isFunction(const QString &token)
    {
        for (auto func : functions)
        {
            if (token == func)
                return true;
        }
        return false;
    }

    int getPrecedence(const QString &token)
    {
        if (token == "+" || token == "-")
            return 1;
        else if (token == "*" || token == "/")
            return 2;
        else if (token == "N")
            return 3;
        else if (token == "^")
            return 4;

    }

    bool isUnaryNegative(QList<QString> & outputQueue, QStack<QString> &opStack, QString &prevToken)
    {
        return outputQueue.count() == 0 && opStack.count() == 0 || prevToken == "(" || prevToken  == "N" || isOp(prevToken);
    }

    void sendToken(const QString & token, QList<QString> & outputQueue, QStack<QString> &opStack, QString &prev)
    {
        if (!token.isEmpty())
        {
            qDebug() << "Token: " << token << isNum(token);

            if (isNum(token) || token == "z" || token == "i" || token == "pi" || token == "e")
            {
                outputQueue.append(token);
            }
            else if (isFunction(token))
            {
                opStack.push(token);
            }
            else if (token == ",")
            {
                while (opStack.count() > 0)
                {
                    if (opStack.top() == "(")
                        break;
                    outputQueue.append(opStack.pop());
                }
            }
            else if (isOp(token))
            {
                if (token == "-" && isUnaryNegative(outputQueue, opStack, prev))
                {
                    opStack.append("N");
                }
                else
                {
                    while (true)
                    {
                        if (opStack.count() > 0)
                        {
                            if (isOp(opStack.top()) && getPrecedence(opStack.top()) > getPrecedence(token) || isOp(opStack.top()) && getPrecedence(opStack.top()) == getPrecedence(token) && token != "^" && opStack.top() != "(")
                            {
                                outputQueue.append(opStack.pop());
                            }
                            else
                            {
                                break;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    opStack.append(token);
                }
            }
            else if (token == "(" || token == "{" || token == "[")
            {
                opStack.append("(");
            }
            else if (token == ")" || token == "}" || token == "]")
            {
                while (true)
                {
                    if (opStack.count() > 0)
                    {
                        if (opStack.top() == "(")
                            break;
                    }
                    else
                    {
                        throw MLB;
                        return;
                    }
                    outputQueue.append(opStack.pop());
                }
                opStack.pop();
            }

        }
        prev = token;
    }

    void processString(QString formula)
    {
        QList<QString> outputQueue;
        QStack<QString> opStack;
        QString buff = "";
        QString prev;

        removeWhitespace(formula);

        if (isValid(formula))
        {
            for (auto& ch : formula)
            {
                if (ch.isLetter() || ch.isNumber() || ch == '.')
                {
                    buff += ch;
                }
                else
                {
                    sendToken(buff, outputQueue, opStack, prev);
                    sendToken(ch, outputQueue, opStack, prev);
                    buff = "";
                }
            }
            sendToken(buff, outputQueue, opStack, prev);

            while (!opStack.isEmpty())
            {
                auto item = opStack.pop();
                if (item != "(")
                    outputQueue.append(item);
                else
                    throw MRB;
            }

            qDebug() << "START";

            for (int i = 0; i < outputQueue.count(); ++i)
                qDebug() << "TOKENS: " << outputQueue[i];
        }
        else
        {
            throw ICD;
        }
    }
}
