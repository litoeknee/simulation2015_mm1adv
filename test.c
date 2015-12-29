/*
This tests the generators in this collection using L'Ecuyer's
TestU01 RNG test suite.

References:

[1]: http://www.iro.umontreal.ca/~simardr/testu01/tu01.html
*/

#include "simplerandom-c.h"
#include <stdio.h>

#define upbound 10000

SimpleRandomCong_t      cong_state;
SimpleRandomSHR3_t      shr3_state;
SimpleRandomMWC1_t      mwc1_state;
SimpleRandomMWC2_t      mwc2_state;
SimpleRandomMWC64_t     mwc64_state;
SimpleRandomKISS_t      kiss_state;
SimpleRandomKISS2_t     kiss2_state;
SimpleRandomLFSR113_t   lfsr113_state;
SimpleRandomLFSR88_t    lfsr88_state;
FILE  *outfile;

unsigned int cong_function(void)
{
    return simplerandom_cong_next(&cong_state);
}

unsigned int shr3_function(void)
{
    return simplerandom_shr3_next(&shr3_state);
}

unsigned int mwc1_function(void)
{
    return simplerandom_mwc1_next(&mwc1_state);
}

unsigned int mwc2_function(void)
{
    return simplerandom_mwc2_next(&mwc2_state);
}

unsigned int mwc64_function(void)
{
    return simplerandom_mwc64_next(&mwc64_state);
}

unsigned int kiss_function(void)
{
    return simplerandom_kiss_next(&kiss_state);
}

unsigned int kiss2_function(void)
{
    return simplerandom_kiss2_next(&kiss2_state);
}

unsigned int lfsr113_function(void)
{
    return simplerandom_lfsr113_next(&lfsr113_state);
}

unsigned int lfsr88_function(void)
{
    return simplerandom_lfsr88_next(&lfsr88_state);
}

int main (void)
{
    unsigned int randomnumber;
    int i = 0;
    outfile = fopen("randomnumber.out", "w");

    /* Initialise all generators */
    simplerandom_cong_seed(&cong_state, UINT32_C(2051391225));
    simplerandom_shr3_seed(&shr3_state, UINT32_C(3360276411));
    simplerandom_mwc1_seed(&mwc1_state, UINT32_C(12345), UINT32_C(65437));
    simplerandom_mwc2_seed(&mwc2_state, UINT32_C(12345), UINT32_C(65437));
    simplerandom_mwc64_seed(&mwc64_state, UINT32_C(7654321), UINT32_C(521288629));
    simplerandom_kiss_seed(&kiss_state, UINT32_C(2247183469), UINT32_C(99545079), UINT32_C(3269400377), UINT32_C(3950144837));
    simplerandom_kiss2_seed(&kiss2_state, UINT32_C(2247183469), UINT32_C(99545079), UINT32_C(3269400377), UINT32_C(3950144837));
    simplerandom_lfsr113_seed(&lfsr113_state, 0, 0, 0, 0);
    simplerandom_lfsr88_seed(&lfsr88_state, 0, 0, 0);

    for (i = 0; i < upbound; i ++) {
        randomnumber = cong_function();
        fprintf(outfile, "%d, %f \n", randomnumber, (randomnumber >> 8 | 1) / 16777216.0);
    }

    for (i = 0; i < upbound; i ++) {
        randomnumber = shr3_function();
        fprintf(outfile, "%d, %f \n", randomnumber, (randomnumber >> 8 | 1) / 16777216.0);
    }

    for (i = 0; i < upbound; i ++) {
        randomnumber = mwc1_function();
        fprintf(outfile, "%d, %f \n", randomnumber, (randomnumber >> 8 | 1) / 16777216.0);
    }

    for (i = 0; i < upbound; i ++) {
        randomnumber = mwc2_function();
        fprintf(outfile, "%d, %f \n", randomnumber, (randomnumber >> 8 | 1) / 16777216.0);
    }

    for (i = 0; i < upbound; i ++) {
        randomnumber = kiss_function();
        fprintf(outfile, "%d, %f \n", randomnumber, (randomnumber >> 8 | 1) / 16777216.0);
    }

    for (i = 0; i < upbound; i ++) {
        randomnumber = mwc64_function();
        fprintf(outfile, "%d, %f \n", randomnumber, (randomnumber >> 8 | 1) / 16777216.0);
    }

    for (i = 0; i < upbound; i ++) {
        randomnumber = kiss2_function();
        fprintf(outfile, "%d, %f \n", randomnumber, (randomnumber >> 8 | 1) / 16777216.0);
    }

    for (i = 0; i < upbound; i ++) {
        randomnumber = lfsr113_function();
        fprintf(outfile, "%d, %f \n", randomnumber, (randomnumber >> 8 | 1) / 16777216.0);
    }

    for (i = 0; i < upbound; i ++) {
        randomnumber = lfsr88_function();
        fprintf(outfile, "%d, %f \n", randomnumber, (randomnumber >> 8 | 1) / 16777216.0);
    }
    return 0;
}

