#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
        COPY

        Description:
                Fully copies one string into another buffer. Free the new buffer
                manually after usage.

        Example Usage:
                char* hello = cstr_copy("hello world"); // hello now contains
                // the string "hello world" - but it can be modified without
                // "bad permissions" errors.
*/
char *cstr_copy(char *cstr);

/*
        TRIM

        Description:
                Removes additional white spaces and tabs ('isspace'-chars) at
                the beginning and end of the _original_ string buffer.

        Example Usage:
                char* trimmed = cstr_trim(cstr_copy("    hello world   "));
                printf(trimmed); // "hello world"
*/
char *cstr_trim(char *cstr);

/*
        SPLIT

        Description:
                Creates a cstr_split_t struct by cutting the supplied cstr in
                pieces. Multiple occurrences of the delimeter in a row get
                ignored.

        Example Usage:
                cstr_split_t* split = cstr_split("hello world! :D", ' ', 0);
                for(int i=0;i<split->count;i++)
                        printf("piece %i: %s\n", i, split->pieces[i]);
                cstr_split_free(split);
*/

typedef struct {
  char **pieces;
  uint16_t count;
} cstr_split_t;

cstr_split_t *cstr_split(char *cstr, char delimeter, bool trim);
void cstr_split_free(cstr_split_t *split);

/*
        MERGE

        Syntax:
                cstr_merge(cstr1, cstr2, ...)

        Description:
                Creates a new c string from the supplied arguments. Make sure
                that you free the new string after usage!

        Example Usage:
                char* asdf = cstr_merge("a", "sd", "f");
                printf(mystr); // prints: 'asdf'
                free(asdf);
*/
#define cstr_merge(...) cstr_merge_internal(NULL, __VA_ARGS__, NULL)

char *cstr_merge_internal(char *arg_start, ...);
