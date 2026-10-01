# Fast sort

My implementation of quick sort for sorting lines from a text file.

The program reads an input `.txt` file and creates three output files near that
input file:

- `<input_name>_sorted_from_begin.txt` - lines sorted from the beginning;
- `<input_name>_sorted_from_end.txt` - lines sorted from the end;
- `<input_name>_original_text.txt` - original text.

While comparing lines, the program skips all non-letter symbols: spaces,
punctuation marks, digits and service symbols. Letter case is ignored.

## Files needed for the program

```text
Headers/
  additional_functions.h
  sort_functions.h

Codes_for_quick_sort/
  Sort.c
  input_functions.c

main_codes/
  IN-OUTput_and_sort.c

txt_file_for_sort/
  onegin_eng.txt  (example input file; you can use any .txt file)

Starter.bat
README.md
```

## Build

Run:

```bat
Starter.bat
```

The script builds:

```text
FastSort.exe
```

Manual build command:

```bat
g++ -Wall -Wextra -I "Headers" main_codes\IN-OUTput_and_sort.c Codes_for_quick_sort\input_functions.c Codes_for_quick_sort\Sort.c -o FastSort.exe
```

## Run

After building:

```bat
FastSort.exe "path\to\input.txt"
```

You can also run it without an argument, then the program will ask for the path
to the input file.

## Main idea

Each text line is stored as:

```c
typedef struct OneTextLineS {
    char *line_start;
    size_t line_length;
} ONE_TEXT_LINE;
```

The program keeps the original text unchanged, copies the line index once,
sorts this copied index from the beginning, writes the first result, then sorts
the same copied index from the end and writes the second result.

Quick sort average complexity: `O(n log n)`.
Worst complexity: `O(n^2)`.
