#ifndef COMPLEX_H
#define COMPLEX_H

#include <QString>


template <class T>
class Complex
{
public:
    T real;
    T imag;
    Complex() {}
    Complex(T r, T i) : real(r), imag(i) {}

    Complex operator+ (const Complex &other) const
    {
        return Complex(real + other.real, imag + other.imag);
    }
    Complex operator- (const Complex &other) const
    {
        return Complex(real - other.real, imag - other.imag);
    }

    Complex operator* (const T &other) const
    {
        return Complex(real * other, imag * other);
    }

    operator const QString()
    {
        if (real == imag == 0)
        {
            return "0";
        }
        else if (imag == 0)
        {
            return QString::number(real);
        }
        else if (real == 0)
        {
            return QString::number(imag) + "i";
        }
        else
        {
            return QString::number(real) + (imag < 0 ? " - " : " + ") + QString::number(imag);
        }
    }

};

struct Token { int type; Complex<double> data; };

struct TokenList {  Token* formula; int count; };


#endif // COMPLEX_H
