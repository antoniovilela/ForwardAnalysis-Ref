#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZAnalysis.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZEvent.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "TLorentzVector.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TMath.h"

#include "FWCore/Utilities/interface/RegexMatch.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"

#include "DataFormats/ParticleFlowReco/interface/PFBlockElementTrack.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "DataFormats/HcalDetId/interface/HcalZDCDetId.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "ForwardAnalysis/Utilities/interface/CastorEnergy.h"

#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"
#include "DataFormats/HcalDetId/interface/HcalElectronicsId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"

using diffractiveZAnalysis::DiffractiveZAnalysis;

const char* DiffractiveZAnalysis::name = "DiffractiveZAnalysis";

DiffractiveZAnalysis::DiffractiveZAnalysis(const edm::ParameterSet& pset):
  triggerResultsTag_(pset.getParameter<edm::InputTag>("TriggerResultsTag")),
  hltPathNames_(pset.getParameter<std::vector<std::string> >("hltPaths")),
  electronTag_(pset.getParameter<edm::InputTag>("electronTag")),
  muonTag_(pset.getParameter<edm::InputTag>("muonTag")),
  pfTag_(pset.getParameter<edm::InputTag>("pfTag")),
  genTag_(pset.getParameter<edm::InputTag>("genTag")),
  PVtxCollectionTag_(pset.getParameter<edm::InputTag>("PVtxCollectionTag")),
  castorHitsTag_(pset.getParameter<edm::InputTag>("castorHitsTag")),
  zdcHitsTag_(pset.getParameter<edm::InputTag>("zdcHitsTag")),
  RunCastor_(pset.getUntrackedParameter<Bool_t>("RunCastor", false)),
  RunZDC_(pset.getUntrackedParameter<Bool_t>("RunZDC", false)),
  RunMC_(pset.getUntrackedParameter<Bool_t>("RunMC", false)),
  RunZPat_(pset.getUntrackedParameter<Bool_t>("RunZPat", false)),
  EachTower_(pset.getUntrackedParameter<Bool_t>("EachTower", false)),
  pTPFThresholdCharged_(pset.getParameter<double>("pTPFThresholdCharged")),
  energyPFThresholdBar_(pset.getParameter<double>("energyPFThresholdBar")),
  energyPFThresholdEnd_(pset.getParameter<double>("energyPFThresholdEnd")),
  energyPFThresholdHF_(pset.getParameter<double>("energyPFThresholdHF")),
  energyThresholdHB_(pset.getParameter<double>("energyThresholdHB")),
  energyThresholdHE_(pset.getParameter<double>("energyThresholdHE")),
  energyThresholdHF_(pset.getParameter<double>("energyThresholdHF")),
  energyThresholdEB_(pset.getParameter<double>("energyThresholdEB")),
  energyThresholdEE_(pset.getParameter<double>("energyThresholdEE")),
  castorThreshold_(pset.getParameter<double>("castorThreshold")),
  fCGeVCastor_(pset.getParameter<double>("fCGeVCastor")),
  caloTowerTag_(pset.getParameter<edm::InputTag>("CaloTowerTag"))
{
}

void DiffractiveZAnalysis::setTFileService(){
  edm::Service<TFileService> fs;
  std::ostringstream oss;
  hltTriggerNamesHisto_ = fs->make<TH1F>("HLTTriggerNames","HLTTriggerNames",1,0,1);
  hltTriggerNamesHisto_->SetBit(TH1::kCanRebin);
  for(unsigned k=0; k < hltPathNames_.size(); ++k){
    oss << "Using HLT reference trigger " << hltPathNames_[k] << std::endl;
    hltTriggerNamesHisto_->Fill(hltPathNames_[k].c_str(),1);
  }
  edm::LogVerbatim("Analysis") << oss.str();

  hltTriggerPassHisto_ = fs->make<TH1F>("HLTTriggerPass","HLTTriggerPass",1,0,1);
  hltTriggerPassHisto_->SetBit(TH1::kCanRebin);
}

DiffractiveZAnalysis::~DiffractiveZAnalysis(){}

void DiffractiveZAnalysis::begin() {
  setTFileService();
}

void DiffractiveZAnalysis::begin(const edm::Run& run, const edm::EventSetup& setup) {}

void DiffractiveZAnalysis::end() {}

void DiffractiveZAnalysis::fill(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  eventData.reset();

  fillTriggerInfo(eventData,event,setup);
  fillMuonsInfo(eventData,event,setup);
  fillElectronsInfo(eventData,event,setup);
  fillTracksInfo(eventData,event,setup);
  fillDetectorVariables(eventData,event,setup);
  fillVariables(eventData,event,setup);
  if (RunMC_) fillGenInfo(eventData,event,setup); 
  if (RunZPat_) fillZPat(eventData,event,setup);
  if (RunCastor_) fillCastor(eventData,event,setup);
  if (RunZDC_) fillZDC(eventData,event,setup);
  if (EachTower_) fillDetectorEnergyEtaInfo(eventData,event,setup);

}

// Fill Trigger
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillTriggerInfo(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  edm::Handle<edm::TriggerResults> triggerResults;
  event.getByLabel(triggerResultsTag_, triggerResults);

  if( triggerResults.isValid() ){
    const edm::TriggerNames& triggerNames = event.triggerNames(*triggerResults);

    size_t idxpath = 0;
    std::vector<std::string>::const_iterator hltpath = hltPathNames_.begin();
    std::vector<std::string>::const_iterator hltpaths_end = hltPathNames_.end(); 
    for(; hltpath != hltpaths_end; ++hltpath,++idxpath){
      std::string resolvedPathName; 
      if( edm::is_glob( *hltpath ) ){
	std::vector< std::vector<std::string>::const_iterator > matches = edm::regexMatch(triggerNames.triggerNames(), *hltpath);

	if( matches.empty() )
	  throw cms::Exception("Configuration") << "Could not find any HLT path of type " << *hltpath << "\n";
	else if( matches.size() > 1)
	  throw cms::Exception("Configuration") << "HLT path type " << *hltpath << " not unique\n";
	else resolvedPathName = *(matches[0]);
      } else{
	resolvedPathName = *hltpath;
      }

      int idx_HLT = triggerNames.triggerIndex(resolvedPathName);
      int accept_HLT = ( triggerResults->wasrun(idx_HLT) && triggerResults->accept(idx_HLT) ) ? 1 : 0;
      eventData.SetHLTPath(idxpath, accept_HLT);
      hltTriggerPassHisto_->Fill( (*hltpath).c_str(), 1 ); 
    }

  }else{
    std::cout << "\n No valid trigger result." <<std::endl;
  }

}

// Fill Reco::Electron
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillElectronsInfo(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  int ElectronN=0;
  bool debug = false;

  edm::Handle<reco::GsfElectronCollection> electrons;
  event.getByLabel(electronTag_,electrons);

  const reco::GsfElectron* electron1=NULL;
  const reco::GsfElectron* electron2=NULL;

  int electronsize = electrons->size();
  int itElectron;

  if(electrons->size()>1){

    for(itElectron=0; itElectron < electronsize; ++itElectron){

      ++ElectronN;

      const reco::GsfElectron* electronAll = &((*electrons)[itElectron]);

      if (electronAll==NULL) continue;
      if (electron1==NULL) {electron1=electronAll; continue;}
      if (electronAll->pt()>electron1->pt()) {
	electron2=electron1;
	electron1=electronAll;
	continue;
      }

      if (electron2==NULL) {electron2=electronAll; continue;}
      if (electronAll->pt()>electron2->pt()) electron2 = electronAll;

    }

    double relIsoFirstElectronDr03 = (electron1->dr03TkSumPt()+electron1->dr03EcalRecHitSumEt()+electron1->dr03HcalTowerSumEt())/electron1->et();
    double relIsoFirstElectronDr04 = (electron1->dr04TkSumPt()+electron1->dr04EcalRecHitSumEt()+electron1->dr04HcalTowerSumEt())/electron1->et();
    double relIsoSecondElectronDr03 = (electron2->dr03TkSumPt()+electron2->dr03EcalRecHitSumEt()+electron2->dr03HcalTowerSumEt())/electron2->et();
    double relIsoSecondElectronDr04 = (electron2->dr04TkSumPt()+electron2->dr04EcalRecHitSumEt()+electron2->dr04HcalTowerSumEt())/electron2->et();

    // Dielectron Mass
    math::XYZTLorentzVector DielectronSystem(0.,0.,0.,0.);
    DielectronSystem += electron1->p4();
    DielectronSystem += electron2->p4();
    eventData.SetDiElectronMass(DielectronSystem.M());
    eventData.SetDiElectronPt(DielectronSystem.pt());
    eventData.SetDiElectronEta(DielectronSystem.eta());
    eventData.SetDiElectronPhi(DielectronSystem.phi());

    eventData.SetElectronsN(ElectronN);
    eventData.SetLeadingElectronPt(electron1->pt());
    eventData.SetLeadingElectronEta(electron1->eta());
    eventData.SetLeadingElectronPhi(electron1->phi());
    eventData.SetLeadingElectronCharge(electron1->charge());
    eventData.SetLeadingElectronP4(electron1->p4());
    eventData.SetSecondElectronPt(electron2->pt());
    eventData.SetSecondElectronEta(electron2->eta());
    eventData.SetSecondElectronPhi(electron2->phi());
    eventData.SetSecondElectronCharge(electron2->charge());
    eventData.SetSecondElectronP4(electron2->p4());

    eventData.SetLeadingElectronTkDr03(electron1->dr03TkSumPt());
    eventData.SetLeadingElectronEcalDr03(electron1->dr03EcalRecHitSumEt());
    eventData.SetLeadingElectronHcalDr03(electron1->dr03HcalTowerSumEt());

    eventData.SetSecondElectronTkDr03(electron2->dr03TkSumPt());
    eventData.SetSecondElectronEcalDr03(electron2->dr03EcalRecHitSumEt());
    eventData.SetSecondElectronHcalDr03(electron2->dr03HcalTowerSumEt());

    eventData.SetLeadingElectronTkDr04(electron1->dr04TkSumPt());
    eventData.SetLeadingElectronEcalDr04(electron1->dr04EcalRecHitSumEt());
    eventData.SetLeadingElectronHcalDr04(electron1->dr04HcalTowerSumEt());

    eventData.SetSecondElectronTkDr04(electron2->dr04TkSumPt());
    eventData.SetSecondElectronEcalDr04(electron2->dr04EcalRecHitSumEt());
    eventData.SetSecondElectronHcalDr04(electron2->dr04HcalTowerSumEt());

    eventData.SetLeadingElectronrelIsoDr03(relIsoFirstElectronDr03);
    eventData.SetLeadingElectronrelIsoDr04(relIsoFirstElectronDr04);
    eventData.SetSecondElectronrelIsoDr03(relIsoSecondElectronDr03);
    eventData.SetSecondElectronrelIsoDr04(relIsoSecondElectronDr04);


    if (debug){
      std::cout << "electron1 -> dr03 TK: " << electron1->dr03TkSumPt() << "| dr03 Ecal: " << electron1->dr03EcalRecHitSumEt() << " | dr03 Hcal: " << electron1->dr03HcalTowerSumEt() << std::endl;
      std::cout << "electron1 -> dr04 TK: " << electron1->dr04TkSumPt() << "| dr04 Ecal: " << electron1->dr04EcalRecHitSumEt() << " | dr04 Hcal: " << electron1->dr04HcalTowerSumEt() <<  std::endl;
      std::cout << "electron2 -> dr03 TK: " << electron2->dr03TkSumPt() << "| dr03 Ecal: " << electron2->dr03EcalRecHitSumEt() << " | dr03 Hcal: " << electron2->dr03HcalTowerSumEt() << std::endl;
      std::cout << "electron2 -> dr04 TK: " << electron2->dr04TkSumPt() << "| dr04 Ecal: " << electron2->dr04EcalRecHitSumEt() << " | dr04 Hcal: " << electron2->dr04HcalTowerSumEt() <<  std::endl;
      std::cout << "Electron Isolation: " << relIsoFirstElectronDr03 << " | " << relIsoFirstElectronDr04 << std::endl;
      std::cout << "NElectron: " << ElectronN << std::endl;
      std::cout << "NSize: " << electrons->size() << std::endl;
      std::cout << "Electron1: " << electron1->pt() << std::endl;
      std::cout << "Electron2: " << electron2->pt() << std::endl;
      std::cout << "Eta Z: " << DielectronSystem.eta() << std::endl;
      std::cout << "Phi Z: " << DielectronSystem.phi() << std::endl;
      std::cout << "pT Z: " << DielectronSystem.pt() << std::endl;
    }

  }
  else {
    eventData.SetDiElectronMass(-999.);
    eventData.SetDiElectronPt(-999.);
    eventData.SetDiElectronEta(-999.);
    eventData.SetDiElectronPhi(-999.);
    eventData.SetElectronsN(0);
    eventData.SetLeadingElectronPt(-999.);
    eventData.SetLeadingElectronEta(-999.);
    eventData.SetLeadingElectronPhi(-999.);
    eventData.SetLeadingElectronCharge(-999);
    eventData.SetSecondElectronPt(-999.);
    eventData.SetSecondElectronEta(-999.);
    eventData.SetSecondElectronPhi(-999.);
    eventData.SetSecondElectronCharge(-999);

    eventData.SetLeadingElectronTkDr03(-999.);
    eventData.SetLeadingElectronEcalDr03(-999.);
    eventData.SetLeadingElectronHcalDr03(-999.);

    eventData.SetSecondElectronTkDr03(-999.);
    eventData.SetSecondElectronEcalDr03(-999.);
    eventData.SetSecondElectronHcalDr03(-999.);

    eventData.SetLeadingElectronTkDr04(-999.);
    eventData.SetLeadingElectronEcalDr04(-999.);
    eventData.SetLeadingElectronHcalDr04(-999.);

    eventData.SetSecondElectronTkDr04(-999.);
    eventData.SetSecondElectronEcalDr04(-999.);
    eventData.SetSecondElectronHcalDr04(-999.);

    eventData.SetLeadingElectronrelIsoDr03(-999.);
    eventData.SetLeadingElectronrelIsoDr04(-999.);
    eventData.SetSecondElectronrelIsoDr03(-999.);
    eventData.SetSecondElectronrelIsoDr04(-999.);
  }

}

