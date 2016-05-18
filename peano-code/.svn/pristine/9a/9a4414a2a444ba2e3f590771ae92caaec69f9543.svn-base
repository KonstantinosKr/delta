#ifndef _PEANO_HEAP_COMPRESSED_FLOATING_POINT_NUMBERS_H_
#define _PEANO_HEAP_COMPRESSED_FLOATING_POINT_NUMBERS_H_


namespace peano {
  namespace heap {
    /**
     * Takes a double and returns the exponent and the mantissa.
     *
     * Is just a wrapper around C++'s std::ldexp.
     */
    template <class T>
    void decompose(
      const double& value,
      char&         exponent,
      T&            mantissa
    );

    /**
     * Decompose floating point value
     *
     * Decomposes the input value into eight different variants with
     * 1,2,3,... bytes (chars) per mantissa. It also gives the errors.
     *
     * You may want to cast one of the integers into a sequence of chars
     * or other data types. While the array mantissa comprises long ints,
     * only the first kth bytes are used in the k-1th entry.
     *
     * Negative mantissa pose a challenge to the byte compression. Due to the
     * integer codes, we may not just hold negative values in the long ints.
     * -1 has all bits set, e.g. So if someone later only stores the minor k
     * bytes of mantissa, this will fail. So we restrict to unsigned values
     * first as we extract the sign and then multiply the biggest byte with
     * the sign in the end. This implies that the biggest byte of each
     * mantissa entry actually holds only seven digits.
     *
     * Please note that there are two compose() operations, i.e. two
     * counterparts of decompose(). Ensure you use the right one.
     *
     * @return exponent Sequence of exponents belonging to the eight decompositions.
     * @return mantissa Sequence of mantissas belonging to the eight decompositions.
     * @return error    Errors associated to the decompositions. They are all positive values.
     *
     * !!! Bug/interoperability
     *
     * This operation works if and only if your system maps a long int onto a 64 bit value.
     *
     * We noticed that this operation yields invalid results with GCC if we
     * switch on optimisation. It works fine with ICC and any optimisation
     * level. We thus make the code translate with -O0 locally if ICC is not
     * used.
     */
    void decomposeIntoEightVariants(
      double        value,
      char          exponent[8],
      long int      mantissa[8],
      double        error[8]
    );

    /**
     * @see decomposeIntoEightVariants
     */
    void decomposeIntoFourVariants(
      double        value,
      char          exponent[4],
      int           mantissa[4],
      double        error[4]
    );

    /**
     * Counterpart of decompose( const double&, char& , T& )
     */
    template <class T>
    double compose(
      const char&  exponent,
      const T&     mantissa
    );

    /**
     * Counterpart of decompose( double, char[8], long int[8], double[8] ).
     *
     * Please note that bytesUsed is basically which entry of the decompose
     * return arrays you used. However, it is increased by one, i.e. if you
     * pass 1, the operation assumes that you've taken the entry exponent[0]
     * of decompose, e.g.
     */
    double compose(
      char         exponent,
      long int     mantissa,
      int          bytesUsed
    );
  }
}


#include "peano/heap/CompressedFloatingPointNumbers.cpph"

#endif
