#include "mainwindow.h"
#include "ui_mainwindow.h"

#define PREFIX ".."

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Timer for moving the fish
    timer = new QTimer(this);
    timerInterval = 20;
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(moveFish()));
    counter = 0;

    // Read in the fish_data json file
    std::ifstream inFile(PREFIX "/python_helper/fish_data.json");
    inFile >> js;

    // Initialize VTK Objects
    renderer = vtkSmartPointer<vtkRenderer>::New();
    vtkNew<vtkNamedColors> colors;

    renderer->SetBackground(colors->GetColor3d("Black").GetData());

    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    ui->qvtkWidget->setRenderWindow(renderWindow);

    renderWindowInteractor = renderWindow->GetInteractor();
    vtkNew<vtkCallbackCommand> clickCallback;
    clickCallback->SetCallback(clickCallbackFunction);
    // We need this MainWindow accessible in the callback function
    clickCallback->SetClientData(this);
    renderWindowInteractor->AddObserver(vtkCommand::LeftButtonPressEvent,
            clickCallback);

    // Add fish models
    for (auto fish : js) {
        vtkNew<vtkOBJImporter> importer;
        std::string fileName = PREFIX "/res/" + fish["file"].get<std::string>() + ".obj";
        // NOTE: this assumes you are running the program from vtk_tutorial/build
        importer->SetFileName(fileName.c_str());
        importer->SetRenderWindow(renderWindow);
        importer->Update();
    }

    auto actors = renderer->GetActors();
    actors->InitTraversal();

    // Iterate over json entries and set up fish and curve actors
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
        // Storing actor and its name here for use in moveFish
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

    // Create the "river"
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

    // Set up scene exporter
    exporter = vtkSmartPointer<vtkOBJExporter>::New();
    exporter->SetActiveRenderer(renderer);
    exporter->SetRenderWindow(renderWindow);
    const char *objFileName = "fish_scene";
    exporter->SetFilePrefix(objFileName);

    // Start the timer
    timer->start(timerInterval);
}

MainWindow::~MainWindow() {
}

// Moves fish, called by the QTimer
void MainWindow::moveFish() {
    int length = 0;
    for (auto fish : fishVec) {
        // Name of fish from tuple
        auto name = std::get<1>(fish);
        // For the first fish read the path length (assumed to be
        // the same for all 3)
        if (length == 0)
            length = js[name]["path"].size();
        // Actually move a fish
        std::get<0>(fish)->SetPosition(js[name]["path"][counter][0],
                js[name]["path"][counter][1],
                js[name]["path"][counter][2]);
    }
    // The mod is so the start back at the beginning
    counter = (counter + 1) % length;
    renderWindow->Render();
}

// Prints information about the event and picks fish
void clickCallbackFunction(vtkObject* caller,
        long unsigned int eventId,
        void* clientData,
        void* vtkNotUsed(callData))
{
    vtkNew<vtkNamedColors> colors;
    // Cast things to the classes we need
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
    // Remove cube while picking then put it back
    auto cube = renderer->GetActors()->GetLastActor();
    vtkNew<vtkActor> cubeCopy;
    // Need to copy it because RemoveActor deletes the object
    cubeCopy->ShallowCopy(cube);
    renderer->RemoveActor(cube);
    picker->Pick(clickPos[0], clickPos[1], 0, renderer);
    renderer->AddActor(cubeCopy);

    double* pos = picker->GetPickPosition();
    cout << "Pick Position: " << pos[0] << " "
        << pos[1] << " " << pos[2] << std::endl;

    auto pickedActor = picker->GetActor();
    // Update information if we clicked a fish
    if (pickedActor == nullptr) {
        cout << "No actor picked." << std::endl;
    } else {
        cout << "Picked actor: " << picker->GetActor() << std::endl;
        mainWindow->getInfoBox()->setPlainText(
                QString::fromStdString(mainWindow->getFishInfo(pickedActor)));
    }
}

// Need to expose two private class members via this and getFishInfo
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

// Functions bound to buttons
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
    // The exporter doesn't work with transparency it seems,
    // this vaguely fixes it although the scene is still mucked up
    std::ofstream outFile;
    outFile.open("fish_scene.mtl", ios::app);
    outFile << "d 0.5\n";
    outFile.close();
}
