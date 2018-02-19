#include "complex.h"

QString complex_to_string(Complex _c)
{
    if (_c.imag() == 0.0) //If the number is real
        return QString::number(_c.real(), 'g', 10);

    if (_c.real() == 0.0) //If the number is exclusively imaginary
        return QString::number(_c.imag(), 'g', 10) + "*i";

    return QString::number(_c.real(), 'g', 10) + (_c.imag() < 0 ? " - " : " + ") + QString::number(fabs(_c.imag()), 'g', 10) + "*i";
}
