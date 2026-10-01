#pragma once
#include <stddef.h>

/** @brief Function type for comparing two elements. */
typedef int (*COMPARE_FUNCTION)(const void *first_element,
                                const void *second_element);

/** @brief One text line with its length. */
typedef struct OneTextLineS {
    char *line_start;
    size_t line_length;
} ONE_TEXT_LINE;

/** @brief Text read from file line by line. */
typedef struct TextS {
    ONE_TEXT_LINE *lines;
    size_t number_of_lines;
} TEXT;

/**
 * @brief Sorts an array with quick sort.
 * @param data Pointer to the beginning of the array.
 * @param number_of_elements Number of elements in the array.
 * @param size_of_element Size of one element in bytes.
 * @param compare Function which compares two elements.
 */
void my_quick_sort(void *data,
                   size_t number_of_elements,
                   size_t size_of_element,
                   COMPARE_FUNCTION compare);

/**
 * @brief Compares two int numbers.
 * @return Negative value if first < second, positive if first > second, 0 otherwise.
 */
int compare_ints(const void *first_element, const void *second_element);

/**
 * @brief Compares two strings from the beginning, skipping non-letter symbols.
 * @return Negative value if first < second, positive if first > second, 0 otherwise.
 */
int compare_strings_from_begin(const void *first_element, const void *second_element);

/**
 * @brief Compares two strings from the end, skipping non-letter symbols.
 * @return Negative value if first < second, positive if first > second, 0 otherwise.
 */
int compare_strings_from_end(const void *first_element, const void *second_element);

/**
 * @brief Reads all lines from a file.
 * @param path Path to the input file.
 * @return Structure with array of lines and its size.
 */
TEXT read_lines_from_file(const char *path);

/**
 * @brief Frees memory used by text lines.
 * @param text Text lines to free.
 */
void free_text_lines(TEXT *text);