// Fill Reco::Muon
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillMuonsInfo(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  bool debug = false;
  int MuonsN=0;

  edm::Handle<reco::MuonCollection> muons;
  event.getByLabel(muonTag_,muons);

  const reco::Muon* muon1=NULL;
  const reco::Muon* muon2=NULL;

  int muonsize = muons->size();
  int itMuon;

  if(muons->size()>1){

    for(itMuon=0; itMuon < muonsize; ++itMuon){

      ++MuonsN;

      const reco::Muon* muonAll = &((*muons)[itMuon]);

      if (muonAll==NULL) continue;

      if (muon1==NULL) {muon1=muonAll; continue;}
      if (muonAll->pt()>muon1->pt()) {
	muon2=muon1;
	muon1=muonAll;
	continue;
      }

      if (muon2==NULL) {muon2=muonAll; continue;}
      if (muonAll->pt()>muon2->pt()) muon2 = muonAll;


      if (debug){
	std::cout << "\n----------------------------------------------------" << std::endl;
	std::cout << "\t energy (ecal, hcal, ho): " << muon1->calEnergy().em << ", " << muon1->calEnergy().had << ", " << muon1->calEnergy().ho << std::endl;
	std::cout << "\t isolation dR=0.3 (sumPt, emEt, hadEt, hoEt, nTracks, nJets): " << muon1->isolationR03().sumPt << ", " << muon1->isolationR03().emEt << ", " << muon1->isolationR03().hadEt << ", " << muon1->isolationR03().hoEt << ", " << muon1->isolationR03().nTracks << ", " << muon1->isolationR03().nJets << std::endl;
	std::cout << "\t isolation dR=0.5 (sumPt, emEt, hadEt, hoEt, nTracks, nJets): " << muon1->isolationR05().sumPt << ", " << muon1->isolationR05().emEt << ", " << muon1->isolationR05().hadEt << ", " << muon1->isolationR05().hoEt << ", " << muon1->isolationR05().nTracks << ", " << muon1->isolationR05().nJets << std::endl;
	std::cout << "\t # matches: " << muon1->numberOfMatches() << std::endl;
	std::cout << "\t # caloCompatibility: " << muon1->caloCompatibility() << std::endl;  
      }

    }

    double muon1SumPtR03 = muon1->isolationR03().sumPt;
    double muon1EmEtR03 = muon1->isolationR03().emEt;
    double muon1HadEtR03 = muon1->isolationR03().hadEt;
    double muon1SumPtR05 = muon1->isolationR05().sumPt;
    double muon1EmEtR05 = muon1->isolationR05().emEt;
    double muon1HadEtR05 = muon1->isolationR05().hadEt;

    double muon2SumPtR03 = muon2->isolationR03().sumPt;
    double muon2EmEtR03 = muon2->isolationR03().emEt;
    double muon2HadEtR03 = muon2->isolationR03().hadEt;
    double muon2SumPtR05 = muon2->isolationR05().sumPt;
    double muon2EmEtR05 = muon2->isolationR05().emEt;
    double muon2HadEtR05 = muon2->isolationR05().hadEt;

    double relIsoFirstMuonDr03 = (muon1SumPtR03 + muon1EmEtR03 + muon1HadEtR03)/muon1->pt();
    double relIsoSecondMuonDr03 = (muon2SumPtR03 + muon2EmEtR03 + muon2HadEtR03)/muon2->pt();
    double relIsoFirstMuonDr05 = (muon1SumPtR05 + muon1EmEtR05 + muon1HadEtR05)/muon1->pt();
    double relIsoSecondMuonDr05 = (muon2SumPtR05 + muon2EmEtR05 + muon2HadEtR05)/muon2->pt();

    // Dimuon Mass
    math::XYZTLorentzVector DimuonSystem(0.,0.,0.,0.);
    DimuonSystem += muon1->p4();
    DimuonSystem += muon2->p4();
    eventData.SetDiMuonMass(DimuonSystem.M());
    eventData.SetDiMuonPt(DimuonSystem.pt());
    eventData.SetDiMuonEta(DimuonSystem.eta());
    eventData.SetDiMuonPhi(DimuonSystem.phi());
    eventData.SetMuonsN(MuonsN);
    eventData.SetLeadingMuonPt(muon1->pt());
    eventData.SetLeadingMuonEta(muon1->eta());
    eventData.SetLeadingMuonPhi(muon1->phi());
    eventData.SetLeadingMuonCharge(muon1->charge());
    eventData.SetLeadingMuonP4(muon1->p4());
    eventData.SetSecondMuonPt(muon2->pt());
    eventData.SetSecondMuonEta(muon2->eta());
    eventData.SetSecondMuonPhi(muon2->phi());
    eventData.SetSecondMuonCharge(muon2->charge());
    eventData.SetSecondMuonP4(muon2->p4());

    eventData.SetLeadingMuonSumPtR03(muon1SumPtR03);
    eventData.SetLeadingMuonEmEtR03(muon1EmEtR03);
    eventData.SetLeadingMuonHadEtR03(muon1HadEtR03);
    eventData.SetLeadingMuonSumPtR05(muon1SumPtR05);
    eventData.SetLeadingMuonEmEtR05(muon1EmEtR05);
    eventData.SetLeadingMuonHadEtR05(muon1HadEtR05);

    eventData.SetSecondMuonSumPtR03(muon2SumPtR03);
    eventData.SetSecondMuonEmEtR03(muon2EmEtR03);
    eventData.SetSecondMuonHadEtR03(muon2HadEtR03);
    eventData.SetSecondMuonSumPtR05(muon2SumPtR05);
    eventData.SetSecondMuonEmEtR05(muon2EmEtR05);
    eventData.SetSecondMuonHadEtR05(muon2HadEtR05);

    eventData.SetLeadingMuonrelIsoDr03(relIsoFirstMuonDr03);
    eventData.SetSecondMuonrelIsoDr03(relIsoSecondMuonDr03);
    eventData.SetLeadingMuonrelIsoDr05(relIsoFirstMuonDr05);
    eventData.SetSecondMuonrelIsoDr05(relIsoSecondMuonDr05);

    if (debug){
      std::cout << "NMuons: " << MuonsN << std::endl;
      std::cout << "NSize: " << muons->size() << std::endl;
      std::cout << "Muon1: " << muon1->pt() << std::endl;
      std::cout << "Muon2: " << muon2->pt() << std::endl;
      std::cout << "Eta Z: " << DimuonSystem.eta() << std::endl;
      std::cout << "Phi Z: " << DimuonSystem.phi() << std::endl;
      std::cout << "pT Z: " << DimuonSystem.pt() << std::endl;
    }

  }
  else{
    eventData.SetDiMuonMass(-999.);
    eventData.SetDiMuonPt(-999.);
    eventData.SetDiMuonEta(-999.);
    eventData.SetDiMuonPhi(-999.);
    eventData.SetMuonsN(0);
    eventData.SetLeadingMuonPt(-999.);
    eventData.SetLeadingMuonEta(-999.);
    eventData.SetLeadingMuonPhi(-999.);
    eventData.SetLeadingMuonCharge(-999);
    eventData.SetSecondMuonPt(-999.);
    eventData.SetSecondMuonEta(-999.);
    eventData.SetSecondMuonPhi(-999.);
    eventData.SetSecondMuonCharge(-999);

    eventData.SetLeadingMuonSumPtR03(-999.);
    eventData.SetLeadingMuonEmEtR03(-999.);
    eventData.SetLeadingMuonHadEtR03(-999.);
    eventData.SetLeadingMuonSumPtR05(-999.);
    eventData.SetLeadingMuonEmEtR05(-999.);
    eventData.SetLeadingMuonHadEtR05(-999.);

    eventData.SetSecondMuonSumPtR03(-999.);
    eventData.SetSecondMuonEmEtR03(-999.);
    eventData.SetSecondMuonHadEtR03(-999.);
    eventData.SetSecondMuonSumPtR05(-999.);
    eventData.SetSecondMuonEmEtR05(-999.);
    eventData.SetSecondMuonHadEtR05(-999.);

    eventData.SetLeadingMuonrelIsoDr03(-999.);
    eventData.SetSecondMuonrelIsoDr03(-999.);
    eventData.SetLeadingMuonrelIsoDr05(-999.);
    eventData.SetSecondMuonrelIsoDr05(-999.);

  } 

}

