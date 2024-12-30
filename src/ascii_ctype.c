/*
 * Copyright (c) 2024 The cutils authors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ascii_ctype.h"

#define SPACE          0x01
#define DIGIT          0x02
#define ALPHA          0x04
#define GLOB_SPECIAL   0x08
#define REGEX_SPECIAL  0x10
#define PATHSPEC_MAGIC 0x20
#define CNTRL          0x40
#define PUNCT          0x80

#define S              SPACE
#define A              ALPHA
#define D              DIGIT
#define G              GLOB_SPECIAL   /* *, ?, [, \\ */
#define R              REGEX_SPECIAL  /* $, (, ), +, ., ^, {, | */
#define P              PATHSPEC_MAGIC /* other non-alnum, except for ] and } */
#define X              CNTRL
#define U              PUNCT
#define Z              CNTRL | SPACE


static const unsigned char char_table[256] = {
    X, X, X, X, X, X, X, X, X, Z, Z, X, X, Z, X, X,	 /*   0.. 15 */
    X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,	 /*  16.. 31 */
    S, P, P, P, R, P, P, P, R, R, G, R, P, P, R, P,	 /*  32.. 47 */
    D, D, D, D, D, D, D, D, D, D, P, P, P, P, P, G,	 /*  48.. 63 */
    P, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A,	 /*  64.. 79 */
    A, A, A, A, A, A, A, A, A, A, A, G, G, U, R, P,	 /*  80.. 95 */
    P, A, A, A, A, A, A, A, A, A, A, A, A, A, A, A,	 /*  96..111 */
    A, A, A, A, A, A, A, A, A, A, A, R, R, U, P, X,	 /* 112..127 */
	/* Nothing in the 128.. range */
};

static const unsigned char hex_table[256] = {
    [0x30] = 1, [0x31] = 1,
    [0x32] = 1, [0x33] = 1,
    [0x34] = 1, [0x35] = 1,
    [0x36] = 1, [0x37] = 1,
    [0x38] = 1, [0x39] = 1,
    [0x41] = 1, [0x42] = 1,
    [0x43] = 1, [0x44] = 1,
    [0x45] = 1, [0x46] = 1,
    [0x61] = 1, [0x62] = 1,
    [0x63] = 1, [0x64] = 1,
    [0x65] = 1, [0x66] = 1
};


static inline int is_test(int c, int mask);
static inline int is_case(int c, int is_lower);
static inline int case_transform(int c, int high);


int is_ascii(int c)
{
    return (c & ~0x7f) == 0;
}

int is_space(int c)
{
    return is_test(c, SPACE);
}

int is_digit(int c)
{
    return is_test(c, DIGIT);
}

int is_alpha(int c)
{
    return is_test(c, ALPHA);
}

int is_alnum(int c)
{
    return is_test(c, ALPHA | DIGIT);
}

int is_print(int c)
{
    return c >= 0x20 && c <= 0x7e;
}

int is_lower(int c)
{
    return is_case(c, 1);
}

int is_upper(int c)
{
    return is_case(c, 0);
}

int is_cntrl(int c)
{
    return is_test(c, CNTRL);
}

int is_punct(int c)
{
    return is_test(c, PUNCT | REGEX_SPECIAL | GLOB_SPECIAL | PATHSPEC_MAGIC);
}

int is_xdigit(int c)
{
    return hex_table[(unsigned char) c] != 0;
}

int is_glob_special(int c)
{
    return is_test(c, GLOB_SPECIAL);
}

int is_regex_special(int c)
{
    return is_test(c, GLOB_SPECIAL | REGEX_SPECIAL);
}

int is_pathspec_magic(int c)
{
    return is_test(c, PATHSPEC_MAGIC);
}

int to_lower(int c)
{
    return case_transform((unsigned char) c, 0x20);
}

int to_upper(int c)
{
    return case_transform((unsigned char) c, 0);
}

static inline int is_test(int c, int mask)
{
    return (char_table[(unsigned char) c] & mask) != 0;
}

static inline int is_case(int c, int is_lower)
{
    if (is_test(c, ALPHA) == 0) {
        return 0;
    }
    
    return ((c & 0x20) != 0) == is_lower;
}

static inline int case_transform(int c, int high)
{
    if (is_test(c, ALPHA)) {
        c = (c & ~0x20) | high;
    }
    
    return c;
}
