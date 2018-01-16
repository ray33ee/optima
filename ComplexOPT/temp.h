#ifndef TEMP
#define TEMP

#include <complex>

using Complex = std::complex<double>;

struct Token { int type; Complex data; };

struct TokenList {  Token* formula; int count; };

#endif // TEMP

