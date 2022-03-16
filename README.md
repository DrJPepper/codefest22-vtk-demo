# Philly Codefest VTK Workshop

A collection of three demos showing how to use VTK in different languages as well as my slides from my Philly Codefest 2022 workshop.

## Installation

All these instructions assume you are in the `codefest22-vtk-demo` directory. **You do not need to install anything to run the JavaScript demo, these dependencies are only for the C++ and Python demos**. If you need to make any changes or improvements to these instructions to get the demos to work for you, please make a pull request with your corrections so I can fix any issues.

### Linux
You need to install `vtk`, `gcc`, `cmake`, `make`, `fmt`, `boost`, `eigen`, `python3` and `qt5` via your distro's package manager. You possibly also need to install the optional VTK dependencies listed in the following Arch specific install process.

**Arch Linux specific commands:**

    # CMake/VTK expects a bunch of optional dependencies
    # even though I don't use them.
    # Everything below the first line is an optional dependency
    # that you may or may not need depending on your
    # environment
    pacman -Syu vtk qt5 cmake fmt boost eigen python \
        ospray level-zero-loader level-zero-driver \
        openvr python-mpi4py liblas adios2 libharu \
        cgns utf8cpp pdal gl2ps glew jdk-openjdk jre-openjdk
    # Below this is for building the C++ demo, it's not needed for
    # the python demo
    cd cpp
    mkdir build
    cd build
    cmake ..
    make

### Mac
I've tested this on Mac and was not able to get the C++ demo to run due to two separate errors. Some issue is arising with a `vtkPolyLine` object that is causing the program to crash. If I just remove the code related to that, then the program still doesn't run because the Mac I have access to only supports up to OpenGL 2.1 but 3.2 is needed.

Having said that, if you still want to try to run the C++ demo, the same caveat in the Linux section about optional VTK dependencies applies here as well. Before running the following commands, install [Homebrew](https://brew.sh/).

    xcode-select --install # If you don't already have CLI dev tools installed
    brew update
    brew upgrade
    brew install qt5 libxml2 vtk gcc python3 cmake fmt boost eigen make
    # Below this is for building the C++ demo, it's not needed for
    # the python demo
    export PATH=/usr/local/bin:$PATH
    hash -r
    cd cpp
    mkdir build
    cd build
    CXX=/usr/local/bin/g++-11 CC=/usr/local/bin/gcc-11 cmake .. -DQt5_DIR=$(brew --prefix qt5)/lib/cmake/Qt5
    gmake

### Windows

I tried for a long time to get everything installed on Windows, but got frustrated and gave up. The C++ and Python demos should theoretically run on windows; you'll need to install the following dependencies:

- [Git](https://gitforwindows.org/)
- [CMake](https://cmake.org/download/)
- [Qt](https://www.qt.io/download-open-source) (install version 5.15.2)
- [Visual Studio (for its compiler)](https://visualstudio.microsoft.com/downloads/)
- [Python](https://www.python.org/downloads/)
- [FMT](https://github.com/fmtlib/fmt)
- [Boost](https://www.boost.org/)
- [Ninja](https://github.com/ninja-build/ninja/releases) (don't use this link to install it, run `python -m pip install ninja` instead)
- [VTK](https://gitlab.kitware.com/vtk/vtk/-/blob/master/Documentation/dev/build.md) (there may be precompiled binaries I just didn't see any on a quick look at the VTK website)

## Running

All these instructions assume you are in the `codefest22-vtk-demo` directory.

### C++

    cd cpp/python_helper
    python3 gen_data.py
    cd ../build
    ./joelsvtkdemo
    # On Mac, run:
    # ./joelsvtkdemo.app/Contents/MacOS/joelsvtkdemo

### Python
I was able to run this demo on Mac (as well as Linux of course)

    cd python
    python3 pythondemo.py

### JavaScript
Doesn't need to be firefox that's just what I use. If you can't open files from the command line just right click -> open in firefox in your file browser.

    cd js
    firefox vtkjs.html

## Additional Sources
- [JSON Library](https://github.com/nlohmann/json)
- Fish Models: [One](https://free3d.com/3d-model/3d-fish-model-low-poly-63627.html), [Two](https://www.cgtrader.com/items/179902/download-page), [Three](https://free3d.com/3d-model/bluegreen-reef-chromis-v2--439073.html)
- [CMake template I worked from](https://github.com/euler0/mini-cmake-qt/tree/qt5)
- [PyQt5 template I worked from](https://gist.github.com/paskino/7b7ed6e4541d682bd5ce5d521505dd8f)
- [Building Dataset](https://www.opendataphilly.org/dataset/buildings)
- [Address to Lat/Long tool](https://www.geocod.io/upload/)
- [VTK.js template I worked from](https://kitware.github.io/vtk-js/examples/SimpleCone.html)
