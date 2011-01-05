#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SortCollectionSelector.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "ForwardAnalysis/Utilities/interface/EtaComparator.h"

typedef ObjectSelector<
          SortCollectionSelector<
            reco::CandidateView,
            GreaterByEta<reco::Candidate>
          >
        > EtaMaxCandViewSelector;

typedef ObjectSelector<
          SortCollectionSelector<
            reco::CandidateView,
            LessByEta<reco::Candidate>
          >
        > EtaMinCandViewSelector;

DEFINE_FWK_MODULE( EtaMaxCandViewSelector );
DEFINE_FWK_MODULE( EtaMinCandViewSelector );
