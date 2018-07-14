# TrussTables
2D Truss solver.

![trusstables](https://user-images.githubusercontent.com/41187898/42723379-4c42bc4c-8765-11e8-83b1-3e5d8699c136.png)

__TrussTables__ performs linear static analysis of 2D truss structures, via the direct stiffness method. The application is written in [C++](https://en.wikipedia.org/wiki/C%2B%2B "C++ Wikipedia"), using the popular [Qt framework](https://www.qt.io/ "Qt").

__TrussTables__ features:

* Truss model input (editing, exporting, and saving)
* Model checking and validation
* Loads input
* Model analysis options
* Model analysis
* Results view and export

Model analysis may include the following options:

* Joint/nodal loads
* predescribed joint displacements
* Thermal loads
* Fabrication error loads
* Influence lines

## Installing

Binaries are provided for Linux ([AppImage](https://en.wikipedia.org/wiki/AppImage "AppImage")) and Windows, see (link to releases)

**Note:**
Linux AppImage executables are built on, tested and target, Linux mint 18.2 LTS and Ubuntu 16.04 LTS.
Simply download and;
```
chmod a+x TrussTables.AppImage
./ TrussTables.AppImage
```
Optionally, for desktop integration;
```
./ TrussTables.AppImage install
```

## Motivation

* [Finite element analysis](https://en.wikipedia.org/wiki/Finite_element_method "Finite element method")
* [C++](https://en.wikipedia.org/wiki/C%2B%2B "C++ Wikipedia")
* [Qt framework](https://www.qt.io/ "Qt")
* Hopefully, a usefull tool to :
  * Students
  * Engineers
  * Coding enthusiast
  * Anybody looking to solve truss problems!

## Built With

* [C++](https://en.wikipedia.org/wiki/C%2B%2B "C++ Wikipedia") - Code
* [Qt framework](https://www.qt.io/ "Qt") - GUI Toolkit
* [GSL](https://www.gnu.org/software/gsl/ "GNU Scientific Library") - Numerical computation library
* [oxygen-icons](https://github.com/KDE/oxygen-icons "oxygen-icons") - UI icons
* [AppImage](https://en.wikipedia.org/wiki/AppImage "AppImage") - Packaging and deployment (Linux)
* [NSIS](http://nsis.sourceforge.net/Main_Page "NSIS") - Install system (Windows)

## Versioning

TrussTables uses [Semver](https://semver.org/ "SemVer")

## Authors

Just me for now!

* __Ambrose Louis Okune__ - initial work - [rembosa](https://github.com/rembosa)

## License

__TrussTables__ is licensed under the GPLv3 - see the [LICENSE](https://github.com/rembosa/TrussTables/blob/master/LICENSE) file for details.
