#ifndef ForwardTTreeAnalysis_FWLiteTools_h
#define ForwardTTreeAnalysis_FWLiteTools_h

#if !defined(__CINT__) && !defined(__MAKECINT__)
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "TMath.h"
#include <vector>
#include <algorithm>
#endif


namespace exclusiveDijetsAnalysis {

enum calo_region_t {Barrel,Endcap,Transition,Forward};

/*
bool sortByEta( const math::XYZTLorentzVector& a, const math::XYZTLorentzVector& b){ 
   return a.eta() < b.eta();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void genRapidityGap(reco::GenParticleCollection const& genParticles, math::XYZTLorentzVector& genGapLow,
                                                                     math::XYZTLorentzVector& genGapHigh){
   // Copy and sort gen particles in eta
   std::vector<math::XYZTLorentzVector> genParticlesSort(0);
   double etaEdgeLow = -999.0;
   double etaEdgeHigh = 999.0;
   reco::GenParticleCollection::const_iterator genpart = genParticles.begin();
   reco::GenParticleCollection::const_iterator genpart_end = genParticles.end();  
   for(; genpart != genpart_end; ++genpart){
      if( genpart->status() != 1 ) continue;

      if((genpart->eta() >= etaEdgeLow) && (genpart->eta() <= etaEdgeHigh))
         genParticlesSort.push_back( genpart->p4() );
   }
   std::stable_sort(genParticlesSort.begin(), genParticlesSort.end(), sortByEta);

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
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////
void setGenInfo(reco::GenParticleCollection const& genParticles, double Ebeam,
                                                                 math::XYZTLorentzVector& genAllParticles,
                                                                 math::XYZTLorentzVector& genAllParticlesInRange,
                                                                 math::XYZTLorentzVector& genAllParticlesHEPlus,
                                                                 math::XYZTLorentzVector& genAllParticlesHEMinus,
                                                                 math::XYZTLorentzVector& genAllParticlesHFPlus,
                                                                 math::XYZTLorentzVector& genAllParticlesHFMinus,
                                                                 math::XYZTLorentzVector& genEtaMax,
                                                                 math::XYZTLorentzVector& genEtaMin,
                                                                 math::XYZTLorentzVector& genProtonPlus,
                                                                 math::XYZTLorentzVector& genProtonMinus){
   /*fwlite::Handle<std::vector<reco::GenParticle> > genParticlesCollection;
   genParticlesCollection.getByLabel(ev,"genParticles");
   const reco::GenParticleCollection& genParticles = *genParticlesCollection;*/

