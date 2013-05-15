#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZAnalysis.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZEvent.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "TLorentzVector.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TMath.h"

#include "FWCore/Utilities/interface/RegexMatch.h"
#include "FWCore/Framework/interface/TriggerNamesService.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"

#include "DataFormats/ParticleFlowReco/interface/PFBlockElementTrack.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "DataFormats/HcalDetId/interface/HcalZDCDetId.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"
#include "DataFormats/HcalDetId/interface/HcalElectronicsId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"

#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"

using diffractiveZAnalysis::DiffractiveZAnalysis;

const char* DiffractiveZAnalysis::name = "DiffractiveZAnalysis";

DiffractiveZAnalysis::DiffractiveZAnalysis(const edm::ParameterSet& pset):
  triggerResultsTag_(pset.getParameter<edm::InputTag>("TriggerResultsTag")),
  hltPathNames_(pset.getParameter<std::vector<std::string> >("hltPaths")),
  electronTag_(pset.getParameter<edm::InputTag>("electronTag")),
  muonTag_(pset.getParameter<edm::InputTag>("muonTag")),
  PVtxCollectionTag_(pset.getParameter<edm::InputTag>("PVtxCollectionTag")),
  RunMC_(pset.getUntrackedParameter<Bool_t>("RunMC", false)),
  RunZPat_(pset.getUntrackedParameter<Bool_t>("RunZPat", false)),
  pTPFThresholdCharged_(pset.getParameter<double>("pTPFThresholdCharged")),
  energyPFThresholdBar_(pset.getParameter<double>("energyPFThresholdBar")),
  energyPFThresholdEnd_(pset.getParameter<double>("energyPFThresholdEnd")),
  energyPFThresholdHF_(pset.getParameter<double>("energyPFThresholdHF")),
  energyThresholdHB_(pset.getParameter<double>("energyThresholdHB")),
  energyThresholdHE_(pset.getParameter<double>("energyThresholdHE")),
  energyThresholdHF_(pset.getParameter<double>("energyThresholdHF")),
  energyThresholdEB_(pset.getParameter<double>("energyThresholdEB")),
  energyThresholdEE_(pset.getParameter<double>("energyThresholdEE")),
  caloTowerTag_(pset.getParameter<edm::InputTag>("CaloTowerTag"))
{
}

void DiffractiveZAnalysis::setTFileService(){
  edm::Service<TFileService> fs;
  std::ostringstream oss;
  hltTriggerNamesHisto_ = fs->make<TH1F>("HLTTriggerNames","HLTTriggerNames",1,0,1);
  hltTriggerNamesHisto_->SetBit(TH1::kCanRebin);
  for(unsigned k=0; k < hltPathNames_.size(); ++k){
    oss << "Using HLT reference trigger " << hltPathNames_[k] << std::endl;
    hltTriggerNamesHisto_->Fill(hltPathNames_[k].c_str(),1);
  }
  edm::LogVerbatim("Analysis") << oss.str();

  hltTriggerPassHisto_ = fs->make<TH1F>("HLTTriggerPass","HLTTriggerPass",1,0,1);
  hltTriggerPassHisto_->SetBit(TH1::kCanRebin);
}

DiffractiveZAnalysis::~DiffractiveZAnalysis(){}

void DiffractiveZAnalysis::begin() {
  setTFileService();
}

void DiffractiveZAnalysis::begin(const edm::Run& run, const edm::EventSetup& setup) {}

void DiffractiveZAnalysis::end() {}

void DiffractiveZAnalysis::fill(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  eventData.reset();

  fillTriggerInfo(eventData,event,setup);
  fillMuonsInfo(eventData,event,setup);
  fillElectronsInfo(eventData,event,setup);
  fillTracksInfo(eventData,event,setup); 
  if (RunMC_) fillGenInfo(eventData,event,setup); 
  if (RunZPat_) fillZPat(eventData,event,setup);

}

// Fill Trigger
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillTriggerInfo(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  edm::Handle<edm::TriggerResults> triggerResults;
  event.getByLabel(triggerResultsTag_, triggerResults);

  if( triggerResults.isValid() ){
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
      hltTriggerPassHisto_->Fill( (*hltpath).c_str(), 1 ); 
    }

  }else{
    std::cout << "\n No valid trigger result." <<std::endl;
  }

}

// Fill Reco::Electron
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillElectronsInfo(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  int ElectronN=0;
  bool debug = false;

  edm::Handle<reco::GsfElectronCollection> electrons;
  event.getByLabel(electronTag_,electrons);

  const reco::GsfElectron* electron1=NULL;
  const reco::GsfElectron* electron2=NULL;

  int electronsize = electrons->size();
  int itElectron;

  if(electrons->size()>1){

    for(itElectron=0; itElectron < electronsize; ++itElectron){

      ++ElectronN;

      const reco::GsfElectron* electronAll = &((*electrons)[itElectron]);

      if (electronAll==NULL) continue;
      if (electron1==NULL) {electron1=electronAll; continue;}
      if (electronAll->pt()>electron1->pt()) {
	electron2=electron1;
	electron1=electronAll;
	continue;
      }

      if (electron2==NULL) {electron2=electronAll; continue;}
      if (electronAll->pt()>electron2->pt()) electron2 = electronAll;

    }

    // Dielectron Mass
    math::XYZTLorentzVector DielectronSystem(0.,0.,0.,0.);
    DielectronSystem += electron1->p4();
    DielectronSystem += electron2->p4();

    eventData.SetDiElectronMass(DielectronSystem.M());
    eventData.SetElectronsN(ElectronN);
    eventData.SetLeadingElectronPt(electron1->pt());
    eventData.SetLeadingElectronEta(electron1->eta());
    eventData.SetLeadingElectronPhi(electron1->phi());
    eventData.SetLeadingElectronCharge(electron1->charge());
    eventData.SetLeadingElectronP4(electron1->p4());
    eventData.SetSecondElectronPt(electron2->pt());
    eventData.SetSecondElectronEta(electron2->eta());
    eventData.SetSecondElectronPhi(electron2->phi());
    eventData.SetSecondElectronCharge(electron2->charge());
    eventData.SetSecondElectronP4(electron2->p4());

    if (debug){
      std::cout << "NElectron: " << ElectronN << std::endl;
      std::cout << "NSize: " << electrons->size() << std::endl;
      std::cout << "Muon1: " << electron1->pt() << std::endl;
      std::cout << "Muon2: " << electron2->pt() << std::endl;
    }

  }

}

