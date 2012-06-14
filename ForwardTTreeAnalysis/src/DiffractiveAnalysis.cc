
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveAnalysis.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/METReco/interface/BeamHaloSummary.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Utilities/interface/RegexMatch.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "ForwardAnalysis/Utilities/interface/LargestGenRapidityGap.h"
#include "ForwardAnalysis/Utilities/interface/CastorEnergy.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

//#include "Utilities/AnalysisTools/interface/FWLiteTools.h"
//#include "MinimumBiasAnalysis/MinimumBiasAnalysis/interface/FWLiteTools.h"

using namespace forwardAnalysis;
using diffractiveAnalysis::DiffractiveAnalysis;

const char* DiffractiveAnalysis::name = "DiffractiveAnalysis";

DiffractiveAnalysis::DiffractiveAnalysis(const edm::ParameterSet& pset):
  vertexTag_(pset.getParameter<edm::InputTag>("vertexTag")),
  trackTag_(pset.getParameter<edm::InputTag>("trackTag")),
  metTag_(pset.getParameter<edm::InputTag>("metTag")),
  jetTag_(pset.getParameter<edm::InputTag>("jetTag")),
  caloTowerTag_(pset.getParameter<edm::InputTag>("caloTowerTag")),
  castorRecHitTag_(pset.getParameter<edm::InputTag>("castorRecHitTag")),
  particleFlowTag_(pset.getParameter<edm::InputTag>("particleFlowTag")),
  genChargedTag_(pset.getParameter<edm::InputTag>("genChargedParticlesTag")),
  triggerResultsTag_(pset.getParameter<edm::InputTag>("triggerResultsTag")),
  hcalTowerSummaryTag_(pset.getParameter<edm::InputTag>("hcalTowerSummaryTag")),
  energyThresholdHB_(pset.getParameter<double>("energyThresholdHB")),
  energyThresholdHE_(pset.getParameter<double>("energyThresholdHE")),
  energyThresholdHF_(pset.getParameter<double>("energyThresholdHF")),
  comEnergy_(pset.getParameter<double>("comEnergy")),
  applyEnergyScaleHCAL_(pset.getParameter<bool>("applyEnergyScaleHCAL")),
  energyScaleHCAL_(-1.),
  accessMCInfo_(pset.getUntrackedParameter<bool>("accessMCInfo",false)),
  //hltPathNames_(pset.getParameter<std::vector<std::string> >("HLTPathNames")),
  hltPathName_(pset.getParameter<std::string>("hltPath")),
  ttBit_(pset.getParameter<int>("techTrigBit")) 
{
  Ebeam_ = comEnergy_/2.;
  if(applyEnergyScaleHCAL_) energyScaleHCAL_ = pset.getParameter<double>("EnergyScaleFactorHCAL");
}

DiffractiveAnalysis::~DiffractiveAnalysis(){}

void DiffractiveAnalysis::begin() {}

void DiffractiveAnalysis::begin(const edm::Run& run, const edm::EventSetup& setup) {}

void DiffractiveAnalysis::end() {}

void DiffractiveAnalysis::fill(DiffractiveEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  eventData.reset();

  int ietasize = 13;
  eventData.multiplicityHFPlusVsiEta_.resize(ietasize);
  eventData.multiplicityHFMinusVsiEta_.resize(ietasize);
  eventData.sumEHFPlusVsiEta_.resize(ietasize);
  eventData.sumEHFMinusVsiEta_.resize(ietasize);
  eventData.sumETHFPlusVsiEta_.resize(ietasize);
  eventData.sumETHFMinusVsiEta_.resize(ietasize);

  fillEventInfo(eventData,event,setup);
  fillNoiseInfo(eventData,event,setup); 
  fillTriggerInfo(eventData,event,setup);
  fillVertexInfo(eventData,event,setup);
  fillTrackInfo(eventData,event,setup);
  fillJetInfo(eventData,event,setup);
  fillMETInfo(eventData,event,setup);
  fillCaloTowerInfo(eventData,event,setup);
  fillCastorInfo(eventData,event,setup);
  fillDiffVariables(eventData,event,setup);
  fillGenInfo(eventData,event,setup);
}

