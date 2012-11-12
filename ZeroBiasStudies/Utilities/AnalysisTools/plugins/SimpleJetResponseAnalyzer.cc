
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

class TTree;

class SimpleJetResponseAnalyzer: public edm::EDAnalyzer
{
  public: 
     explicit SimpleJetResponseAnalyzer(const edm::ParameterSet&);
     ~SimpleJetResponseAnalyzer();

     virtual void beginJob();
     virtual void analyze(const edm::Event&, const edm::EventSetup&);
    
    
  private:
     //edm::InputTag genJetTag_;
     edm::InputTag genToRecoJetMatchTag_;

     TTree* data_;

     struct {
       double genJetPt_;
       double genJetEta_;
       double genJetPhi_;
       double recJetPt_;
       double recJetEta_;
       double recJetPhi_;
     } eventData_;
};


#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/MakerMacros.h"

//#include "DataFormats/JetReco/interface/Jet.h"
//#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/CandMatchMap.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"

using namespace reco;

SimpleJetResponseAnalyzer::SimpleJetResponseAnalyzer(const edm::ParameterSet& pset):
   genToRecoJetMatchTag_(pset.getParameter<edm::InputTag>("GenToRecoJetMatchTag"))
{
}

SimpleJetResponseAnalyzer::~SimpleJetResponseAnalyzer() {}

void SimpleJetResponseAnalyzer::beginJob(){
   edm::Service<TFileService> fs;

   data_ = fs->make<TTree>("data","data");
   data_->Branch("genJetPt",&eventData_.genJetPt_,"genJetPt/D");
   data_->Branch("genJetEta",&eventData_.genJetEta_,"genJetEta/D");
   data_->Branch("genJetPhi",&eventData_.genJetPhi_,"genJetPhi/D");
   data_->Branch("recJetPt",&eventData_.recJetPt_,"recJetPt/D");
   data_->Branch("recJetEta",&eventData_.recJetEta_,"recJetEta/D");
   data_->Branch("recJetPhi",&eventData_.recJetPhi_,"recJetPhi/D");
}

void SimpleJetResponseAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& setup){
   edm::Handle<CandViewMatchMap> genToRecoJetAssociationH;
   event.getByLabel(genToRecoJetMatchTag_,genToRecoJetAssociationH);
   const CandViewMatchMap& genToRecoJetAssociation = *genToRecoJetAssociationH;

   CandViewMatchMap::const_iterator it_key = genToRecoJetAssociation.begin();
   CandViewMatchMap::const_iterator match_end = genToRecoJetAssociation.end();
   for(; it_key != match_end; ++it_key){
      const CandidateBaseRef& genJet = it_key->key;
      const CandidateBaseRef& recJet = it_key->val;

      double response = recJet->pt()/genJet->pt();
      double phiDiff = recJet->phi() - genJet->phi();
      double etaDiff = recJet->eta() - genJet->eta();
      double ptDiff = recJet->pt() - genJet->pt();

      LogTrace("") << " gen. jet pt,eta,phi= " << genJet->pt() << ", " << genJet->eta() << ", " << genJet->phi()
                   << "\n rec. jet pt,eta,phi= " << recJet->pt() << ", " << recJet->eta() << ", " << recJet->phi()
                   << "\n   response,phiDiff,etaDiff,ptDiff= " << response << ", " << phiDiff << ", " << etaDiff << ", " << ptDiff;

      eventData_.genJetPt_ = genJet->pt();
      eventData_.genJetEta_ = genJet->eta();
      eventData_.genJetPhi_ = genJet->phi();
      eventData_.recJetPt_ = recJet->pt();
      eventData_.recJetEta_ = recJet->eta();
      eventData_.recJetPhi_ = recJet->phi();

      data_->Fill();
   }  
}

//DEFINE_FWK_MODULE(SimpleJetResponseAnalyzer);
