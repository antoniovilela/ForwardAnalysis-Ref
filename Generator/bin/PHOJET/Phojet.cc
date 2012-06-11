/*
 *  $Date: 2012/06/11 13:01:34 $
 *  $Revision: 1.1 $
 *  
 */

#include "Phojet.h"
//#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "fostream.h"
#include "time.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

//using namespace edm;
using namespace std;
using namespace phojet;

#include "HepMC/PythiaWrapper6_2.h"
#include "HepMC/IO_HEPEVT.h"

//-------------------------PHOJET functions ------------------------------
extern"C" {
  void pho_event_(int&,double*,double*,double&,int&);
  void pho_pecms_(int&,double&,double&,double&,double&,double&);
  void initphojet_(int&,int&);
}
#define pho_event pho_event_
#define pho_pecms pho_pecms_
#define initphojet initphojet_

extern "C" {
  extern struct {
     int IPROCE,IDNODF,IDIFR1,IDIFR2,IDDPOM,IPRON[4][15];
  } poprcs_;
}
#define poprcs poprcs_

HepMC::IO_HEPEVT conv;

/*//used for defaults
  static const unsigned long kNanoSecPerSec = 1000000000;
  static const unsigned long kAveEventPerSec = 200;*/

Phojet::Phojet(double comEnergy, long int seed, vector<string> const& params):
  event_(0), 
  comEnergy_(comEnergy),
  seed_(seed), 
  params_(params) {
  
  pythiaPylistVerbosity_ = true;
  pythiaHepMCVerbosity_ = true;
  maxEventsToPrint_ = 5;
}

Phojet::~Phojet(){
}

void Phojet::write(std::ostream& out){
  hepMCEvt_->write(out);
}

void Phojet::begin() {
  event_ = 0;

  int iunit = 10;	
  fostream phoconfig(iunit,"phojet.cfg");	
  phoconfig << "PARTICLE1   2212       0.0";
  phoconfig << "PARTICLE2   2212       0.0";

  std::vector<std::string> invalidParams;
  invalidParams.push_back("WW-HERA");
  invalidParams.push_back("GP-HERA");
  invalidParams.push_back("GG-EPEM");
  invalidParams.push_back("WW-EPEM");
  invalidParams.push_back("GG-HION-F");
  invalidParams.push_back("GG-HION-G");
  invalidParams.push_back("GH-HION-F");
  invalidParams.push_back("HG-HIAS-F");
  invalidParams.push_back("BLASER");
  invalidParams.push_back("BEAMST");
  invalidParams.push_back("EVENT-LAB");
  invalidParams.push_back("EVENT-CMS");
  invalidParams.push_back("EVENT-COLL");
  invalidParams.push_back("READ-RNDM");
  invalidParams.push_back("SAVE-RNDM");
  invalidParams.push_back("INIT-RNDM");
  invalidParams.push_back("PARTICLE1");
  invalidParams.push_back("PARTICLE2");

  // Loop over all parameters and stop in case of mistake
  for( vector<string>::const_iterator itPar = params_.begin(); itPar != params_.end(); ++itPar ) {
     for(std::vector<std::string>::const_iterator itInvPar = invalidParams.begin(); itInvPar != invalidParams.end(); ++itInvPar){
        if( 0 == itPar->compare(0,itInvPar->size(),*itInvPar) ){
           //throw edm::Exception(edm::errors::Configuration,"PhojetError") << " Following PHOJET parameter is not accepted in this mode: \"" << *itInvPar << "\"";
           stringstream oss;
           oss << "PhojetError: The following PHOJET parameter is not accepted in this mode: " << endl
               << *itInvPar << endl;      
           throw runtime_error( oss.str() );
        }
     }
     phoconfig << itPar->c_str();
  }
  	
  //edm::Service<RandomNumberGenerator> rng;
  //long seed = (long)rng->mySeed();
  //long seed = 12345;
  cout << " seed= " << seed_ << endl;
  fRandomEngine = new CLHEP::HepJamesRandom(seed_);
  fRandomGenerator = new CLHEP::RandFlat(fRandomEngine);
 
  long seed0 = fRandomGenerator->fireInt(1L,179L);
  long seed1 = fRandomGenerator->fireInt(1L,179L);	 
  long seed2 = fRandomGenerator->fireInt(1L,179L);
  long seed3 = fRandomGenerator->fireInt(1L,179L);

  std::ostringstream sRandomSet;
  sRandomSet << "INIT-RNDM " << seed0 << "   " << seed1 << "   " << seed2 << "   " << seed3;
  phoconfig << sRandomSet.str().c_str();
  //phoconfig << (const char*)"ENDINPUT  ";
  phoconfig << "ENDINPUT  ";

  phoconfig.rewind();
  int irej;
  initphojet(iunit,irej);
  /*if(irej != 0) throw edm::Exception(edm::errors::Configuration,"PhojetError") 
                        <<" Error initializing Phojet.";*/
  if(irej != 0) throw runtime_error("PhojetError: Error initializing Phojet.");

  double p1[4] = {0.,0.,0.,0.};
  double p2[4] = {0.,0.,0.,0.};
  getProtonKinematics(p1,p2);

  double sigmax;
  int mode = -1; 
  pho_event(mode,p1,p2,sigmax,irej);

  sigmaMax_ = sigmax;
  //cout << "Sigmax = " << sigmax << endl; 
  //cout << endl; // Stetically add for the output
  //********                                      
}