// Fill Reco::Muon
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillMuonsInfo(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  bool debug = false;
  int MuonsN=0;

  edm::Handle<reco::MuonCollection> muons;
  event.getByLabel(muonTag_,muons);

  const reco::Muon* muon1=NULL;
  const reco::Muon* muon2=NULL;

  int muonsize = muons->size();
  int itMuon;

  if(muons->size()>1){

    for(itMuon=0; itMuon < muonsize; ++itMuon){

      ++MuonsN;

      const reco::Muon* muonAll = &((*muons)[itMuon]);

      if (muonAll==NULL) continue;

      if (muon1==NULL) {muon1=muonAll; continue;}
      if (muonAll->pt()>muon1->pt()) {
	muon2=muon1;
	muon1=muonAll;
	continue;
      }

      if (muon2==NULL) {muon2=muonAll; continue;}
      if (muonAll->pt()>muon2->pt()) muon2 = muonAll;


      if (debug){
	std::cout << "\n----------------------------------------------------" << std::endl;
	std::cout << "\t energy (ecal, hcal, ho): " << muon1->calEnergy().em << ", " << muon1->calEnergy().had << ", " << muon1->calEnergy().ho << std::endl;
	std::cout << "\t isolation dR=0.3 (sumPt, emEt, hadEt, hoEt, nTracks, nJets): " << muon1->isolationR03().sumPt << ", " << muon1->isolationR03().emEt << ", " << muon1->isolationR03().hadEt << ", " << muon1->isolationR03().hoEt << ", " << muon1->isolationR03().nTracks << ", " << muon1->isolationR03().nJets << std::endl;
	std::cout << "\t isolation dR=0.5 (sumPt, emEt, hadEt, hoEt, nTracks, nJets): " << muon1->isolationR05().sumPt << ", " << muon1->isolationR05().emEt << ", " << muon1->isolationR05().hadEt << ", " << muon1->isolationR05().hoEt << ", " << muon1->isolationR05().nTracks << ", " << muon1->isolationR05().nJets << std::endl;
	std::cout << "\t # matches: " << muon1->numberOfMatches() << std::endl;
	std::cout << "\t # caloCompatibility: " << muon1->caloCompatibility() << std::endl;  
      }

    }

    // Dimuon Mass
    math::XYZTLorentzVector DimuonSystem(0.,0.,0.,0.);
    DimuonSystem += muon1->p4();
    DimuonSystem += muon2->p4();

    eventData.SetDiMuonMass(DimuonSystem.M());
    eventData.SetMuonsN(MuonsN);
    eventData.SetLeadingMuonPt(muon1->pt());
    eventData.SetLeadingMuonEta(muon1->eta());
    eventData.SetLeadingMuonPhi(muon1->phi());
    eventData.SetLeadingMuonCharge(muon1->charge());
    eventData.SetLeadingMuonP4(muon1->p4());
    eventData.SetSecondMuonPt(muon2->pt());
    eventData.SetSecondMuonEta(muon2->eta());
    eventData.SetSecondMuonPhi(muon2->phi());
    eventData.SetSecondMuonCharge(muon2->charge());
    eventData.SetSecondMuonP4(muon2->p4());

    if (debug){
      std::cout << "NMuons: " << MuonsN << std::endl;
      std::cout << "NSize: " << muons->size() << std::endl;
      std::cout << "Muon1: " << muon1->pt() << std::endl;
      std::cout << "Muon2: " << muon2->pt() << std::endl;
    }

  }

}

// Fill Tracks Info
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillTracksInfo(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  std::vector<double> vertexNDOF;
  std::vector<double> vertexChiNorm;
  std::vector<double> vertexMultiplicity;
  double nhit=0;
  std::vector<double> V_x;
  std::vector<double> V_y;
  std::vector<double> V_z; 
  std::vector<double> pt;
  std::vector<double> tracks;
  std::vector<std::vector<double> > tracksPT;

  edm::Handle<reco::VertexCollection>  vertexCollectionHandle;
  event.getByLabel(PVtxCollectionTag_, vertexCollectionHandle);

  for(reco::VertexCollection::const_iterator vtx = vertexCollectionHandle->begin();vtx!=vertexCollectionHandle->end(); ++vtx)
  {
    reco::Vertex::trackRef_iterator it = vtx->tracks_begin();
    reco::Vertex::trackRef_iterator lastTrack = vtx->tracks_end();


    for(;it!=lastTrack;it++) {
      nhit+=(*it)->numberOfValidHits();
      pt.push_back((*it)->pt());
    }

    //Sorting the pt tracks, in order to take only the 31 most energetics
    const int  size = (int) pt.size();
    int *sorted = new int[size];
    double *v = new double[size];

    for (int i=0; i<size; i++) {
      v[i] = pt[i];
    }
    TMath::Sort(size, v, sorted, true);
    for (int i=0; i<size; i++) {
      tracks.push_back(pt[sorted[i]]);
      if (i>30) break;
    }

    tracksPT.push_back(tracks);
    tracks.clear();
    pt.clear();

    double ndof=vtx->ndof();
    double chiNorm=vtx->normalizedChi2();
    double NumbOfTracks=vtx->tracksSize();
    vertexNDOF.push_back(ndof);
    vertexChiNorm.push_back(chiNorm);
    vertexMultiplicity.push_back(NumbOfTracks);
    nhit=0;
    if ( ndof != 0 ) {
      V_x.push_back(vtx->x());
      V_y.push_back(vtx->y());
      V_z.push_back(vtx->z());
    } else {
      V_x.push_back(-999);
      V_y.push_back(-999);
      V_z.push_back(-999);
    }

  } // loop over vtx

  eventData.SetVertexMultiplicity(vertexMultiplicity);
  eventData.SetVertexChiNorm(vertexChiNorm);
  eventData.SetVertexNDOF(vertexNDOF);
  eventData.SetVz(V_z);
  eventData.SetVx(V_x);
  eventData.SetVy(V_y); 
  eventData.SetTracksPt(tracksPT);

}

