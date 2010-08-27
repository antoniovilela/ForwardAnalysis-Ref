// -*- C++ -*-
//
// Package:    CastorActivityFilter
// Class:      CastorActivityFilter
// 
/**\class CastorActivityFilter CastorActivityFilter.cc Filters/CastorActivityFilter/src/CastorActivityFilter.cc

 Description: Filter for selecting events with a rapidity gap in CASTOR
 
 Implementation:
     EDFilter for selecting events with a rapidity gap in CASTOR. Energy summed in a range of time slices. 
     First and last time slice to use is configurable. Same for the energy threshold (in fC). Alternatively
     we could require the signal in each channel to be < pedestal + 3*sigma...  
*/

//
// Original Author:  Panagiotis Katsas,32 4-A16,+41227679151,
//         Created:  Wed Aug 18 13:42:19 CEST 2010
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "ForwardAnalysis/CastorAnalysis/plugins/CastorActivityFilter.h"

#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"
#include "DataFormats/HcalDetId/interface/HcalElectronicsId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "CondFormats/CastorObjects/interface/CastorPedestals.h"
#include "CondFormats/CastorObjects/interface/CastorPedestalWidths.h"
#include "CondFormats/CastorObjects/interface/CastorQIECoder.h"
#include "CondFormats/CastorObjects/interface/CastorQIEData.h"
#include "CondFormats/CastorObjects/interface/CastorQIEShape.h"
#include "CondFormats/CastorObjects/interface/CastorElectronicsMap.h"
#include "CondFormats/CastorObjects/interface/AllObjects.h"
#include "CalibFormats/CastorObjects/interface/CastorDbRecord.h"
#include "CalibFormats/CastorObjects/interface/CastorDbService.h"
#include "CalibFormats/CastorObjects/interface/CastorCalibrations.h"
#include "CalibFormats/CastorObjects/interface/CastorCalibrationWidths.h"
#include "CalibCalorimetry/CastorCalib/interface/CastorDbASCIIIO.h"
//
// class declaration
//

CastorActivityFilter::CastorActivityFilter(const edm::ParameterSet& iConfig)
{
  iEvt_=0;
  firstTime_ = true;
  firstTS = iConfig.getUntrackedParameter<int>("firstTS", 4);
  lastTS = iConfig.getUntrackedParameter<int>("lastTS", 6);
  totalEnergyThreshold = iConfig.getUntrackedParameter<double>("totalEnergyThreshold", 100.);
}


