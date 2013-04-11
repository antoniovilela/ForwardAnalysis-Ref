//-------------------------------------------------------------------------------------------------------->>
// UNIVERSIDADE DO ESTADO DO RIO DE JANEIRO - CMS/Brasil
//-------------------------------------------------------------------------------------------------------->>
//
// Twiki: https://twiki.cern.ch/twiki/bin/view/CMS/UsingPATonFWDDiJets#Macro_Analysis_Example
//

//
// USAGE:
//
// 1) Compile and Copy Binary to your own work area
// ?> cp $CMSSW_BASE/test/slc5_amd64_gcc434/TemplateExample . 
//
// 2) RUNNING
// ?> ./TemplateExample <std::string filein_> <std::string savehistofile_> <std::string processname_> <std::string type_>

//#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TROOT.h>
#include <TChain.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TMath.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

#include "TemplateExample.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfoEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"

using namespace diffractiveAnalysis;
using namespace exclusiveDijetsAnalysis;
using namespace eventInfo;

void TemplateExample::LoadFile(std::string fileinput, std::string processinput){

//
// Open file, declare dictionary class and objects.
//

  inf = NULL;
  tr  = NULL;
  inf = TFile::Open(fileinput.c_str(),"read");
  std::string fdirectory = processinput + "/ProcessedTree";
  tr = (TTree*)inf->Get(fdirectory.c_str());

// Declare Class (defined in PATDiffractive package)

  eventdiff = new DiffractiveEvent();
  eventexcl = new ExclusiveDijetsEvent();
  eventinfo = new EventInfoEvent();
  diff = tr->GetBranch("DiffractiveAnalysis");
  excl = tr->GetBranch("ExclusiveDijetsAnalysis");
  info = tr->GetBranch("EventInfo");

// Declare Objects

  diff->SetAddress(&eventdiff);
  excl->SetAddress(&eventexcl);
  info->SetAddress(&eventinfo);

}

void TemplateExample::CreateHistos(std::string type){

//
// Create All Analysis Histograms for each cut. Using histogram vectors. 
//


// Define tag name of histograms for each cut
 
  std::string step0 = "without_cuts_" + type;
  std::string step1 = "with_type_" + type;  
  std::string step2 = "d_eta4_" + type;
  std::string step3 = "d_eta3_" + type;
  std::string step4 = "d_eta2_" + type;
  std::string step5 = "d_eta1_" + type;

  Folders.push_back(step0);
  Folders.push_back(step1);
  Folders.push_back(step2);
  Folders.push_back(step3);
  Folders.push_back(step4);
  Folders.push_back(step5);

// Define each variable histogram, for each cut

  for (std::vector<std::string>::size_type j=0; j<Folders.size(); j++){

    // For instance: Tracks_without_cuts_#, Tracks_with_type_#, Tracks_d_eta4_#, Tracks_d_eta3_#, Tracks_d_eta2_# and Tracks_d_eta1_#.
    // where # is std::string type.

    char name1[300];
    sprintf(name1,"Tracks_%s",Folders.at(j).c_str());
    TH1D *histo_Tracks = new TH1D(name1,"Tracks Multiplicity; n Tracks; N events",50,0,150);
    m_hVector_tracks.push_back(histo_Tracks);

    char name2[300];
    sprintf(name2,"vertex_%s",Folders.at(j).c_str());
    TH1D *histo_vertex = new TH1D(name2,"Number of Vertex; # Vertex; N events",25,0,25);
    m_hVector_vertex.push_back(histo_vertex);

    char name3[300];
    sprintf(name3,"lumi_%s",Folders.at(j).c_str());
    TH1D *histo_lumi = new TH1D(name3,"Luminosity per Bunch; L_{Bunch} [#mub^{-1}s^{-1}]; N events",25,0,2);
    m_hVector_lumi.push_back(histo_lumi);

  }

}

void TemplateExample::FillHistos(int index){

//
// Fill your histograms. For each variable. 
//

// For instance m_hVector_tracks has Folders.size() dimension. Each dimension corresponds an analysis cut. 
  m_hVector_tracks[index]->Fill(eventdiff->GetMultiplicityTracks());
  m_hVector_vertex[index]->Fill(eventexcl->GetNVertex());
  m_hVector_lumi[index]->Fill(eventinfo->GetInstLumiBunch());
}

