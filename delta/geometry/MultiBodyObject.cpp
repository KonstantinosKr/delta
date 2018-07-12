/*
 * MultiBodyObject.cpp
 *
 *  Created on: 6 Jul 2018
 *      Author: konstantinos
 */

#include "MultiBodyObject.h"

delta::geometry::MultiBodyObject::MultiBodyObject(
std::vector<Object> parts,
std::string                   			component,
int                           			particleID,
std::array<iREAL, 3>         			centre,
delta::geometry::material::MaterialType 	material,
bool                          			isObstacle,
bool                          			isFriction,
bool                          			isConvex,
iREAL 									epsilon,
std::array<iREAL, 3> 					linear,
std::array<iREAL, 3> 					angular
) {

}

delta::geometry::MultiBodyObject::~MultiBodyObject() {

}