CastorActivityFilter::~CastorActivityFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
CastorActivityFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   double sumTotalSignal = 0.;
   int channelsBelowNoise = 0;
   double pedestalfC;
   double channelSignal[224];
   int channelCounter = 0;

  Handle<CastorDigiCollection> castorDigis;
  try{
     iEvent.getByType(castorDigis);
     edm::ESHandle<CastorDbService> conditions;
     iSetup.get<CastorDbRecord>().get(conditions);
     const CastorQIEShape* shape = conditions->getCastorShape(); 

     if(firstTime_)      
       {
	 edm::ESHandle<CastorElectronicsMap> refEMap;
	 iSetup.get<CastorElectronicsMapRcd>().get(refEMap);
	 const CastorElectronicsMap* myRefEMap = refEMap.product();
	 std::vector<HcalGenericDetId> listEMap = myRefEMap->allPrecisionId();
	 for (std::vector<HcalGenericDetId>::const_iterator it = listEMap.begin(); it != listEMap.end(); it++)
	   {     
	     HcalGenericDetId mygenid(it->rawId());
	     if(mygenid.isHcalCastorDetId())
	       {
		 NewBunch a; // new bunch is like the digi holding a bit different variables 
		 HcalCastorDetId chanid(mygenid.rawId());
		 a.detid = chanid;
		 a.usedflag = false;
		 std::string type;
		 type = "CASTOR";
		 for(int i = 0; i != 20; i++)
		   {
		     a.tsCapId[i] = 0;
		     a.tsAdc[i] = 0;
		     a.tsfC[i] = 0.0;
		   }
		 Bunches_.push_back(a);
	       }
	   }
	 firstTime_ = false;
       }

 if(castorDigis->size()>0) {
 std::vector<NewBunch>::iterator bunch_it;
 numBunches_ = 0;

 for(CastorDigiCollection::const_iterator j = castorDigis->begin(); j != castorDigis->end(); j++)
   {
     channelCounter++;
     const CastorDataFrame digi = (const CastorDataFrame)(*j);
     if ( lastTS+1 > digi.size() ) lastTS = digi.size()-1;
     for(bunch_it = Bunches_.begin(); bunch_it != Bunches_.end(); bunch_it++)
       if(bunch_it->detid.rawId() == digi.id().rawId()) break;
     bunch_it->usedflag = true;
     numBunches_++; // 224 "bunches", i.e. digis 
     if ( lastTS+1 > digi.size() ) lastTS = digi.size()-1;

     const CastorCalibrations& calibrations=conditions->getCastorCalibrations(digi.id().rawId());
     double totalSignal = 0.0;
     for(int ts = firstTS; ts != lastTS+1; ts++) // loop over time slices
      {
	if (ts < 10) {
	  const CastorQIECoder* coder = conditions->getCastorCoder(digi.id().rawId());
	  bunch_it->tsCapId[ts] = digi.sample(ts).capid();
	  bunch_it->tsAdc[ts] = digi.sample(ts).adc();
	  double chargefC = coder->charge(*shape, digi.sample(ts).adc(), digi.sample(ts).capid());
	  double pedestalfC_TS1 = coder->charge(*shape, digi.sample(0).adc(), digi.sample(0).capid());
	  double pedestalfC_TS2 = coder->charge(*shape, digi.sample(1).adc(), digi.sample(1).capid());
	  double pedestalfC_TS3 = coder->charge(*shape, digi.sample(2).adc(), digi.sample(2).capid());
	  pedestalfC = (pedestalfC_TS1 + pedestalfC_TS2 + pedestalfC_TS3)/3.;  // get mean pedestal 
	  bunch_it->tsfC[ts] = chargefC - pedestalfC;
	  //	  bunch_it->tsfC[ts] = chargefC - 2.6*calibrations.pedestal(digi.sample(ts).capid()); // factor 2.6 for converting the pedestal in ADC to fC in case of geting the pedestals from the db
	  totalSignal += bunch_it->tsfC[ts]; // sum signal in selected range of time slices per channel 
	  channelSignal[channelCounter] = totalSignal;
	}
      } 

     std::cout << " TS sum total signal = " << totalSignal << " channel " << channelCounter << "signal = " << channelSignal[channelCounter] << std::endl;

     sumTotalSignal += totalSignal; // total signal in CASTOR

     std::cout << "sumTotal = " << sumTotalSignal << std::endl;

    } // digis loop 
  } // if castorDigis->size() > 0 
} // try 

  catch(...) {
    std::cout << " Castor ERROR !" << std::endl;
  }
  std::cout << " sumTotalSignal=" << sumTotalSignal << " threshold is " << totalEnergyThreshold << std::endl; 

  for (int ich=0; ich<224; ich++) {
      if ( channelSignal[ich] < 2*pedestalfC ) channelsBelowNoise++;
    }

  std::cout << "Number of channels below noise " << channelsBelowNoise << std::endl;
   if (sumTotalSignal <= totalEnergyThreshold ) { 
  //  if ( channelsBelowNoise == 224 ) {
   std::cout << "ACCEPTED" << std::endl;
  }
    if ( sumTotalSignal <= totalEnergyThreshold ) return true;
   //  if ( channelsBelowNoise == 224 ) return true;
 else
    return false;

}

//define this as a plug-in
DEFINE_FWK_MODULE(CastorActivityFilter);
