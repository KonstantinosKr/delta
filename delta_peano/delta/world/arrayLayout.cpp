/*
 * ArrayLayout3D.cpp
 *
 *  Created on: 19 Apr 2018
 *      Author: konstantinos
 */

#include <delta/world/arrayLayout.h>

namespace delta {
namespace world {

namespace layout {

  iREAL delta::world::layout::getDiscritization(
	 iREAL 	length,
	 int 		cuts)
  {
	  return length/cuts;
  }

  std::vector<std::array<iREAL, 3>> delta::world::layout::array1d(
	 iREAL 	position[3],
	 iREAL 	xAxisLength,
	 int 		partitions)
  {
	  std::vector<std::array<iREAL, 3>> array;
	  std::array<iREAL, 3> point = {0.0, 0.0, 0.0};

	  iREAL length = getDiscritization(xAxisLength, partitions);

	  point[0] = position[0];
	  point[1] = position[1];
	  point[2] = position[2];
	  array.push_back(point);

	  for(int i=0;i<partitions-1;i++)
	  {
		  point[0] += length;
		  array.push_back(point);
	  }
	  return array;
  }

  std::vector<std::array<iREAL, 3>> delta::world::layout::array2d(
	 iREAL 	position[3],
	 iREAL 	xyAxisLength,
	 int 		partitions)
  {
	  std::vector<std::array<iREAL, 3>> array;

	  iREAL length = getDiscritization(xyAxisLength, partitions);

	  for(int i=0;i<partitions;i++)
	  {
		  std::vector<std::array<iREAL, 3>> tmp = delta::world::layout::array1d(position, xyAxisLength, partitions);
		  for(std::vector<std::array<iREAL, 3>>::iterator j = tmp.begin(); j != tmp.end(); j++)
		  {
			  array.push_back(*j);
		  }
		  position[2] += length;
	  }

	  return array;
  }

  std::vector<std::array<iREAL, 3>> delta::world::layout::array3d(
	 iREAL 	position[3],
	 iREAL 	xyzAxisLength,
	 int 		partitions)
  {
	  std::vector<std::array<iREAL, 3>> array;

	  iREAL length = getDiscritization(xyzAxisLength, partitions);

   iREAL resetx = position[0];
	  iREAL resetz = position[2];

	  for(int i=0;i<partitions;i++)
	  {
		  std::vector<std::array<iREAL, 3>> tmp = delta::world::layout::array2d(position, xyzAxisLength, partitions);
		  for(std::vector<std::array<iREAL, 3>>::iterator j = tmp.begin(); j != tmp.end(); j++)
		  {
			  array.push_back(*j);
		  }
		  position[0] = resetx;
		  position[1] += length;
		  position[2] = resetz;
		  if(position[1] > 1.0)
			  break;
	  }

	  return array;
  }

  std::vector<std::array<iREAL, 3>> delta::world::layout::array3d(
	 iREAL 	position[3],
	 iREAL 	xyzAxisLength,
	 int 		partitionsXYZ,
	 iREAL 	yAxisLength,
	 int 		partitionsY)
  {
	  std::vector<std::array<iREAL, 3>> array;

	  iREAL lengthY =  getDiscritization(yAxisLength, partitionsY);

	  for(int i=0;i<partitionsY;i++)
	  {
		  std::vector<std::array<iREAL, 3>> tmp = delta::world::layout::array2d(position, xyzAxisLength, partitionsXYZ);
		  for(std::vector<std::array<iREAL, 3>>::iterator j = tmp.begin(); j != tmp.end(); j++)
		  {
			  array.push_back(*j);
		  }
		  position[1] += lengthY;
		  position[2] -= xyzAxisLength;
		  if(position[1] > 1.0)
			  break;
	  }

	  return array;
  }

  void delta::world::layout::collapseUniformGrid(
 	iREAL 								position[3],
 	std::vector<std::array<iREAL, 3>>& 	grid,
 	int 									xzcuts,
 	int 									ycuts,
 	iREAL 								elementWidth,
 	iREAL 								elementHeight,
 	iREAL 								epsilon)
 {
   int xAxisLoc = 0;
   int zAxisLoc = 0;
   int yAxisLoc = 0;

   elementWidth += epsilon*2;
   elementHeight += epsilon*2;

   iREAL dx = (grid[1][0] - grid[0][0]) - elementWidth;
   //iREAL dy = (grid[xzcuts*xzcuts][1] - grid[xzcuts*xzcuts-1][1]) - elementHeight;
   iREAL dz = dx;

   ////////////////////////////////////////
   //collapse from left to right on X AXIS
   ////////////////////////////////////////
   for(int i=0; i<grid.size(); i++)
   {
     if(xAxisLoc > 0)
     {
       //printf("%f\n", dx);
       grid[i][0] -= dx;
     }
     if(yAxisLoc > 0)
     {
       //printf("%i\n", yAxisLoc);
       //grid[i][1] -= dy;
     }
     if(zAxisLoc > 0)
     {
       grid[i][2] += dz;
     }

     xAxisLoc++;
     /////////////////////////////
     if(xAxisLoc >= xzcuts)
     {
       xAxisLoc = 0;
       zAxisLoc++;
     }
     if(zAxisLoc >= xzcuts)
     {
       zAxisLoc = 0;
     }
     if(xAxisLoc == 0 && zAxisLoc == 0)
     {
       yAxisLoc++;
     }
     //////////////////////////
   }

 }

  std::vector<std::array<iREAL, 3>> delta::world::layout::makeGridLayout(
     iREAL 	position[3],
     int 		xzcuts,
     int 		ycuts,
     iREAL 	gridxyLength)
 {
   return delta::world::layout::array3d(position, gridxyLength, xzcuts, gridxyLength, ycuts);
 }
}

} /* namespace world */
} /* namespace delta */
