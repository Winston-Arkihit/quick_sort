#include <assert.h>
#include "qsort.h"

static void swap_elements(void *first_element,
                          void *second_element,
                          size_t size_of_element);

static void qsort_recursive(char *data,
                            long left,
                            long right,
                            size_t size_of_element,
                            QSORT_COMPARE_FUNCTION compare);

void qsort(void *data,
           size_t number_of_elements,
           size_t size_of_element,
           QSORT_COMPARE_FUNCTION compare) {
    assert(size_of_element > 0);

    if (number_of_elements < 2) {
        return;
    }

    assert(data != NULL);
    assert(compare != NULL);

    qsort_recursive((char *) data,
                    0,
                    (long) number_of_elements - 1,
                    size_of_element,
                    compare);
}

static void qsort_recursive(char *data,
                            long left,
                            long right,
                            size_t size_of_element,
                            QSORT_COMPARE_FUNCTION compare) {
    assert(data != NULL);
    assert(compare != NULL);
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
        qsort_recursive(data, left, right_index, size_of_element, compare);
    }

    if (left_index < right) {
        qsort_recursive(data, left_index, right, size_of_element, compare);
    }
}

static void swap_elements(void *first_element,
                          void *second_element,
                          size_t size_of_element) {
    assert(first_element != NULL);
    assert(second_element != NULL);
    assert(size_of_element > 0);

    char *first_byte = (char *) first_element;
    char *second_byte = (char *) second_element;

    for (size_t byte_index = 0; byte_index < size_of_element; byte_index++) {
        char temporary_byte = first_byte[byte_index];
        first_byte[byte_index] = second_byte[byte_index];
        second_byte[byte_index] = temporary_byte;
    }
}
