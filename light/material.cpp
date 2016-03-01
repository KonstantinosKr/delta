#include "material.h"
#include "stdio.h"
// interaction per interaction type
iREAL material::iparam[NINT][NFOR];   

//material pairs
int material::pairs[1]; 

//material parameters 
iREAL material::mparam[NMAT][1]; 

//interaction kind
int material::ikind[NFOR];

material::material()
{
}
