print '======================'
print 'PARMES: bounce example'
print '======================'
from math import pi

# material data
rho = 1000.0
E = 1E6
nu = 0.2

# sphere data
rad = 0.5

# contact data
kn = 1E6
da = 1.0
mu = 0.0

# duration and time step ratio
dura = 1.0
ratio = 0.2

# define material
mat = MATERIAL (rho, E, nu)

# define sphere
SPHERE ((0, 0, 2*rad), rad, mat, 1)

# define obstacles
OBSTACLE ([(-1,-1,0, 1,-1,0, 0,1,0)], 2)

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