// Fill Tracks Info
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillTracksInfo(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  std::vector<double> vertexNDOF;
  std::vector<double> vertexChiNorm;
  std::vector<double> vertexMultiplicity;
  double nhit=0;
  std::vector<double> V_x;
  std::vector<double> V_y;
  std::vector<double> V_z; 
  std::vector<double> pt;
  std::vector<double> tracks;
  std::vector<std::vector<double> > tracksPT;

  edm::Handle<reco::VertexCollection>  vertexCollectionHandle;
  event.getByLabel(PVtxCollectionTag_, vertexCollectionHandle);

  for(reco::VertexCollection::const_iterator vtx = vertexCollectionHandle->begin();vtx!=vertexCollectionHandle->end(); ++vtx)
  {
    reco::Vertex::trackRef_iterator it = vtx->tracks_begin();
    reco::Vertex::trackRef_iterator lastTrack = vtx->tracks_end();


    for(;it!=lastTrack;it++) {
      nhit+=(*it)->numberOfValidHits();
      pt.push_back((*it)->pt());
    }

    //Sorting the pt tracks, in order to take only the 31 most energetics
    const int  size = (int) pt.size();
    int *sorted = new int[size];
    double *v = new double[size];

    for (int i=0; i<size; i++) {
      v[i] = pt[i];
    }
    TMath::Sort(size, v, sorted, true);
    for (int i=0; i<size; i++) {
      tracks.push_back(pt[sorted[i]]);
      if (i>30) break;
    }

    tracksPT.push_back(tracks);
    tracks.clear();
    pt.clear();

    double ndof=vtx->ndof();
    double chiNorm=vtx->normalizedChi2();
    double NumbOfTracks=vtx->tracksSize();
    vertexNDOF.push_back(ndof);
    vertexChiNorm.push_back(chiNorm);
    vertexMultiplicity.push_back(NumbOfTracks);
    nhit=0;
    if ( ndof != 0 ) {
      V_x.push_back(vtx->x());
      V_y.push_back(vtx->y());
      V_z.push_back(vtx->z());
    } else {
      V_x.push_back(-999);
      V_y.push_back(-999);
      V_z.push_back(-999);
    }

  } // loop over vtx

  eventData.SetVertexMultiplicity(vertexMultiplicity);
  eventData.SetVertexChiNorm(vertexChiNorm);
  eventData.SetVertexNDOF(vertexNDOF);
  eventData.SetVz(V_z);
  eventData.SetVx(V_x);
  eventData.SetVy(V_y); 
  eventData.SetTracksPt(tracksPT);

}

// Fill Gen Level Information
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillGenInfo(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  bool debug=false;

  //Variable declaration
  int count=0;
  int Nstable_gen=0;
  math::XYZTLorentzVector part(0.,0.,0.,0.);
  math::XYZTLorentzVector partVis(0.,0.,0.,0.);
  math::XYZTLorentzVector partZ(0.,0.,0.,0.);
  double sumECastor_minus_gen=0;
  double sumECastor_plus_gen=0;
  double sumEZDC_minus_gen=0;
  double sumEZDC_plus_gen=0;
  double etaOutcomingProton=0;
  double energyOutcomingProton=0;
  double mostEnergeticXL=0;
  double mostEnergeticXLNum=0;
  std::vector<double> eta_gen_vec;
  double xi_Z_gen_minus=0;
  double xi_Z_gen_plus=0;
  double etaZ_gen=0;
  double energyZ_gen=0;
  double p_diss_mass=0;
  double p_diss=0;
  double xL_p_diss=0;

  double xL_etaGTP5=0;
  double xL_etaLTM5=0;
  int xL_LTM5Num=0;
  int xL_GTP5Num=0;

  std::vector<double> genpt;
  std::vector<double> tracks;
  std::vector<double> eta;
  std::vector<double> etaPT;
  std::vector<double> tracksPT;

  edm::Handle<reco::GenParticleCollection> genParticles;     
  event.getByLabel(genTag_,genParticles);  // standard PYTHIA collection

  for(size_t i = 0; i < genParticles->size(); ++ i) {

    const reco::GenParticle & p = (*genParticles)[i];

    int pdg = p.pdgId();
    int status = p.status();  	 
    double eta_gen = p.eta();
    double part_pt = p.pt();
    double ener_gen= p.energy();
    double px_gen  = p.px();
    double py_gen  = p.py();
    double pz_gen  = p.pz();
    double mass_gen= p.mass();
    bool electronFromZ=false;
    int motherId=0;

    if (  p.mother() ) motherId =  p.mother()->pdgId();

    math::XYZTLorentzVector tmp( px_gen ,py_gen , pz_gen ,ener_gen );

    if (fabs(pdg)==11 || fabs(pdg)==22){ 	    
      if (motherId==23) {
	electronFromZ=true;
	partZ+=tmp;
      }
    }

    if (count==2) {    /// only works for MC Pompyt dissociative
      p_diss_mass= mass_gen;
      p_diss= pz_gen;
      if ( pdg == 2212){
	etaOutcomingProton= eta_gen;
	energyOutcomingProton= ener_gen;
      }
    }

    if (( pdg == 23)){
      xi_Z_gen_minus=( ener_gen - pz_gen )/7000;
      xi_Z_gen_plus=( ener_gen + pz_gen )/7000;
      etaZ_gen=eta_gen;
      energyZ_gen= ener_gen;
    }

    if (status == 1) 
    {
      //// vector to find gaps (cut at 1 GeV in energy)
      if  ( ( fabs(eta_gen) <= 1.5  && ener_gen > energyPFThresholdBar_ )  ||
	  (fabs(eta_gen) > 1.5 && fabs(eta_gen) <= 3 && ener_gen > energyPFThresholdEnd_) ||
	  (fabs(eta_gen) > 3 && ener_gen >energyPFThresholdHF_)  ) {

	eta_gen_vec.push_back( eta_gen);
      }

      if (  count>2) {   
	part+=tmp;
      }

      if (  (fabs(eta_gen) < 4.7) && (part_pt > 0.10) ) {   // if particle has a chance to reach the detector ...
	partVis+=tmp;
      }

      // new xL_gen definition (after Sasha)
      if (count>=2 )
      {
	if (eta_gen > 4.7)  
	{
	  xL_etaGTP5 += pz_gen;
	  xL_GTP5Num++;
	}
	if (eta_gen < -4.7)  
	{
	  xL_etaLTM5 += pz_gen;
	  xL_LTM5Num++;
	}
      }

      if (count>=2 ){
	if (p_diss>0) {
	  if ( xL_p_diss < pz_gen ){
	    xL_p_diss= pz_gen;
	  }
	}
	if (p_diss<0) {
	  if ( xL_p_diss > pz_gen ){
	    xL_p_diss= pz_gen;
	  }
	}
      }

      if ( fabs(eta_gen)>5.2 && fabs(eta_gen)<6.6 ){
	if (debug) std::cout<<"Particle in Castor, having eta "<<eta_gen<<" and energy "<< ener_gen<<std::endl;
	if (eta_gen<0) sumECastor_minus_gen += ener_gen;
	if (eta_gen>0) sumECastor_plus_gen += ener_gen;
      }

      if ( fabs(eta_gen)>8.2  && ( pdg == 2112 || pdg == 22) ){
	if (debug) std::cout<<"Particle in ZDC, having eta "<<eta_gen<<" and energy "<< ener_gen<<std::endl;
	if (eta_gen<0) sumEZDC_minus_gen += ener_gen;
	if (eta_gen>0) sumEZDC_plus_gen += ener_gen;
      }      
      Nstable_gen++;

    }  // status =1
    count++;

  } // loop over particles

  //// Computing GAPs
  const int  size = (int) eta_gen_vec.size();
  int *sortedgen=   new int[size];
  double *vgen = new double[size];
  double eta_gap_limplus_gen = -10.0;
  double eta_gap_limminus_gen = -10.0;

  for (int i=0; i<size; i++) {
    vgen[i] = eta_gen_vec[i];
    if (debug) std::cout<<vgen[i]<<std::endl;
  }
  TMath::Sort(size, vgen, sortedgen, true);

  if (size > 1) {
    double *diff = new double[size-1];
    int *diffsorted = new int[size-1];
    for (int i=0; i<(size-1); i++) {
      diff[i] = fabs(eta_gen_vec[sortedgen[i+1]]-eta_gen_vec[sortedgen[i]]);
      if (debug) {
	std::cout<<" eta " << i << " size " << size << " diff "<< diff[i]<< std::endl;
	std::cout<<" GEN etas "  << " = " << eta_gen_vec[sortedgen[i+1]] << " - " <<  eta_gen_vec[sortedgen[i]] <<  " GAP diff "<< diff[i] << std::endl;
	std::cout<<" GEN etas "  << " = " << eta_gen_vec[sortedgen[i]] << std::endl;
      }
    }

    TMath::Sort(size-1, diff, diffsorted, true);

    //checking the max gap
    double max_eta_gap_gen=diff[diffsorted[0]];
    eta_gap_limminus_gen = eta_gen_vec[sortedgen[diffsorted[0]+1]] ;
    eta_gap_limplus_gen = eta_gen_vec[sortedgen[diffsorted[0]]] ;

    if (debug) std::cout << "GEN eta ranges " <<  eta_gap_limplus_gen  << " " <<  eta_gap_limminus_gen  << std::endl;
    eventData.SetPrimaryGapMaxGen(max_eta_gap_gen);

    if (size>2) {
      double max_second_eta_gap_gen=diff[diffsorted[1]];
      eventData.SetSecondGapMaxGen(max_second_eta_gap_gen);
      if (debug) std::cout<<" diff  " << diff[diffsorted[0]] << " sec " << diff[diffsorted[1]] << " diff size "<< diff[size-2] << std::endl;
    }

    delete [] diff;
    delete [] diffsorted;
  }

  delete [] sortedgen;
  delete [] vgen;

  math::XYZTLorentzVector dataMassG_plus(0.,0.,0.,0.);
  math::XYZTLorentzVector dataMassG_minus(0.,0.,0.,0.);
  int nplusG =0;
  int nminusG =0;
  int numseltracks =0;

  for(size_t i = 0; i < genParticles->size(); ++ i) {
    const reco::GenParticle & p = (*genParticles)[i];
    int status = p.status();  	 
    double eta_gen = p.eta();
    int charge = p.charge();
    double pt = p.pt();

    if( status == 1 && p.energy() > 1 ) {

      math::XYZTLorentzVector tmp( p.px(),p.py(),p.pz(),p.energy());

      if ( eta_gen >= eta_gap_limplus_gen ) {
	dataMassG_plus+=tmp;
	nplusG++;
      }
      else {
	dataMassG_minus+=tmp;
	nminusG++;
      }
    }

    if ( status == 1 ) {
      if ( charge && fabs(eta_gen)<2.6 &&  pt >= 0.1 ) {  // !!  condition for xsec per 3 charged prompt particles
	numseltracks++;
	genpt.push_back(pt);
	eta.push_back(eta_gen);
      }
    }
  } // end of genparticle loop

  double Mx2_gen=partVis.M2(); /// massaquadro visibile generata
  math::XYZTLorentzVector NOZ=partVis-partZ;
  double Mx2_NOZ_gen=NOZ.M2();
  if (debug) {
    std::cout << "Mx2_gen is "<< Mx2_gen<<" while eta of the outcoming proton is "<< etaOutcomingProton <<" and the energy "<< energyOutcomingProton << std::endl;
  }

  mostEnergeticXL = xL_etaGTP5/3500.;
  mostEnergeticXLNum = xL_GTP5Num ;
  if (fabs(xL_etaGTP5)<fabs(xL_etaLTM5)) 
  {
    mostEnergeticXL = xL_etaLTM5/3500.;
    mostEnergeticXLNum = xL_LTM5Num ;
  }

  if (debug) std::cout << "* XLgen " << mostEnergeticXL << " num " << mostEnergeticXLNum << " + " << xL_etaGTP5 << " - " << xL_etaLTM5 <<  std::endl;

  const int  size2 = (int) genpt.size();
  int *sorted = new int[size2];
  double *vv = new double[size2];
  for (int i=0; i<size2; i++) {
    vv[i] = genpt[i];
  }
  TMath::Sort(size2, vv, sorted, true);
  for (int i=0; i<size2; i++) {
    tracks.push_back(genpt[sorted[i]]);
    etaPT.push_back(eta[sorted[i]]);
    if (i>30) break;
  }  //  comes out size of 32!

  eventData.SetTracksPtGen(tracks);
  eventData.SetEtaOfTracksPtGen(etaPT);
  eventData.SetNTracksGen(tracks.size());

  genpt.clear();
  eta.clear();

  delete [] sorted;
  delete [] vv;

  eventData.SetMx2PlusGen(dataMassG_plus.M2());
  eventData.SetMx2MinusGen(dataMassG_minus.M2());
  eventData.SetMx2Gen(Mx2_NOZ_gen);
  eventData.SetMx2ZGen(Mx2_gen);
  eventData.SetNMx2PlusGen(nplusG);
  eventData.SetNMx2MinusGen(nminusG);
  eventData.SetEtaGaplimPlusGen(eta_gap_limplus_gen);
  eventData.SetEtaGaplimMinusGen(eta_gap_limminus_gen);
  eventData.SetNParticlesGen(Nstable_gen);
  eventData.SetsumECastorMinusGen(sumECastor_minus_gen);
  eventData.SetsumECastorPlusGen(sumECastor_plus_gen);
  eventData.SetsumEZDCMinusGen(sumEZDC_minus_gen);
  eventData.SetsumEZDCPlusGen(sumEZDC_plus_gen);
  eventData.SetEtaOutcomingProtonGen(etaOutcomingProton);
  eventData.SetxLGen(mostEnergeticXL);
  eventData.SetxLMostEnergeticGen(mostEnergeticXLNum);
  eventData.SetxiZMinusGen(xi_Z_gen_minus);
  eventData.SetxiZPlusGen(xi_Z_gen_plus);
  eventData.SetEtaZGen(etaZ_gen);
  eventData.SetEnergyZGen(energyZ_gen);
  eventData.SetpDissMassGen(p_diss_mass);
  eventData.SetxLpDissMass(xL_p_diss);

}

