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
#include <stdio.h>
#include <delta/geometry/material.h>
#include <delta/geometry/mesh/mesh.h>
#include "delta/core/algo.h"

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
	    std::array<double, 3>         			centre,
	    delta::geometry::material::MaterialType 	material,
	    bool                          			isObstacle,
	    bool                          			isFriction,
		iREAL 									epsilon,
		std::array<double, 3> linear,
		std::array<double, 3> angular);

    void generateSphere(double  rad);

    void generateMesh(
        double wx,
        double wy,
        double wz,
        double rx,
        double ry,
        double rz,
        int mesh,
        double rad);

    std::string getComponent();
    void setParticleID(int id);

    std::vector<double> getxCoordinates();
    std::vector<double> getyCoordinates();
    std::vector<double> getzCoordinates();

    int getParticleID();

    std::array<double, 3> getCentre();
    void setCentre(double centre[3]);
    double getRad();
    void setRad(double rad);

    double getMass();
    void setMass(double mass);

    double getEpsilon();
    void setEpsilon(double epsilon);

    delta::geometry::material::MaterialType getMaterial();
    bool getIsObstacle();
    bool getIsFriction();

    std::array<double, 9> getInertia();
    void setInertia(double inertia[9]);

    std::array<double, 9> getInverse();
    void setInverse(double inverse[9]);

    std::array<double, 3> getxyzDimensions();
    std::array<double, 3> getCentreOfMass();
    void setCentreOfMass(double centreOfMass[3]);

    void setLinearVelocity(std::array<double, 3>  linearVelocity);
    void setAngularVelocity(std::array<double, 3>  angularVelocity);

    std::array<double, 3> getLinearVelocity();
    std::array<double, 3> getAngularVelocity();

    delta::geometry::mesh::Mesh getMesh();
    void setMesh(
        std::vector<double> xCoordinates,
        std::vector<double> yCoordinates,
        std::vector<double> zCoordinates);
    void setMesh(delta::geometry::mesh::Mesh& mesh);

    double getMinX();
    double getMaxX();

    double getMinY();
    double getMaxY();

    double getMinZ();
    double getMaxZ();

    iREAL computeVolume();

	void computeInertia(
		delta::geometry::material::MaterialType material,
		iREAL& mass,
		iREAL center[3],
		iREAL inertia[9]);

	iREAL computeMass(
	    delta::geometry::material::MaterialType material);

	void computeInverseInertia(
	    iREAL inertia[9],
	    iREAL inverse[9],
	    bool isObject);

    virtual ~Object();

  private:
    std::string           _component;
    int                   _particleID;
    std::array<double, 3> _centre;
    double                _rad;
    double                _mass;
    double 				 _epsilon;

	delta::geometry::mesh::Mesh	*_mesh;

    std::vector<double>   _xCoordinates;
    std::vector<double>   _yCoordinates;
    std::vector<double>   _zCoordinates;

    std::array<double, 3> _linearVelocity;
    std::array<double, 3> _angularVelocity;

    delta::geometry::material::MaterialType _material;

    bool                  _isObstacle;
    bool                  _isFriction;

    std::array<double, 3> _xyzDimensions;
    std::array<double, 3> _centreOfMass;
    std::array<double, 9> _inertia;
    std::array<double, 9> _inverse;

    //dimensions
    double                _wx;
    double                _wy;
    double                _wz;

    //rotational configuration 0-1
    double                _rx;
    double                _ry;
    double                _rz;

    iREAL simplex_J (iREAL *a, iREAL *b, iREAL *c, iREAL *d);
};

#endif
