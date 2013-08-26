#ifndef PFCandInfoEvent_h
#define PFCandInfoEvent_h
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PFCand.h"
#include "DataFormats/Candidate/interface/Particle.h"
//#include "TLorentzVector.h"
#include <string>
#include <vector>
#include <TLorentzVector.h>
//#include "DataFormats/Math/interface/LorentzVector.h"
namespace pfCandInfo{
  class PFCandInfo;
}

class PFCandInfoEvent  {
   public:
       typedef pfCandInfo::PFCandInfo analysis_type;
       static const char* name;
       
       //typedef reco::Particle::LorentzVector LorentzVector;

       PFCandInfoEvent();
       ~PFCandInfoEvent();

       void SetPFCand(const PFCand& pfCand) { PFCandList.push_back(pfCand); }
       const PFCand& GetPFCand(int ipfcand) const { return PFCandList[ipfcand]; }
      // std::vector<PFCand> PFCandList;      
   private:
      friend class pfCandInfo::PFCandInfo;
      void reset();
      std::vector<PFCand> PFCandList;
      
};////end code

#endif


