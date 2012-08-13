#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>
#include <map>

class TH1F;

class PileUpInfoFilter : public edm::EDFilter {
    public:
       explicit PileUpInfoFilter(const edm::ParameterSet&);
       ~PileUpInfoFilter();

       virtual void beginJob();
       virtual bool filter(edm::Event&, const edm::EventSetup&);
   private:
       edm::InputTag pileUpSummaryInfoTag_;
       int numberPUBx0_;

       bool selectBxm1;
       bool selectBxp1;
       int numberPUBxm1_;
       int numberPUBxp1_;

       std::map<std::string,TH1F*> histosTH1F_;
};

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"
#include <vector>


PileUpInfoFilter::PileUpInfoFilter(const edm::ParameterSet& pset):
   pileUpSummaryInfoTag_( pset.getParameter<edm::InputTag>("pileUpSummaryInfoTag") ), 
   numberPUBx0_( pset.getParameter<int>("numberOfPileUpEventsBx0") ),
   selectBxm1(false), selectBxp1(false), numberPUBxm1_(0), numberPUBxp1_(0) {

   if( pset.exists("numberOfPileUpEventsBxm1") ){
      selectBxm1 = true;
      numberPUBxm1_ = pset.getParameter<int>("numberOfPileUpEventsBxm1");
   }
   if( pset.exists("numberOfPileUpEventsBxp1") ){
      selectBxp1 = true;
      numberPUBxp1_ = pset.getParameter<int>("numberOfPileUpEventsBxp1");
   }
}

void PileUpInfoFilter::beginJob(){
   edm::Service<TFileService> fs;

   histosTH1F_["nPileUpBx0"] = fs->make<TH1F>("nPileUpBx0","Number of pile-up events (Bx 0)",30,0,30);
   //if(selectBxm1)
   histosTH1F_["nPileUpBxm1"] = fs->make<TH1F>("nPileUpBxm1","Number of pile-up events (Bx -1)",30,0,30);
   //if(selectBxp1)
   histosTH1F_["nPileUpBxp1"] = fs->make<TH1F>("nPileUpBxp1","Number of pile-up events (Bx +1)",30,0,30);
}

PileUpInfoFilter::~PileUpInfoFilter() {}

bool PileUpInfoFilter::filter(edm::Event& event, const edm::EventSetup& setup){
   /*edm::Handle<PileupSummaryInfo> pileUpInfo;
   event.getByLabel( pileUpInfoTag_, pileUpInfo );
   int n_PU = pileUpInfo->getPU_NumInteractions();

   int requiredPU = numberPU_;
   bool accept = (n_PU == requiredPU);*/

   edm::Handle<std::vector<PileupSummaryInfo> >  PupInfo;
   event.getByLabel(pileUpSummaryInfoTag_, PupInfo);

   std::vector<PileupSummaryInfo>::const_iterator PVI;

   int nm1 = -1; int n0 = -1; int np1 = -1;
   float nTrueInt = -1;
   for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {

      int BX = PVI->getBunchCrossing();

      if(BX == -1) { 
	 nm1 = PVI->getPU_NumInteractions();
      }
      if(BX == 0) { 
	 n0 = PVI->getPU_NumInteractions();
	 nTrueInt = PVI->getTrueNumInteractions(); 
      }
      if(BX == 1) { 
	 np1 = PVI->getPU_NumInteractions();
      }
   }

   histosTH1F_["nPileUpBx0"]->Fill(n0);
   histosTH1F_["nPileUpBxm1"]->Fill(nm1);
   histosTH1F_["nPileUpBxp1"]->Fill(np1);
   bool accept = ( n0 == numberPUBx0_ );
   if(selectBxm1) accept = accept && ( nm1 == numberPUBxm1_ );  
   if(selectBxp1) accept = accept && ( np1 == numberPUBxp1_ );  
   return accept;
}

DEFINE_FWK_MODULE(PileUpInfoFilter);