   math::XYZTLorentzVector allGenParticles(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesInRange(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesHEPlus(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesHEMinus(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesHFPlus(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesHFMinus(0.,0.,0.,0.);
   
   reco::GenParticleCollection::const_iterator proton1 = genParticles.end();
   reco::GenParticleCollection::const_iterator proton2 = genParticles.end();
   for(reco::GenParticleCollection::const_iterator genpart = genParticles.begin(); genpart != genParticles.end();
 ++genpart){
      if( genpart->status() != 1 ) continue;
      if( genpart->pdgId() != 2212 ) continue;
 
      if( ( genpart->pz() > 0.50*Ebeam ) && ( ( proton1 == genParticles.end() ) ||
                                              ( genpart->pz() > proton1->pz() ) ) ) proton1 = genpart;
      if( ( genpart->pz() < -0.50*Ebeam ) && ( ( proton2 == genParticles.end() ) ||
                                               ( genpart->pz() < proton2->pz() ) ) ) proton2 = genpart;
   }

   reco::GenParticleCollection::const_iterator etaMaxParticle = genParticles.end();
   reco::GenParticleCollection::const_iterator etaMinParticle = genParticles.end(); 
   for(reco::GenParticleCollection::const_iterator genpart = genParticles.begin(); genpart != genParticles.end(); ++genpart){
      if(genpart->status() != 1) continue;
      //histosTH1F["EnergyVsEta"]->Fill(genpart->eta(),genpart->energy());      

      /*double pz = genpart->pz();
      if( (proton1 == genParticles.end()) &&
          (genpart->pdgId() == 2212) && (pz > 0.50*Ebeam) ) proton1 = genpart;
      if( (proton2 == genParticles.end()) &&
          (genpart->pdgId() == 2212) && (pz < -0.50*Ebeam) ) proton2 = genpart;*/

      allGenParticles += genpart->p4();
      if(fabs(genpart->eta()) < 5.0) allGenParticlesInRange += genpart->p4();
      if( (genpart->eta() >= 1.3) && (genpart->eta() < 3.0) ) allGenParticlesHEPlus += genpart->p4();
      if( (genpart->eta() > -3.0) && (genpart->eta() <= -1.3) ) allGenParticlesHEMinus += genpart->p4();
      if( (genpart->eta() >= 3.0) && (genpart->eta() < 5.0) ) allGenParticlesHFPlus += genpart->p4();
      if( (genpart->eta() > -5.0) && (genpart->eta() <= -3.0) ) allGenParticlesHFMinus += genpart->p4(); 

      if( (genpart != proton1) && (genpart != proton2) ){
         if( ( etaMaxParticle == genParticles.end() ) ||
             ( genpart->eta() > etaMaxParticle->eta() ) ) etaMaxParticle = genpart;
         if( ( etaMinParticle == genParticles.end() ) ||
             ( genpart->eta() < etaMinParticle->eta() ) ) etaMinParticle = genpart;
      }
   }

   // Commit
   if( proton1 != genParticles.end() ) allGenParticles -= proton1->p4();
   if( proton2 != genParticles.end() ) allGenParticles -= proton2->p4();

   genAllParticles = allGenParticles;
   genAllParticlesInRange = allGenParticlesInRange;
   genAllParticlesHEPlus = allGenParticlesHEPlus;
   genAllParticlesHEMinus = allGenParticlesHEMinus;
   genAllParticlesHFPlus = allGenParticlesHFPlus;
   genAllParticlesHFMinus = allGenParticlesHFMinus;
 
   if( proton1 != genParticles.end() ) genProtonPlus = proton1->p4();
   if( proton2 != genParticles.end() ) genProtonMinus = proton2->p4();

   if( etaMaxParticle != genParticles.end() ) genEtaMax = etaMaxParticle->p4();
   if( etaMinParticle != genParticles.end() ) genEtaMin = etaMinParticle->p4();
}

int pflowId(std::string const& name){
   // FIXME: The labels definition could go somewhere else
   std::vector<std::string> labels_X, labels_h, labels_e, labels_mu, labels_gamma, labels_h0, labels_h_HF, labels_egamma_HF;
   labels_X.push_back("X");
   labels_X.push_back("undefined");
   labels_h.push_back("h");
   labels_h.push_back("chargedHadron");
   labels_h.push_back("hadronCharged");
   labels_e.push_back("e");
   labels_e.push_back("electron");
   labels_mu.push_back("mu");
   labels_mu.push_back("muon");
   labels_gamma.push_back("gamma");
   labels_gamma.push_back("photon");
   labels_h0.push_back("h0");
   labels_h0.push_back("neutralHadron");
   labels_h0.push_back("hadronNeutral");
   labels_h_HF.push_back("h_HF");
   labels_h_HF.push_back("hadronHF");
   labels_egamma_HF.push_back("egamma_HF");
   labels_egamma_HF.push_back("emHF");
   // Find corresponding particle type   
   if( std::find(labels_X.begin(), labels_X.end(), name) != labels_X.end() )
      return reco::PFCandidate::X;
   else if( std::find(labels_h.begin(), labels_h.end(), name) != labels_h.end() )
      return reco::PFCandidate::h;
   else if( std::find(labels_e.begin(), labels_e.end(), name) != labels_e.end() )
      return reco::PFCandidate::e;
   else if( std::find(labels_mu.begin(), labels_mu.end(), name) != labels_mu.end() )
      return reco::PFCandidate::mu;
   else if( std::find(labels_gamma.begin(), labels_gamma.end(), name) != labels_gamma.end() ) 
      return reco::PFCandidate::gamma;
   else if( std::find(labels_h0.begin(), labels_h0.end(), name) != labels_h0.end() ) 
      return reco::PFCandidate::h0;
   else if( std::find(labels_h_HF.begin(), labels_h_HF.end(), name) != labels_h_HF.end() ) 
      return reco::PFCandidate::h_HF;
   else if( std::find(labels_egamma_HF.begin(), labels_egamma_HF.end(), name) != labels_egamma_HF.end() ) 
      return reco::PFCandidate::egamma_HF;
   else return -1;
}

bool pflowThreshold(reco::PFCandidate const& part, std::map<int, std::map<int,std::pair<double,double> > > const& thresholdMap){

   bool accept = true;

   double eta = part.eta();
   int region = -1;
   if( (fabs(eta) >= 0.) && (fabs(eta) < 1.4) ) region = Barrel;
   else if( (fabs(eta) >= 1.4) && (fabs(eta) < 2.6) ) region = Endcap;
   else if( (fabs(eta) >= 2.6) && (fabs(eta) < 3.2) ) region = Transition;
   else if( (fabs(eta) >= 3.2) ) region = Forward;
   std::map<int,std::pair<double,double> > const& thresholds = thresholdMap.find(region)->second;
   
   double ptThreshold = -1.0;
   double eThreshold = -1.0;
   int partType = part.particleId();
   std::map<int,std::pair<double,double> >::const_iterator it_threshold = thresholds.find(partType);
   if(it_threshold != thresholds.end()) {
      ptThreshold = it_threshold->second.first;
      eThreshold = it_threshold->second.second;
   }

   if(part.pt() < ptThreshold) accept = false;
   if(part.energy() < eThreshold) accept = false;

   return accept;
}
/////////////////////////////////////////////////////////////////////////////////////////////////
//added by eliza
double MassDissGen(reco::GenParticleCollection const& genParticles, double rangeEtaMin = -999.,
                                                                    double rangeEtaMax = 999.){
                                                                    
   math::XYZTLorentzVector allGenParticles(0.,0.,0.,0.);
   reco::GenParticleCollection::const_iterator genpart = genParticles.begin();
   reco::GenParticleCollection::const_iterator genpart_end = genParticles.end();
   for(; genpart != genpart_end; ++genpart){
      if( genpart->status() != 1 ) continue;

      if( ( genpart->eta() >= (rangeEtaMin - 0.0001) ) && 
          ( genpart->eta() <= (rangeEtaMax + 0.0001) ) ) allGenParticles += genpart->p4();
   }
   return allGenParticles.M();
}


////////////////////////////////////////////////////////////////////////////
/*
//New functions of FWLITE
template <class VertexColl>
bool goodVertexFilter(const VertexColl& vertexCollection, unsigned int minNumTracks = 2, double maxAbsZ = -1., double maxd0 = -1.){
   bool accept = false; 
   for(typename VertexColl::const_iterator it = vertexCollection.begin();
                                           it != vertexCollection.end(); ++it){
      if(it->tracksSize() > minNumTracks && 
        ( (maxAbsZ <= 0. ) || fabs(it->z()) <= maxAbsZ ) &&
        ( (maxd0 <= 0. ) || fabs(it->position().rho()) <= maxd0 ) ) {accept = true;break;}
   }

   return accept;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////
template <class TrackColl>
bool highPurityTracksFilter(const TrackColl& trackCollection, double thresh, unsigned int numtrack){

   int numhighpurity = 0;
   float fraction = 0.;

   reco::TrackBase::TrackQuality trkQuality = reco::TrackBase::qualityByName("highPurity");

   bool accept = false;
   if(trackCollection.size() > numtrack){ 
      typename TrackColl::const_iterator itk = trackCollection.begin();
      typename TrackColl::const_iterator itk_end = trackCollection.end();
      for(; itk != itk_end; ++itk){
         // std::cout << "HighPurity?  " << itk->quality(_trackQuality) << std::endl;
         if(itk->quality(trkQuality)) ++numhighpurity;
      }
      fraction = (float)numhighpurity/(float)trackCollection.size();
      if(fraction > thresh) accept = true;
  } else{
    //if less than 10 Tracks accept the event anyway    
    accept = true;
  }
  
  return accept;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////
/*
template <class PartColl>
double MassColl(PartColl& partCollection, double ptThreshold = -1.,
                double energyHBThreshold = -1., double energyHEThreshold = -1.,
                double energyHFThreshold = -1., double energyScale = -1.){
   math::XYZTLorentzVector allCands(0.,0.,0.,0.);
   for(typename PartColl::const_iterator part = partCollection.begin();
                                         part != partCollection.end(); ++part){
      double part_pt = part->pt();
      double part_energy = part->energy();
      if(energyScale > 0.){
         part_pt *= energyScale;
         part_energy *= energyScale;
      }

      // HF eta rings 29, 40, 41
      if( ( (fabs(part->eta()) >= 2.866) && (fabs(part->eta()) < 2.976) ) || 
          (fabs(part->eta()) >= 4.730) ) continue;
 
      if(part_pt < ptThreshold) continue;
      if((fabs(part->eta()) < 1.3) && (part_energy < energyHBThreshold)) continue;
      if(((fabs(part->eta()) >= 1.3) && (fabs(part->eta()) < 3.0)) && (part_energy < energyHEThreshold)) continue;
      if((fabs(part->eta()) >= 3.0) && ((fabs(part->eta()) <= 5.0)) && (part_energy < energyHFThreshold)) continue;
      if(energyScale > 0.) allCands += energyScale*part->p4();
      else allCands += part->p4();
   }

   return allCands.M();
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// FIXME: Generalize for any collection with changeable threshold scheme
double MassColl(reco::PFCandidateCollection const& pflowCollection, std::map<int, std::map<int,std::pair<double,double> > > const& thresholdMap){
   math::XYZTLorentzVector allCands(0.,0.,0.,0.);
   reco::PFCandidateCollection::const_iterator part = pflowCollection.begin();
   reco::PFCandidateCollection::const_iterator pfCands_end = pflowCollection.end();
   for(; part != pfCands_end; ++part){
      if(pflowThreshold(*part,thresholdMap)) allCands += part->p4();
   }

   return allCands.M();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class JetColl,class PartColl>
double Rjj(JetColl& jetCollection,PartColl& partCollection){
   math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
   dijetSystem += (jetCollection[0]).p4();
   dijetSystem += (jetCollection[1]).p4(); 

   double Mx = MassColl(partCollection);

   return (dijetSystem.M()/Mx);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int nHFSlice(const std::map<unsigned int, std::vector<unsigned int> >& mapThreshToiEta, unsigned int thresholdHF, unsigned int ieta){
   const std::vector<unsigned int>& vec_iEta = mapThreshToiEta.find(thresholdHF)->second;

   // Count number of ieta entries in vector 
   int count_ieta = (int)std::count(vec_iEta.begin(),vec_iEta.end(),ieta);

   return count_ieta;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int nHCALiEta(const std::map<unsigned int, std::vector<unsigned int> >& iEtaMultiplicity, unsigned int threshold, unsigned int ieta){

   //if(iEtaMultiplicity.find(ieta) != iEtaMultiplicity.end()) count_ieta = iEtaMultiplicity[ieta];
   std::map<unsigned int, std::vector<unsigned int> >::const_iterator it_ieta = iEtaMultiplicity.find(ieta);
   unsigned int count_ieta = 0; 
   if( it_ieta != iEtaMultiplicity.end() ) count_ieta = (it_ieta->second)[threshold];

   return count_ieta;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
double sumEHCALiEta(const std::map<unsigned int, std::vector<double> >& iEtaSumE, unsigned int threshold, unsigned int ieta){

   std::map<unsigned int, std::vector<double> >::const_iterator it_ieta = iEtaSumE.find(ieta);
   double sumE_ieta = 0;
   if( it_ieta != iEtaSumE.end() ) sumE_ieta = (it_ieta->second)[threshold];

   return sumE_ieta;
}
////////////////////////////////////////////////////////////////////////////
std::pair<double,double> xi(reco::PFCandidateCollection const& pflowCollection, double Ebeam, std::map<int,std::map<int,std::pair<double,double> > > const& thresholdMap){

   double xi_towers_plus = 0.;
   double xi_towers_minus = 0.;
   reco::PFCandidateCollection::const_iterator part = pflowCollection.begin();
   reco::PFCandidateCollection::const_iterator pfCands_end = pflowCollection.end();
   for(; part != pfCands_end; ++part){
     if(!pflowThreshold(*part,thresholdMap)) continue;

     xi_towers_plus += part->et()*TMath::Exp(part->eta());
     xi_towers_minus += part->et()*TMath::Exp(-part->eta());
   }

   xi_towers_plus /= 2*Ebeam;
   xi_towers_minus /= 2*Ebeam;
   
   return std::make_pair(xi_towers_plus,xi_towers_minus);
}
///////////////////////////////////////////////////////////////////////////////
std::pair<double,double> EPlusPz(reco::PFCandidateCollection const& pflowCollection, std::map<int, std::map<int,std::pair<double,double> > > const& thresholdMap){
   double e_plus_pz = 0.;
   double e_minus_pz = 0.;
   reco::PFCandidateCollection::const_iterator part = pflowCollection.begin();
   reco::PFCandidateCollection::const_iterator pfCands_end = pflowCollection.end();
   for(; part != pfCands_end; ++part){
      if(!pflowThreshold(*part,thresholdMap)) continue;

      e_plus_pz += part->energy() + part->pz(); 
      e_minus_pz += part->energy() - part->pz();
   }

   return std::make_pair(e_plus_pz,e_minus_pz);
}
//////////////////////////////////////////////////////////////////////////////////
std::pair<double,double> etaMax(reco::PFCandidateCollection const& pflowCollection, std::map<int, std::map<int,std::pair<double,double> > > const& thresholdMap){
   std::vector<double> etaCands;
   reco::PFCandidateCollection::const_iterator part = pflowCollection.begin();
   reco::PFCandidateCollection::const_iterator pfCands_end = pflowCollection.end();
   for(; part != pfCands_end; ++part){                           
      if(!pflowThreshold(*part,thresholdMap)) continue;            
      etaCands.push_back( part->eta() );
   }                                                             
   double eta_max = etaCands.size() ? *( std::max_element(etaCands.begin(), etaCands.end()) ) : -999.;
   double eta_min = etaCands.size() ? *( std::min_element(etaCands.begin(), etaCands.end()) ) : -999.;

   return std::make_pair(eta_max,eta_min);
}

///////////////////////////////////////////////////////////////////////////////////// 
/*
double castorEnergy(CastorRecHitCollection const& castorRecHitCollection, bool isRealData = true){
 
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

   return sumETotCastor;
}
*/
 
} // namespace

#endif

