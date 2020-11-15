/*
 * MultiBodyObject.cpp
 *
 *  Created on: 6 Jul 2018
 *      Author: konstantinos
 */

#include "../structure/MultiBodyObject.h"

delta::geometry::structure::MultiBodyObject::MultiBodyObject(
std::vector<delta::world::structure::Object> parts,
std::string                   				component,
int                           				particleID,
std::array<iREAL, 3>         				centre,
delta::geometry::material::MaterialType 		material,
bool                          				isObstacle,
bool                          				isFriction,
bool                          				isConvex,
iREAL 										epsilon,
std::array<iREAL, 3> 						linear,
std::array<iREAL, 3> 						angular
) {

}

delta::geometry::structure::MultiBodyObject::~MultiBodyObject() {

}

