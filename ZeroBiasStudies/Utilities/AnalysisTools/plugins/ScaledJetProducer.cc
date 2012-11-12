#include "FWCore/Framework/interface/MakerMacros.h"

#include "Utilities/AnalysisTools/interface/ScaledObjectProducer.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/JetCollection.h"

#include <vector>

typedef analysisTools::ScaledObjectProducer<edm::View<reco::Jet>,
                                            std::vector<reco::Jet> > ScaledJetProducer;

DEFINE_FWK_MODULE(ScaledJetProducer);