//
// Fill Detector Variables
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillDetectorVariables(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  bool debug = false;

  double etaMax=-999;
  double etaMin=999;
  double Epz_plus=0;  
  double Epz_minus=0;  

  int nTowersHF_plus = 0;
  int nTowersHF_minus = 0;
  int nTowersHE_plus = 0;
  int nTowersHE_minus = 0;
  int nTowersHB_plus = 0;
  int nTowersHB_minus = 0;
  int nTowersEE_plus = 0;
  int nTowersEE_minus = 0;
  int nTowersEB_plus = 0;
  int nTowersEB_minus = 0;

  //Sum(E)
  double sumEHF_plus = 0.;
  double sumEHF_minus = 0.;
  double sumEHF_L_plus = 0.;
  double sumEHF_L_minus = 0.;
  double sumEHF_S_plus = 0.;
  double sumEHF_S_minus = 0.;
  double sumEHE_plus = 0.;
  double sumEHE_minus = 0.;
  double sumEHB_plus = 0.;
  double sumEHB_minus = 0.;
  double sumEEE_plus = 0.;
  double sumEEE_minus = 0.;
  double sumEEB_plus = 0.;
  double sumEEB_minus = 0.;

  // Sum(ET)
  double sumETHF_plus = 0.;
  double sumETHF_minus = 0.;
  double sumETHE_plus = 0.;
  double sumETHE_minus = 0.;
  double sumETHB_plus = 0.;
  double sumETHB_minus = 0.;
  double sumETEB_plus = 0.;
  double sumETEB_minus = 0.;
  double sumETEE_plus = 0.;
  double sumETEE_minus = 0.;
  double xi_Calo_minus =0;
  double xi_Calo_plus =0;

  edm::Handle<CaloTowerCollection> towerCollectionH;
  event.getByLabel(caloTowerTag_,towerCollectionH);
  const CaloTowerCollection& towerCollection = *towerCollectionH;

  CaloTowerCollection::const_iterator calotower;
  calotower = towerCollection.begin();
  CaloTowerCollection::const_iterator calotowers_end = towerCollection.end();

  for(; calotower != calotowers_end; ++calotower) {

    if (fabs(calotower->eta())> 4.7) continue;   /// excluding ring12 and ring13 of HF

    bool hasHCAL = false;
    bool hasHF = false;
    bool hasHE = false;
    bool hasHB = false;
    bool hasHO = false;
    bool hasECAL = false;
    bool hasEE = false;
    bool hasEB = false;     

    for(size_t iconst = 0; iconst < calotower->constituentsSize(); iconst++){
      DetId adetId = calotower->constituent(iconst);
      if(adetId.det()==DetId::Hcal){
	hasHCAL = true;
	HcalDetId hcalDetId(adetId);
	if(hcalDetId.subdet()==HcalForward) hasHF = true;
	else if(hcalDetId.subdet()==HcalEndcap) hasHE = true;
	else if(hcalDetId.subdet()==HcalBarrel) hasHB = true;
	else if(hcalDetId.subdet()==HcalOuter) hasHO = true;  
      } 
      else if(adetId.det()==DetId::Ecal){
	hasECAL = true;
	EcalSubdetector ecalSubDet = (EcalSubdetector)adetId.subdetId();
	if(ecalSubDet == EcalEndcap) hasEE = true;
	else if(ecalSubDet == EcalBarrel) hasEB = true;
      }
    }
    int zside = calotower->zside();
    double caloTowerEnergy = calotower->energy();
    double caloTowerEmEnergy = calotower->emEnergy();
    double caloTowerHadEnergy = calotower->hadEnergy();
    double caloTowerPz = calotower->pz();
    double caloTowerEt = calotower->et();
    double caloTowerEmEt = calotower->emEt();
    double caloTowerHadEt = calotower->hadEt();
    double EHF_S = 0;
    double EHF_L = 0;

    bool CalAboveTh = false;

    if( hasHF && !hasHE )
    {
      if( caloTowerEnergy > energyThresholdHF_ && fabs(calotower->eta())> 2.98 )   //// excluding HF ring1
      {
	CalAboveTh = true;

	if (debug) std::cout << "HF>> " <<  calotower->id() << " HAD energy "     << caloTowerHadEnergy << " EM energy " << caloTowerEmEnergy << " energy " << caloTowerEnergy << std::endl; 

	// Added Long and short fibers
	// emc=L-S
	// hac=2*S
	// Tot = L+S
	// S = hac/2
	// L = Tot - S

	EHF_S = caloTowerHadEnergy/2;
	EHF_L = caloTowerEnergy - caloTowerHadEnergy/2;

	if(zside >= 0)
	{
	  ++nTowersHF_plus;
	  sumEHF_plus += caloTowerEnergy;
	  sumEHF_S_plus += EHF_S;
	  sumEHF_L_plus += EHF_L;
	  sumETHF_plus += caloTowerEt;
	}
	else
	{
	  ++nTowersHF_minus;
	  sumEHF_minus += caloTowerEnergy;
	  sumEHF_S_minus += EHF_S;
	  sumEHF_L_minus += EHF_L;
	  sumETHF_minus += caloTowerEt;
	}
      }
    }
    else if( hasHE && !hasHF && !hasHB )
    {
      if( caloTowerHadEnergy > energyThresholdHE_)
      {
	CalAboveTh = true;

	if (debug) std::cout << "HE>> " <<  calotower->id() << "  HAD energy "     << caloTowerHadEnergy << " EM energy " << caloTowerEmEnergy << " energy " << caloTowerEnergy << std::endl;

	if(zside >= 0)
	{
	  ++nTowersHE_plus;
	  sumEHE_plus += caloTowerHadEnergy;
	  sumETHE_plus += caloTowerHadEt;
	}
	else
	{
	  ++nTowersHE_minus;
	  sumEHE_minus += caloTowerHadEnergy;
	  sumETHE_minus += caloTowerHadEt;
	}
      }
    }
    else if( hasHB && !hasHE )
    {
      if( caloTowerHadEnergy > energyThresholdHB_)
      {
	CalAboveTh = true;

	if (debug) std::cout << "HB>> " <<  calotower->id() << "  HAD energy "     << caloTowerHadEnergy << " EM energy " << caloTowerEmEnergy << " energy " << caloTowerEnergy << std::endl;

	if(zside >= 0)
	{
	  ++nTowersHB_plus;
	  sumEHB_plus += caloTowerHadEnergy;
	  sumETHB_plus += caloTowerHadEt;
	}
	else
	{
	  ++nTowersHB_minus;
	  sumEHB_minus += caloTowerHadEnergy;
	  sumETHB_minus += caloTowerHadEt;
	}
      }
    }

    if( hasEE && !hasEB )
    {
      if( caloTowerEmEnergy >= energyThresholdEE_)
      {
	CalAboveTh = true;

	if (debug) std::cout << "EE>> " <<  calotower->id() << "  HAD energy "     << caloTowerHadEnergy << " EM energy " << caloTowerEmEnergy << " energy " << caloTowerEnergy << std::endl;

	if(zside >= 0)
	{
	  ++nTowersEE_plus;
	  sumEEE_plus += caloTowerEmEnergy;
	  sumETEE_plus += caloTowerEmEt;
	}
	else
	{
	  ++nTowersEE_minus;
	  sumEEE_minus += caloTowerEmEnergy;
	  sumETEE_minus += caloTowerEmEt;
	}
      }
    }
    else if( hasEB && !hasEE )
    {
      if( caloTowerEmEnergy >= energyThresholdEB_)
      {
	CalAboveTh = true;

	if (debug) std::cout << "EB>> " <<  calotower->id() << " HAD energy "     << caloTowerHadEnergy << " EM energy " << caloTowerEmEnergy << " energy " << caloTowerEnergy << std::endl; 

	if(zside >= 0)
	{
	  ++nTowersEB_plus;
	  sumEEB_plus += caloTowerEmEnergy;
	  sumETEB_plus += caloTowerEmEt;
	}
	else
	{
	  ++nTowersEB_minus;
	  sumEEB_minus += caloTowerEmEnergy;
	  sumETEB_minus += caloTowerEmEt;
	}
      }
    }

    if(CalAboveTh)
    {
      if (calotower->eta() >= etaMax) etaMax=calotower->eta();
      if (calotower->eta() <= etaMin) etaMin=calotower->eta();
      xi_Calo_minus += caloTowerEt * pow(2.71,-calotower->eta()) / (7000);
      xi_Calo_plus += caloTowerEt * pow(2.71,calotower->eta()) / (7000);
      Epz_plus  += caloTowerEnergy + caloTowerPz;
      Epz_minus += caloTowerEnergy - caloTowerPz;
    }

  }  ////has to close calotower loop

  eventData.SetSumEHFPlus(sumEHF_plus);
  eventData.SetSumEHF_SPlus(sumEHF_S_plus);
  eventData.SetSumEHF_LPlus(sumEHF_L_plus);
  eventData.SetSumEtHFPlus(sumETHF_plus);

  eventData.SetSumEHFMinus(sumEHF_minus);
  eventData.SetSumEHF_SMinus(sumEHF_S_minus);
  eventData.SetSumEHF_LMinus(sumEHF_L_minus);
  eventData.SetSumEtHFMinus(sumETHF_minus);

  eventData.SetSumEHEPlus(sumEHE_plus);
  eventData.SetSumEtHEPlus(sumETHE_plus);
  eventData.SetSumEHEMinus(sumEHE_minus);
  eventData.SetSumEtHEMinus(sumETHE_minus);

  eventData.SetSumEHBPlus(sumEHB_plus);
  eventData.SetSumEtHBPlus(sumETHB_plus);
  eventData.SetSumEHBMinus(sumEHB_minus);
  eventData.SetSumEtHBMinus(sumETHB_minus);

  eventData.SetSumEEEPlus(sumEEE_plus);
  eventData.SetSumEtEEPlus(sumETEE_plus);
  eventData.SetSumEEEMinus(sumEEE_minus);
  eventData.SetSumEtEEMinus(sumETEE_minus);

  eventData.SetSumEEBPlus(sumEEB_plus);
  eventData.SetSumEtEBPlus(sumETEB_plus);
  eventData.SetSumEEBMinus(sumEEB_minus);
  eventData.SetSumEtEBMinus(sumETEB_minus);

  eventData.SetEPZCaloPlus(Epz_plus);
  eventData.SetEPZCaloMinus(Epz_minus);
  eventData.SetXiCaloPlus(xi_Calo_plus);
  eventData.SetXiCaloMinus(xi_Calo_minus);

  eventData.SetEtaCaloMax(etaMax);
  eventData.SetEtaCaloMin(etaMin);

  eventData.SetMultiplicityHFPlus(nTowersHF_plus);
  eventData.SetMultiplicityHEPlus(nTowersHE_plus);
  eventData.SetMultiplicityEEPlus(nTowersEE_plus);
  eventData.SetMultiplicityHFMinus(nTowersHF_minus);
  eventData.SetMultiplicityHEMinus(nTowersHE_minus);
  eventData.SetMultiplicityEEMinus(nTowersEE_minus); 

}

