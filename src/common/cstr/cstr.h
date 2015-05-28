#include <stddef.h>

/*
        cstr_merge(str1, str2, ...)

        Creates a new c string from the supplied arguments. Make sure that
        you free the new string after usage!

        Example usage:
                char* asdf = cstr_merge("a", "sd", "f");
                printf(mystr); // prints: 'asdf'
                free(asdf);
*/
#define cstr_merge(...) cstr_merge_internal(NULL, __VA_ARGS__, NULL)

char *cstr_merge_internal(char *arg_start, ...);
