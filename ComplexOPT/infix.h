#ifndef INFIX_H
#define INFIX_H

#include "prefix.h"

class infix : public prefix
{
protected:
    QStack<QString> itsOperatorStack;

    QChar XORSign(const QString &formula, const QChar &sign)
    {
        bool chrExist, chrNew;
        chrExist = formula.indexOf('-') != -1; //If the character is negative, the value is 1
        chrNew = sign == '-';
        return ((chrExist ^ chrNew) ? '-' : '+');
    }



    void SendToken(QString &buffer)
    {
        PushInfixToken(buffer);

        buffer.clear();
    }

    void RemoveWhitespace(QString &formula)
    {
        for (int cnt = 0; cnt < formula.size(); cnt++)
            if (formula.at(cnt).isSpace())
                formula.remove(cnt, 1);
    }

    bool isDouble(const QString &str)
    {
        bool *ok = new bool;
        str.toDouble(ok);
        return *ok;
    }

    bool isWord(const QString &str)
    {
        bool ans = true;
        for (int cnt = 0; cnt < str.size(); cnt++)
            ans &= str.at(cnt).isLetter();
        return ans;
    }

    int GetPrecedence(const QString &str) const
    {
        if (str == "^") return 4;
        if (str == "neg") return 3;
        if (str == "*" || str == "/") return 2;
        if (str == "+" || str == "-") return 1;
        return 0;
    }
    bool IsRightAccosiative(const QString &str) const { if (str == "^") return true; return false; }

public:
    infix() {  }
    infix(const QString &formula) { SetInfix(formula); }


    void PushInfixToken(const QString &token)
    {
            if (token.isEmpty()) //empty token
            {

            }
            else if (isDouble(token) || token == "z" || token == "e" || token == "i" || token == "pi") //double or constant or variable - Send to output queue
            {
                PushPrefixToken(token);
            }
            else if (isWord(token) || token == "(") //function or left bracket - Push on to operator stack
            {
                itsOperatorStack.push(token);
            }
            else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^") //operand
            {
                while (true)
                {
                    if (itsOperatorStack.size() > 0 &&
                            ((!IsRightAccosiative(itsOperatorStack.top()) && GetPrecedence(token) == GetPrecedence(itsOperatorStack.top()) ) ||
                             GetPrecedence(token) < GetPrecedence(itsOperatorStack.top()) ))
                        PushPrefixToken(itsOperatorStack.pop());
                    else
                        break;

                }
                itsOperatorStack.push(token);
            }
            else if (token == ",") //argument seperator
            {
                while (itsOperatorStack.size() > 0)
                {
                    if (itsOperatorStack.top() == "(")
                        break;
                    PushPrefixToken(itsOperatorStack.pop());
                }
            }
            else if (token == ")")
            {
                while (itsOperatorStack.top() != "(" && !itsOperatorStack.isEmpty())
                    PushPrefixToken(itsOperatorStack.pop());

                if (itsOperatorStack.isEmpty())//If there is no matching parenthesis, error
                {
                    qDebug() << "ERROR: infix::PushInfixToken(QString): Mismatched parenthesis, no matching '('";
                    return;
                }
                itsOperatorStack.pop();
                if (itsOperatorStack.isEmpty())//There are no more items on the stack, c'est fini
                    return;
                if (itsOperations.contains(itsOperatorStack.top()))
                    PushPrefixToken(itsOperatorStack.pop());
            }

    }



    void SetInfix(QString f)
    {
        QString strBuffer;
        QString strLeftover;
        QChar chrByte, chrBottom, chrTop;
        bool blTwoComp = false;

        RemoveWhitespace(f);

        itsOperatorStack.clear();
        itsRPN.clear();

        for (int cnt = 0; cnt < f.size(); cnt++)
        {
            chrByte = f.at(cnt).toLower();
            if (!strBuffer.isEmpty())
            {
                chrTop = strBuffer.at(strBuffer.size() - 1); //
                chrBottom = strBuffer.at(0);
            }
            else
            {
                chrTop = '>';
                chrBottom = '<';
            }


            if (chrByte.isNumber())
            {
                if (!chrTop.isNumber() && chrTop != '.' && !blTwoComp) //If the previous character was not part of a number, push the buffer out
                    SendToken(strBuffer);

                if (blTwoComp) //If the sign bit is set, clear to acknowledge
                {
                    strBuffer.append('-');
                    blTwoComp = false;
                }

                strBuffer.append(chrByte);
            }
            else if (chrByte.isLetter())
            {
                if (blTwoComp)
                {
                    strBuffer.append("neg");
                    SendToken(strBuffer);
                    blTwoComp = false;
                }

                if (!chrTop.isLetter()) //If the previous character is not part of a string of letters, push the buffer out
                    SendToken(strBuffer);



                strBuffer.append(chrByte);
            }
            else if (chrByte == '+' || chrByte == '-')
            {
                if (cnt == 0 || f.at(cnt - 1) == '(' || f.at(cnt - 1) == '*' || f.at(cnt - 1) == '/' || f.at(cnt - 1) == '^' || f.at(cnt - 1) == ',') //If the character before is a , then it's a unary m
                {
                    SendToken(strBuffer);
                    blTwoComp = true;

                }
                else if (f.at(cnt - 1) == '+' || f.at(cnt - 1) == '-')
                {
                    blTwoComp = (XORSign(strBuffer, chrByte) == '-'); //If the result of the xor is a -, set the TwoComp bit
                }
                else
                {
                    SendToken(strBuffer);
                    strBuffer.append(chrByte);
                }
            }
            else if (chrByte == '*' || chrByte == '/' || chrByte == '^' || chrByte == '(' || chrByte == ')' || chrByte == ',')
            {


                SendToken(strBuffer);

                if (blTwoComp && chrByte == '(') // "-("
                {
                    strBuffer.append("neg");
                    SendToken(strBuffer);
                    blTwoComp = false;
                }

                strBuffer.append(chrByte);
            }
            else if (chrByte == '.')
            {
                if (strBuffer.indexOf('.') == -1 && isDouble(strBuffer))
                {
                    strBuffer.append(chrByte);
                }
                else
                {
                    //decimal point in the wrong place
                }
            }
            else
            {
                qDebug() << "ERROR: infix::setInfix(const QString &): Invalid character" << chrByte;
            }

            if (cnt == f.size() - 1)
                SendToken(strBuffer);
        }


        while (!itsOperatorStack.isEmpty())
            PushPrefixToken(itsOperatorStack.pop());


    }



    /*static const Complex<double> & CalculateExpression(QString formula)
    {
        infix i;
        i.SetDefault();
        i.SetInfix(formula);
        return i.Calculate(Complex<double> (0,0));
    }*/


};




#endif // INFIX_H
