// ========================================================================
// Base class for an un-systematically shifted (i.e. CV) universe.  Implement
// your own base class with the functions you need. I've implemented GetEnu(),
// GetMuonE() and GetHadronE() as examples: you'll have other variables you
// want.
//
// To add a systematic, inherit from this class and override whichever
// functions you need to. For a "vertical" error, this will mean overriding the
// GetWeight() function to modify the event weight. For a "lateral" error, this
// will mean overriding the function that calculates the quantity that is being
// shifted (muon energy, or hadronic energy or whatever).
//
// For examples of each of those two cases, see ./LateralSystematics.h and
// PlotUtils/GenieSystematics.h. For an example of how to put the whole thing
// together and actually *use* the classes, see the runEventLoop.C macro in
// this directory. `root -l -b load.C+ runEventLoop.C+`
// ========================================================================
#ifndef CVUNIVERSE_H
#define CVUNIVERSE_H

#include "PlotUtils/DefaultCVUniverse.h"
#include "PlotUtils/ChainWrapper.h"
#include <iostream>

class CVUniverse : public PlotUtils::DefaultCVUniverse {
  public:
    // Constructor
    CVUniverse(PlotUtils::ChainWrapper* chw, double nsigma=0)
      : PlotUtils::DefaultCVUniverse(chw, nsigma)
      {}

    // Destructor
    virtual ~CVUniverse(){}

    // All functions we write here should be 'virtual', so that the universes
    // that inherit from CVUniverse can override them.

    // ========================================================================
    // Get Weight
    //
    // We override the various weight getting functions herein in different
    // vertical systematic universe classes.
    // ========================================================================
    virtual double GetWeight() const {
      double wgt_flux_and_cv=1., wgt_nrp=1., wgt_genie=1;

      // flux + cv
      //std::string playlist("minervame1a");
      SetPlaylist("minervame1a");
      double Enu  = GetDouble("mc_incomingE")*1e-3;
      int nu_type = GetInt("mc_incoming");
      wgt_flux_and_cv = GetFluxAndCVWeight(Enu, nu_type);

      // genie
      wgt_genie = GetGenieWeight();

      // mnvtune -- non-res pi
      wgt_nrp = GetNonResPiWeight();

      return wgt_flux_and_cv*wgt_genie*wgt_nrp;
    }


    // ========================================================================
    // Get Variable Functions
    // Write a virtual "Get" function for _any_ variable (coming directly from a
    // branch, or composed of several branches) that will be laterally shifted
    // or affected by the lateral shift of a systematic universe.
    //
    // We override some or all of these function in different systematic
    // universe classes located in LateralSystematics.h.
    // ========================================================================
    virtual double GetEnu() const { return GetMuonE()+GetHadronE(); }
    virtual double GetMuonE() const { return GetDouble("CCNuPionInc_muon_E");}
    virtual double GetHadronE() const { return GetDouble("CCNuPionInc_hadron_recoil_CCInc");}

};


#endif