void TemplateExample::SaveHistos(){

//
// Write histograms in a file.
//

  for (std::vector<std::string>::size_type j=0; j<Folders.size(); j++){
    m_hVector_tracks[j]->Write();
    m_hVector_vertex[j]->Write();
    m_hVector_lumi[j]->Write();
  }

}

void TemplateExample::Run(std::string filein_, std::string savehistofile_, std::string processname_, std::string type_){

//
// Analysis code. Run receive external command line parameters: filein_, savehistofile_, processname_ and type_. See int main()  
//


  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  filein = filein_;
  savehistofile = savehistofile_;
  processname = processname_;
  type = type_;

// Check file integrity.

  TFile check1(filein.c_str());

  if (check1.IsZombie()){

    std::cout << "\n----------------------------------------------" << std::endl;
    std::cout << " There is no the file " << filein << " or the"   << std::endl;
    std::cout << " path is not correct." << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    return;

  }

  if (check1.GetDirectory(processname.c_str())){
    LoadFile(filein,processname);
  }

  else {
    std::cout << "\n-------------------------------------------------" << std::endl;
    std::cout << " There is no directory/path " << processname << std::endl;
    std::cout << " in the file." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    return;
  }

  TFile *outf = new TFile(savehistofile.c_str(),"RECREATE");
  TString outtxt = savehistofile;
  outtxt.ReplaceAll("root","txt");
  std::ofstream outstring(outtxt);

  int NEVENTS = tr->GetEntries();
  int decade = 0;
  int triggercounter[20]={0};

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  unsigned NEntries = tr->GetEntries();
  std::cout << "" << std::endl;
  std::cout<< "Reading Tree: "<< NEntries << " events"<<std::endl;
  std::cout << "" << std::endl;

  std::string status;  

// Loop all Events

  for(int i=0;i<NEVENTS;i++){

    double progress = 10.0*i/(1.0*NEVENTS);
    int l = TMath::FloorNint(progress); 

    if (l > decade){
      std::cout <<"\n<<<<<< STATUS >>>>>>" << std::endl; 
      std::cout<<10*l<<" % completed." << std::endl;
      std::cout <<"<<<<<<<<<<>>>>>>>>>>\n" << std::endl;
    }
    decade = l;          

    tr->GetEntry(i);

    for (int nt=0;nt<20;nt++){
      if(eventexcl->GetHLTPath(nt)){
	triggercounter[nt]++;
      }
    }

    bool triggerZeroBias = false;
    bool triggerHLTPlus = false;
    bool triggerHLTMinus = false;
    bool vertex = false;
    bool tracks = false;
    bool d_eta4 = false;
    bool d_eta3 = false;
    bool d_eta2 = false;
    bool d_eta1 = false;
    bool collisions = false;
    bool unpaired = false;
    bool presel = false;

// Define each analysis cut.

    if (eventexcl->GetHLTPath(0)) triggerZeroBias = true;
    if (eventexcl->GetHLTPath(1)) triggerHLTPlus = true;
    if (eventexcl->GetHLTPath(2)) triggerHLTMinus = true;
    if ((eventdiff->GetSumEnergyHFMinus() < 30 && eventdiff->GetSumEnergyHFPlus() < 30) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990)) presel = true;
    if (eventdiff->GetMultiplicityTracks() > 0) tracks = true;  
    if (eventexcl->GetNVertex() > 0) vertex = true;
    if ((eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990) ) d_eta4 = true;
    if ((eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990) ) d_eta3 = true; 
    if ((eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990) ) d_eta2 = true;
    if ((eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.) || (eventdiff->GetEtaMinFromPFCands() < -990 && eventdiff->GetEtaMaxFromPFCands() < -990) ) d_eta1 = true;

// Applying each cut and Filling histograms for each cut.

  std::string step0 = "without_cuts_" + type;
  std::string step1 = "with_type_" + type;
  std::string step2 = "d_eta4_" + type;
  std::string step3 = "d_eta3_" + type;
  std::string step4 = "d_eta2_" + type;
  std::string step5 = "d_eta1_" + type;



    if (type == "collisions"){
      if (triggerZeroBias && vertex && tracks) collisions = true;
      status = "collisions";
      FillHistos(0); // Fill HISTOS_without_cuts_collision step. See CreateHistos(std::string type). Where HISTOS are each histogram defined in CreateHistos.
                     // Please, do not forget to put them into FillHistos() to fill and into SaveHistos() to save in a file.  
      if (collisions) FillHistos(1); // HISTOS_with_type_collisions
      if (collisions && d_eta4) FillHistos(2); //HISTOS_d_eta4_collisions
      if (collisions && d_eta3) FillHistos(3); //HISTOS_d_eta3_collisions
      if (collisions && d_eta2) FillHistos(4); //HISTOS_d_eta2_collisions
      if (collisions && d_eta1) FillHistos(5); //HISTOS_d_eta1_collisions
    }

    else if (type == "unpaired"){
      if((triggerHLTPlus || triggerHLTMinus) && !vertex && !tracks) unpaired = true; 
      status = "unpaired";
      FillHistos(0);
      if (unpaired) FillHistos(1);
      if (unpaired && d_eta4) FillHistos(2);
      if (unpaired && d_eta3) FillHistos(3);
      if (unpaired && d_eta2) FillHistos(4);
      if (unpaired && d_eta1) FillHistos(5);
    }

    else {
      std::cout << "\n Unrecognized Type of Selection." << std::endl;
      return;
    }

  }   

