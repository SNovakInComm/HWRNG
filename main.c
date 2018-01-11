//
//  main.c
//  Generate Arbitrary Number Of Random Bytes
//  Using hardware rng when available
//
//  Created by Steven Novak on 1/5/18.
//  Copyright © 2018 Steven Novak. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hwrng.h"

// ------------------------- UI Declarations
void PrintUsage();

int GetNumBytes(int, char**);
int GetFileName(int, char**);

// --------------------------------------------------
// ------------------------- Main
// --------------------------------------------------

int main (int argcount, char** args)
{
    int byteCount = 0;
    int fileIndex = -1;
    char* fileName;

    // ------------------------- Validate Input
    if(argcount < 2)
    {
        PrintUsage();
        return -1;
    }

    byteCount = GetNumBytes(argcount, args);
    fileIndex = GetFileName(argcount, args);

    if(byteCount == -1 && fileIndex == -1)
    {
        PrintUsage();
        return -1;
    }

    // ------------------------- Check if CPU supports RDRAND instruction
    if(!CheckHardwareAvailability())
    {
        printf("No hardware support for random number generation!!\n");
        return -1;
    }
    else
        printf("Hardware Random Number Generation Supported!!!\n");
    

    // ------------------------- Generate and random data
    int paddedLength = byteCount + (byteCount % sizeof(unsigned long long));
    char* buffer = (char*)malloc(paddedLength);

    unsigned long long* randomNumber = (unsigned long long*)buffer;

    printf("Generating %d random bytes:", paddedLength);
    for(int i=0; i<paddedLength / sizeof(unsigned long long); i++)
    {
        GetHardware64(randomNumber);
        randomNumber++;
    }
    printf("...done\n");

    
    if(fileIndex > -1 )
    {
        printf("Writing %d random bytes to file", byteCount);
        FILE *outfile;
        errno_t error = fopen_s(&outfile, args[fileIndex], "w");
        char* byte = (char*)buffer;
        for(int i=0; i<byteCount; i++)
        {
            printf("%d ", i);
            fputc(byte[i], outfile);
        }   

        fclose(outfile);
        printf("...done");
    }
    else
    {
        printf("Writing %d Random Bytes:\n", byteCount);
        unsigned int* number = (unsigned int*)buffer;
        for(int i=0; i<byteCount / sizeof(unsigned int); i++)
        {
            printf("%u\n", number[i]);
        }  
    }


    free(buffer);

    return 0;
}

// --------------------------------------------------
// ------------------------- UI Routines
// --------------------------------------------------

void PrintUsage()
{
    printf("\n********************************************\n");
    printf("***** Hardware Random Number Generator *****\n");
    printf("*****            Ver 0.0.1             *****\n");
    printf("*****  Note: RDRAND Support available  *****\n");
    printf("*****   on IvyBridge and newer CPUs    *****\n");
    printf("*****                                  *****\n");
    printf("********************************************\n\n");
    printf("Usage: hwrng <options>\n\n");
    printf("-l <#>\t\t\t Number of bytes to generate\n");
    printf("-o <filename>\t\t Output File Name\n\n");
}

int GetNumBytes(int argcount, char** args)
{
    int numOfBytes = -1;
    for(int i=0; i<argcount; i++)
    {
        if(!strcmp(args[i], "-l"))
        {
            if(i+1< argcount)
                numOfBytes = atoi(args[i+1]); 
            else
                printf("Error parsing number of bytes!\n Ensure -l has a valid integer argument \n");
        }
    }
    return numOfBytes;
}

int GetFileName(int argcount, char** args)
{
    int index = -1;
    for(int i=0; i<argcount; i++)
    {
        if(!strcmp(args[i], "-o") && i < (argcount - 1))
            return i+1;
    }
    return -1;
}