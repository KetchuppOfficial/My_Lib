#include "../My_Lib.h"

//RELATED TO LOG FILE
//***************************************************************************************
FILE *LOG_FILE_ = NULL;

struct Error
{
    enum ERROR_TYPES type;
    const char *description;
};

static const struct Error ERRORS_LIST[] =
{
    /* Basic errors that can occure in any program */

    {UNDEF_BEH,     "Undefined behavior"                  },
    {FUNC_ERROR,    "Error in a callee"                   },
    {NE_MAIN_ARGS,  "Not enough command line arguments"   },
    
    {LOG_NOT_OPEN,  "Impossible to open log file"         },
    {OPEN_ERR,      "Impossible to open file"             },
    {CLOSE_ERR,     "Impossible to close file"            },
    {EMPTY_FILE,    "File is empty"                       },
    
    {POS_VAL,       "Positive value expected"             },
    {NEG_VAL,       "Negative value expected"             },
    {N_POS_VAL,     "Not positive value expected"         },
    {N_NEG_VAL,     "Not negative value expected"         },
    {UNEXP_ZERO,    "Zero is not expected"                },
    {UNEXP_VAL,     "Unexpected value"                    },
    {UNEXP_SYMB,    "Unexpected symbol"                   },

    {ZERO_DIV,      "Zero division detected"              },
    {ZERO_POW_ZERO, "Zero in the power of zero detected"  },

    {NULL_PTR,      "Pointer is NULL"                     },
    {NOT_NULL_PTR,  "Pointer is not NULL"                 },

    {NE_MEM,        "Impossible to allocate enough memory"}
};

static void Close_Log_File (void)
{
    if (fclose (LOG_FILE_) == 0)
        printf ("Log file is closed successfully\n");
    else
        printf ("Log file is NOT closed successfully\n");
}

int Open_Log_File (const char *const log_name)
{
    size_t name_len = strlen (log_name);
    char *name = (char *)calloc (name_len + sizeof (".log"), sizeof (char));
    memcpy (name, log_name, name_len);
    strcat (name, ".log");
    
    LOG_FILE_ = fopen (name, "wb");

    if (!LOG_FILE_)
    {
        LOG_FILE_ = stderr;
        MY_ASSERT (false, "FILE *LOG_FILE_", LOG_NOT_OPEN, ERROR);
    }

    fprintf (LOG_FILE_, "**********************************\n");
    fprintf (LOG_FILE_, "*        THIS IS LOG_FILE        *\n");
    fprintf (LOG_FILE_, "**********************************\n\n");

    atexit (Close_Log_File);

    free (name);

    return NO_ERRORS;
}

int My_Assert_ (bool condition, const char *const file, const int line,
                const char *const fun_name, const char *const var_name, const int err_name)
{
    if (!LOG_FILE_ || !fun_name || !var_name || err_name < 0 || line <= 0)
        return ERROR;

    if (!condition)
    {
        fprintf (LOG_FILE_, "File: %s\n",                                   file);
        fprintf (LOG_FILE_, "Line: %d\n",                                   line);
        fprintf (LOG_FILE_, "Function with error: %s\n",                    fun_name);
        fprintf (LOG_FILE_, "Variable or function that caused error: %s\n", var_name);
        fprintf (LOG_FILE_, "Error description: %s\n\n",                    ERRORS_LIST[err_name].description);

        return 1;
    }

    return 0;
}
//***************************************************************************************

//RELATED TO READING FROM FILE
//***************************************************************************************
FILE *Open_File (const char *const file_name, const char *const mode)
{
    MY_ASSERT (file_name, "const char *const file_name", NULL_PTR, NULL);
    MY_ASSERT (mode,      "const char *const mode",      NULL_PTR, NULL);

    FILE *file_ptr = fopen (file_name, mode);

    MY_ASSERT (file_ptr, "FILE *file_ptr", OPEN_ERR, NULL);

    return file_ptr;
}

int Close_File (FILE *file_ptr, const char *const file_name)
{
    MY_ASSERT (file_ptr,  "FILE *file_ptr",        NULL_PTR, ERROR);
    MY_ASSERT (file_name, "const char *file_name", NULL_PTR, ERROR);

    MY_ASSERT (fclose (file_ptr) == 0, "fclose ()", CLOSE_ERR, ERROR);

    return NO_ERRORS;
}

long Define_File_Size (FILE *file_ptr)
{
    MY_ASSERT (file_ptr, "FILE *file_ptr", NULL_PTR, ERROR);

    long start_pos = ftell (file_ptr);

    if (fseek (file_ptr, 0L, SEEK_END))
        MY_ASSERT (0, "fseek ()", FUNC_ERROR, ERROR);

    long n_symbs = ftell (file_ptr);
    MY_ASSERT (n_symbs > 0L, "ftell ()", FUNC_ERROR, ERROR);

    if (fseek (file_ptr, start_pos, SEEK_SET))
        MY_ASSERT (false, "fseek ()", FUNC_ERROR, ERROR);

    return n_symbs;
}

static char *Make_Buffer_ (FILE *file_ptr, const long n_symbs)
{
    char *buffer = (char *)calloc (n_symbs + 1L, sizeof (char));
    MY_ASSERT (buffer, "char *buffer", NE_MEM, NULL);

    size_t read_symbs = fread (buffer, sizeof (char), n_symbs, file_ptr);

    MY_ASSERT (read_symbs == (size_t)n_symbs, "size_t read_symbs", UNEXP_VAL, NULL);

    return buffer;
}

char *Make_File_Buffer (const char *const file_name, long *const n_symbs)
{
    MY_ASSERT (file_name, "const char *const file_name", NULL_PTR, NULL);
    MY_ASSERT (n_symbs,   "long *const n_symbs",         NULL_PTR, NULL);

    FILE *file = Open_File (file_name, "rb");
    MY_ASSERT (file, "Open_File ()", FUNC_ERROR, NULL);

    *n_symbs = Define_File_Size (file);
    MY_ASSERT (*n_symbs > 0, "Define_File_Size ()", FUNC_ERROR, NULL);

    char *buffer = Make_Buffer_ (file, *n_symbs);
    MY_ASSERT (buffer, "_Make_Buffer ()", FUNC_ERROR, NULL);

    int closure_status = Close_File (file, file_name);
    MY_ASSERT (closure_status != ERROR, "Close_File ()", FUNC_ERROR, NULL);

    return buffer;
}
//***************************************************************************************