void DiffractiveAnalysis::fillEventInfo(DiffractiveEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  if(accessMCInfo_){
     edm::Handle<GenEventInfoProduct> genEventInfo;
     event.getByLabel("generator",genEventInfo);

     int processId = -1;
     if(genEventInfo.isValid()){
        processId = genEventInfo->signalProcessID();
     } else {
        edm::Handle<edm::HepMCProduct> hepMCProduct;
        event.getByLabel("source",hepMCProduct);
        processId = hepMCProduct->getHepMCData().signal_process_id();
     }
     eventData.processId_ = processId;
  } else{
     eventData.processId_ = -1;
  } 

  unsigned int eventNumber = event.id().event();
  unsigned int runNumber = event.id().run();
  unsigned int lumiSection = event.luminosityBlock();
  int bunchCrossing = event.bunchCrossing();

  edm::Handle<double> lumiWeight;
  event.getByLabel("lumiWeight",lumiWeight);
  if(lumiWeight.isValid()){
     double lumi = *lumiWeight;
     eventData.lumiWeight_ = lumi;
  } else{
     eventData.lumiWeight_ = -1.; 
  }

  eventData.eventNumber_ = eventNumber;
  eventData.runNumber_ = runNumber;
  eventData.lumiSection_ = lumiSection;
  eventData.bunchCrossing_ = bunchCrossing;
}

void DiffractiveAnalysis::fillNoiseInfo(DiffractiveEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<HcalNoiseSummary> noiseSummaryH;
  event.getByLabel("hcalnoise",noiseSummaryH);   

  bool passNoiseLoose = noiseSummaryH->passLooseNoiseFilter();
  bool passNoiseTight = noiseSummaryH->passTightNoiseFilter();

  edm::Handle<reco::BeamHaloSummary> beamHaloSummaryH;
  event.getByLabel("BeamHaloSummary",beamHaloSummaryH);

  bool beamHaloLooseId = beamHaloSummaryH->LooseId(); 
  bool beamHaloTightId = beamHaloSummaryH->TightId();

  eventData.LooseNoiseFilter_ = passNoiseLoose ? 1 : 0;
  eventData.TightNoiseFilter_ = passNoiseTight ? 1 : 0;

  eventData.BeamHaloLooseId_ = beamHaloLooseId ? 1 : 0;
  eventData.BeamHaloTightId_ = beamHaloTightId ? 1 : 0;
}

void DiffractiveAnalysis::fillTriggerInfo(DiffractiveEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<edm::TriggerResults> triggerResults;
  event.getByLabel(triggerResultsTag_, triggerResults);

  if(triggerResults.isValid() && hltPathName_ != ""){
     const edm::TriggerNames& triggerNames = event.triggerNames(*triggerResults);
     // In case hltPathName_ is a pattern (e.g. HLT_Jet30U*)
     std::string hltPath;
     if( edm::is_glob(hltPathName_) ){
	std::vector< std::vector<std::string>::const_iterator > matches = edm::regexMatch(triggerNames.triggerNames(), hltPathName_);  

	if( matches.empty() ) throw cms::Exception("Configuration") << "Could not find any HLT path of type " << hltPathName_ << "\n";
	else if( matches.size() > 1) throw cms::Exception("Configuration") << "HLT path type " << hltPathName_ << " not unique\n";
	else hltPath = *(matches[0]);
     } else{
	hltPath = hltPathName_; 
     } 

     unsigned int idxHLT = triggerNames.triggerIndex(hltPath);
   //idxHLT must be less than the size of HLTR or you get a CMSException: _M_range_check(SFonseca 10/04/2011)
   //Ref: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideEgammaHLT#Retrieving_offline_the_HLT_infor

     if (idxHLT < triggerResults->size()) eventData.HLTPath_ = (triggerResults->wasrun(idxHLT) && triggerResults->accept(idxHLT)) ? 1 : 0; 
     else {
   
      edm::LogWarning("Analysis")<<" Trigger index: "<< idxHLT <<" Trigger Results Size: "<< triggerResults->size()  
     <<"  Trigger index  must be equal/more that the Trigger Results !! ";
      eventData.HLTPath_ = -1;
    
       }
  } else{
     eventData.HLTPath_ = -1;
  }

  edm::Handle<L1GlobalTriggerReadoutRecord> gtReadoutRecordH;
  event.getByLabel("gtDigis", gtReadoutRecordH);
  
  if( gtReadoutRecordH.isValid() ){
     const L1GlobalTriggerReadoutRecord* gtReadoutRecord = gtReadoutRecordH.product();
     const TechnicalTriggerWord&  technicalTriggerWordBeforeMask = gtReadoutRecord->technicalTriggerWord();
     //const unsigned int numberTechnicalTriggerBits( technicalTriggerWordBeforeMask.size() );
     
     bool passL1TT = technicalTriggerWordBeforeMask.at(ttBit_);
     eventData.TTBit_ = (int)passL1TT;
  } else{
     eventData.TTBit_ = -1;
  }

}

void DiffractiveAnalysis::fillVertexInfo(DiffractiveEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
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

  if( nGoodVertices ){
     const reco::Vertex& primVertex = vtxColl[0];
     eventData.posXPrimVtx_ = primVertex.x();
     eventData.posYPrimVtx_ = primVertex.y();
     eventData.posZPrimVtx_ = primVertex.z();
  } else{
     eventData.posXPrimVtx_ = -999.;
     eventData.posYPrimVtx_ = -999.;
     eventData.posZPrimVtx_ = -999.;
  }

}

