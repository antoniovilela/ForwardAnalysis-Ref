
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsAnalysis.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h" 

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"

#include "ForwardAnalysis/Utilities/interface/LargestGenRapidityGap.h"
#include "ForwardAnalysis/Utilities/interface/CastorEnergy.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/FWLiteTools.h"

//added by eliza
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "DataFormats/Luminosity/interface/LumiDetails.h"
#include "DataFormats/Luminosity/interface/LumiSummary.h"
//added PU info
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Utilities/interface/RegexMatch.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"
#include "TH2F.h"
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <map>


using namespace forwardAnalysis;
using exclusiveDijetsAnalysis::ExclusiveDijetsAnalysis;

const char* ExclusiveDijetsAnalysis::name = "ExclusiveDijetsAnalysis";

ExclusiveDijetsAnalysis::ExclusiveDijetsAnalysis(const edm::ParameterSet& pset):
  runOnData_(true), 
  vertexTag_(pset.getParameter<edm::InputTag>("VertexTag")),//ok
  trackTag_(pset.getParameter<edm::InputTag>("TrackTag")),
  metTag_(pset.getParameter<edm::InputTag>("METTag")),
  jetTag_(pset.getParameter<edm::InputTag>("JetTag")),//ok
  jetNonCorrTag_(pset.getParameter<edm::InputTag>("JetNonCorrTag")), //ok
  particleFlowTag_(pset.getParameter<edm::InputTag>("ParticleFlowTag")),//ok
  castorRecHitTag_(pset.getParameter<edm::InputTag>("CastorRecHitTag")), //added by liz
  genChargedTag_(pset.getParameter<edm::InputTag>("GenChargedParticlesTag")),//old My dijets analysis
  triggerResultsTag_(pset.getParameter<edm::InputTag>("TriggerResultsTag")),//old My dijets analysis
  trackMultiplicityTag_(pset.getParameter<edm::InputTag>("TrackMultiplicityTag")),//ok
  energyThresholdHB_(pset.getParameter<double>("EnergyThresholdHB")),//old My dijets analysis
  energyThresholdHE_(pset.getParameter<double>("EnergyThresholdHE")),//old My dijets analysis
  energyThresholdHF_(pset.getParameter<double>("EnergyThresholdHF")),//old My dijets analysis
  thresholdHF_(pset.getParameter<unsigned int>("HFThresholdIndex")),//ok
  applyEnergyScaleHCAL_(pset.getParameter<bool>("ApplyEnergyScaleHCAL")),//ok
  energyScaleHCAL_(-1.),//old My dijets analysis
  useJetCorrection_(pset.getParameter<bool>("UseJetCorrection")),//ok
  accessPileUpInfo_(pset.getParameter<bool>("AccessPileUpInfo")),//ok
  Ebeam_(pset.getUntrackedParameter<double>("EBeam",4000.)),//ok
  usePAT_(pset.getUntrackedParameter<bool>("UsePAT",true)),
  accessMCInfo_(pset.getUntrackedParameter<bool>("AccessMCInfo",true)),
  POMPYTMCInfo_(pset.getUntrackedParameter<bool>("POMPYTMCInfo",true)),
  //runOnData_(pset.getUntrackedParameter<bool>("RunOnData",true)), 
  hltPathNames_(pset.getParameter<std::vector<std::string> >("hltPaths")) {
  
  resetPFThresholds(thresholdsPFlowBarrel_);
  resetPFThresholds(thresholdsPFlowEndcap_);
  resetPFThresholds(thresholdsPFlowTransition_);
  resetPFThresholds(thresholdsPFlowForward_);
  setTFileService(); 
  if(pset.exists("PFlowThresholds")){ 
     edm::ParameterSet const& thresholdsPFPset = pset.getParameter<edm::ParameterSet>("PFlowThresholds");
 
     edm::ParameterSet const& thresholdsBarrel = thresholdsPFPset.getParameter<edm::ParameterSet>("Barrel");
     edm::ParameterSet const& thresholdsEndcap = thresholdsPFPset.getParameter<edm::ParameterSet>("Endcap");
     edm::ParameterSet const& thresholdsTransition = thresholdsPFPset.getParameter<edm::ParameterSet>("Transition");
     edm::ParameterSet const& thresholdsForward = thresholdsPFPset.getParameter<edm::ParameterSet>("Forward");

     setPFThresholds(thresholdsPFlowBarrel_,thresholdsBarrel);
     setPFThresholds(thresholdsPFlowEndcap_,thresholdsEndcap);
     setPFThresholds(thresholdsPFlowTransition_,thresholdsTransition);
     setPFThresholds(thresholdsPFlowForward_,thresholdsForward);
  }
  thresholdsPFlow_[Barrel] = thresholdsPFlowBarrel_;
  thresholdsPFlow_[Endcap] = thresholdsPFlowEndcap_; 
  thresholdsPFlow_[Transition] = thresholdsPFlowTransition_;
  thresholdsPFlow_[Forward] = thresholdsPFlowForward_;

  std::map<int,std::pair<double,double> >::const_iterator pfThreshold = thresholdsPFlowBarrel_.begin();
  std::map<int,std::pair<double,double> >::const_iterator pfThresholds_end = thresholdsPFlowBarrel_.end(); 
  std::ostringstream oss;
  oss << "Using the following PF thresholds:\n";
  for(; pfThreshold != pfThresholds_end; ++pfThreshold){
     int key = pfThreshold->first;    
     oss << "  " << key << ": "
                 << "(" << thresholdsPFlow_[Barrel][key].first
                 << "," << thresholdsPFlow_[Barrel][key].second << ")  "
                 << "(" << thresholdsPFlow_[Endcap][key].first
                 << "," << thresholdsPFlow_[Endcap][key].second << ")  "
                 << "(" << thresholdsPFlow_[Transition][key].first
                 << "," << thresholdsPFlow_[Transition][key].second << ")  "
                 << "(" << thresholdsPFlow_[Forward][key].first
                 << "," << thresholdsPFlow_[Forward][key].second << ")\n";
  } 
  LogDebug("Analysis") << oss.str();


  if(applyEnergyScaleHCAL_) energyScaleHCAL_ = pset.getParameter<double>("EnergyScaleFactorHCAL");


  if(useJetCorrection_) jetCorrectionService_ = pset.getParameter<std::string>("JetCorrectionService");
 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::setTFileService(){
  edm::Service<TFileService> fs;
  std::ostringstream oss;
  hltTriggerNamesHisto_ = fs->make<TH1F>("HLTTriggerNames","HLTTriggerNames",1,0,1);
  hltTriggerNamesHisto_->SetBit(TH1::kCanRebin);
  for(unsigned k=0; k < hltPathNames_.size(); ++k){
    oss << "Using HLT reference trigger " << hltPathNames_[k] << std::endl;
    hltTriggerNamesHisto_->Fill(hltPathNames_[k].c_str(),1);
  }

  hltTriggerPassHisto_ = fs->make<TH1F>("HLTTriggerPass","HLTTriggerPass",1,0,1);
  hltTriggerPassHisto_->SetBit(TH1::kCanRebin);
}
//////
ExclusiveDijetsAnalysis::~ExclusiveDijetsAnalysis(){}

void ExclusiveDijetsAnalysis::setBeginRun(const edm::Run& run, const edm::EventSetup& setup){
  if(useJetCorrection_) corrector_ = JetCorrector::getJetCorrector(jetCorrectionService_,setup);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillEventData(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  // Reset info
  eventData.reset();

  runOnData_ = event.isRealData();
 
  // Fill event data
  if(accessMCInfo_ && accessPileUpInfo_){
     fillPileUpInfo(eventData,event,setup);
  } else {
     eventData.SetNPileUpBxm1(-1);
     eventData.SetNPileUpBx0(-1);
     eventData.SetNPileUpBxp1(-1);
  }

  //added by eliza 
  fillEventInfo(eventData,event,setup);
  eventData.hltTrigResults_.resize(hltPathNames_.size());
  eventData.hltTrigNames_.resize(hltPathNames_.size());
  fillTriggerInfo(eventData,event,setup);
  fillVertexInfo(eventData,event,setup);
 

 fillJetInfo(eventData,event,setup);


  fillMultiplicities(eventData,event,setup);
  fillXiInfo(eventData,event,setup);
  fillPFFlowInfo(eventData,event,setup);
   
  //


}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillEventInfo(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  unsigned int eventNumber = event.id().event();
  unsigned int runNumber = event.id().run();
  unsigned int lumiSection = event.luminosityBlock();
  int bunchCrossing = event.bunchCrossing();
/////////////////////////////
 // Instant. luminosity of a lumisection = (delivered luminosity)/(lumisection size=23.36s) 
  edm::LuminosityBlock const& lumiBlock = event.getLuminosityBlock();
  edm::Handle<LumiSummary> s;
  lumiBlock.getByLabel("lumiProducer",s);
  float instLumiLS=-10.;
  if( s->isValid() ){
      instLumiLS=s->avgInsDelLumi(); // calibrated

  }
 // Instant. luminosity of a bunch crossing
  // In EDM  it is uncalibrated. For 7TeV collisions calibration constant is 6.37.
  edm::Handle<LumiDetails> d;
  lumiBlock.getByLabel("lumiProducer",d);
  float instLumiBunchOCC1=-10.;
  if (d->isValid()){
     instLumiBunchOCC1 = d->lumiValue(LumiDetails::kOCC1,event.bunchCrossing());
     instLumiBunchOCC1=instLumiBunchOCC1*6.37;
      //std::cout << "instLumiBunchOCC1 ="<<  instLumiBunchOCC1 <<std::endl;
     eventData.SetInstLumiBunch(instLumiBunchOCC1);
      //std::cout<<"lumivalue 1 "<< d->lumiValue(LumiDetails::kOCC1,1)*6.37<<" "<<d->lumiBeam1Intensity(1)<<std::endl;
  }else{
       std::cout << "no valid lumi detail data" <<std::endl;
       //eventData.SetInstLumiBunch(-9999.);
       }
/////////////////////////////
  edm::Handle<double> lumiWeight;
  event.getByLabel("lumiWeight",lumiWeight);
  if(lumiWeight.isValid()){
     double lumi = *lumiWeight;
     eventData.SetLumiWeight(lumi);
  } else{
     eventData.SetLumiWeight(-1.); 
  }

  eventData.SetEventNumber(eventNumber);
  eventData.SetRunNumber(runNumber);
  eventData.SetLumiSection(lumiSection);
  eventData.SetBunchCrossing(bunchCrossing);
  eventData.SetInstDelLumiLS(instLumiLS); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillTriggerInfo(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
 

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
  
 
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillPileUpInfo(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
//Number of pile-up events

  edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
  event.getByLabel(edm::InputTag("addPileupInfo"), PupInfo);

  std::vector<PileupSummaryInfo>::const_iterator PVI;

  int nm1 = -1; int n0 = -1; int np1 = -1;
  for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {

     int BX = PVI->getBunchCrossing();

     if(BX == -1) { 
       nm1 = PVI->getPU_NumInteractions();
     }
     if(BX == 0) { 
       n0 = PVI->getPU_NumInteractions();
     }
     if(BX == 1) { 
       np1 = PVI->getPU_NumInteractions();
     }

  }
  //eventData.SetMyWeight3D = LumiWeights_.weight3D( nm1,n0,np1);
  eventData.SetNPileUpBxm1(nm1);
  eventData.SetNPileUpBx0(n0);
  eventData.SetNPileUpBxp1(np1);

/////////////////////////////////////////////

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillVertexInfo(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  edm::Handle<edm::View<reco::Vertex> > vertexCollectionH;
  event.getByLabel(vertexTag_,vertexCollectionH);
  const edm::View<reco::Vertex>& vtxColl = *(vertexCollectionH.product());

  // Find number of good vertices
  int nGoodVertices = 0;
  for(edm::View<reco::Vertex>::const_iterator vtx = vtxColl.begin(); vtx != vtxColl.end(); ++vtx){
     if(!vtx->isValid()) continue; // skip non-valid vertices
     if(vtx->isFake()) continue; // skip vertex from beam spot
     ++nGoodVertices;
  } 
  eventData.SetNVertex(nGoodVertices);


}
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillJetInfo(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<edm::View<reco::Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);

  edm::Handle<edm::View<reco::PFCandidate> > particleFlowCollectionH;
  event.getByLabel(particleFlowTag_,particleFlowCollectionH);

  if(jetCollectionH->size() > 1){
     const reco::Jet& jet1 = (*jetCollectionH)[0];// they come out ordered right?
     const reco::Jet& jet2 = (*jetCollectionH)[1];

     eventData.SetLeadingJetPt(jet1.pt());
   
     eventData.SetLeadingJetEta(jet1.eta());
     eventData.SetLeadingJetPhi(jet1.phi());
     eventData.SetLeadingJetP4(jet1.p4());
 
     eventData.SetSecondJetPt(jet2.pt());
    
     eventData.SetSecondJetEta(jet2.eta());
     
     eventData.SetSecondJetPhi(jet2.phi());
     //Using Lorentz Vector
     eventData.SetSecondJetP4(jet2.p4());
  
     eventData.SetJetsAveEta((jet1.eta() + jet2.eta())/2);
     eventData.SetJetsDeltaEta(jet1.eta() - jet2.eta());
     eventData.SetJetsDeltaPhi(M_PI - fabs(jet1.phi() - jet2.phi()));
     eventData.SetJetsDeltaPt(fabs(jet1.pt() - jet2.pt()));

     //Jet mass added by eliza
     math::XYZTLorentzVector jetSystem(0.,0.,0.,0.);
     jetSystem += jet1.p4();
     eventData.SetMassJets(jetSystem.M());
     // Di-jet mass
     math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
     dijetSystem += jet1.p4();
     dijetSystem += jet2.p4();
     eventData.SetMassDijets(dijetSystem.M());  
  
     // M_{X}
     math::XYZTLorentzVector allJets(0.,0.,0.,0.);
     for(edm::View<reco::Jet>::const_iterator jet = jetCollectionH->begin();
                                              jet != jetCollectionH->end(); ++jet) allJets += jet->p4();
  
     eventData.SetMxFromJets(allJets.M());

     math::XYZTLorentzVector allPFCands(0.,0.,0.,0.);
     for(edm::View<reco::PFCandidate>::const_iterator pfCand = particleFlowCollectionH->begin();
                                                      pfCand != particleFlowCollectionH->end();
                                                      ++pfCand) allPFCands += pfCand->p4();


     eventData.SetMxFromPFCands(allPFCands.M());

// Defense Added by Diego     
     if(jetCollectionH->size() > 1){
     double RjjFromJets = Rjj(*jetCollectionH,*jetCollectionH);
     eventData.SetRjjFromJets(RjjFromJets);
     }
     else eventData.SetRjjFromJets(-1);
///////////////////////////////////////////

// Defense Added by Diego 
     if(jetCollectionH->size() > 0){
     double RjFromJets = Rj(*jetCollectionH,*jetCollectionH);
     eventData.SetRjFromJets(RjFromJets);
     }
     else eventData.SetRjFromJets(-1);
///////////////////////////////////////////

     edm::Handle<edm::View<reco::Jet> > jetCollectionNonCorrH;
     event.getByLabel(jetNonCorrTag_,jetCollectionNonCorrH);

// Defense Added by Diego
     if(jetCollectionNonCorrH->size() > 0){
     double RjFromPFCands = Rj(*jetCollectionNonCorrH,*particleFlowCollectionH);
     eventData.SetRjFromPFCands(RjFromPFCands);
     }
     else eventData.SetRjFromPFCands(-1);
//////////////////////////////////////////////

// Defense Added by Diego     
     if(jetCollectionNonCorrH->size() > 1){
     double RjjFromPFCands = Rjj(*jetCollectionNonCorrH,*particleFlowCollectionH);
     eventData.SetRjjFromPFCands(RjjFromPFCands);
     }
     else eventData.SetRjjFromPFCands(-1);
//////////////////////////////////////////////

     //eventData.SetRjFromJets(RjFromJets);
     //eventData.SetRjFromPFCands(RjFromPFCands);
     //eventData.SetRjjFromJets(RjjFromJets);
     //eventData.SetRjjFromPFCands(RjjFromPFCands);

     if(usePAT_){
        const pat::Jet* patJet1 = dynamic_cast<const pat::Jet*>(&jet1);
        const pat::Jet* patJet2 = dynamic_cast<const pat::Jet*>(&jet2);
      
        if(!patJet1 || !patJet2) throw edm::Exception(edm::errors::Configuration) << "Expecting PATJet's as input";

        if(!runOnData_){
           const reco::GenJet* genJet1 = patJet1->genJet();
           const reco::GenJet* genJet2 = patJet2->genJet();
           if(genJet1&&genJet2){
              math::XYZTLorentzVector dijetGenSystem(0.,0.,0.,0.);
              dijetGenSystem += genJet1->p4();
              dijetGenSystem += genJet2->p4();
              double massGen = dijetGenSystem.M();
              eventData.SetMassDijetsGen(massGen);
           }
        } else{
           eventData.SetMassDijetsGen(-999.);
        }

        // B-tagging
        if(doBtag_){
           double bDiscJet1 = patJet1->bDiscriminator(bDiscriminatorName_);
           double bDiscJet2 = patJet2->bDiscriminator(bDiscriminatorName_);

           eventData.SetLeadingJetBDiscriminator(bDiscJet1);
           eventData.SetSecondJetBDiscriminator(bDiscJet2);
        }

     } else {
        // Access info from outside PAT here
        eventData.SetMassDijetsGen(-999.);
        eventData.SetLeadingJetBDiscriminator(-999.);
        eventData.SetSecondJetBDiscriminator(-999.);
     }

  } else{
     eventData.SetLeadingJetPt(-999.);
     eventData.SetSecondJetPt(-999.);
     eventData.SetLeadingJetEta(-999.);
     eventData.SetSecondJetEta(-999.);
     eventData.SetLeadingJetPhi(-999.);
     eventData.SetSecondJetPhi(-999.);
    
     eventData.SetJetsAveEta(-999.);
     eventData.SetJetsDeltaEta(-999.);
     eventData.SetJetsDeltaPhi(-999.);
     eventData.SetJetsDeltaPt(-999.);
  }

 
  
  if(jetCollectionH->size() > 2){
    const reco::Jet& jet1 = (*jetCollectionH)[0];
    const reco::Jet& jet2 = (*jetCollectionH)[1];
    const reco::Jet& jet3 = (*jetCollectionH)[2];
   
    
    eventData.SetThirdJetPt(jet3.pt());
    eventData.SetThirdJetEta(jet3.eta());
    eventData.SetThirdJetPhi(jet3.phi()); //added by Diego
    ///Using Lorentz Vector
    eventData.SetThirdJetP4(jet3.p4());

    //begin of things added by eliza
    //Tri-jet mass
    math::XYZTLorentzVector trijetSystem(0.,0.,0.,0.);
    trijetSystem += jet1.p4();
    trijetSystem += jet2.p4();
    trijetSystem += jet3.p4();
    eventData.SetMassTrijets(trijetSystem.M());
   }else{
    eventData.SetThirdJetPt(-999.);
    eventData.SetThirdJetEta(-999.);
    eventData.SetThirdJetPhi(-999.); // added by Diego
    eventData.SetMassTrijets(-999.);
   } 

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillMultiplicities(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  //////////////////////////////////////////////////////////////////////////////////////
  // Access multiplicities
  edm::Handle<unsigned int> trackMultiplicity; 
  event.getByLabel(trackMultiplicityTag_,trackMultiplicity); 

  unsigned int nTracks = *trackMultiplicity;
  eventData.SetTrackMultiplicity(nTracks);

}
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillXiInfo(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  if(accessMCInfo_){
     // Gen particles
     edm::Handle<reco::GenParticleCollection> genParticlesCollectionH;
     event.getByLabel("genParticles",genParticlesCollectionH);
     const reco::GenParticleCollection& genParticles = *genParticlesCollectionH;   

     math::XYZTLorentzVector genAllParticles(0.,0.,0.,0.),
                             genAllParticlesInRange(0.,0.,0.,0.),
                             genAllParticlesHEPlus(0.,0.,0.,0.),genAllParticlesHEMinus(0.,0.,0.,0.),
                             genAllParticlesHFPlus(0.,0.,0.,0.),genAllParticlesHFMinus(0.,0.,0.,0.),
                             genEtaMax(0.,0.,0.,0.),genEtaMin(0.,0.,0.,0.),
                             genProtonPlus(0.,0.,0.,0.),genProtonMinus(0.,0.,0.,0.);

     setGenInfo(genParticles,Ebeam_,genAllParticles,
                                    genAllParticlesInRange,
                                    genAllParticlesHEPlus,genAllParticlesHEMinus,
                                    genAllParticlesHFPlus,genAllParticlesHFMinus,
                                    genEtaMax,genEtaMin, 
                                    genProtonPlus,genProtonMinus);

     double xigen_plus = -1.;
     double xigen_minus = -1.;
    
     xigen_plus = 1 - genProtonPlus.pz()/Ebeam_;
     xigen_minus = 1 + genProtonMinus.pz()/Ebeam_;

     LargestGenRapidityGap largestGenGap(-999.,999.);
     math::XYZTLorentzVector genGapLowEdge(0.,0.,0.,0.),genGapHighEdge(0.,0.,0.,0.);
     largestGenGap(genParticles,genGapLowEdge,genGapHighEdge);

     double massDissGenPlus = (genGapHighEdge == math::XYZTLorentzVector(0.,0.,0.,0.)) ? -999. : MassDissGen(genParticles,genGapHighEdge.eta(),999.);
     double massDissGenMinus = (genGapLowEdge == math::XYZTLorentzVector(0.,0.,0.,0.)) ? -999. : MassDissGen(genParticles,-999.,genGapLowEdge.eta());

     double deltaEtaGen = 0.;
     if(genGapHighEdge == math::XYZTLorentzVector(0.,0.,0.,0.) ||
        genGapLowEdge == math::XYZTLorentzVector(0.,0.,0.,0.)) deltaEtaGen = -999.;
     else deltaEtaGen = ( genGapHighEdge.eta() - genGapLowEdge.eta() ); 

     double etaGapGenLow = (genGapLowEdge == math::XYZTLorentzVector(0.,0.,0.,0.)) ? -999. : genGapLowEdge.eta();
     double etaGapGenHigh = (genGapHighEdge == math::XYZTLorentzVector(0.,0.,0.,0.)) ? -999. : genGapHighEdge.eta();

     LogDebug("Analysis") << "Gap low,high = " << genGapLowEdge << " , " << genGapHighEdge;
     
     eventData.SetXiGenPlus(xigen_plus);
     eventData.SetXiGenMinus(xigen_minus);
     eventData.SetMxGen(genAllParticles.mass());
     eventData.SetMxGenRange(genAllParticlesInRange.mass()); 
     eventData.SetSumEnergyHEPlusGen(genAllParticlesHEPlus.energy());
     eventData.SetSumEnergyHEMinusGen(genAllParticlesHEMinus.energy());
     eventData.SetSumEnergyHFPlusGen(genAllParticlesHFPlus.energy());
     eventData.SetSumEnergyHFMinusGen(genAllParticlesHFMinus.energy());
     eventData.SetEtaMaxGen(genEtaMax.eta());
     eventData.SetEtaMinGen(genEtaMin.eta());

     eventData.SetDeltaEtaGen(deltaEtaGen);
     eventData.SetEtaGapLow(etaGapGenLow);
     eventData.SetEtaGapHigh(etaGapGenHigh);
     eventData.SetMxGenPlus(massDissGenPlus);
     eventData.SetMxGenMinus(massDissGenMinus);
 
     // Access variables from event 
     edm::Handle<std::vector<float> > edmNtupleMxGen;
     event.getByLabel(edm::InputTag("edmNtupleMxGen","Mx"),edmNtupleMxGen);
 
     eventData.SetMxGenDiss((edmNtupleMxGen.isValid() && edmNtupleMxGen->size()) ? (*edmNtupleMxGen)[0] : -999.);

     edm::Handle<std::vector<float> > edmNtupleEtaMaxGen;
     event.getByLabel(edm::InputTag("edmNtupleEtaMaxGen","etaMax"),edmNtupleEtaMaxGen);

     edm::Handle<std::vector<float> > edmNtupleEtaMinGen;
     event.getByLabel(edm::InputTag("edmNtupleEtaMinGen","etaMin"),edmNtupleEtaMinGen);

     eventData.SetEtaMaxGenNew((edmNtupleEtaMaxGen.isValid() && edmNtupleEtaMaxGen->size()) ? (*edmNtupleEtaMaxGen)[0] : -999.);
     eventData.SetEtaMinGenNew((edmNtupleEtaMinGen.isValid() && edmNtupleEtaMinGen->size()) ? (*edmNtupleEtaMinGen)[0] : -999.);
  } else{
     eventData.SetXiGenPlus(-1.);
     eventData.SetXiGenMinus(-1.);
     eventData.SetMxGen(-1.);
     eventData.SetMxGenDiss(-1.);
     eventData.SetMxGenRange(-1.);
     eventData.SetMxGenPlus(-1.); 
     eventData.SetMxGenMinus(-1.);
     eventData.SetDeltaEtaGen(-1.); 
     eventData.SetEtaGapLow(-999.);
     eventData.SetEtaGapHigh(-999.);
     eventData.SetSumEnergyHEPlusGen(-1.);
     eventData.SetSumEnergyHEMinusGen(-1.);
     eventData.SetSumEnergyHFPlusGen(-1.);
     eventData.SetSumEnergyHFMinusGen(-1.);
     eventData.SetEtaMaxGen(-999.);
     eventData.SetEtaMinGen(-999.);
     eventData.SetEtaMaxGenNew(-999.);
     eventData.SetEtaMinGenNew(-999.);
  }



  edm::Handle<reco::PFCandidateCollection> particleFlowCollectionH;
  event.getByLabel(particleFlowTag_,particleFlowCollectionH);

 
  double MxFromPFCands = MassColl(*particleFlowCollectionH,thresholdsPFlow_);
 
  eventData.SetMxFromPFCands(MxFromPFCands);

 
  std::pair<double,double> xiFromPFCands = xi(*particleFlowCollectionH,Ebeam_,thresholdsPFlow_);
  double xiFromPFCands_plus = xiFromPFCands.first;
  double xiFromPFCands_minus = xiFromPFCands.second;

  
  eventData.SetXiPlusFromPFCands(xiFromPFCands_plus);
  eventData.SetXiMinusFromPFCands(xiFromPFCands_minus);

 
  std::pair<double,double> EPlusPzFromPFCands = EPlusPz(*particleFlowCollectionH,thresholdsPFlow_);
  eventData.SetEPlusPzFromPFCands(EPlusPzFromPFCands.first);
  eventData.SetEMinusPzFromPFCands(EPlusPzFromPFCands.second);

  
  double missingMassFromXiFromPFCands = 2*Ebeam_*sqrt(xiFromPFCands_plus*xiFromPFCands_minus);
  eventData.SetMissingMassFromXiFromPFCands(missingMassFromXiFromPFCands);

  std::pair<double,double> etaMaxFromPFCands = etaMax(*particleFlowCollectionH,thresholdsPFlow_);
  eventData.SetEtaMaxFromPFCands(etaMaxFromPFCands.first);
  eventData.SetEtaMinFromPFCands(etaMaxFromPFCands.second);

  edm::Handle<std::vector<float> > edmNtupleEtaMax;
  event.getByLabel(edm::InputTag("edmNtupleEtaMax","etaMax"),edmNtupleEtaMax);

  edm::Handle<std::vector<float> > edmNtupleEtaMin;
  event.getByLabel(edm::InputTag("edmNtupleEtaMin","etaMin"),edmNtupleEtaMin);

 

  float etaMax_pfCands = edmNtupleEtaMax->size() ? (*edmNtupleEtaMax)[0] : -999.;
  float etaMin_pfCands = edmNtupleEtaMin->size() ? (*edmNtupleEtaMin)[0] : -999.; 
  eventData.SetEtaMaxFromPFCandsNew(etaMax_pfCands);
  eventData.SetEtaMinFromPFCandsNew(etaMin_pfCands);


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////
void ExclusiveDijetsAnalysis::fillCastorInfo(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  std::vector<int> modules;
  modules.push_back(1);
  modules.push_back(2);
  modules.push_back(3);
  modules.push_back(4);
  modules.push_back(5);
   
  edm::Handle<CastorRecHitCollection> castorRecHitCollectionH;
  event.getByLabel(castorRecHitTag_,castorRecHitCollectionH);
  CastorRecHitCollection const& castorRecHitCollection = *castorRecHitCollectionH;  

  double sumETotCastor = CastorEnergy(modules)(castorRecHitCollection,runOnData_);
  eventData.SetSumETotCastor(sumETotCastor); 
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillPFFlowInfo(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  edm::Handle<reco::PFCandidateCollection> particleFlowCollectionH;
  event.getByLabel(particleFlowTag_,particleFlowCollectionH);
  reco::PFCandidateCollection const& pflowCollection = *particleFlowCollectionH;

  reco::PFCandidateCollection::const_iterator part = pflowCollection.begin();
  reco::PFCandidateCollection::const_iterator pfCands_end = pflowCollection.end();
  double sumEHFMinus = 0.;
  double sumEHFPlus  = 0.;
  for(; part != pfCands_end; ++part){
     int partType = part->particleId();
     double eta = part->eta();
     double energy = part->energy();
     double pt = part->pt();
     
     if((3.0 < eta) && (eta < 5.0) ){
        sumEHFPlus += energy;
     }
     if((-5.0 < eta) && (eta < -3.0) ){
        sumEHFMinus += energy;
     }
     
     eventData.SetEtaAllTypes(eta); 
     eventData.SetEnergyAllTypes(energy); 
     eventData.SetPtAllTypes(pt);

     if(partType == reco::PFCandidate::X){
        eventData.SetEtaUndefined(eta); 
        eventData.SetEnergyUndefined(energy); 
        eventData.SetPtUndefined(pt); 
        
     }else if(partType == reco::PFCandidate::h){
        eventData.SetEtaChargedHadron(eta); 
        eventData.SetEnergyChargedHadron(energy);
        eventData.SetPtChargedHadron(pt);  

     }else if(partType == reco::PFCandidate::e){ 
        eventData.SetEtaElectron(eta); 
        eventData.SetEnergyElectron(energy);
        eventData.SetPtElectron(pt);
                
     }else if(partType == reco::PFCandidate::mu){ 
        eventData.SetEtaMuon(eta); 
        eventData.SetEnergyMuon(energy);
        eventData.SetPtMuon(pt);
        
        
     }else if(partType == reco::PFCandidate::gamma){ 
        eventData.SetEtaGamma(eta); 
        eventData.SetEnergyGamma(energy);
        eventData.SetPtGamma(pt);
        
     }else if(partType == reco::PFCandidate::h0){ 
        eventData.SetEtaNeutralHadron(eta); 
        eventData.SetEnergyNeutralHadron(energy);
        eventData.SetPtNeutralHadron(pt);
        
     }else if(partType == reco::PFCandidate::h_HF){ 
        eventData.SetEtaHadronHF(eta); 
        eventData.SetEnergyHadronHF(energy);
        eventData.SetPtHadronHF(pt);
        

     }else if(partType == reco::PFCandidate::egamma_HF){ 
        eventData.SetEtaEGammaHF(eta); 
        eventData.SetEnergyEGammaHF(energy);
        eventData.SetPtEGammaHF(pt); 
     }
  }

  eventData.SetSumEHFPFlowPlus(sumEHFPlus);
  eventData.SetSumEHFPFlowMinus(sumEHFMinus);
} 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//template <class OneJetColl,class OnePartColl>
//added by eliza, maybe would be better to created only one R and then ask for jet,di-jet, tri-jet...
template <class OneJetColl,class OnePartColl>
double ExclusiveDijetsAnalysis::Rj(OneJetColl& jetCollection,OnePartColl& partCollection){
  math::XYZTLorentzVector jetSystem(0.,0.,0.,0.);
  jetSystem += (jetCollection[0]).p4();
  
  math::XYZTLorentzVector allCands(0.,0.,0.,0.);
  for(typename OnePartColl::const_iterator part = partCollection.begin();
                                        part != partCollection.end(); ++part) allCands += part->p4();

  return allCands.M() > 0. ? (jetSystem.M()/allCands.M()) : -1.;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class JetColl,class PartColl>
double ExclusiveDijetsAnalysis::Rjj(JetColl& jetCollection,PartColl& partCollection){

  math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
  dijetSystem += (jetCollection[0]).p4();
  dijetSystem += (jetCollection[1]).p4();

  math::XYZTLorentzVector allCands(0.,0.,0.,0.);
  for(typename PartColl::const_iterator part = partCollection.begin();
                                        part != partCollection.end(); ++part) allCands += part->p4();

  return allCands.M() > 0. ? (dijetSystem.M()/allCands.M()) : -1.;
    
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int ExclusiveDijetsAnalysis::nHFSlice(const std::map<unsigned int, std::vector<unsigned int> >& mapTreshToiEta, unsigned int thresholdHF, unsigned int ieta){
  const std::vector<unsigned int>& vec_iEta = mapTreshToiEta.find(thresholdHF)->second;

  // Count number of ieta entries in vector 
  int count_ieta = (int)std::count(vec_iEta.begin(),vec_iEta.end(),ieta);

  return count_ieta;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::resetPFThresholds(std::map<int,std::pair<double,double> >& thresholdsPFlow){
  thresholdsPFlow[reco::PFCandidate::X] = std::make_pair(-1.,-1.);
  thresholdsPFlow[reco::PFCandidate::h] = std::make_pair(-1.,-1.);
  thresholdsPFlow[reco::PFCandidate::e] = std::make_pair(-1.,-1.);
  thresholdsPFlow[reco::PFCandidate::mu] = std::make_pair(-1.,-1.);
  thresholdsPFlow[reco::PFCandidate::gamma] = std::make_pair(-1.,-1.);
  thresholdsPFlow[reco::PFCandidate::h0] = std::make_pair(-1.,-1.);
  thresholdsPFlow[reco::PFCandidate::h_HF] = std::make_pair(-1.,-1.);
  thresholdsPFlow[reco::PFCandidate::egamma_HF] = std::make_pair(-1.,-1.);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::setPFThresholds(std::map<int,std::pair<double,double> >& thresholdsPFlow, edm::ParameterSet const& thresholdsPFPset){
  std::vector<std::string> pfThresholdNames = thresholdsPFPset.getParameterNames();
  std::vector<std::string>::const_iterator param = pfThresholdNames.begin();
  std::vector<std::string>::const_iterator params_end = pfThresholdNames.end();
  for(; param != params_end; ++param){
     //reco::PFCandidate::ParticleType particleType = pflowId(*param);
     int particleType = pflowId(*param);
     if(particleType == -1)
        throw cms::Exception("Configuration") << "Parameter " << *param
                                              << " does not correspond to any particle type (PF)";

     edm::ParameterSet const& typePSet = thresholdsPFPset.getParameter<edm::ParameterSet>(*param);
     double ptThreshold = -1.;
     if(typePSet.exists("pt")) ptThreshold = typePSet.getParameter<double>("pt");
     double energyThreshold = -1.;
     if(typePSet.exists("energy")) energyThreshold = typePSet.getParameter<double>("energy");
     thresholdsPFlow[particleType].first = ptThreshold;
     thresholdsPFlow[particleType].second = energyThreshold;
  }
}  
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
