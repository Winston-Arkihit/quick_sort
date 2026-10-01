#pragma once
#include <stdio.h>

/**
 * @brief Removes quotes from a file path.
 * @param line String with the path.
 */
void remove_quotes(char * line);
/**
 * @brief Removes the newline at the end of a string.
 * @param string String to change.
 */
void remove_newline(char string[]);
/** @brief Clears the rest of the current input line. */
void clear_input(void);
/** @brief Shows an error and closes the program. */
void program_crash(void);

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define RESET   "\033[0m"
