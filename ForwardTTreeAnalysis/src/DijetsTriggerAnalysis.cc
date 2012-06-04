#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFRingEtSums.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFBitCounts.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCounts.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctCollections.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
//HLT
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Utilities/interface/RegexMatch.h"
//
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
 
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
//#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/FWLiteTools.h"

//#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFRingEtSums.h"
//#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFBitCounts.h"
//#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCounts.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DijetsTriggerAnalysis.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DijetsTriggerEvent.h"

#include "TH1F.h"
#include "TH2F.h"
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <iostream>

using dijetsTriggerAnalysis::DijetsTriggerAnalysis;

const char* DijetsTriggerAnalysis::name = "DijetsTriggerAnalysis";

DijetsTriggerAnalysis::DijetsTriggerAnalysis(const edm::ParameterSet& pset):
     jetTag_(pset.getParameter<edm::InputTag>("jetTag")),
     particleFlowTag_(pset.getParameter<edm::InputTag>("particleFlowTag")),
     caloTowerTag_(pset.getParameter<edm::InputTag>("caloTowerTag")),
     gctDigisTag_(pset.getParameter<edm::InputTag>("gctDigisTag")),
     triggerResultsTag_(pset.getParameter<edm::InputTag>("TriggerResultsTag")), 
     thresholdHFRingEtSum_(pset.getParameter<unsigned int>("hfRingEtSumThreshold")),
     thresholdHFRingBitCount_(pset.getParameter<unsigned int>("hfRingBitCountThreshold")),
     l1TriggerNames_(pset.getParameter<std::vector<std::string> >("l1TriggerNames")),
     hltPathNames_(pset.getParameter<std::vector<std::string> >("hltPaths")) {
     ringNames_.push_back("Ring 1 HF-plus");
     ringNames_.push_back("Ring 1 HF-minus");
     ringNames_.push_back("Ring 2 HF-plus");
     ringNames_.push_back("Ring 2 HF-minus");
     setTFileService(); 
}
   /*const int nMaxTower = 10000;
   float towET_[nMaxTower] ;
   float towEta_[nMaxTower];
   float towPhi_[nMaxTower];
   float towE_[nMaxTower];
   float towEm_[nMaxTower];
   float towHad_[nMaxTower];
   float towOe_[nMaxTower];*/

void DijetsTriggerAnalysis::setTFileService(){
  edm::Service<TFileService> fs;

  std::ostringstream oss;
  h_summaryL1_ = fs->make<TH1F>("summaryL1","summaryL1",l1TriggerNames_.size()+1,0,l1TriggerNames_.size()+1);  
  h_summaryL1_->GetXaxis()->SetBinLabel(1,"All");
  histosCountAll_.resize(l1TriggerNames_.size());
  histosRingEtSum_.resize(l1TriggerNames_.size());
  histosRingBitCount_.resize(l1TriggerNames_.size());
  for(size_t k = 0; k < histosCountAll_.size(); ++k){
     oss << "Creating histograms for L1 reference trigger " << l1TriggerNames_[k] << std::endl;

     h_summaryL1_->GetXaxis()->SetBinLabel(k+2,l1TriggerNames_[k].c_str());
   
     histosCountAll_[k] = fs->make<TH1F>((l1TriggerNames_[k] + "_count").c_str(),(l1TriggerNames_[k] + "_count").c_str(),4,0,4);
     histosCountAll_[k]->GetXaxis()->SetBinLabel(1,"All");
     histosCountAll_[k]->GetXaxis()->SetBinLabel(2,l1TriggerNames_[k].c_str());
     histosCountAll_[k]->GetXaxis()->SetBinLabel(3, "L1HFRingEtSum");
     histosCountAll_[k]->GetXaxis()->SetBinLabel(4, "L1HFBitCount");

     histosRingEtSum_[k].resize(ringNames_.size());
     histosRingBitCount_[k].resize(ringNames_.size());
     for(size_t ihisto = 0; ihisto < ringNames_.size(); ++ihisto){
        std::string hname = ringNames_[ihisto] + "_" + l1TriggerNames_[k] + "_EtSum";
        histosRingEtSum_[k][ihisto] = fs->make<TH1F>(hname.c_str(),hname.c_str(),8,0,8);
        hname = ringNames_[ihisto] + "_" + l1TriggerNames_[k] + "_BitCount";
        histosRingBitCount_[k][ihisto] = fs->make<TH1F>(hname.c_str(),hname.c_str(),8,0,8);
     }
  }

  size_t nTriggers = l1TriggerNames_.size();
  h_correlations_ = fs->make<TH2F>("Correlations","Correlations",nTriggers,0,nTriggers,nTriggers,0,nTriggers);
  for(size_t i = 0; i < nTriggers; ++i){
     for(size_t j = 0; j < nTriggers; ++j){
        h_correlations_->GetXaxis()->SetBinLabel(i+1,l1TriggerNames_[i].c_str());
        h_correlations_->GetYaxis()->SetBinLabel(j+1,l1TriggerNames_[j].c_str());
        correlations_.insert(std::make_pair(std::make_pair(i,j),Correlation()));
     }
  }

  hltTriggerNamesHisto_ = fs->make<TH1F>("HLTTriggerNames","HLTTriggerNames",1,0,1);
  hltTriggerNamesHisto_->SetBit(TH1::kCanRebin);
  for(unsigned k=0; k < hltPathNames_.size(); ++k){
    oss << "Using HLT reference trigger " << hltPathNames_[k] << std::endl;
    hltTriggerNamesHisto_->Fill(hltPathNames_[k].c_str(),1);
  }

  hltTriggerPassHisto_ = fs->make<TH1F>("HLTTriggerPass","HLTTriggerPass",1,0,1);
  hltTriggerPassHisto_->SetBit(TH1::kCanRebin);

  edm::LogInfo("Analysis") << oss.str();
}