void DiffractiveAnalysis::fillTrackInfo(DiffractiveEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  // Access collection
  edm::Handle<edm::View<reco::Track> > trackCollectionH;
  event.getByLabel(trackTag_,trackCollectionH);
  const edm::View<reco::Track>& trackColl = *(trackCollectionH.product());

  double ptSum = 0.;
  int nTracks = 0; 
  edm::View<reco::Track>::const_iterator track = trackColl.begin();
  edm::View<reco::Track>::const_iterator tracks_end = trackColl.end();
  for(; track != tracks_end; ++track){
     ptSum += track->pt();
     ++nTracks;
     // Other variables..
  }

  eventData.multiplicityTracks_ = nTracks;
  eventData.sumPtTracks_ = ptSum;

  if(accessMCInfo_){
     edm::Handle<reco::GenParticleCollection> genChargedParticlesH;
     event.getByLabel(genChargedTag_,genChargedParticlesH);
     const reco::GenParticleCollection& genChargedParticles = *genChargedParticlesH;
     reco::GenParticleCollection::const_iterator genpart = genChargedParticles.begin();
     reco::GenParticleCollection::const_iterator genparts_end = genChargedParticles.end();

     double ptSumGen = 0.;
     int nTracksGen = 0;
     for(; genpart != genparts_end; ++genpart){
        ptSumGen += genpart->pt();
        ++nTracksGen; 
        //...
     }
     eventData.multiplicityTracksGen_ = nTracksGen;
     eventData.sumPtTracksGen_ = ptSumGen;
  } else{
     eventData.multiplicityTracksGen_ = -1;
     eventData.sumPtTracksGen_ = -1.;
  }
}

void DiffractiveAnalysis::fillMETInfo(DiffractiveEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<edm::View<reco::MET> > metCollectionH;
  event.getByLabel(metTag_,metCollectionH);

  const reco::MET& met = (*metCollectionH)[0];
  double sumET = met.sumEt();
  double met_px = met.px();
  double met_py = met.py();
  double met_et = met.pt();
  eventData.sumET_ = sumET;
  eventData.MEx_ = met_px;
  eventData.MEy_ = met_py;
  eventData.MET_ = met_et;
}

void DiffractiveAnalysis::fillJetInfo(DiffractiveEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  edm::Handle<edm::View<reco::Jet> > jetCollectionH;
  event.getByLabel(jetTag_,jetCollectionH);

  if(jetCollectionH->size() > 0){
     const reco::Jet& leadingJet = (*jetCollectionH)[0];

     eventData.leadingJetPt_ = leadingJet.pt();
     eventData.leadingJetEta_ = leadingJet.eta();
     eventData.leadingJetPhi_ = leadingJet.phi();
  } else{
     eventData.leadingJetPt_ = -999.;
     eventData.leadingJetEta_ = -999.;
     eventData.leadingJetPhi_ = -999.;
  }  
}

