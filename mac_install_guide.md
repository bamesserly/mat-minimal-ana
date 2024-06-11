# MAT-MINERvA Setup Guide for a Personal Mac
Here is my experience doing a full MAT-MINERvA and periferals on my Mac m1 personal machine on 2024-06-10.

## Detailed Instructions

### 1. Install git and conda
I use homebrew to manage all machine-wide packages. I used it to download git and conda. Not discussing those here.

### 2. Create and activate conda environment
```
conda create -n root_env
conda activate root_env
conda install root=6.28.12 -c conda-forge
conda install cmake
```
Can't use the latest conda version of root at the moment due to an issue with TUnfold/RooUnfold. Andrew says he has a fix for this but so far haven't tested it.

### 3. Get some large MINERvA data files
You need MINERvA data files in `MParamFiles` and Flux and Reweight files.
  A. IF you have MINERvA computing privileges you can easily download them:
  * Get cvs. I used homebrew: `brew install cvs`.
  * Get a kerberos ticket `kinit <my_username>`.
  * Proceed with the next instructions. These files will get downloaded automatically.
  B. If you DON'T have MINERvA computing, you need to get these files from a collaborator. There are two folders: `MParamFiles` and `MATFluxAndReweightFiles`. Put them both into a single folder. Consider a directory structure like this:
```
/Users/ben/ % tree -L 2 minerva/
minerva
├── minerva_large_files
│   ├── MParamFiles
│   └── MATFluxAndReweightFiles
└── MATAna
```
Where MATAna will serve as our top working directory from here on out. I'll proceed assuming you manually downloaded the large files and they're in `minerva_large_files/`.

### 5. Install MAT-MINERvA:
```
cd MATAna
git clone https://github.com/MinervaExpt/MAT-MINERvA.git
mkdir -p opt/build && cd opt/build
cmake ../../MAT-MINERvA/bootstrap -DCMAKE_INSTALL_PREFIX=`pwd`/.. -DCMAKE_BUILD_TYPE=Release -DFLUX_FILE_DIR=/Users/ben/minerva/minerva_large_files
make install
source setup.sh
cd ../..
```
This downloads and installs `MAT-MINERvA`, `MAT`, and `UnfoldUtils`. Note, we've passed `minerva_large_files/` with the `-DFLUX_FILE_DIR` flag. Skip that flag if you have a kerberized fnal connection and cvs installed.

### 6. Install `GENIEXSecExtract`
```
git clone https://github.com/MinervaExpt/GENIEXSecExtract.git
mkdir opt/buildGENIEXSecExtract; cd opt/buildGENIEXSecExtract
cmake ../../GENIEXSecExtract -DCMAKE_INSTALL_PREFIX=`pwd`/.. -DCMAKE_BUILD_TYPE=Release
make install
cd ../..
```
This is optional. Only needed for parts of the Tutorial.

### 7. Install the Cross Section Tutorial
```
git clone https://github.com/MinervaExpt/MINERvA-101-Cross-Section.git
mkdir opt/buildTutorial; cd opt/buildTutorial
cmake ../../MINERvA-101-Cross-Section -DCMAKE_INSTALL_PREFIX=`pwd`/.. -DCMAKE_BUILD_TYPE=Release
make install
cd ../..
```

Here's my final directory structure now:
```
Projects % tree -L 1 MATAna
MATAna
├── GENIEXSecExtract
├── MAT
├── MAT-MINERvA
├── MINERvA-101-Cross-Section
├── UnfoldUtils
└── opt
```

Test that the cross section tutorial works with `runEventLoop --help`.

### 8. (Optional) Setup and run my bare bones minimal mat event loop example
```
https://github.com/bamesserly/mat-minimal-ana.git
scp -r <my_username>@minervagpvm01.fnal.gov:/pnfs/minerva/persistent/users/bmesserl/pions/20190824/merged/mc/ME1A/CCNuPionInc_mc_AnaTuple_run00110000_Playlist.root .
echo $PWD/CCNuPionInc_mc_AnaTuple_run00110000_Playlist.root > playlist.txt
```
and run my the event loop:
`root -l -b load.C+ runEventLoop.C+`

## Feeling lucky?
Here's all the setup in one code block. Make sure you have `conda` and `git`. If you're a MINERvA collaborator, get a kerberos ticket and `cvs`. If you're not a collaborator, place your manually-downloaded `MParamFiles` and `MATFluxAndReweightFiles` in a `minerva_large_files` and set it to `export MINERVA_LARGE_FILES`.
```
# conda environment
conda create -n root_env
conda activate root_env
conda install root=6.28.12 -c conda-forge
conda install cmake

# MAT-MINERvA + Peripherals
mkdir MATAna; cd MATAna
git clone https://github.com/MinervaExpt/MAT-MINERvA.git
mkdir -p opt/build && cd opt/build
cmake ../../MAT-MINERvA/bootstrap -DCMAKE_INSTALL_PREFIX=`pwd`/.. -DCMAKE_BUILD_TYPE=Release -DFLUX_FILE_DIR=$MINERVA_LARGE_FILES
make install
source setup.sh
cd ../..

# GENIEXSecExtract
git clone https://github.com/MinervaExpt/GENIEXSecExtract.git
mkdir opt/buildGENIEXSecExtract; cd opt/buildGENIEXSecExtract
cmake ../../GENIEXSecExtract -DCMAKE_INSTALL_PREFIX=`pwd`/.. -DCMAKE_BUILD_TYPE=Release
make install
cd ../..

# XSec Tutorial
git clone https://github.com/MinervaExpt/MINERvA-101-Cross-Section.git
mkdir opt/buildTutorial; cd opt/buildTutorial
cmake ../../MINERvA-101-Cross-Section -DCMAKE_INSTALL_PREFIX=`pwd`/.. -DCMAKE_BUILD_TYPE=Release
make install
cd ../..

# MAT Bare Bones Example (kerberos required)
https://github.com/bamesserly/mat-minimal-ana.git
scp -r <my_username>@minervagpvm01.fnal.gov:/pnfs/minerva/persistent/users/bmesserl/pions/20190824/merged/mc/ME1A/CCNuPionInc_mc_AnaTuple_run00110000_Playlist.root .
echo $PWD/CCNuPionInc_mc_AnaTuple_run00110000_Playlist.root > playlist.txt
```