/*void DijetsTriggerAnalysis::endJob(){
   std::ostringstream oss;
   for(size_t k = 0; k < histosCountAll_.size(); ++k){
     float nall = histosCountAll_[k]->GetBinContent(1);
     float ntrig = histosCountAll_[k]->GetBinContent(2); 
     float nHFRingSum = histosCountAll_[k]->GetBinContent(3);

     oss << "Efficiency L1 trigger " << l1TriggerNames_[k] << " = " 
                                     << ( (nall) ? (ntrig/nall) : -1 ) << std::endl
         << "Efficiency L1 HF Rings = " << ( (nall) ? (nHFRingSum/nall) : -1 ) << std::endl
         << "              relative = " << ( (ntrig) ? (nHFRingSum/ntrig) : -1 ) << std::endl;
   }
   edm::LogInfo("Analysis") << oss.str();

   for(size_t i = 0; i < l1TriggerNames_.size(); ++i){
      for(size_t j = 0; j < l1TriggerNames_.size(); ++j){
         std::pair<int,int> index(i,j);
         h_correlations_->SetBinContent(i+1,j+1,correlations_[index].Value());
      }
   }
}*/

DijetsTriggerAnalysis::~DijetsTriggerAnalysis(){}

void DijetsTriggerAnalysis::setBeginRun(const edm::Run& run, const edm::EventSetup& setup){
  //l1GtUtils_.getL1GtRunCache(run, setup, true, false);
}

