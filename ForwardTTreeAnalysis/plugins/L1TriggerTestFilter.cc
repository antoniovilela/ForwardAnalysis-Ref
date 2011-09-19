
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <string>

class TH1F;

class L1TriggerTestFilter: public edm::EDFilter
{
  public: 
    explicit L1TriggerTestFilter(const edm::ParameterSet&);
    ~L1TriggerTestFilter();

    virtual void beginJob(const edm::EventSetup&);
    virtual void endJob();
    virtual bool filter(edm::Event&, const edm::EventSetup&);
  private:
    unsigned int thresholdHFRingEtSum_;
    bool accessL1GctHFRingEtSums_;

    std::vector<std::string> ringNames_;

    TH1F* h_countAll_;
    std::vector<TH1F*> histosRingSum_;

    std::vector<std::string> l1TriggerNames_;
};


#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <DataFormats/L1GlobalCaloTrigger/interface/L1GctHFRingEtSums.h>
#include <DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h>
#include <DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h>
#include <DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCounts.h>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"

typedef std::vector<L1GctHFRingEtSums> L1GctHFRingEtSumsCollection;

L1TriggerTestFilter::L1TriggerTestFilter(const edm::ParameterSet& pset):
     thresholdHFRingEtSum_(pset.getParameter<unsigned int>("HFRingETSumThreshold")),
     accessL1GctHFRingEtSums_(pset.getUntrackedParameter<bool>("AccessL1GctHFRingEtSums",true)),
     l1TriggerNames_(pset.getParameter<std::vector<std::string> >("L1TriggerNames")){}

L1TriggerTestFilter::~L1TriggerTestFilter() {}

void L1TriggerTestFilter::beginJob(const edm::EventSetup& setup){
  edm::Service<TFileService> fs;

  h_countAll_ = fs->make<TH1F>("countAll","countAll",3,0,3);
  h_countAll_->GetXaxis()->SetBinLabel(1,"All");
  h_countAll_->GetXaxis()->SetBinLabel(2,"L1Jets");
  h_countAll_->GetXaxis()->SetBinLabel(3,"L1HFRingEtSum");
  
  ringNames_.push_back("Ring 1 HF-plus");
  ringNames_.push_back("Ring 1 HF-minus");
  ringNames_.push_back("Ring 2 HF-plus");
  ringNames_.push_back("Ring 2 HF-minus");

  histosRingSum_.resize(ringNames_.size());
  assert(histosRingSum_.size() == ringNames_.size());
  for(size_t k = 0; k < histosRingSum_.size(); ++k) histosRingSum_[k] = fs->make<TH1F>(ringNames_[k].c_str(),ringNames_[k].c_str(),8,0,8); 

  /*l1TriggerNames_.push_back("L1_SingleJet30");
  l1TriggerNames_.push_back("L1_SingleJet50");
  l1TriggerNames_.push_back("L1_SingleJet70");
  l1TriggerNames_.push_back("L1_SingleJet100");*/
}

void L1TriggerTestFilter::endJob(){
  float nall = h_countAll_->GetBinContent(1);
  float njets = h_countAll_->GetBinContent(2); 
  float nHFRingETSum = h_countAll_->GetBinContent(3);

  assert((njets <= nall)&&(nHFRingETSum <= njets)&&"Selected higher number of events than in previous step");

  edm::LogInfo("") << "Efficiency L1 jets            = " << ((nall)?(njets/nall):-1) << "\n"
                   << "Efficiency L1 HF Rings ET sum = " << ((nall)?(nHFRingETSum/nall):-1)
                                                         << " relative = " << ((njets)?(nHFRingETSum/njets):-1);
}

bool L1TriggerTestFilter::filter(edm::Event& event, const edm::EventSetup& setup){

  // Access L1 bits
  edm::Handle<L1GlobalTriggerReadoutRecord> l1GTReadoutRcdH;
  event.getByLabel("hltGtDigis", l1GTReadoutRcdH);

  edm::Handle<L1GlobalTriggerObjectMapRecord> l1GTObjMapRcdH;
  event.getByLabel("hltL1GtObjectMap",l1GTObjMapRcdH);
   TString Tname;
   // int Tbits[20];
  if(!l1GTReadoutRcdH.isValid() || !l1GTObjMapRcdH.isValid()) return false;  

  h_countAll_->Fill(0);
  DecisionWord gtDecisionWord = l1GTReadoutRcdH->decisionWord();

  const std::vector<L1GlobalTriggerObjectMap>& objMapVec = l1GTObjMapRcdH->gtObjectMap();
  std::vector<std::string> passedL1;
  for(std::vector<L1GlobalTriggerObjectMap>::const_iterator itMap = objMapVec.begin();
                                                            itMap != objMapVec.end(); ++itMap) {
    // Get trigger bits
    int itrig = (*itMap).algoBitNumber();
    // Get trigger names
    //std::string trigName = (*itMap).algoName();

    if(gtDecisionWord[itrig]) passedL1.push_back((*itMap).algoName());
  }
 
  for(std::vector<std::string>::const_iterator itPassedL1 = passedL1.begin(); itPassedL1 != passedL1.end(); ++itPassedL1)
             LogTrace("") << "Passed L1 trigger " << *itPassedL1;

  // Check if event satisfied one of the pre-defined triggers
  bool acceptL1Jets = false;
  for(std::vector<std::string>::const_iterator itTrigL1 = l1TriggerNames_.begin();
                                               itTrigL1 != l1TriggerNames_.end(); ++itTrigL1){
    if(find(passedL1.begin(),passedL1.end(),*itTrigL1) != passedL1.end()) {acceptL1Jets = true;break;}
  }  
  
  if(!acceptL1Jets) return false;
  h_countAll_->Fill(1);

  bool acceptHFRingEtSum = true;
  if(accessL1GctHFRingEtSums_){
    edm::Handle<L1GctHFRingEtSumsCollection> ringSumsH;
    event.getByLabel("hltGctDigis", ringSumsH);
    L1GctHFRingEtSumsCollection::const_iterator ringSumItr;
  
    // Require sum(ET) threshold for all rings
    for(ringSumItr=ringSumsH->begin(); ringSumItr->bx()==0 && ringSumItr != ringSumsH->end(); ++ringSumItr){
      LogTrace("") << "Is empty: " << ringSumItr->empty();  
      for(size_t k = 0; k < 4; ++k){ 
        LogTrace("") << "ET Sum for " << ringNames_[k] << ": " << ringSumItr->etSum(k);
        histosRingSum_[k]->Fill(ringSumItr->etSum(k));
        if(ringSumItr->etSum(k) > thresholdHFRingEtSum_) acceptHFRingEtSum = false;
      }
    }
  } else{
    edm::Handle<std::vector<L1GctJetCounts> > jetCountsH;
    event.getByLabel("hltGctDigis", jetCountsH);

    for(std::vector<L1GctJetCounts>::const_iterator jetCountItr = jetCountsH->begin();
                                                    jetCountItr != jetCountsH->end(); ++jetCountItr){
      int iring = 0;
      for(int icount = 8; icount <= 11; ++icount,++iring){
        LogTrace("") << "ET Sum index " << icount << ": " << jetCountItr->count(icount);
        histosRingSum_[iring]->Fill(jetCountItr->count(icount));
        if(jetCountItr->count(icount) > thresholdHFRingEtSum_) acceptHFRingEtSum = false; 
      }
    }
  }

  if(!acceptHFRingEtSum) return false;
  h_countAll_->Fill(2);

  return true;
}

DEFINE_FWK_MODULE(L1TriggerTestFilter);
