#ifndef DiffractiveZEvent_h
#define DiffractiveZEvent_h

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Math/interface/LorentzVector.h"

namespace diffractiveZAnalysis {
   class DiffractiveZAnalysis;
}

class DiffractiveZEvent {
   public:
      typedef diffractiveZAnalysis::DiffractiveZAnalysis analysis_type;
      static const char* name;

      typedef reco::Particle::LorentzVector LorentzVector;

      DiffractiveZEvent();
      ~DiffractiveZEvent();

      void SetHLTPath(int idx, int fHLTBit)         { hltTrigResults_[idx] = fHLTBit;}
      void SetDiElectronMass(double fDiElectronMass) { DiElectronMass_ = fDiElectronMass;}
      void SetDiMuonMass(double fDiMuonMass) { DiMuonMass_ = fDiMuonMass;}

      void SetLeadingElectronPt(double fLeadingElectronPt)    { LeadingElectronPt_     = fLeadingElectronPt;}
      void SetLeadingElectronEta(double fLeadingElectronEta)  { LeadingElectronEta_     = fLeadingElectronEta;}
      void SetLeadingElectronPhi(double fLeadingElectronPhi)  { LeadingElectronPhi_    = fLeadingElectronPhi;}
      void SetLeadingElectronP4(LorentzVector fLeadingElectronP4)    { LeadingElectronP4_     = fLeadingElectronP4;}
      void SetLeadingElectronCharge(int fLeadingElectronCharge)  { LeadingElectronCharge_     = fLeadingElectronCharge;}
      void SetSecondElectronPt(double fSecondElectronPt)    { SecondElectronPt_     = fSecondElectronPt;}
      void SetSecondElectronEta(double fSecondElectronEta)  { SecondElectronEta_     = fSecondElectronEta;}
      void SetSecondElectronPhi(double fSecondElectronPhi)  { SecondElectronPhi_    = fSecondElectronPhi;}
      void SetSecondElectronP4(LorentzVector fSecondElectronP4)    { SecondElectronP4_     = fSecondElectronP4;}
      void SetSecondElectronCharge(int fSecondElectronCharge)  { SecondElectronCharge_     = fSecondElectronCharge;}
      void SetElectronsN(int fElectronsN)  { ElectronsN_    = fElectronsN;}

      void SetLeadingMuonPt(double fLeadingMuonPt)    { LeadingMuonPt_     = fLeadingMuonPt;}
      void SetLeadingMuonEta(double fLeadingMuonEta)  { LeadingMuonEta_     = fLeadingMuonEta;}
      void SetLeadingMuonPhi(double fLeadingMuonPhi)  { LeadingMuonPhi_    = fLeadingMuonPhi;}
      void SetLeadingMuonP4(LorentzVector fLeadingMuonP4)    { LeadingMuonP4_     = fLeadingMuonP4;}
      void SetLeadingMuonCharge(int fLeadingMuonCharge)  { LeadingMuonCharge_     = fLeadingMuonCharge;}
      void SetSecondMuonPt(double fSecondMuonPt)    { SecondMuonPt_     = fSecondMuonPt;}
      void SetSecondMuonEta(double fSecondMuonEta)  { SecondMuonEta_     = fSecondMuonEta;}
      void SetSecondMuonPhi(double fSecondMuonPhi)  { SecondMuonPhi_    = fSecondMuonPhi;}
      void SetSecondMuonP4(LorentzVector fSecondMuonP4)    { SecondMuonP4_     = fSecondMuonP4;}
      void SetSecondMuonCharge(int fSecondMuonCharge)  { SecondMuonCharge_     = fSecondMuonCharge;}
      void SetMuonsN(int fMuonsN)  { MuonsN_    = fMuonsN;}


      int GetHLTPath(int idx)                    const { return hltTrigResults_[idx]; }
      double GetDiElectronMass() const {return DiElectronMass_;}
      double GetDiMuonMass() const {return DiMuonMass_;}

      double GetLeadingElectronPt() const {return LeadingElectronPt_;}
      double GetLeadingElectronEta() const {return LeadingElectronEta_;}
      double GetLeadingElectronPhi() const {return LeadingElectronPhi_;}
      const LorentzVector& GetLeadingElectronP4() const {return LeadingElectronP4_;}
      int GetLeadingElectronCharge() const {return LeadingElectronCharge_;}
      double GetSecondElectronPt() const {return SecondElectronPt_;}
      double GetSecondElectronEta() const {return SecondElectronEta_;}
      double GetSecondElectronPhi() const {return SecondElectronPhi_;}
      const LorentzVector& GetSecondElectronP4() const {return SecondElectronP4_;}
      int GetSecondElectronCharge() const {return SecondElectronCharge_;}
      int GetElectronsN() const {return ElectronsN_;}
  
      double GetLeadingMuonPt() const {return LeadingMuonPt_;}
      double GetLeadingMuonEta() const {return LeadingMuonEta_;}
      double GetLeadingMuonPhi() const {return LeadingMuonPhi_;}
      const LorentzVector& GetLeadingMuonP4() const {return LeadingMuonP4_;}
      int GetLeadingMuonCharge() const {return LeadingMuonCharge_;}
      double GetSecondMuonPt() const {return SecondMuonPt_;}
      double GetSecondMuonEta() const {return SecondMuonEta_;}
      double GetSecondMuonPhi() const {return SecondMuonPhi_;}
      const LorentzVector& GetSecondMuonP4() const {return SecondMuonP4_;}
      int GetSecondMuonCharge() const {return SecondMuonCharge_;}
      int GetMuonsN() const {return MuonsN_;}


   private:
      friend class diffractiveZAnalysis::DiffractiveZAnalysis;
      
      void reset();

      int hltTrigResults_[20];
      double DiElectronMass_;
      double DiMuonMass_;

      double LeadingElectronPt_;
      double LeadingElectronEta_;
      double LeadingElectronPhi_;
      LorentzVector LeadingElectronP4_;
      int LeadingElectronCharge_;
      double SecondElectronPt_;
      double SecondElectronEta_;
      double SecondElectronPhi_;
      LorentzVector SecondElectronP4_;
      int SecondElectronCharge_;
      int ElectronsN_;

      double LeadingMuonPt_;
      double LeadingMuonEta_;
      double LeadingMuonPhi_;
      LorentzVector LeadingMuonP4_;
      int LeadingMuonCharge_;
      double SecondMuonPt_;
      double SecondMuonEta_;
      double SecondMuonPhi_;
      LorentzVector SecondMuonP4_;
      int SecondMuonCharge_;
      int MuonsN_;

};

#endif