void DijetsTriggerAnalysis::fillEventData(DijetsTriggerEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  eventData.reset();
  eventData.l1Decision_.resize(l1TriggerNames_.size());
  eventData.l1Prescale_.resize(l1TriggerNames_.size());
  eventData.l1AlgoName_.resize(l1TriggerNames_.size());
  eventData.hltTrigResults_.resize(hltPathNames_.size());
  eventData.hltTrigNames_.resize(hltPathNames_.size());
  dijetsTriggerInfo(eventData,event,setup);
  dijetsTriggerJetInfo(eventData,event,setup); 
  dijetsTriggerCaloTowerInfo(eventData,event,setup);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//void DijetsTriggerAnalysis::analyze(const edm::Event& event, const edm::EventSetup& setup){
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DijetsTriggerAnalysis::dijetsTriggerInfo(DijetsTriggerEvent& eventData, const edm::Event& event, const edm::EventSetup& setup)
{ 
  l1GtUtils_.retrieveL1EventSetup(setup);

  eventData.SetNBits( l1TriggerNames_.size() );

  std::vector<std::string> passedL1;
  std::vector<int> prescalesL1;
  for(size_t k = 0; k < l1TriggerNames_.size(); ++k){
     int ierror = -1;
     int ierr_pr = -1;
     bool decision = l1GtUtils_.decisionBeforeMask(event,l1TriggerNames_[k],ierror);

     int l1decision = -1;
     if(ierror == 0) l1decision = decision ? 1 : 0;
     int prescale = -1;
     if(ierror == 0) prescale = l1GtUtils_.prescaleFactor(event,l1TriggerNames_[k],ierr_pr);

     if(ierror == 0 && decision){
        passedL1.push_back( l1TriggerNames_[k] );
        prescalesL1.push_back(prescale);
     }

     eventData.SetL1Bit(k,l1decision);
     eventData.SetL1Prescale(k,prescale);
     eventData.SetL1AlgoName(k,l1TriggerNames_[k]);
  }

  for(size_t k = 0; k < passedL1.size(); ++k)
     LogTrace("Analysis") << "Passed L1 trigger " << passedL1[k] << " prescale " << prescalesL1[k];

  // Check if event satisfied pre-defined triggers and get correlations
  for(size_t i = 0; i < l1TriggerNames_.size(); ++i){
     for(size_t j = 0; j < l1TriggerNames_.size(); ++j){
        std::pair<int,int> index(i,j);
        Correlation& corr = correlations_[index];
        double x = 0.;
        double y = 0.; 
        if(find(passedL1.begin(),passedL1.end(),l1TriggerNames_[i]) != passedL1.end()) x = 1.;
        if(find(passedL1.begin(),passedL1.end(),l1TriggerNames_[j]) != passedL1.end()) y = 1.; 
        corr.Fill(x,y);
     }
  }
//////////////////////////////////////////////
  edm::Handle<edm::TriggerResults> triggerResults;
  event.getByLabel(triggerResultsTag_, triggerResults);

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
     eventData.SetHLTPathName(idxpath, resolvedPathName);
     hltTriggerPassHisto_->Fill( (*hltpath).c_str(), 1 );
  }
//////////////////////////////////////////////

  const L1GctHFRingEtSumsCollection* gctHFRingEtSumsCollection = 0;
  const L1GctHFBitCountsCollection* gctHFBitCountsCollection = 0;
  edm::Handle<L1GctHFRingEtSumsCollection> gctHFRingEtSumsH;
  event.getByLabel(gctDigisTag_, gctHFRingEtSumsH);
  if( gctHFRingEtSumsH.isValid() ) gctHFRingEtSumsCollection = gctHFRingEtSumsH.product();
  else {
     edm::LogWarning("L1 GCT HF Ring Et sums collection not found.");
     //return;
  }
  edm::Handle<L1GctHFBitCountsCollection> gctHFBitCountsH;
  event.getByLabel(gctDigisTag_, gctHFBitCountsH);
  if( gctHFBitCountsH.isValid() ) gctHFBitCountsCollection = gctHFBitCountsH.product();
  else {
     edm::LogWarning("L1 GCT HF bit counts collection not found.");
     //return;
  }

  h_summaryL1_->Fill(0); 
  for(size_t k = 0; k < l1TriggerNames_.size(); ++k){
    histosCountAll_[k]->Fill(0);
    if( find(passedL1.begin(),passedL1.end(),l1TriggerNames_[k]) != passedL1.end() ){ //Found trigger
       h_summaryL1_->Fill(k+1);
 
       histosCountAll_[k]->Fill(1);

       std::vector<TH1F*>& histosEtSum = histosRingEtSum_[k];
       bool acceptRingEtSum = false;
       if(gctHFRingEtSumsCollection) acceptHFRingEtSum(histosEtSum,gctHFRingEtSumsCollection);

       std::vector<TH1F*>& histosBitCount = histosRingBitCount_[k];
       bool acceptRingBitCount = false; 
       if(gctHFBitCountsCollection) acceptHFRingEtSum(histosBitCount,gctHFBitCountsCollection);

       if(acceptRingEtSum)    histosCountAll_[k]->Fill(2); 
       if(acceptRingBitCount) histosCountAll_[k]->Fill(3); 
    }
  }
  
  if(gctHFRingEtSumsCollection){
     L1GctHFRingEtSumsCollection::const_iterator ringSumItr = gctHFRingEtSumsCollection->begin();
     for(; ringSumItr->bx()==0 && ringSumItr != gctHFRingEtSumsCollection->end(); ++ringSumItr){
	LogTrace("Analysis") << "Is empty: " << ringSumItr->empty();  
	for(size_t k = 0; k < 4; ++k){ 
	   LogTrace("Analysis") << "ET Sum for " << ringNames_[k] << ": " << ringSumItr->etSum(k);
           eventData.SetEtSum(k,ringSumItr->etSum(k));
	}
     }
  }
  if(gctHFBitCountsCollection){
     L1GctHFBitCountsCollection::const_iterator ringSumItr = gctHFBitCountsCollection->begin();
     for(; ringSumItr->bx()==0 && ringSumItr != gctHFBitCountsCollection->end(); ++ringSumItr){
	LogTrace("Analysis") << "Is empty: " << ringSumItr->empty();  
	for(size_t k = 0; k < 4; ++k){ 
	   LogTrace("Analysis") << "Bit count for " << ringNames_[k] << ": " << ringSumItr->bitCount(k);
           eventData.SetBitCount(k,ringSumItr->bitCount(k));
	}
     }
  }
  
}

