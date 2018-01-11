// --------------------------------------------------
//
//      HWRNG (HardWare Random Number Generator)
//
//                  -by Steven Novak
//                  01/11/2018
// --------------------------------------------------
// 
// True random number generation is accomplished thorugh
// use of the RDRAND command. 
//
// To use: Call CheckHardwareAvailability() to determine 
// if the current processor supports RDRAND execution.
//
// If so, call GetHardware64(&randomNumber) to generate 
// a true (not pseudo) random number.
//

#ifndef HWRNG_H
#define HWRNG_H

// ------------------------- RNG Declarations
int CheckHardwareAvailability();

void GetHardware64(unsigned long long*);

#endif