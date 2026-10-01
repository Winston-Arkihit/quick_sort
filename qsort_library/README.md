# qsort library

This folder contains a small reusable quick sort implementation:

```text
qsort.h
qsort.c
```

You can copy these two files into another project and use the function:

```c
qsort(array, number_of_elements, size_of_one_element, compare_function);
```

## What each argument means

Example:

```c
int numbers[] = {5, 1, 4, 2, 3};

qsort(numbers, 5, sizeof(numbers[0]), compare_ints);
```

Here:

```c
numbers
```

is the array that must be sorted.

```c
5
```

is the number of elements in the array.

```c
sizeof(numbers[0])
```

is the size of one element. For `int` it is usually 4 bytes.

```c
compare_ints
```

is the function that explains how two elements must be compared.

The sort function itself does not know whether it sorts `int`, `double`, strings
or structs. It only sees memory bytes. That is why it needs both element size and
a compare function.

## Why compare function gets void pointers

The compare function has this form:

```c
int compare_ints(const void *first, const void *second)
```

`void *` means "pointer to something unknown".

Because qsort is universal, it passes pointers to two elements, but it does not
know their real type. Inside the compare function we know the type, so we cast
pointers back to `int *`:

```c
const int *first_int_pointer = (const int *) first;
const int *second_int_pointer = (const int *) second;
```

Then we take values from these addresses:

```c
int a = *first_int_pointer;
int b = *second_int_pointer;
```

Short version:

```c
int a = *(const int *) first;
int b = *(const int *) second;
```

This means:

1. Treat `first` as pointer to `int`.
2. Go to that address.
3. Take the `int` value from there.

## What compare function must return

The compare function must return:

- negative value if the first element must go before the second;
- positive value if the first element must go after the second;
- zero if they are equal.

For ascending integer sort:

```c
int compare_ints(const void *first, const void *second) {
    int a = *(const int *) first;
    int b = *(const int *) second;

    return (a > b) - (a < b);
}
```

This line:

```c
return (a > b) - (a < b);
```

works like this:

If `a > b`:

```text
(a > b) is 1
(a < b) is 0
1 - 0 = 1
```

So the function returns a positive value.

If `a < b`:

```text
(a > b) is 0
(a < b) is 1
0 - 1 = -1
```

So the function returns a negative value.

If `a == b`:

```text
(a > b) is 0
(a < b) is 0
0 - 0 = 0
```

So the function returns zero.

You can also write it longer and more clearly:

```c
int compare_ints(const void *first, const void *second) {
    int a = *(const int *) first;
    int b = *(const int *) second;

    if (a < b) {
        return -1;
    }

    if (a > b) {
        return 1;
    }

    return 0;
}
```

Both versions do the same thing.

## Full minimal example

```c
#include <stdio.h>
#include "qsort.h"

int compare_ints(const void *first, const void *second) {
    int a = *(const int *) first;
    int b = *(const int *) second;

    return (a > b) - (a < b);
}

int main(void) {
    int numbers[] = {5, 1, 4, 2, 3};
    size_t number_of_elements = sizeof(numbers) / sizeof(numbers[0]);

    qsort(numbers,
          number_of_elements,
          sizeof(numbers[0]),
          compare_ints);

    for (size_t index = 0; index < number_of_elements; index++) {
        printf("%d ", numbers[index]);
    }

    return 0;
}
```

Expected output:

```text
1 2 3 4 5
```

## How to compile this example

If `main.c`, `qsort.c`, and `qsort.h` are in one folder:

```bat
g++ main.c qsort.c -o program.exe
```

If `qsort.h` and `qsort.c` are in `qsort_library/`:

```bat
g++ main.c qsort_library\qsort.c -I "qsort_library" -o program.exe
```