/*
 * Copyright (c) 2024
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ASCII_CTYPE_H
#define ASCII_CTYPE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Checks if a character is within the ASCII range.
 * @param c The character to check.
 * @return Non-zero if the character is ASCII, 0 otherwise.
 */
int is_ascii(int c);

/**
 * @brief Checks if a character is a whitespace character.
 * @param c The character to check.
 * @return Non-zero if the character is a whitespace, 0 otherwise.
 */
int is_space(int c);

/**
 * @brief Checks if a character is a decimal digit.
 * @param c The character to check.
 * @return Non-zero if the character is a digit, 0 otherwise.
 */
int is_digit(int c);

/**
 * @brief Checks if a character is an alphabetic letter.
 * @param c The character to check.
 * @return Non-zero if the character is alphabetic, 0 otherwise.
 */
int is_alpha(int c);

/**
 * @brief Checks if a character is alphanumeric (letter or digit).
 * @param c The character to check.
 * @return Non-zero if the character is alphanumeric, 0 otherwise.
 */
int is_alnum(int c);

/**
 * @brief Checks if a character is printable.
 * @param c The character to check.
 * @return Non-zero if the character is printable, 0 otherwise.
 */
int is_print(int c);

/**
 * @brief Checks if a character is lowercase.
 * @param c The character to check.
 * @return Non-zero if the character is lowercase, 0 otherwise.
 */
int is_lower(int c);

/**
 * @brief Checks if a character is uppercase.
 * @param c The character to check.
 * @return Non-zero if the character is uppercase, 0 otherwise.
 */
int is_upper(int c);

/**
 * @brief Checks if a character is a control character.
 * @param c The character to check.
 * @return Non-zero if the character is a control character, 0 otherwise.
 */
int is_cntrl(int c);

/**
 * @brief Checks if a character is punctuation.
 * @param c The character to check.
 * @return Non-zero if the character is punctuation, 0 otherwise.
 */
int is_punct(int c);

/**
 * @brief Checks if a character is a hexadecimal digit.
 * @param c The character to check.
 * @return Non-zero if the character is a hexadecimal digit, 0 otherwise.
 */
int is_xdigit(int c);

/**
 * @brief Checks if a character is a glob special character.
 * @param c The character to check.
 * @return Non-zero if the character is a glob special character, 0 otherwise.
 */
int is_glob_special(int c);

/**
 * @brief Checks if a character is a regex special character.
 * @param c The character to check.
 * @return Non-zero if the character is a regex special character, 0 otherwise.
 */
int is_regex_special(int c);

/**
 * @brief Checks if a character is a pathspec magic character.
 * @param c The character to check.
 * @return Non-zero if the character is a pathspec magic character, 0 otherwise.
 */
int is_pathspec_magic(int c);

/**
 * @brief Converts a character to lowercase.
 * @param c The character to convert.
 * @return The lowercase equivalent of the character, or the character itself 
 *         if not alphabetic.
 */
int to_lower(int c);

/**
 * @brief Converts a character to uppercase.
 * @param c The character to convert.
 * @return The uppercase equivalent of the character, or the character itself if not alphabetic.
 */
int to_upper(int c);

#ifdef __cplusplus
}
#endif

#endif /* ASCII_CTYPE_H */
