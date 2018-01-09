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

// ------------------------- UI Declarations
void PrintUsage();

int GetNumBytes(int, char**);
int GetFileName(int, char**);

// ------------------------- RNG Declarations
int CheckHardwareAvailability();
void GetHardware64(unsigned long long*);

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
        if(fileIndex < 0)
            printf("%d) %#018llx\n", i, *randomNumber);
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
            fputc(byte[i], outfile);
        }   

        fclose(outfile);
        printf("...done");
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

// --------------------------------------------------
// ------------------------- Random Number Routines
// --------------------------------------------------

int CheckHardwareAvailability()
{
    unsigned int ecx;
    unsigned int leaf = 1;
    unsigned int subleaf = 0;
    
    asm volatile("cpuid"
                 : "=c" (ecx)
                 : "a" (leaf), "c" (subleaf));
    
    
    // Check for RDRAND Support
    if((ecx & 0x40000000) !=  0x40000000)        
    {   
        return 0;
    }
    else
    {
        return 1;
    }
}

void GetHardware64(unsigned long long *number)
{
    unsigned char passed;
        
    asm volatile ("rdrand %0\t\n"
                  "setc %1\t\n"
                  : "=r" (*number), "=qm" (passed));
    if(!passed)
        printf("!!! Critical: There was an unknow error reading the RDRAND instruction !!!\n");
}