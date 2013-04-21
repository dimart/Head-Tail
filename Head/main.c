/*
 * Project: Head
 * Version: 1.0
 * Author: dimart
 * Date: 2012 year
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors.h"

#define SUPPLIER 128

void show_help()
{
    printf(
        "\n\n**********************************HELP***************************************\n"
            "* Launch head.exe and program will start read your information from console.*\n"
            "* After this it will print first 10 strings of your enter.                  *\n"
            "* If you want to custom program, add next keys after >head                  *\n"
            "*    -n <number> after work program print first <number> string             *\n"
            "*    <path>      program will read from file                                *\n"
            "*    help        print this help                                            *\n"
            "*****************************************************************************\n"
    );  
}

int try_open_file(FILE **input, char *path)
{   
    *input = (FILE*) fopen(path, "r");
    if (*input == NULL)
    {    
        error(OPEN_ERROR);
        return 0;
    }
    else
    {
        return 1;
    }
}

int get_parameters(int argc, char** argv, int *numberOfStrings, FILE **input)
{
    switch(argc)
    {
        // >head
        case 1:
            return 1;
            
        // >head input.txt OR >head help
        case 2:
            if ((strcmp(argv[1], "help") == 0)) { show_help(); return 0; }
            return try_open_file(input, argv[1]);
            
        // >head -n 11
        case 3:
            if (strcmp(argv[1], "-n") == 0)
            {
                *numberOfStrings = atoi(argv[2]);
                return 1;
            }
            else
            {
                error(INCORRECT_INPUT);
                return 0;
            }   

        // >head -n 10 input.txt OR head input.txt -n 10
        case 4:
            if ((strcmp(argv[1], "-n") == 0) && (atoi(argv[2]) != 0))
            {
                *numberOfStrings = atoi(argv[2]);
                return try_open_file(input, argv[3]);
            }
            else if ((strcmp(argv[2], "-n") == 0) && (atoi(argv[3]) != 0))
            {
                *numberOfStrings = atoi(argv[3]);
                return try_open_file(input, argv[1]);
            }
            error(INCORRECT_INPUT);
            return 0;

        default:
            error(INCORRECT_INPUT);
            return 0;
    }
}

void show_strings(int numberOfStrings, FILE **input, char **strings)
{
    int i;

    if (*input == stdin)
    { 
        char ch;
        while((ch = fgetc(*input)) != -1);
    }

    for (i = 0; i < numberOfStrings; i++)
        printf("%s\n", strings[i]);

   free(strings);   
}

void read_string(FILE **input, char *string)
{
    int k = 0; //current number of chars
    int charCount = 128;
    char ch;

    while (((ch = fgetc(*input)) != '\n') && !feof(*input))
    {
        if (k == charCount)
        {
            string = realloc(string, charCount += SUPPLIER);
            if (string == NULL) error(MEMORY_ERROR);
        }
        string[k++] = ch;
    }
    string[k] = 0;
}

void head(FILE **input, int numberOfStrings)
{
    int i, scannedStrings = 0;
    char **strings, ch;
    strings = (char**) malloc(sizeof(char*) * numberOfStrings);
    if (strings == NULL) error(MEMORY_ERROR);

    // Initialize an array of strings
    for (i = 0; i < numberOfStrings; i++)
        strings[i] = (char*) malloc(sizeof(char*) * SUPPLIER);
    
    // Start reading
    for (scannedStrings = 0; (scannedStrings < numberOfStrings) && !feof(*input); scannedStrings++)
    { 
        read_string(input, strings[scannedStrings]);
    }
                             
    show_strings(scannedStrings, input, strings);
}

int main(int argc, char **argv)
{
    int numberOfStrings = DEFAULT_COUNT;
    FILE *input = (FILE*) stdin;

    if (get_parameters(argc, argv, &numberOfStrings, &input))
    {
        head(&input, numberOfStrings);
        return 0;
    }
    else
    {
        fclose(input);
        return 0;
    }
}
