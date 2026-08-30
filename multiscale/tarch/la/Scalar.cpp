#include "tarch/la/Scalar.h"

#include <cmath>
#include <algorithm>
#include <limits>


double tarch::la::absoluteWeight(
  double value,
  double relativeError
) {
  const double weight = std::max(
    1.0, std::abs(value)
  );
  return relativeError * weight;
}


double tarch::la::absoluteWeight(
  double value0,
  double value1,
  double relativeError
) {
  const double weight = std::max(
    1.0, std::min(
      std::abs(value0), std::abs(value1)
    )
  );
  return relativeError * weight;
}


double tarch::la::absoluteWeight(
  double   values[],
  int      count,
  double   relativeError
) {
  double weight = std::numeric_limits<double>::max();

  for (int i=0; i<count; i++) {
	 weight = std::min( weight, std::abs(values[i]) );
  }

  weight = std::max(1.0,weight);

  return relativeError * weight;
}
