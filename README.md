Example of a new, systematics-focused method to loop over events.

REQUIRES PlotUtils circa 20190901.

runEventLoop.C
* Creates a PlotUtils::ChainWrapper (like TChain) of events from playlist.txt.
* Creates a HistWrapper (like an MnvHXD) object for Enu out of a couple sample
  systematics
* Loops over ChainWrapper entries
* * Loops over systematics
* * * Check if universe passes cuts
* * * * fill Histwrapper's histograms
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

--B Messerly 20190909
