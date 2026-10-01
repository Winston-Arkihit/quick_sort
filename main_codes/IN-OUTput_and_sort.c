#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "additional_functions.h"
#include "sort_functions.h"

enum {
    MAX_PATH_LENGTH = 520,
    INPUT_BUFFER_LENGTH = 260,
};

static ONE_TEXT_LINE *copy_line_index(const TEXT *text);
static int write_lines(FILE *file, const ONE_TEXT_LINE *lines, size_t number_of_lines);
static void make_output_path(const char *input_path,
                             const char *output_suffix,
                             char *output_path,
                             size_t output_path_size);
static int write_text_result(const char *path,
                             const ONE_TEXT_LINE *lines,
                             size_t number_of_lines);

int main(int argc, char *argv[]) {
    char buffer[INPUT_BUFFER_LENGTH] = {0};
    char *path = NULL;

    printf("===================================================\n"
           "|                  QUICK SORT                     |\n"
           "|                  made by WA                     |\n"
           "===================================================\n"
           "\n");

    if (argc < 2) {
        printf("Enter the path to the file with text lines (can be with quotes): ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input.\n");
            program_crash();
        }
        remove_newline(buffer);
        remove_quotes(buffer);
        path = buffer;
    }
    else {
        remove_quotes(argv[1]);
        path = argv[1];
    }

    printf("Running sort from: " RED "%s" RESET "\n", path);
    TEXT text = read_lines_from_file(path);
    printf("Program got " YELLOW "%zu" RESET " line(s)\n", text.number_of_lines);
    
    ONE_TEXT_LINE *sorted_lines = copy_line_index(&text);

    my_quick_sort(sorted_lines,
                  text.number_of_lines,
                  sizeof(sorted_lines[0]),
                  compare_strings_from_begin);

    char begin_sorted_path[MAX_PATH_LENGTH] = {};
    char end_sorted_path[MAX_PATH_LENGTH] = {};
    char original_text_path[MAX_PATH_LENGTH] = {};
    make_output_path(path,
                     "sorted_from_begin",
                     begin_sorted_path,
                     sizeof(begin_sorted_path));
    make_output_path(path,
                     "sorted_from_end",
                     end_sorted_path,
                     sizeof(end_sorted_path));
    make_output_path(path,
                     "original_text",
                     original_text_path,
                     sizeof(original_text_path));

    if (!write_text_result(begin_sorted_path, sorted_lines, text.number_of_lines)) {
        printf("Problem to create result files\n");
        free(sorted_lines);
        free_text_lines(&text);
        program_crash();
    }

    my_quick_sort(sorted_lines,
                  text.number_of_lines,
                  sizeof(sorted_lines[0]),
                  compare_strings_from_end);

    if (!write_text_result(end_sorted_path, sorted_lines, text.number_of_lines) ||
        !write_text_result(original_text_path, text.lines, text.number_of_lines)) {
        printf("Problem to create result files\n");
        free(sorted_lines);
        free_text_lines(&text);
        program_crash();
    }

    printf("Sort from begin saved into " GREEN "%s" RESET "\n", begin_sorted_path);
    printf("Sort from end saved into " GREEN "%s" RESET "\n", end_sorted_path);
    printf("Original text saved into " GREEN "%s" RESET "\n", original_text_path);
    free(sorted_lines);
    free_text_lines(&text);
    return EXIT_SUCCESS;
}

static ONE_TEXT_LINE *copy_line_index(const TEXT *text) {
    assert(text != NULL);
    assert(text->lines != NULL || text->number_of_lines == 0);

    if (text->number_of_lines == 0) {
        return NULL;
    }

    ONE_TEXT_LINE *lines_copy = (ONE_TEXT_LINE *) calloc(text->number_of_lines,
                                                         sizeof(ONE_TEXT_LINE));
    if (lines_copy == NULL) {
        printf("Memory allocation problem\n");
        program_crash();
    }

    memcpy(lines_copy, text->lines, text->number_of_lines * sizeof(ONE_TEXT_LINE));
    return lines_copy;
}

static int write_lines(FILE *file, const ONE_TEXT_LINE *lines, size_t number_of_lines) {
    assert(file != NULL);
    assert(lines != NULL || number_of_lines == 0);

    for (size_t line_index = 0; line_index < number_of_lines; line_index++) {
        assert(lines[line_index].line_start != NULL);
        if (fprintf(file, "%s", lines[line_index].line_start) < 0) {
            perror("fprintf");
            return 0;
        }
    }

    return 1;
}

static void make_output_path(const char *input_path,
                             const char *output_suffix,
                             char *output_path,
                             size_t output_path_size) {
    assert(input_path != NULL);
    assert(output_suffix != NULL);
    assert(output_path != NULL);
    assert(output_path_size > 0);

    const char *last_slash = strrchr(input_path, '\\');
    const char *last_unix_slash = strrchr(input_path, '/');
    const char *file_name_start = input_path;

    if (last_unix_slash != NULL &&
        (last_slash == NULL || last_unix_slash > last_slash)) {
        last_slash = last_unix_slash;
    }

    if (last_slash != NULL) {
        file_name_start = last_slash + 1;
    }

    const char *last_dot = strrchr(file_name_start, '.');
    size_t file_name_length = last_dot == NULL
        ? strlen(file_name_start)
        : (size_t) (last_dot - file_name_start);

    if (last_slash == NULL) {
        int written_chars = snprintf(output_path,
                                     output_path_size,
                                     "%.*s_%s.txt",
                                     (int) file_name_length,
                                     file_name_start,
                                     output_suffix);
        if (written_chars < 0 || (size_t) written_chars >= output_path_size) {
            printf("Output path is too long\n");
            program_crash();
        }
        return;
    }

    size_t directory_length = (size_t) (last_slash - input_path + 1);
    if (directory_length >= output_path_size) {
        printf("Output path is too long\n");
        program_crash();
    }

    int written_chars = snprintf(output_path,
                                 output_path_size,
                                 "%.*s%.*s_%s.txt",
                                 (int) directory_length,
                                 input_path,
                                 (int) file_name_length,
                                 file_name_start,
                                 output_suffix);
    if (written_chars < 0 || (size_t) written_chars >= output_path_size) {
        printf("Output path is too long\n");
        program_crash();
    }
}

static int write_text_result(const char *path,
                             const ONE_TEXT_LINE *lines,
                             size_t number_of_lines) {
    assert(path != NULL);
    assert(lines != NULL || number_of_lines == 0);

    FILE *file = fopen(path, "wb");
    if (file == NULL) {
        perror("fopen");
        return 0;
    }

    int write_status = write_lines(file, lines, number_of_lines);
    if (fclose(file) != 0) {
        perror("fclose");
        return 0;
    }

    return write_status;
}
