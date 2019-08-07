# v5 CMakeModule

This package is intended as what the students are aiming for when performing the 
reorganization of their code to use the ATLAS CMake macros.

# Getting the Package

Note that in this case, you should [clone the repository recursively](https://riptutorial.com/git/example/11913/clone-recursively)
```
git clone --recursive https://gitlab.cern.ch/usatlas-computing-bootcamp/v5-cmakemodule.git
```
(NOTE : The precise repository may not be the same if the user is using their own space.)

# Compilation

In either case, the user should start by pulling the ATLAS `AnalysisBase:21.2.75` docker image
and entering into the docker image as below
```
docker run --rm -it -v $PWD:/home/atlas/Bootcamp atlas/analysisbase:21.2.75 bash
```
which will place the entire file structure within the `/home/atlas/Bootcamp` directory
of the image.  Go one level above the directory of the repository and create a build directory.
In this directory, perform the CMake configuration and compilation
```
cd ../v5-cmakemodule
mkdir build
cd build
cmake ../source
make
```
which will produce a locally executable `AnalysisPayload` again.

# Output

The output file contains histograms with the number of jets and the dijet invariant mass of the two leading jets. Both historams are created for the following selections

- All jets in the event
- Jets passing the selection pT>50 GeV and |eta|<2.5
- All calibrated jets in the event
- Calibrated jets passing the selection pT>50 GeV and |eta|<2.5