void Phojet::end() {
 
  double p1[4] = {0.,0.,0.,0.};
  double p2[4] = {0.,0.,0.,0.};
  getProtonKinematics(p1,p2);

  int mode = -2;
  int irej;
  double sigcur;
  pho_event(mode,p1,p2,sigcur,irej);
}

bool Phojet::run() {

  double p1[4] = {0.,0.,0.,0.};
  double p2[4] = {0.,0.,0.,0.};
  getProtonKinematics(p1,p2);

  int mode = 1;
  int irej;
  double sigcur; 	
  while(1) {
    pho_event(mode,p1,p2,sigcur,irej);
    if(irej != 0){
      //LogWarning("") << "   Problem in event generation...skipping and retrying.\n";
      cerr << "Phojet: Problem in event generation...skipping and retrying." << endl;
    } else {
      break;
    }
  }

  call_pyhepc( 1 );

  //HepMC::GenEvent* evt = conv.read_next_event();
  hepMCEvt_ = conv.read_next_event();
  ++event_;

  //hepMCEvt_->set_signal_process_id(pypars.msti[0]);
  hepMCEvt_->set_signal_process_id(poprcs.IPROCE);
  hepMCEvt_->set_event_scale(pypars.pari[16]);
  //hepMCEvt_->set_event_number(numberEventsInRun() - remainingEvents() - 1);
  hepMCEvt_->set_event_number(event_ - 1);
 
  //******** Verbosity ********
  if(event_ <= maxEventsToPrint_ &&
     (pythiaPylistVerbosity_ || pythiaHepMCVerbosity_)) {

     // Prints PYLIST info
     //
     if(pythiaPylistVerbosity_) {
        call_pylist(pythiaPylistVerbosity_);
     }
      
     // Prints HepMC event
     //
     if(pythiaHepMCVerbosity_) {
        stringstream oss;
        oss << "\n----------------------" << endl	
            << "Event process id = " << hepMCEvt_->signal_process_id() << endl; 
        cout << oss.str();
        hepMCEvt_->print();
     }
  }   
  return true;
}

void Phojet::getProtonKinematics(double* p1, double* p2){
  double pmass1 = 0.938;
  double pmass2 = 0.938;

  double e1 = comEnergy_/2.;
  double pp1 = sqrt(e1*e1 - pmass1*pmass1);
  double e2 = comEnergy_/2.;
  double pp2 = sqrt(e2*e2 - pmass2*pmass2);

  double p1_new[4] = {0.,0.,pp1,e1};
  double p2_new[4] = {0.,0.,-pp2,e2};

  for(int k = 0; k < 4; ++k){
     p1[k] = p1_new[k];
     p2[k] = p2_new[k];
  }
}
