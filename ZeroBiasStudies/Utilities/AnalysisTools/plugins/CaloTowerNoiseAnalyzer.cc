// user include files
#include "CaloTowerNoiseAnalyzer.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoCaloTowerCandidate.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"

#include "Geometry/CaloTopology/interface/HcalTopology.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h" 
#include "TTree.h"

using namespace reco;
 
CaloTowerNoiseAnalyzer::CaloTowerNoiseAnalyzer(const edm::ParameterSet& conf):
  calotowersTag_(conf.getParameter<edm::InputTag>("CaloTowersTag")),
  accessRecHits_(conf.getUntrackedParameter<bool>("AccessRecHits",false)),
  nThresholdIter_(conf.getParameter<unsigned int>("NumberOfTresholds")),
  eThresholdHBMin_(conf.getParameter<double>("TowerEnergyTresholdHBMin")),
  eThresholdHBMax_(conf.getParameter<double>("TowerEnergyTresholdHBMax")),
  eThresholdHEMin_(conf.getParameter<double>("TowerEnergyTresholdHEMin")),
  eThresholdHEMax_(conf.getParameter<double>("TowerEnergyTresholdHEMax")),
  eThresholdHFMin_(conf.getParameter<double>("TowerEnergyTresholdHFMin")),
  eThresholdHFMax_(conf.getParameter<double>("TowerEnergyTresholdHFMax")),
  eThresholdEBMin_(conf.getParameter<double>("TowerEnergyTresholdEBMin")),
  eThresholdEBMax_(conf.getParameter<double>("TowerEnergyTresholdEBMax")),
  eThresholdEEMin_(conf.getParameter<double>("TowerEnergyTresholdEEMin")),
  eThresholdEEMax_(conf.getParameter<double>("TowerEnergyTresholdEEMax")), 
  nBinsHB_(conf.getUntrackedParameter<int>("NBinsHB",20)),
  nBinsHE_(conf.getUntrackedParameter<int>("NBinsHE",20)),
  nBinsHF_(conf.getUntrackedParameter<int>("NBinsHF",20)),	
  nBinsEB_(conf.getUntrackedParameter<int>("NBinsEB",20)),
  nBinsEE_(conf.getUntrackedParameter<int>("NBinsEE",20)),
  reweightHFTower_(conf.getParameter<bool>("ReweightHFTower")),
  eThresholdHB_(std::vector<double>()),
  eThresholdHE_(std::vector<double>()),
  eThresholdHF_(std::vector<double>()),
  eThresholdEB_(std::vector<double>()),
  eThresholdEE_(std::vector<double>())  {

  if(accessRecHits_) hfRecHitsTag_ = conf.getUntrackedParameter<edm::InputTag>("HFRecHitsLabel",edm::InputTag("hfreco"));

  // reweightHistoName_[0] --> file name
  // reweightHistoName_[1] --> histo path in file
  if(reweightHFTower_){
     reweightHistoName_ = conf.getParameter<std::vector<std::string> >("ReweightHistoName");

     // Access and close file; keep hard copy of histo
     edm::LogVerbatim("Analysis") << "Accessing file " << reweightHistoName_[0] << " histo " << reweightHistoName_[1];
     TFile file(reweightHistoName_[0].c_str(),"read");
     TH1F* histo = static_cast<TH1F*>(file.Get(reweightHistoName_[1].c_str())); 
     reweightHisto_ = *histo;
  }
 
}  
  
CaloTowerNoiseAnalyzer::~CaloTowerNoiseAnalyzer()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}

