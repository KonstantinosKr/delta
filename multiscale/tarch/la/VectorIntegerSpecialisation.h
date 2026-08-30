#if defined(SpecialiseVectorTemplatesForIntegers)


#include "tarch/compiler/CompilerSpecificSettings.h"


namespace tarch {
  namespace la {
    template<>
    struct Vector<2,int>;

    template<>
    struct Vector<3,int>;

    template<>
    struct Vector<4,int>;
  }
}



/**
 * Specialised version of the vector
 *
 * This specialisation for integers does not work with an array. Instead, it
 * uses a fixed number of integer attributes. Some compilers synchronise each
 * array access with the caches (cmp restrict keyword) and thus using two
 * integers is faster than having one integer array with two entries.
 *
 * The impact of such a specialisation has to be studied carefully.
 *
 * @author Roland Wittmann, Tobias Weinzierl
 */
template<>
struct tarch::la::Vector <2, int> {
  private:
    int _values0, _values1;
  public:
    Vector ();
    Vector<2, int>& operator= (const Vector<2, int>& toAssign);
    VectorAssignList<2,int> operator=(const int& value);
    Vector(const Vector<2,int>&  toCopy);
    Vector (const int& initialValue);
    Vector (const int& initialValue0, const int& initialValue1);
    int size() const;
    const int & operator[] (int index) const;
    int & operator[] (int index);
    const int & operator() (int index) const;
    int & operator() (int index);
    std::string toString() const;

    template <class NewScalarType>
    tarch::la::Vector<2,NewScalarType> convertScalar() const {
      tarch::la::Vector<2,NewScalarType> result;
      result[0] = _values0;
      result[1] = _values1;
      return result;
    }
};


template<>
struct tarch::la::Vector <3, int> {
  private:
    int _values0, _values1, _values2;
  public:
    Vector ();
    Vector<3, int>& operator= (const Vector<3, int>& toAssign);
    VectorAssignList<3,int> operator=(const int& value);
    Vector(const Vector<3,int>&  toCopy);
    Vector (const int& initialValue);
    Vector (const int& initialValue0, const int& initialValue1, const int& initialValue2);
    int size() const;
    const int & operator[] (int index) const;
    int & operator[] (int index);
    const int & operator() (int index) const;
    int & operator() (int index);
    std::string toString() const;

    template <class NewScalarType>
    tarch::la::Vector<3,NewScalarType> convertScalar() const {
      tarch::la::Vector<3,NewScalarType> result;
      result[0] = _values0;
      result[1] = _values1;
      result[2] = _values2;
      return result;
    }
};



template<>
struct tarch::la::Vector <4, int> {
  private:
    int _values0, _values1, _values2, _values3;
  public:
    Vector ();
    Vector<4, int>& operator= (const Vector<4, int>& toAssign);
    VectorAssignList<4,int> operator=(const int& value);
    Vector(const Vector<4,int>&  toCopy);
    Vector (const int& initialValue);
    Vector (const int& initialValue0, const int& initialValue1, const int& initialValue2, const int& initialValue3);
    int size() const;
    const int & operator[] (int index) const;
    int & operator[] (int index);
    const int & operator() (int index) const;
    int & operator() (int index);
    std::string toString() const;

    template <class NewScalarType>
    tarch::la::Vector<4,NewScalarType> convertScalar() const {
      tarch::la::Vector<4,NewScalarType> result;
      result[0] = _values0;
      result[1] = _values1;
      result[2] = _values2;
      result[3] = _values3;
      return result;
    }
};
#endif
