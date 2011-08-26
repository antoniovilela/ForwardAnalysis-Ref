
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsAnalysis.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"



#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/FWLiteTools.h"
//---------------------------------------------------------------------------

namespace exclusiveDijetsAnalysis {

ExclusiveDijetsAnalysis::ExclusiveDijetsAnalysis(const edm::ParameterSet& pset):
  vertexTag_(pset.getParameter<edm::InputTag>("VertexTag")),//ok
  trackTag_(pset.getParameter<edm::InputTag>("TrackTag")),//old My dijets analysis
  metTag_(pset.getParameter<edm::InputTag>("METTag")),//old My dijets analysis
  jetTag_(pset.getParameter<edm::InputTag>("JetTag")),//ok
  jetNonCorrTag_(pset.getParameter<edm::InputTag>("JetNonCorrTag")), //ok
  trackMultiplicityTag_(pset.getParameter<edm::InputTag>("TrackMultiplicityTag")),//ok
  particleFlowTag_(pset.getParameter<edm::InputTag>("ParticleFlowTag")),//ok
  genChargedTag_(pset.getParameter<edm::InputTag>("GenChargedParticlesTag")),//old My dijets analysis
  triggerResultsTag_(pset.getParameter<edm::InputTag>("TriggerResultsTag")),//old My dijets analysis
  //  hcalTowerSummaryTag_(pset.getParameter<edm::InputTag>("HCALTowerSummaryTag")),//old My dijets analysis
  //  doBtag_(pset.getParameter<bool>("DoBTag")),
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
  runOnData_(pset.getUntrackedParameter<bool>("RunOnData",true)), 
//  hltPathName1_(pset.getParameter<std::string>("HLTPath1")),//old My dijets analysis
  hltPathName2_(pset.getParameter<std::string>("HLTPath2")),//old My dijets analysis
  hltPathName3_(pset.getParameter<std::string>("HLTPath3"))//,//old My dijets analysis
 //  hltPathName4_(pset.getParameter<std::string>("HLTPath4")),//old My dijets analysis
//   hltPathName5_(pset.getParameter<std::string>("HLTPath5")),//old My dijets analysis
//   hltPathName6_(pset.getParameter<std::string>("HLTPath6")) //old My dijets analysis


{
  
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
void ExclusiveDijetsAnalysis::setGenInfo(const edm::Event& event, const edm::EventSetup& setup){

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
  
  /*if(proton1 != genParticlesCollectionH->end()){
     LogTrace("Analysis") << "Proton (z-plus): " << proton1->pt() << "  " << proton1->eta() << "  " << proton1->phi() << std::endl;
     xigen_plus = 1 - proton1->pz()/Ebeam_;
     allGenParticles -= proton1->p4();
  }
  if(proton2 != genParticlesCollectionH->end()){
     LogTrace("Analysis") << "Proton (z-minus): " << proton2->pt() << "  " << proton2->eta() << "  " << proton2->phi() << std::endl;
     xigen_minus = 1 + proton2->pz()/Ebeam_;
     allGenParticles -= proton2->p4();
  }*/

  if(proton_plus != genParticlesCollectionH->end()) genProtonPlus_ = proton_plus->p4();
  if(proton_minus != genParticlesCollectionH->end()) genProtonMinus_ = proton_minus->p4(); 
  genAllParticles_ = allGenParticles;

  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillEventData(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  // Set internal info
  if(!runOnData_) setGenInfo(event,setup);

  // Fill event data
  if(!runOnData_ && accessPileUpInfo_){
     fillPileUpInfo(eventData,event,setup);
  } else {
     eventData.nPileUpBx0_ = -1;
  }
  fillVertexInfo(eventData,event,setup);
  fillJetInfo(eventData,event,setup);
  fillMultiplicities(eventData,event,setup);
  fillXiInfo(eventData,event,setup);
  //


}
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

/*
 eventData.nVertex_ = nGoodVertices;
  const reco::Vertex& primVertex = (*vertexCollectionH)[0];
  if(primVertex.isValid() && !primVertex.isFake()){
     eventData.posXPrimVtx_ = primVertex.x();
     eventData.posYPrimVtx_ = primVertex.y();
     eventData.posZPrimVtx_ = primVertex.z();
  } else{
     eventData.posXPrimVtx_ = -999.;
     eventData.posYPrimVtx_ = -999.;
     eventData.posZPrimVtx_ = -999.;
  }
*/

//   eventData.nVertex_ = nGoodVertices;
}
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillJetInfo(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<edm::View<reco::Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);

  const reco::Jet& jet1 = (*jetCollectionH)[0];// they come out ordered right?
  const reco::Jet& jet2 = (*jetCollectionH)[1];

  edm::Handle<edm::View<reco::PFCandidate> > particleFlowCollectionH;
  event.getByLabel(particleFlowTag_,particleFlowCollectionH);
/*
  eventData.leadingJetPt_ = jet1.pt();
  eventData.secondJetPt_ = jet2.pt();
  eventData.leadingJetEta_ = jet1.eta();
  eventData.secondJetEta_ = jet2.eta();
  eventData.leadingJetPhi_ = jet1.phi();
  eventData.secondJetPhi_ = jet2.phi();
  */

  eventData.SetLeadingJetPt(jet1.pt());
  eventData.SetSecondJetPt(jet2.pt());
  eventData.SetLeadingJetEta(jet1.eta());
  eventData.SetSecondJetEta(jet2.eta());
  eventData.SetLeadingJetPhi(jet1.phi());
  eventData.SetSecondJetPhi(jet2.phi());


 ///Using Lorentz Vector
  eventData.SetLeadingJetP4(jet1.p4());
  eventData.SetSecondJetP4(jet2.p4());
 

  if(jetCollectionH->size() > 2){
     const reco::Jet& jet3 = (*jetCollectionH)[2];
    // eventData.thirdJetPt_ = jet3.pt();
    // eventData.thirdJetEta_ = jet3.eta();
    
     eventData.SetThirdJetPt(jet3.pt());
     eventData.SetThirdJetEta(jet3.eta());
   ///Using Lorentz Vector
     eventData.SetThirdJetP4(jet3.p4());

 } else{
     //eventData.thirdJetPt_ = -999.;
     //eventData.thirdJetEta_ = -999.;

     eventData.SetThirdJetPt(-999.);
     eventData.SetThirdJetEta(-999.);
///Using Lorentz Vector
//     eventData.SetThirdJetP4(-999.0,-999.0,-999.0,-999.0);

  }

  //eventData.jetsAveEta_ = (jet1.eta() + jet2.eta())/2;
  //eventData.jetsDeltaEta_ = jet1.eta() - jet2.eta();
  //eventData.jetsDeltaPhi_ = M_PI - fabs(jet1.phi() - jet2.phi());
  //eventData.jetsDeltaPt_ = fabs(jet1.pt() - jet2.pt());




  eventData.SetJetsAveEta((jet1.eta() + jet2.eta())/2);
  eventData.SetJetsDeltaEta(jet1.eta() - jet2.eta());
  eventData.SetJetsDeltaPhi(M_PI - fabs(jet1.phi() - jet2.phi()));
  eventData.SetJetsDeltaPt(fabs(jet1.pt() - jet2.pt()));

  // Di-jet mass
  math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
  dijetSystem += jet1.p4();
  dijetSystem += jet2.p4();
//  eventData.massDijets_ = dijetSystem.M();
  eventData.SetMassDijets(dijetSystem.M());



  if(!runOnData_)  eventData.SetMxGen(genAllParticles_.M());  //eventData.MxGen_ = genAllParticles_.M();
  else eventData.SetMxGen(-1.);        //eventData.MxGen_ = -1.;

  // M_{X}
  math::XYZTLorentzVector allJets(0.,0.,0.,0.);
  for(edm::View<reco::Jet>::const_iterator jet = jetCollectionH->begin();
                                           jet != jetCollectionH->end(); ++jet) allJets += jet->p4();
  //eventData.MxFromJets_ = allJets.M();

  eventData.SetMxFromJets(allJets.M());


  math::XYZTLorentzVector allPFCands(0.,0.,0.,0.);
  for(edm::View<reco::PFCandidate>::const_iterator pfCand = particleFlowCollectionH->begin();
                                                   pfCand != particleFlowCollectionH->end(); ++pfCand) allPFCands += pfCand->p4();

//  eventData.MxFromPFCands_ = allPFCands.M();

    eventData.SetMxFromPFCands(allPFCands.M());

  // Rjj
  double RjjFromJets = Rjj(*jetCollectionH,*jetCollectionH);
  
  edm::Handle<edm::View<reco::Jet> > jetCollectionNonCorrH;
  event.getByLabel(jetNonCorrTag_,jetCollectionNonCorrH);
  double RjjFromPFCands = Rjj(*jetCollectionNonCorrH,*particleFlowCollectionH);
 
  //eventData.RjjFromJets_ = RjjFromJets;
  //eventData.RjjFromPFCands_ = RjjFromPFCands;

  eventData.SetRjjFromJets(RjjFromJets);
  eventData.SetRjjFromPFCands(RjjFromPFCands);

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
//           eventData.massDijetsGen_ = massGen;
            eventData.SetMassDijetsGen(massGen);

        }
     } else{
//        eventData.massDijetsGen_ = -999.;
          eventData.SetMassDijetsGen(-999.);

     }

     // B-tagging
     if(doBtag_){
        double bDiscJet1 = patJet1->bDiscriminator(bDiscriminatorName_);
        double bDiscJet2 = patJet2->bDiscriminator(bDiscriminatorName_);

        //eventData.leadingJetBDiscriminator_ = bDiscJet1;
        //eventData.secondJetBDiscriminator_ = bDiscJet2;
        eventData.SetLeadingJetBDiscriminator(bDiscJet1);
        eventData.SetSecondJetBDiscriminator(bDiscJet2);
   

  }
  } else {
     // access info from outside PAT here
     //eventData.massDijetsGen_ = -999.;
     //eventData.leadingJetBDiscriminator_ = -999.;
     //eventData.secondJetBDiscriminator_ = -999.;

     eventData.SetMassDijetsGen(-999.);
     eventData.SetLeadingJetBDiscriminator(-999.);
     eventData.SetSecondJetBDiscriminator(-999.);

  }
}
  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillMultiplicities(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  //////////////////////////////////////////////////////////////////////////////////////
  //ORiginal Tunico
// //  // Access multiplicities
//   edm::Handle<unsigned int> trackMultiplicity; 
//   //event.getByLabel("trackMultiplicityTransverseRegion","trackMultiplicity",trackMultiplicity);
//   event.getByLabel(trackMultiplicityTag_,trackMultiplicity); 

//   edm::Handle<std::vector<unsigned int> > nHBPlus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHBplus"),nHBPlus);

//   edm::Handle<std::vector<unsigned int> > nHBMinus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHBminus"),nHBMinus);

//   edm::Handle<std::vector<unsigned int> > nHEPlus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHEplus"),nHEPlus);

//   edm::Handle<std::vector<unsigned int> > nHEMinus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHEminus"),nHEMinus);

//   edm::Handle<std::vector<unsigned int> > nHFPlus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHFplus"),nHFPlus);

//   edm::Handle<std::vector<unsigned int> > nHFMinus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHFminus"),nHFMinus);
// edm::Handle<std::map<unsigned int, std::vector<unsigned int> > > iEtaHFMultiplicityPlus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFMultiplicityPlus"),iEtaHFMultiplicityPlus);

//   edm::Handle<std::map<unsigned int, std::vector<unsigned int> > > iEtaHFMultiplicityMinus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFMultiplicityMinus"),iEtaHFMultiplicityMinus);

//   edm::Handle<std::vector<double> > sumEHBplus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHBplus"),sumEHBplus);

//   edm::Handle<std::vector<double> > sumEHBminus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHBminus"),sumEHBminus);

//   edm::Handle<std::vector<double> > sumETHBplus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHBplus"),sumETHBplus);

//   edm::Handle<std::vector<double> > sumETHBminus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHBminus"),sumETHBminus);

//   edm::Handle<std::vector<double> > sumEHEplus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHEplus"),sumEHEplus);

//   edm::Handle<std::vector<double> > sumEHEminus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHEminus"),sumEHEminus);

//   edm::Handle<std::vector<double> > sumETHEplus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHEplus"),sumETHEplus);

//   edm::Handle<std::vector<double> > sumETHEminus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHEminus"),sumETHEminus);

//   edm::Handle<std::vector<double> > sumETHFplus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHFplus"),sumETHFplus);

//   edm::Handle<std::vector<double> > sumETHFminus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHFminus"),sumETHFminus);


//   edm::Handle<std::vector<double> > sumEHFminus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHFminus"),sumEHFminus);

//   edm::Handle<std::vector<double> > sumEHFplus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHFplus"),sumEHFplus);

//   edm::Handle<std::map<unsigned int, std::vector<double> > > iEtaHFEnergySumPlus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFEnergySumPlus"),iEtaHFEnergySumPlus);

//   edm::Handle<std::map<unsigned int, std::vector<double> > > iEtaHFEnergySumMinus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFEnergySumMinus"),iEtaHFEnergySumMinus);

//   edm::Handle<std::map<unsigned int, std::vector<double> > > iEtaHFETSumPlus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFETSumPlus"),iEtaHFETSumPlus);

//   edm::Handle<std::map<unsigned int, std::vector<double> > > iEtaHFETSumMinus;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFETSumMinus"),iEtaHFETSumMinus);

//    edm::Handle<std::vector<double> > thresholdsHB;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"thresholdsHB"),thresholdsHB);
//   size_t indexThresholdHB = std::lower_bound((*thresholdsHB).begin(),(*thresholdsHB).end(),energyThresholdHB_) - (*thresholdsHB).begin();

//   edm::Handle<std::vector<double> > thresholdsHE;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"thresholdsHE"),thresholdsHE);
//   size_t indexThresholdHE = std::lower_bound((*thresholdsHE).begin(),(*thresholdsHE).end(),energyThresholdHE_) - (*thresholdsHE).begin();

//   edm::Handle<std::vector<double> > thresholdsHF;
//   event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"thresholdsHF"),thresholdsHF);
//   size_t indexThresholdHF = std::lower_bound((*thresholdsHF).begin(),(*thresholdsHF).end(),energyThresholdHF_) - (*thresholdsHF).begin();
  
//   unsigned int nHB_plus = (*nHBPlus)[indexThresholdHB];
//   unsigned int nHB_minus = (*nHBMinus)[indexThresholdHB];

//   unsigned int nHE_plus = (*nHEPlus)[indexThresholdHE];
//   unsigned int nHE_minus = (*nHEMinus)[indexThresholdHE];

//   unsigned int nHF_plus = (*nHFPlus)[indexThresholdHF];
//   unsigned int nHF_minus = (*nHFMinus)[indexThresholdHF];

//   double sumEHB_plus = (*sumEHBplus)[indexThresholdHB];
//   double sumEHB_minus = (*sumEHBminus)[indexThresholdHB];

//   double sumETHB_plus = (*sumETHBplus)[indexThresholdHB];
//   double sumETHB_minus = (*sumETHBminus)[indexThresholdHB];

//   double sumEHE_plus = (*sumEHEplus)[indexThresholdHE];
//   double sumEHE_minus = (*sumEHEminus)[indexThresholdHE];

//   double sumETHE_plus = (*sumETHEplus)[indexThresholdHE];
//   double sumETHE_minus = (*sumETHEminus)[indexThresholdHE];

//   double sumEHF_plus = (*sumEHFplus)[indexThresholdHF];
//   double sumEHF_minus = (*sumEHFminus)[indexThresholdHF];

//   double sumETHF_plus = (*sumETHFplus)[indexThresholdHF];
//   double sumETHF_minus = (*sumETHFminus)[indexThresholdHF];


// /*
//   edm::Handle<std::vector<unsigned int> > nHFPlus;
//   event.getByLabel("hfTower","nHFplus",nHFPlus);
  
//   edm::Handle<std::vector<unsigned int> > nHFMinus;
//   event.getByLabel("hfTower","nHFminus",nHFMinus);

//   edm::Handle<std::map<unsigned int, std::vector<unsigned int> > > mapThreshToiEtaPlus;
//   event.getByLabel("hfTower","mapTreshToiEtaplus",mapThreshToiEtaPlus);

//   edm::Handle<std::map<unsigned int, std::vector<unsigned int> > > mapThreshToiEtaMinus;
//   event.getByLabel("hfTower","mapTreshToiEtaminus",mapThreshToiEtaMinus);

//   edm::Handle<std::vector<double> > sumEHFplus;
//   event.getByLabel("hfTower","sumEHFplus",sumEHFplus);

//   edm::Handle<std::vector<double> > sumEHFminus;
//   event.getByLabel("hfTower","sumEHFminus",sumEHFminus);
// */
  

//   //unsigned int nTracks = *trackMultiplicity;

//  // unsigned int nHF_plus = (*nHFPlus)[thresholdHF_];
//   //unsigned int nHF_minus = (*nHFMinus)[thresholdHF_];

//   //double sumE_plus = (*sumEHFplus)[thresholdHF_];
//   //double sumE_minus = (*sumEHFminus)[thresholdHF_];



// //  eventData.trackMultiplicity_ = nTracks;
// //  eventData.multiplicityHFPlus_ = nHF_plus;
//  // eventData.multiplicityHFMinus_ = nHF_minus;
// //  eventData.sumEnergyHFPlus_ = sumE_plus;
// //  eventData.sumEnergyHFMinus_ = sumE_minus;



//   unsigned int nTracks = *trackMultiplicity;
//   // Multiplicity of Tracks
//   eventData.SetTrackMultiplicity(nTracks);
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

  
//   for(unsigned int ieta = 29, index = 0; ieta <= 41; ++ieta,++index){
//     double sumETHFPlus_ieta = exclusiveDijetsAnalysis::sumEHCALiEta(*iEtaHFETSumPlus,indexThresholdHF,ieta);
//     eventData.sumETHFPlusVsiEta_[index] = sumETHFPlus_ieta;
//     //eventData.SetSumETHFPlusVsiEta(index,sumETHFPlus_ieta);

//     unsigned int nHFMinus_ieta = exclusiveDijetsAnalysis::nHCALiEta(*iEtaHFMultiplicityMinus,indexThresholdHF,ieta);
//     eventData.multiplicityHFMinusVsiEta_[index] = nHFMinus_ieta;
//     //eventData.SetMultiplicityHFMinusVsiEta(index,nHFMinus_ieta);    

//     double sumEHFMinus_ieta = exclusiveDijetsAnalysis::sumEHCALiEta(*iEtaHFEnergySumMinus,indexThresholdHF,ieta); 
//     eventData.sumEHFMinusVsiEta_[index] = sumEHFMinus_ieta;
//   //  eventData.SetSumEHFMinusVsiEta(index,sumEHFMinus_ieta);

//     double sumETHFMinus_ieta = exclusiveDijetsAnalysis::sumEHCALiEta(*iEtaHFETSumMinus,indexThresholdHF,ieta);
//     eventData.sumETHFMinusVsiEta_[index] = sumETHFMinus_ieta;
//    //eventData.SetSumETHFMinusVsiEta(index,sumETHFMinus_ieta);



// //   unsigned int nHFPlus_ieta = nHFSlice(*mapThreshToiEtaPlus,thresholdHF_,ieta);
//     //eventData.multiplicityHFPlusVsiEta_[index] = nHFPlus_ieta;
//      //eventData.SetMultiplicityHFPlusVsiEta[index](nHFPlus_ieta);


//     // unsigned int nHFMinus_ieta = nHFSlice(*mapThreshToiEtaMinus,thresholdHF_,ieta);
//     // eventData.multiplicityHFMinusVsiEta_[index] = nHFMinus_ieta;
//     // eventData.SetMultiplicityHFMinusVsiEta[index](nHFMinus_ieta);

    

//  }


}
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ExclusiveDijetsAnalysis::fillXiInfo(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  if(!runOnData_){
     // Gen particles
     edm::Handle<edm::View<reco::GenParticle> > genParticlesCollectionH;
     event.getByLabel("genParticles",genParticlesCollectionH);
   
     edm::View<reco::GenParticle>::const_iterator proton1 = genParticlesCollectionH->end();
     edm::View<reco::GenParticle>::const_iterator proton2 = genParticlesCollectionH->end();
     for(edm::View<reco::GenParticle>::const_iterator genpart = genParticlesCollectionH->begin();
                                                   genpart != genParticlesCollectionH->end(); ++genpart){
        if(genpart->status() != 1) continue;
        double pz = genpart->pz();
        if((proton1 == genParticlesCollectionH->end())&&(genpart->pdgId() == 2212)&&(pz > 0.75*Ebeam_)) proton1 = genpart;
        else if((proton2 == genParticlesCollectionH->end())&&(genpart->pdgId() == 2212)&&(pz < -0.75*Ebeam_)) proton2 = genpart;
     }

     double xigen_plus = -1.;
     double xigen_minus = -1.;
     if((proton1 != genParticlesCollectionH->end())&&(proton2 != genParticlesCollectionH->end())){
        LogTrace("Analysis") << "Proton (z-plus): " << proton1->pt() << "  " << proton1->eta() << "  " << proton1->phi() << std::endl;
        LogTrace("Analysis") << "Proton (z-minus): " << proton2->pt() << "  " << proton2->eta() << "  " << proton2->phi() << std::endl;
        xigen_plus = 1 - proton1->pz()/Ebeam_;
        xigen_minus = 1 + proton2->pz()/Ebeam_;
     }


    // eventData.xiGenPlus_ = xigen_plus;
    // eventData.xiGenMinus_ = xigen_minus;
     eventData.SetXiGenPlus(xigen_plus);
     eventData.SetXiGenMinus(xigen_minus);


  } else{
    // eventData.xiGenPlus_ = -1.;
    // eventData.xiGenMinus_ = -1.;
     eventData.SetXiGenPlus(-1.);
     eventData.SetXiGenMinus(-1.); 

 }
  edm::Handle<edm::View<reco::Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);

  edm::Handle<edm::View<reco::PFCandidate> > particleFlowCollectionH;
  event.getByLabel(particleFlowTag_,particleFlowCollectionH);

  std::pair<double,double> xiFromJets = xi(*jetCollectionH);
  double xiFromJets_plus = xiFromJets.first;
  double xiFromJets_minus = xiFromJets.second;

  std::pair<double,double> xiFromPFCands = xi(*particleFlowCollectionH,useJetCorrection_);
  double xiFromPFCands_plus = xiFromPFCands.first;
  double xiFromPFCands_minus = xiFromPFCands.second;