//
// Fill Physics Variables
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillVariables(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  bool debug=false;

  std::vector<double> etas;
  double etaTimesEnergy=0.;
  double Epz_PF_plus=0.;
  double Epz_PF_minus=0.;
  double xi_PF_minus=0.;
  double xi_PF_plus=0.;
  double sumpx=0.;
  double sumpy=0.;
  double sumpz=0.;
  double sumpxModule=0.;
  double sumpyModule=0.;
  double sumpzModule=0.;
  double sumEnergyPF=0.;

  double PtThPFCharged = 0.1;  // it was 0.15
  double EnThPFBar = 1.5;
  double EnThPFEnd = 3.5; // 4.;
  double EnThPFFw  = 6.0; // 6; 

  int nPart_PF=0;

  std::vector<double> electronEnergy;
  std::vector<double> muEnergy;

  edm::Handle<reco::VertexCollection> Vertexes;
  event.getByLabel(PVtxCollectionTag_, Vertexes); 

  edm::Handle <reco::PFCandidateCollection> PFCandidates;
  event.getByLabel(pfTag_,PFCandidates);
  reco::PFCandidateCollection::const_iterator iter;

  eventData.SetVertex(Vertexes->size());

  for (reco::PFCandidateCollection::const_iterator iter = PFCandidates->begin(); iter != PFCandidates->end(); ++iter) {

    const reco::PFCandidate *particle = &(*iter);
    double et=particle->et();
    double energy=particle->energy();
    double pt=particle->pt();
    double p=particle->p();
    double px=particle->px();
    double py=particle->py();
    double pz=particle->pz();
    double eta=particle->eta();
    double charge=particle->charge();
    double theta=particle->theta();

    // Fill 2D TTree (eta,energy);

    //eta cut - excluding ring 12 13 HF  
    if (fabs(eta)>4.7) continue;

    //int type=particle->particleId();

    if (particle->particleId()==reco::PFCandidate::e) electronEnergy.push_back(et);
    if (particle->particleId()==reco::PFCandidate::mu) muEnergy.push_back(et);

    TLorentzVector tmp(px,py,pz,energy);

    if  (  (fabs(charge) >0 && pt >  PtThPFCharged ) ||
	(fabs(charge) == 0  && ( (fabs(eta) <= 1.5 && energy > EnThPFBar)  ||
				 (fabs(eta) > 1.5 && fabs(eta) <= 3 && energy > EnThPFEnd) ||
				 (fabs(eta) > 3 && energy >EnThPFFw) ) )   )
    {        

      nPart_PF++;

      Epz_PF_plus+=p+p*TMath::Cos(theta);
      Epz_PF_minus+=p-p*TMath::Cos(theta);
      xi_PF_minus += et * pow(2.71,-eta) / (7000);
      xi_PF_plus += et * pow(2.71,eta) / (7000);

      etaTimesEnergy+=eta*energy;
      sumpxModule +=fabs(px);
      sumpyModule +=fabs(py);
      sumpzModule +=fabs(pz);
      sumpx +=px;
      sumpy +=py;
      sumpz +=pz;
      sumEnergyPF +=energy;
      etas.push_back(eta);

    } 

  }

  eventData.SetXi_PF_minus(xi_PF_minus);
  eventData.SetXi_PF_plus(xi_PF_plus);
  eventData.SetEpz_PF_minus(Epz_PF_minus);
  eventData.SetEpz_PF_plus(Epz_PF_plus);
  eventData.SetMultiplicityPF(nPart_PF);
  eventData.SetSumEtaTimesEnergyPF(etaTimesEnergy);
  eventData.SetSumpxModulePF(sumpxModule);
  eventData.SetSumpyModulePF(sumpyModule);
  eventData.SetSumpzModulePF(sumpzModule);
  eventData.SetSumpxPF(sumpx);
  eventData.SetSumpyPF(sumpy);
  eventData.SetSumpzPF(sumpz);
  eventData.SetSumEnergyPF(sumEnergyPF);

  //// Computing GAPs
  //// adding two fake entries at +-4.9 in etas!!!
  etas.push_back(4.9);
  etas.push_back(-4.9);

  const int  size = (int) etas.size();
  int *sorted = new int[size];
  double *v = new double[size];
  double eta_gap_limplus = -10.0;
  double eta_gap_limminus = -10.0;

  for (int i=0; i<size; i++) {
    v[i] = etas[i];
    if (debug) std::cout<<v[i]<<std::endl;
  }
  TMath::Sort(size, v, sorted, true);

  if (size > 1) {
    double *diff = new double[size-1];
    int *diffsorted = new int[size-1];
    for (int i=0; i<(size-1); i++) {
      diff[i] = fabs(etas[sorted[i+1]]-etas[sorted[i]]);
    }

    TMath::Sort(size-1, diff, diffsorted, true);

    //checking the max gap
    double max_eta_gap=diff[diffsorted[0]];
    eta_gap_limminus = etas[sorted[diffsorted[0]+1]] ;
    eta_gap_limplus = etas[sorted[diffsorted[0]]] ;

    eventData.SetMaxGapPF(max_eta_gap);
    eventData.SetLimPlusGapPF(eta_gap_limplus);
    eventData.SetLimMinusGapPF(eta_gap_limminus);

    if (size>2) {
      double max_second_eta_gap=diff[diffsorted[1]];
      if (debug) std::cout<<" diff  " << diff[diffsorted[0]] << " sec " << diff[diffsorted[1]] << " diff size "<< diff[size-2] <<std::endl;
      eventData.SetSecondMaxGapPF(max_second_eta_gap);
    }

    else {
      eventData.SetSecondMaxGapPF(-999.);
    }

    delete [] diff;
    delete [] diffsorted;

  }

  else {

    eventData.SetMaxGapPF(-999.);
    eventData.SetSecondMaxGapPF(-999.);
    eventData.SetLimPlusGapPF(-999.);
    eventData.SetLimMinusGapPF(-999.);

  }

  delete [] sorted;
  delete [] v;

  //sorting electron energy
  const int  size3 = (int) electronEnergy.size();
  int *sorted3 = new int[size3];
  double *v3 = new double[size3];

  for (int i=0; i<size3; i++) {
    v3[i] = electronEnergy[i];
  }
  TMath::Sort(size3, v3, sorted3, true);
  for (int i=0; i<size3; i++) {
    electronEnergy[i] = v3[sorted3[i]];
  }

  //sorting muon energy
  const int  size4 = (int) muEnergy.size();
  int *sorted4 = new int[size4];
  double *v4 = new double[size4];

  for (int i=0; i<size4; i++) {
    v4[i] = muEnergy[i];
  }
  TMath::Sort(size4, v4, sorted4, true);
  for (int i=0; i<size4; i++) {
    muEnergy[i] = v4[sorted4[i]];
  }
  delete [] sorted3;
  delete [] v3;
  delete [] sorted4;
  delete [] v4;

  /// Loop to compute Mx2 a destra e a sinistra del GAP
  TLorentzVector dataMass_plus(0.,0.,0.,0.);
  TLorentzVector dataMass_minus(0.,0.,0.,0.);
  int nplus =0;
  int nminus =0;

  for (reco::PFCandidateCollection::const_iterator iter = PFCandidates->begin(); iter != PFCandidates->end(); ++iter) {
    const reco::PFCandidate *particle = &(*iter);
    double energy=particle->energy();
    double pt=particle->pt();
    double px=particle->px();
    double py=particle->py();
    double pz=particle->pz();
    double eta=particle->eta();
    double charge=particle->charge();

    //eta cut - excluding ring 12 13 HF  
    if (fabs(eta)>4.7) continue;

    TLorentzVector tmp(px,py,pz,energy); 

    if  (  (fabs(charge) >0 && pt >  PtThPFCharged ) ||
	(fabs(charge) == 0  && ( (fabs(eta) <= 1.5 && energy > EnThPFBar)  ||
				 (fabs(eta) > 1.5 && fabs(eta) <= 3 && energy > EnThPFEnd) ||
				 (fabs(eta) > 3 && energy >EnThPFFw) ) )   )
    {        

      if ( eta >= eta_gap_limplus ) {
	dataMass_plus+=tmp;
	nplus++;
      }
      else {
	dataMass_minus+=tmp;
	nminus++;
      }
    }
  }  // PF loop

  eventData.SetElectronEnergyPF(-999.); // First Electron, Fill Second Electron also. Eta, phi, pT and ISO from PF.
  eventData.SetMuEnergyPF(-999.); // First Muon, Fill Second Muon also. Eta, phi, pT and ISO from PF.
  eventData.SetMultiplicityGapPlusPF(nplus);
  eventData.SetMultiplicityGapMinusPF(nminus);

}

