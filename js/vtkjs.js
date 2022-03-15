"use strict";

window.addEventListener('load', function init() {
  // "Imports"
  const vtkActor = vtk.Rendering.Core.vtkActor;
  const vtkSphereSource = vtk.Filters.Sources.vtkSphereSource;
  const vtkLineSource = vtk.Filters.Sources.vtkLineSource;
  const vtkTubeFilter = vtk.Filters.General.vtkTubeFilter;
  const vtkMapper = vtk.Rendering.Core.vtkMapper;
  const vtkOpenGLRenderWindow = vtk.Rendering.OpenGL.vtkRenderWindow;
  const vtkRenderWindow = vtk.Rendering.Core.vtkRenderWindow;
  const vtkRenderWindowInteractor = vtk.Rendering.Core.vtkRenderWindowInteractor;
  const vtkRenderer = vtk.Rendering.Core.vtkRenderer;
  const vtkInteractorStyleTrackballCamera = vtk.Interaction.Style.vtkInteractorStyleTrackballCamera;

  // Initialization
  const renderWindow = vtkRenderWindow.newInstance();
  const renderer = vtkRenderer.newInstance({ background: [0.2, 0.3, 0.4] });
  renderWindow.addRenderer(renderer);

  const openglRenderWindow = vtkOpenGLRenderWindow.newInstance();
  renderWindow.addView(openglRenderWindow);

  const container = document.getElementById('vtk');
  openglRenderWindow.setSize(container.width, container.height);
  openglRenderWindow.setContainer(container);

  const { width, height } = container.getBoundingClientRect();
  openglRenderWindow.setSize(width, height);

  const interactor = vtkRenderWindowInteractor.newInstance();
  interactor.setView(openglRenderWindow);
  interactor.initialize();
  interactor.bindEvents(container);

  interactor.setInteractorStyle(vtkInteractorStyleTrackballCamera.newInstance());

  // Storage for spheres
  let spheres = [];
  let tubes = [];

  // Event listeners
  const addButton = document.getElementById("add-node");
  addButton.addEventListener('click', (e) => {
    const spacing = 30.0;
    const sphereSource = vtkSphereSource.newInstance({ radius: 1.0 });
    sphereSource.setCenter(Math.random() * spacing, Math.random() * spacing, Math.random() * spacing);

    const mapper = vtkMapper.newInstance();
    mapper.setInputConnection(sphereSource.getOutputPort());

    const actor = vtkActor.newInstance();
    actor.setMapper(mapper);

    spheres.push(actor);
    renderer.addActor(actor);
    if (spheres.length > 1) {
      let index;
      const lineSource = vtkLineSource.newInstance();
      lineSource.setPoint1(sphereSource.getCenter());
      if (spheres.length > 2) {
        index = Math.round(Math.random() * (spheres.length - 2));
      } else {
        index = 0;
      }
      lineSource.setPoint2(spheres[index].getCenter());
      lineSource.setResolution(20);
      lineSource.update();
      const tubeFilter = vtkTubeFilter.newInstance();
      tubeFilter.setInputConnection(lineSource.getOutputPort());
      tubeFilter.setNumberOfSides(8);
      tubeFilter.setRadius(0.2);
      tubeFilter.update();
      const mapperL = vtkMapper.newInstance();
      mapperL.setInputConnection(tubeFilter.getOutputPort());
      const actorL = vtkActor.newInstance();
      actorL.setMapper(mapperL);
      if (spheres.length == 2) {
        tubes[0] = [actorL];
      } else if (spheres.length > 2) {
        tubes[index].push(actorL);
      }
      tubes[spheres.length - 1] = [actorL];
      renderer.addActor(actorL);
    }
    renderer.resetCamera();
    renderWindow.render();
  });

  const resetButton = document.getElementById("reset-camera");
  resetButton.addEventListener('click', (e) => {
    renderer.resetCamera();
    renderWindow.render();
  });

  const removeLast = document.getElementById("remove-last");
  removeLast.addEventListener('click', (e) => {
    if (spheres.length) {
      const actor = spheres.splice(spheres.length - 1, 1)[0];
      let spheresTubes = tubes.splice(tubes.length - 1, 1)[0];
      for (let tube of spheresTubes)
        renderer.removeActor(tube);
      renderer.removeActor(actor);
      renderer.resetCamera();
      renderWindow.render();
    }
  });

  const removeFirst = document.getElementById("remove-first");
  removeFirst.addEventListener('click', (e) => {
    if (spheres.length) {
      const actor = spheres.splice(0, 1)[0];
      let spheresTubes = tubes.splice(0, 1)[0];
      for (let tube of spheresTubes)
        renderer.removeActor(tube);
      renderer.removeActor(actor);
      renderer.resetCamera();
      renderWindow.render();
    }
  });
});
