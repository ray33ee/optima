#include "parseformula.h"

namespace parseFormula
{
    /* List of functions */
    static const char* functions[] = { "log", "neg", "conj", "sqrt", "ln", "exp", "sinh", "cosh", "tanh", "sin", "cos", "tan", "asinh", "acosh", "atanh", "asin", "acos", "atan", "inv", "mod", "arg" };

    /* Number of non-operator functions */
    static const int size = 21;

    /* Exception thrown when a left bracket is missing */
    class MissingLeftBracket : public std::exception
    {
        virtual const char* what() const throw() { return "ParseFormula: Missing left bracket in formula"; }
    } MLB;

    /* Exception thrown when a right bracket is missing */
    class MissingRightBracket : public std::exception
    {
        virtual const char* what() const throw() { return "ParseFormula: Missing right bracket in formula"; }
    } MRB;

    /* Exception thrown when unknown token is found */
    class UnknownTokenType : public std::exception
    {
        virtual const char* what() const throw() { return "ParseFormula: Invalid sequence found in formula"; }
    } UTT;

    /* Exception thrown when operator is not used properly */
    class OperatorAbuse : public std::exception
    {
        virtual const char* what() const throw() { return "ParseFormula: Operator not used properly"; }
    } OA;

    inline bool isNum(const QString &str)
    {
        bool res;
        str.toDouble(&res);
        return res;
    }

    inline bool isOp(const QString &token)
    {
        return token == "+" || token == "-" || token == "*" || token == "/" || token == "^" || token == "neg";
    }

    inline bool isFunction(const QString &token)
    {
        for (auto func : functions)
            if (token == func)
                return true;
        return false;
    }

    inline int getPrecedence(const QString &token)
    {
        if (token == "+" || token == "-")
            return 1;
        else if (token == "*" || token == "/")
            return 2;
        else if (token == "neg")
            return 3;
        else //if (token == "^")
            return 4;
    }

    inline double getIndex(const QString &token)
    {
        if (isOp(token) && token != "neg")
        {
            if (token == "+")
                return 0.0;
            else if (token == "-")
                return 1.0;
            else if (token == "*")
                return 2.0;
            else if (token == "/")
                return 3.0;
            else if (token == "^")
                return 4.0;
        }
        else
        {
            for (int i = 0; i < size; ++i)
                if (functions[i] == token)
                    return i + 5.0;
        }
        return -1.0;
    }

    inline bool isUnaryNegative(const QVector<Token> & outputQueue, const QStack<QString> &opStack, const QString &prevToken)
    {
        return (outputQueue.count() == 0 && opStack.count() == 0) || prevToken == "(" || prevToken  == "neg" || isOp(prevToken);
    }

    void sendToken(const QString & token, QVector<Token> & outputQueue, QStack<QString> &opStack, QString &prev)
    {
        if (!token.isEmpty())
        {
            if (token == "z")
                outputQueue.append(Token{1,{0.0,0.0}});
            else if (isNum(token))
                outputQueue.append(Token{3,{token.toDouble(),0.0}});
            else if (token == "i")
                outputQueue.append(Token{3,{0.0,1.0}});
            else if (token == "pi")
                outputQueue.append(Token{3,{M_PI,0.0}});
            else if (token == "e")
                outputQueue.append(Token{3,{M_E,0.0}});
            else if (isFunction(token)) //Non-operator function
            {
                opStack.push(token);
            }
            else if (token == ",") //Argument seperator
            {
                while (opStack.count() > 0)
                {
                    if (opStack.top() == "(")
                        break;
                    outputQueue.append(Token{2, {getIndex(opStack.pop()), 0.0}}); //
                }
            }
            else if (isOp(token)) //Operator
            {
                if (token == "-" && isUnaryNegative(outputQueue, opStack, prev))
                {
                    opStack.append("neg");
                }
                else
                {
                    while (!opStack.isEmpty())
                    {
                        if ((isOp(opStack.top()) && getPrecedence(opStack.top()) > getPrecedence(token)) || (isOp(opStack.top()) && getPrecedence(opStack.top()) == getPrecedence(token) && token != "^" && opStack.top() != "("))
                            outputQueue.append(Token{2, {getIndex(opStack.pop()), 0.0}});
                        else
                            break;
                    }
                    opStack.append(token);
                }
            }
            else if (token == "(" || token == "{" || token == "[") //Left bracket
            {
                opStack.append("(");
            }
            else if (token == ")" || token == "}" || token == "]") //Right bracket
            {
                while (!opStack.isEmpty())
                {
                    if (opStack.top() != "(")
                        outputQueue.append(Token{2, {getIndex(opStack.pop()), 0.0}});
                    else
                        break;
                }

                if (opStack.isEmpty()) //If there is no matching parenthesis, error
                    throw MLB;

                opStack.pop();

                if (opStack.isEmpty()) //There are no more items on the stack, c'est fini
                    return;
                if (isFunction(opStack.top()) || isOp(opStack.top()))
                    outputQueue.append(Token{2, {getIndex(opStack.pop()), 0.0}});
            }
            else
                throw UTT;

            prev = token;
        }
    }

    bool verify_formula(const QVector<Token> &f)
    {
        int pretend_stack_size = 0;
        for (auto it = f.begin(); it != f.end(); ++it)
        {
            if (it->type == 1 || it->type == 3) //If its z or constant we push (so add 1)
                ++pretend_stack_size;
            else if (it->type == 2 && it->data.real() < 6) //If its binary operator we pop 2 and push 1 (so decrement 1)
                --pretend_stack_size;
        }
        return (pretend_stack_size == 1);
    }

    QVector<Token> processString(QString formula, bool isExpression)
    {
        QVector<Token> outputQueue;
        QStack<QString> opStack;
        QString buff = "";
        QString prev = "";

        //Remove whitespace
        formula.remove(QRegExp("[ \n\t]"));

        for (auto it = formula.constBegin(); it != formula.constEnd(); ++it)
        {
            if (it->isLetter() || it->isNumber() || *it == '.' || (*it == 'e' && (it-1)->isNumber()) || ((*it == '+' || *it == '-') && *(it-1) == 'e')) // ||
            {
                buff += *it;
            }
            else
            {
                sendToken(buff, outputQueue, opStack, prev);
                sendToken(*it, outputQueue, opStack, prev);
                buff = "";
            }
        }
        sendToken(buff, outputQueue, opStack, prev);

        while (!opStack.isEmpty())
        {
            auto item = opStack.pop();
            if (item != "(")
                outputQueue.append(Token{2, {getIndex(item), 0.0}});
            else
                throw MRB;
        }

        /*qDebug() << "New List:";
        for (auto it = outputQueue.begin(); it != outputQueue.end(); ++it)
            qDebug() << "Token: " << it->type << it->data.real() << it->data.imag();*/

        if (!verify_formula(outputQueue))
            throw OA;

        return outputQueue;
    }
}
