#include <stdio.h>
#include "errors.h"

void error(int error_code)
{
    switch(error_code)
    {
        case OPEN_ERROR:
            fprintf(stderr, "\nERROR: Cannot open file.");
            show_help();
            break;
        case INCORRECT_INPUT:
            fprintf(stderr, "\nERROR: Incorrect input.");
            show_help();
            break;
        case MEMORY_ERROR:
            fprintf(stderr, "\nMEMORY ERROR.");
            break;
    }
}