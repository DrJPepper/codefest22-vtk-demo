#ifndef mainwindow_h
#define mainwindow_h

#include <QMainWindow>
#include <QScopedPointer>
#include <QtCore>
#include <QtCore/QtGlobal>
#include <QApplication>
#include <QWidget>
#include <QTextBrowser>
#include <QVTKOpenGLStereoWidget.h>

#include <float.h>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <random>
#include <regex>
#include <sstream>
#include <string>

#include <vtkActor.h>
#include <vtkTriangle.h>
#include <vtkActor2D.h>
#include <vtkActorCollection.h>
#include <vtkBoxWidget.h>
#include <vtkCamera.h>
#include <vtkCellArray.h>
#include <vtkColor.h>
#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkCylinderSource.h>
#include <vtkDataSetMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkLight.h>
#include <vtkLineSource.h>
#include <vtkNamedColors.h>
#include <vtkOBJExporter.h>
#include <vtkOBJImporter.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkProperty2D.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTextActor.h>
#include <vtkTextProperty.h>
#include <vtkTubeFilter.h>
#include <vtkPolyLine.h>

#include <vtkCallbackCommand.h>
#include <vtkCommand.h>
#include <vtkPropPicker.h>
#include <vtkRendererCollection.h>
#include <vtkPointPicker.h>
#include <vtkPoints.h>
#include <vtkRenderWindowInteractor.h>

#include "lib/json.hpp"

using json = nlohmann::json;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QWidget *parent = 0);
        virtual ~MainWindow();
        QTextBrowser* getInfoBox();
        std::string getFishInfo(vtkActor *pickedFish);

    private Q_SLOTS:
        void moveFish();
        void on_pauseButton_clicked();
        void on_exportButton_clicked();
        void on_centerButton_clicked();

    private:
        json js;
        int counter;
        int timerInterval;
        std::vector<std::tuple<vtkSmartPointer<vtkActor>, std::string>> fishVec;
        QScopedPointer<Ui::MainWindow> ui;
        QTimer *timer;
        vtkSmartPointer<vtkRenderer> renderer;
        vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
        vtkSmartPointer<vtkOBJExporter> exporter;
        vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor;
};

void clickCallbackFunction(vtkObject* caller, long unsigned int eventId,
        void* clientData, void* callData);

#endif
