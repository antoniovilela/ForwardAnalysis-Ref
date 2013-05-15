#ifndef DiffractiveZEvent_h
#define DiffractiveZEvent_h

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Common/interface/DetSetVector.h"

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

    void SetVertexMultiplicity(const std::vector<double>& fVertexMultiplicity) { VertexMultiplicity_ = fVertexMultiplicity; }
    void SetVertexChiNorm(const std::vector<double>& fVertexChiNorm) { VertexChiNorm_ = fVertexChiNorm; }
    void SetVertexNDOF(const std::vector<double>& fVertexNDOF) { VertexNDOF_ = fVertexNDOF; }
    void SetVz(const std::vector<double>& fVz) { Vz_ = fVz; }
    void SetVx(const std::vector<double>& fVx) { Vx_ = fVx; }
    void SetVy(const std::vector<double>& fVy) { Vy_ = fVy; }
    void SetTracksPt(const std::vector<std::vector<double> >& fTracksPt) { TracksPt_ = fTracksPt; }

    void SetPrimaryGapMaxGen(double fPrimaryGapMaxGen)    { PrimaryGapMaxGen_     = fPrimaryGapMaxGen;}
    void SetSecondGapMaxGen(double fSecondGapMaxGen)    { SecondGapMaxGen_    = fSecondGapMaxGen;}
    void SetTracksPtGen(const std::vector<double>& fTracksPtGen)    { TracksPtGen_     = fTracksPtGen;}
    void SetEtaOfTracksPtGen(const std::vector<double>& fEtaOfTracksPtGen)    { EtaOfTracksPtGen_     = fEtaOfTracksPtGen;}
    void SetNTracksGen(int fNTracksGen)    { NTracksGen_     = fNTracksGen;}
    void SetMx2PlusGen(double fMx2PlusGen)    { Mx2PlusGen_     = fMx2PlusGen;}
    void SetMx2MinusGen(double fMx2MinusGen)    { Mx2MinusGen_     = fMx2MinusGen;}
    void SetMx2Gen(double fMx2Gen)    { Mx2Gen_     = fMx2Gen;}
    void SetMx2ZGen(double fMx2ZGen)    { Mx2ZGen_     = fMx2ZGen;}
    void SetNMx2PlusGen(int fNMx2PlusGen)    { NMx2PlusGen_     = fNMx2PlusGen;}
    void SetNMx2MinusGen(int fNMx2MinusGen)    { NMx2MinusGen_     = fNMx2MinusGen;}
    void SetEtaGaplimPlusGen(double fEtaGaplimPlusGen)    { EtaGaplimPlusGen_     = fEtaGaplimPlusGen;}
    void SetEtaGaplimMinusGen(double fEtaGaplimMinusGen)    { EtaGaplimMinusGen_     = fEtaGaplimMinusGen;}
    void SetNParticlesGen(int fNParticlesGen)    { NParticlesGen_     = fNParticlesGen;}
    void SetsumECastorMinusGen(double fsumECastorMinusGen)    { sumECastorMinusGen_     = fsumECastorMinusGen;}
    void SetsumECastorPlusGen(double fsumECastorPlusGen)    { sumECastorPlusGen_     = fsumECastorPlusGen;}
    void SetsumEZDCMinusGen(double fsumEZDCMinusGen)    { sumEZDCMinusGen_     = fsumEZDCMinusGen;}
    void SetsumEZDCPlusGen(double fsumEZDCPlusGen)    { sumEZDCPlusGen_     = fsumEZDCPlusGen;}
    void SetEtaOutcomingProtonGen(double fEtaOutcomingProtonGen)    { EtaOutcomingProtonGen_     = fEtaOutcomingProtonGen;}
    void SetxLGen(double fxLGen)   { xLGen_     = fxLGen;}
    void SetxLMostEnergeticGen(double fxLMostEnergeticGen)    { xLMostEnergeticGen_     = fxLMostEnergeticGen;}
    void SetxiZMinusGen(double fxiZMinusGen)    { xiZMinusGen_     = fxiZMinusGen;}
    void SetxiZPlusGen(double fxiZPlusGen)    { xiZPlusGen_     = fxiZPlusGen;}
    void SetEtaZGen(double fEtaZGen)    { EtaZGen_     = fEtaZGen;}
    void SetEnergyZGen(double fEnergyZGen)    { EnergyZGen_     = fEnergyZGen;}
    void SetpDissMassGen(double fpDissMassGen)    { pDissMassGen_     = fpDissMassGen;}
    void SetxLpDissMass(double fxLpDissMass)    { xLpDissMass_     = fxLpDissMass;}

    void SetDiElectronMassPF(double fDiElectronMassPF) { DiElectronMassPF_ = fDiElectronMassPF;}
    void SetDiMuonMassPF(double fDiMuonMassPF) { DiMuonMassPF_ = fDiMuonMassPF;}

    void SetSumEHFPlus(double fSumEHFPlus)    { SumEHFPlus_ = fSumEHFPlus;}
    void SetSumEHF_SPlus(double fSumEHF_SPlus)    { SumEHF_SPlus_ = fSumEHF_SPlus;}
    void SetSumEHF_LPlus(double fSumEHF_LPlus)    { SumEHF_LPlus_ = fSumEHF_LPlus;}
    void SetSumEtHFPlus(double fSumEtHFPlus)    { SumEtHFPlus_ = fSumEtHFPlus;}

    void SetSumEHFMinus(double fSumEHFMinus)    { SumEHFMinus_ = fSumEHFMinus;}
    void SetSumEHF_SMinus(double fSumEHF_SMinus)    { SumEHF_SMinus_ = fSumEHF_SMinus;}
    void SetSumEHF_LMinus(double fSumEHF_LMinus)    { SumEHF_LMinus_ = fSumEHF_LMinus;}
    void SetSumEtHFMinus(double fSumEtHFMinus)    { SumEtHFMinus_ = fSumEtHFMinus;}

    void SetSumEHEPlus(double fSumEHEPlus)    { SumEHEPlus_ = fSumEHEPlus;}
    void SetSumEtHEPlus(double fSumEtHEPlus)    { SumEtHEPlus_ = fSumEtHEPlus;}
    void SetSumEHEMinus(double fSumEHEMinus)    { SumEHEMinus_ = fSumEHEMinus;}
    void SetSumEtHEMinus(double fSumEtHEMinus)    { SumEtHEMinus_ = fSumEtHEMinus;}

    void SetSumEHBPlus(double fSumEHBPlus)    { SumEHBPlus_ = fSumEHBPlus;}
    void SetSumEtHBPlus(double fSumEtHBPlus)    { SumEtHBPlus_ = fSumEtHBPlus;}
    void SetSumEHBMinus(double fSumEHBMinus)    { SumEHBMinus_ = fSumEHBMinus;}
    void SetSumEtHBMinus(double fSumEtHBMinus)    { SumEtHBMinus_ = fSumEtHBMinus;}

    void SetSumEEEPlus(double fSumEEEPlus)    { SumEEEPlus_ = fSumEEEPlus;}
    void SetSumEtEEPlus(double fSumEtEEPlus)    { SumEtEEPlus_ = fSumEtEEPlus;}
    void SetSumEEEMinus(double fSumEEEMinus)    { SumEEEMinus_ = fSumEEEMinus;}
    void SetSumEtEEMinus(double fSumEtEEMinus)    { SumEtEEMinus_ = fSumEtEEMinus;}

    void SetSumEEBPlus(double fSumEEBPlus)    { SumEEBPlus_ = fSumEEBPlus;}
    void SetSumEtEBPlus(double fSumEtEBPlus)    { SumEtEBPlus_ = fSumEtEBPlus;}
    void SetSumEEBMinus(double fSumEEBMinus)    { SumEEBMinus_ = fSumEEBMinus;}
    void SetSumEtEBMinus(double fSumEtEBMinus)    { SumEtEBMinus_ = fSumEtEBMinus;}

    void SetEPZCaloPlus(double fEPZCaloPlus)    { EPZCaloPlus_ = fEPZCaloPlus;}
    void SetEPZCaloMinus(double fEPZCaloMinus)    { EPZCaloMinus_ = fEPZCaloMinus;}
    void SetXiCaloPlus(double fXiCaloPlus)    { XiCaloPlus_ = fXiCaloPlus;}
    void SetXiCaloMinus(double fXiCaloMinus)    { XiCaloMinus_ = fXiCaloMinus;}

    void SetEtaCaloMax(double fEtaCaloMax)    { EtaCaloMax_ = fEtaCaloMax;}
    void SetEtaCaloMin(double fEtaCaloMin)    { EtaCaloMin_ = fEtaCaloMin;}

    void SetVertex(int fVertex)    { Vertex_ = fVertex;}
    void SetXi_PF_minus(double fXi_PF_minus)    { Xi_PF_minus_ = fXi_PF_minus;}
    void SetXi_PF_plus(double fXi_PF_plus)    { Xi_PF_plus_ = fXi_PF_plus;}
    void SetEpz_PF_minus(double fEpz_PF_minus)    { Epz_PF_minus_= fEpz_PF_minus;}
    void SetEpz_PF_plus(double fEpz_PF_plus)    { Epz_PF_plus_ = fEpz_PF_plus;}
    void SetMultiplicityPF(int fMultiplicityPF)    { MultiplicityPF_ = fMultiplicityPF;}
    void SetSumEtaTimesEnergyPF(double fSumEtaTimesEnergyPF)    { SumEtaTimesEnergyPF_ = fSumEtaTimesEnergyPF;}
    void SetSumpxModulePF(double fSumpxModulePF)    { SumpxModulePF_ = fSumpxModulePF;}
    void SetSumpyModulePF(double fSumpyModulePF)    { SumpyModulePF_ = fSumpyModulePF;}
    void SetSumpzModulePF(double fSumpzModulePF)    { SumpzModulePF_ = fSumpzModulePF;}
    void SetSumpxPF(double fSumpxPF)    { SumpxPF_ = fSumpxPF;}
    void SetSumpyPF(double fSumpyPF)    { SumpyPF_ = fSumpyPF;}
    void SetSumpzPF(double fSumpzPF)    { SumpzPF_ = fSumpzPF;}
    void SetSumEnergyPF(double fSumEnergyPF)    { SumEnergyPF_ = fSumEnergyPF;}
    void SetMuEnergyPF(double fMuEnergyPF)    { MuEnergyPF_ = fMuEnergyPF;}
    void SetElectronEnergyPF(double fElectronEnergyPF)    { ElectronEnergyPF_ = fElectronEnergyPF;}
    void SetMaxGapPF(double fMaxGapPF)    { MaxGapPF_ = fMaxGapPF;}
    void SetSecondMaxGapPF(double fSecondMaxGapPF)    { SecondMaxGapPF_ = fSecondMaxGapPF;}
    void SetLimPlusGapPF(double fLimPlusGapPF)    { LimPlusGapPF_ = fLimPlusGapPF;}
    void SetLimMinusGapPF(double fLimMinusGapPF)    { LimMinusGapPF_ = fLimMinusGapPF;}
    void SetMultiplicityGapPlusPF(int fMultiplicityGapPlusPF)    { MultiplicityGapPlusPF_ = fMultiplicityGapPlusPF;}
    void SetMultiplicityGapMinusPF(int fMultiplicityGapMinusPF)    { MultiplicityGapMinusPF_ = fMultiplicityGapMinusPF;}

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

    double GetVertexMultiplicity(int i) const { return VertexMultiplicity_[i]; }
    double GetVertexChiNorm(int i) const { return VertexChiNorm_[i]; }
    double GetVertexNDOF(int i) const { return VertexNDOF_[i]; }
    double GetVz(int i) const { return Vz_[i]; }
    double GetVx(int i) const { return Vx_[i]; }
    double GetVy(int i) const { return Vy_[i]; }
    double GetTracksPt(int i,int j) const { return TracksPt_[i][j]; }

    double GetPrimaryGapMaxGen()    const {return PrimaryGapMaxGen_;}
    double GetSecondGapMaxGen()    const {return SecondGapMaxGen_;}
    double GetTracksPtGen(int i)    const {return TracksPtGen_[i];}
    double GetEtaOfTracksPtGen(int i)    const {return EtaOfTracksPtGen_[i];}
    int GetNTracksGen()    const {return NTracksGen_;}
    double GetMx2PlusGen()    const {return Mx2PlusGen_;}
    double GetMx2MinusGen()    const {return Mx2MinusGen_;}
    double GetMx2Gen()    const {return Mx2Gen_;}
    double GetMx2ZGen()    const {return Mx2ZGen_;}
    int GetNMx2PlusGen()    const {return NMx2PlusGen_;}
    int GetNMx2MinusGen()    const {return NMx2MinusGen_;}
    double GetEtaGaplimPlusGen()    const {return EtaGaplimPlusGen_;}
    double GetEtaGaplimMinusGen()    const {return EtaGaplimMinusGen_;}
    int GetNParticlesGen()    const {return NParticlesGen_;}
    double GetsumECastorMinusGen()    const {return sumECastorMinusGen_;}
    double GetsumECastorPlusGen()    const {return sumECastorPlusGen_;}
    double GetsumEZDCMinusGen()    const {return sumEZDCMinusGen_;}
    double GetsumEZDCPlusGen()    const {return sumEZDCPlusGen_;}
    double GetEtaOutcomingProtonGen()    const {return EtaOutcomingProtonGen_;}
    double GetxLGen()   const {return xLGen_;}
    double GetxLMostEnergeticGen()    const {return xLMostEnergeticGen_;}
    double GetxiZMinusGen()    const {return xiZMinusGen_;}
    double GetxiZPlusGen()    const {return xiZPlusGen_;}
    double GetEtaZGen()    const {return EtaZGen_;}
    double GetEnergyZGen()    const {return EnergyZGen_;}
    double GetpDissMassGen()    const {return pDissMassGen_;}
    double GetxLpDissMass()    const {return xLpDissMass_;}

    double GetDiElectronMassPF() const {return DiElectronMassPF_;}
    double GetDiMuonMassPF() const {return DiMuonMassPF_;}

    double GetSumEHFPlus()    const {return SumEHFPlus_;}
    double GetSumEHF_SPlus()    const {return SumEHF_SPlus_;}
    double GetSumEHF_LPlus()    const {return SumEHF_LPlus_;}
    double GetSumEtHFPlus()    const {return SumEtHFPlus_;}

    double GetSumEHFMinus()    const {return SumEHFMinus_;}
    double GetSumEHF_SMinus()    const {return SumEHF_SMinus_;}
    double GetSumEHF_LMinus()    const {return SumEHF_LMinus_;}
    double GetSumEtHFMinus()    const {return SumEtHFMinus_;}

    double GetSumEHEPlus()    const {return SumEHEPlus_;}
    double GetSumEtHEPlus()    const {return SumEtHEPlus_;}
    double GetSumEHEMinus()    const {return SumEHEMinus_;}
    double GetSumEtHEMinus()    const {return SumEtHEMinus_;}

    double GetSumEHBPlus()    const {return SumEHBPlus_;}
    double GetSumEtHBPlus()    const {return SumEtHBPlus_;}
    double GetSumEHBMinus()    const {return SumEHBMinus_;}
    double GetSumEtHBMinus()    const {return SumEtHBMinus_;}

    double GetSumEEEPlus()    const {return SumEEEPlus_;}
    double GetSumEtEEPlus()    const {return SumEtEEPlus_;}
    double GetSumEEEMinus()    const {return SumEEEMinus_;}
    double GetSumEtEEMinus()    const {return SumEtEEMinus_;}

    double GetSumEEBPlus()    const {return SumEEBPlus_;}
    double GetSumEtEBPlus()    const {return SumEtEBPlus_;}
    double GetSumEEBMinus()    const {return SumEEBMinus_;}
    double GetSumEtEBMinus()    const {return SumEtEBMinus_;}

    double GetEPZCaloPlus()    const {return EPZCaloPlus_;}
    double GetEPZCaloMinus()    const {return EPZCaloMinus_;}
    double GetXiCaloPlus()    const {return XiCaloPlus_;}
    double GetXiCaloMinus()    const {return XiCaloMinus_;}

    double GetEtaCaloMax()    const {return EtaCaloMax_;}
    double GetEtaCaloMin()    const {return EtaCaloMin_;}

    int GetVertex()    const {return Vertex_;}
    double GetXi_PF_minus()    const {return Xi_PF_minus_;}
    double GetXi_PF_plus()    const {return Xi_PF_plus_;}
    double GetEpz_PF_minus()    const {return Epz_PF_minus_;}
    double GetEpz_PF_plus()    const {return Epz_PF_plus_;}
    int GetMultiplicityPF()    const {return MultiplicityPF_;}
    double GetSumEtaTimesEnergyPF()    const {return SumEtaTimesEnergyPF_;}
    double GetSumpxModulePF()    const {return SumpxModulePF_;}
    double GetSumpyModulePF()    const {return SumpyModulePF_;}
    double GetSumpzModulePF()    const {return SumpzModulePF_;}
    double GetSumpxPF()    const {return SumpxPF_;}
    double GetSumpyPF()    const {return SumpyPF_;}
    double GetSumpzPF()    const {return SumpzPF_;}
    double GetSumEnergyPF()    const {return SumEnergyPF_;}
    double GetMuEnergyPF()    const {return MuEnergyPF_;}
    double GetElectronEnergyPF()    const {return MaxGapPF_;}
    double GetMaxGapPF()    const {return EtaCaloMin_;}
    double GetSecondMaxGapPF()    const {return SecondMaxGapPF_;}
    double GetLimPlusGapPF()    const {return LimPlusGapPF_;}
    double GetLimMinusGapPF()    const {return LimMinusGapPF_;}
    int GetMultiplicityGapPlusPF()    const {return MultiplicityGapPlusPF_;}
    int GetMultiplicityGapMinusPF()    const {return MultiplicityGapMinusPF_;}

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

    std::vector<double> VertexMultiplicity_;
    std::vector<double> VertexChiNorm_;
    std::vector<double> VertexNDOF_;
    std::vector<double> Vz_;
    std::vector<double> Vx_;
    std::vector<double> Vy_;
    std::vector<std::vector<double> > TracksPt_;

    double PrimaryGapMaxGen_;
    double SecondGapMaxGen_;
    std::vector<double> TracksPtGen_;
    std::vector<double> EtaOfTracksPtGen_;
    int NTracksGen_;
    double Mx2PlusGen_;
    double Mx2MinusGen_;
    double Mx2Gen_;
    double Mx2ZGen_;
    int NMx2PlusGen_;
    int NMx2MinusGen_;
    double EtaGaplimPlusGen_;
    double EtaGaplimMinusGen_;
    int NParticlesGen_;
    double sumECastorMinusGen_;
    double sumECastorPlusGen_;
    double sumEZDCMinusGen_;
    double sumEZDCPlusGen_;
    double EtaOutcomingProtonGen_;
    double xLGen_;
    double xLMostEnergeticGen_;
    double xiZMinusGen_;
    double xiZPlusGen_;
    double EtaZGen_;
    double EnergyZGen_;
    double pDissMassGen_;
    double xLpDissMass_;

    double DiElectronMassPF_;
    double DiMuonMassPF_;

    double SumEHFPlus_;
    double SumEHF_SPlus_;
    double SumEHF_LPlus_;
    double SumEtHFPlus_;
    double SumEHFMinus_;
    double SumEHF_SMinus_;
    double SumEHF_LMinus_;
    double SumEtHFMinus_;
    double SumEHEPlus_;
    double SumEtHEPlus_;
    double SumEHEMinus_;
    double SumEtHEMinus_;
    double SumEHBPlus_;
    double SumEtHBPlus_;
    double SumEHBMinus_;
    double SumEtHBMinus_;
    double SumEEEPlus_;
    double SumEtEEPlus_;
    double SumEEEMinus_;
    double SumEtEEMinus_;
    double SumEEBPlus_;
    double SumEtEBPlus_;
    double SumEEBMinus_;
    double SumEtEBMinus_;
    double EPZCaloPlus_;
    double EPZCaloMinus_;
    double XiCaloPlus_;
    double XiCaloMinus_;
    double EtaCaloMax_;
    double EtaCaloMin_;

    int Vertex_;
    double Xi_PF_minus_;
    double Xi_PF_plus_;
    double Epz_PF_minus_;
    double Epz_PF_plus_;
    int MultiplicityPF_;
    double SumEtaTimesEnergyPF_;
    double SumpxModulePF_;
    double SumpyModulePF_;
    double SumpzModulePF_;
    double SumpxPF_;
    double SumpyPF_;
    double SumpzPF_;
    double SumEnergyPF_;
    double MuEnergyPF_;
    double ElectronEnergyPF_;
    double MaxGapPF_;
    double SecondMaxGapPF_;
    double LimPlusGapPF_;
    double LimMinusGapPF_;
    int MultiplicityGapPlusPF_;
    int MultiplicityGapMinusPF_;

};

#endif
