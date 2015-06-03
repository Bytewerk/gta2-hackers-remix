#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// when adding new functions, consult this Wikipedia (english) article:
// Comparison_of_programming_languages_(string_functions)

/*
        LOWER & UPPER

        Description:
                Converts all characters to lowercase or uppercase.

        Example Usage:
                char* low = cstr_lower("MiXeD CaSe", true);
                char* up  = cstr_upper(low, true);
                printf("%s, %s\n", low, up); //  mixed case MIXED CASE
                free(low);
                free(up);
*/
#define cstr_lower(CSTR, BOOL_COPY) cstr_lower_upper(CSTR, BOOL_COPY, true)
#define cstr_upper(CSTR, BOOL_COPY) cstr_lower_upper(CSTR, BOOL_COPY, false)
char *cstr_lower_upper(char *cstr, bool copy, bool lower);

/*
        REPLACE

        Description:
                Replaces all instances of 'find' with the 'replace' character.
                Depending on the copy-boolean, this happens inline or on a copy
                of the cstr-buffer. The function always returns a reference to
                the replaced string (must be free'd when copy is true!)

                Count can be 0 to replace all instances, or the maximum number
                of chars that should be replaced.

                Replace can also be the null terminator to cut off the string.

        Example Usage:
                char* hello = "hello world!";
                char* heeeo = cstr_replace(hello, 'l', 'e', 2, true);
                printf(heeeo); // "heeeo world"
                free(heeeo);
*/
char *cstr_replace(char *cstr, char find, char replace, uint16_t count,
                   bool copy);

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
                ignored - this means trim will not be necessary in most cases,
                when the delimeter is ' '.

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