//
// Fill Pat:Muon and Pat:Electron objects 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillZPat(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  // Declaring Variables

  bool debug = false;
  int ElectronsN=0;
  int MuonsN=0;

  // Detector Objects and Candidates
  edm::Handle<std::vector<pat::Muon> > muons;
  event.getByLabel("patMuons", muons);

  edm::Handle<std::vector<pat::Electron> > electrons;
  event.getByLabel("patElectrons", electrons);

  if (debug) {

    if (muons->size() > 1) {
      std::cout << "More than 2 Muons!" << std::endl; 
    }

    if (electrons->size() > 1) {    
      std::cout << "More than 2 Electrons!" << std::endl;
    }

  }

  // Read information of Muon Candidate

  const pat::Muon* muon1=NULL;
  const pat::Muon* muon2=NULL;

  int muonsize = muons->size();
  int itMuon;

  if(muons->size()>1){

    for(itMuon=0; itMuon < muonsize; ++itMuon){

      ++MuonsN;

      const pat::Muon* muonAll = &((*muons)[itMuon]);

      if (muonAll==NULL) continue;

      if (muon1==NULL) {muon1=muonAll; continue;}
      if (muonAll->pt()>muon1->pt()) {
	muon2=muon1;
	muon1=muonAll;
	continue;
      }

      if (muon2==NULL) {muon2=muonAll; continue;}
      if (muonAll->pt()>muon2->pt()) muon2 = muonAll;

    }

    double muon1SumPtR03 = muon1->isolationR03().sumPt;
    double muon1EmEtR03 = muon1->isolationR03().emEt;
    double muon1HadEtR03 = muon1->isolationR03().hadEt;    
    double muon1SumPtR05 = muon1->isolationR05().sumPt;
    double muon1EmEtR05 = muon1->isolationR05().emEt;
    double muon1HadEtR05 = muon1->isolationR05().hadEt;    

    double muon2SumPtR03 = muon2->isolationR03().sumPt;
    double muon2EmEtR03 = muon2->isolationR03().emEt;
    double muon2HadEtR03 = muon2->isolationR03().hadEt;
    double muon2SumPtR05 = muon2->isolationR05().sumPt;
    double muon2EmEtR05 = muon2->isolationR05().emEt;
    double muon2HadEtR05 = muon2->isolationR05().hadEt;

    double relIsoFirstMuonDr03 = (muon1SumPtR03 + muon1EmEtR03 + muon1HadEtR03)/muon1->pt();
    double relIsoSecondMuonDr03 = (muon2SumPtR03 + muon2EmEtR03 + muon2HadEtR03)/muon2->pt();
    double relIsoFirstMuonDr05 = (muon1SumPtR05 + muon1EmEtR05 + muon1HadEtR05)/muon1->pt();
    double relIsoSecondMuonDr05 = (muon2SumPtR05 + muon2EmEtR05 + muon2HadEtR05)/muon2->pt();

    double relIsoFirstMuon = (muon1->trackIso()+muon1->ecalIso()+muon1->hcalIso())/muon1->pt();
    double relIsoSecondMuon = (muon2->trackIso()+muon2->ecalIso()+muon2->hcalIso())/muon2->pt();

    // DiMuon Mass
    math::XYZTLorentzVector DipatMuonSystem(0.,0.,0.,0.);
    DipatMuonSystem += muon1->p4();
    DipatMuonSystem += muon2->p4();

    eventData.SetPatDiMuonMass(DipatMuonSystem.M());
    eventData.SetPatDiMuonEta(DipatMuonSystem.eta());
    eventData.SetPatDiMuonPhi(DipatMuonSystem.phi());
    eventData.SetPatDiMuonPt(DipatMuonSystem.pt());

    eventData.SetPatNMuon(muons->size());
    eventData.SetPatMuon1Pt(muon1->pt());
    eventData.SetPatMuon1Charge(muon1->charge());
    eventData.SetPatMuon1Phi(muon1->phi());
    eventData.SetPatMuon1Eta(muon1->eta());
    eventData.SetPatMuon1Et(muon1->et());

    eventData.SetPatMuon2Pt(muon2->pt());
    eventData.SetPatMuon2Charge(muon2->charge());
    eventData.SetPatMuon2Phi(muon2->phi());
    eventData.SetPatMuon2Eta(muon2->eta());
    eventData.SetPatMuon2Et(muon2->et());

    eventData.SetPatMuon1SumPtR03(muon1SumPtR03);
    eventData.SetPatMuon1EmEtR03(muon1EmEtR03);
    eventData.SetPatMuon1HadEtR03(muon1HadEtR03);    
    eventData.SetPatMuon1SumPtR05(muon1SumPtR05);
    eventData.SetPatMuon1EmEtR05(muon1EmEtR05);
    eventData.SetPatMuon1HadEtR05(muon1HadEtR05);    

    eventData.SetPatMuon2SumPtR03(muon2SumPtR03);
    eventData.SetPatMuon2EmEtR03(muon2EmEtR03);
    eventData.SetPatMuon2HadEtR03(muon2HadEtR03);    
    eventData.SetPatMuon2SumPtR05(muon2SumPtR05);
    eventData.SetPatMuon2EmEtR05(muon2EmEtR05);
    eventData.SetPatMuon2HadEtR05(muon2HadEtR05);  

    eventData.SetPatMuon1relIsoDr03(relIsoFirstMuonDr03);
    eventData.SetPatMuon2relIsoDr03(relIsoSecondMuonDr03);
    eventData.SetPatMuon1relIsoDr05(relIsoFirstMuonDr05);
    eventData.SetPatMuon2relIsoDr05(relIsoSecondMuonDr05);

    eventData.SetPatMuon1relIso(relIsoFirstMuon);
    eventData.SetPatMuon2relIso(relIsoSecondMuon);

    if (debug){
      std::cout<<"Muon1 -> 0.3 Radion Rel Iso: "<<relIsoFirstMuonDr03<<" sumPt "<<muon1SumPtR03<<" emEt "<<muon1EmEtR03<<" hadEt "<<muon1HadEtR03<<std::endl;
      std::cout<<"Muon1 -> 0.5 Radion Rel Iso: "<<relIsoFirstMuonDr05<<" sumPt "<<muon1SumPtR05<<" emEt "<<muon1EmEtR05<<" hadEt "<<muon1HadEtR05<<std::endl;
      std::cout << "Muon1 -> trackIso(): " << muon1->trackIso() << " | muon1 -> ecalIso(): " << muon1->ecalIso() << " | muon1 -> hcalIso(): " << muon1->hcalIso() << " | muon1->Iso(): " << relIsoFirstMuon << std::endl; 
      std::cout<<"Muon2 -> 0.3 Radion Rel Iso: "<<relIsoSecondMuonDr03<<" sumPt "<<muon2SumPtR03<<" emEt "<<muon2EmEtR03<<" hadEt "<<muon2HadEtR03<<std::endl;
      std::cout<<"Muon2 -> 0.5 Radion Rel Iso: "<<relIsoSecondMuonDr05<<" sumPt "<<muon2SumPtR05<<" emEt "<<muon2EmEtR05<<" hadEt "<<muon2HadEtR05<<std::endl;
      std::cout << "Muon2 -> trackIso(): " << muon2->trackIso() << " | muon2 -> ecalIso(): " << muon2->ecalIso() << " | muon2 -> hcalIso(): " << muon2->hcalIso() << " | muon2->Iso(): " << relIsoSecondMuon << std::endl;  
      std::cout << "NSize: " << muons->size() << std::endl;
      std::cout << "Muon1: " << muon1->pt() << std::endl;
      std::cout << "Muon2: " << muon2->pt() << std::endl;
      std::cout << "Muon1, p4(): " << muon1->p4() << std::endl;
      std::cout << "Muon2, p4(): " << muon2->p4() << std::endl;
      std::cout << "DiMuon, M(): " << DipatMuonSystem.M() << std::endl;
      std::cout << "Eta Z: " << DipatMuonSystem.eta() << std::endl;
      std::cout << "Phi Z: " << DipatMuonSystem.phi() << std::endl;
      std::cout << "pT Z: " << DipatMuonSystem.pt() << std::endl;
    }

  }

  else {

    eventData.SetPatMuon1Pt(-999.);
    eventData.SetPatMuon1Charge(-999);
    eventData.SetPatMuon1Phi(-999.);
    eventData.SetPatMuon1Eta(-999.);
    eventData.SetPatMuon1Et(-999.);

    eventData.SetPatMuon2Pt(-999.);
    eventData.SetPatMuon2Charge(-999);
    eventData.SetPatMuon2Phi(-999.);
    eventData.SetPatMuon2Eta(-999.);
    eventData.SetPatMuon2Et(-999.);

    eventData.SetPatMuon1SumPtR03(-999.);
    eventData.SetPatMuon1EmEtR03(-999.);
    eventData.SetPatMuon1HadEtR03(-999.);
    eventData.SetPatMuon1SumPtR05(-999.);
    eventData.SetPatMuon1EmEtR05(-999.);
    eventData.SetPatMuon1HadEtR05(-999.);

    eventData.SetPatMuon2SumPtR03(-999.);
    eventData.SetPatMuon2EmEtR03(-999.);
    eventData.SetPatMuon2HadEtR03(-999.);
    eventData.SetPatMuon2SumPtR05(-999.);
    eventData.SetPatMuon2EmEtR05(-999.);
    eventData.SetPatMuon2HadEtR05(-999.);

    eventData.SetPatMuon1relIsoDr03(-999.);
    eventData.SetPatMuon2relIsoDr03(-999.);
    eventData.SetPatMuon1relIsoDr05(-999.);
    eventData.SetPatMuon2relIsoDr05(-999.);

    eventData.SetPatMuon1relIso(-999.);
    eventData.SetPatMuon2relIso(-999.);

    eventData.SetPatDiMuonMass(-999.);
    eventData.SetPatDiMuonPt(-999.);
    eventData.SetPatDiMuonEta(-999.);
    eventData.SetPatDiMuonPhi(-999.);

  } 

  // Read Information of Electron Candidate 

  const pat::Electron* electron1=NULL;
  const pat::Electron* electron2=NULL;

  int electronsize = electrons->size();
  int itElectron;

  if(electrons->size()>1){

    for(itElectron=0; itElectron < electronsize; ++itElectron){

      ++ElectronsN;

      const pat::Electron* electronAll = &((*electrons)[itElectron]);

      if (electronAll==NULL) continue;
      if (electron1==NULL) {electron1=electronAll; continue;}
      if (electronAll->pt()>electron1->pt()) {
	electron2=electron1;
	electron1=electronAll;
	continue;
      }

      if (electron2==NULL) {electron2=electronAll; continue;}
      if (electronAll->pt()>electron2->pt()) electron2 = electronAll;

    }

    double relIsoFirstElectronDr03 = (electron1->dr03TkSumPt()+electron1->dr03EcalRecHitSumEt()+electron1->dr03HcalTowerSumEt())/electron1->et();
    double relIsoFirstElectronDr04 = (electron1->dr04TkSumPt()+electron1->dr04EcalRecHitSumEt()+electron1->dr04HcalTowerSumEt())/electron1->et();
    double relIsoSecondElectronDr03 = (electron2->dr03TkSumPt()+electron2->dr03EcalRecHitSumEt()+electron2->dr03HcalTowerSumEt())/electron2->et();
    double relIsoSecondElectronDr04 = (electron2->dr04TkSumPt()+electron2->dr04EcalRecHitSumEt()+electron2->dr04HcalTowerSumEt())/electron2->et();

    // Dielectron Mass
    math::XYZTLorentzVector DipatElectronSystem(0.,0.,0.,0.);
    DipatElectronSystem += electron1->p4();
    DipatElectronSystem += electron2->p4();

    eventData.SetPatDiElectronMass(DipatElectronSystem.M());
    eventData.SetPatDiElectronEta(DipatElectronSystem.eta());
    eventData.SetPatDiElectronPhi(DipatElectronSystem.phi());
    eventData.SetPatDiElectronPt(DipatElectronSystem.pt());

    // Fill Electron Variables
    eventData.SetPatNElectron(electrons->size());
    eventData.SetPatElectron1Pt(electron1->pt());
    eventData.SetPatElectron1Charge(electron1->charge());
    eventData.SetPatElectron1Phi(electron1->phi());
    eventData.SetPatElectron1Eta(electron1->eta());
    eventData.SetPatElectron1Et(electron1->et());

    eventData.SetPatElectron2Pt(electron2->pt());
    eventData.SetPatElectron2Charge(electron2->charge());
    eventData.SetPatElectron2Phi(electron2->phi());
    eventData.SetPatElectron2Eta(electron2->eta());
    eventData.SetPatElectron2Et(electron2->et());

    eventData.SetPatElectron1TkDr03(electron1->dr03TkSumPt());
    eventData.SetPatElectron1EcalDr03(electron1->dr03EcalRecHitSumEt());
    eventData.SetPatElectron1HcalDr03(electron1->dr03HcalTowerSumEt());

    eventData.SetPatElectron2TkDr03(electron2->dr03TkSumPt());
    eventData.SetPatElectron2EcalDr03(electron2->dr03EcalRecHitSumEt());
    eventData.SetPatElectron2HcalDr03(electron2->dr03HcalTowerSumEt());

    eventData.SetPatElectron1TkDr04(electron1->dr04TkSumPt());
    eventData.SetPatElectron1EcalDr04(electron1->dr04EcalRecHitSumEt());
    eventData.SetPatElectron1HcalDr04(electron1->dr04HcalTowerSumEt());

    eventData.SetPatElectron2TkDr04(electron2->dr04TkSumPt());
    eventData.SetPatElectron2EcalDr04(electron2->dr04EcalRecHitSumEt());
    eventData.SetPatElectron2HcalDr04(electron2->dr04HcalTowerSumEt());

    eventData.SetPatElectron1relIsoDr03(relIsoFirstElectronDr03);
    eventData.SetPatElectron1relIsoDr04(relIsoFirstElectronDr04);
    eventData.SetPatElectron2relIsoDr03(relIsoSecondElectronDr03);
    eventData.SetPatElectron2relIsoDr04(relIsoSecondElectronDr04);

    if (debug) {
      std::cout << "electron1 -> dr03 TK: " << electron1->dr03TkSumPt() << "| dr03 Ecal: " << electron1->dr03EcalRecHitSumEt() << " | dr03 Hcal: " << electron1->dr03HcalTowerSumEt() << std::endl;
      std::cout << "electron1 -> dr04 TK: " << electron1->dr04TkSumPt() << "| dr04 Ecal: " << electron1->dr04EcalRecHitSumEt() << " | dr04 Hcal: " << electron1->dr04HcalTowerSumEt() <<  std::endl;
      std::cout << "electron2 -> dr03 TK: " << electron2->dr03TkSumPt() << "| dr03 Ecal: " << electron2->dr03EcalRecHitSumEt() << " | dr03 Hcal: " << electron2->dr03HcalTowerSumEt() << std::endl;
      std::cout << "electron2 -> dr04 TK: " << electron2->dr04TkSumPt() << "| dr04 Ecal: " << electron2->dr04EcalRecHitSumEt() << " | dr04 Hcal: " << electron2->dr04HcalTowerSumEt() <<  std::endl;
      std::cout << "NElectron: " << ElectronsN << std::endl;
      std::cout << "NSize: " << electrons->size() << std::endl;
      std::cout << "Electron1: "<< electron1->pt() << std::endl;
      std::cout << "Electron2: " << electron2->pt() << std::endl;
      std::cout << "Electron1, p4(): " << electron1->p4() << std::endl;
      std::cout << "Electron2, p4(): " << electron2->p4() << std::endl;
      std::cout << "DiElectron, M(): " << DipatElectronSystem.M() << std::endl;
      std::cout << "Eta Z: " << DipatElectronSystem.eta() << std::endl;
      std::cout << "Phi Z: " << DipatElectronSystem.phi() << std::endl;
      std::cout << "pT Z: " << DipatElectronSystem.pt() << std::endl;
    }

  }
  else{
    eventData.SetPatElectron1Pt(-999.);
    eventData.SetPatElectron1Charge(-999);
    eventData.SetPatElectron1Phi(-999.);
    eventData.SetPatElectron1Eta(-999.);
    eventData.SetPatElectron1Et(-999.);

    eventData.SetPatElectron2Pt(-999.);
    eventData.SetPatElectron2Charge(-999);
    eventData.SetPatElectron2Phi(-999.);
    eventData.SetPatElectron2Eta(-999.);
    eventData.SetPatElectron2Et(-999.);

    eventData.SetPatElectron1TkDr03(-999.);
    eventData.SetPatElectron1EcalDr03(-999.);
    eventData.SetPatElectron1HcalDr03(-999.);

    eventData.SetPatElectron2TkDr03(-999.);
    eventData.SetPatElectron2EcalDr03(-999.);
    eventData.SetPatElectron2HcalDr03(-999.);

    eventData.SetPatElectron1TkDr04(-999.);
    eventData.SetPatElectron1EcalDr04(-999.);
    eventData.SetPatElectron1HcalDr04(-999.);

    eventData.SetPatElectron2TkDr04(-999.);
    eventData.SetPatElectron2EcalDr04(-999.);
    eventData.SetPatElectron2HcalDr04(-999.);

    eventData.SetPatElectron1relIsoDr03(-999.);
    eventData.SetPatElectron1relIsoDr04(-999.);
    eventData.SetPatElectron2relIsoDr03(-999.);
    eventData.SetPatElectron2relIsoDr04(-999.);

    eventData.SetPatDiElectronMass(-999.);
    eventData.SetPatDiElectronEta(-999.);
    eventData.SetPatDiElectronPhi(-999.);
    eventData.SetPatDiElectronPt(-999.);

  }


}

