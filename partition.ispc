/*
The MIT License (MIT)

Copyright (c) 2015 Tomasz Koziara

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

#include "partition.h"
#include "algo.h"

typedef unsigned int uint;

/* calculate extrema of x, y, z */
task void extrema (uniform int span, uniform int n, uniform iREAL x[], uniform iREAL y[], uniform iREAL z[], uniform iREAL extents[])
{
  uniform int start = taskIndex*span;
  uniform int end = taskIndex == taskCount-1 ? n : start+span;

  iREAL e[6] = {iREAL_MAX,iREAL_MAX,iREAL_MAX,-iREAL_MAX,-iREAL_MAX,-iREAL_MAX};

  foreach (i = start ... end)
  {
    if (x[i] < e[0]) e[0] = x[i];
    if (y[i] < e[1]) e[1] = y[i];
    if (z[i] < e[2]) e[2] = z[i];
    if (x[i] > e[3]) e[3] = x[i];
    if (y[i] > e[4]) e[4] = y[i];
    if (z[i] > e[5]) e[5] = z[i];
  }

  uniform iREAL * uniform out = &extents [6*taskIndex];

  out[0] = reduce_min (e[0]);
  out[1] = reduce_min (e[1]);
  out[2] = reduce_min (e[2]);
  out[3] = reduce_max (e[3]);
  out[4] = reduce_max (e[4]);
  out[5] = reduce_max (e[5]);
}

/* Expands a 10-bit integer into 30 bits by inserting 2 zeros after each bit */
/* https://developer.nvidia.com/content/thinking-parallel-part-iii-tree-construction-gpu */
inline uint expandbits(uint v)
{
  v = (v * 0x00010001u) & 0xFF0000FFu;
  v = (v * 0x00000101u) & 0x0F00F00Fu;
  v = (v * 0x00000011u) & 0xC30C30C3u;
  v = (v * 0x00000005u) & 0x49249249u;
  return v;
}

/* Calculates a 30-bit Morton code for the given 3D point located within the unit cube [0,1] */
/* https://developer.nvidia.com/content/thinking-parallel-part-iii-tree-construction-gpu */
task void morton (uniform int span, uniform int n, uniform iREAL x[], uniform iREAL y[], uniform iREAL z[], uniform iREAL extents[], uniform uint code[])
{
  uniform int start = taskIndex*span;
  uniform int end = taskIndex == taskCount-1 ? n : start+span;

  uniform iREAL wx = extents[3]-extents[0],
               wy = extents[4]-extents[1],
	       wz = extents[5]-extents[2];

  foreach (i = start ... end)
  {
    iREAL px = (x[i]-extents[0])/wx,
         py = (y[i]-extents[1])/wy,
	 pz = (z[i]-extents[2])/wz;

    iREAL qx = min(max(px * 1024.0f, 0.0f), 1023.0f),
         qy = min(max(py * 1024.0f, 0.0f), 1023.0f),
         qz = min(max(pz * 1024.0f, 0.0f), 1023.0f);

    uint xx = expandbits((int)qx),
         yy = expandbits((int)qy),
         zz = expandbits((int)qz);

    code[i] = xx*4 + yy*2 + zz;
  }
}

/* quick sort on unsigned integers */
static void quick_sort (uniform uint a[], uniform int n, uniform int order[])
{
  uniform uint p, t;
  uniform int i, j;

  if (n < 2) return;

  p = a[n/2];

  for (i = 0, j = n - 1;; i++, j--)
  {
    while (a[i] < p) i++;
    while (p < a[j]) j--;
    if (i >= j) break;

    t = a[i];
    a[i] = a[j];
    a[j] = t;

    t = order[i];
    order[i] = order[j];
    order[j] = t;
  }

  quick_sort (a, i, order);
  quick_sort (a+i, n-i, order+i);
}

/* parallel quick sort task */
task void quick_task (uniform int span, uniform int n, uniform uint a[], uniform int order[])
{
  uniform int start = taskIndex*span;
  uniform int end = taskIndex == taskCount-1 ? n : start+span;

  quick_sort (a+start, end-start, order+start);
}

