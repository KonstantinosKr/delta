# Delta
We present a novel DEM (discrete element method) code with explicit time stepping. DEM codes simulate billions of small particles that interact with each other primarily through collisions. Different to state-of-the-art codes, we rely on triangulated non-spherical particles. This is computationally demanding, and we thus devise an asynchronous data exchange communication technique using MPI (Message Passing Interface) on manycore supercomputers, we discuss possible solutions to handle ghost particles that overlap multiple subdomains. At the compute node level, shared memory parallelism as well as vectorised SIMD executions are studied. On the algorithmic side, we explore a hybrid parallelisation approach and memory layouts that are suited to combine robust exact geometry checks with a fast penalty-based method.

The following research would not have been possible without the funding from both the Engineering and Physical Sciences Research Council (EPSRC) and EDF Energy Generation.

I would like to thank Durham University for the supercomputing resources and technical assistance. This work was made possible with the use of Durham University's local HPC facilities Hamilton and Intel Xeon Phi cooprocessor cluster. I appreciate the support from Intel through Durham's Intel Parallel Computing Centre (IPCC) which gave me access to latest Intel software. I also made use of the facilities of N8 HPC provided and funded by the N8 consortium and EPSRC (Grant No. N8HPC_DUR_TW_PEANO). The Centre is coordinated at the time by the Universities of Leeds and Manchester. I'm also grateful for the access to ATHOS HPC resources provided by EDF R\&D in France and Advanced Research Computing High End Resource (ARCHER) HPC services at the Edinburgh Parallel Computing Centre. 

experiment videos can be found here: https://www.youtube.com/channel/UCZNTVjbweeKB0eKhsqBmXZQ

Konstantinos Krestenitis (contact @ ikonstantinos [dot] com)
