/*
 * Project: Tail
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
            "* Launch tail.exe and program will start read your information from console.*\n"
            "* After all it will print last 10 strings of your enter.                    *\n"
            "* If you want to custom program, add next keys after >tail                  *\n"
            "*    -n <number> after work program print last <number> string              *\n"
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
            is_stdin = 0;
            return 1;
        }
}

int get_parameters(int argc, char** argv, int *numberOfStrings, FILE **input)
{
    switch(argc)
    {
        // >tail
        case 1:
            return 1;
            
        // >tail input.txt OR >tail help
        case 2:
            if ((strcmp(argv[1], "help") == 0)) { show_help(); return 0; }
            return try_open_file(input, argv[1]);
            
        // >tail -n 11
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

        // >tail -n 10 input.txt OR tail input.txt -n 10
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

void show_strings(int numberOfStrings, int scannedStrings, FILE **input, char **strings)
{
    int i;

    int start = scannedStrings > numberOfStrings ? (scannedStrings % numberOfStrings) : 0; 
    int count = scannedStrings < numberOfStrings ? scannedStrings : numberOfStrings;    

    for (i = 0; i < count; i++)
    {
        printf("%s\n", strings[(start + i) % numberOfStrings]);
        free(strings[(start + i) % numberOfStrings]);
    }

    free(strings);
}

char* read_string(FILE **input)
{
    int k = 0; //current number of chars
    int defaultCharCount = 128;
    int currentCharCount = 0;
    char ch = 0;
    char* string = (char *) malloc(sizeof(char) * SUPPLIER);
    if (string == NULL) error(MEMORY_ERROR);
    
    while (((ch = fgetc(*input)) != '\n') && (!feof(*input)))
    {
        if (currentCharCount == defaultCharCount)
        {
            string = realloc(string, sizeof(char) * (defaultCharCount += SUPPLIER));
            if (string == NULL) error(MEMORY_ERROR);
        }
        string[currentCharCount++] = ch;
    }

    string[currentCharCount] = '\0';
    return string;  
}


void tail(FILE **input, const int numberOfStrings)
{
    int scannedStrings;
    char **strings, *buffer;
    strings = (char**) malloc(sizeof(char*) * numberOfStrings);
    if (strings == NULL) error(MEMORY_ERROR);

    //start reading
    for (scannedStrings = 0; !feof(*input); scannedStrings++)
    {
        if ((buffer = read_string(input)) == 0) break; 
        strings[scannedStrings % numberOfStrings] = buffer;
    }

    show_strings(numberOfStrings, scannedStrings, input, strings);
}

int main(int argc, char **argv)
{
    int numberOfStrings = DEFAULT_COUNT;
    FILE *input = (FILE*) stdin;

    if (get_parameters(argc, argv, &numberOfStrings, &input))
    {
        tail(&input, numberOfStrings);
        return 0;
    }
    else
    {
        fclose(input);
        return 0;
    }
}
