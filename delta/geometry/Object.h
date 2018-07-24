/*
 The MIT License (MIT)

 Copyright (c) 2018 Konstantinos Krestenitis

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

#ifndef GEOMETRY_OBJECT_H
#define GEOMETRY_OBJECT_H

#include <vector>
#include <array>
#include <string>
#include <delta/geometry/material.h>
#include "delta/geometry/structures/Mesh.h"

namespace delta {
  namespace geometry {
	class Object;
  }
}

class delta::geometry::Object
{
  public:
	Object();

	/*
	 * Mesh object with mesh unspecified
	 */
	Object(
	  std::string                   				component,
	  int                           				particleID,
	  std::array<iREAL, 3>         				centre,
	  delta::geometry::material::MaterialType 	material,
	  bool                          				isObstacle,
	  bool                          				isFriction,
	  bool                          				isConvex,
	  iREAL 										epsilon,
	  std::array<iREAL, 3> linear,
	  std::array<iREAL, 3> angular
	);

	/*
	 * Mesh object with mesh specified
	 */
	Object(
	  std::string                   				component,
	  int                           				particleID,
	  delta::geometry::mesh::Mesh*				mesh,
	  std::array<iREAL, 3>         				centre,
	  delta::geometry::material::MaterialType 	material,
	  bool                          				isObstacle,
	  bool                          				isFriction,
	  bool                          				isConvex,
	  iREAL 										epsilon,
	  std::array<iREAL, 3> 						linear,
	  std::array<iREAL, 3> 						angular
	);

	/*
	 * Sphere object
	 */
	Object(
	  std::string                   				component,
	  iREAL										rad,
	  int                           				particleID,
	  std::array<iREAL, 3>         				centre,
	  delta::geometry::material::MaterialType 	material,
	  bool                          				isObstacle,
	  bool                          				isFriction,
	  bool                          				isConvex,
	  iREAL 										epsilon,
	  std::array<iREAL, 3> 						linear,
	  std::array<iREAL, 3> 						angular
	);

	/*
	 *  Set Particle ID
	 *
	 *  Set particle id using id value.
	 *	This modifies local data.
	 *
	 *  @param id
	 *  @returns void
	 */
    void setParticleID(int id);

	/*
	 *  Set Center
	 *
	 *  Set center of geometry.
	 *	This modifies local data.
	 *
	 *  @param center
	 *  @returns void
	 */
    void setCentre(
    		  iREAL centre[3]);

	/*
	 *  Set Radius
	 *
	 *  Set radius of geometry.
	 *	This modifies local data.
	 *
	 *  @param rad
	 *  @returns void
	 */
    void setRad(
    		  iREAL rad);

	/*
	 *  Set Mass
	 *
	 *  Set mass of geometry.
	 *	This modifies local data.
	 *
	 *  @param mass
	 *  @returns void
	 */
    void setMass(
    		  iREAL mass);

	/*
	 *  Set Epsilon
	 *
	 *  Set epsilon margin of geometry.
	 *	This modifies local data.
	 *
	 *  @param epsilon
	 *  @returns void
	 */
    void setEpsilon(
    		  iREAL epsilon);

	/*
	 *  Set Inertia
	 *
	 *  Set inertia geometry.
	 *	This modifies local data.
	 *
	 *  @param inertia
	 *  @returns void
	 */
    void setInertia(
    		  iREAL inertia[9]);

	/*
	 *  Set Inverse Inertia
	 *
	 *  Set inverse inertia geometry.
	 *	This modifies local data.
	 *
	 *  @param inverse
	 *  @returns void
	 */
    void setInverse(
    		  iREAL inverse[9]);

	/*
	 *  Set Center of Mass
	 *
	 *  Set center of mass of geometry.
	 *	This modifies local data.
	 *
	 *  @param centerOfMass
	 *  @returns void
	 */
    void setCentreOfMass(
    		  iREAL	centreOfMass[3]);

	/*
	 *  Set Linear Velocity
	 *
	 *  Set linear velocity of body.
	 *	This modifies local data.
	 *
	 *  @param linearVelocity
	 *  @returns void
	 */
    void setLinearVelocity(
    		  std::array<iREAL, 3>  linearVelocity);

	/*
	 *  Set Angular Velocity
	 *
	 *  Set angular velocity of body.
	 *	This modifies local data.
	 *
	 *  @param angularVelocity
	 *  @returns void
	 */
    void setAngularVelocity(
    		  std::array<iREAL, 3>  angularVelocity);

	/*
	 *  Set Mesh
	 *
	 *  Set mesh of body.
	 *	This modifies local data.
	 *
	 *  @param mesh
	 *  @returns void
	 */
    void setMesh(
    		  delta::geometry::mesh::Mesh& mesh);

	/*
	 *  Set Mesh
	 *
	 *  Set mesh of body using SoA data structure.
	 *	This modifies local data.
	 *
	 *  @param xCoordinates
  	 *  @param yCoordinates
   	 *  @param zCoordinates
	 *  @returns void
	 */
    void setMesh(
		  std::vector<iREAL> xCoordinates,
		  std::vector<iREAL> yCoordinates,
		  std::vector<iREAL> zCoordinates);

	/*
	 *  Get Component Id
	 *
	 *  Get id of component.
	 *
	 *  @param none
	 *  @returns string
	 */
    std::string getComponent();

	/*
	 *  Get Radius
	 *
	 *  Get rad of body.
	 *
	 *  @param none
	 *  @returns iREAL
	 */
    iREAL getRad();

	/*
	 *  Get Diameter
	 *
	 *  Get diameter of body.
	 *
	 *  @param none
	 *  @returns iREAL
	 */
	iREAL getDiameter();

	/*
	 *  Get Mass
	 *
	 *  Get mass of body.
	 *
	 *  @param none
	 *  @returns iREAL
	 */
    iREAL getMass();

	/*
	 *  Get Epsilon
	 *
	 *  Get epsilon of body.
	 *
	 *  @param none
	 *  @returns iREAL
	 */
    iREAL getEpsilon();

	/*
	 *  Get Halo Diameter
	 *
	 *  Get halo diameter of body.
	 *
	 *  @param none
	 *  @returns iREAL
	 */
	iREAL getHaloDiameter();

	/*
	 *  Get Number of Triangles
	 *
	 *  Get number of triangles of body.
	 *
	 *  @param none
	 *  @returns iREAL
	 */
	int 	getNumberOfTriangles();

	/*
	 *  Get Global Particle ID
	 *
	 *  Gets global particle id of body.
	 *
	 *  @param none
	 *  @returns int
	 */
	int 	getGlobalParticleId();

	/*
	 *  Get Local Particle ID
	 *
	 *  Gets local particle id of body.
	 *
	 *  @param none
	 *  @returns int
	 */
	int 	getLocalParticleId();

	/*
	 *  Get Is Obstacle
	 *
	 *  Gets bool on whether body is obstacle.
	 *
	 *  @param none
	 *  @returns bool
	 */
    bool getIsObstacle();

	/*
	 *  Get Is Friction
	 *
	 *  Gets bool on whether body applies friction.
	 *
	 *  @param none
	 *  @returns bool
	 */
    bool getIsFriction();

	/*
	 *  Get Volume
	 *
	 *  Gets volume of object
	 *
	 *  @param none
	 *  @returns iREAL
	 */
    iREAL computeVolume();

	/*
	 *  Get Center
	 *
	 *  Gets center of geometry of object
	 *
	 *  @param none
	 *  @returns vertex
	 */
    std::array<iREAL, 3> getCentre();

	/*
	 *  Get Inertia
	 *
	 *  Gets inertia of object
	 *
	 *  @param none
	 *  @returns inertia
	 */
    std::array<iREAL, 9> getInertia();

	/*
	 *  Get Inverse Inertia
	 *
	 *  Gets inverse inertia of object
	 *
	 *  @param none
	 *  @returns inverse
	 */
    std::array<iREAL, 9> getInverse();

	/*
	 *  Get Center of Mass
	 *
	 *  Gets center of mass of object
	 *
	 *  @param none
	 *  @returns vertex
	 */
    std::array<iREAL, 3> getCentreOfMass();

	/*
	 *  Get Linear Velocity
	 *
	 *  Gets linear velocity of object
	 *
	 *  @param none
	 *  @returns velocity
	 */
    std::array<iREAL, 3> getLinearVelocity();

	/*
	 *  Get Angular Velocity
	 *
	 *  Gets angular velocity of object
	 *
	 *  @param none
	 *  @returns velocity
	 */
    std::array<iREAL, 3> getAngularVelocity();

	/*
	 *  Get Material
	 *
	 *  Gets material of object
	 *
	 *  @param none
	 *  @returns material
	 */
    delta::geometry::material::MaterialType getMaterial();

	/*
	 *  Get Mesh
	 *
	 *  Gets mesh of object
	 *
	 *  @param none
	 *  @returns mesh
	 */
    delta::geometry::mesh::Mesh 	getMesh();

	/*
	 *  Compute Inertia
	 *
	 *  Computes inertia of object
	 *	This modifies local data.
	 *
	 *
	 *  @param none
	 *  @returns inertia
	 */
	void computeInertia(
			delta::geometry::material::MaterialType material,
			iREAL& mass,
			iREAL center[3],
			iREAL inertia[9]);

	/*
	 *  Compute Inverse Inertia
	 *
	 *  Computes inverse inertia of object
	 *	This modifies local data.
	 *
	 *
	 *  @param none
	 *  @returns inverse
	 */
	void computeInverseInertia(
			iREAL inertia[9],
			iREAL inverse[9],
			bool isObject);

	/*
	 *  Compute Mass
	 *
	 *  Computes mass of object
	 *	This modifies local data.
	 *
	 *
	 *  @param none
	 *  @returns mass
	 */
	iREAL computeMass(
			delta::geometry::material::MaterialType material);

	bool getIsConvex();

    virtual ~Object();

    std::array<iREAL, 3> 	_linearVelocity;
    std::array<iREAL, 3> 	_angularVelocity;

    std::array<iREAL, 3> 	_centre;
    std::array<iREAL, 3> 	_centreOfMass;

  private:
    std::string           	_component;
    int                   	_globalParticleID;
    int                  	_localParticleID;

    iREAL                	_rad;
    iREAL				 	_haloDiameter;
    iREAL				 	_diameter;
    iREAL               		_mass;
    iREAL 				 	_epsilon;

	delta::geometry::mesh::Mesh* _mesh;

    delta::geometry::material::MaterialType _material;

    bool                  	_isObstacle;
    bool                  	_isFriction;
    bool                  	_isConvex;

    std::array<iREAL, 9> 	_inertia;
    std::array<iREAL, 9> 	_inverse;

    //dimensions
    iREAL                	_wx;
    iREAL                	_wy;
    iREAL                	_wz;
};

#endif
