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
#include <array>
#include <string>
#include <delta/geometry/material.h>
#include <delta/geometry/properties.h>
#include <delta/geometry/granulates.h>
#include <delta/geometry/cubes.h>


namespace delta {
  namespace world {
    class object;
  }
}

class delta::world::object
{
  public:

    object(
        std::string                   component,
        int                           particleID,
        std::array<double, 3>         centre,
        std::array<double, 3>         linearVelocity,
        std::array<double, 3>         angularVelocity,
        delta::geometry::material::MaterialType material,
        bool                          isObstacle,
        bool                          isFriction
        );

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
    std::vector<double> getxCoordinates();
    std::vector<double> getyCoordinates();
    std::vector<double> getzCoordinates();
    int getParticleID();
    std::array<double, 3> getCentre();
    double getRad();
    double getMass();

    delta::geometry::material::MaterialType getMaterial();
    bool getIsObstacle();
    bool getIsFriction();

    std::array<double, 9> getInertia();
    std::array<double, 9> getInverse();
    std::array<double, 3> getxyzDimensions();
    std::array<double, 3> getCentreOfMass();

    std::array<double, 3> getLinearVelocity();
    std::array<double, 3> getAngularVelocity();

    virtual ~object();

    //std::vector<double> getXcoordinates();
    //std::vector<double> getYcoordinates();
    //std::vector<double> getZcoordinates();

  private:
    std::string           _component;
    int                   _particleID;
    std::array<double, 3> _centre;
    double                _rad;
    double                _mass;

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

    double                _wx;
    double                _wy;
    double                _wz;

    double                _rx;
    double                _ry;
    double                _rz;
};