/* merge tree for parallel quick sort */
struct merge_tree
{
  uniform int leaf;
  uniform int parent;
  uniform int left;
  uniform int right;

  uniform uint * uniform b;
  uniform int * uniform p;
};

/* create in place merge tree and store b[] and p[] at leaves */
static void build_tree (uniform int parent, uniform int node, uniform merge_tree tree[],
 uniform int * uniform i, uniform int span, uniform uint b[], uniform int p[], uniform int n)
{
  if (n == 1)
  {
    tree[node].parent = parent;
    tree[node].leaf =
    tree[node].left =
    tree[node].right = node;

    tree[node].b = b;
    tree[node].p = p;
  }
  else
  {
    tree[node].parent = parent;
    tree[node].leaf = -1;
    tree[node].left = ++(*i);
    tree[node].right = ++(*i);

    tree[node].b = NULL;
    tree[node].p = NULL;

    build_tree (node, tree[node].left, tree, i, span, b, p, n/2);
    build_tree (node, tree[node].right, tree, i, span, b+span*(n/2), p+span*(n/2), n-n/2);
  }
}

/* initialise merge tree leaf pointers */
static uniform int init_leaf (uniform int left, uniform int right, uniform merge_tree tree[])
{
  if (tree[left].leaf < 0) tree[left].leaf = init_leaf (tree[left].left, tree[left].right, tree);
  if (tree[right].leaf < 0) tree[right].leaf = init_leaf (tree[right].left, tree[right].right, tree);

  if (tree[tree[left].leaf].b[0] < tree[tree[right].leaf].b[0]) return tree[left].leaf;
  else return tree[right].leaf;
}

/* sort n unsigned integers and return their ordering */
static void parallel_sort (uniform int n, uniform uint a[], uniform int order[], uniform int threads)
{
  uniform int num = threads;
  uniform int span = n/num + 1; /* one extre stopgap item per range */
  uniform int i, j, start, end;

  uniform uint * uniform b = uniform new uniform uint[n+num]; /* initial size plus stopgaps */
  uniform int * uniform p = uniform new uniform int[n+num];

  for (j = 0; j < num; j ++) /* initialise buffers */
  {
    start = j*span;
    end = j == num-1 ? n+num : start+span;

    foreach (k = start ... end-1) b[k] = a[k-j];

    b[end-1] = 0xFFFFFFFFu; /* stopgap prevents going beyond range when merging */

    foreach (k = start ... end-1) p[k] = k-j; /* mind the back shift, k-j, here and above */

    p[end-1] = -1;
  }

  launch[num] quick_task (span, n+num, b, p); /* parallel fragmented quick sort */
  sync;

  uniform merge_tree * uniform tree = uniform new uniform merge_tree [2*num]; /* tree and leaves size */

  i = 0;
  build_tree (-1, 0, tree, &i, span, b, p, num); /* build in place tree */
  tree->leaf = init_leaf (tree->left, tree->right, tree); /* initialise leaf pointers */

  for (i = 0; i < n; i ++) /* merge loop */
  {
    uniform int leaf = tree->leaf; /* root leaf pointer is the smallest element */

    a[i] = *(tree[leaf].b++); /* get and iterate array */
    order[i] = *(tree[leaf].p++); /* get and iterate order */

    uniform int node = tree[leaf].parent; /* leaf's parent starts tree update */

    while (node >= 0) /* climb the tree and update leaf pointers */
    {
      if (tree[tree[tree[node].left].leaf].b[0] < /* select smaller leaf of the two siblings */
	  tree[tree[tree[node].right].leaf].b[0]) tree[node].leaf = tree[tree[node].left].leaf;
      else tree[node].leaf = tree[tree[node].right].leaf;

      node = tree[node].parent; /* climb up */
    }
  }

  /* O((n/m)log(n/m)) + O(n*log(m)), where n - input size, m - number of tasks */

  delete b;
  delete p;
  delete tree;
}

/* population count (number of one bits) from http://aggregate.org/MAGIC/ */
inline static uniform unsigned int ones (uniform unsigned int x)
{
  x -= ((x >> 1) & 0x55555555);
  x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
  x = (((x >> 4) + x) & 0x0f0f0f0f);
  x += (x >> 8);
  x += (x >> 16);
  return (x & 0x0000003f);
}

