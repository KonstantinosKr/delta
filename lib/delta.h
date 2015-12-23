#include "bf.h"
#include "penalty.h"
#include "hybrid.h"

/**
 * Compute maximum distance between earth and sun
 *
 * Here's a longer description of what is going on
 *
 * @author your name
 *
 * @param method  Which distance calculation shall be used (0=brute force, 1=penalty, 2=hybrid)
 * @param epsilon 
 */
void delta (int method, iREAL epsilon,
            unsigned int s1, unsigned int e1, unsigned int s2, unsigned int e2, 
            iREAL *t[3][3], unsigned long long int *ncontacts);