// Fill Gen Level Information
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillGenInfo(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  bool debug=false;

  //Variable declaration
  int count=0;
  int Nstable_gen=0;
  math::XYZTLorentzVector part(0.,0.,0.,0.);
  math::XYZTLorentzVector partVis(0.,0.,0.,0.);
  math::XYZTLorentzVector partZ(0.,0.,0.,0.);
  double sumECastor_minus_gen=0;
  double sumECastor_plus_gen=0;
  double sumEZDC_minus_gen=0;
  double sumEZDC_plus_gen=0;
  double etaOutcomingProton=0;
  double energyOutcomingProton=0;
  double mostEnergeticXL=0;
  double mostEnergeticXLNum=0;
  std::vector<double> eta_gen_vec;
  double xi_Z_gen_minus=0;
  double xi_Z_gen_plus=0;
  double etaZ_gen=0;
  double energyZ_gen=0;
  double p_diss_mass=0;
  double p_diss=0;
  double xL_p_diss=0;

  double xL_etaGTP5=0;
  double xL_etaLTM5=0;
  int xL_LTM5Num=0;
  int xL_GTP5Num=0;

  std::vector<double> genpt;
  std::vector<double> tracks;
  std::vector<double> eta;
  std::vector<double> etaPT;
  std::vector<double> tracksPT;


  edm::Handle<reco::GenParticleCollection> genParticles;     
  event.getByLabel("genParticles",genParticles);  // standard PYTHIA collection

  for(size_t i = 0; i < genParticles->size(); ++ i) {

    const reco::GenParticle & p = (*genParticles)[i];

    int pdg = p.pdgId();
    int status = p.status();  	 
    double eta_gen = p.eta();
    double part_pt = p.pt();
    double ener_gen= p.energy();
    double px_gen  = p.px();
    double py_gen  = p.py();
    double pz_gen  = p.pz();
    double mass_gen= p.mass();
    bool electronFromZ=false;
    int motherId=0;

    if (  p.mother() ) motherId =  p.mother()->pdgId();

    math::XYZTLorentzVector tmp( px_gen ,py_gen , pz_gen ,ener_gen );

    if (fabs(pdg)==11 || fabs(pdg)==22){ 	    
      if (motherId==23) {
	electronFromZ=true;
	partZ+=tmp;
      }
    }

    if (count==2) {    /// only works for MC Pompyt dissociative
      p_diss_mass= mass_gen;
      p_diss= pz_gen;
      if ( pdg == 2212){
	etaOutcomingProton= eta_gen;
	energyOutcomingProton= ener_gen;
      }
    }

    if (( pdg == 23)){
      xi_Z_gen_minus=( ener_gen - pz_gen )/7000;
      xi_Z_gen_plus=( ener_gen + pz_gen )/7000;
      etaZ_gen=eta_gen;
      energyZ_gen= ener_gen;
    }

    if (status == 1) 
    {
      //// vector to find gaps (cut at 1 GeV in energy)
      if  ( ( fabs(eta_gen) <= 1.5  && ener_gen > energyPFThresholdBar_ )  ||
	  (fabs(eta_gen) > 1.5 && fabs(eta_gen) <= 3 && ener_gen > energyPFThresholdEnd_) ||
	  (fabs(eta_gen) > 3 && ener_gen >energyPFThresholdHF_)  ) {

	eta_gen_vec.push_back( eta_gen);
      }

      if (  count>2) {   
	part+=tmp;
      }

      if (  (fabs(eta_gen) < 4.7) && (part_pt > 0.10) ) {   // if particle has a chance to reach the detector ...
	partVis+=tmp;
      }

      // new xL_gen definition (after Sasha)
      if (count>=2 )
      {
	if (eta_gen > 4.7)  
	{
	  xL_etaGTP5 += pz_gen;
	  xL_GTP5Num++;
	}
	if (eta_gen < -4.7)  
	{
	  xL_etaLTM5 += pz_gen;
	  xL_LTM5Num++;
	}
      }

      if (count>=2 ){
	if (p_diss>0) {
	  if ( xL_p_diss < pz_gen ){
	    xL_p_diss= pz_gen;
	  }
	}
	if (p_diss<0) {
	  if ( xL_p_diss > pz_gen ){
	    xL_p_diss= pz_gen;
	  }
	}
      }

      if ( fabs(eta_gen)>5.2 && fabs(eta_gen)<6.6 ){
	if (debug) std::cout<<"Particle in Castor, having eta "<<eta_gen<<" and energy "<< ener_gen<<std::endl;
	if (eta_gen<0) sumECastor_minus_gen += ener_gen;
	if (eta_gen>0) sumECastor_plus_gen += ener_gen;
      }

      if ( fabs(eta_gen)>8.2  && ( pdg == 2112 || pdg == 22) ){
	if (debug) std::cout<<"Particle in ZDC, having eta "<<eta_gen<<" and energy "<< ener_gen<<std::endl;
	if (eta_gen<0) sumEZDC_minus_gen += ener_gen;
	if (eta_gen>0) sumEZDC_plus_gen += ener_gen;
      }      
      Nstable_gen++;

    }  // status =1
    count++;

  } // loop over particles

  //// Computing GAPs
  const int  size = (int) eta_gen_vec.size();
  int *sortedgen=   new int[size];
  double *vgen = new double[size];
  double eta_gap_limplus_gen = -10.0;
  double eta_gap_limminus_gen = -10.0;

  for (int i=0; i<size; i++) {
    vgen[i] = eta_gen_vec[i];
    if (debug) std::cout<<vgen[i]<<std::endl;
  }
  TMath::Sort(size, vgen, sortedgen, true);

  if (size > 1) {
    double *diff = new double[size-1];
    int *diffsorted = new int[size-1];
    for (int i=0; i<(size-1); i++) {
      diff[i] = fabs(eta_gen_vec[sortedgen[i+1]]-eta_gen_vec[sortedgen[i]]);
      if (debug) {
	std::cout<<" eta " << i << " size " << size << " diff "<< diff[i]<< std::endl;
	std::cout<<"GEN  etas "  << " = " << eta_gen_vec[sortedgen[i+1]] << " - " <<  eta_gen_vec[sortedgen[i]] <<  " GAP diff "<< diff[i] << std::endl;
	std::cout<<" GEN etas "  << " = " << eta_gen_vec[sortedgen[i]] << std::endl;
      }
    }

    TMath::Sort(size-1, diff, diffsorted, true);

    //checking the max gap
    double max_eta_gap_gen=diff[diffsorted[0]];
    eta_gap_limminus_gen = eta_gen_vec[sortedgen[diffsorted[0]+1]] ;
    eta_gap_limplus_gen = eta_gen_vec[sortedgen[diffsorted[0]]] ;

    if (debug) std::cout << "GEN eta ranges " <<  eta_gap_limplus_gen  << " " <<  eta_gap_limminus_gen  << std::endl;
    eventData.SetPrimaryGapMaxGen(max_eta_gap_gen);

    if (size>2) {
      double max_second_eta_gap_gen=diff[diffsorted[1]];
      eventData.SetSecondGapMaxGen(max_second_eta_gap_gen);
      if (debug) std::cout<<" diff  " << diff[diffsorted[0]] << " sec " << diff[diffsorted[1]] << " diff size "<< diff[size-2] << std::endl;
    }

    delete [] diff;
    delete [] diffsorted;
  }

  delete [] sortedgen;
  delete [] vgen;

  math::XYZTLorentzVector dataMassG_plus(0.,0.,0.,0.);
  math::XYZTLorentzVector dataMassG_minus(0.,0.,0.,0.);
  int nplusG =0;
  int nminusG =0;
  int numseltracks =0;

  for(size_t i = 0; i < genParticles->size(); ++ i) {
    const reco::GenParticle & p = (*genParticles)[i];
    int status = p.status();  	 
    double eta_gen = p.eta();
    int charge = p.charge();
    double pt = p.pt();

    if( status == 1 && p.energy() > 1 ) {

      math::XYZTLorentzVector tmp( p.px(),p.py(),p.pz(),p.energy());

      if ( eta_gen >= eta_gap_limplus_gen ) {
	dataMassG_plus+=tmp;
	nplusG++;
      }
      else {
	dataMassG_minus+=tmp;
	nminusG++;
      }
    }

    if ( status == 1 ) {
      if ( charge && fabs(eta_gen)<2.6 &&  pt >= 0.1 ) {  // !!  condition for xsec per 3 charged prompt particles
	numseltracks++;
	genpt.push_back(pt);
	eta.push_back(eta_gen);
      }
    }
  } // end of genparticle loop

  float Mx2_gen=partVis.M2(); /// massaquadro visibile generata
  math::XYZTLorentzVector NOZ=partVis-partZ;
  float Mx2_NOZ_gen=NOZ.M2();
  if (debug) {
    std::cout << "Mx2_gen is "<< Mx2_gen<<" while eta of the outcoming proton is "<< etaOutcomingProton <<" and the energy "<< energyOutcomingProton << std::endl;
  }

  mostEnergeticXL = xL_etaGTP5/3500.;
  mostEnergeticXLNum = xL_GTP5Num ;
  if (fabs(xL_etaGTP5)<fabs(xL_etaLTM5)) 
  {
    mostEnergeticXL = xL_etaLTM5/3500.;
    mostEnergeticXLNum = xL_LTM5Num ;
  }

  if (debug) std::cout << "* XLgen " << mostEnergeticXL << " num " << mostEnergeticXLNum << " + " << xL_etaGTP5 << " - " << xL_etaLTM5 <<  std::endl;

  const int  size2 = (int) genpt.size();
  int *sorted = new int[size2];
  double *vv = new double[size2];
  for (int i=0; i<size2; i++) {
    vv[i] = genpt[i];
  }
  TMath::Sort(size2, vv, sorted, true);
  for (int i=0; i<size2; i++) {
    tracks.push_back(genpt[sorted[i]]);
    etaPT.push_back(eta[sorted[i]]);
    if (i>30) break;
  }  //  comes out size of 32!

  eventData.SetTracksPtGen(tracks);
  eventData.SetEtaOfTracksPtGen(etaPT);
  eventData.SetNTracksGen(tracks.size());

  genpt.clear();
  eta.clear();

  delete [] sorted;
  delete [] vv;

  eventData.SetMx2PlusGen(dataMassG_plus.M2());
  eventData.SetMx2MinusGen(dataMassG_minus.M2());
  eventData.SetMx2Gen(Mx2_NOZ_gen);
  eventData.SetMx2ZGen(Mx2_gen);
  eventData.SetNMx2PlusGen(nplusG);
  eventData.SetNMx2MinusGen(nminusG);
  eventData.SetEtaGaplimPlusGen(eta_gap_limplus_gen);
  eventData.SetEtaGaplimMinusGen(eta_gap_limminus_gen);
  eventData.SetNParticlesGen(Nstable_gen);
  eventData.SetsumECastorMinusGen(sumECastor_minus_gen);
  eventData.SetsumECastorPlusGen(sumECastor_plus_gen);
  eventData.SetsumEZDCMinusGen(sumEZDC_minus_gen);
  eventData.SetsumEZDCPlusGen(sumEZDC_plus_gen);
  eventData.SetEtaOutcomingProtonGen(etaOutcomingProton);
  eventData.SetxLGen(mostEnergeticXL);
  eventData.SetxLMostEnergeticGen(mostEnergeticXLNum);
  eventData.SetxiZMinusGen(xi_Z_gen_minus);
  eventData.SetxiZPlusGen(xi_Z_gen_plus);
  eventData.SetEtaZGen(etaZ_gen);
  eventData.SetEnergyZGen(energyZ_gen);
  eventData.SetpDissMassGen(p_diss_mass);
  eventData.SetxLpDissMass(xL_p_diss);

}

