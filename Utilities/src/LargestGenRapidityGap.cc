#include "ForwardAnalysis/Utilities/interface/LargestGenRapidityGap.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "ForwardAnalysis/Utilities/interface/EtaComparator.h"

#include <vector>
#include <algorithm>

namespace forwardAnalysis {

void LargestGenRapidityGap::operator()(reco::GenParticleCollection const& genParticles, 
                                                    math::XYZTLorentzVector& genGapLow,
                                                    math::XYZTLorentzVector& genGapHigh){
  // Copy and sort gen particles in eta
  std::vector<math::XYZTLorentzVector> genParticlesSort(0);
  reco::GenParticleCollection::const_iterator genpart = genParticles.begin();
  reco::GenParticleCollection::const_iterator genpart_end = genParticles.end();  
  for(; genpart != genpart_end; ++genpart){
        if( genpart->status() != 1 ) continue;

      if((genpart->eta() >= etaEdgeLow_) && (genpart->eta() <= etaEdgeHigh_))
         genParticlesSort.push_back( genpart->p4() );
   }
   std::stable_sort(genParticlesSort.begin(), genParticlesSort.end(), LessByEta<math::XYZTLorentzVector>());

   // Cases: 0, 1 or > 1 particles in selected range
   math::XYZTLorentzVector def_vec(0.,0.,0.,0.);
   if( genParticlesSort.size() == 0 ){
      genGapLow = def_vec; genGapHigh = def_vec;
      return;
   } else if( genParticlesSort.size() == 1 ){
      genGapLow = def_vec;
      genGapHigh = genParticlesSort[0];
      return;
   } else{
      //FIXME; There must be a STL algorithm for this
      double deltaEtaMax = 0.;
      std::vector<math::XYZTLorentzVector>::const_iterator genPartDeltaEtaMax = genParticlesSort.end();
      std::vector<math::XYZTLorentzVector>::const_iterator genpart = genParticlesSort.begin();
      std::vector<math::XYZTLorentzVector>::const_iterator genpart_end = genParticlesSort.end();
      for(; genpart != genpart_end; ++genpart){
         std::vector<math::XYZTLorentzVector>::const_iterator next = genpart + 1;
         double deltaEta = ( next != genpart_end ) ? ( next->eta() - genpart->eta() ) : 0.;
         if( deltaEta > (deltaEtaMax - 0.0001) ){
            deltaEtaMax = deltaEta;
            genPartDeltaEtaMax = genpart;
         } 
      }
      if( genPartDeltaEtaMax != genpart_end ){
         std::vector<math::XYZTLorentzVector>::const_iterator next = genPartDeltaEtaMax + 1;
         if( next != genpart_end ){
            genGapLow = (*genPartDeltaEtaMax);
            genGapHigh = (*next);
         } else{
            genGapLow = def_vec;
            genGapHigh = (*genPartDeltaEtaMax);
         }
      } else{
         genGapLow = def_vec; genGapHigh = def_vec;
         return;
      }
   } 
}

}
