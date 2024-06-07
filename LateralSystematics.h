#include <iostream>

#include "CVUniverse.h"

// An example of a lateral shift, where we have to change the value of
// one variable (in this case, muon energy). We need to give the
// number of sigma to the constructor
class MuonERangeCurvatureShiftUniverse : public CVUniverse {
 public:
  MuonERangeCurvatureShiftUniverse(PlotUtils::ChainWrapper* chw, double nsigma)
      : CVUniverse(chw, nsigma) {}

  // MeV
  virtual double GetMuonE() const override {
    double muon_E_shift = GetDouble("CCNuPionInc_minosRangeCurveShift");
    double shift_val = m_nsigma * muon_E_shift;
    return shift_val + CVUniverse::GetMuonE();
  }

  virtual std::string ShortName() const override { return "EmuRangeCurve"; }
  virtual std::string LatexName() const override {
    return "MINOS Muon Energy - Range & Curvature";
  }
};
