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
#include <delta/core/read.h>
#include <delta/geometry/Object.h>
#include <delta/geometry/mesh/Triangle.h>
#include <delta/geometry/material.h>

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