void CaloTowerNoiseAnalyzer::beginJob(){

  edm::Service<TFileService> fs;
  TH1::SetDefaultSumw2(true);

  histosTH1F_["NEvents"] = fs->make<TH1F>("NEvents","NEvents",1,0,1);

  char hname[50];
  for(unsigned int i = 0; i < nThresholdIter_; ++i){
     eThresholdHB_.push_back(eThresholdHBMin_ + i*((eThresholdHBMax_ - eThresholdHBMin_)/((double)nThresholdIter_))); 
     eThresholdHE_.push_back(eThresholdHEMin_ + i*((eThresholdHEMax_ - eThresholdHEMin_)/((double)nThresholdIter_)));
     eThresholdHF_.push_back(eThresholdHFMin_ + i*((eThresholdHFMax_ - eThresholdHFMin_)/((double)nThresholdIter_)));
     eThresholdEB_.push_back(eThresholdEBMin_ + i*((eThresholdEBMax_ - eThresholdEBMin_)/((double)nThresholdIter_)));
     eThresholdEE_.push_back(eThresholdEEMin_ + i*((eThresholdEEMax_ - eThresholdEEMin_)/((double)nThresholdIter_)));

     edm::LogVerbatim("Analysis") << "Threshold HB" << i << ": " << eThresholdHB_[i] << "\n"
                                  << "Threshold HE" << i << ": " << eThresholdHE_[i] << "\n"
                                  << "Threshold HF" << i << ": " << eThresholdHF_[i] << "\n"
                                  << "Threshold HB" << i << ": " << eThresholdEB_[i] << "\n"
                                  << "Threshold HE" << i << ": " << eThresholdEE_[i];
     // HB
     sprintf(hname,"nHBPlus_%d",i);
     histosNHBPlus_.push_back(fs->make<TH1F>(hname,"Towers mult. HB plus",nBinsHB_,0,nBinsHB_));
     sprintf(hname,"nHBMinus_%d",i);
     histosNHBMinus_.push_back(fs->make<TH1F>(hname,"Towers mult. HB minus",nBinsHB_,0,nBinsHB_));
     sprintf(hname,"sumEHBplus_%d",i);
     histosSumEHBPlus_.push_back(fs->make<TH1F>(hname,"Energy sum HB plus",100,0.,200.));
     sprintf(hname,"sumEHBminus_%d",i);
     histosSumEHBMinus_.push_back(fs->make<TH1F>(hname,"Energy sum HB minus",100,0.,200.));
     // HE
     sprintf(hname,"nHEplus_%d",i);
     histosNHEPlus_.push_back(fs->make<TH1F>(hname,"Towers mult. HE plus",nBinsHE_,0,nBinsHE_));
     sprintf(hname,"nHEminus_%d",i);
     histosNHEMinus_.push_back(fs->make<TH1F>(hname,"Towers mult. HE minus",nBinsHE_,0,nBinsHE_));
     sprintf(hname,"sumEHEplus_%d",i);
     histosSumEHEPlus_.push_back(fs->make<TH1F>(hname,"Energy sum HE plus",100,0.,200.));
     sprintf(hname,"sumEHEminus_%d",i);
     histosSumEHEMinus_.push_back(fs->make<TH1F>(hname,"Energy sum HE minus",100,0.,200.));
     // HF
     sprintf(hname,"nHFplus_%d",i);
     histosNHFPlus_.push_back(fs->make<TH1F>(hname,"Towers mult. HF plus",nBinsHF_,0,nBinsHF_));
     sprintf(hname,"nHFminus_%d",i);
     histosNHFMinus_.push_back(fs->make<TH1F>(hname,"Towers mult. HF minus",nBinsHF_,0,nBinsHF_));	
     sprintf(hname,"sumEHFplus_%d",i);
     histosSumEHFPlus_.push_back(fs->make<TH1F>(hname,"Energy sum HF plus",100,0.,200.));   
     sprintf(hname,"sumEHFminus_%d",i);
     histosSumEHFMinus_.push_back(fs->make<TH1F>(hname,"Energy sum HF minus",100,0.,200.));
     // 
     // EB
     sprintf(hname,"nEBPlus_%d",i);
     histosNEBPlus_.push_back(fs->make<TH1F>(hname,"Towers mult. EB plus",nBinsEB_,0,nBinsEB_));
     sprintf(hname,"nEBMinus_%d",i);
     histosNEBMinus_.push_back(fs->make<TH1F>(hname,"Towers mult. EB minus",nBinsEB_,0,nBinsEB_));
     sprintf(hname,"sumEEBplus_%d",i);
     histosSumEEBPlus_.push_back(fs->make<TH1F>(hname,"Energy sum EB plus",100,0.,200.));
     sprintf(hname,"sumEEBminus_%d",i);
     histosSumEEBMinus_.push_back(fs->make<TH1F>(hname,"Energy sum EB minus",100,0.,200.));
     // EE
     sprintf(hname,"nEEplus_%d",i);
     histosNEEPlus_.push_back(fs->make<TH1F>(hname,"Towers mult. EE plus",nBinsEE_,0,nBinsEE_));
     sprintf(hname,"nEEminus_%d",i);
     histosNEEMinus_.push_back(fs->make<TH1F>(hname,"Towers mult. EE minus",nBinsEE_,0,nBinsEE_));
     sprintf(hname,"sumEEEplus_%d",i);
     histosSumEEEPlus_.push_back(fs->make<TH1F>(hname,"Energy sum EE plus",100,0.,200.));
     sprintf(hname,"sumEEEminus_%d",i);
     histosSumEEEMinus_.push_back(fs->make<TH1F>(hname,"Energy sum EE minus",100,0.,200.));
  }

  // HB
  histosTH1F_["energyHB"] = fs->make<TH1F>("energyHB","Tower Energy HB",200,0.,8.0);
  histosTH1F_["energyHBplus"] = fs->make<TH1F>("energyHBplus","Tower Energy HB-plus",200,0.,8.0);
  histosTH1F_["energyHBminus"] = fs->make<TH1F>("energyHBminus","Tower Energy HB-minus",200,0.,8.0);
  histosTH1F_["emEnergyHB"] = fs->make<TH1F>("emEnergyHB","EM Tower Energy HB",200,0.,8.0);
  histosTH1F_["hadEnergyHB"] = fs->make<TH1F>("hadEnergyHB","HAD Tower Energy HB",200,0.,8.0);
  histosTH1F_["emFractionHB"] = fs->make<TH1F>("emFractionHB","EM energy fraction HB",200,0.,1.1);
  histosTH1F_["energyHBECAL"] = fs->make<TH1F>("energyHBECAL","Tower Energy HB",200,0.,8.0);
  histosTH1F_["energyHBECALplus"] = fs->make<TH1F>("energyHBECALplus","Tower Energy HB-plus",200,0.,8.0);
  histosTH1F_["energyHBECALminus"] = fs->make<TH1F>("energyHBECALminus","Tower Energy HB-minus",200,0.,8.0);
  histosTH1F_["energyHBNOECAL"] = fs->make<TH1F>("energyHBNOECAL","Tower Energy HB",200,0.,8.0);
  histosTH1F_["energyHBNOECALplus"] = fs->make<TH1F>("energyHBNOECALplus","Tower Energy HB-plus",200,0.,8.0);
  histosTH1F_["energyHBNOECALminus"] = fs->make<TH1F>("energyHBNOECALminus","Tower Energy HB-minus",200,0.,8.0);
  // HE 
  histosTH1F_["energyHE"] = fs->make<TH1F>("energyHE","Tower Energy HE",200,0.,10.0);
  histosTH1F_["energyHEplus"] = fs->make<TH1F>("energyHEplus","Tower Energy HE-plus",200,0.,10.0);
  histosTH1F_["energyHEminus"] = fs->make<TH1F>("energyHEminus","Tower Energy HE-minus",200,0.,10.0);
  histosTH1F_["emEnergyHE"] = fs->make<TH1F>("emEnergyHE","EM Tower Energy HE",200,0.,10.0);
  histosTH1F_["hadEnergyHE"] = fs->make<TH1F>("hadEnergyHE","HAD Tower Energy HE",200,0.,10.0);
  histosTH1F_["emFractionHE"] = fs->make<TH1F>("emFractionHE","EM energy fraction HE",200,0.,1.1);
  histosTH1F_["energyHEECAL"] = fs->make<TH1F>("energyHEECAL","Tower Energy HE",200,0.,10.0);
  histosTH1F_["energyHEECALplus"] = fs->make<TH1F>("energyHEECALplus","Tower Energy HE-plus",200,0.,10.0);
  histosTH1F_["energyHEECALminus"] = fs->make<TH1F>("energyHEECALminus","Tower Energy HE-minus",200,0.,10.0);
  histosTH1F_["energyHENOECAL"] = fs->make<TH1F>("energyHENOECAL","Tower Energy HE",200,0.,10.0);
  histosTH1F_["energyHENOECALplus"] = fs->make<TH1F>("energyHENOECALplus","Tower Energy HE-plus",200,0.,10.0);
  histosTH1F_["energyHENOECALminus"] = fs->make<TH1F>("energyHENOECALminus","Tower Energy HE-minus",200,0.,10.0);
  // HF
  histosTH1F_["energyHF"] = fs->make<TH1F>("energyHF","Tower Energy HF",200,0.,15.0);
  histosTH1F_["energyHFplus"] = fs->make<TH1F>("energyHFplus","Tower Energy HF-plus",200,0.,15.0);
  histosTH1F_["energyHFminus"] = fs->make<TH1F>("energyHFminus","Tower Energy HF-minus",200,0.,15.0);
  histosTH1F_["emEnergyHF"] = fs->make<TH1F>("emEnergyHF","EM Tower Energy HF",200,0.,15.0); 
  histosTH1F_["hadEnergyHF"] = fs->make<TH1F>("hadEnergyHF","HAD Tower Energy HF",200,0.,15.0);
  histosTH1F_["emFractionHF"] = fs->make<TH1F>("emFractionHF","EM energy fraction HF",200,0.,1.1);
  // EB
  histosTH1F_["energyEB"] = fs->make<TH1F>("energyEB","Tower Energy EB",200,0.,8.0);
  histosTH1F_["energyEBplus"] = fs->make<TH1F>("energyEBplus","Tower Energy EB-plus",200,0.,8.0);
  histosTH1F_["energyEBminus"] = fs->make<TH1F>("energyEBminus","Tower Energy EB-minus",200,0.,8.0);
  histosTH1F_["emEnergyEB"] = fs->make<TH1F>("emEnergyEB","EM Tower Energy EB",200,0.,8.0);
  histosTH1F_["hadEnergyEB"] = fs->make<TH1F>("hadEnergyEB","HAD Tower Energy EB",200,0.,8.0);
  histosTH1F_["energyEBHCAL"] = fs->make<TH1F>("energyEBHCAL","Tower Energy EB",200,0.,8.0);
  histosTH1F_["energyEBHCALplus"] = fs->make<TH1F>("energyEBHCALplus","Tower Energy EB-plus",200,0.,8.0);
  histosTH1F_["energyEBHCALminus"] = fs->make<TH1F>("energyEBHCALminus","Tower Energy EB-minus",200,0.,8.0);
  histosTH1F_["energyEBNOHCAL"] = fs->make<TH1F>("energyEBNOHCAL","Tower Energy EB",200,0.,8.0);
  histosTH1F_["energyEBNOHCALplus"] = fs->make<TH1F>("energyEBNOHCALplus","Tower Energy EB-plus",200,0.,8.0);
  histosTH1F_["energyEBNOHCALminus"] = fs->make<TH1F>("energyEBNOHCALminus","Tower Energy EB-minus",200,0.,8.0);
  // EE
  histosTH1F_["energyEE"] = fs->make<TH1F>("energyEE","Tower Energy EE",200,0.,10.0);
  histosTH1F_["energyEEplus"] = fs->make<TH1F>("energyEEplus","Tower Energy EE-plus",200,0.,10.0);
  histosTH1F_["energyEEminus"] = fs->make<TH1F>("energyEEminus","Tower Energy EE-minus",200,0.,10.0);
  histosTH1F_["emEnergyEE"] = fs->make<TH1F>("emEnergyEE","EM Tower Energy EE",200,0.,10.0);
  histosTH1F_["hadEnergyEE"] = fs->make<TH1F>("hadEnergyEE","HAD Tower Energy EE",200,0.,10.0);
  histosTH1F_["energyEEHCAL"] = fs->make<TH1F>("energyEEHCAL","Tower Energy EE",200,0.,10.0);
  histosTH1F_["energyEEHCALplus"] = fs->make<TH1F>("energyEEHCALplus","Tower Energy EE-plus",200,0.,10.0);
  histosTH1F_["energyEEHCALminus"] = fs->make<TH1F>("energyEEHCALminus","Tower Energy EE-minus",200,0.,10.0);
  histosTH1F_["energyEENOHCAL"] = fs->make<TH1F>("energyEENOHCAL","Tower Energy EE",200,0.,10.0);
  histosTH1F_["energyEENOHCALplus"] = fs->make<TH1F>("energyEENOHCALplus","Tower Energy EE-plus",200,0.,10.0);
  histosTH1F_["energyEENOHCALminus"] = fs->make<TH1F>("energyEENOHCALminus","Tower Energy EE-minus",200,0.,10.0);
  
  // Misc.
  histosTH1F_["energyvseta"] = fs->make<TH1F>("energyvseta","Tower Energy vs eta",100,-5.1,5.1);
  histosTH2F_["etavsphi"] = fs->make<TH2F>("etavsphi","etavsphi",100,-5.,5.,100,-3.1416,3.1416);
  histosTH2F_["ietavsiphi"] = fs->make<TH2F>("ietavsiphi","ietavsiphi",85,-42,43,75,0,75);
	
  histosTH1F_["energyvsetaHFMax"] = fs->make<TH1F>("energyvsetaHFMax","Tower Energy vs eta",100,-5.1,5.1);
  histosTH2F_["etavsphiHFMax"] = fs->make<TH2F>("etavsphiHFMax","Tower Energy vs eta x phi",100,-5.,5.,100,-3.1416,3.1416);
  	
  histosTH1F_["hbtiming"] = fs->make<TH1F>("hbtiming","HBHE Timing",10,-0.5,9.5);
  histosTH1F_["hfhitenergy"] = fs->make<TH1F>("hfhitenergy","HF RecHit energy",100,-2.,20.);
  histosTH1F_["hflongenergy"] = fs->make<TH1F>("hflongenergy","HF RecHit Long Fiber energy",100,-2.,20.);
  histosTH1F_["hfshortenergy"] = fs->make<TH1F>("hfshortenergy","HF RecHit Short Fiber energy",100,-2.,20.);
  histosTH1F_["hfhitenergyplus"] = fs->make<TH1F>("hfhitenergyplus","HF RecHit energy",100,-2.,20.);
  histosTH1F_["hflongenergyplus"] = fs->make<TH1F>("hflongenergyplus","HF RecHit Long Fiber energy",100,-2.,20.);
  histosTH1F_["hfshortenergyplus"] = fs->make<TH1F>("hfshortenergyplus","HF RecHit Short Fiber energy",100,-2.,20.);
  histosTH1F_["hfhitenergyminus"] = fs->make<TH1F>("hfhitenergyminus","HF RecHit energy",100,-2.,20.);
  histosTH1F_["hflongenergyminus"] = fs->make<TH1F>("hflongenergyminus","HF RecHit Long Fiber energy",100,-2.,20.);
  histosTH1F_["hfshortenergyminus"] = fs->make<TH1F>("hfshortenergyminus","HF RecHit Short Fiber energy",100,-2.,20.);
  histosTH1F_["hfenergyvstime"] = fs->make<TH1F>("hfenergyvstime","HF time",100,-100.,150.);

  histosTH1F_["hflongenergyFromTwr"] = fs->make<TH1F>("hflongenergyFromTwr","HF RecHit Long Fiber energy",100,-2.,20.);
  histosTH1F_["hfshortenergyFromTwr"] = fs->make<TH1F>("hfshortenergyFromTwr","HF RecHit Short Fiber energy",100,-2.,20.);
}     

