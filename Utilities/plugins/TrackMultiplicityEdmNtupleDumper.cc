#ifndef DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_TrackMultiplicityEdmNtupleDumper_h
#define DiffractiveForwardAnalysis_SingleDiffractiveWAnalysis_TrackMultiplicityEdmNtupleDumper_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TrackMultiplicityEdmNtupleDumper : public edm::EDProducer {
public:
  TrackMultiplicityEdmNtupleDumper( const edm::ParameterSet & );
private:
  void produce( edm::Event &, const edm::EventSetup & );
  edm::InputTag tracksTag_;
};

#endif

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

using namespace reco;

TrackMultiplicityEdmNtupleDumper::TrackMultiplicityEdmNtupleDumper(const edm::ParameterSet& pset) {
	tracksTag_ = pset.getParameter<edm::InputTag>("TracksTag");

	std::string alias;
	produces<unsigned int>( alias = "trackMultiplicity" ).setBranchAlias( alias );
	produces<unsigned int>( alias = "trackMultiplicityEtaPlus" ).setBranchAlias( alias );
	produces<unsigned int>( alias = "trackMultiplicityEtaMinus" ).setBranchAlias( alias );
}

void TrackMultiplicityEdmNtupleDumper::produce(edm::Event& event, const edm::EventSetup& setup) {
	// Tracks
        edm::Handle<edm::View<Track> > trackCollectionH;
        event.getByLabel(tracksTag_,trackCollectionH);
        const edm::View<reco::Track>& trkColl = *(trackCollectionH.product());

	unsigned int ntracks = 0;
	unsigned int ntracks_plus = 0;
	unsigned int ntracks_minus = 0;
        for(edm::View<Track>::const_iterator track = trkColl.begin(); track != trkColl.end(); ++track){
		++ntracks;
		if(track->eta() >= 0.) ++ntracks_plus;
		else ++ntracks_minus;
	}
	std::auto_ptr<unsigned int> trackMultiplicity(new unsigned int(ntracks));
	std::auto_ptr<unsigned int> trackMultiplicityEtaPlus(new unsigned int(ntracks_plus));
	std::auto_ptr<unsigned int> trackMultiplicityEtaMinus(new unsigned int(ntracks_minus));

	event.put(trackMultiplicity,"trackMultiplicity");
	event.put(trackMultiplicityEtaPlus,"trackMultiplicityEtaPlus");
	event.put(trackMultiplicityEtaMinus,"trackMultiplicityEtaMinus");
}

//DEFINE_FWK_MODULE(TrackMultiplicityEdmNtupleDumper);
 