/* leading zero count from http://aggregate.org/MAGIC/ */
inline static uniform unsigned int lzc (uniform unsigned int x)
{
  x |= (x >> 1);
  x |= (x >> 2);
  x |= (x >> 4);
  x |= (x >> 8);
  x |= (x >> 16);
  return (32 - ones(x));
}

/* generalised leading zero count as required by the radix tree algorithm */
inline static uniform int delta (uniform int i, uniform unsigned int codei, uniform int j, uniform int n, uniform unsigned int code[])
{
  if (j < 0 || j > n-1) return -1;

  uniform unsigned int codej = code[j];

  if (codei == codej) return 32 + lzc (i ^ j);
  else return lzc (codei ^ codej);
}

/* http://stackoverflow.com/questions/14579920/fast-sign-of-integer-in-c */
inline static uniform int sign (uniform int x)
{
  return (uniform int)(x > 0) - (uniform int)(x < 0);
}

/* binary space partitioning radix tree */
struct radix_tree
{
  uniform int parent;
  uniform int first;
  uniform int size;
  uniform int split;

  uniform iREAL coord;
  uniform int dimension;
};

/* find minimum coord in re-order-ed [start,end) range */
inline static uniform iREAL mincoord (uniform iREAL coord[], uniform int order[], uniform int start, uniform int end)
{
  uniform iREAL ret = iREAL_MAX;

  for (uniform int i = start; i < end; i ++)
  {
    uniform iREAL val = coord[order[i]];
    if (val < ret) ret = val;
  }

  return ret;
}

/* from https://research.nvidia.com/publication/maximizing-parallelism-construction-bvhs-octrees-and-k-d-trees */
task void radix_tree_create (uniform int span, uniform int n, uniform unsigned int code[],
  uniform radix_tree tree[], uniform int order[], uniform iREAL * uniform point[3])
{
  uniform int start = taskIndex*span;
  uniform int end = taskIndex == taskCount-1 ? n-1: start+span;

  for (uniform int i = start; i < end; i ++)
  {
    uniform int codei = code[i];

    uniform int d = sign (delta(i, codei, i+1, n, code) - delta(i, codei, i-1, n, code));

    uniform int dmin = delta (i, codei, i-d, n, code);

    uniform int lmax = 2;

    while (delta (i, codei, i+lmax*d, n, code) > dmin) lmax *= 2;

    uniform int l = 0;

    for (uniform int t = lmax/2; t >= 1; t /= 2)
    {
      if (delta (i, codei, i+(l+t)*d, n, code) > dmin) l = l+t;
    }

    uniform int j = i+l*d;

    tree[i].first = d > 0 ? i : j;
    tree[i].size = l+1;

    if (tree[i].size <= CUTOFF) /* create terminal node and copy particle data */
    {
      tree[i].split = 0;
      tree[i].coord = 0.0;
      tree[i].dimension = -1; /* mark as terminal node */
    }
    else
    {
      uniform int dnode = delta (i, codei, j, n, code);

      uniform int s = 0;

      for (uniform int t = (l+1)/2;; t = (t+1)/2)
      {
	if (delta (i, codei, i+(s+t)*d, n, code) > dnode) s = s+t;
	if (t == 1) break;
      }

      tree[i].split = i+s*d+min(d,0);

      if (tree[i].first != tree[i].split) /* not left leaf => left node */
      {
	tree[tree[i].split].parent = i; /* left node parent */
      }

      if (tree[i].first+tree[i].size-1 != tree[i].split+1) /* not right leaf => right node */
      {
	tree[tree[i].split+1].parent = i; /* right node parent */
      }

      uniform int dimension = (dnode-2)%3;

      tree[i].coord = mincoord (point[dimension], order, tree[i].split+1, tree[i].first+tree[i].size);
      tree[i].dimension = dimension;
    }

#if 0
    print ("i = %, j = %, d = %, dmin = %, dnode = %, split = %, size = %, parent = %, dim = %, coord = %\n",
      i, j, d, dmin, dnode, tree[i].split, tree[i].size, tree[i].parent, dim, tree[i].coord);
#endif
  }

  if (taskIndex == 0)
  {
    tree[0].parent = -1; /* nil root's parent */

    if (n == 1) /* handle the signe sphere case */
    {
      tree[0].first = 0;
      tree[0].size = 1;
      tree[0].dimension = -1;
    }
  }
}