//
// Fill Detector Variables
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillDetectorVariables(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  bool debug = false;

  double etaMax=-999;
  double etaMin=999;
  double Epz_plus=0;  
  double Epz_minus=0;  

  int nTowersHF_plus = 0;
  int nTowersHF_minus = 0;
  int nTowersHE_plus = 0;
  int nTowersHE_minus = 0;
  int nTowersHB_plus = 0;
  int nTowersHB_minus = 0;
  int nTowersEE_plus = 0;
  int nTowersEE_minus = 0;
  int nTowersEB_plus = 0;
  int nTowersEB_minus = 0;

  //Sum(E)
  double sumEHF_plus = 0.;
  double sumEHF_minus = 0.;
  double sumEHF_L_plus = 0.;
  double sumEHF_L_minus = 0.;
  double sumEHF_S_plus = 0.;
  double sumEHF_S_minus = 0.;
  double sumEHE_plus = 0.;
  double sumEHE_minus = 0.;
  double sumEHB_plus = 0.;
  double sumEHB_minus = 0.;
  double sumEEE_plus = 0.;
  double sumEEE_minus = 0.;
  double sumEEB_plus = 0.;
  double sumEEB_minus = 0.;

  // Sum(ET)
  double sumETHF_plus = 0.;
  double sumETHF_minus = 0.;
  double sumETHE_plus = 0.;
  double sumETHE_minus = 0.;
  double sumETHB_plus = 0.;
  double sumETHB_minus = 0.;
  double sumETEB_plus = 0.;
  double sumETEB_minus = 0.;
  double sumETEE_plus = 0.;
  double sumETEE_minus = 0.;
  double xi_Calo_minus =0;
  double xi_Calo_plus =0;

  edm::Handle<CaloTowerCollection> towerCollectionH;
  event.getByLabel(caloTowerTag_,towerCollectionH);
  const CaloTowerCollection& towerCollection = *towerCollectionH;

  CaloTowerCollection::const_iterator calotower;
  calotower = towerCollection.begin();
  CaloTowerCollection::const_iterator calotowers_end = towerCollection.end();

  for(; calotower != calotowers_end; ++calotower) {

    if (fabs(calotower->eta())> 4.7) continue;   /// excluding ring12 and ring13 of HF

    bool hasHCAL = false;
    bool hasHF = false;
    bool hasHE = false;
    bool hasHB = false;
    bool hasHO = false;
    bool hasECAL = false;
    bool hasEE = false;
    bool hasEB = false;     

    for(size_t iconst = 0; iconst < calotower->constituentsSize(); iconst++){
      DetId adetId = calotower->constituent(iconst);
      if(adetId.det()==DetId::Hcal){
	hasHCAL = true;
	HcalDetId hcalDetId(adetId);
	if(hcalDetId.subdet()==HcalForward) hasHF = true;
	else if(hcalDetId.subdet()==HcalEndcap) hasHE = true;
	else if(hcalDetId.subdet()==HcalBarrel) hasHB = true;
	else if(hcalDetId.subdet()==HcalOuter) hasHO = true;  
      } 
      else if(adetId.det()==DetId::Ecal){
	hasECAL = true;
	EcalSubdetector ecalSubDet = (EcalSubdetector)adetId.subdetId();
	if(ecalSubDet == EcalEndcap) hasEE = true;
	else if(ecalSubDet == EcalBarrel) hasEB = true;
      }
    }
    int zside = calotower->zside();
    double caloTowerEnergy = calotower->energy();
    double caloTowerEmEnergy = calotower->emEnergy();
    double caloTowerHadEnergy = calotower->hadEnergy();
    double caloTowerPz = calotower->pz();
    double caloTowerEt = calotower->et();
    double caloTowerEmEt = calotower->emEt();
    double caloTowerHadEt = calotower->hadEt();
    double EHF_S = 0;
    double EHF_L = 0;

    bool CalAboveTh = false;

    if( hasHF && !hasHE )
    {
      if( caloTowerEnergy > energyThresholdHF_ && fabs(calotower->eta())> 2.98 )   //// excluding HF ring1
      {
	CalAboveTh = true;

	if (debug) std::cout << "HF>> " <<  calotower->id() << " HAD energy "     << caloTowerHadEnergy << " EM energy " << caloTowerEmEnergy << " energy " << caloTowerEnergy << std::endl; 

	// Added Long and short fibers
	// emc=L-S
	// hac=2*S
	// Tot = L+S
	// S = hac/2
	// L = Tot - S

	EHF_S = caloTowerHadEnergy/2;
	EHF_L = caloTowerEnergy - caloTowerHadEnergy/2;

	if(zside >= 0)
	{
	  ++nTowersHF_plus;
	  sumEHF_plus += caloTowerEnergy;
	  sumEHF_S_plus += EHF_S;
	  sumEHF_L_plus += EHF_L;
	  sumETHF_plus += caloTowerEt;
	}
	else
	{
	  ++nTowersHF_minus;
	  sumEHF_minus += caloTowerEnergy;
	  sumEHF_S_minus += EHF_S;
	  sumEHF_L_minus += EHF_L;
	  sumETHF_minus += caloTowerEt;
	}

	//If you wish, fill (calotower->eta(),EHF_S) here.
	//If you wish, fill (calotower->eta(),EHF_L) here.

      }
    }
    else if( hasHE && !hasHF && !hasHB )
    {
      if( caloTowerHadEnergy > energyThresholdHE_)
      {
	CalAboveTh = true;

	if (debug) std::cout << "HE>> " <<  calotower->id() << "  HAD energy "     << caloTowerHadEnergy << " EM energy " << caloTowerEmEnergy << " energy " << caloTowerEnergy << std::endl;

	if(zside >= 0)
	{
	  ++nTowersHE_plus;
	  sumEHE_plus += caloTowerHadEnergy;
	  sumETHE_plus += caloTowerHadEt;
	}
	else
	{
	  ++nTowersHE_minus;
	  sumEHE_minus += caloTowerHadEnergy;
	  sumETHE_minus += caloTowerHadEt;
	}
      }
    }
    else if( hasHB && !hasHE )
    {
      if( caloTowerHadEnergy > energyThresholdHB_)
      {
	CalAboveTh = true;

	if (debug) std::cout << "HB>> " <<  calotower->id() << "  HAD energy "     << caloTowerHadEnergy << " EM energy " << caloTowerEmEnergy << " energy " << caloTowerEnergy << std::endl;

	if(zside >= 0)
	{
	  ++nTowersHB_plus;
	  sumEHB_plus += caloTowerHadEnergy;
	  sumETHB_plus += caloTowerHadEt;
	}
	else
	{
	  ++nTowersHB_minus;
	  sumEHB_minus += caloTowerHadEnergy;
	  sumETHB_minus += caloTowerHadEt;
	}
      }
    }

    if( hasEE && !hasEB )
    {
      if( caloTowerEmEnergy >= energyThresholdEE_)
      {
	CalAboveTh = true;

	if (debug) std::cout << "EE>> " <<  calotower->id() << "  HAD energy "     << caloTowerHadEnergy << " EM energy " << caloTowerEmEnergy << " energy " << caloTowerEnergy << std::endl;

	if(zside >= 0)
	{
	  ++nTowersEE_plus;
	  sumEEE_plus += caloTowerEmEnergy;
	  sumETEE_plus += caloTowerEmEt;
	}
	else
	{
	  ++nTowersEE_minus;
	  sumEEE_minus += caloTowerEmEnergy;
	  sumETEE_minus += caloTowerEmEt;
	}
      }
    }
    else if( hasEB && !hasEE )
    {
      if( caloTowerEmEnergy >= energyThresholdEB_)
      {
	CalAboveTh = true;

	if (debug) std::cout << "EB>> " <<  calotower->id() << " HAD energy "     << caloTowerHadEnergy << " EM energy " << caloTowerEmEnergy << " energy " << caloTowerEnergy << std::endl; 

	if(zside >= 0)
	{
	  ++nTowersEB_plus;
	  sumEEB_plus += caloTowerEmEnergy;
	  sumETEB_plus += caloTowerEmEt;
	}
	else
	{
	  ++nTowersEB_minus;
	  sumEEB_minus += caloTowerEmEnergy;
	  sumETEB_minus += caloTowerEmEt;
	}
      }
    }

    if(CalAboveTh)
    {
      if (calotower->eta() >= etaMax) etaMax=calotower->eta();
      if (calotower->eta() <= etaMin) etaMin=calotower->eta();
      xi_Calo_minus += caloTowerEt * pow(2.71,-calotower->eta()) / (7000);
      xi_Calo_plus += caloTowerEt * pow(2.71,calotower->eta()) / (7000);
      Epz_plus  += caloTowerEnergy + caloTowerPz;
      Epz_minus += caloTowerEnergy - caloTowerPz;
    }

  }  ////has to close calotower loop

  eventData.SetSumEHFPlus(sumEHF_plus);
  eventData.SetSumEHF_SPlus(sumEHF_S_plus);
  eventData.SetSumEHF_LPlus(sumEHF_L_plus);
  eventData.SetSumEtHFPlus(sumETHF_plus);

  eventData.SetSumEHFMinus(sumEHF_minus);
  eventData.SetSumEHF_SMinus(sumEHF_S_minus);
  eventData.SetSumEHF_LMinus(sumEHF_L_minus);
  eventData.SetSumEtHFMinus(sumETHF_minus);

  eventData.SetSumEHEPlus(sumEHE_plus);
  eventData.SetSumEtHEPlus(sumETHE_plus);
  eventData.SetSumEHEMinus(sumEHE_minus);
  eventData.SetSumEtHEMinus(sumETHE_minus);

  eventData.SetSumEHBPlus(sumEHB_plus);
  eventData.SetSumEtHBPlus(sumETHB_plus);
  eventData.SetSumEHBMinus(sumEHB_minus);
  eventData.SetSumEtHBMinus(sumETHB_minus);

  eventData.SetSumEEEPlus(sumEEE_plus);
  eventData.SetSumEtEEPlus(sumETEE_plus);
  eventData.SetSumEEEMinus(sumEEE_minus);
  eventData.SetSumEtEEMinus(sumETEE_minus);

  eventData.SetSumEEBPlus(sumEEB_plus);
  eventData.SetSumEtEBPlus(sumETEB_plus);
  eventData.SetSumEEBMinus(sumEEB_minus);
  eventData.SetSumEtEBMinus(sumETEB_minus);

  eventData.SetEPZCaloPlus(Epz_plus);
  eventData.SetEPZCaloMinus(Epz_minus);
  eventData.SetXiCaloPlus(xi_Calo_plus);
  eventData.SetXiCaloMinus(xi_Calo_minus);

  eventData.SetEtaCaloMax(etaMax);
  eventData.SetEtaCaloMin(etaMin);

  // Do not forget to Fill Calo Multiplicities!!! 
  

}

