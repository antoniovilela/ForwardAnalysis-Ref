//-------------------------------------------------------------------------------------------------------->>
// UNIVERSIDADE DO ESTADO DO RIO DE JANEIRO - CMS/Brasil
//-------------------------------------------------------------------------------------------------------->>
// Project: Exclusive Dijets Analysis
//-------------------------------------------------------------------------------------------------------->>
//
// Twiki: https://twiki.cern.ch/twiki/bin/view/CMS/FwdPhysicsExclusiveDijetsAnalysis
//
//

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

#include "TriggerEff.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfoEvent.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

using namespace diffractiveAnalysis;
using namespace exclusiveDijetsAnalysis;
using namespace eventInfo;
using namespace reweight;

void TriggerEff::LoadFile(std::string fileinput, std::string processinput){

  inf = NULL;
  tr  = NULL;
  inf = TFile::Open(fileinput.c_str(),"read");
  tr = (TTree*)inf->Get(processinput.c_str());
  eventdiff = new DiffractiveEvent();
  eventexcl = new ExclusiveDijetsEvent();
  eventinfo = new EventInfoEvent();
  diff = tr->GetBranch("DiffractiveAnalysis");
  excl = tr->GetBranch("ExclusiveDijetsAnalysis");
  info = tr->GetBranch("EventInfo");
  diff->SetAddress(&eventdiff);
  excl->SetAddress(&eventexcl);
  info->SetAddress(&eventinfo);

}