#if 0
static void maxlevel (uniform radix_tree tree[], uniform int node, uniform int level, uniform int * uniform lmax, uniform int * uniform lsum)
{
  if (*lmax < level) *lmax = level;

  if (tree[node].dimension < 0)
  {
    *lsum +=  tree[node].size;
    return; /* terminal node */
  }

  uniform int j = tree[node].split;

  if (tree[node].first != j) /* not left leaf */
    maxlevel (tree, j, level+1, lmax, lsum);

  if ((tree[node].first+tree[node].size-1) != (j+1)) /* not right leaf */
    maxlevel (tree, j+1, level+1, lmax, lsum);
}

static void printlevel (uniform radix_tree tree[], uniform int node, uniform int level, uniform int lprint)
{
  if (level < lprint)
  {
    if (tree[node].dimension < 0) return; /* terminal node */

    uniform int j = tree[node].split;

    if (tree[node].first != j) /* not left leaf */
      printlevel (tree, j, level+1, lprint);

    if ((tree[node].first+tree[node].size-1) != (j+1)) /* not right leaf */
      printlevel (tree, j+1, level+1, lprint);
  }
  else if (level == lprint)
  {
    print ("%: i = %, split = %, size = %, parent = %, dim = %, coord = %\n",
      level, node, tree[node].split, tree[node].size,
      tree[node].parent, tree[node].dimension, tree[node].coord);
  }
}

static void printree (uniform radix_tree tree[])
{
  uniform int lsum = 0, lmax =  1;
  
  maxlevel (tree, 0, 1, &lmax, &lsum);

  print (">>> leaf sum = % <<<\n", lsum);

  for (uniform int lprint = 0; lprint < lmax; lprint ++)
  {
    printlevel (tree, 0, 0, lprint);
  }
}

static void maxlevel (uniform partitioning tree[], uniform int node, uniform int level, uniform int * uniform lmax, uniform int * uniform lsum)
{
  if (*lmax < level) *lmax = level;

  if (tree[node].dimension < 0)
  {
    *lsum +=  tree[node].data->size;
    return; /* terminal node */
  }

  maxlevel (tree, tree[node].left, level+1, lmax, lsum);
  maxlevel (tree, tree[node].right, level+1, lmax, lsum);
}

static void printlevel (uniform partitioning tree[], uniform int node, uniform int level, uniform int lprint)
{
  if (level < lprint)
  {
    if (tree[node].dimension < 0) return; /* terminal node */

    printlevel (tree, tree[node].left, level+1, lprint);
    printlevel (tree, tree[node].right, level+1, lprint);
  }
  else if (level == lprint)
  {
    print ("%: i = %, left = %, right = %, dim = %, coord = %\n",
      level, node, tree[node].left, tree[node].right,
      tree[node].dimension, tree[node].coord);
  }
}

static void printree (uniform partitioning tree[])
{
  uniform int lsum = 0, lmax =  1;
  
  maxlevel (tree, 0, 1, &lmax, &lsum);

  print (">>> leaf sum = % <<<\n", lsum);

  for (uniform int lprint = 0; lprint < lmax; lprint ++)
  {
    printlevel (tree, 0, 0, lprint);
  }
}

static void print_bits (uniform uint code)
{
  for (uniform int i = 0; i < 32; i ++)
  {
    if (code & (0x80000000>>i)) print ("1");
    else print ("0");
  }
}
#endif

/* calculate size of the radix tree */
static void radix_tree_size (uniform radix_tree tree[], uniform int node, uniform int * uniform size)
{
  if (tree[node].dimension >= 0) /* node */
  {
    (*size) += 2; /* two potential siblings */

    uniform int j = tree[node].split;

    if (tree[node].first != j) /* not left leaf */
      radix_tree_size (tree, j, size);

    if ((tree[node].first+tree[node].size-1) != (j+1)) /* not right leaf */
      radix_tree_size (tree, j+1, size);
  }
}

