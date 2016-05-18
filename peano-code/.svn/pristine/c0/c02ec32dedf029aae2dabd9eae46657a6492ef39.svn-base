// Copyright (C) 2009 Technische Universitaet Muenchen
// This file is part of the Peano project. For conditions of distribution and
// use, please see the copyright notice at www5.in.tum.de/peano
#ifndef _PEANO_KERNEL_PARALLEL_LEVEL_TRANSFER_OPERATORS_H_
#define _PEANO_KERNEL_PARALLEL_LEVEL_TRANSFER_OPERATORS_H_


#include "tarch/la/Vector.h"
#include "peano/utils/Globals.h"


namespace peano {
  namespace parallel {
    /**
     * This file is a container of inter-level operators. They are stateless, i.e.
     * do not belong to a certain class.
     *
     * The operation accepts the @f$ 2^d @f$ coarse grid vertices' adjacency
     * information in a @f$ 2^d \cdot 2^d @f$ array of integers. It then derives the
     * adjacent ranks for the specified fine grid vertex. In the regular case,
     * the overall domain is interpreted as the coarse grid: the subdomain is
     * told at fork time which ranks handle the adjacent subdomains (the
     * initial domain splitting always equals a Cartesian grid). It then
     * derives for all subvertices which adjacency flags are there.
     *
     * @image html peano/parallel/LevelTransferOperators.png
     *
     *
     * !! Algorithm
     *
     * The algorithm recursively runs along every axis and merges the flags as
     * illustrated in the picture.
     *
     * @todo Hier fehlt ein Bild
     * @image html peano/parallel/LevelTransferOperators_recursive.png
     *
     *
     * !! Adjacency Lists
     *
     * An adjacency list is just a sequence of ranks of size @f$ 2^d @f$. Its
     * entries are enumerated lexicographically, and they tell Peano which
     * ranks are responsible for the adjacent cells.
     *
     * @image html peano/parallel/AdjacencyListsEnumeration.png
     *
     */
    tarch::la::Vector<TWO_POWER_D,int> getAdjacencyList(
      const tarch::la::Vector<TWO_POWER_D_TIMES_TWO_POWER_D,int>& coarseGridAdjacencyInformation,
      const tarch::la::Vector<DIMENSIONS,int>&                    fineGridNumberOfGridPoints,
      const tarch::la::Vector<DIMENSIONS,int>&                    fineGridPoint
    );
  }
}


#endif
