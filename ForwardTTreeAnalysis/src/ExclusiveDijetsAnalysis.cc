
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

namespace exclusiveDijetsAnalysis {

ExclusiveDijetsAnalysis::ExclusiveDijetsAnalysis(const edm::ParameterSet& pset):
  jetTag_(pset.getParameter<edm::InputTag>("JetTag")),
  jetNonCorrTag_(pset.getParameter<edm::InputTag>("JetNonCorrTag")), 
  particleFlowTag_(pset.getParameter<edm::InputTag>("ParticleFlowTag")),
  vertexTag_(pset.getParameter<edm::InputTag>("VertexTag")),
  trackMultiplicityTag_(pset.getParameter<edm::InputTag>("TrackMultiplicityTag")),
  doBtag_(pset.getParameter<bool>("DoBTag")),
  thresholdHF_(pset.getParameter<unsigned int>("HFThresholdIndex")),
  useJetCorrection_(pset.getParameter<bool>("UseJetCorrection")),
  accessPileUpInfo_(pset.getParameter<bool>("AccessPileUpInfo")),
  Ebeam_(pset.getUntrackedParameter<double>("EBeam",5000.)),
  usePAT_(pset.getUntrackedParameter<bool>("UsePAT",true)),
  runOnData_(pset.getUntrackedParameter<bool>("RunOnData",false)), 
  genProtonPlus_(0.,0.,0.,0.),
  genProtonMinus_(0.,0.,0.,0.),
  genAllParticles_(0.,0.,0.,0.)  
{
  if(useJetCorrection_) jetCorrectionService_ = pset.getParameter<std::string>("JetCorrectionService");
  if(doBtag_){
     bDiscriminatorName_ = pset.getParameter<std::string>("BDiscriminatorName");
  }  
}

ExclusiveDijetsAnalysis::~ExclusiveDijetsAnalysis(){}

void ExclusiveDijetsAnalysis::setBeginRun(const edm::Run& run, const edm::EventSetup& setup){
  if(useJetCorrection_) corrector_ = JetCorrector::getJetCorrector(jetCorrectionService_,setup);
}

void ExclusiveDijetsAnalysis::setGenInfo(const edm::Event& event, const edm::EventSetup& setup){
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
}

void ExclusiveDijetsAnalysis::fillPileUpInfo(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<std::map<int,int> > pileUpMap;
  event.getByLabel("pileUpInfo",pileUpMap);
  std::map<int,int>::const_iterator bx0Iter = pileUpMap->find(0);
  if(bx0Iter == pileUpMap->end()){
     edm::LogError("Analysis") << ">>> Pile-up info does not contain bunch crossing 0 ..skipping";
     return;
  }
  int nPileUpBx0 = bx0Iter->second;
  LogTrace("Analysis") << "  Number of pile-up events in bunch crossing 0: " << nPileUpBx0;
  eventData.nPileUpBx0_ = nPileUpBx0;
}

void ExclusiveDijetsAnalysis::fillVertexInfo(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  // Access vertex collection
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

  eventData.nVertex_ = nGoodVertices;
}

void ExclusiveDijetsAnalysis::fillJetInfo(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<edm::View<reco::Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);

  const reco::Jet& jet1 = (*jetCollectionH)[0];// they come out ordered right?
  const reco::Jet& jet2 = (*jetCollectionH)[1];

  edm::Handle<edm::View<reco::PFCandidate> > particleFlowCollectionH;
  event.getByLabel(particleFlowTag_,particleFlowCollectionH);

  eventData.leadingJetPt_ = jet1.pt();
  eventData.secondJetPt_ = jet2.pt();
  eventData.leadingJetEta_ = jet1.eta();
  eventData.secondJetEta_ = jet2.eta();
  eventData.leadingJetPhi_ = jet1.phi();
  eventData.secondJetPhi_ = jet2.phi();
  
  if(jetCollectionH->size() > 2){
     const reco::Jet& jet3 = (*jetCollectionH)[2];
     eventData.thirdJetPt_ = jet3.pt();
     eventData.thirdJetEta_ = jet3.eta();
  } else{
     eventData.thirdJetPt_ = -999.;
     eventData.thirdJetEta_ = -999.;
  }

  eventData.jetsAveEta_ = (jet1.eta() + jet2.eta())/2;
  eventData.jetsDeltaEta_ = jet1.eta() - jet2.eta();
  eventData.jetsDeltaPhi_ = M_PI - fabs(jet1.phi() - jet2.phi());
  eventData.jetsDeltaPt_ = fabs(jet1.pt() - jet2.pt());

  // Di-jet mass
  math::XYZTLorentzVector dijetSystem(0.,0.,0.,0.);
  dijetSystem += jet1.p4();
  dijetSystem += jet2.p4();
  eventData.massDijets_ = dijetSystem.M();

  if(!runOnData_) eventData.MxGen_ = genAllParticles_.M();
  else eventData.MxGen_ = -1.;

  // M_{X}
  math::XYZTLorentzVector allJets(0.,0.,0.,0.);
  for(edm::View<reco::Jet>::const_iterator jet = jetCollectionH->begin();
                                           jet != jetCollectionH->end(); ++jet) allJets += jet->p4();
  eventData.MxFromJets_ = allJets.M();

  math::XYZTLorentzVector allPFCands(0.,0.,0.,0.);
  for(edm::View<reco::PFCandidate>::const_iterator pfCand = particleFlowCollectionH->begin();
                                                   pfCand != particleFlowCollectionH->end(); ++pfCand) allPFCands += pfCand->p4();

  eventData.MxFromPFCands_ = allPFCands.M();

