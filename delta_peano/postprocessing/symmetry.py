import matplotlib.pyplot as plt
import numpy as np

def symmetryPlot(N, x, listMass, listDiameter, listCentre, centrePoint):
    N = len(listCentre)

    x = np.array([np.array(i[0] - centrePoint) for i in listCentre])
    y = np.array([np.array(i[1]) for i in listCentre])

    minx = min(x)
    maxx = max(x)
    width = abs(minx - maxx)
    miny = min(y)
    maxy = max(y)
    height = abs(miny - maxy)


    area = np.array([np.array(i * 100000) for i in listDiameter])  # 0 to 15 point radii

    ######################################################################
    ######################################################################
    ####EXPECTED value for particle center
    IEx = sum(x) / N
    print("IEx: %f" % IEx)

    IEarray = [a * b for a, b in zip(x, listMass)]
    IEmass = sum(IEarray) / sum(listMass)
    print("IEm: %f" % IEmass)

    ####WIDTH value of pile
    Mx = abs(max([abs(IEx - i) for i in x]))
    print("Mx: %f" % Mx)

    xm = [a * b for a, b in zip(listMass, x)]
    Mm = max([abs(IEmass - i) for i in xm])
    print("Mm: %f" % Mm)

    #####Variance value of pile
    Varx = sum([pow(i - IEx, 2) for i in x]) / N
    print("Varx: %f" % Varx)

    xm = [a * b for a, b in zip(listMass, x)]
    Varmass = sum([pow(i - IEmass, 2) for i in xm]) / N
    print("Varm: %f" % Varmass)

    #####PLOT PILE
    colors = np.random.rand(N)
    plt.scatter(x, y, s=area, c=colors, alpha=0.5)
    plt.show()
    ######################################################################
    ######################################################################