void DiffractiveAnalysis::fillCaloTowerInfo(DiffractiveEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  // Access multiplicities
  edm::Handle<std::vector<unsigned int> > nHEPlus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHEplus"),nHEPlus);
  
  edm::Handle<std::vector<unsigned int> > nHEMinus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHEminus"),nHEMinus);

  edm::Handle<std::vector<unsigned int> > nHFPlus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHFplus"),nHFPlus);
  
  edm::Handle<std::vector<unsigned int> > nHFMinus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"nHFminus"),nHFMinus);

  edm::Handle<std::map<unsigned int, std::vector<unsigned int> > > iEtaHFMultiplicityPlus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFMultiplicityPlus"),iEtaHFMultiplicityPlus);

  edm::Handle<std::map<unsigned int, std::vector<unsigned int> > > iEtaHFMultiplicityMinus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFMultiplicityMinus"),iEtaHFMultiplicityMinus);
  
  edm::Handle<std::vector<double> > sumEHEplus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHEplus"),sumEHEplus);

  edm::Handle<std::vector<double> > sumEHEminus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHEminus"),sumEHEminus);

  edm::Handle<std::vector<double> > sumETHEplus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHEplus"),sumETHEplus);
 
  edm::Handle<std::vector<double> > sumETHEminus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHEminus"),sumETHEminus);

  edm::Handle<std::vector<double> > sumEHFplus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHFplus"),sumEHFplus);

  edm::Handle<std::vector<double> > sumEHFminus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumEHFminus"),sumEHFminus);

  edm::Handle<std::map<unsigned int, std::vector<double> > > iEtaHFEnergySumPlus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFEnergySumPlus"),iEtaHFEnergySumPlus);

  edm::Handle<std::map<unsigned int, std::vector<double> > > iEtaHFEnergySumMinus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFEnergySumMinus"),iEtaHFEnergySumMinus);

  edm::Handle<std::vector<double> > sumETHFplus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHFplus"),sumETHFplus);
  
  edm::Handle<std::vector<double> > sumETHFminus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"sumETHFminus"),sumETHFminus);
 
  edm::Handle<std::map<unsigned int, std::vector<double> > > iEtaHFETSumPlus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFETSumPlus"),iEtaHFETSumPlus);

  edm::Handle<std::map<unsigned int, std::vector<double> > > iEtaHFETSumMinus;
  event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"iEtaHFETSumMinus"),iEtaHFETSumMinus);

  if( iEtaHFMultiplicityPlus.isValid() ){
     // FIXME
     edm::Handle<std::vector<double> > thresholdsHE;
     event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"thresholdsHE"),thresholdsHE);
     size_t indexThresholdHE = std::lower_bound((*thresholdsHE).begin(),(*thresholdsHE).end(),energyThresholdHE_) - (*thresholdsHE).begin();

     edm::Handle<std::vector<double> > thresholdsHF;
     event.getByLabel(edm::InputTag(hcalTowerSummaryTag_.label(),"thresholdsHF"),thresholdsHF);
     size_t indexThresholdHF = std::lower_bound((*thresholdsHF).begin(),(*thresholdsHF).end(),energyThresholdHF_) - (*thresholdsHF).begin();
     
     unsigned int nHE_plus = (*nHEPlus)[indexThresholdHE];
     unsigned int nHE_minus = (*nHEMinus)[indexThresholdHE];

     unsigned int nHF_plus = (*nHFPlus)[indexThresholdHF];
     unsigned int nHF_minus = (*nHFMinus)[indexThresholdHF];

     double sumEHE_plus = (*sumEHEplus)[indexThresholdHE];
     double sumEHE_minus = (*sumEHEminus)[indexThresholdHE];

     double sumETHE_plus = (*sumETHEplus)[indexThresholdHE];
     double sumETHE_minus = (*sumETHEminus)[indexThresholdHE];

     double sumEHF_plus = (*sumEHFplus)[indexThresholdHF];
     double sumEHF_minus = (*sumEHFminus)[indexThresholdHF];

     double sumETHF_plus = (*sumETHFplus)[indexThresholdHF];
     double sumETHF_minus = (*sumETHFminus)[indexThresholdHF];
    
     eventData.multiplicityHEPlus_ = nHE_plus;
     eventData.multiplicityHEMinus_ = nHE_minus;
     eventData.multiplicityHFPlus_ = nHF_plus;
     eventData.multiplicityHFMinus_ = nHF_minus;
     eventData.sumEnergyHEPlus_ = sumEHE_plus;
     eventData.sumEnergyHEMinus_ = sumEHE_minus;
     eventData.sumETHEPlus_ = sumETHE_plus;
     eventData.sumETHEMinus_ = sumETHE_minus;
     eventData.sumEnergyHFPlus_ = sumEHF_plus;
     eventData.sumEnergyHFMinus_ = sumEHF_minus;
     eventData.sumETHFPlus_ = sumETHF_plus;
     eventData.sumETHFMinus_ = sumETHF_minus;

     for(unsigned int ieta = 29, index = 0; ieta <= 41; ++ieta,++index){
	unsigned int nHFPlus_ieta = nHCALiEta(*iEtaHFMultiplicityPlus,indexThresholdHF,ieta);
	eventData.multiplicityHFPlusVsiEta_[index] = nHFPlus_ieta;
	double sumEHFPlus_ieta = sumEHCALiEta(*iEtaHFEnergySumPlus,indexThresholdHF,ieta);
	eventData.sumEHFPlusVsiEta_[index] = sumEHFPlus_ieta;
	double sumETHFPlus_ieta = sumEHCALiEta(*iEtaHFETSumPlus,indexThresholdHF,ieta);
	eventData.sumETHFPlusVsiEta_[index] = sumETHFPlus_ieta; 

	unsigned int nHFMinus_ieta = nHCALiEta(*iEtaHFMultiplicityMinus,indexThresholdHF,ieta);
	eventData.multiplicityHFMinusVsiEta_[index] = nHFMinus_ieta;
	double sumEHFMinus_ieta = sumEHCALiEta(*iEtaHFEnergySumMinus,indexThresholdHF,ieta);
	eventData.sumEHFMinusVsiEta_[index] = sumEHFMinus_ieta;     
	double sumETHFMinus_ieta = sumEHCALiEta(*iEtaHFETSumMinus,indexThresholdHF,ieta);
	eventData.sumETHFMinusVsiEta_[index] = sumETHFMinus_ieta;
     }
  }

  //...
  edm::Handle<CaloTowerCollection> caloTowerCollectionH;
  event.getByLabel(caloTowerTag_,caloTowerCollectionH);

  if( caloTowerCollectionH.isValid() ){
     double energyScale = (applyEnergyScaleHCAL_) ? energyScaleHCAL_ : -1.;
     double MxFromTowers = MassColl(*caloTowerCollectionH,-1.,energyThresholdHB_,energyThresholdHE_,energyThresholdHF_,energyScale);
     
     std::pair<double,double> xiFromTowers = Xi(*caloTowerCollectionH,Ebeam_,-1.,energyThresholdHB_,energyThresholdHE_,energyThresholdHF_,energyScale);
     double xiFromTowers_plus = xiFromTowers.first;
     double xiFromTowers_minus = xiFromTowers.second;

     std::pair<double,double> EPlusPzFromTowers = EPlusPz(*caloTowerCollectionH,-1.,energyThresholdHB_,energyThresholdHE_,energyThresholdHF_,energyScale);

     eventData.MxFromTowers_ = MxFromTowers;
     eventData.xiPlusFromTowers_ = xiFromTowers_plus;
     eventData.xiMinusFromTowers_ = xiFromTowers_minus;
     eventData.EPlusPzFromTowers_ = EPlusPzFromTowers.first;
     eventData.EMinusPzFromTowers_ = EPlusPzFromTowers.second;
  } else{
     eventData.MxFromTowers_ = -999.;
     eventData.xiPlusFromTowers_ = -999.;
     eventData.xiMinusFromTowers_ = -999.;
     eventData.EPlusPzFromTowers_ = -999.;
     eventData.EMinusPzFromTowers_ = -999.;
  }

}