void TriggerEff::Run(std::string filein_, std::string savehistofile_, std::string processname_, int optTriggerRef_, int optTrigger_, int bin_){

  filein = filein_;
  savehistofile = savehistofile_;
  processname = processname_;
  filein = filein_;
  optTrigger = optTrigger_;
  optTriggerRef = optTriggerRef_;
  bin = bin_;

  std::cout << "" << std::endl;
  std::cout << "Running..." << std::endl;
  std::cout << "" << std::endl;
  std::cout << "<< INPUTS >>" << std::endl;
  std::cout << " " << std::endl;
  std::cout << "Input file: " << filein << std::endl;
  std::cout << " " << std::cout;
  std::cout << "Output file: " << savehistofile << std::endl;
  std::cout << " " << std::cout; 
  std::cout << "Reference Trigger Option: " << optTriggerRef << std::endl;
  std::cout << "Trigger Option: " << optTrigger << std::endl;
  std::cout << "Bin: " << bin << std::endl;
  std::cout << " " << std::endl;


  TFile check1(filein.c_str());

  if (check1.IsZombie()){

    std::cout << "----------------------------------------------" << std::endl;
    std::cout << " There is no PatTuple file or the"   << std::endl;
    std::cout << " path is not correct." << std::endl;
    std::cout << " Edit the source and recompile." << std::endl;
    std::cout << "----------------------------------------------" << std::endl;
    return;

  }

  LoadFile(filein,processname);

  TFile *outf = new TFile(savehistofile.c_str(),"RECREATE");

  TString outtxt = savehistofile;
  outtxt.ReplaceAll("root","txt");  
  std::ofstream outstring(outtxt); 

  int NEVENTS = tr->GetEntries();

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  unsigned NEntries = tr->GetEntries();
  std::cout << "" << std::endl;
  std::cout<< "Reading Tree: "<< NEntries << " events"<<std::endl;
  std::cout << "" << std::endl;

  int decade = 0;

  int triggercounter[20]={0};
  double TotalE = 0.;
  double counter[10] = {0};
  double deltaphi_ = 0.;

  std::vector <std::string> Folders;
  Folders.push_back("without_cuts");
  Folders.push_back("with_RefTrigger");
  Folders.push_back("with_RefTriggerCutsOffLine_eta4");
  Folders.push_back("with_RefTriggerCutsOffLine_eta3");
  Folders.push_back("with_RefTriggerCutsOffLine_eta2");
  Folders.push_back("with_RefTriggerCutsOffLine_eta1");
  Folders.push_back("with_RefTriggerCutsOffLineAndTrigger_eta4");
  Folders.push_back("with_RefTriggerCutsOffLineAndTrigger_eta3");
  Folders.push_back("with_RefTriggerCutsOffLineAndTrigger_eta2");
  Folders.push_back("with_RefTriggerCutsOffLineAndTrigger_eta1");

  for (int j=0; j<10; j++){

    char name1[300];
    sprintf(name1,"Events_%s",Folders.at(j).c_str());
    TH1D *histo_m_Evt_lumis = new TH1D(name1,"; Lumis; N events",bin,0,2.0);
    m_hVector_Evt_lumis.push_back(histo_m_Evt_lumis);

    char name2[300];
    sprintf(name2,"Eff_%s",Folders.at(j).c_str());
    TH1D *histo_m_Eff_lumis = new TH1D(name2,"; Lumis; Efficiency",bin,0,2.0);
    m_hVector_Eff_lumis.push_back(histo_m_Eff_lumis);

    char name3[300];
    sprintf(name3,"Events_PFEtamax_%s",Folders.at(j).c_str());
    TH1D *histo_m_Evt_PFEtamax = new TH1D(name3,";Particle Flow #eta_{max}; N events",bin,0,5.5);
    m_hVector_Evt_pfetamax.push_back(histo_m_Evt_PFEtamax);

    char name4[300];
    sprintf(name4,"Events_PFEtamin_%s",Folders.at(j).c_str());
    TH1D *histo_m_Evt_PFEtamin = new TH1D(name4,";Particle Flow #eta_{min}; N events",bin,-5.5,0);
    m_hVector_Evt_pfetamin.push_back(histo_m_Evt_PFEtamin);

  }

  for(int i=0;i<NEVENTS;i++) {

    bool gap = false;
    double etacut;

    ++TotalE;

    double progress = 10.0*i/(1.0*NEVENTS);
    int l = TMath::FloorNint(progress); 

    if (l > decade){
      std::cout <<"\n<<<<<< STATUS >>>>>>" << std::endl; 
      std::cout<<10*l<<" % completed." << std::endl;
      for(int k=0; k < 20; k++){
	std::cout <<"Trigger List Fired: "<< eventexcl->GetHLTPath(k) << std::endl; 
      }
      std::cout <<"<<<<<<<<<<>>>>>>>>>>\n" << std::endl;
    }


    for (int nt=0;nt<20;nt++){
      if(eventexcl->GetHLTPath(nt)){
	triggercounter[nt]++;
      }
    }


    decade = l;          

    tr->GetEntry(i);

    if( i % 1000 == 0 ){
      std::cout << "\nEvent " << i << std::endl;
    }

    deltaphi_ = fabs(eventexcl->GetLeadingJetPhi()-eventexcl->GetSecondJetPhi());

    counter[0]++;

    m_hVector_Evt_lumis.at(0)->Fill(eventinfo->GetInstLumiBunch());
    m_hVector_Eff_lumis.at(0)->Fill(eventinfo->GetInstLumiBunch());
    m_hVector_Evt_pfetamax.at(0)->Fill(eventdiff->GetEtaMaxFromPFCands());
    m_hVector_Evt_pfetamin.at(0)->Fill(eventdiff->GetEtaMinFromPFCands());



    if(eventexcl->GetHLTPath(optTriggerRef)){

      counter[1]++;
      m_hVector_Evt_lumis.at(1)->Fill(eventinfo->GetInstLumiBunch());
      m_hVector_Eff_lumis.at(1)->Fill(eventinfo->GetInstLumiBunch());
      m_hVector_Evt_pfetamax.at(1)->Fill(eventdiff->GetEtaMaxFromPFCands());
      m_hVector_Evt_pfetamin.at(1)->Fill(eventdiff->GetEtaMinFromPFCands());

      for (int i=0; i < 4; i++ ){

	if(i==0) etacut = 4.;
	if(i==1) etacut = 3.;
	if(i==2) etacut = 2.;
	if(i==3) etacut = 1.;

	if(eventdiff->GetEtaMinFromPFCands() < -990. && eventdiff->GetEtaMaxFromPFCands() < -990.) gap = true;
	if((eventexcl->GetLeadingJetPt() > 50. && eventexcl->GetSecondJetPt() > 30. ) && (eventexcl->GetLeadingJetPt() < 400. && eventexcl->GetSecondJetPt() < 400.)){
	  if(deltaphi_>M_PI) deltaphi_=2.0*M_PI-deltaphi_;
	  if(deltaphi_>0.5*M_PI) {
	    if(eventdiff->GetSumEnergyHFPlus() < 30 && eventdiff->GetSumEnergyHFMinus() < 30){
	      if(eventexcl->GetNVertex() > 0 && eventexcl->GetNVertex()<= 1){         
		if( (eventdiff->GetEtaMinFromPFCands() > -etacut && eventdiff->GetEtaMaxFromPFCands() < etacut ) || (gap) ){

		  counter[i+2]++;
		  m_hVector_Evt_lumis.at(i+2)->Fill(eventinfo->GetInstLumiBunch());
		  m_hVector_Eff_lumis.at(i+2)->Fill(eventinfo->GetInstLumiBunch());
		  m_hVector_Evt_pfetamax.at(i+2)->Fill(eventdiff->GetEtaMaxFromPFCands());
		  m_hVector_Evt_pfetamin.at(i+2)->Fill(eventdiff->GetEtaMinFromPFCands());

		  if(eventexcl->GetHLTPath(optTrigger)){
		    counter[i+6]++;
		    m_hVector_Evt_lumis.at(i+6)->Fill(eventinfo->GetInstLumiBunch());
		    m_hVector_Eff_lumis.at(i+6)->Fill(eventinfo->GetInstLumiBunch());
		    m_hVector_Evt_pfetamax.at(i+6)->Fill(eventdiff->GetEtaMaxFromPFCands());
		    m_hVector_Evt_pfetamin.at(i+6)->Fill(eventdiff->GetEtaMinFromPFCands());
		  } 	
		}
	      } 
	    }
	  }
	}  
      }
    }
  }

  //Scalling Plots
  for (int k=0; k<10; k++){
    m_hVector_Eff_lumis.at(k)->Scale(1./counter[k]);
  }

  outstring << "" << std::endl;
  outstring << "<< INPUTS >>" << std::endl;
  outstring << " " << std::endl;
  outstring << "Input file: " << filein << std::endl;
  outstring << "Output file: " << savehistofile << std::endl;
  outstring << " " << std::endl;
  outstring << "Trigger Ref Option: " << optTriggerRef << std::endl;
  outstring << "Trigger Option: " << optTrigger << std::endl;
  outstring << "Bin: " << bin << std::endl;
  outstring << " " << std::endl;
  outstring << "Number of Events: " << TotalE << std::endl;
  outstring << "Number of Events Without Cuts: " << counter[0] << std::endl;
  outstring << "Number of Events With Ref. Trigger: " << counter[1] << std::endl;
  outstring << "Number of Events With Cut Off Line Eta4 : " << counter[2] << std::endl;
  outstring << "Number of Events With Cut Off Line Eta3 : " << counter[3] << std::endl;
  outstring << "Number of Events With Cut Off Line Eta2 : " << counter[4] << std::endl;
  outstring << "Number of Events With Cut Off Line Eta1 : " << counter[5] << std::endl;
  outstring << "Number of Events With Trigger and Eta4 : " << counter[6] << std::endl;
  outstring << "Number of Events With Trigger and Eta3 : " << counter[7] << std::endl;
  outstring << "Number of Events With Trigger and Eta2 : " << counter[8] << std::endl;
  outstring << "Number of Events With Trigger and Eta1 : " << counter[9] << std::endl;
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


  outf->Write();
  outf->Close();
  outstring.close();

}

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
int main(int argc, char **argv)
{
  AutoLibraryLoader::enable();

  const char *s1="*";
  std::string filein_;
  std::string savehistofile_;
  std::string processname_;
  int optTrigger_;
  int optTriggerRef_;
  int bin_;

  if (argc > 1 && strcmp(s1,argv[1]) != 0)  filein_ = argv[1];
  if (argc > 2 && strcmp(s1,argv[2]) != 0)  savehistofile_  = argv[2];
  if (argc > 3 && strcmp(s1,argv[3]) != 0)  processname_  = argv[3];
  if (argc > 4 && strcmp(s1,argv[4]) != 0)  optTriggerRef_   = atoi(argv[4]);
  if (argc > 5 && strcmp(s1,argv[5]) != 0)  optTrigger_   = atoi(argv[5]);
  if (argc > 6 && strcmp(s1,argv[6]) != 0)  bin_   = atoi(argv[6]);

  TriggerEff* triggereff = new TriggerEff();   
  triggereff->Run(filein_, savehistofile_, processname_, optTriggerRef_, optTrigger_, bin_);

  return 0;
}
#endif