  // Rjj
  double RjjFromJets = Rjj(*jetCollectionH,*jetCollectionH);
  
  edm::Handle<edm::View<reco::Jet> > jetCollectionNonCorrH;
  event.getByLabel(jetNonCorrTag_,jetCollectionNonCorrH);
  double RjjFromPFCands = Rjj(*jetCollectionNonCorrH,*particleFlowCollectionH);
 
  eventData.RjjFromJets_ = RjjFromJets;
  eventData.RjjFromPFCands_ = RjjFromPFCands;

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
           eventData.massDijetsGen_ = massGen;
        }
     } else{
        eventData.massDijetsGen_ = -999.;
     }

     // B-tagging
     if(doBtag_){
        double bDiscJet1 = patJet1->bDiscriminator(bDiscriminatorName_);
        double bDiscJet2 = patJet2->bDiscriminator(bDiscriminatorName_);

        eventData.leadingJetBDiscriminator_ = bDiscJet1;
        eventData.secondJetBDiscriminator_ = bDiscJet2;
     }
  } else {
     // access info from outside PAT here
     eventData.massDijetsGen_ = -999.;
     eventData.leadingJetBDiscriminator_ = -999.;
     eventData.secondJetBDiscriminator_ = -999.;
  }
}

void ExclusiveDijetsAnalysis::fillMultiplicities(ExclusiveDijetsEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  // Access multiplicities
  edm::Handle<unsigned int> trackMultiplicity; 
  //event.getByLabel("trackMultiplicityTransverseRegion","trackMultiplicity",trackMultiplicity);
  event.getByLabel(trackMultiplicityTag_,trackMultiplicity); 

  edm::Handle<std::vector<unsigned int> > nHFPlus;
  event.getByLabel("hfTower","nHFplus",nHFPlus);
  
  edm::Handle<std::vector<unsigned int> > nHFMinus;
  event.getByLabel("hfTower","nHFminus",nHFMinus);

  edm::Handle<std::map<unsigned int, std::vector<unsigned int> > > mapThreshToiEtaPlus;
  event.getByLabel("hfTower","mapTreshToiEtaplus",mapThreshToiEtaPlus);

  edm::Handle<std::map<unsigned int, std::vector<unsigned int> > > mapThreshToiEtaMinus;
  event.getByLabel("hfTower","mapTreshToiEtaminus",mapThreshToiEtaMinus);

  edm::Handle<std::vector<double> > sumEHFplus;
  event.getByLabel("hfTower","sumEHFplus",sumEHFplus);

  edm::Handle<std::vector<double> > sumEHFminus;
  event.getByLabel("hfTower","sumEHFminus",sumEHFminus);

  unsigned int nTracks = *trackMultiplicity;

  unsigned int nHF_plus = (*nHFPlus)[thresholdHF_];
  unsigned int nHF_minus = (*nHFMinus)[thresholdHF_];

  double sumE_plus = (*sumEHFplus)[thresholdHF_];
  double sumE_minus = (*sumEHFminus)[thresholdHF_];

  eventData.trackMultiplicity_ = nTracks;
  eventData.multiplicityHFPlus_ = nHF_plus;
  eventData.multiplicityHFMinus_ = nHF_minus;
  eventData.sumEnergyHFPlus_ = sumE_plus;
  eventData.sumEnergyHFMinus_ = sumE_minus;
  
  for(unsigned int ieta = 29, index = 0; ieta <= 41; ++ieta,++index){
     unsigned int nHFPlus_ieta = nHFSlice(*mapThreshToiEtaPlus,thresholdHF_,ieta);
     eventData.multiplicityHFPlusVsiEta_[index] = nHFPlus_ieta;

     unsigned int nHFMinus_ieta = nHFSlice(*mapThreshToiEtaMinus,thresholdHF_,ieta);
     eventData.multiplicityHFMinusVsiEta_[index] = nHFMinus_ieta;
  }
}

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
     eventData.xiGenPlus_ = xigen_plus;
     eventData.xiGenMinus_ = xigen_minus;
  } else{
     eventData.xiGenPlus_ = -1.;
     eventData.xiGenMinus_ = -1.;
  }

  edm::Handle<double> xiTowerPlus;
  event.getByLabel("xiTower","xiTowerplus",xiTowerPlus);

  edm::Handle<double> xiTowerMinus;
  event.getByLabel("xiTower","xiTowerminus",xiTowerMinus);

  double xiTower_plus = *xiTowerPlus;
  double xiTower_minus = *xiTowerMinus;

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

  eventData.xiTowerPlus_ = xiTower_plus;
  eventData.xiTowerMinus_ = xiTower_minus;
  eventData.xiPlusFromJets_ = xiFromJets_plus;
  eventData.xiMinusFromJets_ = xiFromJets_minus;
  eventData.xiPlusFromPFCands_ = xiFromPFCands_plus;
  eventData.xiMinusFromPFCands_ = xiFromPFCands_minus;

  double missingMass = 2*Ebeam_*sqrt(xiFromPFCands_plus*xiFromPFCands_minus);
  eventData.missingMassFromXi_ = missingMass;
}

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

unsigned int ExclusiveDijetsAnalysis::nHFSlice(const std::map<unsigned int, std::vector<unsigned int> >& mapTreshToiEta, unsigned int thresholdHF, unsigned int ieta){
  const std::vector<unsigned int>& vec_iEta = mapTreshToiEta.find(thresholdHF)->second;

  // Count number of ieta entries in vector 
  int count_ieta = (int)std::count(vec_iEta.begin(),vec_iEta.end(),ieta);

  return count_ieta;
}

} // namespace
