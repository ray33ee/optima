#ifndef RPN_H
#define RPN_H

#include <QStack>
#include <QVector>
#include <QString>

#include "temp.h"

class prefix
{
protected:

    QVector<Token> itsRPN;

    /* String lookups */
    QList<QString> itsOperations;

    /* complex values */
    QList< Complex > itsVariable; //operand
    QList<Complex> itsDefined; //constant

    /* user defined functions */
    QList<QString> itsUser;


    int GetType(const QString &token) const;
    bool PushPrefixToken(const QString &token);
    //Complex Evaluate(int type, const QList<Complex> &operands) const;

public:
    prefix() {  }
    prefix(const QList<QString> &operations,
          const QList<Complex > &variable):
        itsOperations(operations),
        itsVariable(variable)
    {}

    /* Set members to default */
    void SetDefault();

    /* Set indervidual members */
    void SetOperations(const QList<QString> &operations) { itsOperations = operations; }

    void SetVariable(const QList<Complex> &variables) { itsVariable = variables; }


    QVector<Token> GetList() { return itsRPN; } //DEBUG


    /* get list */
    const QVector<Token>& GetConstRPN() const { return itsRPN; }

    QVector<Token>& GetRPN() { return itsRPN; }

    /* The following functions allow direct access to their respective members. USE WITH CAUTION */
    QList<QString>& GetOperations() { return itsOperations; }

    QList<Complex >& GetVariable() { return itsVariable; }




    bool SetPrefix(const QString &string);
    bool SetPrefix(const QList<QString> &rpn);
    void SetPrefix(const prefix &data) { *this = data; }

    Complex  Calculate(int *error = 0) const;
    Complex  Calculate(const Complex  &variable, int *error = 0);
    Complex  Calculate(const QList<Complex> &variable, int *error = 0);






};





#endif // RPN_H