//
// Fill Physics Variables
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillVariables(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  bool debug=false;

  std::vector<double> etas;
  double etaTimesEnergy=0.;
  float Epz_PF_plus=0.;
  float Epz_PF_minus=0.;
  double xi_PF_minus=0.;
  double xi_PF_plus=0.;
  double sumpx=0.;
  double sumpy=0.;
  double sumpz=0.;
  double sumpxModule=0.;
  double sumpyModule=0.;
  double sumpzModule=0.;
  double sumEnergyPF=0.;

  double PtThPFCharged = 0.1;  // it was 0.15
  double EnThPFBar = 1.5;
  double EnThPFEnd = 3.5; // 4.;
  double EnThPFFw  = 6.0; // 6; 

  int nPart_PF=0;

  std::vector<double> electronEnergy;
  std::vector<double> muEnergy;

  edm::Handle<reco::VertexCollection> Vertexes;
  event.getByLabel(PVtxCollectionTag_, Vertexes); 

  edm::Handle <reco::PFCandidateCollection> PFCandidates;
  event.getByLabel("particleFlow",PFCandidates);
  reco::PFCandidateCollection::const_iterator iter;

  eventData.SetVertex(Vertexes->size());

  for (reco::PFCandidateCollection::const_iterator iter = PFCandidates->begin(); iter != PFCandidates->end(); ++iter) {

    const reco::PFCandidate *particle = &(*iter);
    double et=particle->et();
    double energy=particle->energy();
    double pt=particle->pt();
    double p=particle->p();
    double px=particle->px();
    double py=particle->py();
    double pz=particle->pz();
    double eta=particle->eta();
    double charge=particle->charge();
    double theta=particle->theta();

    // Fill 2D TTree (eta,energy);

    //eta cut - excluding ring 12 13 HF  
    if (fabs(eta)>4.7) continue;

    //int type=particle->particleId();

    if (particle->particleId()==reco::PFCandidate::e) electronEnergy.push_back(et);
    if (particle->particleId()==reco::PFCandidate::mu) muEnergy.push_back(et);

    TLorentzVector tmp(px,py,pz,energy);

    if  (  (fabs(charge) >0 && pt >  PtThPFCharged ) ||
	(fabs(charge) == 0  && ( (fabs(eta) <= 1.5 && energy > EnThPFBar)  ||
				 (fabs(eta) > 1.5 && fabs(eta) <= 3 && energy > EnThPFEnd) ||
				 (fabs(eta) > 3 && energy >EnThPFFw) ) )   )
    {        

      nPart_PF++;

      Epz_PF_plus+=p+p*TMath::Cos(theta);
      Epz_PF_minus+=p-p*TMath::Cos(theta);
      xi_PF_minus += et * pow(2.71,-eta) / (7000);
      xi_PF_plus += et * pow(2.71,eta) / (7000);

      etaTimesEnergy+=eta*energy;
      sumpxModule +=fabs(px);
      sumpyModule +=fabs(py);
      sumpzModule +=fabs(pz);
      sumpx +=px;
      sumpy +=py;
      sumpz +=pz;
      sumEnergyPF +=energy;
      etas.push_back(eta);

    } 

  }

  eventData.SetXi_PF_minus(xi_PF_minus);
  eventData.SetXi_PF_plus(xi_PF_plus);
  eventData.SetEpz_PF_minus(Epz_PF_minus);
  eventData.SetEpz_PF_plus(Epz_PF_plus);
  eventData.SetMultiplicityPF(nPart_PF);
  eventData.SetSumEtaTimesEnergyPF(etaTimesEnergy);
  eventData.SetSumpxModulePF(sumpxModule);
  eventData.SetSumpyModulePF(sumpyModule);
  eventData.SetSumpzModulePF(sumpzModule);
  eventData.SetSumpxPF(sumpx);
  eventData.SetSumpyPF(sumpy);
  eventData.SetSumpzPF(sumpz);
  eventData.SetSumEnergyPF(sumEnergyPF);

  //// Computing GAPs
  //// adding two fake entries at +-4.9 in etas!!!
  etas.push_back(4.9);
  etas.push_back(-4.9);

  const int  size = (int) etas.size();
  int *sorted = new int[size];
  double *v = new double[size];
  double eta_gap_limplus = -10.0;
  double eta_gap_limminus = -10.0;

  for (int i=0; i<size; i++) {
    v[i] = etas[i];
    if (debug) std::cout<<v[i]<<std::endl;
  }
  TMath::Sort(size, v, sorted, true);

  if (size > 1) {
    double *diff = new double[size-1];
    int *diffsorted = new int[size-1];
    for (int i=0; i<(size-1); i++) {
      diff[i] = fabs(etas[sorted[i+1]]-etas[sorted[i]]);
    }

    TMath::Sort(size-1, diff, diffsorted, true);

    //checking the max gap
    double max_eta_gap=diff[diffsorted[0]];
    eta_gap_limminus = etas[sorted[diffsorted[0]+1]] ;
    eta_gap_limplus = etas[sorted[diffsorted[0]]] ;

    eventData.SetMaxGapPF(max_eta_gap);
    eventData.SetLimPlusGapPF(eta_gap_limplus);
    eventData.SetLimMinusGapPF(eta_gap_limminus);

    if (size>2) {
      double max_second_eta_gap=diff[diffsorted[1]];
      if (debug) std::cout<<" diff  " << diff[diffsorted[0]] << " sec " << diff[diffsorted[1]] << " diff size "<< diff[size-2] <<std::endl;
      eventData.SetSecondMaxGapPF(max_second_eta_gap);
    }

    else {
      eventData.SetSecondMaxGapPF(-999.);
    }

    delete [] diff;
    delete [] diffsorted;

  }

  else {

    eventData.SetMaxGapPF(-999.);
    eventData.SetSecondMaxGapPF(-999.);
    eventData.SetLimPlusGapPF(-999.);
    eventData.SetLimMinusGapPF(-999.);

  }

  delete [] sorted;
  delete [] v;

  //sorting electron energy
  const int  size3 = (int) electronEnergy.size();
  int *sorted3 = new int[size3];
  double *v3 = new double[size3];

  for (int i=0; i<size3; i++) {
    v3[i] = electronEnergy[i];
  }
  TMath::Sort(size3, v3, sorted3, true);
  for (int i=0; i<size3; i++) {
    electronEnergy[i] = v3[sorted3[i]];
  }

  //sorting muon energy
  const int  size4 = (int) muEnergy.size();
  int *sorted4 = new int[size4];
  double *v4 = new double[size4];

  for (int i=0; i<size4; i++) {
    v4[i] = muEnergy[i];
  }
  TMath::Sort(size4, v4, sorted4, true);
  for (int i=0; i<size4; i++) {
    muEnergy[i] = v4[sorted4[i]];
  }
  delete [] sorted3;
  delete [] v3;
  delete [] sorted4;
  delete [] v4;

  /// Loop to compute Mx2 a destra e a sinistra del GAP
  TLorentzVector dataMass_plus(0.,0.,0.,0.);
  TLorentzVector dataMass_minus(0.,0.,0.,0.);
  int nplus =0;
  int nminus =0;

  for (reco::PFCandidateCollection::const_iterator iter = PFCandidates->begin(); iter != PFCandidates->end(); ++iter) {
    const reco::PFCandidate *particle = &(*iter);
    double energy=particle->energy();
    double pt=particle->pt();
    double px=particle->px();
    double py=particle->py();
    double pz=particle->pz();
    double eta=particle->eta();
    double charge=particle->charge();

    //eta cut - excluding ring 12 13 HF  
    if (fabs(eta)>4.7) continue;

    TLorentzVector tmp(px,py,pz,energy); 

    if  (  (fabs(charge) >0 && pt >  PtThPFCharged ) ||
	(fabs(charge) == 0  && ( (fabs(eta) <= 1.5 && energy > EnThPFBar)  ||
				 (fabs(eta) > 1.5 && fabs(eta) <= 3 && energy > EnThPFEnd) ||
				 (fabs(eta) > 3 && energy >EnThPFFw) ) )   )
    {        

      if ( eta >= eta_gap_limplus ) {
	dataMass_plus+=tmp;
	nplus++;
      }
      else {
	dataMass_minus+=tmp;
	nminus++;
      }
    }
  }  // PF loop

  eventData.SetElectronEnergyPF(electronEnergy[0]); // First Electron, Fill Second Electron also. Eta, phi, pT and ISO from PF.
  eventData.SetMuEnergyPF(muEnergy[0]); // First Muon, Fill Second Muon also. Eta, phi, pT and ISO from PF.
  eventData.SetMultiplicityGapPlusPF(nplus);
  eventData.SetMultiplicityGapMinusPF(nminus);

}