void DiffractiveAnalysis::fillGenInfo(DiffractiveEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
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
     if(genGapHighEdge == math::XYZTLorentzVector(0.,0.,0.,0.) || genGapLowEdge == math::XYZTLorentzVector(0.,0.,0.,0.)) deltaEtaGen = -999.;
     else deltaEtaGen = genGapHighEdge.eta() - genGapLowEdge.eta(); 
     double etaGapLow = (genGapLowEdge == math::XYZTLorentzVector(0.,0.,0.,0.)) ? -999. : genGapLowEdge.eta();
     double etaGapHigh = (genGapHighEdge == math::XYZTLorentzVector(0.,0.,0.,0.)) ? -999. : genGapHighEdge.eta();

     LogDebug("Analysis|DiffractiveAnalysis") << "Gap low,high = " << genGapLowEdge << " , " << genGapHighEdge;
 
     eventData.xiGenPlus_ = xigen_plus;
     eventData.xiGenMinus_ = xigen_minus;
     eventData.MxGen_ = genAllParticles.mass();
     eventData.MxGenRange_ = genAllParticlesInRange.mass(); 
     eventData.sumEnergyHEPlusGen_ = genAllParticlesHEPlus.energy();
     eventData.sumEnergyHEMinusGen_ = genAllParticlesHEMinus.energy();
     eventData.sumEnergyHFPlusGen_ = genAllParticlesHFPlus.energy();
     eventData.sumEnergyHFMinusGen_ = genAllParticlesHFMinus.energy();
     eventData.etaMaxGen_ = genEtaMax.eta();
     eventData.etaMinGen_ = genEtaMin.eta();

     eventData.deltaEtaGen_ = deltaEtaGen;
     eventData.etaGapLow_ = etaGapLow;
     eventData.etaGapHigh_ = etaGapHigh;
     eventData.MxGenPlus_ = massDissGenPlus;
     eventData.MxGenMinus_ = massDissGenMinus;
 
     // Access variables from event 
     edm::Handle<std::vector<float> > edmNtupleMxGen;
     event.getByLabel(edm::InputTag("edmNtupleMxGen","Mx"),edmNtupleMxGen);
 
     eventData.MxGenDiss_ = (edmNtupleMxGen.isValid() && edmNtupleMxGen->size()) ? (*edmNtupleMxGen)[0] : -999.;
     eventData.MyGenDiss_ = (edmNtupleMxGen.isValid() && edmNtupleMxGen->size() > 1) ? (*edmNtupleMxGen)[1] : -999.;

   
  } else{
     eventData.xiGenPlus_ = -1.;
     eventData.xiGenMinus_ = -1.;
     eventData.MxGen_ = -1.;
     eventData.MxGenDiss_ = -1.;
     eventData.MyGenDiss_ = -1.;
     eventData.MxGenRange_ = -1.;
     eventData.MxGenPlus_ = -1.; 
     eventData.MxGenMinus_ = -1.;
     eventData.deltaEtaGen_ = -1.; 
     eventData.etaGapLow_ = -999.;
     eventData.etaGapHigh_ = -999.;
     eventData.sumEnergyHEPlusGen_ = -1.;
     eventData.sumEnergyHEMinusGen_ = -1.;
     eventData.sumEnergyHFPlusGen_ = -1.;
     eventData.sumEnergyHFMinusGen_ = -1.;
     eventData.etaMaxGen_ = -999.;
     eventData.etaMinGen_ = -999.;
  }

}