//
// Fill Castor 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillCastor(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){



  // Phi: 16 modules, rh.id().sector(); 
  // Z: 14 modules, rh.id().module(); 
  // Channel definition: 16*(rh.id().module()-1) + rh.id().sector(); 
  // For 2010, Castor uses only first five modules. 

  bool debug = false;
  bool debug_deep = false;
  std::vector<double> castor_tower;

  edm::Handle<CastorRecHitCollection> CastorRecHits;
  event.getByLabel(castorHitsTag_,CastorRecHits); 

  double sumCastorTower[16];
  bool accept[16];

  for(int isec = 0; isec < 16; isec++) {
    accept[isec] = false;
    sumCastorTower[isec] = 0; 
  }

  for (size_t i = 0; i < CastorRecHits->size(); ++i) {

    bool used_cha = false;
    const CastorRecHit & rh = (*CastorRecHits)[i];
    int cha = 16*(rh.id().module()-1) + rh.id().sector();    

    if(cha != 5 && cha != 6) used_cha = true;  
    if(used_cha == false) continue; 

    // Only 5th modules
    if (rh.id().module() > 5 ) continue;

    if (debug_deep) std::cout << "Energy: " << rh.energy()*fCGeVCastor_ << " | Sector: " << rh.id().sector() << " | Module: " << rh.id().module() << " | Channel: " << cha << std::endl;

    for(int isec = 0; isec < 16; isec++) {
      if (rh.id().sector()== isec+1) sumCastorTower[isec]+=rh.energy()*fCGeVCastor_;
    }

  }

  for (int isec = 0; isec < 16;isec++) {
    // 4 sigma for threshold.
    if (sumCastorTower[isec] > 4.*castorThreshold_) accept[isec]=true;
    if (accept[isec]==true) {
      castor_tower.push_back(sumCastorTower[isec]);
    }
    else castor_tower.push_back(-999.);
  }

  if (debug){
    for (int isec=0;isec<16;isec++){
      if(accept[isec]) std::cout << "Sector "<< isec+1 << ", Total Energy [GeV]: " << sumCastorTower[isec] << std::endl;
    }
  }

  eventData.SetCastorTowerEnergy(castor_tower);

}

