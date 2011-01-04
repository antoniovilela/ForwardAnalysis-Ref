#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

 typedef SingleObjectSelector<
           reco::PFCandidateCollection, 
           StringCutObjectSelector<reco::PFCandidate> 
         > PFCandidateSelector;

DEFINE_FWK_MODULE( PFCandidateSelector );