/* create paritioning tree from the radix tree and copy particles into it */
static void partitioning_tree_create (uniform radix_tree rtree[], uniform int rnode,
  uniform partitioning ptree[], uniform int pnode, uniform int * uniform i)
{
  ptree[pnode].coord = rtree[rnode].coord;
  ptree[pnode].dimension = rtree[rnode].dimension;

  if (rtree[rnode].dimension >= 0) /* node */
  {
    ptree[pnode].left = ++(*i);
    ptree[pnode].right = ++(*i);
    ptree[pnode].data = NULL;

    uniform int j = rtree[rnode].split;

    if (rtree[rnode].first != j) /* not left leaf */
      partitioning_tree_create (rtree, j, ptree, ptree[pnode].left, i);
    else /* left leaf */
    {
      uniform int left = ptree[pnode].left;
      ptree[left].coord = 0.0;
      ptree[left].dimension = -1;
      ptree[left].left = ptree[left].right = -1;
      ptree[left].data = uniform new uniform leaf_data;
      ptree[left].data->size = 0;
    }

    if ((rtree[rnode].first+rtree[rnode].size-1) != (j+1)) /* not right leaf */
      partitioning_tree_create (rtree, j+1, ptree, ptree[pnode].right, i);
    else /* right leaf */
    {
      uniform int right = ptree[pnode].right;
      ptree[right].coord = 0.0;
      ptree[right].dimension = -1;
      ptree[right].left = ptree[right].right = -1;
      ptree[right].data = uniform new uniform leaf_data;
      ptree[right].data->size = 0;
    }
  }
  else /* leaf */
  {
    ptree[pnode].left = ptree[pnode].right = -1;
    ptree[pnode].data = uniform new uniform leaf_data;
    ptree[pnode].data->size = 0;
  }
}

/* zero leaf data */
static void partitioning_tree_zero (uniform struct partitioning tree[], uniform int node)
{
  if (tree[node].dimension >= 0) /* node */
  {
    partitioning_tree_zero (tree, tree[node].left);
    partitioning_tree_zero (tree, tree[node].right);
  }
  else /* leaf */
  {
    tree[node].data->size = 0;
  }
}

/* free leaf data */
static void partitioning_tree_free (uniform struct partitioning tree[], uniform int node)
{
  if (tree[node].dimension >= 0) /* node */
  {
    partitioning_tree_free (tree, tree[node].left);
    partitioning_tree_free (tree, tree[node].right);
  }
  else /* leaf */
  {
    delete tree[node].data;
  }
}

/* drop ellipsoid down the partitioning tree */
static void drop_ellipsoid (uniform partitioning tree[], uniform int node, uniform int i,
  uniform int ellcol[], uniform int part[], uniform iREAL * uniform center[6],
  uniform iREAL * uniform radii[3], uniform iREAL * uniform orient[18],
  uniform int * uniform repart)
{
  uniform int d = tree[node].dimension;

  if (d >= 0) /* node */
  {
    if (center[d][i] < tree[node].coord) /* "<" is congruent with the selection of coord in radix_tree_create */
      drop_ellipsoid (tree, tree[node].left, i, ellcol, part, center, radii, orient, repart);
    else drop_ellipsoid (tree, tree[node].right, i, ellcol, part, center, radii, orient, repart);
  }
  else /* leaf */
  {
    uniform leaf_data * uniform l = tree[node].data;

    uniform int j = atomic_add_global (&l->size, 1);

    if (j < LSIZE)
    {
      l->color[j] = ellcol[i];
      l->part[j] = part[i];
      l->ell[j] = i;
      l->center[0][j] = center[0][i];
      l->center[1][j] = center[1][i];
      l->center[2][j] = center[2][i];
      l->radii[0][j] = radii[0][i];
      l->radii[1][j] = radii[1][i];
      l->radii[2][j] = radii[2][i];

      if (radii[1][i] > 0.) /* ellipsoid */
      {
	l->orient[0][j] = orient[0][i];
	l->orient[1][j] = orient[1][i];
	l->orient[2][j] = orient[2][i];
	l->orient[3][j] = orient[3][i];
	l->orient[4][j] = orient[4][i];
	l->orient[5][j] = orient[5][i];
	l->orient[6][j] = orient[6][i];
	l->orient[7][j] = orient[7][i];
	l->orient[8][j] = orient[8][i];
      }
    }
    else atomic_add_global (repart, 1);
  }
}

