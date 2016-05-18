#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>


#define pfor(counter,from,to,minGrainSize) \
  { \
  const int tailoredMinGrainSize = minGrainSize==0 ? to-from+1 : minGrainSize; \
  tbb::parallel_for( tbb::blocked_range<int>(from,to,tailoredMinGrainSize),\
  [&](const tbb::blocked_range<int>& r) { for(int counter=r.begin(); counter!=r.end(); ++counter) {

#define endpfor \
  }}); }
