
#include "ForwardAnalysis/Utilities/interface/CastorEnergy.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

using forwardAnalysis::CastorEnergy;

double CastorEnergy::operator()(CastorRecHitCollection const& castorRecHitCollection, bool isRealData){
 
  double sumETotCastor = 0.;
  
  // Loop over rec hits
  CastorRecHitCollection::const_iterator castorRecHit = castorRecHitCollection.begin();
  CastorRecHitCollection::const_iterator castorRecHits_end = castorRecHitCollection.end();
  for(; castorRecHit != castorRecHits_end; ++castorRecHit) {
     const CastorRecHit& recHit = (*castorRecHit);

     int sectorId  = recHit.id().sector();
     int moduleId  = recHit.id().module();
     double energy = recHit.energy();
     //double time   = recHit.time();

     if( !isRealData ) energy *= 62.5;

     // Apply thresholds on rec. hits, etc.
     // ...

     // Use only selected subset of modules
     //if( moduleId > 5 ) continue;
     if( std::find(modules_.begin(),modules_.end(),moduleId) == modules_.end() ) continue;

     // Reject known bad detector areas
     if( moduleId == 1 && sectorId == 5 )  continue;
     if( moduleId == 1 && sectorId == 6 )  continue;

     sumETotCastor += energy;
  }

  return sumETotCastor;
}
