from optparse import OptionParser
import logging
from collada import *
import vtk



def main():
    logging.basicConfig(level=logging.DEBUG)
    logging.debug('This is a debug message')
    logging.warning('This is a warning message')
    logging.error('This is an error message')
    logging.critical('This is a critical message')

    parser = OptionParser()
    parser.add_option("-f", "--file", dest="filename", help="scene FILE", metavar="FILE")

    (options, args) = parser.parse_args()
    logging.debug(options)
    
    logging.info("Opening "  + options.filename)
    f = open(options.filename, 'rb')
    
    mesh = Collada(f)
    logging.info(mesh)
    geom = mesh.geometries 
    print(geom)
   
    
    for particle in geom:
        print(particle)
        for triangle in particle.primitives[0]:
            print(triangle)

    boundgeoms = list(mesh.scene.objects('geometry'))
    print(boundgeoms)

    for g in boundgeoms:
        print(g)
        for primSet in g.primitives():
            for prim in primSet:
                print(prim)


def vtkView():

    colors = vtk.vtkNamedColors()

    # Create a triangle
    points = vtk.vtkPoints()
    points.InsertNextPoint(1.0, 0.0, 0.0)
    points.InsertNextPoint(0.0, 0.0, 0.0)
    points.InsertNextPoint(0.0, 1.0, 0.0)

    triangle = vtk.vtkTriangle()
    triangle.GetPointIds().SetId(0, 0)
    triangle.GetPointIds().SetId(1, 1)
    triangle.GetPointIds().SetId(2, 2)

    triangles = vtk.vtkCellArray()
    triangles.InsertNextCell(triangle)

    # Create a polydata object
    trianglePolyData = vtk.vtkPolyData()

    # Add the geometry and topology to the polydata
    trianglePolyData.SetPoints(points)
    trianglePolyData.SetPolys(triangles)

    # Create mapper and actor
    mapper = vtk.vtkPolyDataMapper()
    mapper.SetInputData(trianglePolyData)
    actor = vtk.vtkActor()
    actor.GetProperty().SetColor(colors.GetColor3d("Cyan"))
    actor.SetMapper(mapper)

    # Create a renderer, render window, and an interactor
    renderer = vtk.vtkRenderer()
    renderWindow = vtk.vtkRenderWindow()
    renderWindow.SetWindowName("Triangle")
    renderWindow.AddRenderer(renderer)
    renderWindowInteractor = vtk.vtkRenderWindowInteractor()
    renderWindowInteractor.SetRenderWindow(renderWindow)

    # Add the actors to the scene
    renderer.AddActor(actor)
    renderer.SetBackground(colors.GetColor3d("DarkGreen"))

    # Render and interact
    renderWindow.Render()
    renderWindowInteractor.Start()

if __name__ == "__main__":
    main()

    #vtkView()
