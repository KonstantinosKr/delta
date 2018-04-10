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

#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <array>
#include <string>
#include <delta/geometry/mesh/Mesh.h>
#include <delta/geometry/material.h>

namespace delta {
  namespace geometry {
	class Object;
  }
}

class delta::geometry::Object
{
  public:
	Object();

	Object(
	    std::string                   			component,
	    int                           			particleID,
	    std::array<iREAL, 3>         			centre,
	    delta::geometry::material::MaterialType 	material,
	    bool                          			isObstacle,
	    bool                          			isFriction,
		iREAL 									epsilon,
		std::array<iREAL, 3> linear,
		std::array<iREAL, 3> angular
		);

    void generateSphere(iREAL  rad);

    void generateMesh(
			iREAL wx,
			iREAL wy,
			iREAL wz,
			iREAL rx,
			iREAL ry,
			iREAL rz,
			int mesh,
			iREAL rad
			);

    void 	setParticleID(int id);
    void 	setCentre(iREAL centre[3]);
    void 	setRad(iREAL rad);
    void 	setMass(iREAL mass);
    void 	setEpsilon(iREAL epsilon);
    void 	setInertia(iREAL inertia[9]);
    void 	setInverse(iREAL inverse[9]);
    void 	setCentreOfMass(iREAL centreOfMass[3]);
    void 	setLinearVelocity(std::array<iREAL, 3>  linearVelocity);
    void 	setAngularVelocity(std::array<iREAL, 3>  angularVelocity);
    void 	setMesh(delta::geometry::mesh::Mesh& mesh);
    void 	setMesh(
    			  std::vector<iREAL> xCoordinates,
			  std::vector<iREAL> yCoordinates,
			  std::vector<iREAL> zCoordinates);

    std::string getComponent();
    int 		getParticleID();
    iREAL 	getRad();
	iREAL 	getDiameter();
    iREAL 	getMass();
    iREAL 	getEpsilon();
	iREAL 	getHaloDiameter();
	int 		getNumberOfTriangles();
	int 		getGlobalParticleId();
	int 		getLocalParticleId();
    bool 	getIsObstacle();
    bool 	getIsFriction();

    iREAL 	getMinX();
    iREAL 	getMaxX();
    iREAL 	getMinY();
    iREAL 	getMaxY();
    iREAL 	getMinZ();
    iREAL 	getMaxZ();
    iREAL 	computeVolume();


    std::vector<iREAL> 		getxCoordinates();
    std::vector<iREAL> 		getyCoordinates();
    std::vector<iREAL> 		getzCoordinates();
    std::array<iREAL, 3> 	getCentre();
    std::array<iREAL, 9> 	getInertia();
    std::array<iREAL, 9> 	getInverse();
    std::array<iREAL, 3> 	getxyzDimensions();
    std::array<iREAL, 3> 	getCentreOfMass();
    std::array<iREAL, 3> 	getLinearVelocity();
    std::array<iREAL, 3> 	getAngularVelocity();

    delta::geometry::material::MaterialType 	getMaterial();
    delta::geometry::mesh::Mesh 				getMesh();

	void computeInertia(
			delta::geometry::material::MaterialType material,
			iREAL& mass,
			iREAL center[3],
			iREAL inertia[9]
			);

	iREAL computeMass(
			delta::geometry::material::MaterialType material);

	void computeInverseInertia(
			iREAL inertia[9],
			iREAL inverse[9],
			bool isObject
			);

    virtual ~Object();

  private:
    std::string           	_component;
    int                   	_globalParticleID;
    int                  	_localParticleID;
    std::array<iREAL, 3> 	_centre;
    iREAL                	_rad;

    iREAL				 	_haloDiameter;
    iREAL				 	_diameter;
    iREAL               		_mass;
    iREAL 				 	_epsilon;

	delta::geometry::mesh::Mesh	*_mesh;

    std::vector<iREAL>   	_xCoordinates;
    std::vector<iREAL>   	_yCoordinates;
    std::vector<iREAL>   	_zCoordinates;

    std::array<iREAL, 3> 	_linearVelocity;
    std::array<iREAL, 3> 	_angularVelocity;

    delta::geometry::material::MaterialType _material;

    bool                  	_isObstacle;
    bool                  	_isFriction;

    std::array<iREAL, 3> 	_xyzDimensions;
    std::array<iREAL, 3> 	_centreOfMass;
    std::array<iREAL, 9> 	_inertia;
    std::array<iREAL, 9> 	_inverse;

    //dimensions
    iREAL                	_wx;
    iREAL                	_wy;
    iREAL                	_wz;

    //rotational configuration 0-1
    iREAL                	_rx;
    iREAL                	_ry;
    iREAL                	_rz;

    iREAL simplex_J (iREAL *a, iREAL *b, iREAL *c, iREAL *d);
};

#endif