//
// Fill ZDC
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillZDC(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  // ZDC have two sections: section 1 = EM, section 2 = HAD. EM has 5 modules. Had has 4 modules. 

  bool debug = false;
  bool debug_deep = false;

  double ZDCNSumEMEnergy = 0.;
  double ZDCNSumHADEnergy = 0.;
  double ZDCPSumEMEnergy = 0.;
  double ZDCPSumHADEnergy = 0.;

  double ZDCNSumEMTime = 0.;
  double ZDCNSumHADTime = 0.;
  double ZDCPSumEMTime = 0.;
  double ZDCPSumHADTime = 0.;

  int DigiDataADC[180];
  float DigiDatafC[180];

  edm::Handle <ZDCDigiCollection> zdc_digi_h;
  event.getByType(zdc_digi_h);
  edm::ESHandle<HcalDbService> conditions;
  const ZDCDigiCollection *zdc_digi = zdc_digi_h.failedToGet()? 0 : &*zdc_digi_h;

  edm::Handle <ZDCRecHitCollection> zdc_recHits_h;
  event.getByLabel(zdcHitsTag_, zdc_recHits_h);
  const ZDCRecHitCollection *zdc_recHits = zdc_recHits_h.failedToGet()? 0 : &*zdc_recHits_h;

  setup.get<HcalDbRecord>().get(conditions);

  if (zdc_recHits) {
    for (ZDCRecHitCollection::const_iterator zhit = zdc_recHits->begin(); zhit != zdc_recHits->end(); zhit++){		

      // Some Variables
      int ZDCSide      = (zhit->id()).zside();
      int ZDCSection   = (zhit->id()).section();
      //Float_t ZDCEnergy = zhit->energy();
      //Float_t ZDCRecHitTime = zhit->time();
      //int ZDCChannel   = (zhit->id()).channel();

      if (zhit->energy() >= 0.){

	if (ZDCSide == -1){

	  if (ZDCSection == 1 ){
	    ZDCNSumEMEnergy += zhit->energy();
	    ZDCNSumEMTime += zhit->time();
	  }

	  if (ZDCSection == 2 ){
	    ZDCNSumHADEnergy += zhit->energy();
	    ZDCNSumHADTime += zhit->time();
	  }

	}

	if (ZDCSide == 1){

	  if (ZDCSection == 1 ){
	    ZDCPSumEMEnergy += zhit->energy();
	    ZDCPSumEMTime += zhit->time();
	  }

	  if (ZDCSection == 2 ){
	    ZDCPSumHADEnergy += zhit->energy();
	    ZDCPSumHADTime += zhit->time();
	  }

	}

      }

    }

    if (debug){
      std::cout << "ZDC + | Total EM Energy: " << ZDCPSumEMEnergy << std::endl;
      std::cout << "ZDC + | Total HAD Energy: " << ZDCPSumHADEnergy << std::endl;
      std::cout << "ZDC + | EM <Time>: " << ZDCPSumEMTime/5. << std::endl;
      std::cout << "ZDC + | HAD <Time>: " << ZDCPSumHADTime/4. << std::endl;
      std::cout << "ZDC - | Total EM Energy: " << ZDCNSumEMEnergy << std::endl;
      std::cout << "ZDC - | Total HAD Energy: " << ZDCNSumHADEnergy << std::endl;
      std::cout << "ZDC - | EM <Time>: " << ZDCNSumEMTime/5. << std::endl;
      std::cout << "ZDC - | HAD <Time>: " << ZDCNSumHADTime/4. << std::endl;
    }

  }

  if (zdc_digi){
    for(int i=0; i<180; i++){DigiDatafC[i]=0;DigiDataADC[i]=0;}
    for (ZDCDigiCollection::const_iterator j=zdc_digi->begin();j!=zdc_digi->end();j++){
      const ZDCDataFrame digi = (const ZDCDataFrame)(*j);		
      int iSide      = digi.id().zside();
      int iSection   = digi.id().section();
      int iChannel   = digi.id().channel();
      int chid = (iSection-1)*5+(iSide+1)/2*9+(iChannel-1);

      const HcalQIEShape* qieshape=conditions->getHcalShape();
      const HcalQIECoder* qiecoder=conditions->getHcalCoder(digi.id());
      CaloSamples caldigi;
      HcalCoderDb coder(*qiecoder,*qieshape);

      coder.adc2fC(digi,caldigi);

      int fTS = digi.size();

      for (int i = 0; i < fTS; ++i) {
	DigiDatafC[i+chid*10] = caldigi[i];
	DigiDataADC[i+chid*10] = digi[i].adc();
	if (debug_deep){
	  std::cout << "DigiDataADC["<<i+chid*10<<"]: " << DigiDataADC[i+chid*10] << std::endl;
	  std::cout << "DigiDatafC["<<i+chid*10<<"]: " << DigiDatafC[i+chid*10] << std::endl;
	}
      }

      if (debug){
	std::cout << "iSide: " << iSide << std::endl;
	std::cout << "iSection: " << iSection << std::endl;
	std::cout << "iChannel: " << iChannel << std::endl;
	std::cout << "chid: " << chid << std::endl;
      }

    }

  }

}

//
// Fill Tower Information Energy x Eta
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillDetectorEnergyEtaInfo(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  bool debug = false;

  std::vector<double> energy_tower;
  std::vector<double> eta_tower; 

  edm::Handle<CaloTowerCollection> towerCollectionH;
  event.getByLabel(caloTowerTag_,towerCollectionH);
  const CaloTowerCollection& towerCollection = *towerCollectionH;

  CaloTowerCollection::const_iterator calotower;
  calotower = towerCollection.begin();
  CaloTowerCollection::const_iterator calotowers_end = towerCollection.end();

  for(; calotower != calotowers_end; ++calotower) {

    if (fabs(calotower->eta())> 4.7) continue;   /// excluding ring12 and ring13 of HF

    bool hasHCAL = false;
    bool hasHF = false;
    bool hasHE = false;
    bool hasHB = false;
    bool hasHO = false;
    bool hasECAL = false;
    bool hasEE = false;
    bool hasEB = false;  

    for(size_t iconst = 0; iconst < calotower->constituentsSize(); iconst++){

      DetId adetId = calotower->constituent(iconst);
      if(adetId.det()==DetId::Hcal){
	hasHCAL = true;
	HcalDetId hcalDetId(adetId);
	if(hcalDetId.subdet()==HcalForward) hasHF = true;
	else if(hcalDetId.subdet()==HcalEndcap) hasHE = true;
	else if(hcalDetId.subdet()==HcalBarrel) hasHB = true;
	else if(hcalDetId.subdet()==HcalOuter) hasHO = true;  
      } 
      else if(adetId.det()==DetId::Ecal){
	hasECAL = true;
	EcalSubdetector ecalSubDet = (EcalSubdetector)adetId.subdetId();
	if(ecalSubDet == EcalEndcap) hasEE = true;
	else if(ecalSubDet == EcalBarrel) hasEB = true;
      }
    }

    bool CalAboveTh = false;
    double caloTowerEnergy = -999.;
    double caloTowerEta = -999;
    double caloTowerEmEnergy = calotower->emEnergy();
    double caloTowerHadEnergy = calotower->hadEnergy();

    if( hasHF && !hasHE )
    {
      if( caloTowerEnergy > energyThresholdHF_ && fabs(calotower->eta())> 2.98 )   //// excluding HF ring1
      {
	CalAboveTh = true;
	caloTowerEnergy = calotower->energy();
	caloTowerEta = calotower->eta();
	energy_tower.push_back(caloTowerEnergy);
	eta_tower.push_back(caloTowerEta);

	if (debug) {
	  std::cout << "HF Energy for each CaloTower (GeV): " << caloTowerEnergy << " | Eta for each CaloTower: " << caloTowerEta << std::endl;
	}

      }
    }
    else if( hasHE && !hasHF && !hasHB )
    {
      if( caloTowerHadEnergy > energyThresholdHE_)
      {
	CalAboveTh = true;
	caloTowerEnergy = calotower->energy();
	caloTowerEta = calotower->eta();
	energy_tower.push_back(caloTowerEnergy);
	eta_tower.push_back(caloTowerEta);

	if (debug) {
	  std::cout << "HE Energy for each CaloTower (GeV): " << caloTowerEnergy << " | Eta for each CaloTower: " << caloTowerEta << std::endl;
	}

      }
    }
    else if( hasHB && !hasHE )
    {
      if( caloTowerHadEnergy > energyThresholdHB_)
      {
	CalAboveTh = true;
	caloTowerEnergy = calotower->energy();
	caloTowerEta = calotower->eta();
	energy_tower.push_back(caloTowerEnergy);
	eta_tower.push_back(caloTowerEta);

	if (debug) {
	  std::cout << "HB Energy for each CaloTower (GeV): " << caloTowerEnergy << " | Eta for each CaloTower: " << caloTowerEta << std::endl;
	}

      }
    }

    if( hasEE && !hasEB )
    {
      if( caloTowerEmEnergy >= energyThresholdEE_)
      {
	CalAboveTh = true;    
	caloTowerEnergy = calotower->energy();
	caloTowerEta = calotower->eta();
	energy_tower.push_back(caloTowerEnergy);
	eta_tower.push_back(caloTowerEta);
	if (debug) {
	  std::cout << "EB Energy for each CaloTower (GeV): " << caloTowerEnergy << " | Eta for each CaloTower: " << caloTowerEta << std::endl;
	}

      }
    }
    else if( hasEB && !hasEE )
    {
      if( caloTowerEmEnergy >= energyThresholdEB_)
      {
	CalAboveTh = true;
	caloTowerEnergy = calotower->energy();
	caloTowerEta = calotower->eta();
	energy_tower.push_back(caloTowerEnergy);
	eta_tower.push_back(caloTowerEta);

	if (debug) {
	  std::cout << "EB Energy for each CaloTower (GeV): " << caloTowerEnergy << " | Eta for each CaloTower: " << caloTowerEta << std::endl;
	}

      }
    }

    eventData.SetEachTowerEta(eta_tower);
    eventData.SetEachTowerEnergy(energy_tower);

  }  ////has to close calotower loop

}

