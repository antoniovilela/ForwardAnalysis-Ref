// System Includes
#include <memory>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <time.h>
#include <math.h>
#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"
#include "TLorentzVector.h"
#include <cmath>
#include <string>

// Standard CMSSW Includes
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// MessageLogger
#include "FWCore/MessageLogger/interface/MessageLogger.h"

// ROOT Services
#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// Reco::TrackCollection
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

// Reco::CASTOR Tower
#include "DataFormats/CastorReco/interface/CastorJet.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"

// Reco::JetCollection
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/RecoCandidate/interface/RecoCaloTowerCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

//GenParticle
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

// DeltaR
#include "DataFormats/Math/interface/deltaR.h"

using namespace edm;
using namespace std;
using namespace math;
using namespace reco;

//
// class declaration
//

class ExclusiveSkim : public edm::EDFilter
{
public:
    explicit ExclusiveSkim(const edm::ParameterSet&);
    ~ExclusiveSkim();

private:
    virtual void beginJob() ;
    virtual bool filter(edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    // ----------member data ---------------------------

    string calAlgoFilter;
    double energyThresholdTower;
    double energyThresholdHF;
    double etaMaxLimit;
    double etaMinLimit;
    double pTJet1Limit;
    double pTJet2Limit;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

ExclusiveSkim::ExclusiveSkim(const edm::ParameterSet& iConfig) :
        calAlgoFilter( iConfig.getUntrackedParameter<string>("calAlgoFilter")),
        energyThresholdTower( iConfig.getParameter<double>( "energyThresholdTower" ) ),
        energyThresholdHF( iConfig.getParameter<double>( "energyThresholdHF" ) ),
	etaMaxLimit( iConfig.getParameter<double>( "etaMaxLimit" ) ),
	etaMinLimit( iConfig.getParameter<double>( "etaMinLimit" ) ),
	pTJet1Limit( iConfig.getParameter<double>( "pTJet1Limit" ) ),
	pTJet2Limit( iConfig.getParameter<double>( "pTJet2Limit" ) )
{
    //now do what ever initialization is needed

}


ExclusiveSkim::~ExclusiveSkim()
{

    // do anything here that needs to be done at desctruction time
    // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
ExclusiveSkim::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

bool FilterResult = false;

// Jet
/////////////////////

Handle<reco::CaloJetCollection> jetsis5;
iEvent.getByLabel("ak5CaloJets",jetsis5);

// Declaring Variables
int jet5size = jetsis5->size();
int Njets5=0.;
int ijet5;
const reco::CaloJet* jet1_max=NULL;
const reco::CaloJet* jet2_max=NULL;
const reco::CaloJet* jet1_min=NULL;
const reco::CaloJet* jet2_min=NULL;

// Jets Above Et cut
double jet1Eta_max = 0;
double jet2Eta_max = 0;
double jet1Eta_min = 0;
double jet2Eta_min = 0;
double jet1Phi_max = 0;
double jet2Phi_max = 0;
double jet1Phi_min = 0;
double jet2Phi_min = 0;
double jet1Pt_max = 0;
double jet2Pt_max = 0;
double jet1Pt_min = 0;
double jet2Pt_min = 0;


// Particle Flow Objects

       //////////////////////////////////
       // Particle Flow Eta Max e Eta Min
       //////////////////////////////////

Handle<reco::PFCandidateCollection> pfsis;
iEvent.getByLabel("particleFlow",pfsis);

// Declaring Variables
int pfsissize = pfsis->size();
int Npfsis=0.;
int ipfsis;
const reco::PFCandidate* pfsis1_max=NULL;
const reco::PFCandidate* pfsis2_max=NULL;
const reco::PFCandidate* pfsis1_min=NULL;
const reco::PFCandidate* pfsis2_min=NULL;

// Jets Above Et cut
double pfsis1Eta_max = 0;
double pfsis2Eta_max = 0;
double pfsis1Eta_min = 0;
double pfsis2Eta_min = 0;


   if (pfsissize >= 2){

   for(ipfsis=0; ipfsis < pfsissize; ++ipfsis){
   ++Npfsis;		
   const reco::PFCandidate* pfsis_max = &((*pfsis)[ipfsis]);

      
            if (pfsis_max==NULL) continue;
            //cout << "PF[" << ipfsis<< "]: Eta = "<< pfsis_max->eta() << endl;
            if (pfsis1_max==NULL) {pfsis1_max=pfsis_max; continue;}
            if (pfsis_max->eta()>pfsis1_max->eta()) {
            pfsis2_max=pfsis1_max;
            pfsis1_max=pfsis_max;
            continue;
            }
   
         if (pfsis2_max==NULL) {pfsis2_max=pfsis_max; continue;}
         if (pfsis_max->eta()>pfsis2_max->eta()) pfsis2_max = pfsis_max;


    }

    pfsis1Eta_max = pfsis1_max->eta();
    pfsis2Eta_max = pfsis2_max->eta();

    //cout << "Maximum Eta" << pfsis1Eta_max << endl;
    //cout << "Second Maximum Eta" << pfsis2Eta_max << endl;


    }

   if (pfsissize >= 2){

   for(ipfsis=0; ipfsis < pfsissize; ++ipfsis){
   ++Npfsis;		
   const reco::PFCandidate* pfsis_min = &((*pfsis)[ipfsis]);

      
            if (pfsis_min==NULL) continue;
            //cout << "PF[" << ipfsis<< "]: Eta = "<< pfsis_min->eta() << endl;
            if (pfsis1_min==NULL) {pfsis1_min=pfsis_min; continue;}
            if (pfsis_min->eta()<pfsis1_min->eta()) {
            pfsis2_min=pfsis1_min;
            pfsis1_min=pfsis_min;
            continue;
            }
   
         if (pfsis2_min==NULL) {pfsis2_min=pfsis_min; continue;}
         if (pfsis_min->eta()<pfsis2_min->eta()) pfsis2_min = pfsis_min;

    }

    pfsis1Eta_min = pfsis1_min->eta();
    pfsis2Eta_min = pfsis2_min->eta();

    //cout << "Minimum Eta" << pfsis1Eta_min << endl;
    //cout << "Second Minimum Eta" << pfsis2Eta_min << endl;

    }

// end PF Objects



Handle<CaloTowerCollection> caloTowers;
iEvent.getByLabel( "towerMaker", caloTowers );

double HFEnergyMinus = 0;
double HFEnergyPlus = 0;
bool signalpf;
bool signaljet;

        for ( CaloTowerCollection::const_iterator cal = caloTowers->begin(); cal != caloTowers->end(); ++ cal )
        {
            if (fabs(cal->eta()) > 2.9 && fabs(cal->eta()) < 5.2)
            {
                if (cal->energy() > energyThresholdTower)
                {
                    if (cal->zside() < 0)
                    {
                        HFEnergyMinus += cal->energy();
                    }
                    if (cal->zside() > 0)
                    {
                        HFEnergyPlus += cal->energy();
                    }
                }
            }

        }


        if (HFEnergyMinus < energyThresholdHF && HFEnergyPlus < energyThresholdHF){

                      if (pfsis1Eta_max < etaMaxLimit && pfsis1Eta_min > etaMinLimit){
                       signalpf = true;
                      }
                      else signalpf = false;

        }

if (jet5size >= 2){

   for(ijet5=0; ijet5 < jet5size; ++ijet5){
   ++Njets5;		
   const reco::CaloJet* jetAll_max = &((*jetsis5)[ijet5]);

      
            if (jetAll_max==NULL) continue;
            //cout << "Jet[" << ijet5<< "]: Eta = "<< jetAll_max->eta() << endl;
            if (jet1_max==NULL) {jet1_max=jetAll_max; continue;}
            if (jetAll_max->eta()>jet1_max->eta()) {
            jet2_max=jet1_max;
            jet1_max=jetAll_max;
            continue;
            }
   
         if (jet2_max==NULL) {jet2_max=jetAll_max; continue;}
         if (jetAll_max->eta()>jet2_max->eta()) jet2_max = jetAll_max;


    }

    jet1Eta_max = jet1_max->eta();
    jet2Eta_max = jet2_max->eta();
    jet1Phi_max = jet1_max->phi();
    jet2Phi_max = jet2_max->phi();
    jet1Pt_max = jet1_max->pt();
    jet2Pt_max = jet2_max->pt();

    }

if (jet5size >= 2){

   for(ijet5=0; ijet5 < jet5size; ++ijet5){
   ++Njets5;		
   const reco::CaloJet* jetAll_min = &((*jetsis5)[ijet5]);

            if (jetAll_min==NULL) continue;
            //cout << "Jet[" << ijet5<< "]: Eta = "<< jetAll_min->eta() << endl;
            if (jet1_min==NULL) {jet1_min=jetAll_min; continue;}
            if (jetAll_min->eta()<jet1_min->eta()) {
            jet2_min=jet1_min;
            jet1_min=jetAll_min;
            continue;
            }
   
         if (jet2_min==NULL) {jet2_min=jetAll_min; continue;}
         if (jetAll_min->eta()<jet2_min->eta()) jet2_min = jetAll_min;

    }

    jet1Eta_min = jet1_min->eta();
    jet2Eta_min = jet2_min->eta();
    jet1Phi_min = jet1_min->phi();
    jet2Phi_min = jet2_min->phi();
    jet1Pt_min = jet1_min->pt();
    jet2Pt_min = jet2_min->pt();

    }

//    if (fabs(jet1Phi_max - jet1Phi_min) > 2){
       
       
if ( (jet1Pt_max > pTJet1Limit && jet2Pt_max > pTJet2Limit) ){
                   signaljet = true;
}
            
else {signaljet = false;}
   
FilterResult = signaljet & signalpf;      

return FilterResult;


}

// ------------ method called once each job just before starting event loop  ------------
void
ExclusiveSkim::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
ExclusiveSkim::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(ExclusiveSkim);

