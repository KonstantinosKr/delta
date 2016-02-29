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
  ikind[0] = GRANULAR;
  ikind[1] = GRANULAR;
  ikind[2] = GRANULAR;
  
  //GRANULAR interaction type parameters 
  iparam[SPRING][GRANULAR] = 1E2;
  iparam[DAMPER][GRANULAR] = 1;
  iparam[FRISTAT][GRANULAR] = 0;
  iparam[FRIDYN][GRANULAR] = 0;
  iparam[FRIROL][GRANULAR] = 0;
  iparam[FRIDRIL][GRANULAR] = 0;
  iparam[KSKN][GRANULAR] = 0;
  iparam[LAMBDA][GRANULAR] = 0;
  iparam[YOUNG2][GRANULAR] = 0;
  iparam[KSKN2][GRANULAR] = 0;
  iparam[SIGC][GRANULAR] = 0;
  iparam[TAUC][GRANULAR] = 0;
  iparam[ALPHA][GRANULAR] = 0;
}
