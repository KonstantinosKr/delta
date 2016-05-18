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

#include <vector>
#include <limits>
#include <cfloat>
#include "algo.h"
#include "math.h"
    /**
     * penalty method for nonlinear constrained optimization of convex problem
     *
     * Penalty is the iterative Newton solver method that solves the optimization problem
     * of finding the distance between two triangles in 3D.
     *
     * This operation always runs a fixed number of Newton iterations.
     *
     * @author Konstantinos Krestenitis
     *
     * @param numberOfNewtonIterations  Number of Newton iterations to be ran.
     *            The routine always runs exactly this number of iterations no
     *            matter whether the iterations do converge or not.
     * @param error This is an out parameter where the iteration tells the
     *            invoking code what the final error has been.
     *
     * @param xCoordinatesOfTriangleA[3] x coordinates of triangle A
     * @param yCoordinatesOfTriangleA[3] y coordinates of triangle A
     * @param zCoordinatesOfTriangleA[3] z coordinates of triangle A
     * @param xCoordinatesOfTriangleB[3] x coordinates of triangle B
     * @param yCoordinatesOfTriangleB[3] y coordinates of triangle B
     * @param zCoordinatesOfTriangleB[3] z coordinates of triangle B
     * @param xPA x coordinate of Point P on triangle A that define closest distance between triangle A-B
     * @param xPA y coordinate of Point P on triangle A that define closest distance between triangle A-B
     * @param xPA z coordinate of Point P on triangle A that define closest distance between triangle A-B
     * @param xPA x coordinate of Point Q on triangle B that define closest distance between triangle B-A
     * @param xPA y coordinate of Point Q on triangle B that define closest distance between triangle B-A
     * @param xPA z coordinate of Point Q on triangle B that define closest distance between triangle B-A
     *
     */
    void penalty(
      double   xCoordinatesOfTriangleA[3],
      double   yCoordinatesOfTriangleA[3],
      double   zCoordinatesOfTriangleA[3],
      double   xCoordinatesOfTriangleB[3],
      double   yCoordinatesOfTriangleB[3],
      double   zCoordinatesOfTriangleB[3],
      double&  xPA,
      double&  yPA,
      double&  zPA,
      double&  xPB,
      double&  yPB,
      double&  zPB,
      int      numberOfNewtonIterations,
      double&  error
    );

