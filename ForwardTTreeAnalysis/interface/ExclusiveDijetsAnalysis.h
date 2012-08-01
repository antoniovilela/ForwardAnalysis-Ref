#ifndef ExclusiveDijetsAnalysis_h
#define ExclusiveDijetsAnalysis_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include <string>
#include <map>
#include <vector>
class TH1F;
class TH2F;
class JetCorrector;
class ExclusiveDijetsEvent;

namespace exclusiveDijetsAnalysis {

class ExclusiveDijetsAnalysis {
  public:
     typedef ExclusiveDijetsEvent event_type;
     static const char* name;

     ExclusiveDijetsAnalysis() { }
     ExclusiveDijetsAnalysis(const edm::ParameterSet&);
     ~ExclusiveDijetsAnalysis();

     void begin();
     void begin(const edm::Run&, const edm::EventSetup&);
     void fill(ExclusiveDijetsEvent&, const edm::Event&, const edm::EventSetup&);
     void end();
  private:
    
     template <class OneJetColl,class OnePartColl> //eliza
     double Rj(OneJetColl& jetCollection,OnePartColl& partCollection); //eliza
     template <class JetColl,class PartColl>
     double Rjj(JetColl& jetCollection,PartColl& partCollection);
     template <class ThreeJetColl,class ThreePartColl> //eliza
     double Rjjj(ThreeJetColl& jetCollection,ThreePartColl& partCollection);//eliza

     unsigned int nHFSlice(const std::map<unsigned int, std::vector<unsigned int> >& mapTreshToiEta, unsigned int thresholdHF, unsigned int ieta);
 
     //void setGenInfo(const edm::Event&, const edm::EventSetup&);
     void setTFileService();
     void fillTriggerInfo(ExclusiveDijetsEvent&, const edm::Event&, const edm::EventSetup&); //added by eliza
     void fillPileUpInfo(ExclusiveDijetsEvent&, const edm::Event&, const edm::EventSetup&);
     void fillVertexInfo(ExclusiveDijetsEvent&, const edm::Event&, const edm::EventSetup&);
     void fillJetInfo(ExclusiveDijetsEvent&, const edm::Event&, const edm::EventSetup&);
     void fillMultiplicities(ExclusiveDijetsEvent&, const edm::Event&, const edm::EventSetup&);
     void fillCastorInfo(ExclusiveDijetsEvent&, const edm::Event&, const edm::EventSetup&);
     void fillXiInfo(ExclusiveDijetsEvent&, const edm::Event&, const edm::EventSetup&);
     void fillPFFlowInfo(ExclusiveDijetsEvent&, const edm::Event&, const edm::EventSetup&); //added by eliza
     
     // PF functions
     void resetPFThresholds(std::map<int,std::pair<double,double> >&);
     void setPFThresholds(std::map<int,std::pair<double,double> >&, edm::ParameterSet const&);
    
     void fillEventVariables(ExclusiveDijetsEvent&, const edm::Event&, const edm::EventSetup&);

     bool runOnData_;
     //bool initializeTFileService_;

     edm::InputTag vertexTag_;
     edm::InputTag trackTag_;
     edm::InputTag metTag_;
     edm::InputTag jetTag_;
     edm::InputTag jetNonCorrTag_;
     edm::InputTag particleFlowTag_;
     edm::InputTag castorRecHitTag_; //eliza
     edm::InputTag genChargedTag_;
     edm::InputTag triggerResultsTag_;
     edm::InputTag hcalTowerSummaryTag_;
     edm::InputTag trackMultiplicityTag_;
     bool doBtag_;
     std::string bTagMode_;
     std::string bDiscriminatorName_;
     double bDiscMin_;

     double energyThresholdHB_;
     double energyThresholdHE_;
     double energyThresholdHF_;

     unsigned int thresholdHF_;
     bool applyEnergyScaleHCAL_;
     double energyScaleHCAL_;
     bool useJetCorrection_;
     bool accessPileUpInfo_;
     double Ebeam_;
     bool usePAT_; 

     //new variables:
     bool accessMCInfo_;
     bool POMPYTMCInfo_;
     
     std::vector<std::string> hltPathNames_;
   
     std::string selectionPathName_;

     std::map<int,std::pair<double,double> > thresholdsPFlowBarrel_;
     std::map<int,std::pair<double,double> > thresholdsPFlowEndcap_;
     std::map<int,std::pair<double,double> > thresholdsPFlowTransition_;
     std::map<int,std::pair<double,double> > thresholdsPFlowForward_;
     std::map<int,std::map<int,std::pair<double,double> > > thresholdsPFlow_;

    
     std::string jetCorrectionService_;
     const JetCorrector* corrector_;

     TH1F *hltTriggerPassHisto_,*hltTriggerNamesHisto_;
};

} // namespace
#endif 
