/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Tomasz Koziara
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * err.c: basic error handling
 */

#include "err.h"

namespace delta {
  namespace hull {
  ERRSTACK *__errstack__ = NULL; /* global error context */

  short WARNINGS_ENABLED = 1; /* warnings flag */

  /* get error string */
  char* errstring (int error)
  {
	switch (error)
	{
	  case ERR_OUT_OF_MEMORY: return "Out of memory";
	  case ERR_FILE_OPEN: return "File open failed";
	  case ERR_FILE_READ: return "File read failed";
	  case ERR_FILE_WRITE: return "File write failed";
	  case ERR_FILE_CLOSE: return "File close failed";
	  case ERR_FILE_EMPTY: return "File empty";
	  case ERR_FILE_FORMAT: return "Invalid file format";
	  case ERR_NOT_IMPLEMENTED: return "Not implemented";
	  case ERR_TMS_INTEGRATE_CONSTANT: return "Cannot integrate constant time series (no limits)";
	  case ERR_TMS_TIME_NOT_INCREASED: return "Time series time instants must be increasing";
	  case ERR_PBF_INDEX_FILE_CORRUPTED: return "PBF index file corrupted";
	  case ERR_PBF_OUTPUT_TIME_DECREASED: return "PBF output time decreased";
	  case ERR_PBF_WRITE: return "PBF write error";
	  case ERR_PBF_READ: return "PBF read error";
	  case ERR_MTX_LU_FACTOR: return  "LU factorisation failed";
	  case ERR_MTX_CHOL_FACTOR: return  "Cholesky factorisation failed";
	  case ERR_MTX_CHOL_SOLVE: return  "Cholesky solution failed";
	  case ERR_MTX_MATRIX_INVERT: return  "Matrix inversion failed";
	  case ERR_MTX_EIGEN_DECOMP: return  "Eigen decomposition failed";
	  case ERR_MTX_KIND: return "Invalid matrix kind";
	  case ERR_MTX_IFAC: return "Invalid operation involving a factorised sparse inverse matrix";
	  case ERR_MSH_UNSUPPORTED_ELEMENT: return "An element has an unsupported type";
	  case ERR_MSH_UNSUPPORTED_FACE: return "An element face has an unsupported type";
	  case ERR_MSH_ELEMENT_WITH_NODE: return "An element with a given node was not found";
	  case ERR_CVX_ZERO_NORMAL: return "Zero face normal generated during a CONVEX face update";
	  case ERR_LDY_EIGEN_DECOMP: return  "Eigen decomposition of a diagonal W block failed";
	  case ERR_GLV_WINDOW: return "Creation of a viewer window failed";
	  case ERR_BOD_NEW3_SINGULAR_JACOBIAN: return "NEW3 Singular Jacobian";
	  case ERR_BOD_NEW3_NEWTON_DIVERGENCE: return "NEW3 Newton method divergence";
	  case ERR_BOD_MAX_FREQ_LE0: return "Maximal eigen frequency of a body not positive";
	  case ERR_BOD_KIND: return "Invalid body kind";
	  case ERR_BOD_SCHEME: return "Invalid body time integration scheme";
	  case ERR_BOD_SCHEME_NOT_CONVERGED: return "Time integration scheme failed to converge";
	  case ERR_BOD_MOTION_INVERT: return "Body motion not invertible";
	  case ERR_BOD_ENERGY_CONSERVATION: return "Energy conservation failed";
	  case ERR_RND_NO_DOMAIN: return "Nothing to visualise";
	  case ERR_RND_STACK_OVERFLOW: return "Stack overflow in rendering module";
	  case ERR_PCK_UNPACK: return "Trying to unpack more elements then stored";
	  case ERR_DOM_DEPTH: return "Unphysical interpenetration has occurred";
	  case ERR_DOM_TOO_MANY_BODIES: return "Too many bodies";
	  case ERR_DOM_TOO_MANY_CONSTRAINTS: return "Too many constraints";
	  case ERR_ZOLTAN_INIT: return "Zoltan initialization failed";
	  case ERR_ZOLTAN: return "Zoltan call failed";
	  case ERR_GAUSS_SEIDEL_DIAGONAL_DIVERGED: return "GAUSS_SEIDEL_SOLVER failed with error code DIAGONAL_DIVERGED";
	  case ERR_GAUSS_SEIDEL_DIVERGED: return "GAUSS_SEIDEL_SOLVER failed with error code DIVERGED";
	  case ERR_FEM_MASS_NOT_SPD: return "Mass matrix is not symmetric-positive-definite in FEM module";
	  case ERR_FEM_COORDS_INVERT: return "Global to local coordinates convertion failed in FEM module";
	  case ERR_FEM_CUT_VOLUME: return "Volume cut of FE mesh does not match the shape volume in FEM module";
	  case ERR_FEM_ROT_SINGULAR_JACOBIAN: return "FEM rotation update singular Jacobian";
	  case ERR_FEM_ROT_NEWTON_DIVERGENCE: return "FEM rotation update Newton method divergence";
	  case ERR_FEM_POINT_OUTSIDE: return "Referential point outside of domain";
	  case ERR_BUG_FOUND: return "A bug was found";
	}

	return "Unknown";
  }
  }
}
