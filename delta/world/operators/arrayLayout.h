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

#ifndef WORLD_ARRAYLAYOUT_H_
#define WORLD_ARRAYLAYOUT_H_

#include <vector>
#include <array>

namespace delta {
  namespace world {
	namespace layout {
		/*
		 *  Get Discretisation
		 *
		 *  Discretise length by cuts and
		 *  return sub cut lengths.
		 *
		 *  @param length
		 *  @param cuts
		 *  @returns iREAL
		 */
	 iREAL getDiscritization(
			   iREAL 	length,
			   int 		cuts);

		/*
		 *  Get One Dimensional Discretisation Positions
		 *
		 *  Discretise 1d length and return positions.
		 *
		 *  @param position 		: position of array
		 *  @param xAxisLength 	: length
		 *  @param partitions	: segmentations
		 *  @returns vector of positions
		 */
	 std::vector<std::array<iREAL, 3>> array1d(
		  iREAL position[3],
		  iREAL xAxisLength,
		  int 	partitions);

		/*
		 *  Get Two Dimensional Discretisation Positions
		 *
		 *  Discretise 2d length and return positions.
		 *
		 *  @param position 		: position of array
		 *  @param xAxisLength 	: length
		 *  @param partitions	: segmentations
		 *  @returns vector of positions
		 */
	 std::vector<std::array<iREAL, 3>> array2d(
		  iREAL position[3],
		  iREAL xyAxisLength,
		  int 	partitions);

		/*
		 *  Get Three Dimensional Discretisation Positions
		 *
		 *  Discretise 3d length and return positions.
		 *
		 *  @param position 	: position of array
		 *  @param xAxisLength 	: length
		 *  @param partitions	: segmentations
		 *  @returns vector of positions
		 */
	 std::vector<std::array<iREAL, 3>> array3d(
		  iREAL position[3],
		  iREAL xyzAxisLength,
		  int 	partitions);

		/*
		 *  Get Three Dimensional Discretisation Positions
		 *
		 *  Discretise 3d length and return positions.
		 *
		 *  @param position 			: position of array
		 *  @param xyzAxisLength 	: length along all axis
		 *  @param partitionsXYZ		: segmentations on all axis
		 *  @param yAxisLength 		: length along y axis
		 *  @param partitionsY		: segmentations on y axis
		 *  @returns vector of positions
		 */
	 std::vector<std::array<iREAL, 3>> array3d(
		  iREAL position[3],
		  iREAL xyzAxisLength,
		  int 	partitionsXYZ,
		  iREAL yAxisLength,
		  int 	partitionsY);


	  /*
	   *  Collapse Grid
	   *
	   *  Stitch grid elements so that they are adjucent.
	   *
	   *  @param position			: position of the layout
	   *  @param grid 				: grid layout
	   *  @param xzcuts 			: number of xz cuts
	   *  @param ycuts				: number of y cuts
	   *  @param elementWidth 		: width of the elements
	   *  @param elementHeight		: height of the elements
	   *  @param epsilon			: epsilon margin
	   *  @returns void
	   */
  void collapseUniformGrid(
	  iREAL 								position[3],
	  std::vector<std::array<iREAL, 3>>& 	grid,
	  int 									xzcuts,
	  int 									ycuts,
	  iREAL 								elementWidth,
	  iREAL 								elementHeight,
	  iREAL 								epsilon);

	  /*
	   *  Make grid layout
	   *
	   *  Setup grid for grid deployment layout of particles
	   *
	   *  @param position				: position of the layout
	   *  @param xzcuts 				: number of xz cuts
	   *  @param ycuts					: number of y cuts
	   *  @param gridxyLength			: grid xyz length
	   *  @returns void
	   */
  std::vector<std::array<iREAL, 3>> makeGridLayout(
	  iREAL 	position[3],
	  int 		xzcuts,
	  int 		ycuts,
	  iREAL 	gridxyLength);
	}
  } /* namespace world */
} /* namespace delta */

#endif /* WORLD_ARRAYLAYOUT_H_ */