void DiffractiveAnalysis::fillDiffVariables(DiffractiveEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  

  // Leave only PF-based variables
  edm::Handle<reco::PFCandidateCollection> particleFlowCollectionH;
  event.getByLabel(particleFlowTag_,particleFlowCollectionH);

 
  double MxFromPFCands = MassColl(*particleFlowCollectionH);
  eventData.MxFromPFCands_ = MxFromPFCands;

  std::pair<double,double> xiFromPFCands = Xi(*particleFlowCollectionH,Ebeam_);
  double xiFromPFCands_plus = xiFromPFCands.first;
  double xiFromPFCands_minus = xiFromPFCands.second;

  eventData.xiPlusFromPFCands_ = xiFromPFCands_plus;
  eventData.xiMinusFromPFCands_ = xiFromPFCands_minus;

  
  std::pair<double,double> EPlusPzFromPFCands = EPlusPz(*particleFlowCollectionH);
  eventData.EPlusPzFromPFCands_ = EPlusPzFromPFCands.first;
  eventData.EMinusPzFromPFCands_ = EPlusPzFromPFCands.second;

  edm::Handle<std::vector<float> > edmNtupleEtaMax;
  event.getByLabel(edm::InputTag("edmNtupleEtaMax","etaMax"),edmNtupleEtaMax);

  edm::Handle<std::vector<float> > edmNtupleEtaMin;
  event.getByLabel(edm::InputTag("edmNtupleEtaMin","etaMin"),edmNtupleEtaMin);

  float etaMax_pfCands = edmNtupleEtaMax->size() ? (*edmNtupleEtaMax)[0] : -999.;
  float etaMin_pfCands = edmNtupleEtaMin->size() ? (*edmNtupleEtaMin)[0] : -999.; 
  eventData.etaMaxFromPFCands_ = etaMax_pfCands;
  eventData.etaMinFromPFCands_ = etaMin_pfCands;
}

void DiffractiveAnalysis::fillCastorInfo(DiffractiveEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){
  // Castor RecHit collection
  edm::Handle<CastorRecHitCollection> castorRecHitCollectionH;
  event.getByLabel(castorRecHitTag_,castorRecHitCollectionH);

  if( castorRecHitCollectionH.isValid() ){
     int modules[] = {1,2,3,4,5};
     std::vector<int> vec_modules(modules, modules + sizeof(modules)/sizeof(int));

     double sumETotCastor = CastorEnergy(vec_modules)(*castorRecHitCollectionH,event.isRealData());
     eventData.sumETotCastor_ = sumETotCastor;
  }
}

template <class PartColl>
double DiffractiveAnalysis::MassColl(PartColl& partCollection, double ptThreshold,
                                     double energyHBThreshold, double energyHEThreshold,
				     double energyHFThreshold, double energyScale){
   math::XYZTLorentzVector allCands(0.,0.,0.,0.);
   for(typename PartColl::const_iterator part = partCollection.begin();
	 part != partCollection.end(); ++part){
      double part_pt = part->pt();
      double part_energy = part->energy();
      if(energyScale > 0.){
	 part_pt *= energyScale;
	 part_energy *= energyScale;
      }

      // HF eta rings 29, 40, 41
      if( ( (fabs(part->eta()) >= 2.866) && (fabs(part->eta()) < 2.976) ) || 
	    (fabs(part->eta()) >= 4.730) ) continue;

      if(part_pt < ptThreshold) continue;
      if((fabs(part->eta()) < 1.3) && (part_energy < energyHBThreshold)) continue;
      if(((fabs(part->eta()) >= 1.3) && (fabs(part->eta()) < 3.0)) && (part_energy < energyHEThreshold)) continue;
      if((fabs(part->eta()) >= 3.0) && ((fabs(part->eta()) <= 5.0)) && (part_energy < energyHFThreshold)) continue;
      if(energyScale > 0.) allCands += energyScale*part->p4();
      else allCands += part->p4();
   }

   return allCands.M();
}