bool DijetsTriggerAnalysis::acceptHFRingEtSum(std::vector<TH1F*>& histosRingSum, const L1GctHFRingEtSumsCollection* ringSumCollection){
   bool accept = true;

   L1GctHFRingEtSumsCollection::const_iterator ringSumItr = ringSumCollection->begin();
   // Require sum(ET) threshold for all rings
   for(; ringSumItr->bx()==0 && ringSumItr != ringSumCollection->end(); ++ringSumItr){
      LogTrace("Analysis") << "Is empty: " << ringSumItr->empty();  
      for(size_t k = 0; k < 4; ++k){ 
         LogTrace("Analysis") << "ET Sum for " << ringNames_[k] << ": " << ringSumItr->etSum(k);
         histosRingSum[k]->Fill(ringSumItr->etSum(k));
         if(ringSumItr->etSum(k) > thresholdHFRingEtSum_) accept = false;
      }
   }

   return accept;
}

bool DijetsTriggerAnalysis::acceptHFRingEtSum(std::vector<TH1F*>& histosRingSum, const L1GctHFBitCountsCollection* ringSumCollection){
   bool accept = true;

   L1GctHFBitCountsCollection::const_iterator ringSumItr = ringSumCollection->begin();
   // Require bit count threshold for all rings
   for(; ringSumItr->bx()==0 && ringSumItr != ringSumCollection->end(); ++ringSumItr){
      LogTrace("Analysis") << "Is empty: " << ringSumItr->empty();  
      for(size_t k = 0; k < 4; ++k){ 
         LogTrace("Analysis") << "Bit count for " << ringNames_[k] << ": " << ringSumItr->bitCount(k);
         histosRingSum[k]->Fill(ringSumItr->bitCount(k));
         if(ringSumItr->bitCount(k) > thresholdHFRingBitCount_) accept = false;
      }
   }

   return accept;
}  
void DijetsTriggerAnalysis::dijetsTriggerJetInfo(DijetsTriggerEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<edm::View<reco::Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);
  
  eventData.nJet_ = jetCollectionH->size();

  if(jetCollectionH->size() > 0){
     const reco::Jet& leadingJet = (*jetCollectionH)[0];
     //const reco::Jet& secondJet = (*jetCollectionH)[1];
     //const reco::Jet& thirdJet = (*jetCollectionH)[2];     
 
     eventData.leadingJetPt_ = leadingJet.pt();
     eventData.leadingJetEta_ = leadingJet.eta();
     eventData.leadingJetPhi_ = leadingJet.phi();
   } else{
     eventData.leadingJetPt_ = -999.;
     eventData.leadingJetEta_ = -999.;
     eventData.leadingJetPhi_ = -999.;}  
  
  if(jetCollectionH->size() > 1){
     //const reco::Jet& leadingJet = (*jetCollectionH)[0];
     const reco::Jet& secondJet = (*jetCollectionH)[1];
     const reco::Jet& thirdJet = (*jetCollectionH)[2];    
     
     eventData.secondJetPt_ = secondJet.pt();
     eventData.secondJetEta_ = secondJet.eta();
     eventData.secondJetPhi_ = secondJet.phi();
 
     eventData.thirdJetPt_ = thirdJet.pt();
     eventData.thirdJetEta_ = thirdJet.eta();
     eventData.thirdJetPhi_ = thirdJet.phi(); 

  } else{
        
     eventData.secondJetPt_ = -999.;
     eventData.secondJetEta_ = -999.;
     eventData.secondJetPhi_ = -999.;

     eventData.thirdJetPt_ = -999.;
     eventData.thirdJetEta_ = -999.;
     eventData.thirdJetPhi_ = -999.;
  }  
}
void DijetsTriggerAnalysis::dijetsTriggerCaloTowerInfo(DijetsTriggerEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  edm::Handle<CaloTowerCollection> caloTowerCollectionH;
  event.getByLabel(caloTowerTag_,caloTowerCollectionH);
  const CaloTowerCollection& caloTower = *caloTowerCollectionH;
  CaloTowerCollection::const_iterator tower = caloTower.begin();
  CaloTowerCollection::const_iterator tower_end = caloTower.end();

  double sumEHFMinus = 0.;
  double sumEHFPlus  = 0.;
  if( caloTowerCollectionH.isValid() ){


   for(; tower != tower_end; tower++){
      double eta = tower->eta();
      double energy = tower->energy();
      //double pt = tower->pt();
     
      if((3.0 < eta) && (eta < 5.0) ){
        sumEHFPlus += energy;
      }
      if((-5.0 < eta) && (eta < -3.0) ){
        sumEHFMinus += energy;
      }
     
     //eventData.SetEtaAllTowers(eta); 
     //eventData.SetEnergyAllTowers(energy); 
     //eventData.SetPtAllTowers(pt);
   }

   eventData.SetSumEHFPlus(sumEHFPlus);
   eventData.SetSumEHFMinus(sumEHFMinus);
  }else{
   eventData.SetSumEHFPlus(-999.);
   eventData.SetSumEHFMinus(-999.);
 
  }
}


  
 
