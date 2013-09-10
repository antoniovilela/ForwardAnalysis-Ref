#ifndef PFCand_h
#define PFCand_h
//#include "TLorentzVector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Particle.h"

#include <vector>

namespace pfCandInfo {
  class PFCandInfo;
}

//class PFCand : public TLorentzVector{
class PFCand {
  public:
     typedef reco::Particle::LorentzVector LorentzVector;

     PFCand();
     ~PFCand();
  
     enum ParticleType {
      X=0,         // undefined
      h,           // charged hadron
      e,           // electron 
      mu,          // muon 
      gamma,       // photon
      h0,          // neutral hadron
      h_HF,        // HF tower identified as a hadron
      egamma_HF    // HF tower identified as an EM PFCandicle
     };

     //------------ Set FWD methods ------------------------------
     void SetPFCandP4(PFCand::LorentzVector& fPFCandP4)   { pfCandP4 = fPFCandP4; }
     void SetCharge(double fCharge)                         { Charge = fCharge; }
     void SetParticleId(PFCand::ParticleType fParticleId)             { ParticleId = fParticleId; }
     //------------ Get FWD methods ------------------------------
     const LorentzVector& GetPFCandP4()          const { return pfCandP4; }
     double  GetCharge()                         const { return Charge; }
     PFCand::ParticleType GetParticleId()        const { return ParticleId; }

     //ParticleType  ParticleId;
     // TLorentzVector PFCandVectorP4;  
     //Double_t Charge;

  private:
     //friend class pfCandInfo::PFCandInfo;
     //TLorentzVector PFCandVectorP4;
     LorentzVector pfCandP4;
     Double_t Charge;
     ParticleType  ParticleId;
          
//////////////////////////////////
};//end code

#endif
