#pragma once
typedef float num;
typedef num (*op)(num, num);
typedef unsigned int uint;
typedef unsigned char uchar;

enum Iden {I_lit, I_add, I_sub, I_mul, I_div, I_index};
