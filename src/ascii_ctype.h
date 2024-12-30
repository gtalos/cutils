/*
 * Copyright (c) 2024 The cutils authors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ASCII_CTYPE_H
#define ASCII_CTYPE_H

int is_ascii(int c);
int is_space(int c);
int is_digit(int c);
int is_alpha(int c);
int is_alnum(int c);
int is_print(int c);
int is_lower(int c);
int is_upper(int c);
int to_lower(int c);
int to_upper(int c);
int is_cntrl(int c);
int is_punct(int c);
int is_xdigit(int c);

#endif /* ASCII_CTYPE_H */