#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZAnalysis.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZEvent.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "FWCore/Utilities/interface/RegexMatch.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

using diffractiveZAnalysis::DiffractiveZAnalysis;

const char* DiffractiveZAnalysis::name = "DiffractiveZAnalysis";

DiffractiveZAnalysis::DiffractiveZAnalysis(const edm::ParameterSet& pset):
  triggerResultsTag_(pset.getParameter<edm::InputTag>("TriggerResultsTag")),
  electronTag_(pset.getParameter<edm::InputTag>("electronTag")),
  muonTag_(pset.getParameter<edm::InputTag>("muonTag"))
{
}

DiffractiveZAnalysis::~DiffractiveZAnalysis(){}

void DiffractiveZAnalysis::begin() {}

void DiffractiveZAnalysis::begin(const edm::Run& run, const edm::EventSetup& setup) {}

void DiffractiveZAnalysis::end() {}

void DiffractiveZAnalysis::fill(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
 

  eventData.reset();

//  fillTriggerInfo(eventData,event,setup);
  fillMuonsInfo(eventData,event,setup);
  fillElectronsInfo(eventData,event,setup);

}

// Fill Trigger
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
  }
 
}else{
        std::cout << "\n No valid trigger result." <<std::endl;
     }

}

// Fill Reco::Electron
void DiffractiveZAnalysis::fillElectronsInfo(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

    int ElectronN=0;

     edm::Handle<reco::GsfElectronCollection> electrons;
     event.getByLabel(electronTag_,electrons);

     const reco::GsfElectron* electron1=NULL;
     const reco::GsfElectron* electron2=NULL;

     int electronsize = electrons->size();
     int itElectron;

     if(electrons.isValid() && electrons->size()>1){

      for(itElectron=0; itElectron < electronsize; ++itElectron){

       ++ElectronN;

       const reco::GsfElectron* electronAll = &((*electrons)[itElectron]);

       //std::cout << "Muon[" << itElectron<< "]: Eta = "<< electronAll->eta() << " | Phi = " << electronAll->phi() << " | pt = " << electronAll->pt() << std::endl;

         if (electronAll==NULL) continue;

         if (electron1==NULL) {electron1=electronAll; continue;}
         if (electronAll->pt()>electron1->pt()) {
           electron2=electron1;
           electron1=electronAll;
           continue;
         }

         if (electron2==NULL) {electron2=electronAll; continue;}
         if (electronAll->pt()>electron2->pt()) electron2 = electronAll;


/*     std::cout << "\n----------------------------------------------------" << std::endl;
       std::cout << "\t energy (ecal, hcal, ho): " << itElectron->calEnergy().em << ", " << 
                                                      itElectron->calEnergy().had << ", " << itElectron->calEnergy().ho << std::endl;
       std::cout << "\t isolation dR=0.3 (sumPt, emEt, hadEt, hoEt, nTracks, nJets): " << 
                                                      itElectron->isolationR03().sumPt << ", " << itElectron->isolationR03().emEt << ", " << 
                                                      itElectron->isolationR03().hadEt << ", " << itElectron->isolationR03().hoEt << ", " <<
                                                      itElectron->isolationR03().nTracks << ", " << itElectron->isolationR03().nJets << std::endl;
       std::cout << "\t isolation dR=0.5 (sumPt, emEt, hadEt, hoEt, nTracks, nJets): " << 
                                                      itElectron->isolationR05().sumPt << ", " << itElectron->isolationR05().emEt << ", " << 
                                                      itElectron->isolationR05().hadEt << ", " << itElectron->isolationR05().hoEt << ", " <<
                                                      itElectron->isolationR05().nTracks << ", " << itElectron->isolationR05().nJets << std::endl;
       std::cout << "\t # matches: " << itElectron->numberOfMatches() << std::endl;
       std::cout << "\t # caloCompatibility: " << itElectron->caloCompatibility() << std::endl;  
*/


     }

      // Dielectron Mass
      math::XYZTLorentzVector DielectronSystem(0.,0.,0.,0.);
      DielectronSystem += electron1->p4();
      DielectronSystem += electron2->p4();
      eventData.SetDiElectronMass(DielectronSystem.M());

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

/*     std::cout << "NElectron: " << ElectronN << std::endl;
       std::cout << "NSize: " << electrons->size() << std::endl;
       std::cout << "Muon1: " << electron1->pt() << std::endl;
       std::cout << "Muon2: " << electron2->pt() << std::endl;
*/

     }


}

// Fill Reco::Muon
void DiffractiveZAnalysis::fillMuonsInfo(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

     int MuonsN=0;

     edm::Handle<reco::MuonCollection> muons;
     event.getByLabel(muonTag_,muons);

     const reco::Muon* muon1=NULL;
     const reco::Muon* muon2=NULL;

     int muonsize = muons->size();
     int itMuon;

     if(muons.isValid() && muons->size()>1){

      for(itMuon=0; itMuon < muonsize; ++itMuon){

       ++MuonsN;
 
       const reco::Muon* muonAll = &((*muons)[itMuon]);

       //std::cout << "Muon[" << itMuon<< "]: Eta = "<< muonAll->eta() << " | Phi = " << muonAll->phi() << " | pt = " << muonAll->pt() << std::endl;

         if (muonAll==NULL) continue;
  
         if (muon1==NULL) {muon1=muonAll; continue;}
         if (muonAll->pt()>muon1->pt()) {
           muon2=muon1;
           muon1=muonAll;
           continue;
         }
   
         if (muon2==NULL) {muon2=muonAll; continue;}
         if (muonAll->pt()>muon2->pt()) muon2 = muonAll;


/*     std::cout << "\n----------------------------------------------------" << std::endl;

       std::cout << "\t energy (ecal, hcal, ho): " << itMuon->calEnergy().em << ", " << 
                                                      itMuon->calEnergy().had << ", " << itMuon->calEnergy().ho << std::endl;
       std::cout << "\t isolation dR=0.3 (sumPt, emEt, hadEt, hoEt, nTracks, nJets): " << 
                                                      itMuon->isolationR03().sumPt << ", " << itMuon->isolationR03().emEt << ", " << 
                                                      itMuon->isolationR03().hadEt << ", " << itMuon->isolationR03().hoEt << ", " <<
                                                      itMuon->isolationR03().nTracks << ", " << itMuon->isolationR03().nJets << std::endl;
       std::cout << "\t isolation dR=0.5 (sumPt, emEt, hadEt, hoEt, nTracks, nJets): " << 
                                                      itMuon->isolationR05().sumPt << ", " << itMuon->isolationR05().emEt << ", " << 
                                                      itMuon->isolationR05().hadEt << ", " << itMuon->isolationR05().hoEt << ", " <<
                                                      itMuon->isolationR05().nTracks << ", " << itMuon->isolationR05().nJets << std::endl;
       std::cout << "\t # matches: " << itMuon->numberOfMatches() << std::endl;
       std::cout << "\t # caloCompatibility: " << itMuon->caloCompatibility() << std::endl;  
*/


     }

      // Dimuon Mass
      math::XYZTLorentzVector DimuonSystem(0.,0.,0.,0.);
      DimuonSystem += muon1->p4();
      DimuonSystem += muon2->p4();
      eventData.SetDiMuonMass(DimuonSystem.M());

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

/*     std::cout << "NMuons: " << MuonsN << std::endl;
       std::cout << "NSize: " << muons->size() << std::endl;
       std::cout << "Muon1: " << muon1->pt() << std::endl;
       std::cout << "Muon2: " << muon2->pt() << std::endl;
*/

     }

}

