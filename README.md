# Minimal MAT-MINERvA Example

This is my original (extremely bare bones) prototype/example of a MAT-MINERvA
analysis.

This example highlights a root file entry loop and histogram-filling with
"many-universe" systematics, using all of the essential awesome features of the
MAT.

Currently, to get this running, you basically need to be a MINERvA
collaborator; MAT-MINERvA is required beyond the publically-available MAT, and
it references a sample root data file that still exists but is only accessible
from minerva gpvms.

The code is still valuable as a reference, or if you are a MINERvA
collaborator, you can set it all up here:
[here](https://github.com/MinervaExpt/MAT-MINERvA).

It was last tested circa 2019-09 but I have no reason to believe it won't work.

runEventLoop.C
* Creates a PlotUtils::ChainWrapper (like TChain) of events from playlist.txt.
* Creates a HistWrapper (like an MnvHXD) object for Enu out of a couple sample
  systematics
* Loops over ChainWrapper entries
    * Loops over systematics
        * Check if universe passes cuts
            * fill Histwrapper's histograms
* plot a bunch of stuff

To run: `root -l -b load.C+ runEventLoop.C+`

CVUniverse.h
Defines a "central value universe". Give it a name, a way to access the CV
weight, and a way to access various variables.
Systematically shifted universes will inherit from this base CV class, and 
overwrite their GetWeight or GetVariable functions.

LateralSystematics.h
Contains an example "lateral" systematic that shifts the muon energy. This
shift only affects muon energy, so GetMuonE is the only function overwritten
from the base.

--B Messerly 2024-06
