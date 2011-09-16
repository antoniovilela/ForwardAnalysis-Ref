
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsAnalysis.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h" //eliza

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
#include "DataFormats/Math/interface/LorentzVector.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "FWCore/Utilities/interface/RegexMatch.h"
#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <map>

//end 
//---------------------------------------------------------------------------

using namespace forwardAnalysis;
using exclusiveDijetsAnalysis::ExclusiveDijetsAnalysis;

ExclusiveDijetsAnalysis::ExclusiveDijetsAnalysis(const edm::ParameterSet& pset):
  runOnData_(true), 
  vertexTag_(pset.getParameter<edm::InputTag>("VertexTag")),//ok
  trackTag_(pset.getParameter<edm::InputTag>("TrackTag")),//old My dijets analysis
  metTag_(pset.getParameter<edm::InputTag>("METTag")),//old My dijets analysis
  jetTag_(pset.getParameter<edm::InputTag>("JetTag")),//ok
  jetNonCorrTag_(pset.getParameter<edm::InputTag>("JetNonCorrTag")), //ok
  particleFlowTag_(pset.getParameter<edm::InputTag>("ParticleFlowTag")),//ok
  castorRecHitTag_(pset.getParameter<edm::InputTag>("CastorRecHitTag")), //added by liz
  genChargedTag_(pset.getParameter<edm::InputTag>("GenChargedParticlesTag")),//old My dijets analysis
  triggerResultsTag_(pset.getParameter<edm::InputTag>("TriggerResultsTag")),//old My dijets analysis
  //hcalTowerSummaryTag_(pset.getParameter<edm::InputTag>("HCALTowerSummaryTag")),//old My dijets analysis
  trackMultiplicityTag_(pset.getParameter<edm::InputTag>("TrackMultiplicityTag")),//ok
  //doBtag_(pset.getParameter<bool>("DoBTag")),
  energyThresholdHB_(pset.getParameter<double>("EnergyThresholdHB")),//old My dijets analysis
  energyThresholdHE_(pset.getParameter<double>("EnergyThresholdHE")),//old My dijets analysis
  energyThresholdHF_(pset.getParameter<double>("EnergyThresholdHF")),//old My dijets analysis
  thresholdHF_(pset.getParameter<unsigned int>("HFThresholdIndex")),//ok
  applyEnergyScaleHCAL_(pset.getParameter<bool>("ApplyEnergyScaleHCAL")),//ok
  energyScaleHCAL_(-1.),//old My dijets analysis
  useJetCorrection_(pset.getParameter<bool>("UseJetCorrection")),//ok
  accessPileUpInfo_(pset.getParameter<bool>("AccessPileUpInfo")),//ok
  Ebeam_(pset.getUntrackedParameter<double>("EBeam",3500.)),//ok
  usePAT_(pset.getUntrackedParameter<bool>("UsePAT",true)),
  accessMCInfo_(pset.getUntrackedParameter<bool>("AccessMCInfo",true)),//old My dijets analysis
  POMPYTMCInfo_(pset.getUntrackedParameter<bool>("POMPYTMCInfo",true)),//old My dijets analysis
  //runOnData_(pset.getUntrackedParameter<bool>("RunOnData",true)), 
  //hltPathName1_(pset.getParameter<std::string>("HLTPath1")),//old My dijets analysis
  //hltPathName2_(pset.getParameter<std::string>("HLTPath2")),//old My dijets analysis
  //hltPathName3_(pset.getParameter<std::string>("HLTPath3"))//,//old My dijets analysis
  //hltPathName4_(pset.getParameter<std::string>("HLTPath4")),//old My dijets analysis
  //hltPathName5_(pset.getParameter<std::string>("HLTPath5")),//old My dijets analysis
  //hltPathName6_(pset.getParameter<std::string>("HLTPath6")) //old My dijets analysis
  hltPathNames_(pset.getParameter<std::vector<std::string> >("hltPaths")) {
  
  resetPFThresholds(thresholdsPFlowBarrel_);
  resetPFThresholds(thresholdsPFlowEndcap_);
  resetPFThresholds(thresholdsPFlowTransition_);
  resetPFThresholds(thresholdsPFlowForward_);
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
 //  if(doBtag_){
//      bDiscriminatorName_ = pset.getParameter<std::string>("BDiscriminatorName");
//   }  
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ExclusiveDijetsAnalysis::~ExclusiveDijetsAnalysis(){}

void ExclusiveDijetsAnalysis::setBeginRun(const edm::Run& run, const edm::EventSetup& setup){
  if(useJetCorrection_) corrector_ = JetCorrector::getJetCorrector(jetCorrectionService_,setup);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*void ExclusiveDijetsAnalysis::setGenInfo(const edm::Event& event, const edm::EventSetup& setup){

  if (accessMCInfo_){
  // Gen particles
  edm::Handle<edm::View<reco::GenParticle> > genParticlesCollectionH;
  event.getByLabel("genParticles",genParticlesCollectionH);

  math::XYZTLorentzVector allGenParticles(0.,0.,0.,0.);
  edm::View<reco::GenParticle>::const_iterator proton_plus = genParticlesCollectionH->end();
  edm::View<reco::GenParticle>::const_iterator proton_minus = genParticlesCollectionH->end();
  for(edm::View<reco::GenParticle>::const_iterator genpart = genParticlesCollectionH->begin();
                                                   genpart != genParticlesCollectionH->end(); ++genpart){
     if(genpart->status() != 1) continue;
     allGenParticles += genpart->p4();
  
     double pz = genpart->pz();
     if((proton_plus == genParticlesCollectionH->end())&&(genpart->pdgId() == 2212)&&(pz > 0.75*Ebeam_)) proton_plus = genpart;
     else if((proton_minus == genParticlesCollectionH->end())&&(genpart->pdgId() == 2212)&&(pz < -0.75*Ebeam_)) proton_minus = genpart;
  }
  // Define all particles's system subtracting the scattered protons (makes sense in diffractive/elastic topologies)
  if(proton_plus != genParticlesCollectionH->end()) allGenParticles -= proton_plus->p4();
  if(proton_minus != genParticlesCollectionH->end()) allGenParticles -= proton_minus->p4();
  
  if(proton1 != genParticlesCollectionH->end()){
     LogTrace("Analysis") << "Proton (z-plus): " << proton1->pt() << "  " << proton1->eta() << "  " << proton1->phi() << std::endl;
     xigen_plus = 1 - proton1->pz()/Ebeam_;
     allGenParticles -= proton1->p4();
  }
  if(proton2 != genParticlesCollectionH->end()){
     LogTrace("Analysis") << "Proton (z-minus): " << proton2->pt() << "  " << proton2->eta() << "  " << proton2->phi() << std::endl;
     xigen_minus = 1 + proton2->pz()/Ebeam_;
     allGenParticles -= proton2->p4();
  }*/

  /*if(proton_plus != genParticlesCollectionH->end()) genProtonPlus_ = proton_plus->p4();
  if(proton_minus != genParticlesCollectionH->end()) genProtonMinus_ = proton_minus->p4(); 
  genAllParticles_ = allGenParticles;

  }
}*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillEventData(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  // Reset info
  eventData.reset();

  runOnData_ = event.isRealData();
  // Set internal info
  //if(!runOnData_) setGenInfo(event,setup);

  // Fill event data
  if(!runOnData_ && accessPileUpInfo_){
     fillPileUpInfo(eventData,event,setup);
  } else {
     eventData.nPileUpBx0_ = -1;
  }

  //added by eliza 
  fillEventInfo(eventData,event,setup);
  fillTriggerInfo(eventData,event,setup);
  fillVertexInfo(eventData,event,setup);
  fillJetInfo(eventData,event,setup);
  fillMultiplicities(eventData,event,setup);
  fillXiInfo(eventData,event,setup);
  fillPFFlowInfo(eventData,event,setup);
   
  //


}
//new information: (eliza)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillEventInfo(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  unsigned int eventNumber = event.id().event();
  unsigned int runNumber = event.id().run();
  unsigned int lumiSection = event.luminosityBlock();
  int bunchCrossing = event.bunchCrossing();

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
   
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillTriggerInfo(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  //FIXME
  //if(!accessMCInfo_){

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
  }
  
  /*std::string hltPath2;
  std::string hltPath3;
 
  // In case hltPathName2_ is a pattern (e.g. HLT_Jet30U*)
  if( edm::is_glob(hltPathName2_) ){
     std::vector< std::vector<std::string>::const_iterator > matches = edm::regexMatch(triggerNames.triggerNames(), hltPathName2_);  

     if( matches.empty() ) throw cms::Exception("Configuration") << "Could not find any HLT path of type " << hltPathName2_ << "\n";
     else if( matches.size() > 1) throw cms::Exception("Configuration") << "HLT path type " << hltPathName2_ << " not unique\n";
     else hltPath2 = *(matches[0]);
  } else{
     hltPath2 = hltPathName2_; 
  } 
  std::cout << "Using HLT path2 " << hltPath2 << std::endl; 
   //----------------------------------------------------------
   // In case hltPathName3_ is a pattern (e.g. HLT_Jet30U*)
  if( edm::is_glob(hltPathName3_) ){
     std::vector< std::vector<std::string>::const_iterator > matches = edm::regexMatch(triggerNames.triggerNames(), hltPathName3_);  

     if( matches.empty() ) throw cms::Exception("Configuration") << "Could not find any HLT path of type " << hltPathName3_ << "\n";
     else if( matches.size() > 1) throw cms::Exception("Configuration") << "HLT path type " << hltPathName3_ << " not unique\n";
     else hltPath3 = *(matches[0]);
  } else{
     hltPath3 = hltPathName3_; 
  } 
  std::cout << "Using HLT path3 :" << hltPath3 << std::endl; 
  //--------------------------------------------------------
   
  int idx_HLT_Jet15U = triggerNames.triggerIndex(hltPath2);
  int idx_HLT_L1Jet6U = triggerNames.triggerIndex(hltPath3);
  
  eventData.SetHLT_Jet15U((triggerResults->wasrun(idx_HLT_Jet15U) && triggerResults->accept(idx_HLT_Jet15U)) ? 1 : 0);
  eventData.SetHLT_L1Jet6U((triggerResults->wasrun(idx_HLT_L1Jet6U) && triggerResults->accept(idx_HLT_L1Jet6U)) ? 1 : 0);*/
}
// end new information (eliza): eventdatainfo & trigger

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillPileUpInfo(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
//  edm::Handle<std::map<int,int> > pileUpMap;
//   event.getByLabel("pileUpInfo",pileUpMap);
//   std::map<int,int>::const_iterator bx0Iter = pileUpMap->find(0);
//   if(bx0Iter == pileUpMap->end()){
//      edm::LogError("Analysis") << ">>> Pile-up info does not contain bunch crossing 0 ..skipping";
//      return;
//   }
//   int nPileUpBx0 = bx0Iter->second;
//   LogTrace("Analysis") << "  Number of pile-up events in bunch crossing 0: " << nPileUpBx0;
//   //eventData.nPileUpBx0_ = nPileUpBx0;
//     eventData.SetNPileUpBx0(nPileUpBx0);
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

  /*const reco::Vertex& primVertex = (*vertexCollectionH)[0];
  if(primVertex.isValid() && !primVertex.isFake()){
     eventData.SetposXPrimVtx(primVertex.x());
     eventData.SetposYPrimVtx(primVertex.y());
     eventData.SetposZPrimVtx(primVertex.z());
  } else{
     eventData.SetposXPrimVtx(-999.);
     eventData.SetposYPrimVtx(-999.);
     eventData.SetposZPrimVtx(-999.);
  }*/

  //eventData.nVertex_ = nGoodVertices;
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
     //const reco::Jet& jet3 = (*jetCollectionH)[2];

     eventData.SetLeadingJetPt(jet1.pt());
     //std::cout << "Leading Jet pt: " << jet1.pt() << std::endl;
     eventData.SetLeadingJetEta(jet1.eta());
     eventData.SetLeadingJetPhi(jet1.phi());
     eventData.SetLeadingJetP4(jet1.p4());
 
     eventData.SetSecondJetPt(jet2.pt());
     //std::cout << "Second Jet pt: " << jet2.pt() << std::endl;
     eventData.SetSecondJetEta(jet2.eta());
     //std::cout << "Second Jet eta: " << jet2.eta() << std::endl;
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

     //Rj,Rjj & Rjj added by eliza
     double RjFromJets = Rj(*jetCollectionH,*jetCollectionH);
     double RjjFromJets = Rjj(*jetCollectionH,*jetCollectionH);
     //double RjjjFromJets = Rjjj(*jetCollectionH,*jetCollectionH);

     edm::Handle<edm::View<reco::Jet> > jetCollectionNonCorrH;
     event.getByLabel(jetNonCorrTag_,jetCollectionNonCorrH);

     double RjFromPFCands = Rj(*jetCollectionNonCorrH,*particleFlowCollectionH);
     double RjjFromPFCands = Rjj(*jetCollectionNonCorrH,*particleFlowCollectionH);
     //double RjjjFromPFCands = Rjjj(*jetCollectionNonCorrH,*particleFlowCollectionH);

     eventData.SetRjFromJets(RjFromJets);
     eventData.SetRjFromPFCands(RjFromPFCands);
     eventData.SetRjjFromJets(RjjFromJets);
     eventData.SetRjjFromPFCands(RjjFromPFCands);

     if(usePAT_){
        const pat::Jet* patJet1 = dynamic_cast<const pat::Jet*>(&jet1);
        const pat::Jet* patJet2 = dynamic_cast<const pat::Jet*>(&jet2);
        //const pat::Jet* patJet3 = dynamic_cast<const pat::Jet*>(&jet3);
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
     //eventData.SetLeadingJetP4(-999.);
     //eventData.SetSecondJetP4(-999.);
     eventData.SetJetsAveEta(-999.);
     eventData.SetJetsDeltaEta(-999.);
     eventData.SetJetsDeltaPhi(-999.);
     eventData.SetJetsDeltaPt(-999.);
  }

  //if(!runOnData_)  eventData.SetMxGen(genAllParticles_.M());  //eventData.MxGen_ = genAllParticles_.M();
  //else eventData.SetMxGen(-1.);        //eventData.MxGen_ = -1.;
  
  if(jetCollectionH->size() > 2){
    const reco::Jet& jet1 = (*jetCollectionH)[0];
    const reco::Jet& jet2 = (*jetCollectionH)[1];
    const reco::Jet& jet3 = (*jetCollectionH)[2];
    //eventData.thirdJetPt_ = jet3.pt();
    //eventData.thirdJetEta_ = jet3.eta();
    
    eventData.SetThirdJetPt(jet3.pt());
    eventData.SetThirdJetEta(jet3.eta());
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
//   //Multiplicity on HB
//   eventData.SetMultiplicityHBPlus(nHB_plus);
//   eventData.SetMultiplicityHBMinus(nHB_minus);
//   //Multiplicity on HE
//   eventData.SetMultiplicityHEPlus(nHE_plus);
//   eventData.SetMultiplicityHEMinus(nHE_minus);
//   //Multiplicity on HF
//   eventData.SetMultiplicityHFPlus(nHF_plus);
//   eventData.SetMultiplicityHFMinus(nHF_minus);
//   //Sum Energy on HB
//   eventData.SetSumEnergyHBPlus(sumEHB_plus);
//   eventData.SetSumEnergyHBMinus(sumEHB_minus);
//  //Sum Et on HB
//   eventData.SetSumETHBPlus(sumETHB_plus);
//   eventData.SetSumETHBMinus(sumETHB_minus);
//  //Sum Energy on HE
//   eventData.SetSumEnergyHEPlus(sumEHE_plus);
//   eventData.SetSumEnergyHEMinus(sumEHE_minus);
//  //Sum Et on HE
//    eventData.SetSumETHEPlus(sumETHE_plus);
//   eventData.SetSumETHEMinus(sumETHE_minus);
//  //Sum Et on HF
//   eventData.SetSumETHFPlus(sumETHF_plus);
//   eventData.SetSumETHFMinus(sumETHF_minus);
//  //Sum Energy on HF
//   eventData.SetSumEnergyHFPlus(sumEHF_plus);
//   eventData.SetSumEnergyHFMinus(sumEHF_minus);

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
     //if(genProtonPlus.pz() > 0.75*Ebeam_) xigen_plus = 1 - genProtonPlus.pz()/Ebeam_;
     //if(genProtonMinus.pz() < -0.75*Ebeam_) xigen_minus = 1 + genProtonMinus.pz()/Ebeam_;
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

  /*edm::Handle<edm::View<reco::Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);*/

  /*edm::Handle<CaloTowerCollection> caloTowerCollectionH;
  event.getByLabel(caloTowerTag_,caloTowerCollectionH);*/

  edm::Handle<reco::PFCandidateCollection> particleFlowCollectionH;
  event.getByLabel(particleFlowTag_,particleFlowCollectionH);

  //double energyScale = (applyEnergyScaleHCAL_) ? energyScaleHCAL_ : -1.; 
  //double MxFromJets = MassColl(*jetCollectionH,10.);
  //double MxFromTowers = analysisTools::MassColl(*caloTowerCollectionH,-1.,energyThresholdHB_,energyThresholdHE_,energyThresholdHF_,energyScale);
  double MxFromPFCands = MassColl(*particleFlowCollectionH,thresholdsPFlow_);
  //eventData.MxFromTowers_ = MxFromTowers;
  eventData.SetMxFromPFCands(MxFromPFCands);

  //std::pair<double,double> xiFromJets = xi(*jetCollectionH,Ebeam_,10.);
  //std::pair<double,double> xiFromTowers = analysisTools::xi(*caloTowerCollectionH,Ebeam_,-1.,energyThresholdHB_,energyThresholdHE_,energyThresholdHF_,energyScale);
  //double xiFromTowers_plus = xiFromTowers.first;
  //double xiFromTowers_minus = xiFromTowers.second;
  std::pair<double,double> xiFromPFCands = xi(*particleFlowCollectionH,Ebeam_,thresholdsPFlow_);
  double xiFromPFCands_plus = xiFromPFCands.first;
  double xiFromPFCands_minus = xiFromPFCands.second;

  //eventData.xiPlusFromTowers_ = xiFromTowers_plus;
  //eventData.xiMinusFromTowers_ = xiFromTowers_minus;
  //eventData.xiPlusFromJets_ = xiFromJets_plus;
  //eventData.xiMinusFromJets_ = xiFromJets_minus;
  eventData.SetXiPlusFromPFCands(xiFromPFCands_plus);
  eventData.SetXiMinusFromPFCands(xiFromPFCands_minus);

  /*std::pair<double,double> EPlusPzFromTowers = analysisTools::EPlusPz(*caloTowerCollectionH,-1.,energyThresholdHB_,energyThresholdHE_,energyThresholdHF_,energyScale);
  eventData.EPlusPzFromTowers_ = EPlusPzFromTowers.first;
  eventData.EMinusPzFromTowers_ = EPlusPzFromTowers.second;*/
  std::pair<double,double> EPlusPzFromPFCands = EPlusPz(*particleFlowCollectionH,thresholdsPFlow_);
  eventData.SetEPlusPzFromPFCands(EPlusPzFromPFCands.first);
  eventData.SetEMinusPzFromPFCands(EPlusPzFromPFCands.second);

  //double missingMassFromXiFromTowers = 2*Ebeam_*sqrt(xiFromTowers_plus*xiFromTowers_minus);
  //eventData.missingMassFromXiFromTowers_ = missingMassFromXiFromTowers;
  //double missingMassFromXiFromJets = 2*Ebeam_*sqrt(xiFromJets_plus*xiFromJets_minus);
  //eventData.missingMassFromXiFromJets_ = missingMassFromXiFromJets;
  double missingMassFromXiFromPFCands = 2*Ebeam_*sqrt(xiFromPFCands_plus*xiFromPFCands_minus);
  eventData.SetMissingMassFromXiFromPFCands(missingMassFromXiFromPFCands);

  std::pair<double,double> etaMaxFromPFCands = etaMax(*particleFlowCollectionH,thresholdsPFlow_);
  eventData.SetEtaMaxFromPFCands(etaMaxFromPFCands.first);
  eventData.SetEtaMinFromPFCands(etaMaxFromPFCands.second);

  edm::Handle<std::vector<float> > edmNtupleEtaMax;
  event.getByLabel(edm::InputTag("edmNtupleEtaMax","etaMax"),edmNtupleEtaMax);

  edm::Handle<std::vector<float> > edmNtupleEtaMin;
  event.getByLabel(edm::InputTag("edmNtupleEtaMin","etaMin"),edmNtupleEtaMin);

  //LogDebug("Analysis") << ">>>> edmNtupleEtaMax size: " << edmNtupleEtaMax->size();

  float etaMax_pfCands = edmNtupleEtaMax->size() ? (*edmNtupleEtaMax)[0] : -999.;
  float etaMin_pfCands = edmNtupleEtaMin->size() ? (*edmNtupleEtaMin)[0] : -999.; 
  eventData.SetEtaMaxFromPFCandsNew(etaMax_pfCands);
  eventData.SetEtaMinFromPFCandsNew(etaMin_pfCands);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
  // Castor RecHit collection
  edm::Handle<CastorRecHitCollection> castorRecHitCollectionH;
  event.getByLabel(castorRecHitTag_,castorRecHitCollectionH);
  double sumETotCastor = castorEnergy(*castorRecHitCollectionH, event.isRealData());
 
  eventData.SetsumETotCastor(sumETotCastor);

*/

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

  return (jetSystem.M()/allCands.M());
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

  return (dijetSystem.M()/allCands.M());
  
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//added by eliza
/*template <class ThreeJetColl,class ThreePartColl>
double ExclusiveDijetsAnalysis::Rjjj(ThreeJetColl& jetCollection,ThreePartColl& partCollection){
  math::XYZTLorentzVector trijetSystem(0.,0.,0.,0.);
  trijetSystem += (jetCollection[0]).p4();
  std::cout << " jet1 :" << (jetCollection[0]).p4() << std::endl; 
  trijetSystem += (jetCollection[1]).p4();
  std::cout << " jet2 :" << (jetCollection[1]).p4() << std::endl; 
  trijetSystem += (jetCollection[2]).p4();
  std::cout << " jet3 :" << (jetCollection[2]).p4() << std::endl; 
  
  math::XYZTLorentzVector allCands(0.,0.,0.,0.);
  for(typename ThreePartColl::const_iterator part = partCollection.begin();
                                        part != partCollection.end(); ++part) allCands += part->p4();

  return (trijetSystem.M()/allCands.M());
   

}*/

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
