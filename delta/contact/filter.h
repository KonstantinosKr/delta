/*
 The MIT License (MIT)

 Copyright (c) 2015 Konstantinos Krestenitis

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

#ifndef _DELTA_COLLISION_FILTER_H_
#define _DELTA_COLLISION_FILTER_H_

#include "contactpoint.h"
#include <vector>
#include <cmath>

namespace delta {
  namespace contact {
    /**
     * Run through all the contactPoints and remove all those that are closer
     * to each other than hMin.
     */
    void filterOldContacts(
        std::vector<contactpoint>& dataStoredContactPoints,
        std::vector<contactpoint>& newContactPoints,
        iREAL rA,
        iREAL rB);

    void filterOldContacts(
        std::vector<contactpoint>& dataStoredContactPoints,
        std::vector<contactpoint>& newContactPoints);

    void filterOldContacts(
        std::vector<contactpoint>& dataStoredContactPoints,
        std::vector<contactpoint>& newContactPoints,
        iREAL hMin);

    void filterNewContacts(
        std::vector<contactpoint>& newContactPoints);
  }
}

#endif
