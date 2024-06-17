# MAT-MINERvA Setup Guide for a Personal Unix Machine

- [Introduction](#introduction)
- [Prerequisites](#prerequisites)
- [1. Conda Environment](#conda-environment)
- [2. MINERvA Large Files](#minerva-large-files)
- [3. Install MAT-MINERvA](#install-mat-minerva)
- [4. Install Tutorial](#install-tutorial)
- [Project Directory Structure](#final-directory-structure)

-------------------------

## Introduction
Here is my experience doing a full MAT-MINERvA, periferals, and cross section tutorial install on my Mac m1 personal machine 2024-06. It takes me about 5 minutes.

While this was all done on mac, I fully expect it to work as-is on linux and even on Windows, specifically with [WSL](https://learn.microsoft.com/en-us/windows/wsl/about).

## Prerequisites
git and conda. I use homebrew to manage all system-wide packages, and I used it to download git and conda.

## Conda Environment
Use conda to install root and cmake:
```
conda create -n root_env
conda activate root_env
conda install root=6.28.12 -c conda-forge
conda install cmake
```
There's an issue with the latest version of root and TUnfold, hence the slightly older version of root. This has been fixed on a MAT-MINERvA branch which I have yet to test.

## MINERvA Large Files
MAT-MINERvA installation requires some data files for fluxes, constants, and weights, all totalling to about 2.5 G. Anything resembling a realistic analysis of MINERvA data (including our xsec tutorial) will quickly require these files. If you aren't here to do MINERvA analysis, then you should just install the [`MAT`](https://github.com/MinervaExpt/MAT), and then you won't need them.

**For MINERvA collaborators:**
* get a kerberos ticket
* install `cvs` (e.g. `brew install cvs`)

The needed files will be downloaded automatically in the next step

**OR For Non MINERvA collaborators:**
* You need two folders: `MParamFiles` and `MATFluxAndReweightFiles`. Get them from a collaborator.
* Put them both into a single folder and note the location for the next step.

Hopefully before 2025, these files will be publically available, along with the entire MINERvA dataset.

## Install MAT-MINERvA
**For MINERvA collaborators** (with `cvs` and a kerberos ticket):
```
mkdir MATAna; cd MATAna
git clone https://github.com/MinervaExpt/MAT-MINERvA.git
mkdir -p opt/build && cd opt/build
cmake ../../MAT-MINERvA/bootstrap -DCMAKE_INSTALL_PREFIX=`pwd`/.. -DCMAKE_BUILD_TYPE=Release
make install
source setup.sh
cd ../..
```

**OR For Non MINERvA collaborators:**
First set the location of your large files from the previous step, then install:
```
export MINERVA_LARGE_FILES=/path/to/my/files
mkdir MATAna; cd MATAna
git clone https://github.com/MinervaExpt/MAT-MINERvA.git
mkdir -p opt/build && cd opt/build
cmake ../../MAT-MINERvA/bootstrap -DCMAKE_INSTALL_PREFIX=`pwd`/.. -DCMAKE_BUILD_TYPE=Release -DFLUX_FILE_DIR=$MINERVA_LARGE_FILES
make install
source setup.sh
cd ../..
```
This downloads and installs `MAT-MINERvA`, `MAT`, and `UnfoldUtils`.

## Install Tutorial
```
git clone https://github.com/MinervaExpt/GENIEXSecExtract.git
mkdir opt/buildGENIEXSecExtract; cd opt/buildGENIEXSecExtract
cmake ../../GENIEXSecExtract -DCMAKE_INSTALL_PREFIX=`pwd`/.. -DCMAKE_BUILD_TYPE=Release
make install
cd ../..

git clone https://github.com/MinervaExpt/MINERvA-101-Cross-Section.git
mkdir opt/buildTutorial; cd opt/buildTutorial
cmake ../../MINERvA-101-Cross-Section -DCMAKE_INSTALL_PREFIX=`pwd`/.. -DCMAKE_BUILD_TYPE=Release
make install
cd ../..
```

Test that the cross section tutorial works with `runEventLoop --help`.

And you may want my bare-bones example that focuses more on the core features of the MAT (and less on MAT-MINERvA):
```
https://github.com/bamesserly/mat-minimal-ana.git
scp -r <my_username>@minervagpvm01.fnal.gov:/pnfs/minerva/persistent/users/bmesserl/pions/20190824/merged/mc/ME1A/CCNuPionInc_mc_AnaTuple_run00110000_Playlist.root .
echo $PWD/CCNuPionInc_mc_AnaTuple_run00110000_Playlist.root > playlist.txt
```
and run the event loop:
```
root -l -b load.C+ runEventLoop.C+
```
Note that the last two lines involve a download from the MINERvA gpvms. Look forward to the day when MINERvA data is public.

## Final directory structure:
Here's my final directory structure now:
```
/Users/ben/ % tree -L 2 minerva/
minerva
├── minerva_large_files
│   ├── MParamFiles
│   └── MATFluxAndReweightFiles
└── MATAna
    ├── GENIEXSecExtract
    ├── MAT
    ├── MAT-MINERvA
    ├── MINERvA-101-Cross-Section
    ├── UnfoldUtils
    ├── mat-minimal-ana
    └── opt
```
