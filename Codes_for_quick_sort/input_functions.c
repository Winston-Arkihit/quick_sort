#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "..\Headers\additional_functions.h"
#include "..\Headers\sort_functions.h"

static size_t get_file_size(const char *path);
static char *allocate_read_buffer(size_t file_size, size_t *buffer_size);
static char *copy_string(const char *string, size_t string_length);
static void add_line(TEXT *text,
                     size_t *capacity,
                     const char *line_start,
                     size_t line_length);
static void append_char_to_line(char **line_buffer,
                                size_t *line_length,
                                size_t *line_capacity,
                                char symbol);

void clear_input(void) {
    int ch = 0;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void remove_newline(char string[]) {
    assert(string != NULL);

    for (int i = 0; string[i] != '\0'; i++) {
        if (string[i] == '\n') {
            string[i] = '\0';
            break;
        }
    }
}

void remove_quotes(char *line) {
    assert(line != NULL);

    int write_index = 0;
    for (int current_index = 0; line[current_index] != '\0'; current_index++) {
        if (line[current_index] != '"') {
            line[write_index] = line[current_index];
            write_index++;
        }
    }
    line[write_index] = '\0';
}

void program_crash(void) {
    printf(RED "\nInput error. " RESET "Press any key to exit...\n");
    _getch();
    exit(EXIT_FAILURE);
}

TEXT read_lines_from_file(const char *path) {
    assert(path != NULL);

    const size_t START_CAPACITY = 64;

    size_t file_size = get_file_size(path);
    size_t read_buffer_size = 0;
    char *read_buffer = allocate_read_buffer(file_size, &read_buffer_size);
    if (file_size > 0 && read_buffer_size == file_size) {
        printf("Read buffer size is equal to file size\n");
    }
    else if (file_size > 0) {
        double memory_decrease = (double) file_size / (double) read_buffer_size;
        printf("Read buffer is " YELLOW "%.2lf" RESET " time(s) smaller than file size\n",
               memory_decrease);
    }

    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        free(read_buffer);
        printf(RED "Problem to open file with text\n" RESET);
        program_crash();
    }

    TEXT text = {};
    size_t capacity = START_CAPACITY;
    text.lines = (ONE_TEXT_LINE *) calloc(capacity, sizeof(ONE_TEXT_LINE));
    if (text.lines == NULL) {
        free(read_buffer);
        fclose(file);
        printf(RED "Memory allocation problem\n" RESET);
        program_crash();
    }

    char *line_buffer = NULL;
    size_t line_length = 0;
    size_t line_capacity = 0;

    size_t number_of_read_symbols = 0;
    while ((number_of_read_symbols = fread(read_buffer, sizeof(char), read_buffer_size, file)) > 0) {
        for (size_t symbol_index = 0; symbol_index < number_of_read_symbols; symbol_index++) {
            append_char_to_line(&line_buffer,
                                &line_length,
                                &line_capacity,
                                read_buffer[symbol_index]);

            if (read_buffer[symbol_index] == '\n') {
                add_line(&text, &capacity, line_buffer, line_length);
                line_length = 0;
            }
        }
    }

    if (ferror(file)) {
        free(read_buffer);
        free(line_buffer);
        fclose(file);
        free_text_lines(&text);
        printf(RED "Problem while reading file\n" RESET);
        program_crash();
    }

    if (line_length > 0) {
        add_line(&text, &capacity, line_buffer, line_length);
    }

    free(line_buffer);
    free(read_buffer);
    fclose(file);
    return text;
}

void free_text_lines(TEXT *text) {
    if (text == NULL || text->lines == NULL) {
        return;
    }

    for (size_t line_index = 0; line_index < text->number_of_lines; line_index++) {
        free(text->lines[line_index].line_start);
    }

    free(text->lines);
    text->lines = NULL;
    text->number_of_lines = 0;
}

static size_t get_file_size(const char *path) {
    assert(path != NULL);

    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        printf(RED "Problem to open file for size counting\n" RESET);
        program_crash();
    }

    size_t file_size = 0;
    int symbol = 0;
    while ((symbol = fgetc(file)) != EOF) {
        file_size++;
    }

    if (ferror(file)) {
        fclose(file);
        printf(RED "Problem while counting file size\n" RESET);
        program_crash();
    }

    fclose(file);
    return file_size;
}

static char *allocate_read_buffer(size_t file_size, size_t *buffer_size) {
    assert(buffer_size != NULL);

    size_t current_buffer_size = file_size == 0 ? 1 : file_size;
    char *buffer = NULL;

    while (current_buffer_size > 0) {
        buffer = (char *) calloc(current_buffer_size, sizeof(char));
        if (buffer != NULL) {
            *buffer_size = current_buffer_size;
            return buffer;
        }

        current_buffer_size /= 2;
    }

    printf(RED "Memory allocation problem for read buffer\n" RESET);
    program_crash();
    return NULL;
}

static char *copy_string(const char *string, size_t string_length) {
    assert(string != NULL);

    char *copy = (char *) calloc(string_length + 1, sizeof(char));
    if (copy == NULL) {
        printf(RED "Memory allocation problem while copying string\n" RESET);
        program_crash();
    }

    memcpy(copy, string, string_length);
    copy[string_length] = '\0';
    return copy;
}

static void add_line(TEXT *text,
                     size_t *capacity,
                     const char *line_start,
                     size_t line_length) {
    assert(text != NULL);
    assert(capacity != NULL);
    assert(line_start != NULL);
    assert(text->lines != NULL);

    if (text->number_of_lines == *capacity) {
        *capacity *= 2;
        ONE_TEXT_LINE *new_lines = (ONE_TEXT_LINE *) realloc(text->lines,
                                                             *capacity * sizeof(ONE_TEXT_LINE));
        if (new_lines == NULL) {
            free_text_lines(text);
            printf(RED "Memory reallocation problem\n" RESET);
            program_crash();
        }
        text->lines = new_lines;
    }

    text->lines[text->number_of_lines].line_start = copy_string(line_start, line_length);
    text->lines[text->number_of_lines].line_length = line_length;
    text->number_of_lines++;
}

static void append_char_to_line(char **line_buffer,
                                size_t *line_length,
                                size_t *line_capacity,
                                char symbol) {
    assert(line_buffer != NULL);
    assert(line_length != NULL);
    assert(line_capacity != NULL);

    if (*line_length + 1 >= *line_capacity) {
        size_t new_capacity = *line_capacity == 0 ? 64 : *line_capacity * 2;
        char *new_line_buffer = (char *) realloc(*line_buffer,
                                                 new_capacity * sizeof(char));
        if (new_line_buffer == NULL) {
            free(*line_buffer);
            printf(RED "Memory reallocation problem for line buffer\n" RESET);
            program_crash();
        }

        *line_buffer = new_line_buffer;
        *line_capacity = new_capacity;
    }

    (*line_buffer)[*line_length] = symbol;
    (*line_length)++;
    (*line_buffer)[*line_length] = '\0';
}
