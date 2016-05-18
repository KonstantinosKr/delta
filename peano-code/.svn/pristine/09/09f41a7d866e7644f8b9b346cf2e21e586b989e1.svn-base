// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www.peano-framework.org
#ifndef _TARCH_LA_VECTORASSIGNLIST_H_
#define _TARCH_LA_VECTORASSIGNLIST_H_


#include "tarch/Assertions.h"
#include <string>


namespace tarch {
  namespace la {
    template<int Size, typename Scalar>
    class VectorAssignList;

    template<int Size, typename Scalar>
    VectorAssignList<Size,Scalar> assignList(Vector<Size,Scalar>& vector);
  }
}


/**
 * Wrapper around vector objects to enable comma separated list assignment.
 */
template<int Size, typename Scalar>
class tarch::la::VectorAssignList {
  private:
    Vector<Size,Scalar>& _vector;
    int _index;

  public:
    /**
     * Constructor.
     */
    VectorAssignList(Vector<Size,Scalar>& vector);

    /**
     * Constructor, to start assignment not from first component (index > 0).
     */
    VectorAssignList(Vector<Size,Scalar>& vector, int index);

    /**
     * Destructor, asserts that the right amount of elements have been assigned.
     */
    ~VectorAssignList();

    /**
     * Assigns the first element.
     */
    VectorAssignList<Size,Scalar>& operator= (const Scalar& toAssign);

    /**
     * Assigns all futher elements.
     */
    VectorAssignList<Size,Scalar>& operator, (const Scalar & toAssign);

    std::string toString() const;
};



#include "tarch/la/VectorAssignList.cpph"

#endif
