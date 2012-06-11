#ifndef PhojetInterface_Phojet_h
#define PhojetInterface_Phojet_h

/** \class Phojet
 *
 * Generates Phojet HepMC events - Standalone
 *
 ***************************************/

#define PYCOMP pycomp_

#include "HepMC/GenEvent.h"
#include "CLHEP/Random/JamesRandom.h"
#include "CLHEP/Random/RandFlat.h"

#include <vector>
#include <string>
#include <ostream>

namespace phojet
{
  class Phojet{
  public:
    Phojet(double, long, std::vector<std::string> const&);
    ~Phojet();

    void begin();
    bool run();
    void end();
    void write(std::ostream&);

    const HepMC::GenEvent* event() const { return hepMCEvt_; } 

  private:
    void getProtonKinematics(double* p1, double* p2);
    
    HepMC::GenEvent  *hepMCEvt_;
 
    /// Pythia PYLIST Verbosity flag
    unsigned int pythiaPylistVerbosity_;
    /// HepMC verbosity flag
    bool pythiaHepMCVerbosity_;
    /// Events to print if verbosity
    unsigned int maxEventsToPrint_;    
   
    unsigned int event_;
    double comEnergy_;
    long seed_;
    double sigmaMax_; 
    std::vector<std::string> params_;
  
    CLHEP::HepRandomEngine* fRandomEngine;
    CLHEP::RandFlat*        fRandomGenerator; 
  };
} 

#endif