template <class Coll>
std::pair<double,double> DiffractiveAnalysis::Xi(Coll& partCollection, double Ebeam,
                                                 double ptThreshold,
                                                 double energyHBThreshold, double energyHEThreshold,
						 double energyHFThreshold, double energyScale){

   double xi_towers_plus = 0.;
   double xi_towers_minus = 0.;
   for(typename Coll::const_iterator part = partCollection.begin(); part != partCollection.end(); ++part){

      double part_pt = part->pt();
      double part_energy = part->energy();
      if(energyScale > 0.){
	 part_pt *= energyScale;
	 part_energy *= energyScale;
      }

      // HF eta rings 29, 40, 41
      if( ( (fabs(part->eta()) >= 2.866) && (fabs(part->eta()) < 2.976) ) || 
	    (fabs(part->eta()) >= 4.730) ) continue;

      if(part_pt < ptThreshold) continue;
      if((fabs(part->eta()) < 1.3) && (part_energy < energyHBThreshold)) continue;
      if(((fabs(part->eta()) >= 1.3) && (fabs(part->eta()) < 3.0)) && (part_energy < energyHEThreshold)) continue;
      if((fabs(part->eta()) >= 3.0) && ((fabs(part->eta()) <= 5.0)) && (part_energy < energyHFThreshold)) continue;

      double part_et = part->et();
      double part_eta = part->eta();
      if(energyScale > 0.) part_et *= energyScale;

      xi_towers_plus += part_et*TMath::Exp(part_eta);
      xi_towers_minus += part_et*TMath::Exp(-part_eta);
   }

   xi_towers_plus /= 2*Ebeam;
   xi_towers_minus /= 2*Ebeam;

   return std::make_pair(xi_towers_plus,xi_towers_minus);
}

template <class Coll>
std::pair<double,double> DiffractiveAnalysis::EPlusPz(Coll& partCollection,
                                                      double ptThreshold,
                                                      double energyHBThreshold,
                                                      double energyHEThreshold,
				                      double energyHFThreshold,
                                                      double energyScale){
   double e_plus_pz = 0.;
   double e_minus_pz = 0.;
   typename Coll::const_iterator part = partCollection.begin();
   typename Coll::const_iterator part_end = partCollection.end();
   for(; part != part_end; ++part){
      double part_pt = part->pt();
      double part_energy = part->energy();
      double part_pz = part->pz();
      if(energyScale > 0.){
	 part_pt *= energyScale;
	 part_energy *= energyScale;
	 part_pz *= energyScale;
      }

      // HF eta rings 29, 40, 41
      if( ( (fabs(part->eta()) >= 2.866) && (fabs(part->eta()) < 2.976) ) || 
	    (fabs(part->eta()) >= 4.730) ) continue;

      if(part_pt < ptThreshold) continue;
      if((fabs(part->eta()) < 1.3) && (part_energy < energyHBThreshold)) continue;
      if(((fabs(part->eta()) >= 1.3) && (fabs(part->eta()) < 3.0)) && (part_energy < energyHEThreshold)) continue; 
      if((fabs(part->eta()) >= 3.0) && ((fabs(part->eta()) <= 5.0)) && (part_energy < energyHFThreshold)) continue;

      e_plus_pz += part_energy + part_pz; 
      e_minus_pz += part_energy - part_pz;
   }

   return std::make_pair(e_plus_pz,e_minus_pz);
}

unsigned int nHFSlice(const std::map<unsigned int, std::vector<unsigned int> >& mapThreshToiEta, unsigned int thresholdHF, unsigned int ieta){
   const std::vector<unsigned int>& vec_iEta = mapThreshToiEta.find(thresholdHF)->second;

   // Count number of ieta entries in vector 
   int count_ieta = (int)std::count(vec_iEta.begin(),vec_iEta.end(),ieta);

   return count_ieta;
}

unsigned int DiffractiveAnalysis::nHCALiEta(const std::map<unsigned int, std::vector<unsigned int> >& iEtaMultiplicity, unsigned int threshold, unsigned int ieta){

   std::map<unsigned int, std::vector<unsigned int> >::const_iterator it_ieta = iEtaMultiplicity.find(ieta);
   unsigned int count_ieta = 0; 
   if( it_ieta != iEtaMultiplicity.end() ) count_ieta = (it_ieta->second)[threshold];

   return count_ieta;
}

double DiffractiveAnalysis::sumEHCALiEta(const std::map<unsigned int, std::vector<double> >& iEtaSumE, unsigned int threshold, unsigned int ieta){

   std::map<unsigned int, std::vector<double> >::const_iterator it_ieta = iEtaSumE.find(ieta);
   double sumE_ieta = 0;
   if( it_ieta != iEtaSumE.end() ) sumE_ieta = (it_ieta->second)[threshold];

   return sumE_ieta;
}

