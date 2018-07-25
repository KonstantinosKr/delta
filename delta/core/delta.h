/*
 The MIT License (MIT)

 Copyright (c) 10 Mar 2017 Konstantinos Krestenitis

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#ifndef DELTA_CORE_DELTA_H_
#define DELTA_CORE_DELTA_H_

#include <map>
#include <vector>
#include <vector>
#include <iostream>
#include <delta/core/io/read.h>
#include <delta/geometry/material.h>
#include <delta/geometry/structure/Triangle.h>
#include <delta/world/structure/Object.h>

/*
#if GCC_VERSION >= 4.2
#  include <unordered_map>
#  define EXT std
#elif GCC_VERSION >= 4
#  include <tr1/unordered_map>
#  define EXT std
#else
#  include <ext/hash_map>
#  define unordered_map __gnu_cxx::hash_map
#  define EXT __gnu_cxx
#endif
*/


#if !defined(iREAL)
#define iREAL double
#endif

namespace delta {
	namespace core {
		class Delta;
	}
}

class delta::core::Delta
{
	private:
		//int _test;

	public:

    Delta();
    virtual ~Delta();

	//public:
	  //static std::vector< delta::geometry::Object> 	_objects;
};


#endif /* DELTA_CORE_DELTA_H_ */
