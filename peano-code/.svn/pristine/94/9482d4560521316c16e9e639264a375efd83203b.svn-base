#include "tarch/la/Vector.h"


#if defined(SpecialiseVectorTemplatesForIntegers)

tarch::la::Vector<2,int>::Vector() {
}


tarch::la::Vector<3,int>::Vector() {
}


tarch::la::Vector<4,int>::Vector() {
}


tarch::la::Vector<2,int>&  tarch::la::Vector<2,int>::operator= (
  const Vector<2,int>&  toAssign
) {
  _values0 = toAssign._values0;
  _values1 = toAssign._values1;
  return *this;
}


tarch::la::Vector<3,int>&  tarch::la::Vector<3,int>::operator= (
  const Vector<3,int>&  toAssign
) {
  _values0 = toAssign._values0;
  _values1 = toAssign._values1;
  _values2 = toAssign._values2;
  return *this;
}


tarch::la::Vector<4,int>&  tarch::la::Vector<4,int>::operator= (
  const Vector<4,int>&  toAssign
) {
  _values0 = toAssign._values0;
  _values1 = toAssign._values1;
  _values2 = toAssign._values2;
  _values3 = toAssign._values3;
  return *this;
}


tarch::la::VectorAssignList<2,int> tarch::la::Vector<2,int>::operator=(
  const int& value
) {
  _values0 = value;
  return VectorAssignList<2,int>(*this,1);
}


tarch::la::VectorAssignList<3,int> tarch::la::Vector<3,int>::operator=(
  const int& value
) {
  _values0 = value;
  return VectorAssignList<3,int>(*this,1);
}


tarch::la::VectorAssignList<4,int> tarch::la::Vector<4,int>::operator=(
  const int& value
) {
  _values0 = value;
  return VectorAssignList<4,int>(*this,1);
}


tarch::la::Vector<2,int>::Vector (
  const Vector<2,int>& toCopy
):
  _values0( toCopy._values0 ),
  _values1( toCopy._values1 ) {
}


tarch::la::Vector<3,int>::Vector (
  const Vector<3,int>& toCopy
):
  _values0( toCopy._values0 ),
  _values1( toCopy._values1 ),
  _values2( toCopy._values2 ) {
}


tarch::la::Vector<4,int>::Vector (
  const Vector<4,int>& toCopy
):
  _values0( toCopy._values0 ),
  _values1( toCopy._values1 ),
  _values2( toCopy._values2 ),
  _values3( toCopy._values3 ) {
}


tarch::la::Vector<2,int>::Vector(
  const int& initialValue
):
  _values0(initialValue),
  _values1(initialValue)
{
}


tarch::la::Vector<3,int>::Vector(
  const int& initialValue
):
  _values0(initialValue),
  _values1(initialValue),
  _values2(initialValue)
{
}


tarch::la::Vector<4,int>::Vector(
  const int& initialValue
):
  _values0(initialValue),
  _values1(initialValue),
  _values2(initialValue),
  _values3(initialValue)
{
}


tarch::la::Vector<2,int>::Vector(
  const int& initialValue0,
  const int& initialValue1
): 
  _values0(initialValue0),
  _values1(initialValue1) {
}


tarch::la::Vector<3,int>::Vector(
  const int& initialValue0,
  const int& initialValue1,
  const int& initialValue2
):
  _values0(initialValue0),
  _values1(initialValue1),
  _values2(initialValue2) {
}


tarch::la::Vector<4,int>::Vector(
  const int& initialValue0,
  const int& initialValue1,
  const int& initialValue2,
  const int& initialValue3
):
  _values0(initialValue0),
  _values1(initialValue1),
  _values2(initialValue2),
  _values3(initialValue3) {
}


int tarch::la::Vector<2,int>::size() const {
  return 2;
}


int tarch::la::Vector<3,int>::size() const {
  return 3;
}


int tarch::la::Vector<4,int>::size() const {
  return 4;
}


const int& tarch::la::Vector<2,int>::operator[] ( int index ) const {
  assertion ( index >= 0 );
  assertion ( index < 2 );
  if (index==1) return _values1;
           else return _values0;
}


const int& tarch::la::Vector<3,int>::operator[] ( int index ) const {
  assertion ( index >= 0 );
  assertion ( index < 3 );
       if (index==2) return _values2;
  else if (index==1) return _values1;
                else return _values0;
}


const int& tarch::la::Vector<4,int>::operator[] ( int index ) const {
  assertion ( index >= 0 );
  assertion ( index < 4 );
       if (index==3) return _values3;
  else if (index==2) return _values2;
  else if (index==1) return _values1;
                else return _values0;
}


int& tarch::la::Vector<2,int>::operator[] ( int index ) {
  assertion ( index >= 0 );
  assertion ( index < 2 );
  if (index==1) return _values1;
           else return _values0;
}


int& tarch::la::Vector<3,int>::operator[] ( int index ) {
  assertion ( index >= 0 );
  assertion ( index < 3 );
       if (index==2) return _values2;
  else if (index==1) return _values1;
                else return _values0;
}


int& tarch::la::Vector<4,int>::operator[] ( int index ) {
  assertion ( index >= 0 );
  assertion ( index < 4 );
       if (index==3) return _values3;
  else if (index==2) return _values2;
  else if (index==1) return _values1;
                else return _values0;
}


const int& tarch::la::Vector<2,int>::operator() ( int index ) const {
  assertion ( index >= 0 );
  assertion ( index < 2 );
  if (index==1) return _values1;
           else return _values0;
}


const int& tarch::la::Vector<3,int>::operator() ( int index ) const {
  assertion ( index >= 0 );
  assertion ( index < 3 );
       if (index==2) return _values2;
  else if (index==1) return _values1;
                else return _values0;
}


const int& tarch::la::Vector<4,int>::operator() ( int index ) const {
  assertion ( index >= 0 );
  assertion ( index < 4 );
       if (index==3) return _values3;
  else if (index==2) return _values2;
  else if (index==1) return _values1;
                else return _values0;
}


int& tarch::la::Vector<2,int>::operator() ( int index ) {
  assertion ( index >= 0 );
  assertion ( index < 2 );
  if (index==1) return _values1;
           else return _values0;
}


int& tarch::la::Vector<3,int>::operator() ( int index ) {
  assertion ( index >= 0 );
  assertion ( index < 3 );
       if (index==2) return _values2;
  else if (index==1) return _values1;
                else return _values0;
}


int& tarch::la::Vector<4,int>::operator() ( int index ) {
  assertion ( index >= 0 );
  assertion ( index < 4 );
       if (index==3) return _values3;
  else if (index==2) return _values2;
  else if (index==1) return _values1;
                else return _values0;
}


std::string tarch::la::Vector<2,int>::toString() const {
  std::ostringstream os;
  os << "["  << _values0
     << "," << _values1
     << "]";
  return os.str();
}


std::string tarch::la::Vector<3,int>::toString() const {
  std::ostringstream os;
  os << "["  << _values0
     << "," << _values1
     << "," << _values2
     << "]";
  return os.str();
}


std::string tarch::la::Vector<4,int>::toString() const {
  std::ostringstream os;
  os << "["  << _values0
     << "," << _values1
     << "," << _values2
     << "," << _values3
     << "]";
  return os.str();
}

#endif