/* store ellipsoids at tree leaves */
task void store_ellipsoids (uniform int span, uniform partitioning tree[], uniform int ellnum, uniform int ellcol[],
  uniform int part[], uniform iREAL * uniform center[6], uniform iREAL * uniform radii[3], uniform iREAL * uniform orient[18],
  uniform int * uniform repart)
{
  uniform int start = taskIndex*span;
  uniform int end = taskIndex == taskCount-1 ? ellnum: start+span;

  for (uniform int i = start; i < end; i ++)
  {
    drop_ellipsoid (tree, 0, i, ellcol, part, center, radii, orient, repart);
  }
}

/* create partitioning tree */
export uniform partitioning * uniform partitioning_create (uniform int threads, uniform int ellnum, uniform iREAL * uniform center[6])
{
  uniform int span = ellnum/threads;

  uniform iREAL * uniform extents = uniform new uniform iREAL [6*threads];

  launch[threads] extrema (span, ellnum, center[0], center[1], center[2], extents);
  sync;

  for (uniform int i = 1; i < threads; i ++)
  {
    uniform iREAL * uniform e = &extents [6*i];

    if (e[0] < extents[0]) extents[0] = e[0];
    if (e[1] < extents[1]) extents[1] = e[1];
    if (e[2] < extents[2]) extents[2] = e[2];
    if (e[3] > extents[3]) extents[3] = e[3];
    if (e[4] > extents[4]) extents[4] = e[4];
    if (e[5] > extents[5]) extents[5] = e[5];
  }

  uniform uint * uniform code = uniform new uniform uint [ellnum];

  launch[threads] morton (span, ellnum, center[0], center[1], center[2], extents, code);
  sync;

  uniform int * uniform order = uniform new uniform int [ellnum];

#if 1
  parallel_sort (ellnum, code, order, threads);
#else
  foreach (i = 0 ... ellnum) order[i] = i;
  quick_sort (code, ellnum, order);
#endif

  uniform radix_tree * uniform rtree = uniform new uniform radix_tree [ellnum];

  launch[threads] radix_tree_create (span, ellnum, code, rtree, order, center);
  sync;

#if 0
  for (uniform int i = 0; i < ellnum; i ++)
  {
    print ("%: ", i);
    print_bits (code[i]);
    print (", %, %, %\n", center[0][order[i]], center[1][order[i]], center[2][order[i]]);
  }
#endif

  uniform int i = 1;

  radix_tree_size (rtree, 0, &i);

#if 0
  printree (rtree);
  print ("size 1 = %\n", i);
#endif


  uniform partitioning * uniform ptree = uniform new uniform partitioning[i];

  i = 0;

  partitioning_tree_create (rtree, 0, ptree, 0, &i);

#if 0
  print ("size 2 = %\n", i);
  printree (ptree);
#endif

  /* clean up */
  delete extents;
  delete code;
  delete order;
  delete rtree;

  return ptree;
}

/* store ellipsoids in the partitioning tree leaves */
export uniform int partitioning_store (uniform int threads, uniform partitioning * uniform tree,
  uniform int ellnum, uniform int ellcol[], uniform int part[], uniform iREAL * uniform center[6],
  uniform iREAL * uniform radii[3], uniform iREAL * uniform orient[18])
{
  partitioning_tree_zero (tree, 0);

  uniform int repart = 0;

  launch [threads] store_ellipsoids (ellnum/threads, tree, ellnum, ellcol, part, center, radii, orient, &repart);

  return repart;
}

/* copy partitioned data back to global buffers and free its memory */
export void partitioning_destroy (uniform partitioning * uniform tree)
{
  partitioning_tree_free (tree, 0);

  delete tree;
}
