#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "..\Headers\sort_functions.h"

#ifdef NDEBUG
    #define my_assert(equation)
#else
    #define my_assert(equation) if(equation){printf("Assert failed: %s\n", #equation); abort();}
#endif

static void swap_elements(void *first_element,
                          void *second_element,
                          size_t size_of_element);

static void quick_sort_recursive(char *data,
                                 long left,
                                 long right,
                                 size_t size_of_element,
                                 COMPARE_FUNCTION compare);

static int compare_letters(int first_letter, int second_letter);

void my_quick_sort(void *data,
                   size_t number_of_elements,
                   size_t size_of_element,
                   COMPARE_FUNCTION compare) {
    assert(size_of_element > 0);

    if (number_of_elements < 2) {
        return;
    }

    my_assert(data == NULL);
    my_assert(compare == NULL);

    quick_sort_recursive((char *) data,
                         0,
                         (long) number_of_elements - 1,
                         size_of_element,
                         compare);
}

static void quick_sort_recursive(char *data,
                                 long left,
                                 long right,
                                 size_t size_of_element,
                                 COMPARE_FUNCTION compare) {
    my_assert(data == NULL);
    my_assert(compare == NULL);
    assert(left >= 0);
    assert(right >= left);
    assert(size_of_element > 0);

    long left_index = left;
    long right_index = right;
    char *pivot = data + ((left + right) / 2) * (long) size_of_element;

    while (left_index <= right_index) {
        while (compare(data + left_index * (long) size_of_element, pivot) < 0) {
            left_index++;
        }

        while (compare(data + right_index * (long) size_of_element, pivot) > 0) {
            right_index--;
        }

        if (left_index <= right_index) {
            char *left_element = data + left_index * (long) size_of_element;
            char *right_element = data + right_index * (long) size_of_element;

            if (left_element == pivot) {
                pivot = right_element;
            }
            else if (right_element == pivot) {
                pivot = left_element;
            }

            swap_elements(left_element, right_element, size_of_element);
            left_index++;
            right_index--;
        }
    }

    if (left < right_index) {
        quick_sort_recursive(data, left, right_index, size_of_element, compare);
    }

    if (left_index < right) {
        quick_sort_recursive(data, left_index, right, size_of_element, compare);
    }
}

static void swap_elements(void *first_element,
                          void *second_element,
                          size_t size_of_element) {
    my_assert(first_element == NULL);
    my_assert(second_element == NULL);
    assert(size_of_element > 0);

    char *first_byte = (char *) first_element;
    char *second_byte = (char *) second_element;

    for (size_t byte_index = 0; byte_index < size_of_element; byte_index++) {
        char temporary_byte = first_byte[byte_index];
        first_byte[byte_index] = second_byte[byte_index];
        second_byte[byte_index] = temporary_byte;
    }
}

int compare_ints(const void *first_element, const void *second_element) {
    my_assert(first_element == NULL);
    my_assert(second_element == NULL);

    int first_number = *(const int *) first_element;
    int second_number = *(const int *) second_element;

    return (first_number > second_number) - (first_number < second_number);
}

int compare_strings_from_begin(const void *first_element, const void *second_element) {
    my_assert(first_element == NULL);
    my_assert(second_element == NULL);

    const ONE_TEXT_LINE first_line = *(const ONE_TEXT_LINE *) first_element;
    const ONE_TEXT_LINE second_line = *(const ONE_TEXT_LINE *) second_element;
    const char *first_string = first_line.line_start;
    const char *second_string = second_line.line_start;
    my_assert(first_string == NULL);
    my_assert(second_string == NULL);

    size_t first_index = 0;
    size_t second_index = 0;

    while (first_string[first_index] != '\0' || second_string[second_index] != '\0') {
        while (first_string[first_index] != '\0' &&
               !isalpha((unsigned char) first_string[first_index])) {
            first_index++;
        }

        while (second_string[second_index] != '\0' &&
               !isalpha((unsigned char) second_string[second_index])) {
            second_index++;
        }

        if (first_string[first_index] == '\0' || second_string[second_index] == '\0') {
            break;
        }

        int comparison_result = compare_letters(first_string[first_index],
                                                second_string[second_index]);
        if (comparison_result != 0) {
            return comparison_result;
        }

        first_index++;
        second_index++;
    }

    return compare_letters(first_string[first_index], second_string[second_index]);
}

int compare_strings_from_end(const void *first_element, const void *second_element) {
    my_assert(first_element == NULL);
    my_assert(second_element == NULL);

    const ONE_TEXT_LINE first_line = *(const ONE_TEXT_LINE *) first_element;
    const ONE_TEXT_LINE second_line = *(const ONE_TEXT_LINE *) second_element;
    const char *first_string = first_line.line_start;
    const char *second_string = second_line.line_start;
    my_assert(first_string == NULL);
    my_assert(second_string == NULL);

    long first_index = (long) first_line.line_length - 1;
    long second_index = (long) second_line.line_length - 1;

    while (first_index >= 0 || second_index >= 0) {
        while (first_index >= 0 &&
               !isalpha((unsigned char) first_string[first_index])) {
            first_index--;
        }

        while (second_index >= 0 &&
               !isalpha((unsigned char) second_string[second_index])) {
            second_index--;
        }

        if (first_index < 0 || second_index < 0) {
            break;
        }

        int comparison_result = compare_letters(first_string[first_index],
                                                second_string[second_index]);
        if (comparison_result != 0) {
            return comparison_result;
        }

        first_index--;
        second_index--;
    }

    if (first_index < 0 && second_index < 0) {
        return 0;
    }

    return (first_index >= 0) - (second_index >= 0);
}

static int compare_letters(int first_letter, int second_letter) {
    int first_lower_letter = tolower((unsigned char) first_letter);
    int second_lower_letter = tolower((unsigned char) second_letter);

    return (first_lower_letter > second_lower_letter) -
           (first_lower_letter < second_lower_letter);
}
