#ifndef LIB_ERROR_H_
#define LIB_ERROR_H_

#include "sound.h"
#include "uart.h"

#define error(m)           { printf(m);          while (1) play_error(); }
#define error1(m, a)       { printf(m, a);       while (1) play_error(); }
#define error2(m, a, b)    { printf(m, a, b);    while (1) play_error(); }
#define error3(m, a, b, c) { printf(m, a, b, c); while (1) play_error(); }

#endif  // LIB_ERROR_H_
