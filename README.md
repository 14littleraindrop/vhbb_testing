# v3 GitModule Submodule JetSelector

This package is intended as what the students are aiming for when performing the 
reorganization of their code which allows for the use of submodules in Git

# Getting the Package

Note that in this case, you should [clone the repository recursively](https://riptutorial.com/git/example/11913/clone-recursively)
```
git clone --recursive https://gitlab.cern.ch/usatlas-computing-bootcamp/v3-gitmodule-submodule-jetselector.git
```
(NOTE : The precise repository may not be the same if the user is using their own space.)

# Compilation

In either case, the user should start by pulling the ATLAS `AnalysisBase,21.2.75` docker image
and entering into the docker image as below
```
docker run --rm -it -v $PWD:/home/atlas/Bootcamp atlas/analysisbase:21.2.75 bash
```
which will place the entire file structure within the `/home/atlas/Bootcamp` directory
of the image.  Go one level above the directory of the repository and create a build directory.
In this directory, perform the CMake configuration and compilation
```
cd ..
mkdir build
cd build
cmake ../v3-gitmodule-submodule-jetselector
make
```
which will produce a locally executable `AnalysisPayload` again.

# Output

## Basic Kinematic Cuts

This code should produce a pair of histograms that shows the number of jets and the
dijet invariant mass of the two leading jets among only those jets that pass the 
selections of pT>50 GeV and |eta|<2.5.

## With B-Tagging

For this, the user will have to implement the `bool JetSelector::isJetBFlavor(const xAOD::Jet* jet)` function.
Then, they should output the same two histograms (the number of jets and the dijet invariant mass of the two 
highest pT jets) but only for the subset of jets which have been b-tagged.