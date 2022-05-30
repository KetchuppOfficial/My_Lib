#ifndef LOG_FILE_H_INCLUDED
#define LOG_FILE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

enum ERROR_TYPES
{
    UNDEF_BEH     = 0,
    FUNC_ERROR    = 1,
    NE_MAIN_ARGS  = 2,

    LOG_NOT_OPEN  = 3,
    OPEN_ERR      = 4,
    CLOSE_ERR     = 5,
    EMPTY_FILE    = 6,

    POS_VAL       = 7,
    NEG_VAL       = 8,
    N_POS_VAL     = 9,
    N_NEG_VAL     = 10,
    UNEXP_ZERO    = 11,
    UNEXP_VAL     = 12,
    UNEXP_SYMB    = 13,

    ZERO_DIV      = 14,
    ZERO_POW_ZERO = 15,

    NULL_PTR      = 16,
    NOT_NULL_PTR  = 17,

    NE_MEM        = 18,
};

enum ERRORS
{
    NO_ERRORS = INT_MAX,
    ERROR     = INT_MIN,
};

#define MY_ASSERT(condition, var, err_num, error)                               \
do                                                                              \
{                                                                               \
    if (!(condition))                                                           \
    {                                                                           \
        _My_Assert (false, __FILE__, __LINE__, __FUNCTION__, var, err_num);     \
        return error;                                                           \
    }                                                                           \
}                                                                               \
while (0)

int _My_Assert         (bool condition, const char *const file, const int line,
                        const char *const fun_name, const char *const var_name, const int err_name);
int  Open_Log_File     (const char *const log_name);
FILE *Open_File        (const char *const file_name, const char *const mode);
long Define_File_Size  (FILE *file_ptr);
int  Close_File        (FILE *file_ptr, const char *file_name);
char *Make_File_Buffer (const char *const file_name, long *const n_symbs);

#endif
