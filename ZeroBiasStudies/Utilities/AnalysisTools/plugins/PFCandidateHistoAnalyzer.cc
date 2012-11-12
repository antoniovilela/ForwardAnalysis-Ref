#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/HistoAnalyzer.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

typedef HistoAnalyzer<reco::PFCandidateCollection> PFCandidateHistoAnalyzer;

//DEFINE_FWK_MODULE( PFCandidateHistoAnalyzer );
