#ifndef ForwardAnalysis_Utilities_LargestGenRapidityGap_h
#define ForwardAnalysis_Utilities_LargestGenRapidityGap_h

/* \class LargestGenRapidityGap
 *
 * Usage: LargestGenRapidityGap(a,b)(...) 
 *
 */

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "DataFormats/Math/interface/LorentzVector.h"

namespace forwardAnalysis {

class LargestGenRapidityGap {
  public:
     LargestGenRapidityGap(double etaEdgeLow, double etaEdgeHigh):
        etaEdgeLow_(etaEdgeLow), etaEdgeHigh_(etaEdgeHigh) {}
     ~LargestGenRapidityGap() {}

     void operator()(reco::GenParticleCollection const&,math::XYZTLorentzVector&,
                                                        math::XYZTLorentzVector&);
  private:
     double etaEdgeLow_, etaEdgeHigh_;
};

} // namespace
#endif
