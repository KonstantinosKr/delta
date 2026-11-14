print '==========================='
print 'PARMES: hopper flow example'
print '==========================='
from math import pi

# material data
rho = 1000.0
E = 1E6
nu = 0.2

# sphere data
num = 25 # number of spheres along x, y, z
rad = 0.5/num

# contact data
kn = 1E6
da = 1.0
mu = 0.0

# duration and time step ratio
dura = 1.0
ratio = 0.2

# define material
mat = MATERIAL (rho, E, nu)

# define spheres
print 'Generating ', num**3, ' spheres'
for i in range (0,num):
  for j in range (0,num):
    for k in range (0,num):
      SPHERE ((2*i*rad+rad,2*j*rad+rad,2*k*rad+rad), rad, mat, 1)

# define obstacles
print 'Generating hopper geometry'
triangles = [(0.0,0.0,0.0, 1.0,0.0,0.0, 0.0,0.0,1.0), (0.0,0.0,1.0, 1.0,0.0,0.0, 1.0,0.0,1.0),
             (0.0,1.0,0.0, 1.0,1.0,0.0, 0.0,1.0,1.0), (0.0,1.0,1.0, 1.0,1.0,0.0, 1.0,1.0,1.0),
	     (0.0,0.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0), (0.0,0.0,1.0, 0.0,1.0,0.0, 0.0,1.0,1.0),
	     (1.0,0.0,0.0, 1.0,1.0,0.0, 1.0,0.0,1.0), (1.0,0.0,1.0, 1.0,1.0,0.0, 1.0,1.0,1.0),
	     (0.0,0.0,0.0, 1.0,0.0,0.0, 0.4,0.4,-1.0), (0.4,0.4,-1.0, 1.0,0.0,0.0, 0.6,0.4,-1.0),
	     (0.0,0.0,0.0, 0.0,1.0,0.0, 0.4,0.4,-1.0), (0.4,0.4,-1.0, 0.0,1.0,0.0, 0.4,0.6,-1.0),
	     (0.0,1.0,0.0, 1.0,1.0,0.0, 0.4,0.6,-1.0), (0.4,0.6,-1.0, 1.0,1.0,0.0, 0.6,0.6,-1.0),
	     (1.0,1.0,0.0, 1.0,0.0,0.0, 0.6,0.4,-1.0), (0.6,0.4,-1.0, 1.0,1.0,0.0, 0.6,0.6,-1.0),
	     (-1.0,-1.0,-2.0, 2.0,-1.0,-2.0, -1.0,2.0,-2.0), (-1.0,2.0,-2.0, 2.0,-1.0,-2.0, 2.0,2.0,-2.0),
	     (-1.0,-1.0,-2.0, 2.0,-1.0,-2.0, -1.0,-1.0,-1.0), (-1.0,-1.0,-1.0, 2.0,-1.0,-2.0, 2.0,-1.0,-1.0),
	     (-1.0,-1.0,-2.0, -1.0,-1.0,-1.0, -1.0,2.0,-2.0), (-1.0,2.0,-2.0, -1.0,-1.0,-1.0, -1.0,2.0,-1.0),
	     (-1.0,2.0,-2.0, 2.0,2.0,-2.0, -1.0,2.0,-1.0), (-1.0,2.0,-1.0, 2.0,2.0,-2.0, 2.0,2.0,-1.0),
	     (2.0,-1.0,-2.0, 2.0,-1.0,-1.0, 2.0,2.0,-2.0), (2.0,2.0,-2.0, 2.0,-1.0,-1.0, 2.0,2.0,-1.0)]
OBSTACLE (triangles, 2)

# define default surface pairing
PAIRING (0, 0, 'granular', kn, da, mu)

# define gravity
GRAVITY (0, 0, -10)

# estimate critical time step
hcrit = CRITICAL()
step = ratio * hcrit
print 'Critical time step: %.2e' % hcrit

# run simulation
print 'Running', int(dura/step), 'DEM steps of size %.2e' % step, '...'
t = DEM (dura, step, 0.02)
print 'Finished after %f seconds' % t
