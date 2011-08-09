
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class CastorActivityFilter : public edm::EDFilter {
    public:
       explicit CastorActivityFilter( edm::ParameterSet const& );
       ~CastorActivityFilter();

       virtual void beginJob();
       virtual bool filter( edm::Event&, edm::EventSetup const& );
   private:
       edm::InputTag castorRecHitTag_; 

       //double energyThresholdCastor_;
       double sumETotCastorMax_;
};

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

CastorActivityFilter::CastorActivityFilter(edm::ParameterSet const& pset):
  castorRecHitTag_( pset.getParameter<edm::InputTag>("CastorRecHitTag") ), 
  //energyThresholdCastor_( pset.getParameter<double>("EnergyThresholdCastor") ),
  sumETotCastorMax_( pset.getParameter<double>("SumEMaxCastor") ) {}

CastorActivityFilter::~CastorActivityFilter() {}

void CastorActivityFilter::beginJob() {}

bool CastorActivityFilter::filter(edm::Event& event, edm::EventSetup const& setup){

  // Castor RecHit collection
  edm::Handle<CastorRecHitCollection> castorRecHitCollectionH;
  event.getByLabel(castorRecHitTag_,castorRecHitCollectionH);
  const CastorRecHitCollection& castorRecHitCollection = *castorRecHitCollectionH;

  bool isRealData = event.isRealData();
 
  double sumETotCastor = 0.,
         sumETotCastorNMod5 = 0.,
         sumETotCastorNMod4 = 0.,
         sumETotCastorNMod3 = 0.,
         sumETotCastorNMod2 = 0.;
  
  // Loop over rec hits
  CastorRecHitCollection::const_iterator castorRecHit = castorRecHitCollection.begin();
  CastorRecHitCollection::const_iterator castorRecHits_end = castorRecHitCollection.end();
  for(; castorRecHit != castorRecHits_end; ++castorRecHit) {
     const CastorRecHit& recHit = (*castorRecHit);

     int sectorId  = recHit.id().sector();
     int moduleId  = recHit.id().module();
     double energy = recHit.energy();
     double time   = recHit.time();

     if( !isRealData ) energy *= 62.5;

     if( moduleId > 5 ) continue;

     if( moduleId == 1 && sectorId == 5 ) continue;
     if( moduleId == 1 && sectorId == 6) continue;

     sumETotCastor += energy;

     if( moduleId <= 5 ) sumETotCastorNMod5 += energy;
     if( moduleId <= 4 ) sumETotCastorNMod4 += energy;
     if( moduleId <= 3 ) sumETotCastorNMod3 += energy;
     if( moduleId <= 2 ) sumETotCastorNMod2 += energy;
  }

  bool accept = true;
  if( (sumETotCastorMax_ >= 0. && sumETotCastor > sumETotCastorMax_) ) accept = false;

  return accept;
}
DEFINE_FWK_MODULE(CastorActivityFilter);
