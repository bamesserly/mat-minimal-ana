//==============================================================================
// Loop entries, make cuts, fill histograms.
// * Uses the New Systematics Framework and "Universe" objects.
// * loop universes, make cuts and fill histograms with the correct lateral
// shifts and weights for each universe.
// * TChain --> PlotUtils::ChainWrapper.
// * MnvHXD --> PlotUtils::HistWrapper.
// * Genie, flux, non-resonant pion, and some detector systematics calculated.
//==============================================================================
#include "CVUniverse.h"
#include "PlotUtils/ChainWrapper.h"
#include "PlotUtils/makeChainWrapper.h"
#include "PlotUtils/HistWrapper.h"
#include "PlotUtils/GenieSystematics.h"
#include "PlotUtils/FluxSystematics.h"
#include "PlotUtils/MnvTuneSystematics.h"
#include "LateralSystematics.h"
#include <iostream>

// ROOT's interpreter, CINT, doesn't understand some legitimate c++ code so we
// shield it.
#ifndef __CINT__
#include "plotting_functions.h"
#endif

bool PassesCuts(CVUniverse& univ) {
  // Cut on muon energy (a variable we're varying in a systematic
  // universe) to show that the cut is applied separately to each
  // universe
  return univ.GetInt("isMinosMatchTrack") == 1 &&
         univ.GetDouble("CCNuPionInc_muon_qpqpe") < 0.0 &&
         univ.GetInt("tdead") <= 1&&
         univ.GetMuonE() > 2e3;
}
//======================================================================

// Get container of systematics
std::map< std::string, std::vector<CVUniverse*> > 
    GetErrorBands(PlotUtils::ChainWrapper* chain) {

  typedef std::map< std::string, std::vector<CVUniverse*> > SystMap;

  SystMap error_bands;

  // CV
  error_bands[std::string("CV")].push_back( new CVUniverse(chain) );

  //Detector systematics, lateral shifts
  error_bands[std::string("MuonERC")].push_back(
      new MuonERangeCurvatureShiftUniverse(chain, -1));

  error_bands[std::string("MuonERC")].push_back(
      new MuonERangeCurvatureShiftUniverse(chain, +1));
  
  //Flux
  int n_flux_universes = 50;
  SystMap flux_systematics = 
      PlotUtils::GetFluxSystematicsMap<CVUniverse>(chain,n_flux_universes);
  error_bands.insert(flux_systematics.begin(), flux_systematics.end());

  //GENIE
  SystMap genie_systematics = 
      PlotUtils::GetGenieSystematicsMap<CVUniverse>(chain,true);// change that true to a switch on do_nonrespi_tune
  error_bands.insert(genie_systematics.begin(), genie_systematics.end());

  return error_bands;
}
//======================================================================

// Main
void runEventLoop() {

  // Make a chain of events
  PlotUtils::ChainWrapper* chain = makeChainWrapperPtr("playlist.txt", "CCNuPionInc");

  // Make a map of systematic universes
  std::map< std::string, std::vector<CVUniverse*> > error_bands = GetErrorBands(chain);

  // Use the vector of systematic universes to make your MnvH1D
  PlotUtils::HistWrapper<CVUniverse> hw_enu("hw_enu", "E_{#nu} NEW Method",
                                            nbins, xmin, xmax, error_bands);

  // Do not use nu-e constraint by default (for purposes of template)
  PlotUtils::DefaultCVUniverse::SetNuEConstraint(false);
  // Use minervame1A by default (for purposes of template)
  PlotUtils::DefaultCVUniverse::SetPlaylist("minervame1A");
  // Assume muon neutrino analysis by default (for purposes of template)
  PlotUtils::DefaultCVUniverse::SetAnalysisNuPDG(14);

  //=========================================
  // Entry Loop
  //=========================================
  for(int i=0; i<chain->GetEntries(); ++i){
    if(i%500000==0) std::cout << (i/1000) << "k " << std::endl;

    //=========================================
    // For every systematic, loop over the universes, and fill the
    // appropriate histogram in the MnvH1D
    //=========================================
    for (auto band : error_bands){
      std::vector<CVUniverse*> error_band_universes = band.second;
      for (auto universe : error_band_universes){

        // Tell the Event which entry in the TChain it's looking at
        universe->SetEntry(i);

        //=========================================
        // CUTS in each universe
        //=========================================
        if(PassesCuts(*universe)){

          // Fill the MnvH1D's universe's histogram
          hw_enu.univHist(universe)->Fill(universe->GetEnu(), 
                                          universe->GetWeight());

        } // End if passed cuts
      } // End band's universe loop
    } // End Band loop
  } //End entries loop

  
  // This function copies the MnvH1D's CV histo to each error band's CV histos.
  hw_enu.SyncCVHistos();

  //=========================================
  // Plot stuff
  //=========================================

  // DrawMCWithErrorBand
  PlotCVAndError(hw_enu.hist, "TEST");

  // Plot Error Summary
  PlotErrorSummary(hw_enu.hist, "TEST");

  //Plot individual universes
  unsigned int universe = 0;
  PlotVertUniverse("EmuRangeCurve", universe, "TEST", hw_enu.hist);
  universe = 1;
  PlotVertUniverse("EmuRangeCurve", universe, "TEST", hw_enu.hist);

  for (int i = 0; i < 5; ++i) PlotVertUniverse("Flux", i, "TEST", hw_enu.hist);
  
  //Plot the Bands
  PlotVertBand("EmuRangeCurve", "TEST", hw_enu.hist);
  PlotVertBand("Flux", "TEST", hw_enu.hist);

  PlotTotalError(hw_enu.hist, "TEST");


  //=========================================

  // We have to delete the events ourselves. Should use some sort of
  // smart pointer to fix that
  for(auto band : error_bands){
    std::vector<CVUniverse*> band_universes = band.second;
    for(unsigned int i_universe = 0; i_universe < band_universes.size(); ++i_universe)
      delete band_universes[i_universe];
  } 

  std::cout << "Success" << std::endl;
}
