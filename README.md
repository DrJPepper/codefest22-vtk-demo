# Philly Codefest VTK Workshop

This is still a work in progress currently

## Installation

All these instructions assume you are in the `codefest22-vtk-demo` directory.

### Linux
You need to install `vtk`, `gcc`, `cmake`, `fmt`, `boost`, `eigen`, `python3` and `qt5` via your distros package manager. You possibly also need to install the optional VTK dependencies listed in the following Arch specific install process.

**Arch Linux specific commands:**

    # CMake/VTK expects a bunch of optional dependencies even though I don't use them
                                                     # <-- Everything beyond this point may be needed for cmake and make to succeed
    pacman -Syu vtk qt5 cmake fmt boost eigen python ospray level-zero-loader level-zero-driver openvr python-mpi4py liblas adios2 libharu cgns utf8cpp pdal gl2ps glew jdk-openjdk jre-openjdk
    mkdir build
    cd build
    cmake ..
    make

### Mac
I won't have access to a Mac until Wednesday to test this, but this should be what's needed to get the C++ and python demos to run. Before running the following commands, install [Homebrew](https://brew.sh/). If you need to modify this to get it to work for you, please make a pull request with the corrected installation process.

    xcode-select --install
    brew update
    brew upgrade
    brew install qt5 libxml2 vtk gcc python3 cmake fmt boost eigen
    mkdir build
    cd build
    CXX=/usr/local/bin/gcc-11 CC=/usr/local/bin/gcc-11 cmake .. -DQt5_DIR=$(brew --prefix qt5)/lib/cmake/Qt5
    make

### Windows

I tried for a long time to get everything installed on Windows but got frustrated and gave up. The C++ and Python demo should theoretically run on windows; you'll need to install the following dependencies (not needed for JS demo of course):

- [Git](https://gitforwindows.org/)
- [CMake](https://cmake.org/download/)
- [Qt](https://www.qt.io/download-open-source) (install version 5.15.2)
- [Visual Studio (for its compiler)](https://visualstudio.microsoft.com/downloads/)
- [Python](https://www.python.org/downloads/)
- [FMT](https://github.com/fmtlib/fmt)
- [Boost](https://www.boost.org/)
- [Ninja](https://github.com/ninja-build/ninja/releases) (don't use this link to install it run `python -m pip install ninja` instead)
- [VTK](https://gitlab.kitware.com/vtk/vtk/-/blob/master/Documentation/dev/build.md) (there may be precompiled binaries I just didn't see any on a quick look at the VTK website)

## Running
Todo

## Additional Sources
- [JSON Library](https://github.com/nlohmann/json)
- Fish Models: [One](https://free3d.com/3d-model/3d-fish-model-low-poly-63627.html), [Two](https://www.cgtrader.com/items/179902/download-page), [Three](https://free3d.com/3d-model/bluegreen-reef-chromis-v2--439073.html)
