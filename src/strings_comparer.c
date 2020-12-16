#include "sortings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PARAMETERS_NUMBER 6

typedef void (*sorting_func_t)(strings_array_t, array_size_t, comparator_func_t);

struct parameters_t {
    array_size_t strings_number;
    char *input_filename;
    char *output_filename;
    sorting_func_t algorithm;
    comparator_func_t comparator;
};

int check_format(char *name) {
    char *ending = ".txt";
    unsigned int name_length = strlen(name), ending_length = strlen(ending);
    if (strcmp(name + name_length - ending_length, ending) != 0)
        return -1;
    else
        return 0;
}

int asc(const char *str1, const char *str2) {
    return strcmp(str1, str2);
}

int des(const char *str1, const char *str2) {
    return strcmp(str2, str1);
}

int parse_parameters(int argc, char **argv, struct parameters_t *parameters) {
    if (argc != PARAMETERS_NUMBER) {
        fprintf(stderr, "Incorrect number of parameters\n");
        return -1;
    }
    char *end;
    parameters->strings_number = strtoul(argv[1], &end, 10);
    if (*end != '\0') {
        fprintf(stderr, "Incorrect number of strings\n");
        return -1;
    }
    parameters->input_filename = argv[2];
    if (check_format(parameters->input_filename) != 0) {
        fprintf(stderr, "The input file is not txt\n");
        return -1;
    }
    parameters->output_filename = argv[3];
     if (check_format(parameters->output_filename) != 0) {
        fprintf(stderr, "The output file is not txt\n");
        return -1;
    }
    if (strcmp(argv[4], "bubble") == 0)
        parameters->algorithm = bubble;
    else if (strcmp(argv[4], "insertion") == 0)
        parameters->algorithm = insertion;
    else if (strcmp(argv[4], "merge") == 0)
        parameters->algorithm = merge;
    else if (strcmp(argv[4], "quick") == 0)
        parameters->algorithm = quick;
    else if (strcmp(argv[4], "radix") == 0)
        parameters->algorithm = radix;
    else {
        fprintf(stderr, "The algorithm name is incorrect\n");
        return -1;
    }
    if (strcmp(argv[5], "asc") == 0)
        parameters->comparator = asc;
    else if (strcmp(argv[5], "des") == 0)
        parameters->comparator = des;
    else {
        fprintf(stderr, "The comparator name is incorrect\n");
        return -1;
    }
    return 0;
}

int read_input_file(char *filename, array_size_t *strings_number, strings_array_t array) {
    FILE *input_file = fopen(filename, "rb");
    if (input_file == NULL) {
        fprintf(stderr, "Failed to open input file\n");
        return -1;
    }
    for (unsigned int i = 0; i < (unsigned int)*strings_number && !feof(input_file); i++) {
        if (fgets(array[i], MAX_INPUT_STRING_SIZE, input_file) == NULL) {
            fprintf(stderr,"Failed to read strings from the input file\n");
            fclose(input_file);
            return -1;
        }
    }
    if (fclose(input_file) != 0) {
        fprintf(stderr, "Failed to close the input file\n");
        return -1;
    }
    return 0;
}

int write_output_file(char *filename, array_size_t *strings_number, strings_array_t array) {
    FILE *output_file = fopen(filename, "wb");
    if (output_file == NULL) {
        fprintf(stderr, "Failed to create output file\n");
        return -1;
    }
    if (*strings_number > 0) {
        for (unsigned int i = 0; i < (unsigned int)*strings_number; i++) {
            if (fputs(array[i], output_file) == EOF) {
                fprintf(stderr, "Failed to write strings in the output file\n");
                fclose(output_file);
                return -1;
            }
            if(strcspn(array[i], "\n") == strlen(array[i])) {
                if(fputs("\n", output_file) == EOF) {
                    fprintf(stderr, "Failed to add new line in output file\n");
                    return -1;
                 }
            }
        }
    }
    else {
         if (fputs("\n", output_file) == EOF) {
        fprintf(stderr, "Failed to add new line in output file\n");
        fclose(output_file);
        return -1;
        }
    }
    if (fclose(output_file) != 0) {
        fprintf(stderr, "Failed to close the output file\n");
        return -1;
    }
    return 0;
}

void free_array(strings_array_t array, unsigned int *size) {
    for(unsigned int i = 0; i < *size; i++)
        free(array[i]);
    free(array);
}

int main(int argc, char **argv) {
    struct parameters_t parameters;
    if (parse_parameters(argc, argv, &parameters) != 0)
        return -1;

    strings_array_t strings_array = (char**)malloc(sizeof(char*) * parameters.strings_number);
    if(strings_array == NULL) {
        fprintf(stderr, "Failed to allocate memory for strings\n");
        return -1;
    }
    for(unsigned int i = 0; i < parameters.strings_number; i++) {
        strings_array[i] = (char*)malloc(sizeof(char) * MAX_INPUT_STRING_SIZE);
        if(strings_array[i] == NULL) {
            free_array(strings_array, &i);
            fprintf(stderr, "Failed to allocate memory for string\n");
            return -1;
        }
    }
    if (read_input_file(parameters.input_filename, &parameters.strings_number, strings_array) != 0) {
        free_array(strings_array, (unsigned int *)&parameters.strings_number);
        return -1;
    }
    parameters.algorithm(strings_array, parameters.strings_number, parameters.comparator);

    if (write_output_file(parameters.output_filename, &parameters.strings_number, strings_array) != 0) {
         free_array(strings_array, (unsigned int *)&parameters.strings_number);
         return -1;
    }
    free_array(strings_array, (unsigned int *)&parameters.strings_number);
    return 0;
}