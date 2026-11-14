/*
 * cube.cpp
 *
 *  Created on: 28 Aug 2018
 *      Author: konstantinos
 */

#include "box.h"

bool delta::contact::detection::box(
	std::array<iREAL, 6> A,
	iREAL epsilonA,
	std::array<iREAL, 6> B,
	iREAL epsilonB)
{
  ////////////////////////

  iREAL xA_min = A[0];
  iREAL yA_min = A[1];
  iREAL zA_min = A[2];

  iREAL xA_max = A[3];
  iREAL yA_max = A[4];
  iREAL zA_max = A[5];

  iREAL x1_A [3] = {xA_min, yA_min, zA_min};
  iREAL x2_A [3] = {xA_max, yA_min, zA_min};
  iREAL x3_A [3] = {xA_min, yA_max, zA_min};
  iREAL x4_A [3] = {xA_max, yA_max, zA_min};

  iREAL x5_A [3] = {xA_min, yA_min, zA_max};
  iREAL x6_A [3] = {xA_max, yA_min, zA_max};
  iREAL x7_A [3] = {xA_min, yA_max, zA_max};
  iREAL x8_A [3] = {xA_max, yA_max, zA_max};

  ////////////////////

  iREAL xB_min = B[0];
  iREAL yB_min = B[1];
  iREAL zB_min = B[2];

  iREAL xB_max = B[3];
  iREAL yB_max = B[4];
  iREAL zB_max = B[5];

  iREAL x1_B [3] = {xB_min, yB_min, zB_min};
  iREAL x2_B [3] = {xB_max, yB_min, zB_min};
  iREAL x3_B [3] = {xB_min, yB_max, zB_min};
  iREAL x4_B [3] = {xB_max, yB_max, zB_min};

  iREAL x5_B [3] = {xB_min, yB_min, zB_max};
  iREAL x6_B [3] = {xB_max, yB_min, zB_max};
  iREAL x7_B [3] = {xB_min, yB_max, zB_max};
  iREAL x8_B [3] = {xB_max, yB_max, zB_max};

  //////////////////////////

  if(isInBox(x1_A, B))
	return true;
  if(isInBox(x2_A, B))
	return true;
  if(isInBox(x3_A, B))
	return true;
  if(isInBox(x4_A, B))
	return true;

  if(isInBox(x5_A, B))
	return true;
  if(isInBox(x6_A, B))
	return true;
  if(isInBox(x7_A, B))
	return true;
  if(isInBox(x8_A, B))
	return true;

  return false;
}

bool delta::contact::detection::isInBox(
	iREAL x[3],
	std::array<iREAL,6> bbox)
{
  bool axisXinside = false;
  bool axisYinside = false;
  bool axisZinside = false;

  if(x[0] >= bbox[0] && x[0] <= bbox[3])
  {
	axisXinside = true;
  } else
  {
	return false;
  }

  if(x[1] >= bbox[1] && x[1] <= bbox[4])
  {
	axisYinside = true;
  } else
  {
	return false;
  }

  if(x[2] >= bbox[2] && x[2] <= bbox[5])
  {
	axisZinside = true;
  } else
  {
	return false;
  }
  return true;
}
