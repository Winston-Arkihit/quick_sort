#pragma once
#include <stddef.h>

typedef int (*QSORT_COMPARE_FUNCTION)(const void *first_element,
                                      const void *second_element);

void qsort(void *data,
           size_t number_of_elements,
           size_t size_of_element,
           QSORT_COMPARE_FUNCTION compare);
