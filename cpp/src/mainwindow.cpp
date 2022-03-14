#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    timerInterval = 20;
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(moveFish()));

    counter = 0;
    std::ifstream inFile("../python_helper/fish_data.json");
    inFile >> js;

    renderer = vtkSmartPointer<vtkRenderer>::New();
    vtkNew<vtkNamedColors> colors;

    renderer->SetBackground(colors->GetColor3d("Black").GetData());

    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    ui->qvtkWidget->setRenderWindow(renderWindow);

    renderWindowInteractor = renderWindow->GetInteractor();
    vtkNew<vtkCallbackCommand> clickCallback;
    clickCallback->SetCallback(clickCallbackFunction);
    clickCallback->SetClientData(this);
    renderWindowInteractor->AddObserver(vtkCommand::LeftButtonPressEvent,
            clickCallback);

    for (auto fish : js) {
        vtkNew<vtkOBJImporter> importer;
        std::string fileName = "../res/" + fish["file"].get<std::string>() + ".obj";
        // NOTE: this assumes you are running the program from vtk_tutorial/build
        importer->SetFileName(fileName.c_str());
        importer->SetRenderWindow(renderWindow);
        importer->Update();
    }

    auto actors = renderer->GetActors();
    actors->InitTraversal();

    for (auto& fishIT : js.items()) {
        auto fishJS = fishIT.value();
        // The importer makes two actors for some reason but only the second
        // is the fish, so we need to skip the first
        actors->GetNextItem();
        auto fish = actors->GetNextItem();
        fish->ForceOpaqueOn();
        fish->RotateY(fishJS["rotate_y"]);
        fish->RotateX(fishJS["rotate_x"]);
        fish->SetScale(fishJS["scale"], fishJS["scale"], fishJS["scale"]);
        fishVec.push_back({fish, fishIT.key()});
        vtkNew<vtkPoints> points;
        for (auto point : fishJS["path"])
            points->InsertNextPoint(point[0], point[1], point[2]);
        vtkNew<vtkPolyLine> polyLine;
        int ptCount = fishJS["path"].size();
        polyLine->GetPointIds()->SetNumberOfIds(ptCount);
        for (int i = 0; i < ptCount; i++)
            polyLine->GetPointIds()->SetId(i, i);
        vtkNew<vtkCellArray> cells;
        cells->InsertNextCell(polyLine);

        // Create a polydata to store everything in
        vtkNew<vtkPolyData> polyData;

        // Add the points to the dataset
        polyData->SetPoints(points);

        // Add the lines to the dataset
        polyData->SetLines(cells);

        // Setup actor and mapper
        vtkNew<vtkPolyDataMapper> mapperLine;
        mapperLine->SetInputData(polyData);

        vtkNew<vtkActor> actorLine;
        actorLine->SetMapper(mapperLine);
        actorLine->GetProperty()->SetColor(colors->GetColor3d("Tomato").GetData());
        renderer->AddActor(actorLine);
    }

    vtkNew<vtkCubeSource> cubeSource;
    cubeSource->SetBounds(-20.0, 20.0, -10.0, 10.0, -10.0, 100.0);
    vtkNew<vtkPolyDataMapper> mapper1;
    mapper1->SetInputConnection(cubeSource->GetOutputPort());
    vtkNew<vtkActor> cube;
    cube->SetMapper(mapper1);
    cube->GetProperty()->SetColor(
            colors->GetColor3d("Aqua").GetData());
    cube->GetProperty()->SetOpacity(0.3);
    renderer->AddActor(cube);

    renderer->ResetCamera();
    renderWindow->Render();

    exporter = vtkSmartPointer<vtkOBJExporter>::New();
    exporter->SetActiveRenderer(renderer);
    exporter->SetRenderWindow(renderWindow);
    const char *objFileName = "fish_scene";
    exporter->SetFilePrefix(objFileName);

    timer->start(timerInterval);
}

MainWindow::~MainWindow() {
}

void MainWindow::moveFish() {
    int length = 0;
    for (auto fish : fishVec) {
        auto name = std::get<1>(fish);
        if (length == 0)
            length = js[name]["path"].size();
        std::get<0>(fish)->SetPosition(js[name]["path"][counter][0],
                js[name]["path"][counter][1],
                js[name]["path"][counter][2]);
    }
    counter = (counter + 1) % length;
    renderWindow->Render();
}

void clickCallbackFunction(vtkObject* caller,
        long unsigned int eventId,
        void* clientData,
        void* vtkNotUsed(callData))
{
    vtkNew<vtkNamedColors> colors;
    auto mainWindow = reinterpret_cast<MainWindow*>(clientData);
    auto interactor = reinterpret_cast<vtkRenderWindowInteractor*>(caller);
    int* clickPos = interactor->GetEventPosition();
    cout << "Click callback" << endl;
    cout << "Event: " << vtkCommand::GetStringFromEventId(eventId)
        << "\nClick Position: " << clickPos[0] << ", " << clickPos[1]
        << std::endl;
    // Pick from this location.
    vtkNew<vtkPropPicker> picker;
    auto renderer = interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
    auto cube = renderer->GetActors()->GetLastActor();
    vtkNew<vtkActor> cubeCopy;
    cubeCopy->ShallowCopy(cube);
    renderer->RemoveActor(cube);
    picker->Pick(clickPos[0], clickPos[1], 0, renderer);
    renderer->AddActor(cubeCopy);

    double* pos = picker->GetPickPosition();
    cout << "Pick Position: " << pos[0] << " "
        << pos[1] << " " << pos[2] << std::endl;

    auto pickedActor = picker->GetActor();
    if (pickedActor == nullptr) {
        cout << "No actor picked." << std::endl;
    } else {
        cout << "Picked actor: " << picker->GetActor() << std::endl;
        mainWindow->getInfoBox()->setPlainText(QString::fromStdString(mainWindow->getFishInfo(pickedActor)));
    }
}

QTextBrowser* MainWindow::getInfoBox() {
    return ui->infoBox;
}

std::string MainWindow::getFishInfo(vtkActor *pickedFish) {
    for (auto fish : fishVec) {
        if (pickedFish == std::get<0>(fish)) {
            return js[std::get<1>(fish)]["info"];
        }
    }
    return "Fish info not found";
}

void MainWindow::on_pauseButton_clicked() {
    if (timer->isActive()) {
        timer->stop();
        ui->pauseButton->setText("Start");
    } else {
        timer->start(timerInterval);
        ui->pauseButton->setText("Pause");
    }
}

void MainWindow::on_centerButton_clicked() {
    renderer->ResetCamera();
    renderWindow->Render();
}

void MainWindow::on_exportButton_clicked() {
    exporter->Update();
    std::ofstream outFile;
    outFile.open("fish_scene.mtl", ios::app);
    outFile << "d 0.5\n";
    outFile.close();
}