// Saving *.txt file with number of events for each trigger defined in HLTPath in your *.py used to create your pattuple. HLTPath has 20 dimension.

  outstring << "" << std::endl;
  outstring << "<< INPUTS >>" << std::endl;
  outstring << " " << std::endl;
  outstring << "Input file: " << filein << std::endl;
  outstring << "Output file: " << savehistofile << std::endl;
  outstring << " " << std::endl;
  outstring << "Type: " << status << std::endl;
  outstring << "" << std::endl;
  outstring << "Total Trigger Fired: " <<  std::endl;
  outstring << "Trigger 0: " << triggercounter[0] << std::endl;
  outstring << "Trigger 1: " << triggercounter[1] << std::endl;
  outstring << "Trigger 2: " << triggercounter[2] << std::endl;
  outstring << "Trigger 3: " << triggercounter[3] << std::endl;
  outstring << "Trigger 4: " << triggercounter[4] << std::endl;
  outstring << "Trigger 5: " << triggercounter[5] << std::endl;
  outstring << "Trigger 6: " << triggercounter[6] << std::endl;
  outstring << "Trigger 7: " << triggercounter[7] << std::endl;
  outstring << "Trigger 8: " << triggercounter[8] << std::endl;
  outstring << "Trigger 9: " << triggercounter[9] << std::endl;
  outstring << "Trigger 10: " << triggercounter[10] << std::endl;
  outstring << "Trigger 11: " << triggercounter[11] << std::endl;
  outstring << "Trigger 12: " << triggercounter[12] << std::endl;
  outstring << "Trigger 13: " << triggercounter[13] << std::endl;
  outstring << "Trigger 14: " << triggercounter[14] << std::endl;
  outstring << "Trigger 15: " << triggercounter[15] << std::endl;
  outstring << "Trigger 16: " << triggercounter[16] << std::endl;
  outstring << "Trigger 17: " << triggercounter[17] << std::endl;
  outstring << "Trigger 18: " << triggercounter[18] << std::endl;
  outstring << "Trigger 19: " << triggercounter[19] << std::endl;
  outstring << "" << std::endl;

  SaveHistos();
  outf->Close();

}

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
int main(int argc, char **argv)
{

//
// Main Code
//

  AutoLibraryLoader::enable();

  const char *s1="*";
  std::string filein_;
  std::string savehistofile_;
  std::string processname_;
  std::string type_;

// Parsing arguments

  if (argc > 1 && strcmp(s1,argv[1]) != 0)  filein_ = argv[1];
  if (argc > 2 && strcmp(s1,argv[2]) != 0)  savehistofile_  = argv[2];
  if (argc > 3 && strcmp(s1,argv[3]) != 0)  processname_  = argv[3];
  if (argc > 4 && strcmp(s1,argv[4]) != 0)  type_  = argv[4];

  TemplateExample* example = new TemplateExample();   

//CreateHisto receiving type_ argument
  example->CreateHistos(type_);

//Run receiving filein_, savehistofile_, processname_ and type_ argument
  example->Run(filein_, savehistofile_, processname_, type_);

  return 0;
}
#endif
