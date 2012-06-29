#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

#include <TH1F.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"

#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "PhysicsTools/FWLite/interface/CommandLineParser.h"


int main(int argc, char* argv[]) 
{
  // define what muon you are using; this is necessary as FWLite is not 
  // capable of reading edm::Views
  //using reco::Muon;
  //using reco::CaloJet;

  // ----------------------------------------------------------------------
  // First Part: 
  //
  //  * enable the AutoLibraryLoader 
  //  * book the histograms of interest 
  //  * open the input file
  // ----------------------------------------------------------------------

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // initialize command line parser
  optutl::CommandLineParser parser ("Analyze FWLite Histograms");

  // set defaults
  parser.integerValue ("maxEvents"  ) = 1000;
  parser.integerValue ("outputEvery") =   10;
  parser.stringValue  ("outputFile" ) = "analyzeFWLiteHistograms.root";

  // parse arguments
  parser.parseArguments (argc, argv);
  int maxEvents_ = parser.integerValue("maxEvents");
  unsigned int outputEvery_ = parser.integerValue("outputEvery");
  std::string outputFile_ = parser.stringValue("outputFile");
  std::vector<std::string> inputFiles_ = parser.stringVector("inputFiles");

  // book a set of histograms
  fwlite::TFileService fs = fwlite::TFileService(outputFile_.c_str());
  TFileDirectory dir = fs.mkdir("analyzeBasicPat");

  
  float etaBinsHCALBoundaries[] = {-5.205, -4.903, -4.730,
                                   -4.552, -4.377, -4.204, -4.027, -3.853, -3.677, -3.503, -3.327, -3.152,
                                   -3.000, -2.868, -2.650, -2.500, 
                                   -2.322, -2.172, -2.043, -1.930, -1.830, -1.740, -1.653, -1.566, -1.479,
                                   -1.392, -1.305, -1.218, -1.131, -1.044, -0.957, -0.870, -0.783, 
                                   -0.696, -0.609, -0.522, -0.435, -0.348, -0.261, -0.174, -0.087,
                                    0.000, 0.087, 0.174, 0.261, 0.348, 0.435, 0.522, 0.609, 0.696,
                                    0.783, 0.870, 0.957, 1.044, 1.131, 1.218, 1.305, 1.392,
                                    1.479, 1.566, 1.653, 1.740, 1.830, 1.930, 2.043, 2.172, 2.322,
                                    2.500, 2.650, 2.868, 3.000,
                                    3.152, 3.327, 3.503, 3.677, 3.853, 4.027, 4.204, 4.377, 4.552,
                                    4.730, 4.903, 5.205}; // 41 + 41 bins

  TH1F* h_etaMaxFromPFCands_  = dir.make<TH1F>("etaMaxFromPFCandsVarBin","etaMaxFromPFCandsVarBin",82,etaBinsHCALBoundaries);

  // loop the events
  int ievt=0;  
  for(unsigned int iFile=0; iFile<inputFiles_.size(); ++iFile){
    // open input file (can be located on castor)
    TFile* inFile = TFile::Open(inputFiles_[iFile].c_str());
    if( inFile ){
      // ----------------------------------------------------------------------
      // Second Part: 
      //
      //  * loop the events in the input file 
      //  * receive the collections of interest via fwlite::Handle
      //  * fill the histograms
      //  * after the loop close the input file
      // ----------------------------------------------------------------------      
      fwlite::Event ev(inFile);
      for(ev.toBegin(); !ev.atEnd(); ++ev, ++ievt){
	edm::EventBase const & event = ev;
	// break loop if maximal number of events is reached 
	if(maxEvents_>0 ? ievt+1>maxEvents_ : false) break;
	// simple event counter
	if(outputEvery_!=0 ? (ievt>0 && ievt%outputEvery_==0) : false) 
	  std::cout << "  processing event: " << ievt << std::endl;

	// Handle to the muon collection
	edm::Handle<DiffractiveEvent> eventdiff_;
	event.getByLabel(std::string("diffractiveAnalysis"), eventdiff_);
  
        h_etaMaxFromPFCands_->Fill( eventdiff_->GetEtaMaxFromPFCands() ); 
      }  
      // close input file
      inFile->Close();
    }
    // break loop if maximal number of events is reached:
    // this has to be done twice to stop the file loop as well
    if(maxEvents_>0 ? ievt+1>maxEvents_ : false) break;
  }
  return 0;
}
