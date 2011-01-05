#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SortCollectionSelector.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "ForwardAnalysis/Utilities/interface/EtaComparator.h"

typedef ObjectSelector<
          SortCollectionSelector<
            reco::CandidateCollection,
            GreaterByEta<reco::Candidate>
          >
        > EtaMaxCandSelector;

typedef ObjectSelector<
          SortCollectionSelector<
            reco::CandidateCollection,
            LessByEta<reco::Candidate>
          >
        > EtaMinCandSelector;

DEFINE_FWK_MODULE( EtaMaxCandSelector );
DEFINE_FWK_MODULE( EtaMinCandSelector );