//
// Fill Pat:Muon and Pat:Electron objects 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DiffractiveZAnalysis::fillZPat(DiffractiveZEvent& eventData, const edm::Event& event, const edm::EventSetup& setup){

  math::XYZPoint PrimaryVertex;
  TLorentzVector m1electron(0.,0.,0.,0.);
  TLorentzVector m2electron(0.,0.,0.,0.);
  bool firstelectron=false;
  bool secondelectron=false;
  double mee=0.;
  TLorentzVector m1muon(0.,0.,0.,0.);
  TLorentzVector m2muon(0.,0.,0.,0.);
  bool firstmuon=false;
  bool secondmuon=false;
  double mmumu=0.;
  double relIso=0.;

  edm::Handle<reco::VertexCollection> Vertexes;
  event.getByLabel("offlinePrimaryVertices", Vertexes); 

  edm::Handle <reco::PFCandidateCollection> PFCandidates;
  event.getByLabel("particleFlow",PFCandidates); 
  reco::PFCandidateCollection::const_iterator iter;

  edm::Handle<std::vector<pat::Muon> > muons;
  event.getByLabel("patMuons", muons);

  edm::Handle<std::vector<pat::Electron> > electrons;
  event.getByLabel("patElectrons", electrons);

  for( std::vector<pat::Electron>::const_iterator elec=electrons->begin(); elec!=electrons->end(); ++elec ){

    double pt=elec->pt();
    double charge=elec->charge();
    double phi=elec->phi();
    double eta=elec->eta();
    double relIsoE;

    relIsoE = (elec->dr03TkSumPt()+elec->dr03EcalRecHitSumEt()+elec->dr03HcalTowerSumEt())/elec->et();

    std::cout << "electron -> dr03 TK: " << elec->dr03TkSumPt() << "| dr04 Ecal: " << elec->dr04EcalRecHitSumEt() << " | dr04 Hcal: " << elec->dr04HcalTowerSumEt() << "relIsoE: " << relIsoE <<  std::endl;

    /*
       double sumPtR03e = elec->isolationR03().sumPt;
       double emEtR03e = elec->isolationR03().emEt;
       double hadEtR03e = elec->isolationR03().hadEt;    
       double relIsoR03e = (sumPtR03e + emEtR03e + hadEtR03e)/elec->pt();
       double sumPtR05e = elec->isolationR05().sumPt;
       double emEtR05e = elec->isolationR05().emEt;
       double hadEtR05e = elec->isolationR05().hadEt;
       double relIsoR05e = (sumPtR05e + emEtR05e + hadEtR05e)/elec->pt();
       std::cout<<"e 0.3 Radion Rel Iso: "<<relIsoR03e<<"e sumPt "<<sumPtR03e<<"e emEt "<<emEtR03e<<"e hadEt "<<hadEtR03e<<std::endl;
       std::cout<<"e 0.5 Radion Rel Iso: "<<relIsoR05e<<"e sumPt "<<sumPtR05e<<"e emEt "<<emEtR05e<<"e hadEt "<<hadEtR05e<<std::endl;
     */


    if (pt>=2 && (charge)>0){
      PrimaryVertex=elec->vertex();
      m1electron.SetPtEtaPhiM(pt,eta,phi,0.0);
      firstmuon=true;
    }

    if (pt>=2 && (charge)<0){
      PrimaryVertex=elec->vertex();
      m2electron.SetPtEtaPhiM(pt,eta,phi,0.0);
      secondelectron=true;
    }

  }
  if (firstelectron==true && secondelectron==true){
    TLorentzVector Zee = m1electron+m2electron;
    mee = Zee.M();
    firstelectron=false;
    secondelectron=false;
    std::cout << "Dielectron Mass: " << mee << std::endl;
    eventData.SetDiElectronMassPF(mee);
  }

  for( std::vector<pat::Muon>::const_iterator muon=muons->begin(); muon!=muons->end(); ++muon ){

    double charge=muon->charge();
    double phi=muon->phi();
    double eta=muon->eta();
    double pt=muon->pt();


    double sumPtR03 = muon->isolationR03().sumPt;
    double emEtR03 = muon->isolationR03().emEt;
    double hadEtR03 = muon->isolationR03().hadEt;    
    double relIsoR03 = (sumPtR03 + emEtR03 + hadEtR03)/muon->pt();
    double sumPtR05 = muon->isolationR05().sumPt;
    double emEtR05 = muon->isolationR05().emEt;
    double hadEtR05 = muon->isolationR05().hadEt;    
    double relIsoR05 = (sumPtR05 + emEtR05 + hadEtR05)/muon->pt();
    std::cout<<" 0.3 Radion Rel Iso: "<<relIsoR03<<" sumPt "<<sumPtR03<<" emEt "<<emEtR03<<" hadEt "<<hadEtR03<<std::endl;
    std::cout<<" 0.5 Radion Rel Iso: "<<relIsoR05<<" sumPt "<<sumPtR05<<" emEt "<<emEtR05<<" hadEt "<<hadEtR05<<std::endl;

    relIso = (muon->trackIso()+muon->ecalIso()+muon->hcalIso())/muon->pt();

    //std::cout << "muon -> dr03 TK: " << muon->dr03TkSumPt() << "| dr04 Ecal: " << muon->dr04EcalRecHitSumEt() << " | dr04 Hcal: " << muon->dr04HcalTowerSumEt() << std::endl;

    //std::cout << "muon -> TrackIso(): " << muon->userIsolation() << std::endl;

    std::cout << "muon -> trackIso(): " << muon->trackIso() << " | muon -> ecalIso(): " << muon->ecalIso() << " | muon -> hcalIso(): " << muon->hcalIso() << " | muon->Iso(): " << relIso << std::endl; 



    if (pt>=2 && (charge)>0){
      PrimaryVertex=muon->vertex();
      m1muon.SetPtEtaPhiM(pt,eta,phi,0.0);     
      firstmuon=true;
    }
    if (pt>=2 && (charge)<0){
      PrimaryVertex=muon->vertex();
      m2muon.SetPtEtaPhiM(pt,eta,phi,0.0); 
      secondmuon=true;
    }
  }
  if (firstmuon==true && secondmuon==true){
    TLorentzVector Zmumu = m1muon+m2muon;
    mmumu = Zmumu.M();
    firstmuon=false;
    secondmuon=false;
    std::cout << "Dimuon Mass: " << mmumu << std::endl;
    eventData.SetDiMuonMassPF(mmumu);
  }

}
