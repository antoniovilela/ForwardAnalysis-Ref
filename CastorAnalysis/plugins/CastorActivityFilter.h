// -*- C++ -*-
//
// Package:   CastorActivityFilter
// Class:     CastorActivityFilter
//
// Original Author:  Panos Katsas

#ifndef CastorActivityFilter_H
#define CastorActivityFilter_H

// system include files
#include <memory>
#include <vector>
#include <map>
#include <set>

// user include files
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/HcalDetId/interface/HcalCastorDetId.h" 
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h" 
#include "DQM/CastorMonitor/interface/CastorBunch.h"
//
// class declaration
//

class CastorActivityFilter : public edm::EDFilter {
 public:
  explicit CastorActivityFilter( const edm::ParameterSet & );
  ~CastorActivityFilter();
  
 private:
  virtual bool filter ( edm::Event &, const edm::EventSetup & );
  
  int firstTS;
  int lastTS;
  double totalEnergyThreshold;
  int iEvt_;
  bool firstTime_;
  int numBunches_;
  std::vector<NewBunch> Bunches_;

};

#endif
