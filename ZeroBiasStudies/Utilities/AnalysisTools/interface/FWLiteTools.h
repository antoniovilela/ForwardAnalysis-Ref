#ifndef UtilitiesAnalysisTools_FWLiteTools_h
#define UtilitiesAnalysisTools_FWLiteTools_h

#if !defined(__CINT__) && !defined(__MAKECINT__)
//Headers for the data items
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/Track.h"
#endif

#include "TMath.h"

#include <map>
#include <algorithm>

namespace analysisTools {

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

template <class PartColl>
double MassColl(PartColl& partCollection){
   math::XYZTLorentzVector allCands(0.,0.,0.,0.);
   for(typename PartColl::const_iterator part = partCollection.begin();
                                         part != partCollection.end(); ++part) allCands += part->p4();

   return allCands.M();
}

template <class Coll>
std::pair<double,double> xi(Coll& partCollection, double Ebeam, double ptThreshold = -1.,
                            double energyHBThreshold = -1., double energyHEThreshold = -1.,
                            double energyHFThreshold = -1., double energyScale = -1.){

   double xi_towers_plus = 0.;
   double xi_towers_minus = 0.;
   for(typename Coll::const_iterator part = partCollection.begin(); part != partCollection.end(); ++part){

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

      //double correction = (jetCorrector)?(jetCorrector->getCorrection(part->pt(),part->eta())):1.;
      double part_et = part->et();
      double part_eta = part->eta();
      if(energyScale > 0.) part_et *= energyScale;

      xi_towers_plus += part_et*TMath::Exp(part_eta);
      xi_towers_minus += part_et*TMath::Exp(-part_eta);
   }

   xi_towers_plus /= 2*Ebeam;
   xi_towers_minus /= 2*Ebeam;
   
   return std::make_pair(xi_towers_plus,xi_towers_minus);
}

template <class Coll>
std::pair<double,double> xi(Coll& partCollection, double Ebeam){

   double xi_towers_plus = 0.;
   double xi_towers_minus = 0.;
   for(typename Coll::const_iterator part = partCollection.begin(); part != partCollection.end(); ++part){
      double part_et = part->et();
      double part_eta = part->eta();

      xi_towers_plus += part_et*TMath::Exp(part_eta);
      xi_towers_minus += part_et*TMath::Exp(-part_eta);
   }
   xi_towers_plus /= 2*Ebeam;
   xi_towers_minus /= 2*Ebeam;

   return std::make_pair(xi_towers_plus,xi_towers_minus);
}

template <class Coll>
std::pair<double,double> EPlusPz(Coll& partCollection, double ptThreshold = -1.,
                                 double energyHBThreshold = -1., double energyHEThreshold = -1.,
                                 double energyHFThreshold = -1., double energyScale = -1.){
   double e_plus_pz = 0.;
   double e_minus_pz = 0.;
   typename Coll::const_iterator part = partCollection.begin();
   typename Coll::const_iterator part_end = partCollection.end();
   for(; part != part_end; ++part){
      double part_pt = part->pt();
      double part_energy = part->energy();
      double part_pz = part->pz();
      if(energyScale > 0.){
         part_pt *= energyScale;
         part_energy *= energyScale;
         part_pz *= energyScale;
      }

      // HF eta rings 29, 40, 41
      if( ( (fabs(part->eta()) >= 2.866) && (fabs(part->eta()) < 2.976) ) || 
          (fabs(part->eta()) >= 4.730) ) continue;

      if(part_pt < ptThreshold) continue;
      if((fabs(part->eta()) < 1.3) && (part_energy < energyHBThreshold)) continue;
      if(((fabs(part->eta()) >= 1.3) && (fabs(part->eta()) < 3.0)) && (part_energy < energyHEThreshold)) continue; 
      if((fabs(part->eta()) >= 3.0) && ((fabs(part->eta()) <= 5.0)) && (part_energy < energyHFThreshold)) continue;

      e_plus_pz += part_energy + part_pz; 
      e_minus_pz += part_energy - part_pz;
   }

   return std::make_pair(e_plus_pz,e_minus_pz);
}

template <class Coll>
std::pair<double,double> EPlusPz(Coll& partCollection){
   double e_plus_pz = 0.;
   double e_minus_pz = 0.;
   typename Coll::const_iterator part = partCollection.begin();
   typename Coll::const_iterator part_end = partCollection.end();
   for(; part != part_end; ++part){
      double part_energy = part->energy();
      double part_pz = part->pz();

      e_plus_pz += part_energy + part_pz;
      e_minus_pz += part_energy - part_pz;
   }

   return std::make_pair(e_plus_pz,e_minus_pz);
}

template <class JetColl,class PartColl>
double Rjj(JetColl& jetCollection,PartColl& partCollection){
   math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
   dijetSystem += (jetCollection[0]).p4();
   dijetSystem += (jetCollection[1]).p4(); 

   double Mx = MassColl(partCollection);

   return (dijetSystem.M()/Mx);
}

unsigned int nHFSlice(const std::map<unsigned int, std::vector<unsigned int> >& mapThreshToiEta, unsigned int thresholdHF, unsigned int ieta){
   const std::vector<unsigned int>& vec_iEta = mapThreshToiEta.find(thresholdHF)->second;

   // Count number of ieta entries in vector 
   int count_ieta = (int)std::count(vec_iEta.begin(),vec_iEta.end(),ieta);

   return count_ieta;
}

unsigned int nHCALiEta(const std::map<unsigned int, std::vector<unsigned int> >& iEtaMultiplicity, unsigned int threshold, unsigned int ieta){

   //if(iEtaMultiplicity.find(ieta) != iEtaMultiplicity.end()) count_ieta = iEtaMultiplicity[ieta];
   std::map<unsigned int, std::vector<unsigned int> >::const_iterator it_ieta = iEtaMultiplicity.find(ieta);
   unsigned int count_ieta = 0; 
   if( it_ieta != iEtaMultiplicity.end() ) count_ieta = (it_ieta->second)[threshold];

   return count_ieta;
}

double sumEHCALiEta(const std::map<unsigned int, std::vector<double> >& iEtaSumE, unsigned int threshold, unsigned int ieta){

   std::map<unsigned int, std::vector<double> >::const_iterator it_ieta = iEtaSumE.find(ieta);
   double sumE_ieta = 0;
   if( it_ieta != iEtaSumE.end() ) sumE_ieta = (it_ieta->second)[threshold];

   return sumE_ieta;
}

} // namespace
#endif
