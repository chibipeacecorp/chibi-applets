#include <stdio.h>
#include <string.h>


/*    MATH    ******************************************************/
int clampAddInt(int val, int adder, int max)
{
    if ( val + adder <= max ) { return val + adder; }
    return max;
}

int clampSubInt(int val, int subber, int min)
{
    if ( val - subber >= min ) { return val - subber; }
    return min;
}

int clampInt(int val, int min, int max)
{
	if (val >= min && val <= max) { return val; }
	if (val < min) { return min; }
	if (val > max) { return max; }
    return -1;
}

int invertInt(int val) { return val * -1; }



/*    DEBUG    ******************************************************/
