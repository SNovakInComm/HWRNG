// --------------------------------------------------
//
//      HWRNG (HardWare Random Number Generator)
//
//                  -by Steven Novak
//                  01/11/2018
// --------------------------------------------------

#include "hwrng.h"

// --------------------------------------------------
// ------------------------- Random Number Routines
// --------------------------------------------------

// ----- Checks if the processor supports RDRAND
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
        return 0;
    else
        return 1;
}

// ----- Checks if the processor supports RDRAND
void GetHardware64(unsigned long long *number)
{
    unsigned char passed;
        
    asm volatile ("rdrand %0\t\n"
                  "setc %1\t\n"
                  : "=r" (*number), "=qm" (passed));
    if(!passed)
    {
        // !!! Critical: There was an unknow error reading the RDRAND instruction !!!
        *number = 0;
    }
}