void CaloTowerNoiseAnalyzer::analyze(const edm::Event& event, const edm::EventSetup& iSetup) {
  //Calo Towers
  edm::Handle<CaloTowerCollection> towerCollection;  
  event.getByLabel(calotowersTag_,towerCollection);

  histosTH1F_["NEvents"]->Fill(0.);

  int sizeCaloTowers = towerCollection->size();

  LogTrace("Analysis") << " =================> Treating event " << event.id()
                       << " Number of Calo Towers " << sizeCaloTowers;

  if(sizeCaloTowers == 0) return;

  std::vector<int> nhb_plus(nThresholdIter_,0);
  std::vector<int> nhb_minus(nThresholdIter_,0);
  std::vector<double> sumw_hb_plus(nThresholdIter_,0.);
  std::vector<double> sumw_hb_minus(nThresholdIter_,0.);
  std::vector<double> sume_hb_plus(nThresholdIter_,0.);
  std::vector<double> sume_hb_minus(nThresholdIter_,0.);
  std::vector<double> sumew_hb_plus(nThresholdIter_,0.);
  std::vector<double> sumew_hb_minus(nThresholdIter_,0.);
  std::vector<int> nhe_plus(nThresholdIter_,0);
  std::vector<int> nhe_minus(nThresholdIter_,0);
  std::vector<double> sumw_he_plus(nThresholdIter_,0.);
  std::vector<double> sumw_he_minus(nThresholdIter_,0.);
  std::vector<double> sume_he_plus(nThresholdIter_,0.);
  std::vector<double> sume_he_minus(nThresholdIter_,0.);
  std::vector<double> sumew_he_plus(nThresholdIter_,0.);
  std::vector<double> sumew_he_minus(nThresholdIter_,0.);
  std::vector<int> nhf_plus(nThresholdIter_,0);
  std::vector<int> nhf_minus(nThresholdIter_,0);
  std::vector<double> sumw_hf_plus(nThresholdIter_,0.);
  std::vector<double> sumw_hf_minus(nThresholdIter_,0.); 
  std::vector<double> sume_hf_plus(nThresholdIter_,0.);
  std::vector<double> sume_hf_minus(nThresholdIter_,0.);
  std::vector<double> sumew_hf_plus(nThresholdIter_,0.);
  std::vector<double> sumew_hf_minus(nThresholdIter_,0.); 
  std::vector<int> neb_plus(nThresholdIter_,0);
  std::vector<int> neb_minus(nThresholdIter_,0);
  std::vector<double> sumw_eb_plus(nThresholdIter_,0.);
  std::vector<double> sumw_eb_minus(nThresholdIter_,0.);
  std::vector<double> sume_eb_plus(nThresholdIter_,0.);
  std::vector<double> sume_eb_minus(nThresholdIter_,0.);
  std::vector<double> sumew_eb_plus(nThresholdIter_,0.);
  std::vector<double> sumew_eb_minus(nThresholdIter_,0.);
  std::vector<int> nee_plus(nThresholdIter_,0);
  std::vector<int> nee_minus(nThresholdIter_,0);
  std::vector<double> sumw_ee_plus(nThresholdIter_,0.);
  std::vector<double> sumw_ee_minus(nThresholdIter_,0.);
  std::vector<double> sume_ee_plus(nThresholdIter_,0.);
  std::vector<double> sume_ee_minus(nThresholdIter_,0.);
  std::vector<double> sumew_ee_plus(nThresholdIter_,0.);
  std::vector<double> sumew_ee_minus(nThresholdIter_,0.);

  double calotwrMaxEnergyHF = 0.;
  CaloTowerCollection::const_iterator calotwrMaxHF = towerCollection->end();
  CaloTowerCollection::const_iterator calotower = towerCollection->begin(); 
  CaloTowerCollection::const_iterator calotowers_end = towerCollection->end();
  for(; calotower != calotowers_end; ++calotower){
     //const CaloTower* calotower = &(*towerCollection)[idx];
     /*std::cout << "Eta: " << calotower->eta() << std::endl
                 << "Phi: " << calotower->phi() << std::endl
                 << "Et: " << calotower->et() << std::endl
                 << "Energy: " << calotower->energy() << std::endl
	         << "EM energy: " << calotower->emEnergy() << std::endl
	         << "HAD energy: " << calotower->hadEnergy() << std::endl
	         << "iEta: " << calotower->id().ieta() << std::endl	
	         << "iPhi: " << calotower->id().iphi() << std::endl
	         << "Detector: " << calotower->id().det() << std::endl
	         << "Sub Det Id: " << calotower->id().subdetId() << std::endl
	         << "Raw Id: " << calotower->id().rawId() << std::endl
		 << "Constituents: " << std::endl;*/
     int ieta = calotower->id().ieta();
     int iphi = calotower->id().iphi();

     bool hasHCAL = false;		
     bool hasHF = false;
     bool hasHE = false;
     bool hasHO = false;
     bool hasHB = false;
     bool hasECAL = false; 
     bool hasEB = false;
     bool hasEE = false;
     for(size_t iconst = 0; iconst < calotower->constituentsSize(); iconst++){
        /*std::cout << "    " << iconst << ":  " << std::endl
		    << "    Detector: " << calotower->constituent(iconst).det() << std::endl
		    << "    Sub Det Id: " << calotower->constituent(iconst).subdetId() << std::endl
                    << "    Raw Id: " << calotower->constituent(iconst).rawId() << std::endl;*/
	DetId detId = calotower->constituent(iconst);
	if(detId.det()==DetId::Hcal){
           hasHCAL = true;
	   HcalDetId hcalDetId(detId);
	   if(hcalDetId.subdet()==HcalForward) hasHF = true;		
	   else if(hcalDetId.subdet()==HcalOuter) hasHO = true;
           else if(hcalDetId.subdet()==HcalEndcap) hasHE = true;
	   else if(hcalDetId.subdet()==HcalBarrel) hasHB = true;
           //std::cout << "HCAL " << hcalDetId.subdet() << std::endl;
        } else if(detId.det()==DetId::Ecal){
           hasECAL = true;
           EcalSubdetector ecalSubDet = (EcalSubdetector)detId.subdetId();
           if(ecalSubDet == EcalBarrel) hasEB = true;
           else if(ecalSubDet == EcalEndcap) hasEE = true; 
           //std::cout << "ECAL " << ecalSubDet << std::endl;
        }
     }
     /*std::cout << " HCAL: " << hasHCAL << std::endl
               << "  HB: " << hasHB << std::endl
               << "  HE: " << hasHE << std::endl
               << "  HF: " << hasHF << std::endl
               << "  HO: " << hasHO << std::endl     
               << " ECAL: " << hasECAL << std::endl
               << "  EB: " << hasEB << std::endl
               << "  EE: " << hasEE << std::endl;*/
				 	
     int zside = calotower->id().zside();
     double eta = calotower->eta();
     double phi = calotower->phi();

     double energy = calotower->energy();
     double emEnergy = calotower->emEnergy();
     double hadEnergy = calotower->hadEnergy();     

     double weightHB = 1.0;
     double weightHE = 1.0;
     double weightHF = 1.0;
     double weightEB = 1.0;
     double weightEE = 1.0;
     // HF
     if(hasHF&&(!hasHE)){
        if(reweightHFTower_){
           int xbin = reweightHisto_.GetXaxis()->FindBin(energy);
           int nBins = reweightHisto_.GetNbinsX(); // 1 <= xbin <= nBins
           //weight = (xbin <= nBins) ? reweightHisto_.GetBinContent(xbin) : reweightHisto_.GetBinContent(nBins);
           weightHF = (xbin <= nBins) ? reweightHisto_.GetBinContent(xbin) : 1.;
        }
     }	
     
     for(unsigned int i = 0; i < nThresholdIter_; ++i){
        bool ethreshHF = (energy >= eThresholdHF_[i]);
        bool ethreshHB = (hadEnergy >= eThresholdHB_[i]);
        bool ethreshHE = (hadEnergy >= eThresholdHE_[i]);
        bool ethreshEB = (emEnergy >= eThresholdEB_[i]);
        bool ethreshEE = (emEnergy >= eThresholdEE_[i]);
        if( ( hasHB && (!hasHE) ) && ethreshHB ){
           if(zside >= 0){
              nhb_plus[i]++; sumw_hb_plus[i] += weightHB;
              sume_hb_plus[i] += hadEnergy; sumew_hb_plus[i] += hadEnergy*weightHB;
           } else{
              nhb_minus[i]++; sumw_hb_minus[i] += weightHB;
              sume_hb_minus[i] += hadEnergy; sumew_hb_minus[i] += hadEnergy*weightHB;
           }
        } else if( ( hasHE && (!hasHB) && (!hasHF) ) && ethreshHE ){
           if(zside > 0){
              nhe_plus[i]++; sumw_he_plus[i] += weightHE;
              sume_he_plus[i] += hadEnergy; sumew_he_plus[i] += hadEnergy*weightHE;
           } else{
              nhe_minus[i]++; sumw_he_minus[i] += weightHE;
              sume_he_minus[i] += hadEnergy; sumew_he_minus[i] += hadEnergy*weightHE;
           }
        } else if( ( hasHF && (!hasHE) ) && ethreshHF ){
	   if(zside > 0){
              nhf_plus[i]++; sumw_hf_plus[i] += weightHF;
              sume_hf_plus[i] += energy; sumew_hf_plus[i] += energy*weightHF;
           } else{
              nhf_minus[i]++; sumw_hf_minus[i] += weightHF;
              sume_hf_minus[i] += energy; sumew_hf_minus[i] += energy*weightHF;  
           }
	}
        if( hasEB && ethreshEB ){
           if(zside >= 0){
              neb_plus[i]++; sumw_eb_plus[i] += weightEB;
              sume_eb_plus[i] += emEnergy; sumew_eb_plus[i] += emEnergy*weightEB;
           } else{
              neb_minus[i]++; sumw_eb_minus[i] += weightEB;
              sume_eb_minus[i] += emEnergy; sumew_eb_minus[i] += emEnergy*weightEB;
           }
        } else if( hasEE && ethreshEE ){
           if(zside >= 0){
              nee_plus[i]++; sumw_ee_plus[i] += weightEE;
              sume_ee_plus[i] += emEnergy; sumew_ee_plus[i] += emEnergy*weightEE;
           } else{
              nee_minus[i]++; sumw_ee_minus[i] += weightEE;
              sume_ee_minus[i] += emEnergy; sumew_ee_minus[i] += emEnergy*weightEE;
           }
        }
     }
 
     if( hasHB && (!hasHE) ){
        histosTH1F_["energyHB"]->Fill(energy,weightHB);
        if(zside >= 0) histosTH1F_["energyHBplus"]->Fill(energy,weightHB);
        else histosTH1F_["energyHBminus"]->Fill(energy,weightHB);
        histosTH1F_["emEnergyHB"]->Fill(emEnergy,weightHB);
        histosTH1F_["hadEnergyHB"]->Fill(hadEnergy,weightHB);
        if(hasECAL){
           histosTH1F_["energyHBECAL"]->Fill(energy,weightHB);
           if(zside >= 0) histosTH1F_["energyHBECALplus"]->Fill(energy,weightHB);
           else histosTH1F_["energyHBECALminus"]->Fill(energy,weightHB);
        } else{
           histosTH1F_["energyHBNOECAL"]->Fill(energy,weightHB);
           if(zside >= 0) histosTH1F_["energyHBNOECALplus"]->Fill(energy,weightHB);
           else histosTH1F_["energyHBNOECALminus"]->Fill(energy,weightHB);
        }
        histosTH1F_["energyvseta"]->Fill(eta,hadEnergy*weightHB);
        histosTH2F_["etavsphi"]->Fill(eta,phi,hadEnergy*weightHB);
        histosTH2F_["ietavsiphi"]->Fill(ieta,iphi,hadEnergy*weightHB);
     } else if( hasHE && (!hasHB) && (!hasHF) ){
        histosTH1F_["energyHE"]->Fill(energy,weightHE);
        if(zside > 0) histosTH1F_["energyHEplus"]->Fill(energy,weightHE);
        else histosTH1F_["energyHEminus"]->Fill(energy,weightHE);
        histosTH1F_["emEnergyHE"]->Fill(emEnergy,weightHE);
        histosTH1F_["hadEnergyHE"]->Fill(hadEnergy,weightHE);
        if(hasECAL){
           histosTH1F_["energyHEECAL"]->Fill(energy,weightHB);
           if(zside >= 0) histosTH1F_["energyHEECALplus"]->Fill(energy,weightHB);
           else histosTH1F_["energyHEECALminus"]->Fill(energy,weightHB);
        }
        histosTH1F_["energyvseta"]->Fill(eta,hadEnergy*weightHE);
        histosTH2F_["etavsphi"]->Fill(eta,phi,hadEnergy*weightHE);
        histosTH2F_["ietavsiphi"]->Fill(ieta,iphi,hadEnergy*weightHE);
     } else if( hasHF && (!hasHE) ){
        histosTH1F_["energyHF"]->Fill(energy,weightHF);
	if(zside > 0) histosTH1F_["energyHFplus"]->Fill(energy,weightHF);
	else histosTH1F_["energyHFminus"]->Fill(energy,weightHF);
        histosTH1F_["emEnergyHF"]->Fill(emEnergy,weightHF);
        histosTH1F_["hadEnergyHF"]->Fill(hadEnergy,weightHF);

        //Valid for HF emEnergy = E(L) - E(S); hadEnergy = 2*E(S)
        double energyShort = calotower->hadEnergy()/2.;
        double energyLong = calotower->emEnergy() + energyShort;

        histosTH1F_["hflongenergyFromTwr"]->Fill(energyLong,weightHF);
        histosTH1F_["hfshortenergyFromTwr"]->Fill(energyShort,weightHF);
        double eTot = emEnergy + hadEnergy;
        if(eTot > 50.0) histosTH1F_["emFractionHF"]->Fill(fabs(emEnergy/eTot),weightHF);
     
        histosTH1F_["energyvseta"]->Fill(eta,energy*weightHF);
        histosTH2F_["etavsphi"]->Fill(eta,phi,energy*weightHF);
        histosTH2F_["ietavsiphi"]->Fill(ieta,iphi,energy*weightHF);

	if(energy > calotwrMaxEnergyHF){
	   calotwrMaxEnergyHF = energy;
	   calotwrMaxHF = calotower;
	}
     }

     if( hasEB && (!hasEE) ){
        histosTH1F_["energyEB"]->Fill(energy,weightEB);
        if(zside >= 0) histosTH1F_["energyEBplus"]->Fill(energy,weightEB);
        else histosTH1F_["energyEBminus"]->Fill(energy,weightEB);
        histosTH1F_["emEnergyEB"]->Fill(emEnergy,weightEB);
        histosTH1F_["hadEnergyEB"]->Fill(hadEnergy,weightEB);
        if(hasHCAL){
           histosTH1F_["energyEBHCAL"]->Fill(energy,weightEB);
           if(zside >= 0) histosTH1F_["energyEBHCALplus"]->Fill(energy,weightEB);
           else histosTH1F_["energyEBHCALminus"]->Fill(energy,weightEB);
        } else{
           histosTH1F_["energyEBNOHCAL"]->Fill(energy,weightEB);
           if(zside >= 0) histosTH1F_["energyEBNOHCALplus"]->Fill(energy,weightEB);
           else histosTH1F_["energyEBNOHCALminus"]->Fill(energy,weightEB);
        }
        histosTH1F_["energyvseta"]->Fill(eta,emEnergy*weightEB);
        histosTH2F_["etavsphi"]->Fill(eta,phi,emEnergy*weightEB);
        histosTH2F_["ietavsiphi"]->Fill(ieta,iphi,emEnergy*weightEB);
     } else if( hasEE && (!hasEB) ){
        histosTH1F_["energyEE"]->Fill(energy,weightEE);
        if(zside >= 0) histosTH1F_["energyEEplus"]->Fill(energy,weightEE);
        else histosTH1F_["energyEEminus"]->Fill(energy,weightEE);
        histosTH1F_["emEnergyEE"]->Fill(emEnergy,weightEE);
        histosTH1F_["hadEnergyEE"]->Fill(hadEnergy,weightEE);
        if(hasHCAL){
           histosTH1F_["energyEEHCAL"]->Fill(energy,weightEE);
           if(zside >= 0) histosTH1F_["energyEEHCALplus"]->Fill(energy,weightEE);
           else histosTH1F_["energyEEHCALminus"]->Fill(energy,weightEE);
        } else{
           histosTH1F_["energyEENOHCAL"]->Fill(energy,weightEE);
           if(zside >= 0) histosTH1F_["energyEENOHCALplus"]->Fill(energy,weightEE);
           else histosTH1F_["energyEENOHCALminus"]->Fill(energy,weightEE);
        }
        histosTH1F_["energyvseta"]->Fill(eta,emEnergy*weightEE);
        histosTH2F_["etavsphi"]->Fill(eta,phi,emEnergy*weightEE);
        histosTH2F_["ietavsiphi"]->Fill(ieta,iphi,emEnergy*weightEE);
     }
  }

  if(calotwrMaxHF != calotowers_end){
     histosTH1F_["energyvsetaHFMax"]->Fill(calotwrMaxHF->eta(),calotwrMaxHF->energy());
     histosTH2F_["etavsphiHFMax"]->Fill(calotwrMaxHF->eta(),calotwrMaxHF->phi(),calotwrMaxHF->energy());
  }

  for(unsigned int i = 0; i < nThresholdIter_; ++i){	
     histosNHBPlus_[i]->Fill(sumw_hb_plus[i]);
     histosNHBMinus_[i]->Fill(sumw_hb_minus[i]);
     histosSumEHBPlus_[i]->Fill(sumew_hb_plus[i]);
     histosSumEHBMinus_[i]->Fill(sumew_hb_minus[i]);

     histosNHEPlus_[i]->Fill(sumw_he_plus[i]);
     histosNHEMinus_[i]->Fill(sumw_he_minus[i]);
     histosSumEHEPlus_[i]->Fill(sumew_he_plus[i]);
     histosSumEHEMinus_[i]->Fill(sumew_he_minus[i]);

     histosNHFPlus_[i]->Fill(sumw_hf_plus[i]);
     histosNHFMinus_[i]->Fill(sumw_hf_minus[i]);
     histosSumEHFPlus_[i]->Fill(sumew_hf_plus[i]);
     histosSumEHFMinus_[i]->Fill(sumew_hf_minus[i]);

     histosNEBPlus_[i]->Fill(sumw_eb_plus[i]);
     histosNEBMinus_[i]->Fill(sumw_eb_minus[i]);
     histosSumEEBPlus_[i]->Fill(sumew_eb_plus[i]);
     histosSumEEBMinus_[i]->Fill(sumew_eb_minus[i]);

     histosNEEPlus_[i]->Fill(sumw_ee_plus[i]);
     histosNEEMinus_[i]->Fill(sumw_ee_minus[i]);
     histosSumEEEPlus_[i]->Fill(sumew_ee_plus[i]);
     histosSumEEEMinus_[i]->Fill(sumew_ee_minus[i]);
  }

  if(accessRecHits_){
     edm::Handle<HFRecHitCollection> hfrh;
     event.getByLabel(hfRecHitsTag_,hfrh);
     const HFRecHitCollection Hithf = *(hfrh.product());

     for(HFRecHitCollection::const_iterator hhit=Hithf.begin(); hhit!=Hithf.end(); hhit++) {
        //std::cout << "rec hit energy,time= " << hhit->energy() << "  " << hhit->time() << std::endl; 
	if(hhit->energy() > 0.6) histosTH1F_["hfenergyvstime"]->Fill(hhit->time(),hhit->energy());
        HcalDetId hcalDetId(hhit->detid());
	histosTH1F_["hfhitenergy"]->Fill(hhit->energy());
	if(hcalDetId.zside() > 0) histosTH1F_["hfhitenergyplus"]->Fill(hhit->energy());
	else histosTH1F_["hfhitenergyminus"]->Fill(hhit->energy());
	if(hcalDetId.depth() == 1){
	   histosTH1F_["hflongenergy"]->Fill(hhit->energy());
	   if(hcalDetId.zside() > 0) histosTH1F_["hflongenergyplus"]->Fill(hhit->energy());
	   else histosTH1F_["hflongenergyminus"]->Fill(hhit->energy());
	} else{
	   histosTH1F_["hfshortenergy"]->Fill(hhit->energy());
	   if(hcalDetId.zside() > 0) histosTH1F_["hfshortenergyplus"]->Fill(hhit->energy());
           else histosTH1F_["hfshortenergyminus"]->Fill(hhit->energy());
	}
     }

     /*edm::Handle<HBHEDigiCollection> hbhe_digi; 
     //event.getByLabel("hcalZeroSuppressedDigis",hbhe_digi);
     event.getByLabel("hcalDigis",hbhe_digi);

     if(!hbhe_digi.failedToGet()){
        int adcs[10] = {};
     
        //CORRECT:  Timing plot should be done using linearized ADC's!
        for (HBHEDigiCollection::const_iterator j=hbhe_digi->begin(); j!=hbhe_digi->end(); j++){
           const HBHEDataFrame digi = (const HBHEDataFrame)(*j);
	   HcalDetId id = digi.id();
	   if (id.subdet() != 1) continue; 
	   int maxadc = 0;
	   for (int TS = 0; TS < 10 && TS < digi.size(); ++TS){     
	      adcs[TS] = digi[TS].adc();
	      if (digi[TS].adc() > maxadc) maxadc = digi[TS].adc();
	   }
	   std::cout << "maxadc= " << maxadc << std::endl;
	   for (int TS = 0; TS < 10 && TS < digi.size(); ++TS){     
	      if (maxadc > 10) histhbtiming_->Fill(TS,adcs[TS]);
	   }
        }
     }*/
  }
}

DEFINE_FWK_MODULE(CaloTowerNoiseAnalyzer);