double DiffractiveAnalysis::MassDissGen(reco::GenParticleCollection const& genParticles, double rangeEtaMin,
                                                                    double rangeEtaMax){
                                                                    
   math::XYZTLorentzVector allGenParticles(0.,0.,0.,0.);
   reco::GenParticleCollection::const_iterator genpart = genParticles.begin();
   reco::GenParticleCollection::const_iterator genpart_end = genParticles.end();
   for(; genpart != genpart_end; ++genpart){
      if( genpart->status() != 1 ) continue;

      if( ( genpart->eta() >= (rangeEtaMin - 0.0001) ) && 
          ( genpart->eta() <= (rangeEtaMax + 0.0001) ) ) allGenParticles += genpart->p4();
   }
   return allGenParticles.M();
}

void DiffractiveAnalysis::setGenInfo(reco::GenParticleCollection const& genParticles, double Ebeam,
                                                                 math::XYZTLorentzVector& genAllParticles,
                                                                 math::XYZTLorentzVector& genAllParticlesInRange,
                                                                 math::XYZTLorentzVector& genAllParticlesHEPlus,
                                                                 math::XYZTLorentzVector& genAllParticlesHEMinus,
                                                                 math::XYZTLorentzVector& genAllParticlesHFPlus,
                                                                 math::XYZTLorentzVector& genAllParticlesHFMinus,
                                                                 math::XYZTLorentzVector& genEtaMax,
                                                                 math::XYZTLorentzVector& genEtaMin,
                                                                 math::XYZTLorentzVector& genProtonPlus,
								 math::XYZTLorentzVector& genProtonMinus){

   math::XYZTLorentzVector allGenParticles(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesInRange(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesHEPlus(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesHEMinus(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesHFPlus(0.,0.,0.,0.);
   math::XYZTLorentzVector allGenParticlesHFMinus(0.,0.,0.,0.);

   reco::GenParticleCollection::const_iterator proton1 = genParticles.end();
   reco::GenParticleCollection::const_iterator proton2 = genParticles.end();
   for(reco::GenParticleCollection::const_iterator genpart = genParticles.begin();
                                                   genpart != genParticles.end();
	                                           ++genpart){
      if( genpart->status() != 1 ) continue;
      if( genpart->pdgId() != 2212 ) continue;

      if( ( genpart->pz() > 0.50*Ebeam ) && ( ( proton1 == genParticles.end() ) ||
	       ( genpart->pz() > proton1->pz() ) ) ) proton1 = genpart;
      if( ( genpart->pz() < -0.50*Ebeam ) && ( ( proton2 == genParticles.end() ) ||
	       ( genpart->pz() < proton2->pz() ) ) ) proton2 = genpart;
   }

   reco::GenParticleCollection::const_iterator etaMaxParticle = genParticles.end();
   reco::GenParticleCollection::const_iterator etaMinParticle = genParticles.end(); 
   for(reco::GenParticleCollection::const_iterator genpart = genParticles.begin(); genpart != genParticles.end(); ++genpart){
      if(genpart->status() != 1) continue;

      allGenParticles += genpart->p4();
      if(fabs(genpart->eta()) < 5.0) allGenParticlesInRange += genpart->p4();
      if( (genpart->eta() >= 1.3) && (genpart->eta() < 3.0) ) allGenParticlesHEPlus += genpart->p4();
      if( (genpart->eta() > -3.0) && (genpart->eta() <= -1.3) ) allGenParticlesHEMinus += genpart->p4();
      if( (genpart->eta() >= 3.0) && (genpart->eta() < 5.0) ) allGenParticlesHFPlus += genpart->p4();
      if( (genpart->eta() > -5.0) && (genpart->eta() <= -3.0) ) allGenParticlesHFMinus += genpart->p4(); 

      if( (genpart != proton1) && (genpart != proton2) ){
	 if( ( etaMaxParticle == genParticles.end() ) ||
	       ( genpart->eta() > etaMaxParticle->eta() ) ) etaMaxParticle = genpart;
	 if( ( etaMinParticle == genParticles.end() ) ||
	       ( genpart->eta() < etaMinParticle->eta() ) ) etaMinParticle = genpart;
      }
   }

   // Commit
   if( proton1 != genParticles.end() ) allGenParticles -= proton1->p4();
   if( proton2 != genParticles.end() ) allGenParticles -= proton2->p4();

   genAllParticles = allGenParticles;
   genAllParticlesInRange = allGenParticlesInRange;
   genAllParticlesHEPlus = allGenParticlesHEPlus;
   genAllParticlesHEMinus = allGenParticlesHEMinus;
   genAllParticlesHFPlus = allGenParticlesHFPlus;
   genAllParticlesHFMinus = allGenParticlesHFMinus;

   if( proton1 != genParticles.end() ) genProtonPlus = proton1->p4();
   if( proton2 != genParticles.end() ) genProtonMinus = proton2->p4();

   if( etaMaxParticle != genParticles.end() ) genEtaMax = etaMaxParticle->p4();
   if( etaMinParticle != genParticles.end() ) genEtaMin = etaMinParticle->p4();
}
