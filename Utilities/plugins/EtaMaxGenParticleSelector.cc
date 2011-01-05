#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SortCollectionSelector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "ForwardAnalysis/Utilities/interface/EtaComparator.h"

typedef ObjectSelector<
          SortCollectionSelector<
            reco::GenParticleCollection,
            GreaterByEta<reco::Candidate>
          >
        > EtaMaxGenParticleSelector;

typedef ObjectSelector<
          SortCollectionSelector<
            reco::GenParticleCollection,
            LessByEta<reco::Candidate>
          >
        > EtaMinGenParticleSelector;

DEFINE_FWK_MODULE( EtaMaxGenParticleSelector );
DEFINE_FWK_MODULE( EtaMinGenParticleSelector );
