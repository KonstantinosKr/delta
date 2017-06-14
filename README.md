# Delta
We present a novel DEM (discrete element method) code with explicit time stepping. DEM codes simulate billions of small particles that interact with each other primarily through collisions. Different to state-of-the-art codes, we rely on triangulated non-spherical particles. This is computationally demanding, and we thus devise an asynchronous data exchange communication technique using MPI (Message Passing Interface) on manycore supercomputers, we discuss possible solutions to handle ghost particles that overlap multiple subdomains. At the compute node level, shared memory parallelism as well as vectorised SIMD executions are studied. On the algorithmic side, we explore a hybrid parallelisation approach and memory layouts that are suited to combine robust exact geometry checks with a fast penalty-based method.

experiment videos can be found here: https://www.youtube.com/channel/UCZNTVjbweeKB0eKhsqBmXZQ

Konstantinos Krestenitis (contact @ ikonstantinos [dot] com)
