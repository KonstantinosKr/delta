/*
 * hyb.c
 * Copyright (C) 2008, Tomasz Koziara (t.koziara AT gmail.com)
 * --------------------------------------------------------------------------
 * box overlap detection using the hybrid streamed segment tree described in:
 * Afran Zomorodian, Herbert Edelsbrunner, "Fast software for box intersection",
 * 2002, International Journal of Computer Geometry and Applications, Vol. 12,
 * Numbers 1-2, pp. 142-172.
 */

/* This file is part of Solfec.
 * Solfec is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * Solfec is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Solfec. If not, see <http://www.gnu.org/licenses/>. */

#ifndef __hyb__
#define __hyb__

typedef struct box BOX; /* axis aligned box */

/* bounding box */
struct box
{
  double extents [6]; /* min x, y, z, max x, y, z */

  void *sgp; /* shape and geometric object pair */

  void *body; /* owner of the shape */

  void *mark; /* auxiliary marker used by hashing algorithms */
};

typedef void (*BOX_Overlap_Create)  (void *data, BOX *one, BOX *two); /* created overlap callback => returns a user pointer */

/* report overlaps between n boxes */
void hybrid (BOX **boxes, int n, void *data, BOX_Overlap_Create create);

/* report overlaps between two sets of boxes */
void hybrid_ext (BOX **seta, int na, BOX **setb, int nb, void *data, BOX_Overlap_Create create);

#endif
