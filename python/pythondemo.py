import sys

from PyQt5.QtWidgets import QApplication, QLabel, QMainWindow, QPushButton
from PyQt5.QtCore import QFile, QIODevice
from PyQt5.uic import loadUi
from vtk.qt.QVTKRenderWindowInteractor import QVTKRenderWindowInteractor
import vtk

class MainWindow(QMainWindow):

    def __init__(self, parent = None):
        QMainWindow.__init__(self, parent)

        loadUi("pydemo.ui", self)
        self.vtkWidget = QVTKRenderWindowInteractor(self.qvtkFrame)
        self.vtkWidget.setFixedSize(self.qvtkFrame.size())

        self.ren = vtk.vtkRenderer()
        self.vtkWidget.GetRenderWindow().AddRenderer(self.ren)
        self.iren = self.vtkWidget.GetRenderWindow().GetInteractor()
        self.iren.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())

        self.exporter = vtk.vtkOBJExporter()
        self.exporter.SetActiveRenderer(self.ren)
        self.exporter.SetRenderWindow(self.vtkWidget.GetRenderWindow())
        self.exporter.SetFilePrefix("philly_scene")

        colors = vtk.vtkNamedColors()
        info = {}

        lines = [x.strip().split(',') for x in
                open('philly_data.csv').readlines()][1:]
        for line in lines:
            # Create source
            source = vtk.vtkCubeSource()
            a = 0.0002
            height = (float(line[7])*.0003048*90/10000) * 10
            source.SetBounds(0, a, 0, a, -height, 0)
            source.SetCenter(float(line[0]), float(line[1]), 0 - height / 2)

            # Create a mapper
            mapper = vtk.vtkPolyDataMapper()
            mapper.SetInputConnection(source.GetOutputPort())

            # Create an actor
            actor = vtk.vtkActor()
            actor.SetMapper(mapper)
            actor.GetProperty().SetColor(list(colors.GetColor3d("SaddleBrown")))

            self.ren.AddActor(actor)

            source = vtk.vtkCubeSource()
            build_height = (float(line[8])*.0003048*90/10000) * 10
            source.SetBounds(0, a, 0, a, -build_height, 0)
            source.SetCenter(float(line[0]), float(line[1]), -height - build_height / 2)

            # Create a mapper
            mapper = vtk.vtkPolyDataMapper()
            mapper.SetInputConnection(source.GetOutputPort())

            # Create an actor
            actor = vtk.vtkActor()
            actor.SetMapper(mapper)
            actor.GetProperty().SetColor(list(colors.GetColor3d("Silver")))

            self.ren.AddActor(actor)
            info[actor] = f'{line[2]} {line[3]}\n{line[4]}, {line[5]} '\
                          f'{line[6]}\nLat: {line[0]}, Long: {line[1]}\n'\
                          f'Ground Elevation: {line[7]} ft\n'\
                          f'Building Height: {line[8]} ft\n'

        lines = [x.strip().split(',') for x in
                open('vague_outline.csv').readlines()]
        points = vtk.vtkPoints()
        pt_count = len(lines)
        for line in lines:
            points.InsertNextPoint(float(line[0]), float(line[1]), 0.0)
        poly_line = vtk.vtkPolyLine()
        poly_line.GetPointIds().SetNumberOfIds(pt_count)
        for i in range(pt_count):
            poly_line.GetPointIds().SetId(i, i)
        cells = vtk.vtkCellArray()
        cells.InsertNextCell(poly_line)
        poly_data = vtk.vtkPolyData()
        poly_data.SetPoints(points)
        poly_data.SetLines(cells)
        mapper = vtk.vtkPolyDataMapper()
        mapper.SetInputData(poly_data)
        actor = vtk.vtkActor()
        actor.SetMapper(mapper)
        actor.GetProperty().SetColor(list(colors.GetColor3d("DarkGreen")))
        self.ren.AddActor(actor)

        callback_function.ren = self.ren
        callback_function.info = info
        callback_function.info_box = self.infoBox
        self.iren.AddObserver('LeftButtonPressEvent', callback_function)

        reset_camera.camera = self.ren.GetActiveCamera()
        reset_camera.renWin = self.vtkWidget.GetRenderWindow()
        self.centerButton.clicked.connect(reset_camera)
        reset_camera()

        export_scene.exporter = self.exporter
        self.exportButton.clicked.connect(export_scene)

        self.show()
        self.iren.Initialize()

def export_scene():
    export_scene.exporter.Update()

def reset_camera():
    reset_camera.camera.SetPosition(39.7219,  -75.2668, -0.556774)
    reset_camera.camera.SetFocalPoint(39.9996,  -75.1667,  0.016131)
    reset_camera.camera.SetClippingRange(0.461478,  0.858228)
    reset_camera.camera.SetViewUp(0.818622,  0.347245,  -0.45747)
    reset_camera.camera.SetDistance(0.644482)
    reset_camera.renWin.Render()

def callback_function(caller, ev):
    picker = vtk.vtkPropPicker()
    pos = caller.GetEventPosition()
    picker.PickProp(pos[0], pos[1], callback_function.ren)
    picked_actor = picker.GetActor()
    if picked_actor in callback_function.info.keys():
        callback_function.info_box.setPlainText(
                callback_function.info[picked_actor])

def main():
    app = QApplication(sys.argv)
    #global window
    window = MainWindow()
    window.show()

    sys.exit(app.exec())

if __name__ == "__main__":
    main()
