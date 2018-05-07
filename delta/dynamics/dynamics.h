/*
 The MIT License (MIT)

 Copyright (c) 20 Sep 2016 Konstantinos Krestenitis

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

namespace delta {
  namespace dynamics {

	/*
	* Update Angular Velocity
	*
	* Returns the updated angular velocity.
	*
	* @param refAngular is the referential angular velocity
	* @param rotation is the rotation matrix
	* @param inertia is inertia
	* @param inverse is the inverse inertia
	* @param mass is the mass of the particle
	* @param torque is the applied torque
	* @param step is the step size
	* @return void
	*/
    void updateAngular(
        iREAL *refAngular,
        iREAL *rotation,
        iREAL *inertia,
        iREAL *inverse,
        iREAL mass,
        iREAL *torque,
        iREAL step);

	/*
	* Update Rotational Matrix
	*
	* Returns the updated rotational matrix.
	*
	* @param angular is the angular velocity
	* @param refAngular is the referential angular velocity
	* @param rotation is the rotational matrix
	* @param step is the step size
	* @return void
	*/
    void updateRotationMatrix(
        iREAL *angular,
        iREAL *refAngular,
        iREAL *rotation,
        iREAL step);

	/*
	* Update Vertices
	*
	* Returns the updated vertices of the mesh according to the new orientation matrix.
	*
	* @param x is x coordinates of the mesh
	* @param y is y coordinates of the mesh
	* @param z is z coordinates of the mesh
	* @param refx is referential x coordinates of the mesh
	* @param refy is referential y coordinates of the mesh
	* @param refz is referential z coordinates of the mesh
	* @param rotation is the rotational matrix
	* @param position is the center of mass
	* @param refposition is the referential center of mass
	* @return void
	*/
    void updateVertices(
        iREAL* x,
        iREAL* y,
        iREAL* z,
        iREAL* refx,
        iREAL* refy,
        iREAL* refz,
        iREAL* rotation,
        iREAL* position,
        iREAL* refposition);
  }
}
