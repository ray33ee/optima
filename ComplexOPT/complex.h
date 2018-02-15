#ifndef COMPLEX_H
#define COMPLEX_H

#include <complex>
#include <QString>

using Complex = std::complex<double>;

QString complex_to_string(Complex);

#endif // COMPLEX_H
