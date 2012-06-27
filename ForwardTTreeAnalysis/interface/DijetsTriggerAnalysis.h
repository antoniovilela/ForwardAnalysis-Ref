#ifndef DijetsTriggerAnalysis_h
#define DijetsTriggerAnalysis_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "L1Trigger/GlobalTriggerAnalyzer/interface/L1GtUtils.h"

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFRingEtSums.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctHFBitCounts.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctCollections.h"

#include <string>
#include <map>
#include <vector>

class TH1F;
class TH2F;
class DijetsTriggerEvent;

namespace dijetsTriggerAnalysis {

   class DijetsTriggerAnalysis {
      public:
         typedef DijetsTriggerEvent event_type;
         static const char* name;

         DijetsTriggerAnalysis() { }
	 DijetsTriggerAnalysis(const edm::ParameterSet&);
	 ~DijetsTriggerAnalysis();

	 void setBeginRun(const edm::Run&, const edm::EventSetup&);
	 void fillEventData(DijetsTriggerEvent&, const edm::Event&, const edm::EventSetup&);
      private:
//         unsigned int nHFSlice(const std::map<unsigned int, std::vector<unsigned int> >& mapTreshToiEta, unsigned int thresholdHF, unsigned int ieta);
         //=============================================== 
	 bool acceptHFRingEtSum(std::vector<TH1F*>&, const L1GctHFRingEtSumsCollection*);
	 bool acceptHFRingEtSum(std::vector<TH1F*>&, const L1GctHFBitCountsCollection*);
         void setTFileService();
	 void dijetsTriggerInfo(DijetsTriggerEvent&, const edm::Event&, const edm::EventSetup&); 
         void dijetsTriggerJetInfo(DijetsTriggerEvent&, const edm::Event&, const edm::EventSetup&);
         void dijetsTriggerCaloTowerInfo(DijetsTriggerEvent&, const edm::Event&, const edm::EventSetup&);

           

	 class Correlation{
	    public:
	       Correlation():sumEvt_(0.),sumX_(0.),sumX2_(0.),sumY_(0.),sumY2_(0.),sumXY_(0.) {}
	       void Fill(double x, double y) {++sumEvt_;sumX_ += x;sumX2_ += x*x;sumY_ += y;sumY2_ += y*y;sumXY_ += x*y;}
	       double Value() {
		  double covxy = sumXY_/sumEvt_ - (sumX_/sumEvt_)*(sumY_/sumEvt_);
		  double sigx = sqrt(fabs(sumX2_/sumEvt_ - (sumX_/sumEvt_)*(sumX_/sumEvt_)));
		  double sigy = sqrt(fabs(sumY2_/sumEvt_ - (sumY_/sumEvt_)*(sumY_/sumEvt_)));

		  return ((sigx == 0.)||(sigy == 0.)) ? 0. : covxy/(sigx*sigy);
	       }
	    private:
	       L1GtUtils m_l1GtUtils;
               double sumEvt_;
	       double sumX_;
	       double sumX2_;
	       double sumY_;
	       double sumY2_;
	       double sumXY_;
	 };


         edm::InputTag jetTag_;
         edm::InputTag particleFlowTag_;
         edm::InputTag caloTowerTag_;
	 edm::InputTag gtDigisTag_;
	 edm::InputTag l1GtObjectMapTag_; 
	 edm::InputTag gctDigisTag_;
         edm::InputTag triggerResultsTag_;
       
	 L1GtUtils l1GtUtils_;

	 unsigned int thresholdHFRingEtSum_;
	 unsigned int thresholdHFRingBitCount_;

	 std::vector<std::string> ringNames_;

	 TH1F* h_summaryL1_;
	 std::vector<TH1F*> histosCountAll_;
	 std::vector<std::vector<TH1F*> > histosRingEtSum_;
	 std::vector<std::vector<TH1F*> > histosRingBitCount_;

	 std::vector<std::string> l1TriggerNames_;

         std::vector<std::string> hltPathNames_;

	 TH2F* h_correlations_;
	 std::map<std::pair<int,int>,Correlation> correlations_;

         TH1F *hltTriggerPassHisto_,*hltTriggerNamesHisto_;
         //int jtow;         
         //std::vector<double>towET_;
   };
} // namespace
#endif //end code