//  eventData.xiTowerPlus_ = xiTower_plus;
  //eventData.xiTowerMinus_ = xiTower_minus;
  //eventData.xiPlusFromJets_ = xiFromJets_plus;
  //eventData.xiMinusFromJets_ = xiFromJets_minus;
  //eventData.xiPlusFromPFCands_ = xiFromPFCands_plus;
  //eventData.xiMinusFromPFCands_ = xiFromPFCands_minus;

  //eventData.SetXiTowerPlus(xiTower_plus);
 // eventData.SetXiTowerMinus(xiTower_minus);
  eventData.SetXiPlusFromJets(xiFromJets_plus);
  eventData.SetXiMinusFromJets(xiFromJets_minus);
  eventData.SetXiPlusFromPFCands(xiFromPFCands_plus);
  eventData.SetXiMinusFromPFCands(xiFromPFCands_minus);

  double missingMass = 2*Ebeam_*sqrt(xiFromPFCands_plus*xiFromPFCands_minus);
 // eventData.missingMassFromXi_ = missingMass;

  eventData.SetMissingMassFromXi(missingMass);

}
  ////////////////////////////////////////////////////////////////////////////////////////////////////
template <class Coll>
std::pair<double,double> ExclusiveDijetsAnalysis::xi(Coll& partCollection, bool useJetCorr){

  double xi_towers_plus = 0.;
  double xi_towers_minus = 0.;
  double pt_min = 5.;   
  for(typename Coll::const_iterator part = partCollection.begin(); part != partCollection.end(); ++part){
    double correction = (useJetCorr&&corrector_&&(part->pt() >= pt_min))?(corrector_->correction(part->p4())):1.;
    xi_towers_plus += correction*part->et()*exp(part->eta());
    xi_towers_minus += correction*part->et()*exp(-part->eta());
  }

  xi_towers_plus /= 2*Ebeam_;
  xi_towers_minus /= 2*Ebeam_;
   
  return std::make_pair(xi_towers_plus,xi_towers_minus);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int ExclusiveDijetsAnalysis::nHFSlice(const std::map<unsigned int, std::vector<unsigned int> >& mapTreshToiEta, unsigned int thresholdHF, unsigned int ieta){
  const std::vector<unsigned int>& vec_iEta = mapTreshToiEta.find(thresholdHF)->second;

  // Count number of ieta entries in vector 
  int count_ieta = (int)std::count(vec_iEta.begin(),vec_iEta.end(),ieta);

  return count_ieta;
}

////////////////////////////////////////////////////////////////////////
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


} // namespace
