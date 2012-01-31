//-------------------------------------------------------------------------------------------------------->>
// UNIVERSIDADE DO ESTADO DO RIO DE JANEIRO - CMS/Brasil
//-------------------------------------------------------------------------------------------------------->>
// Project: Exclusive Dijets Analysis
// Version: v1
//-------------------------------------------------------------------------------------------------------->>
//
// Instructions to run the code: (a) using script ro run multiple parameters or (b) bash command line. 
//
// First Step: compile the code.
//             ?> scram b -r
//
// Second Step: copy from $CMSSW_BASE/test/slc.../Binary file to your own area with pileup root files.
//
// (A) SCRIPT TO RUN MULTIPLE PARAMETERS
// -------------------------------------
// $> python RunExclAnalysis.py
//
//
// (B) COMMAND LINE
// ----------------
// $> ./ExclDijetsComp "Inputfile.root" "outputfile.root" <pT(Jet1) Cut> <pT(Jet2) Cut> <Number of Vertex Cut> <Trigger Option> <Turn on(off) PU Reweight> <Turn on(off) Luminosity Reweight> <Turn on(off) Trigger Efficiency> <Turn on(off) event-per-event Weight> <Luminosity Weight Factor> <Trigger Efficiency Factor>
//
// TURN ON  = 1
// TURN OFF = 0
//
// I)   If you turn off PU reweight, the default weight will be 1;
// II)  If you turn off Luminosity reweight, the default weight will be 1;
// III) If you turn off Trigger Efficiency, the default weight will be 1;
// IV)  If you turn off event-per-event weight (some MC sample), the default weight will be 1;
//
// EXAMPLE: ./ExclDijetsComp "inputfile.root" "outputfile.root" 60 55 2 1 0 1 1 0.0003 2.3
//
//       DO NOT FORGET to set line 512 for choose SIMULATED TRIGGER OR TRIGGER!
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
#include <vector>
#include <fstream>

#include "ExclDijetsComp.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/ExclusiveDijetsEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "KKousour/QCDAnalysis/interface/QCDEvent.h"

//#include "LumiReweightingStandAlone.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

//using namespace forwardAnalysis;
using namespace diffractiveAnalysis;
using namespace exclusiveDijetsAnalysis;
using namespace reweight;

void ExclDijetsComp::LoadFile(std::string fileinput){

   inf = NULL;
   tr  = NULL;
   inf = TFile::Open(fileinput.c_str(),"read");
   tr = (TTree*)inf->Get("forwardQCDTTreeAnalysis/ProcessedTree");
   eventdiff = new DiffractiveEvent();
   eventexcl = new ExclusiveDijetsEvent();
   eventqcd = new QCDEvent();
   diff = tr->GetBranch("DiffractiveAnalysis");
   excl = tr->GetBranch("ExclusiveDijetsAnalysis");
   qcd = tr->GetBranch("QCDAnalysis");
   diff->SetAddress(&eventdiff);
   excl->SetAddress(&eventexcl);
   qcd->SetAddress(&eventqcd);

}

void ExclDijetsComp::Run(std::string filein_, std::string savehistofile_, double jet1PT_, double jet2PT_, int optnVertex_, int optTrigger_, bool switchWeightPU_, bool switchWeightLumi_, bool switchWeightEff_, bool switchWeightePw_, double weightlumipass_, double triggereffpass_){

   filein = filein_;
   savehistofile = savehistofile_;
   filein = filein_;
   jet1PT = jet1PT_;
   jet2PT = jet2PT_;
   optnVertex = optnVertex_;
   optTrigger = optTrigger_;
   switchWeightPU = switchWeightPU_;
   switchWeightLumi = switchWeightLumi_;
   switchWeightEff = switchWeightEff_;
   switchWeightePw = switchWeightePw_;
   weightlumipass = weightlumipass_;
   triggereffpass = triggereffpass_;

   std::cout << "" << std::endl;
   std::cout << "Running..." << std::endl;
   std::cout << "" << std::endl;
   std::cout << "<< INPUTS >>" << std::endl;
   std::cout << " " << std::endl;
   std::cout << "Input file: " << filein << std::endl;
   std::cout << " " << std::cout;
   std::cout << "Output file: " << savehistofile << std::endl;
   std::cout << " " << std::cout; 
   std::cout << "pT(Jet1): " << jet1PT << "GeV" << std::endl;
   std::cout << "pT(Jet2): " << jet2PT << "GeV" << std::endl;
   std::cout << "# Vertex: " << optnVertex << std::endl;
   std::cout << "Trigger Option: " << optTrigger << std::endl;
   std::cout << " " << std::endl;
   std::cout << "--> TRUE = 1 FALSE = 0" << std::endl;
   std::cout << "PU Weight: " << switchWeightPU << std::endl;
   std::cout << "Lumi. Weight: " << switchWeightLumi << std::endl;
   std::cout << "Eff. Corr.: " << switchWeightEff << std::endl;
   std::cout << "Evt. - Evt. Weight: " << switchWeightePw << std::endl;
   std::cout << " " << std::endl;
   std::cout << "--> Factors" << std::endl;
   std::cout << "Lumi. Weight: " << weightlumipass << std::endl;
   std::cout << "Trigger Eff.: " << triggereffpass << std::endl;
   std::cout << "" << std::endl;

   LoadFile(filein);  

   edm::LumiReWeighting LumiWeights_("pileup15to3000_BXs_mc.root","147146-149711-pileup_2.root ","pileupmcBx0","pileup");

   if (optnVertex == 0){

      std::cout << "---------------------------------------------------------------" << std::endl;
      std::cout << "Please, restart your setup. Respect the Condition # Vertex > 0)" << std::endl;
      std::cout << "---------------------------------------------------------------" << std::endl;
      return;

   }

   std::cout << " " << std::endl;
   std::cout << "pT(jet1) > " << jet1PT << std::endl;
   std::cout << "pT(jet2) > " << jet2PT << std::endl;
   std::cout << " " << std::endl;

   // Root file with histograms
   TFile *outf = new TFile(savehistofile.c_str(),"RECREATE");
   
   // File with Number of Events
   TString outtxt = savehistofile;
   outtxt.ReplaceAll("root","txt");  
   std::ofstream outstring(outtxt); 
   
   //int NEVENTS = tr->GetEntriesFast();
   int NEVENTS = tr->GetEntries();

   TH1::SetDefaultSumw2(true);
   TH2::SetDefaultSumw2(true);

   // Control Plots A: without cuts
   TH1D *h_DeltaEtaGenwc = new TH1D("DeltaEtaGen_without_cuts","#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
   TH1D *h_MxGenRangewc = new TH1D("MxGenRange_without_cuts","Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
   TH1D *h_RJJwc = new TH1D("RJJ_without_cuts","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFwc = new TH2F( "mHF_without_cuts", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_Caloswc = new TH2F( "ETCalos_without_cuts", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_trackswc = new TH1D("tracks_without_cuts","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_pfetamaxwc = new TH1D("pfetamax_without_cuts","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminwc = new TH1D("pfetamin_without_cuts","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_aSumEwc = new TH1D("aEnergy_without_cuts","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetswc = new TH1D("deltaEtaJets_without_cuts","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetswc = new TH1D("deltaPhiJets_without_cuts","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetswc = new TH1D("deltaPtJets_without_cuts","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMasswc = new TH1D("dijetMass_without_cuts","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1wc = new TH1D("pTJet1_without_cuts","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2wc = new TH1D("pTJet2_without_cuts","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1wc = new TH1D("etaJet1_without_cuts","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2wc = new TH1D("etaJet2_without_cuts","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1wc = new TH1D("phiJet1_without_cuts","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2wc = new TH1D("phiJet2_without_cuts","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFwc = new TH1D("deltaEtaPF_without_cuts","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFwc = new TH1D("absdeltaEtaPF_without_cuts","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0.0,10);
   TH1D *h_puBx0wc = new TH1D("pileupmcBx0_without_cuts","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1wc = new TH1D("pileupmcBxm1_without_cuts","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1wc = new TH1D("pileupmcBxp1_without_cuts","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexwc = new TH1D("vertex_without_cuts","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFpluswc = new TH1D("sumEHFplus_without_cuts","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminuswc = new TH1D("sumEHFminus_without_cuts","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEpluswc = new TH1D("sumEHEplus_without_cuts","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminuswc = new TH1D("sumEHEminus_without_cuts","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfpluswc = new TH1D("sumEHFpfplus_without_cuts","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminuswc = new TH1D("sumEHFpfminus_without_cuts","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);

   //------------>

   // ControlPlots B: with trigger
   TH1D *h_DeltaEtaGenwt = new TH1D("DeltaEtaGen_with_trigger","#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
   TH1D *h_MxGenRangewt = new TH1D("MxGenRange_with_trigger","Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
   TH1D *h_RJJwt = new TH1D("RJJ_with_trigger","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFwt = new TH2F( "mHF_with_trigger", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_Caloswt = new TH2F( "ETCalos_with_trigger", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_trackswt = new TH1D("tracks_with_trigger","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_pfetamaxwt = new TH1D("pfetamax_with_trigger","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminwt = new TH1D("pfetamin_with_trigger","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_aSumEwt = new TH1D("aEnergy_with_trigger","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetswt = new TH1D("deltaEtaJets_with_trigger","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetswt = new TH1D("deltaPhiJets_with_trigger","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetswt = new TH1D("deltaPtJets_with_trigger","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMasswt = new TH1D("dijetMass_with_trigger","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1wt = new TH1D("pTJet1_with_trigger","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2wt = new TH1D("pTJet2_with_trigger","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1wt = new TH1D("etaJet1_with_trigger","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2wt = new TH1D("etaJet2_with_trigger","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1wt = new TH1D("phiJet1_with_trigger","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2wt = new TH1D("phiJet2_with_trigger","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFwt = new TH1D("deltaEtaPF_with_trigger","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFwt = new TH1D("absdeltaEtaPF_with_trigger","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_puBx0wt = new TH1D("pileupmcBx0_with_trigger","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1wt = new TH1D("pileupmcBxm1_with_trigger","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1wt = new TH1D("pileupmcBxp1_with_trigger","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexwt = new TH1D("vertex_with_trigger","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFpluswt = new TH1D("sumEHFplus_with_trigger","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminuswt = new TH1D("sumEHFminus_with_trigger","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEpluswt = new TH1D("sumEHEplus_with_trigger","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminuswt = new TH1D("sumEHEminus_with_trigger","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfpluswt = new TH1D("sumEHFpfplus_with_trigger","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminuswt = new TH1D("sumEHFpfminus_with_trigger","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);
   //------------>

   // STEP1: trigger + Vertex Cut
   TH1D *h_DeltaEtaGenstep1 = new TH1D("DeltaEtaGen_step1","#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
   TH1D *h_MxGenRangestep1 = new TH1D("MxGenRange_step1","Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
   TH1D *h_RJJstep1 = new TH1D("RJJ_step1","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFstep1 = new TH2F( "mHF_step1", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_Calosstep1 = new TH2F( "ETCalos_step1", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksstep1 = new TH1D("tracks_step1","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_pfetamaxstep1 = new TH1D("pfetamax_step1","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminstep1 = new TH1D("pfetamin_step1","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_aSumEstep1 = new TH1D("aEnergy_step1","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsstep1 = new TH1D("deltaEtaJets_step1","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsstep1 = new TH1D("deltaPhiJets_step1","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsstep1 = new TH1D("deltaPtJets_step1","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassstep1 = new TH1D("dijetMass_step1","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1step1 = new TH1D("pTJet1_step1","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2step1 = new TH1D("pTJet2_step1","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1step1= new TH1D("etaJet1_step1","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2step1 = new TH1D("etaJet2_step1","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1step1 = new TH1D("phiJet1_step1","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2step1 = new TH1D("phiJet2_step1","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFstep1 = new TH1D("deltaEtaPF_step1","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFstep1 = new TH1D("absdeltaEtaPF_step1","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_puBx0step1 = new TH1D("pileupmcBx0_step1","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1step1 = new TH1D("pileupmcBxm1_step1","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1step1 = new TH1D("pileupmcBxp1_step1","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexstep1 = new TH1D("vertex_step1","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusstep1 = new TH1D("sumEHFplus_step1","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusstep1 = new TH1D("sumEHFminus_step1","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusstep1 = new TH1D("sumEHEplus_step1","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusstep1 = new TH1D("sumEHEminus_step1","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusstep1 = new TH1D("sumEHFpfplus_step1","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusstep1 = new TH1D("sumEHFpfminus_step1","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);
   //------------>

   // STEP2: trigger + Vertex Cut + pT (Jet1 and Jet2) Cut 
   TH1D *h_DeltaEtaGenstep2 = new TH1D("DeltaEtaGen_step2","#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
   TH1D *h_MxGenRangestep2 = new TH1D("MxGenRange_step2","Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
   TH1D *h_RJJstep2 = new TH1D("RJJ_step2","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFstep2 = new TH2F( "mHF_step2", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_Calosstep2 = new TH2F( "ETCalos_step2", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksstep2 = new TH1D("tracks_step2","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_pfetamaxstep2 = new TH1D("pfetamax_step2","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminstep2 = new TH1D("pfetamin_step2","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_aSumEstep2 = new TH1D("aEnergy_step2","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsstep2 = new TH1D("deltaEtaJets_step2","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsstep2 = new TH1D("deltaPhiJets_step2","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsstep2 = new TH1D("deltaPtJets_step2","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassstep2 = new TH1D("dijetMass_step2","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1step2 = new TH1D("pTJet1_step2","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2step2 = new TH1D("pTJet2_step2","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1step2= new TH1D("etaJet1_step2","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2step2 = new TH1D("etaJet2_step2","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1step2 = new TH1D("phiJet1_step2","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2step2 = new TH1D("phiJet2_step2","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFstep2 = new TH1D("deltaEtaPF_step2","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFstep2 = new TH1D("absdeltaEtaPF_step2","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_puBx0step2 = new TH1D("pileupmcBx0_step2","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1step2 = new TH1D("pileupmcBxm1_step2","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1step2 = new TH1D("pileupmcBxp1_step2","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexstep2 = new TH1D("vertex_step2","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusstep2 = new TH1D("sumEHFplus_step2","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusstep2 = new TH1D("sumEHFminus_step2","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusstep2 = new TH1D("sumEHEplus_step2","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusstep2 = new TH1D("sumEHEminus_step2","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusstep2 = new TH1D("sumEHFpfplus_step2","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusstep2 = new TH1D("sumEHFpfminus_step2","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);
   //------------>

   // ALL STEP3: trigger + Vertex Cut + pT (Jet1 and Jet2) Cut + Jets All CMS Acceptance 
   TH1D *h_DeltaEtaGenAllstep3 = new TH1D("DeltaEtaGenAll_step3","#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
   TH1D *h_MxGenRangeAllstep3 = new TH1D("MxGenRangeAll_step3","Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
   TH1D *h_RJJAllstep3 = new TH1D("RJJAll_step3","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFAllstep3 = new TH2F( "mHFAll_step3", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_CalosAllstep3 = new TH2F( "ETCalosAll_step3", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksAllstep3 = new TH1D("tracksAll_step3","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_pfetamaxAllstep3 = new TH1D("pfetamaxAll_step3","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminAllstep3 = new TH1D("pfetaminAll_step3","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_aSumEAllstep3 = new TH1D("aEnergyAll_step3","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsAllstep3 = new TH1D("deltaEtaJetsAll_step3","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsAllstep3 = new TH1D("deltaPhiJetsAll_step3","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsAllstep3 = new TH1D("deltaPtJetsAll_step3","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassAllstep3 = new TH1D("dijetMassAll_step3","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1Allstep3 = new TH1D("pTJet1All_step3","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2Allstep3 = new TH1D("pTJet2All_step3","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1Allstep3 = new TH1D("etaJet1All_step3","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2Allstep3 = new TH1D("etaJet2All_step3","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1Allstep3 = new TH1D("phiJet1All_step3","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2Allstep3 = new TH1D("phiJet2All_step3","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFAllstep3 = new TH1D("deltaEtaPFAll_step3","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFAllstep3 = new TH1D("absdeltaEtaPFAll_step3","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_puBx0Allstep3 = new TH1D("pileupmcBx0All_step3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1Allstep3 = new TH1D("pileupmcBxm1All_step3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1Allstep3 = new TH1D("pileupmcBxp1All_step3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexAllstep3 = new TH1D("vertexAll_step3","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusAllstep3 = new TH1D("sumEHFplusAll_step3","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusAllstep3 = new TH1D("sumEHFminusAll_step3","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusAllstep3 = new TH1D("sumEHEplusAll_step3","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusAllstep3 = new TH1D("sumEHEminusAll_step3","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusAllstep3 = new TH1D("sumEHFpfplusAll_step3","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusAllstep3 = new TH1D("sumEHFpfminusAll_step3","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);
   //------------>

   // ALL STEP4-X: trigger + Vertex Cut + pT (Jet1 and Jet2) Cut + Jets All CMS Acceptance + |Eta Max and Eta Min| < X Cut 
   // |Eta max and Eta min| < 4
   TH1D *h_RJJAllstep4_4 = new TH1D("RJJAll_step4_4","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFAllstep4_4 = new TH2F( "mHFAll_step4_4", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_CalosAllstep4_4 = new TH2F( "ETCalosAll_step4_4", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksAllstep4_4 = new TH1D("tracksAll_step4_4","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_aSumEAllstep4_4 = new TH1D("aEnergyAll_step4_4","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsAllstep4_4 = new TH1D("deltaEtaJetsAll_step4_4","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsAllstep4_4 = new TH1D("deltaPhiJetsAll_step4_4","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsAllstep4_4 = new TH1D("deltaPtJetsAll_step4_4","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassAllstep4_4 = new TH1D("dijetMassAll_step4_4","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1Allstep4_4 = new TH1D("pTJet1All_step4_4","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2Allstep4_4 = new TH1D("pTJet2All_step4_4","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1Allstep4_4 = new TH1D("etaJet1All_step4_4","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2Allstep4_4 = new TH1D("etaJet2All_step4_4","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1Allstep4_4 = new TH1D("phiJet1All_step4_4","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2Allstep4_4 = new TH1D("phiJet2All_step4_4","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFAllstep4_4 = new TH1D("deltaEtaPFAll_step4_4","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFAllstep4_4 = new TH1D("absdeltaEtaPFAll_step4_4","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_pfetamaxAllstep4_4 = new TH1D("pfetamaxAll_step4_4","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminAllstep4_4 = new TH1D("pfetaminAll_step4_4","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_puBx0Allstep4_4 = new TH1D("pileupmcBx0All_step4_4","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1Allstep4_4 = new TH1D("pileupmcBxm1All_step4_4","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1Allstep4_4 = new TH1D("pileupmcBxp1All_step4_4","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexAllstep4_4 = new TH1D("vertexAll_step4_4","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusAllstep4_4 = new TH1D("sumEHFplusAll_step4_4","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusAllstep4_4 = new TH1D("sumEHFminusAll_step4_4","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusAllstep4_4 = new TH1D("sumEHEplusAll_step4_4","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusAllstep4_4 = new TH1D("sumEHEminusAll_step4_4","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusAllstep4_4 = new TH1D("sumEHFpfplusAll_step4_4","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusAllstep4_4 = new TH1D("sumEHFpfminusAll_step4_4","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);

   // |Eta max and Eta min| < 3
   TH1D *h_RJJAllstep4_3 = new TH1D("RJJAll_step4_3","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFAllstep4_3 = new TH2F( "mHFAll_step4_3", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_CalosAllstep4_3 = new TH2F( "ETCalosAll_step4_3", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksAllstep4_3 = new TH1D("tracksAll_step4_3","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_aSumEAllstep4_3 = new TH1D("aEnergyAll_step4_3","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsAllstep4_3 = new TH1D("deltaEtaJetsAll_step4_3","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsAllstep4_3 = new TH1D("deltaPhiJetsAll_step4_3","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsAllstep4_3 = new TH1D("deltaPtJetsAll_step4_3","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassAllstep4_3 = new TH1D("dijetMassAll_step4_3","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1Allstep4_3 = new TH1D("pTJet1All_step4_3","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2Allstep4_3 = new TH1D("pTJet2All_step4_3","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1Allstep4_3 = new TH1D("etaJet1All_step4_3","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2Allstep4_3 = new TH1D("etaJet2All_step4_3","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1Allstep4_3 = new TH1D("phiJet1All_step4_3","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2Allstep4_3 = new TH1D("phiJet2All_step4_3","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFAllstep4_3 = new TH1D("deltaEtaPFAll_step4_3","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFAllstep4_3 = new TH1D("absdeltaEtaPFAll_step4_3","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_pfetamaxAllstep4_3 = new TH1D("pfetamaxAll_step4_3","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminAllstep4_3 = new TH1D("pfetaminAll_step4_3","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_puBx0Allstep4_3 = new TH1D("pileupmcBx0All_step4_3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1Allstep4_3 = new TH1D("pileupmcBxm1All_step4_3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1Allstep4_3 = new TH1D("pileupmcBxp1All_step4_3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexAllstep4_3 = new TH1D("vertexAll_step4_3","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusAllstep4_3 = new TH1D("sumEHFplusAll_step4_3","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusAllstep4_3 = new TH1D("sumEHFminusAll_step4_3","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusAllstep4_3 = new TH1D("sumEHEplusAll_step4_3","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusAllstep4_3 = new TH1D("sumEHEminusAll_step4_3","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusAllstep4_3 = new TH1D("sumEHFpfplusAll_step4_3","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusAllstep4_3 = new TH1D("sumEHFpfminusAll_step4_3","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);

   // |Eta max and Eta min| < 2
   TH1D *h_RJJAllstep4_2 = new TH1D("RJJAll_step4_2","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFAllstep4_2 = new TH2F( "mHFAll_step4_2", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_CalosAllstep4_2 = new TH2F( "ETCalosAll_step4_2", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksAllstep4_2 = new TH1D("tracksAll_step4_2","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_aSumEAllstep4_2 = new TH1D("aEnergyAll_step4_2","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsAllstep4_2 = new TH1D("deltaEtaJetsAll_step4_2","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsAllstep4_2 = new TH1D("deltaPhiJetsAll_step4_2","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsAllstep4_2 = new TH1D("deltaPtJetsAll_step4_2","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassAllstep4_2 = new TH1D("dijetMassAll_step4_2","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1Allstep4_2 = new TH1D("pTJet1All_step4_2","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2Allstep4_2 = new TH1D("pTJet2All_step4_2","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1Allstep4_2 = new TH1D("etaJet1All_step4_2","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2Allstep4_2 = new TH1D("etaJet2All_step4_2","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1Allstep4_2 = new TH1D("phiJet1All_step4_2","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2Allstep4_2 = new TH1D("phiJet2All_step4_2","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFAllstep4_2 = new TH1D("deltaEtaPFAll_step4_2","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFAllstep4_2 = new TH1D("absdeltaEtaPFAll_step4_2","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_pfetamaxAllstep4_2 = new TH1D("pfetamaxAll_step4_2","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminAllstep4_2 = new TH1D("pfetaminAll_step4_2","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_puBx0Allstep4_2 = new TH1D("pileupmcBx0All_step4_2","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1Allstep4_2 = new TH1D("pileupmcBxm1All_step4_2","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1Allstep4_2 = new TH1D("pileupmcBxp1All_step4_2","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexAllstep4_2 = new TH1D("vertexAll_step4_2","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusAllstep4_2 = new TH1D("sumEHFplusAll_step4_2","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusAllstep4_2 = new TH1D("sumEHFminusAll_step4_2","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusAllstep4_2 = new TH1D("sumEHEplusAll_step4_2","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusAllstep4_2 = new TH1D("sumEHEminusAll_step4_2","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusAllstep4_2 = new TH1D("sumEHFpfplusAll_step4_2","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusAllstep4_2 = new TH1D("sumEHFpfminusAll_step4_2","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);

   // |Eta max and Eta min| < 1
   TH1D *h_RJJAllstep4_1 = new TH1D("RJJAll_step4_1","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFAllstep4_1 = new TH2F( "mHFAll_step4_1", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_CalosAllstep4_1 = new TH2F( "ETCalosAll_step4_1", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksAllstep4_1 = new TH1D("tracksAll_step4_1","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_aSumEAllstep4_1 = new TH1D("aEnergyAll_step4_1","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsAllstep4_1 = new TH1D("deltaEtaJetsAll_step4_1","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsAllstep4_1 = new TH1D("deltaPhiJetsAll_step4_1","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsAllstep4_1 = new TH1D("deltaPtJetsAll_step4_1","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassAllstep4_1 = new TH1D("dijetMassAll_step4_1","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1Allstep4_1 = new TH1D("pTJet1All_step4_1","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2Allstep4_1 = new TH1D("pTJet2All_step4_1","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1Allstep4_1 = new TH1D("etaJet1All_step4_1","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2Allstep4_1 = new TH1D("etaJet2All_step4_1","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1Allstep4_1 = new TH1D("phiJet1All_step4_1","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2Allstep4_1 = new TH1D("phiJet2All_step4_1","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFAllstep4_1 = new TH1D("deltaEtaPFAll_step4_1","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFAllstep4_1 = new TH1D("absdeltaEtaPFAll_step4_1","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_pfetamaxAllstep4_1 = new TH1D("pfetamaxAll_step4_1","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminAllstep4_1 = new TH1D("pfetaminAll_step4_1","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_puBx0Allstep4_1 = new TH1D("pileupmcBx0All_step4_1","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1Allstep4_1 = new TH1D("pileupmcBxm1All_step4_1","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1Allstep4_1 = new TH1D("pileupmcBxp1All_step4_1","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexAllstep4_1 = new TH1D("vertexAll_step4_1","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusAllstep4_1 = new TH1D("sumEHFplusAll_step4_1","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusAllstep4_1 = new TH1D("sumEHFminusAll_step4_1","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusAllstep4_1 = new TH1D("sumEHEplusAll_step4_1","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusAllstep4_1 = new TH1D("sumEHEminusAll_step4_1","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusAllstep4_1 = new TH1D("sumEHFpfplusAll_step4_1","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusAllstep4_1 = new TH1D("sumEHFpfminusAll_step4_1","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);
   //------------>

   // Tracker STEP3: trigger + Vertex Cut + pT (Jet1 and Jet2) Cut + Jets All CMS Acceptance 
   TH1D *h_DeltaEtaGenTrackerstep3 = new TH1D("DeltaEtaGenTracker_step3","#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
   TH1D *h_MxGenRangeTrackerstep3 = new TH1D("MxGenRangeTracker_step3","Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
   TH1D *h_RJJTrackerstep3 = new TH1D("RJJTracker_step3","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFTrackerstep3 = new TH2F( "mHFTracker_step3", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_CalosTrackerstep3 = new TH2F( "ETCalosTracker_step3", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksTrackerstep3 = new TH1D("tracksTracker_step3","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_pfetamaxTrackerstep3 = new TH1D("pfetamaxTracker_step3","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminTrackerstep3 = new TH1D("pfetaminTracker_step3","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_aSumETrackerstep3 = new TH1D("aEnergyTracker_step3","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsTrackerstep3 = new TH1D("deltaEtaJetsTracker_step3","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsTrackerstep3 = new TH1D("deltaPhiJetsTracker_step3","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsTrackerstep3 = new TH1D("deltaPtJetsTracker_step3","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassTrackerstep3 = new TH1D("dijetMassTracker_step3","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1Trackerstep3 = new TH1D("pTJet1Tracker_step3","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2Trackerstep3 = new TH1D("pTJet2Tracker_step3","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_deltaEtaPFTrackerstep3 = new TH1D("deltaEtaPFTracker_step3","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFTrackerstep3 = new TH1D("absdeltaEtaPFTracker_step3","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_etaJet1Trackerstep3 = new TH1D("etaJet1Tracker_step3","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2Trackerstep3 = new TH1D("etaJet2Tracker_step3","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1Trackerstep3 = new TH1D("phiJet1Tracker_step3","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2Trackerstep3 = new TH1D("phiJet2Tracker_step3","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_puBx0Trackerstep3 = new TH1D("pileupmcBx0Tracker_step3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1Trackerstep3 = new TH1D("pileupmcBxm1Tracker_step3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1Trackerstep3 = new TH1D("pileupmcBxp1Tracker_step3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexTrackerstep3 = new TH1D("vertexTracker_step3","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusTrackerstep3 = new TH1D("sumEHFplusTracker_step3","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusTrackerstep3 = new TH1D("sumEHFminusTracker_step3","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusTrackerstep3 = new TH1D("sumEHEplusTracker_step3","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusTrackerstep3 = new TH1D("sumEHEminusTracker_step3","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusTrackerstep3 = new TH1D("sumEHFpfplusTracker_step3","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusTrackerstep3 = new TH1D("sumEHFpfminusTracker_step3","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);
   //------------>

   // Tracker STEP4-X: trigger + Vertex Cut + pT (Jet1 and Jet2) Cut + Jets at CMS Tracker Acceptance + |Eta Max and Eta Min| < X Cut 
   // |Eta max and Eta min| < 4
   TH1D *h_RJJTrackerstep4_4 = new TH1D("RJJTracker_step4_4","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFTrackerstep4_4 = new TH2F( "mHFTracker_step4_4", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_CalosTrackerstep4_4 = new TH2F( "ETCalosTracker_step4_4", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksTrackerstep4_4 = new TH1D("tracksTracker_step4_4","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_aSumETrackerstep4_4 = new TH1D("aEnergyTracker_step4_4","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsTrackerstep4_4 = new TH1D("deltaEtaJetsTracker_step4_4","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsTrackerstep4_4 = new TH1D("deltaPhiJetsTracker_step4_4","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsTrackerstep4_4 = new TH1D("deltaPtJetsTracker_step4_4","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassTrackerstep4_4 = new TH1D("dijetMassTracker_step4_4","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1Trackerstep4_4 = new TH1D("pTJet1Tracker_step4_4","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2Trackerstep4_4 = new TH1D("pTJet2Tracker_step4_4","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1Trackerstep4_4 = new TH1D("etaJet1Tracker_step4_4","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2Trackerstep4_4 = new TH1D("etaJet2Tracker_step4_4","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1Trackerstep4_4 = new TH1D("phiJet1Tracker_step4_4","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2Trackerstep4_4 = new TH1D("phiJet2Tracker_step4_4","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFTrackerstep4_4 = new TH1D("deltaEtaPFTracker_step4_4","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFTrackerstep4_4 = new TH1D("absdeltaEtaPFTracker_step4_4","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_pfetamaxTrackerstep4_4 = new TH1D("pfetamaxTracker_step4_4","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminTrackerstep4_4 = new TH1D("pfetaminTracker_step4_4","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_puBx0Trackerstep4_4 = new TH1D("pileupmcBx0Tracker_step4_4","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1Trackerstep4_4 = new TH1D("pileupmcBxm1Tracker_step4_4","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1Trackerstep4_4 = new TH1D("pileupmcBxp1Tracker_step4_4","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexTrackerstep4_4 = new TH1D("vertexTracker_step4_4","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusTrackerstep4_4 = new TH1D("sumEHFplusTracker_step4_4","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusTrackerstep4_4 = new TH1D("sumEHFminusTracker_step4_4","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusTrackerstep4_4 = new TH1D("sumEHEplusTracker_step4_4","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusTrackerstep4_4 = new TH1D("sumEHEminusTracker_step4_4","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusTrackerstep4_4 = new TH1D("sumEHFpfplusTracker_step4_4","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusTrackerstep4_4 = new TH1D("sumEHFpfminusTracker_step4_4","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);

   // |Eta max and Eta min| < 3
   TH1D *h_RJJTrackerstep4_3 = new TH1D("RJJTracker_step4_3","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFTrackerstep4_3 = new TH2F( "mHFTracker_step4_3", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_CalosTrackerstep4_3 = new TH2F( "ETCalosTracker_step4_3", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksTrackerstep4_3 = new TH1D("tracksTracker_step4_3","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_aSumETrackerstep4_3 = new TH1D("aEnergyTracker_step4_3","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsTrackerstep4_3 = new TH1D("deltaEtaJetsTracker_step4_3","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsTrackerstep4_3 = new TH1D("deltaPhiJetsTracker_step4_3","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsTrackerstep4_3 = new TH1D("deltaPtJetsTracker_step4_3","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassTrackerstep4_3 = new TH1D("dijetMassTracker_step4_3","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1Trackerstep4_3 = new TH1D("pTJet1Tracker_step4_3","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2Trackerstep4_3 = new TH1D("pTJet2Tracker_step4_3","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1Trackerstep4_3 = new TH1D("etaJet1Tracker_step4_3","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2Trackerstep4_3 = new TH1D("etaJet2Tracker_step4_3","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1Trackerstep4_3 = new TH1D("phiJet1Tracker_step4_3","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2Trackerstep4_3 = new TH1D("phiJet2Tracker_step4_3","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFTrackerstep4_3 = new TH1D("deltaEtaPFTracker_step4_3","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFTrackerstep4_3 = new TH1D("absdeltaEtaPFTracker_step4_3","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_pfetamaxTrackerstep4_3 = new TH1D("pfetamaxTracker_step4_3","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminTrackerstep4_3 = new TH1D("pfetaminTracker_step4_3","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_puBx0Trackerstep4_3 = new TH1D("pileupmcBx0Tracker_step4_3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1Trackerstep4_3 = new TH1D("pileupmcBxm1Tracker_step4_3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1Trackerstep4_3 = new TH1D("pileupmcBxp1Tracker_step4_3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexTrackerstep4_3 = new TH1D("vertexTracker_step4_3","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusTrackerstep4_3 = new TH1D("sumEHFplusTracker_step4_3","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusTrackerstep4_3 = new TH1D("sumEHFminusTracker_step4_3","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusTrackerstep4_3 = new TH1D("sumEHEplusTracker_step4_3","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusTrackerstep4_3 = new TH1D("sumEHEminusTracker_step4_3","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusTrackerstep4_3 = new TH1D("sumEHFpfplusTracker_step4_3","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusTrackerstep4_3 = new TH1D("sumEHFpfminusTracker_step4_3","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);

   // |Eta max and Eta min| < 2
   TH1D *h_RJJTrackerstep4_2 = new TH1D("RJJTracker_step4_2","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFTrackerstep4_2 = new TH2F( "mHFTracker_step4_2", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_CalosTrackerstep4_2 = new TH2F( "ETCalosTracker_step4_2", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksTrackerstep4_2 = new TH1D("tracksTracker_step4_2","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_aSumETrackerstep4_2 = new TH1D("aEnergyTracker_step4_2","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsTrackerstep4_2 = new TH1D("deltaEtaJetsTracker_step4_2","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsTrackerstep4_2 = new TH1D("deltaPhiJetsTracker_step4_2","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsTrackerstep4_2 = new TH1D("deltaPtJetsTracker_step4_2","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassTrackerstep4_2 = new TH1D("dijetMassTracker_step4_2","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1Trackerstep4_2 = new TH1D("pTJet1Tracker_step4_2","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2Trackerstep4_2 = new TH1D("pTJet2Tracker_step4_2","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1Trackerstep4_2 = new TH1D("etaJet1Tracker_step4_2","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2Trackerstep4_2 = new TH1D("etaJet2Tracker_step4_2","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1Trackerstep4_2 = new TH1D("phiJet1Tracker_step4_2","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2Trackerstep4_2 = new TH1D("phiJet2Tracker_step4_2","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFTrackerstep4_2 = new TH1D("deltaEtaPFTracker_step4_2","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFTrackerstep4_2 = new TH1D("absdeltaEtaPFTracker_step4_2","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_pfetamaxTrackerstep4_2 = new TH1D("pfetamaxTracker_step4_2","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminTrackerstep4_2 = new TH1D("pfetaminTracker_step4_2","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_puBx0Trackerstep4_2 = new TH1D("pileupmcBx0Tracker_step4_2","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1Trackerstep4_2 = new TH1D("pileupmcBxm1Tracker_step4_2","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1Trackerstep4_2 = new TH1D("pileupmcBxp1Tracker_step4_2","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexTrackerstep4_2 = new TH1D("vertexTracker_step4_2","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusTrackerstep4_2 = new TH1D("sumEHFplusTracker_step4_2","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusTrackerstep4_2 = new TH1D("sumEHFminusTracker_step4_2","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusTrackerstep4_2 = new TH1D("sumEHEplusTracker_step4_2","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusTrackerstep4_2 = new TH1D("sumEHEminusTracker_step4_2","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusTrackerstep4_2 = new TH1D("sumEHFpfplusTracker_step4_2","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusTrackerstep4_2 = new TH1D("sumEHFpfminusTracker_step4_2","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);

   // |Eta max and Eta min| < 1
   TH1D *h_RJJTrackerstep4_1 = new TH1D("RJJTracker_step4_1","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFTrackerstep4_1 = new TH2F( "mHFTracker_step4_1", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_CalosTrackerstep4_1 = new TH2F( "ETCalosTracker_step4_1", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksTrackerstep4_1 = new TH1D("tracksTracker_step4_1","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_aSumETrackerstep4_1 = new TH1D("aEnergyTracker_step4_1","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsTrackerstep4_1 = new TH1D("deltaEtaJetsTracker_step4_1","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsTrackerstep4_1 = new TH1D("deltaPhiJetsTracker_step4_1","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsTrackerstep4_1 = new TH1D("deltaPtJetsTracker_step4_1","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassTrackerstep4_1 = new TH1D("dijetMassTracker_step4_1","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1Trackerstep4_1 = new TH1D("pTJet1Tracker_step4_1","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2Trackerstep4_1 = new TH1D("pTJet2Tracker_step4_1","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1Trackerstep4_1 = new TH1D("etaJet1Tracker_step4_1","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2Trackerstep4_1 = new TH1D("etaJet2Tracker_step4_1","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1Trackerstep4_1 = new TH1D("phiJet1Tracker_step4_1","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2Trackerstep4_1 = new TH1D("phiJet2Tracker_step4_1","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFTrackerstep4_1 = new TH1D("deltaEtaPFTracker_step4_1","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFTrackerstep4_1 = new TH1D("absdeltaEtaPFTracker_step4_1","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_pfetamaxTrackerstep4_1 = new TH1D("pfetamaxTracker_step4_1","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminTrackerstep4_1 = new TH1D("pfetaminTracker_step4_1","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_puBx0Trackerstep4_1 = new TH1D("pileupmcBx0Tracker_step4_1","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1Trackerstep4_1 = new TH1D("pileupmcBxm1Tracker_step4_1","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1Trackerstep4_1 = new TH1D("pileupmcBxp1Tracker_step4_1","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexTrackerstep4_1 = new TH1D("vertexTracker_step4_1","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusTrackerstep4_1 = new TH1D("sumEHFplusTracker_step4_1","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusTrackerstep4_1 = new TH1D("sumEHFminusTracker_step4_1","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusTrackerstep4_1 = new TH1D("sumEHEplusTracker_step4_1","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusTrackerstep4_1 = new TH1D("sumEHEminusTracker_step4_1","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusTrackerstep4_1 = new TH1D("sumEHFpfplusTracker_step4_1","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusTrackerstep4_1 = new TH1D("sumEHFpfminusTracker_step4_1","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);
   //------------>

 // Jets Eta 2 STEP3: trigger + Vertex Cut + pT (Jet1 and Jet2) Cut + (Eta(jets) < 2 and Eta(jets) > -2) 
   TH1D *h_DeltaEtaGenJetsEta2step3 = new TH1D("DeltaEtaGenJetsEta2_step3","#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
   TH1D *h_MxGenRangeJetsEta2step3 = new TH1D("MxGenRangeJetsEta2_step3","Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
   TH1D *h_RJJJetsEta2step3 = new TH1D("RJJJetsEta2_step3","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFJetsEta2step3 = new TH2F( "mHFJetsEta2_step3", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_CalosJetsEta2step3 = new TH2F( "ETCalosJetsEta2_step3", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksJetsEta2step3 = new TH1D("tracksJetsEta2_step3","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_pfetamaxJetsEta2step3 = new TH1D("pfetamaxJetsEta2_step3","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminJetsEta2step3 = new TH1D("pfetaminJetsEta2_step3","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_aSumEJetsEta2step3 = new TH1D("aEnergyJetsEta2_step3","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsJetsEta2step3 = new TH1D("deltaEtaJetsJetsEta2_step3","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsJetsEta2step3 = new TH1D("deltaPhiJetsJetsEta2_step3","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsJetsEta2step3 = new TH1D("deltaPtJetsJetsEta2_step3","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassJetsEta2step3 = new TH1D("dijetMassJetsEta2_step3","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1JetsEta2step3 = new TH1D("pTJet1JetsEta2_step3","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2JetsEta2step3 = new TH1D("pTJet2JetsEta2_step3","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_deltaEtaPFJetsEta2step3 = new TH1D("deltaEtaPFJetsEta2_step3","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFJetsEta2step3 = new TH1D("absdeltaEtaPFJetsEta2_step3","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_etaJet1JetsEta2step3 = new TH1D("etaJet1JetsEta2_step3","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2JetsEta2step3 = new TH1D("etaJet2JetsEta2_step3","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1JetsEta2step3 = new TH1D("phiJet1JetsEta2_step3","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2JetsEta2step3 = new TH1D("phiJet2JetsEta2_step3","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_puBx0JetsEta2step3 = new TH1D("pileupmcBx0JetsEta2_step3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1JetsEta2step3 = new TH1D("pileupmcBxm1JetsEta2_step3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1JetsEta2step3 = new TH1D("pileupmcBxp1JetsEta2_step3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexJetsEta2step3 = new TH1D("vertexJetsEta2_step3","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusJetsEta2step3 = new TH1D("sumEHFplusJetsEta2_step3","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusJetsEta2step3 = new TH1D("sumEHFminusJetsEta2_step3","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusJetsEta2step3 = new TH1D("sumEHEplusJetsEta2_step3","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusJetsEta2step3 = new TH1D("sumEHEminusJetsEta2_step3","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusJetsEta2step3 = new TH1D("sumEHFpfplusJetsEta2_step3","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusJetsEta2step3 = new TH1D("sumEHFpfminusJetsEta2_step3","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);

   //------------>

   // Jets Eta 2 STEP4-X: trigger + Vertex Cut + pT (Jet1 and Jet2) Cut + Jets at CMS Jets Eta 2 Acceptance + |Eta Max and Eta Min| < X Cut 
   // |Eta max and Eta min| < 4
   TH1D *h_RJJJetsEta2step4_4 = new TH1D("RJJJetsEta2_step4_4","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFJetsEta2step4_4 = new TH2F( "mHFJetsEta2_step4_4", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_CalosJetsEta2step4_4 = new TH2F( "ETCalosJetsEta2_step4_4", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksJetsEta2step4_4 = new TH1D("tracksJetsEta2_step4_4","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_aSumEJetsEta2step4_4 = new TH1D("aEnergyJetsEta2_step4_4","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsJetsEta2step4_4 = new TH1D("deltaEtaJetsJetsEta2_step4_4","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsJetsEta2step4_4 = new TH1D("deltaPhiJetsJetsEta2_step4_4","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsJetsEta2step4_4 = new TH1D("deltaPtJetsJetsEta2_step4_4","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassJetsEta2step4_4 = new TH1D("dijetMassJetsEta2_step4_4","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1JetsEta2step4_4 = new TH1D("pTJet1JetsEta2_step4_4","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2JetsEta2step4_4 = new TH1D("pTJet2JetsEta2_step4_4","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1JetsEta2step4_4 = new TH1D("etaJet1JetsEta2_step4_4","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2JetsEta2step4_4 = new TH1D("etaJet2JetsEta2_step4_4","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1JetsEta2step4_4 = new TH1D("phiJet1JetsEta2_step4_4","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2JetsEta2step4_4 = new TH1D("phiJet2JetsEta2_step4_4","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFJetsEta2step4_4 = new TH1D("deltaEtaPFJetsEta2_step4_4","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFJetsEta2step4_4 = new TH1D("absdeltaEtaPFJetsEta2_step4_4","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_pfetamaxJetsEta2step4_4 = new TH1D("pfetamaxJetsEta2_step4_4","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminJetsEta2step4_4 = new TH1D("pfetaminJetsEta2_step4_4","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_puBx0JetsEta2step4_4 = new TH1D("pileupmcBx0JetsEta2_step4_4","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1JetsEta2step4_4 = new TH1D("pileupmcBxm1JetsEta2_step4_4","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1JetsEta2step4_4 = new TH1D("pileupmcBxp1JetsEta2_step4_4","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexJetsEta2step4_4 = new TH1D("vertexJetsEta2_step4_4","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusJetsEta2step4_4 = new TH1D("sumEHFplusJetsEta2_step4_4","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusJetsEta2step4_4 = new TH1D("sumEHFminusJetsEta2_step4_4","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusJetsEta2step4_4 = new TH1D("sumEHEplusJetsEta2_step4_4","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusJetsEta2step4_4 = new TH1D("sumEHEminusJetsEta2_step4_4","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusJetsEta2step4_4 = new TH1D("sumEHFpfplusJetsEta2_step4_4","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusJetsEta2step4_4 = new TH1D("sumEHFpfminusJetsEta2_step4_4","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);

   // |Eta max and Eta min| < 3
   TH1D *h_RJJJetsEta2step4_3 = new TH1D("RJJJetsEta2_step4_3","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFJetsEta2step4_3 = new TH2F( "mHFJetsEta2_step4_3", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_CalosJetsEta2step4_3 = new TH2F( "ETCalosJetsEta2_step4_3", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksJetsEta2step4_3 = new TH1D("tracksJetsEta2_step4_3","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_aSumEJetsEta2step4_3 = new TH1D("aEnergyJetsEta2_step4_3","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsJetsEta2step4_3 = new TH1D("deltaEtaJetsJetsEta2_step4_3","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsJetsEta2step4_3 = new TH1D("deltaPhiJetsJetsEta2_step4_3","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsJetsEta2step4_3 = new TH1D("deltaPtJetsJetsEta2_step4_3","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassJetsEta2step4_3 = new TH1D("dijetMassJetsEta2_step4_3","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1JetsEta2step4_3 = new TH1D("pTJet1JetsEta2_step4_3","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2JetsEta2step4_3 = new TH1D("pTJet2JetsEta2_step4_3","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1JetsEta2step4_3 = new TH1D("etaJet1JetsEta2_step4_3","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2JetsEta2step4_3 = new TH1D("etaJet2JetsEta2_step4_3","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1JetsEta2step4_3 = new TH1D("phiJet1JetsEta2_step4_3","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2JetsEta2step4_3 = new TH1D("phiJet2JetsEta2_step4_3","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFJetsEta2step4_3 = new TH1D("deltaEtaPFJetsEta2_step4_3","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFJetsEta2step4_3 = new TH1D("absdeltaEtaPFJetsEta2_step4_3","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_pfetamaxJetsEta2step4_3 = new TH1D("pfetamaxJetsEta2_step4_3","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminJetsEta2step4_3 = new TH1D("pfetaminJetsEta2_step4_3","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_puBx0JetsEta2step4_3 = new TH1D("pileupmcBx0JetsEta2_step4_3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1JetsEta2step4_3 = new TH1D("pileupmcBxm1JetsEta2_step4_3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1JetsEta2step4_3 = new TH1D("pileupmcBxp1JetsEta2_step4_3","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexJetsEta2step4_3 = new TH1D("vertexJetsEta2_step4_3","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusJetsEta2step4_3 = new TH1D("sumEHFplusJetsEta2_step4_3","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusJetsEta2step4_3 = new TH1D("sumEHFminusJetsEta2_step4_3","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusJetsEta2step4_3 = new TH1D("sumEHEplusJetsEta2_step4_3","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusJetsEta2step4_3 = new TH1D("sumEHEminusJetsEta2_step4_3","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusJetsEta2step4_3 = new TH1D("sumEHFpfplusJetsEta2_step4_3","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusJetsEta2step4_3 = new TH1D("sumEHFpfminusJetsEta2_step4_3","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);

   // |Eta max and Eta min| < 2
   TH1D *h_RJJJetsEta2step4_2 = new TH1D("RJJJetsEta2_step4_2","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFJetsEta2step4_2 = new TH2F( "mHFJetsEta2_step4_2", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_CalosJetsEta2step4_2 = new TH2F( "ETCalosJetsEta2_step4_2", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksJetsEta2step4_2 = new TH1D("tracksJetsEta2_step4_2","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_aSumEJetsEta2step4_2 = new TH1D("aEnergyJetsEta2_step4_2","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsJetsEta2step4_2 = new TH1D("deltaEtaJetsJetsEta2_step4_2","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsJetsEta2step4_2 = new TH1D("deltaPhiJetsJetsEta2_step4_2","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsJetsEta2step4_2 = new TH1D("deltaPtJetsJetsEta2_step4_2","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassJetsEta2step4_2 = new TH1D("dijetMassJetsEta2_step4_2","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1JetsEta2step4_2 = new TH1D("pTJet1JetsEta2_step4_2","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2JetsEta2step4_2 = new TH1D("pTJet2JetsEta2_step4_2","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1JetsEta2step4_2 = new TH1D("etaJet1JetsEta2_step4_2","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2JetsEta2step4_2 = new TH1D("etaJet2JetsEta2_step4_2","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1JetsEta2step4_2 = new TH1D("phiJet1JetsEta2_step4_2","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2JetsEta2step4_2 = new TH1D("phiJet2JetsEta2_step4_2","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFJetsEta2step4_2 = new TH1D("deltaEtaPFJetsEta2_step4_2","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFJetsEta2step4_2 = new TH1D("absdeltaEtaPFJetsEta2_step4_2","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_pfetamaxJetsEta2step4_2 = new TH1D("pfetamaxJetsEta2_step4_2","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminJetsEta2step4_2 = new TH1D("pfetaminJetsEta2_step4_2","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_puBx0JetsEta2step4_2 = new TH1D("pileupmcBx0JetsEta2_step4_2","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1JetsEta2step4_2 = new TH1D("pileupmcBxm1JetsEta2_step4_2","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1JetsEta2step4_2 = new TH1D("pileupmcBxp1JetsEta2_step4_2","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexJetsEta2step4_2 = new TH1D("vertexJetsEta2_step4_2","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusJetsEta2step4_2 = new TH1D("sumEHFplusJetsEta2_step4_2","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusJetsEta2step4_2 = new TH1D("sumEHFminusJetsEta2_step4_2","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusJetsEta2step4_2 = new TH1D("sumEHEplusJetsEta2_step4_2","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusJetsEta2step4_2 = new TH1D("sumEHEminusJetsEta2_step4_2","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusJetsEta2step4_2 = new TH1D("sumEHFpfplusJetsEta2_step4_2","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusJetsEta2step4_2 = new TH1D("sumEHFpfminusJetsEta2_step4_2","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);

   // |Eta max and Eta min| < 1
   TH1D *h_RJJJetsEta2step4_1 = new TH1D("RJJJetsEta2_step4_1","R_{jj} Distribution; R_{jj}; N events",50,0,1);
   TH2F *h_MultHFJetsEta2step4_1 = new TH2F( "mHFJetsEta2_step4_1", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
   TH2F *h_ET_CalosJetsEta2step4_1 = new TH2F( "ETCalosJetsEta2_step4_1", "HF^{+} and Castor; #sum Energy HF^{+}; log_{10} #sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
   TH1D *h_tracksJetsEta2step4_1 = new TH1D("tracksJetsEta2_step4_1","Tracks Multiplicity; n Tracks; N events",50,0,150);
   TH1D *h_aSumEJetsEta2step4_1 = new TH1D("aEnergyJetsEta2_step4_1","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
   TH1D *h_deltaEtaJetsJetsEta2step4_1 = new TH1D("deltaEtaJetsJetsEta2_step4_1","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
   TH1D *h_deltaPhiJetsJetsEta2step4_1 = new TH1D("deltaPhiJetsJetsEta2_step4_1","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
   TH1D *h_deltaPtJetsJetsEta2step4_1 = new TH1D("deltaPtJetsJetsEta2_step4_1","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
   TH1D *h_dijetMassJetsEta2step4_1 = new TH1D("dijetMassJetsEta2_step4_1","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
   TH1D *h_pTJet1JetsEta2step4_1 = new TH1D("pTJet1JetsEta2_step4_1","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_pTJet2JetsEta2step4_1 = new TH1D("pTJet2JetsEta2_step4_1","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
   TH1D *h_etaJet1JetsEta2step4_1 = new TH1D("etaJet1JetsEta2_step4_1","Leading Jet - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_etaJet2JetsEta2step4_1 = new TH1D("etaJet2JetsEta2_step4_1","Second Jet  - #eta Distribution; #eta; N events",50,-5.5,5.5);
   TH1D *h_phiJet1JetsEta2step4_1 = new TH1D("phiJet1JetsEta2_step4_1","Leading Jet - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_phiJet2JetsEta2step4_1 = new TH1D("phiJet2JetsEta2_step4_1","Second Jet  - #phi Distribution; #phi [rad]; N events",50,-3.3,3.3);
   TH1D *h_deltaEtaPFJetsEta2step4_1 = new TH1D("deltaEtaPFJetsEta2_step4_1","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,0,10);
   TH1D *h_absdeltaEtaPFJetsEta2step4_1 = new TH1D("absdeltaEtaPFJetsEta2_step4_1","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0,10);
   TH1D *h_pfetamaxJetsEta2step4_1 = new TH1D("pfetamaxJetsEta2_step4_1","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
   TH1D *h_pfetaminJetsEta2step4_1 = new TH1D("pfetaminJetsEta2_step4_1","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
   TH1D *h_puBx0JetsEta2step4_1 = new TH1D("pileupmcBx0JetsEta2_step4_1","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxm1JetsEta2step4_1 = new TH1D("pileupmcBxm1JetsEta2_step4_1","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_puBxp1JetsEta2step4_1 = new TH1D("pileupmcBxp1JetsEta2_step4_1","PileUp Monte Carlo; # Pile Up; N events",25,0,25);
   TH1D *h_vertexJetsEta2step4_1 = new TH1D("vertexJetsEta2_step4_1","Number of Vertex; # Vertex; N events",25,0,25);
   TH1D *h_sumEHFplusJetsEta2step4_1 = new TH1D("sumEHFplusJetsEta2_step4_1","HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFminusJetsEta2step4_1 = new TH1D("sumEHFminusJetsEta2_step4_1","HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEplusJetsEta2step4_1 = new TH1D("sumEHEplusJetsEta2_step4_1","HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHEminusJetsEta2step4_1 = new TH1D("sumEHEminusJetsEta2_step4_1","HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfplusJetsEta2step4_1 = new TH1D("sumEHFpfplusJetsEta2_step4_1","HF^{+} - Sum of PF Energy; #sum E_{HF^{+}_{PF}} [GeV]; N events",100,0,2000);
   TH1D *h_sumEHFpfminusJetsEta2step4_1 = new TH1D("sumEHFpfminusJetsEta2_step4_1","HF^{-} - Sum of PF Energy; #sum E_{HF^{-}_{PF}} [GeV]; N events",100,0,2000);
   //------------>

   unsigned NEntries = tr->GetEntries();
   std::cout << "" << std::endl;
   std::cout<< "Reading Tree: "<< NEntries << " events"<<std::endl;
   std::cout << "" << std::endl;

   int decade = 0;

   // Event by Event Analysis
   //////////////////////////

   int TotalE = 0;
   double counterTrigger = 0.;
   double counterJetsstep1 = 0.;
   double counterJetsstep2 = 0.;
   double counterJetsAllstep3 = 0.;
   double counterJetsAllstep4_4 = 0.;
   double counterJetsAllstep4_3 = 0.;
   double counterJetsAllstep4_2 = 0.;
   double counterJetsAllstep4_1 = 0.;
   double counterJetsTrackerstep3 = 0.;
   double counterJetsTrackerstep4_4 = 0.;
   double counterJetsTrackerstep4_3 = 0.;
   double counterJetsTrackerstep4_2 = 0.;
   double counterJetsTrackerstep4_1 = 0.;
   double counterJetsEta2step3 = 0.;
   double counterJetsEta2step4_4 = 0.;
   double counterJetsEta2step4_3 = 0.;
   double counterJetsEta2step4_2 = 0.;
   double counterJetsEta2step4_1 = 0.;
   double deltaphi_ = 0.;
   double aSumE_ = 0.;
   double absdeltaetapf_ = 0.;
   double deltaetapf_ = .0;

   for(int i=0;i<NEVENTS;i++) {
      double totalweight = -999.;
      double totalweightbxm1 = -999.;
      double totalweightbxp1 = -999.;
      ++TotalE;

      //----------- Progress Report -------------
      
      double progress = 10.0*i/(1.0*NEVENTS);
      int k = TMath::FloorNint(progress); 
      if (k > decade) 
	 std::cout<<10*k<<" %"<<std::endl;
      decade = k;          

      //----------- Read the Event --------------
      tr->GetEntry(i);

      deltaphi_ = fabs(eventexcl->GetLeadingJetPhi()-eventexcl->GetSecondJetPhi());
      aSumE_ = (eventdiff->GetSumEnergyHFPlus() - eventdiff->GetSumEnergyHFMinus())/(eventdiff->GetSumEnergyHFPlus() + eventdiff->GetSumEnergyHFMinus());
      absdeltaetapf_ = fabs(eventdiff->GetEtaMaxFromPFCands()-eventdiff->GetEtaMinFromPFCands());
      deltaetapf_ = (eventdiff->GetEtaMaxFromPFCands()-eventdiff->GetEtaMinFromPFCands());

      //------------------------------------------------------------------------------------------
      //
      // SET PILE-UP REWEIGHT METHOD!!!!
      // NOTE: you can use eventexcl->GetNPileUpBxm1() or eventexcl->GetNPileUpBxp1()
      double weight;
      double weightbxp1;
      double weightbxm1;
 
      if (switchWeightPU) { 
	 //weight = LumiWeights_.weightOOT(eventexcl->GetNPileUpBx0(),eventexcl->GetNPileUpBxm1());} 
	 weight = LumiWeights_.weight( eventexcl->GetNPileUpBx0()); 
         weightbxp1 = LumiWeights_.weight( eventexcl->GetNPileUpBxp1()); 
         weightbxm1 = LumiWeights_.weight( eventexcl->GetNPileUpBxm1()); 
      }
      else { weight = 1.0;
             weightbxp1 = 1.0;
             weightbxm1 = 1.0;
      }
      //------------------------------------------------------------------------------------------

      double weightlumi;
      double triggereff;
      double weightepw;
      if (switchWeightLumi) { weightlumi = weightlumipass; }
      else { weightlumi = 1.0;}

      if (switchWeightEff) { triggereff = triggereffpass;}
      else { triggereff = 1.0;}

      if (switchWeightePw) { weightepw = eventqcd->evtHdr().weight();}
      else { weightepw = 1.0;}

      //---------->>
      /*
	 std::cout << "BEFORE TRIGGER" << std::endl;
	 std::cout << "PileUp-1: " << eventexcl->GetNPileUpBxm1() << std::endl; 
	 std::cout << "PileUp0: " << eventexcl->GetNPileUpBx0() << std::endl;
	 std::cout << "PileUp+1: " << eventexcl->GetNPileUpBxp1() << std::endl;
	 std::cout << "==============" << std::endl;   
       */
      //---------->>

      totalweight = triggereff*weight*weightlumi*weightepw;
      totalweightbxm1 = triggereff*weightbxm1*weightlumi*weightepw;
      totalweightbxp1 = triggereff*weightbxp1*weightlumi*weightepw;

      if( i % 1000 == 0 ){
	    std::cout << "\nEvent " << i << std::endl
            << "Nr. events Bx 0   : " << eventexcl->GetNPileUpBx0() << std::endl
            << "Nr. events Bxm1   : " << eventexcl->GetNPileUpBxm1() << std::endl
            << "Nr. events Bxp1   : " << eventexcl->GetNPileUpBxp1() << std::endl
	    << "Pile-up weight    : " << weight << std::endl
	    << "Lumi corr.        : " << weightlumi << std::endl
	    << "Trigger corr.     : " << triggereff << std::endl
	    << "MC event weight   : " << weightepw << std::endl
	    << "Total weight BX0  : " << totalweight << std::endl
            << "Total weight BXm1 : " << totalweightbxm1 << std::endl
            << "Total weight BXp1 : " << totalweightbxp1 << std::endl;
      }
      // Without Cuts          
      ////////////////////////////////////////////////
      h_DeltaEtaGenwc->Fill(eventexcl->GetDeltaEtaGen(),totalweight);
      h_MxGenRangewc->Fill(eventexcl->GetMxGenRange(),totalweight);
      h_RJJwc->Fill(eventexcl->GetRjjFromJets(),totalweight);
      h_MultHFwc->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
      h_ET_Caloswc->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
      h_trackswc->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
      h_aSumEwc->Fill(aSumE_,totalweight);
      h_pfetaminwc->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
      h_pfetamaxwc->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
      h_deltaEtaJetswc->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
      h_deltaPhiJetswc->Fill(deltaphi_,totalweight);
      h_deltaPtJetswc->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
      h_dijetMasswc->Fill(eventexcl->GetMassDijets(),totalweight);
      h_pTJet1wc->Fill(eventexcl->GetLeadingJetPt(),totalweight);
      h_pTJet2wc->Fill(eventexcl->GetSecondJetPt(),totalweight);
      h_etaJet1wc->Fill(eventexcl->GetLeadingJetEta(),totalweight);
      h_etaJet2wc->Fill(eventexcl->GetSecondJetEta(),totalweight);
      h_phiJet1wc->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
      h_phiJet2wc->Fill(eventexcl->GetSecondJetPhi(),totalweight);
      h_deltaEtaPFwc->Fill(deltaetapf_,totalweight);
      h_absdeltaEtaPFwc->Fill(absdeltaetapf_,totalweight);
      h_puBx0wc->Fill(eventexcl->GetNPileUpBx0(),totalweight);
      h_puBxm1wc->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
      h_puBxp1wc->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
      h_vertexwc->Fill(eventexcl->GetNVertex(), totalweight);
      h_sumEHFpluswc->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
      h_sumEHFminuswc->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
      h_sumEHEpluswc->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
      h_sumEHEminuswc->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
      h_sumEHFpfpluswc->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
      h_sumEHFpfminuswc->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);
      //////////////////////////////////////////////////


      //------------------------------------------------------------------------------------------
      //
      // SET TRIGGER OR TRIGGER EMULATOR!!!!
      //
      //
      // SIMULATED TRIGGER
      //if (eventexcl->GetSumEHFPFlowPlus() < 50 && eventexcl->GetSumEHFPFlowMinus() < 50 && eventexcl->GetLeadingJetP4().Pt() > 30 && eventexcl->GetSecondJetP4().Pt() > 30){
	
         // TRIGGER
	 if (eventexcl->GetHLTPath(optTrigger)){
	 //
	 //------------------------------------------------------------------------------------------

	 counterTrigger+=totalweight;

	 // With Trigger: online or emulate          
	 ////////////////////////////////////////////////
	 h_DeltaEtaGenwt->Fill(eventexcl->GetDeltaEtaGen(),totalweight);
	 h_MxGenRangewt->Fill(eventexcl->GetMxGenRange(),totalweight);
	 h_RJJwt->Fill(eventexcl->GetRjjFromJets(),totalweight);
	 h_MultHFwt->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
	 h_ET_Caloswt->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
	 h_trackswt->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
	 h_aSumEwt->Fill(aSumE_,totalweight);
	 h_pfetaminwt->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
	 h_pfetamaxwt->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
	 h_deltaEtaJetswt->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
	 h_deltaPhiJetswt->Fill(deltaphi_,totalweight);
	 h_deltaPtJetswt->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
	 h_dijetMasswt->Fill(eventexcl->GetMassDijets(),totalweight);
	 h_pTJet1wt->Fill(eventexcl->GetLeadingJetPt(),totalweight);
	 h_pTJet2wt->Fill(eventexcl->GetSecondJetPt(),totalweight);
         h_etaJet1wt->Fill(eventexcl->GetLeadingJetEta(),totalweight);
         h_etaJet2wt->Fill(eventexcl->GetSecondJetEta(),totalweight);
         h_phiJet1wt->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
         h_phiJet2wt->Fill(eventexcl->GetSecondJetPhi(),totalweight);
	 h_deltaEtaPFwt->Fill(deltaetapf_,totalweight);
	 h_absdeltaEtaPFwt->Fill(absdeltaetapf_,totalweight);
         h_puBx0wt->Fill(eventexcl->GetNPileUpBx0(),totalweight);
         h_puBxm1wt->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
         h_puBxp1wt->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
         h_vertexwt->Fill(eventexcl->GetNVertex(), totalweight);
         h_sumEHFpluswt->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
         h_sumEHFminuswt->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
         h_sumEHEpluswt->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
         h_sumEHEminuswt->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
         h_sumEHFpfpluswt->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
         h_sumEHFpfminuswt->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);
         //////////////////////////////////////////////////


	 if(eventexcl->GetNVertex() > 0 && eventexcl->GetNVertex()<= optnVertex){

            counterJetsstep1+=totalweight; 

	    //---------->>

	    /*
	       std::cout << "" << std::endl;
	       std::cout << "PileUp-1: " << eventexcl->GetNPileUpBxm1() << std::endl;    
	       std::cout << "PileUp0: " << eventexcl->GetNPileUpBx0() << std::endl;
	       std::cout << "PileUp+1: " << eventexcl->GetNPileUpBxp1() << std::endl;
	       std::cout << "Weigh["<<i<<"]: " << LumiWeights_.ITweight(eventexcl->GetNPileUpBx0()) << std::endl;
	       std::cout << "" << std::endl;
	     */

	    //std::cout << "" << std::endl;
	    //std::cout << "Weight["<<i<<"]: " << weight << std::endl;
	    //std::cout << "" << std::endl;

	    //---------->

	    // STEP1        
	    ////////////////////////////////////////////////
	    h_DeltaEtaGenstep1->Fill(eventexcl->GetDeltaEtaGen(),totalweight);
	    h_MxGenRangestep1->Fill(eventexcl->GetMxGenRange(),totalweight);
	    h_RJJstep1->Fill(eventexcl->GetRjjFromJets(),totalweight);
	    h_MultHFstep1->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
	    h_ET_Calosstep1->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
	    h_tracksstep1->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
	    h_aSumEstep1->Fill(aSumE_,totalweight);
	    h_pfetaminstep1->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
	    h_pfetamaxstep1->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
	    h_deltaEtaJetsstep1->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
	    h_deltaPhiJetsstep1->Fill(deltaphi_,totalweight);
	    h_deltaPtJetsstep1->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
	    h_dijetMassstep1->Fill(eventexcl->GetMassDijets(),totalweight);
	    h_pTJet1step1->Fill(eventexcl->GetLeadingJetPt(),totalweight);
	    h_pTJet2step1->Fill(eventexcl->GetSecondJetPt(),totalweight);
	    h_etaJet1step1->Fill(eventexcl->GetLeadingJetEta(),totalweight);
            h_etaJet2step1->Fill(eventexcl->GetSecondJetEta(),totalweight);
            h_phiJet1step1->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
            h_phiJet2step1->Fill(eventexcl->GetSecondJetPhi(),totalweight);
            h_deltaEtaPFstep1->Fill(deltaetapf_,totalweight);
	    h_absdeltaEtaPFstep1->Fill(absdeltaetapf_,totalweight);
            h_puBx0step1->Fill(eventexcl->GetNPileUpBx0(),totalweight);
            h_puBxm1step1->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
            h_puBxp1step1->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
            h_vertexstep1->Fill(eventexcl->GetNVertex(), totalweight);
            h_sumEHFplusstep1->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
            h_sumEHFminusstep1->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
            h_sumEHEplusstep1->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
            h_sumEHEminusstep1->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
            h_sumEHFpfplusstep1->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
            h_sumEHFpfminusstep1->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);
	    //////////////////////////////////////////////////


	    // Setting Analysis Cut
	    if (eventexcl->GetLeadingJetP4().Pt() > jet1PT && eventexcl->GetSecondJetP4().Pt() > jet2PT ){

	       counterJetsstep2+=totalweight;

	       // STEP2         
	       ////////////////////////////////////////////////
	       h_DeltaEtaGenstep2->Fill(eventexcl->GetDeltaEtaGen(),totalweight);
	       h_MxGenRangestep2->Fill(eventexcl->GetMxGenRange(),totalweight);
	       h_RJJstep2->Fill(eventexcl->GetRjjFromJets(),totalweight);
	       h_MultHFstep2->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
	       h_ET_Calosstep2->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
	       h_tracksstep2->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
	       h_aSumEstep2->Fill(aSumE_,totalweight);
	       h_pfetaminstep2->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
	       h_pfetamaxstep2->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
	       h_deltaEtaJetsstep2->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
	       h_deltaPhiJetsstep2->Fill(deltaphi_,totalweight);
	       h_deltaPtJetsstep2->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
	       h_dijetMassstep2->Fill(eventexcl->GetMassDijets(),totalweight);
	       h_pTJet1step2->Fill(eventexcl->GetLeadingJetPt(),totalweight);
	       h_pTJet2step2->Fill(eventexcl->GetSecondJetPt(),totalweight);
               h_etaJet1step2->Fill(eventexcl->GetLeadingJetEta(),totalweight);
               h_etaJet2step2->Fill(eventexcl->GetSecondJetEta(),totalweight);
               h_phiJet1step2->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
               h_phiJet2step2->Fill(eventexcl->GetSecondJetPhi(),totalweight);
	       h_deltaEtaPFstep2->Fill(deltaetapf_,totalweight);
	       h_absdeltaEtaPFstep2->Fill(absdeltaetapf_,totalweight);
               h_puBx0step2->Fill(eventexcl->GetNPileUpBx0(),totalweight);
               h_puBxm1step2->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
               h_puBxp1step2->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
               h_vertexstep2->Fill(eventexcl->GetNVertex(), totalweight);
               h_sumEHFplusstep2->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
               h_sumEHFminusstep2->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
               h_sumEHEplusstep2->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
               h_sumEHEminusstep2->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
               h_sumEHFpfplusstep2->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
               h_sumEHFpfminusstep2->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);
	       //////////////////////////////////////////////////

	       if (eventexcl->GetLeadingJetP4().Eta() < 5.2 && eventexcl->GetSecondJetP4().Eta() < 5.2 && eventexcl->GetLeadingJetP4().Eta() > -5.2 && eventexcl->GetSecondJetP4().Eta() > -5.2){

		  counterJetsAllstep3+=totalweight;

		  // ALL - STEP3         
		  ////////////////////////////////////////////////
		  h_DeltaEtaGenAllstep3->Fill(eventexcl->GetDeltaEtaGen(),totalweight);
		  h_MxGenRangeAllstep3->Fill(eventexcl->GetMxGenRange(),totalweight);
		  h_RJJAllstep3->Fill(eventexcl->GetRjjFromJets(),totalweight);
		  h_MultHFAllstep3->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
		  h_ET_CalosAllstep3->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
		  h_tracksAllstep3->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
		  h_aSumEAllstep3->Fill(aSumE_,totalweight);
		  h_pfetaminAllstep3->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
		  h_pfetamaxAllstep3->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
		  h_deltaEtaJetsAllstep3->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
		  h_deltaPhiJetsAllstep3->Fill(deltaphi_,totalweight);
		  h_deltaPtJetsAllstep3->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
		  h_dijetMassAllstep3->Fill(eventexcl->GetMassDijets(),totalweight);
		  h_pTJet1Allstep3->Fill(eventexcl->GetLeadingJetPt(),totalweight);
		  h_pTJet2Allstep3->Fill(eventexcl->GetSecondJetPt(),totalweight);
                  h_etaJet1Allstep3->Fill(eventexcl->GetLeadingJetEta(),totalweight);
                  h_etaJet2Allstep3->Fill(eventexcl->GetSecondJetEta(),totalweight);
                  h_phiJet1Allstep3->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
                  h_phiJet2Allstep3->Fill(eventexcl->GetSecondJetPhi(),totalweight);
		  h_deltaEtaPFAllstep3->Fill(deltaetapf_,totalweight);
		  h_absdeltaEtaPFAllstep3->Fill(absdeltaetapf_,totalweight);
                  h_puBx0Allstep3->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                  h_puBxm1Allstep3->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
                  h_puBxp1Allstep3->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
                  h_vertexAllstep3->Fill(eventexcl->GetNVertex(), totalweight);
                  h_sumEHFplusAllstep3->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
                  h_sumEHFminusAllstep3->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
                  h_sumEHEplusAllstep3->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
                  h_sumEHEminusAllstep3->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
                  h_sumEHFpfplusAllstep3->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
                  h_sumEHFpfminusAllstep3->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);

		  // Eta max and Eta min cut
		  if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){

                     counterJetsAllstep4_4+=totalweight;

		     h_RJJAllstep4_4->Fill(eventexcl->GetRjjFromJets(),totalweight);
		     h_MultHFAllstep4_4->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
		     h_ET_CalosAllstep4_4->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
		     h_tracksAllstep4_4->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
		     h_aSumEAllstep4_4->Fill(aSumE_,totalweight);
		     h_deltaEtaJetsAllstep4_4->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
		     h_deltaPhiJetsAllstep4_4->Fill(deltaphi_,totalweight);
		     h_deltaPtJetsAllstep4_4->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
		     h_dijetMassAllstep4_4->Fill(eventexcl->GetMassDijets(),totalweight);
		     h_pTJet1Allstep4_4->Fill(eventexcl->GetLeadingJetPt(),totalweight);
		     h_pTJet2Allstep4_4->Fill(eventexcl->GetSecondJetPt(),totalweight);
                     h_etaJet1Allstep4_4->Fill(eventexcl->GetLeadingJetEta(),totalweight);
                     h_etaJet2Allstep4_4->Fill(eventexcl->GetSecondJetEta(),totalweight);
                     h_phiJet1Allstep4_4->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
                     h_phiJet2Allstep4_4->Fill(eventexcl->GetSecondJetPhi(),totalweight);
	             h_deltaEtaPFAllstep4_4->Fill(deltaetapf_,totalweight);
                     h_absdeltaEtaPFAllstep4_4->Fill(absdeltaetapf_,totalweight);
                     h_pfetaminAllstep4_4->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
                     h_pfetamaxAllstep4_4->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
                     h_puBx0Allstep4_4->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     h_puBxm1Allstep4_4->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
                     h_puBxp1Allstep4_4->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
                     h_vertexAllstep4_4->Fill(eventexcl->GetNVertex(), totalweight);
                     h_sumEHFplusAllstep4_4->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
                     h_sumEHFminusAllstep4_4->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
                     h_sumEHEplusAllstep4_4->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
                     h_sumEHEminusAllstep4_4->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
                     h_sumEHFpfplusAllstep4_4->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
                     h_sumEHFpfminusAllstep4_4->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);
                  }

		  if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){

                     counterJetsAllstep4_3+=totalweight;

		     h_RJJAllstep4_3->Fill(eventexcl->GetRjjFromJets(),totalweight);
		     h_MultHFAllstep4_3->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
		     h_ET_CalosAllstep4_3->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
		     h_tracksAllstep4_3->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
		     h_aSumEAllstep4_3->Fill(aSumE_,totalweight);
		     h_deltaEtaJetsAllstep4_3->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
		     h_deltaPhiJetsAllstep4_3->Fill(deltaphi_,totalweight);
		     h_deltaPtJetsAllstep4_3->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
		     h_dijetMassAllstep4_3->Fill(eventexcl->GetMassDijets(),totalweight);
		     h_pTJet1Allstep4_3->Fill(eventexcl->GetLeadingJetPt(),totalweight);
		     h_pTJet2Allstep4_3->Fill(eventexcl->GetSecondJetPt(),totalweight);
                     h_etaJet1Allstep4_3->Fill(eventexcl->GetLeadingJetEta(),totalweight);
                     h_etaJet2Allstep4_3->Fill(eventexcl->GetSecondJetEta(),totalweight);
                     h_phiJet1Allstep4_3->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
                     h_phiJet2Allstep4_3->Fill(eventexcl->GetSecondJetPhi(),totalweight);
                     h_deltaEtaPFAllstep4_3->Fill(deltaetapf_,totalweight);
                     h_absdeltaEtaPFAllstep4_3->Fill(absdeltaetapf_,totalweight);
                     h_pfetaminAllstep4_3->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
                     h_pfetamaxAllstep4_3->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
                     h_puBx0Allstep4_3->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     h_puBxm1Allstep4_3->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
                     h_puBxp1Allstep4_3->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
                     h_vertexAllstep4_3->Fill(eventexcl->GetNVertex(), totalweight);
                     h_sumEHFplusAllstep4_3->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
                     h_sumEHFminusAllstep4_3->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
                     h_sumEHEplusAllstep4_3->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
                     h_sumEHEminusAllstep4_3->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
                     h_sumEHFpfplusAllstep4_3->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
                     h_sumEHFpfminusAllstep4_3->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);
		  }

		  if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){

                     counterJetsAllstep4_2+=totalweight;

		     h_RJJAllstep4_2->Fill(eventexcl->GetRjjFromJets(),totalweight);
		     h_MultHFAllstep4_2->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
		     h_ET_CalosAllstep4_2->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
		     h_tracksAllstep4_2->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
		     h_aSumEAllstep4_2->Fill(aSumE_,totalweight);
		     h_deltaEtaJetsAllstep4_2->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
		     h_deltaPhiJetsAllstep4_2->Fill(deltaphi_,totalweight);
		     h_deltaPtJetsAllstep4_2->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
		     h_dijetMassAllstep4_2->Fill(eventexcl->GetMassDijets(),totalweight);
		     h_pTJet1Allstep4_2->Fill(eventexcl->GetLeadingJetPt(),totalweight);
		     h_pTJet2Allstep4_2->Fill(eventexcl->GetSecondJetPt(),totalweight);
                     h_etaJet1Allstep4_2->Fill(eventexcl->GetLeadingJetEta(),totalweight);
                     h_etaJet2Allstep4_2->Fill(eventexcl->GetSecondJetEta(),totalweight);
                     h_phiJet1Allstep4_2->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
                     h_phiJet2Allstep4_2->Fill(eventexcl->GetSecondJetPhi(),totalweight);
                     h_deltaEtaPFAllstep4_2->Fill(deltaetapf_,totalweight);
                     h_absdeltaEtaPFAllstep4_2->Fill(absdeltaetapf_,totalweight);
                     h_pfetaminAllstep4_2->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
                     h_pfetamaxAllstep4_2->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
                     h_puBx0Allstep4_2->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     h_puBxm1Allstep4_2->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
                     h_puBxp1Allstep4_2->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
                     h_vertexAllstep4_2->Fill(eventexcl->GetNVertex(), totalweight);
                     h_sumEHFplusAllstep4_2->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
                     h_sumEHFminusAllstep4_2->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
                     h_sumEHEplusAllstep4_2->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
                     h_sumEHEminusAllstep4_2->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
                     h_sumEHFpfplusAllstep4_2->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
                     h_sumEHFpfminusAllstep4_2->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);
		  }

		  if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){

                     counterJetsAllstep4_1+=totalweight;

		     h_RJJAllstep4_1->Fill(eventexcl->GetRjjFromJets(),totalweight);
		     h_MultHFAllstep4_1->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
		     h_ET_CalosAllstep4_1->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
		     h_tracksAllstep4_1->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
		     h_aSumEAllstep4_1->Fill(aSumE_,totalweight);
		     h_deltaEtaJetsAllstep4_1->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
		     h_deltaPhiJetsAllstep4_1->Fill(deltaphi_,totalweight);
		     h_deltaPtJetsAllstep4_1->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
		     h_dijetMassAllstep4_1->Fill(eventexcl->GetMassDijets(),totalweight);
		     h_pTJet1Allstep4_1->Fill(eventexcl->GetLeadingJetPt(),totalweight);
		     h_pTJet2Allstep4_1->Fill(eventexcl->GetSecondJetPt(),totalweight);
                     h_etaJet1Allstep4_1->Fill(eventexcl->GetLeadingJetEta(),totalweight);
                     h_etaJet2Allstep4_1->Fill(eventexcl->GetSecondJetEta(),totalweight);
                     h_phiJet1Allstep4_1->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
                     h_phiJet2Allstep4_1->Fill(eventexcl->GetSecondJetPhi(),totalweight);
                     h_deltaEtaPFAllstep4_1->Fill(deltaetapf_,totalweight);
                     h_absdeltaEtaPFAllstep4_1->Fill(absdeltaetapf_,totalweight);
                     h_pfetaminAllstep4_1->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
                     h_pfetamaxAllstep4_1->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
                     h_puBx0Allstep4_1->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     h_puBxm1Allstep4_1->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
                     h_puBxp1Allstep4_1->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
                     h_vertexAllstep4_1->Fill(eventexcl->GetNVertex(), totalweight);
                     h_sumEHFplusAllstep4_1->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
                     h_sumEHFminusAllstep4_1->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
                     h_sumEHEplusAllstep4_1->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
                     h_sumEHEminusAllstep4_1->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
                     h_sumEHFpfplusAllstep4_1->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
                     h_sumEHFpfminusAllstep4_1->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);
                   }

	       }  
	       // end Jets at all CMS Acceptance

	       if (eventexcl->GetLeadingJetP4().Eta() < 2.9 && eventexcl->GetSecondJetP4().Eta() < 2.9 && eventexcl->GetLeadingJetP4().Eta() > -2.9 && eventexcl->GetSecondJetP4().Eta() > -2.9){

		  counterJetsTrackerstep3+=totalweight;

		  // Tracker - STEP3         
		  ////////////////////////////////////////////////
		  h_DeltaEtaGenTrackerstep3->Fill(eventexcl->GetDeltaEtaGen(),totalweight);
		  h_MxGenRangeTrackerstep3->Fill(eventexcl->GetMxGenRange(),totalweight);
		  h_RJJTrackerstep3->Fill(eventexcl->GetRjjFromJets(),totalweight);
		  h_MultHFTrackerstep3->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
		  h_ET_CalosTrackerstep3->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
		  h_tracksTrackerstep3->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
		  h_aSumETrackerstep3->Fill(aSumE_,totalweight);
		  h_pfetaminTrackerstep3->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
		  h_pfetamaxTrackerstep3->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
		  h_deltaEtaJetsTrackerstep3->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
		  h_deltaPhiJetsTrackerstep3->Fill(deltaphi_,totalweight);
		  h_deltaPtJetsTrackerstep3->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
		  h_dijetMassTrackerstep3->Fill(eventexcl->GetMassDijets(),totalweight);
		  h_pTJet1Trackerstep3->Fill(eventexcl->GetLeadingJetPt(),totalweight);
		  h_pTJet2Trackerstep3->Fill(eventexcl->GetSecondJetPt(),totalweight);
		  h_deltaEtaPFTrackerstep3->Fill(deltaetapf_,totalweight);
		  h_absdeltaEtaPFTrackerstep3->Fill(absdeltaetapf_,totalweight);
                  h_etaJet1Trackerstep3->Fill(eventexcl->GetLeadingJetEta(),totalweight);
                  h_etaJet2Trackerstep3->Fill(eventexcl->GetSecondJetEta(),totalweight);
                  h_phiJet1Trackerstep3->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
                  h_phiJet2Trackerstep3->Fill(eventexcl->GetSecondJetPhi(),totalweight);
                  h_puBx0Trackerstep3->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                  h_puBxm1Trackerstep3->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
                  h_puBxp1Trackerstep3->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
                  h_vertexTrackerstep3->Fill(eventexcl->GetNVertex(), totalweight);
                  h_sumEHFplusTrackerstep3->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
                  h_sumEHFminusTrackerstep3->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
                  h_sumEHEplusTrackerstep3->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
                  h_sumEHEminusTrackerstep3->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
                  h_sumEHFpfplusTrackerstep3->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
                  h_sumEHFpfminusTrackerstep3->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);

		  // Eta max and Eta min cut
		  if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){

                     counterJetsTrackerstep4_4+=totalweight;

		     h_RJJTrackerstep4_4->Fill(eventexcl->GetRjjFromJets(),totalweight);
		     h_MultHFTrackerstep4_4->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
		     h_ET_CalosTrackerstep4_4->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
		     h_tracksTrackerstep4_4->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
		     h_aSumETrackerstep4_4->Fill(aSumE_,totalweight);
		     h_deltaEtaJetsTrackerstep4_4->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
		     h_deltaPhiJetsTrackerstep4_4->Fill(deltaphi_,totalweight);
		     h_deltaPtJetsTrackerstep4_4->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
		     h_dijetMassTrackerstep4_4->Fill(eventexcl->GetMassDijets(),totalweight);
		     h_pTJet1Trackerstep4_4->Fill(eventexcl->GetLeadingJetPt(),totalweight);
		     h_pTJet2Trackerstep4_4->Fill(eventexcl->GetSecondJetPt(),totalweight);
                     h_etaJet1Trackerstep4_4->Fill(eventexcl->GetLeadingJetEta(),totalweight);
                     h_etaJet2Trackerstep4_4->Fill(eventexcl->GetSecondJetEta(),totalweight);
                     h_phiJet1Trackerstep4_4->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
                     h_phiJet2Trackerstep4_4->Fill(eventexcl->GetSecondJetPhi(),totalweight);
                     h_deltaEtaPFTrackerstep4_4->Fill(deltaetapf_,totalweight);
                     h_absdeltaEtaPFTrackerstep4_4->Fill(absdeltaetapf_,totalweight);
                     h_pfetaminTrackerstep4_4->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
                     h_pfetamaxTrackerstep4_4->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
                     h_puBx0Trackerstep4_4->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     h_puBxm1Trackerstep4_4->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
                     h_puBxp1Trackerstep4_4->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
                     h_vertexTrackerstep4_4->Fill(eventexcl->GetNVertex(), totalweight);
                     h_sumEHFplusTrackerstep4_4->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
                     h_sumEHFminusTrackerstep4_4->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
                     h_sumEHEplusTrackerstep4_4->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
                     h_sumEHEminusTrackerstep4_4->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
                     h_sumEHFpfplusTrackerstep4_4->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
                     h_sumEHFpfminusTrackerstep4_4->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);
		  }

		  if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){

                     counterJetsTrackerstep4_3+=totalweight;

		     h_RJJTrackerstep4_3->Fill(eventexcl->GetRjjFromJets(),totalweight);
		     h_MultHFTrackerstep4_3->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
		     h_ET_CalosTrackerstep4_3->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
		     h_tracksTrackerstep4_3->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
		     h_aSumETrackerstep4_3->Fill(aSumE_,totalweight);
		     h_deltaEtaJetsTrackerstep4_3->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
		     h_deltaPhiJetsTrackerstep4_3->Fill(deltaphi_,totalweight);
		     h_deltaPtJetsTrackerstep4_3->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
		     h_dijetMassTrackerstep4_3->Fill(eventexcl->GetMassDijets(),totalweight);
		     h_pTJet1Trackerstep4_3->Fill(eventexcl->GetLeadingJetPt(),totalweight);
		     h_pTJet2Trackerstep4_3->Fill(eventexcl->GetSecondJetPt(),totalweight);
                     h_etaJet1Trackerstep4_3->Fill(eventexcl->GetLeadingJetEta(),totalweight);
                     h_etaJet2Trackerstep4_3->Fill(eventexcl->GetSecondJetEta(),totalweight);
                     h_phiJet1Trackerstep4_3->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
                     h_phiJet2Trackerstep4_3->Fill(eventexcl->GetSecondJetPhi(),totalweight);
                     h_deltaEtaPFTrackerstep4_3->Fill(deltaetapf_,totalweight);
                     h_absdeltaEtaPFTrackerstep4_3->Fill(absdeltaetapf_,totalweight);
                     h_pfetaminTrackerstep4_3->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
                     h_pfetamaxTrackerstep4_3->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
                     h_puBx0Trackerstep4_3->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     h_puBxm1Trackerstep4_3->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
                     h_puBxp1Trackerstep4_3->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
                     h_vertexTrackerstep4_3->Fill(eventexcl->GetNVertex(), totalweight);
                     h_sumEHFplusTrackerstep4_3->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
                     h_sumEHFminusTrackerstep4_3->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
                     h_sumEHEplusTrackerstep4_3->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
                     h_sumEHEminusTrackerstep4_3->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
                     h_sumEHFpfplusTrackerstep4_3->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
                     h_sumEHFpfminusTrackerstep4_3->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);
		  }

		  if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){

                     counterJetsTrackerstep4_2+=totalweight;

		     h_RJJTrackerstep4_2->Fill(eventexcl->GetRjjFromJets(),totalweight);
		     h_MultHFTrackerstep4_2->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
		     h_ET_CalosTrackerstep4_2->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
		     h_tracksTrackerstep4_2->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
		     h_aSumETrackerstep4_2->Fill(aSumE_,totalweight);
		     h_deltaEtaJetsTrackerstep4_2->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
		     h_deltaPhiJetsTrackerstep4_2->Fill(deltaphi_,totalweight);
		     h_deltaPtJetsTrackerstep4_2->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
		     h_dijetMassTrackerstep4_2->Fill(eventexcl->GetMassDijets(),totalweight);
		     h_pTJet1Trackerstep4_2->Fill(eventexcl->GetLeadingJetPt(),totalweight);
		     h_pTJet2Trackerstep4_2->Fill(eventexcl->GetSecondJetPt(),totalweight);
                     h_etaJet1Trackerstep4_2->Fill(eventexcl->GetLeadingJetEta(),totalweight);
                     h_etaJet2Trackerstep4_2->Fill(eventexcl->GetSecondJetEta(),totalweight);
                     h_phiJet1Trackerstep4_2->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
                     h_phiJet2Trackerstep4_2->Fill(eventexcl->GetSecondJetPhi(),totalweight);
                     h_deltaEtaPFTrackerstep4_2->Fill(deltaetapf_,totalweight);
                     h_absdeltaEtaPFTrackerstep4_2->Fill(absdeltaetapf_,totalweight);
                     h_pfetaminTrackerstep4_2->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
                     h_pfetamaxTrackerstep4_2->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
                     h_puBx0Trackerstep4_2->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     h_puBxm1Trackerstep4_2->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
                     h_puBxp1Trackerstep4_2->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
                     h_vertexTrackerstep4_2->Fill(eventexcl->GetNVertex(), totalweight);
                     h_sumEHFplusTrackerstep4_2->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
                     h_sumEHFminusTrackerstep4_2->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
                     h_sumEHEplusTrackerstep4_2->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
                     h_sumEHEminusTrackerstep4_2->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
                     h_sumEHFpfplusTrackerstep4_2->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
                     h_sumEHFpfminusTrackerstep4_2->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);
		  }

		  if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){

                     counterJetsTrackerstep4_1+=totalweight;

		     h_RJJTrackerstep4_1->Fill(eventexcl->GetRjjFromJets(),totalweight);
		     h_MultHFTrackerstep4_1->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
		     h_ET_CalosTrackerstep4_1->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
		     h_tracksTrackerstep4_1->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
		     h_aSumETrackerstep4_1->Fill(aSumE_,totalweight);
		     h_deltaEtaJetsTrackerstep4_1->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
		     h_deltaPhiJetsTrackerstep4_1->Fill(deltaphi_,totalweight);
		     h_deltaPtJetsTrackerstep4_1->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
		     h_dijetMassTrackerstep4_1->Fill(eventexcl->GetMassDijets(),totalweight);
		     h_pTJet1Trackerstep4_1->Fill(eventexcl->GetLeadingJetPt(),totalweight);
		     h_pTJet2Trackerstep4_1->Fill(eventexcl->GetSecondJetPt(),totalweight);
                     h_etaJet1Trackerstep4_1->Fill(eventexcl->GetLeadingJetEta(),totalweight);
                     h_etaJet2Trackerstep4_1->Fill(eventexcl->GetSecondJetEta(),totalweight);
                     h_phiJet1Trackerstep4_1->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
                     h_phiJet2Trackerstep4_1->Fill(eventexcl->GetSecondJetPhi(),totalweight);
                     h_deltaEtaPFTrackerstep4_1->Fill(deltaetapf_,totalweight);
                     h_absdeltaEtaPFTrackerstep4_1->Fill(absdeltaetapf_,totalweight);
                     h_pfetaminTrackerstep4_1->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
                     h_pfetamaxTrackerstep4_1->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
                     h_puBx0Trackerstep4_1->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     h_puBxm1Trackerstep4_1->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
                     h_puBxp1Trackerstep4_1->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
                     h_vertexTrackerstep4_1->Fill(eventexcl->GetNVertex(), totalweight);
                     h_sumEHFplusTrackerstep4_1->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
                     h_sumEHFminusTrackerstep4_1->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
                     h_sumEHEplusTrackerstep4_1->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
                     h_sumEHEminusTrackerstep4_1->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
                     h_sumEHFpfplusTrackerstep4_1->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
                     h_sumEHFpfminusTrackerstep4_1->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);
                  }

	       } // end jets at tracker


	       if (eventexcl->GetLeadingJetP4().Eta() < 2. && eventexcl->GetSecondJetP4().Eta() < 2. && eventexcl->GetLeadingJetP4().Eta() > -2. && eventexcl->GetSecondJetP4().Eta() > -2.){

		  counterJetsEta2step3+=totalweight;

		  // JetsEta2 - STEP3         
		  ////////////////////////////////////////////////
		  h_DeltaEtaGenJetsEta2step3->Fill(eventexcl->GetDeltaEtaGen(),totalweight);
		  h_MxGenRangeJetsEta2step3->Fill(eventexcl->GetMxGenRange(),totalweight);
		  h_RJJJetsEta2step3->Fill(eventexcl->GetRjjFromJets(),totalweight);
		  h_MultHFJetsEta2step3->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
		  h_ET_CalosJetsEta2step3->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
		  h_tracksJetsEta2step3->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
		  h_aSumEJetsEta2step3->Fill(aSumE_,totalweight);
		  h_pfetaminJetsEta2step3->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
		  h_pfetamaxJetsEta2step3->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
		  h_deltaEtaJetsJetsEta2step3->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
		  h_deltaPhiJetsJetsEta2step3->Fill(deltaphi_,totalweight);
		  h_deltaPtJetsJetsEta2step3->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
		  h_dijetMassJetsEta2step3->Fill(eventexcl->GetMassDijets(),totalweight);
		  h_pTJet1JetsEta2step3->Fill(eventexcl->GetLeadingJetPt(),totalweight);
		  h_pTJet2JetsEta2step3->Fill(eventexcl->GetSecondJetPt(),totalweight);
		  h_deltaEtaPFJetsEta2step3->Fill(deltaetapf_,totalweight);
		  h_absdeltaEtaPFJetsEta2step3->Fill(absdeltaetapf_,totalweight);
                  h_etaJet1JetsEta2step3->Fill(eventexcl->GetLeadingJetEta(),totalweight);
                  h_etaJet2JetsEta2step3->Fill(eventexcl->GetSecondJetEta(),totalweight);
                  h_phiJet1JetsEta2step3->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
                  h_phiJet2JetsEta2step3->Fill(eventexcl->GetSecondJetPhi(),totalweight);
                  h_puBx0JetsEta2step3->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                  h_puBxm1JetsEta2step3->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
                  h_puBxp1JetsEta2step3->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
                  h_vertexJetsEta2step3->Fill(eventexcl->GetNVertex(), totalweight);
                  h_sumEHFplusJetsEta2step3->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
                  h_sumEHFminusJetsEta2step3->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
                  h_sumEHEplusJetsEta2step3->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
                  h_sumEHEminusJetsEta2step3->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
                  h_sumEHFpfplusJetsEta2step3->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
                  h_sumEHFpfminusJetsEta2step3->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);

		  // Eta max and Eta min cut
		  if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){

                     counterJetsEta2step4_4+=totalweight;

		     h_RJJJetsEta2step4_4->Fill(eventexcl->GetRjjFromJets(),totalweight);
		     h_MultHFJetsEta2step4_4->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
		     h_ET_CalosJetsEta2step4_4->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
		     h_tracksJetsEta2step4_4->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
		     h_aSumEJetsEta2step4_4->Fill(aSumE_,totalweight);
		     h_deltaEtaJetsJetsEta2step4_4->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
		     h_deltaPhiJetsJetsEta2step4_4->Fill(deltaphi_,totalweight);
		     h_deltaPtJetsJetsEta2step4_4->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
		     h_dijetMassJetsEta2step4_4->Fill(eventexcl->GetMassDijets(),totalweight);
		     h_pTJet1JetsEta2step4_4->Fill(eventexcl->GetLeadingJetPt(),totalweight);
		     h_pTJet2JetsEta2step4_4->Fill(eventexcl->GetSecondJetPt(),totalweight);
                     h_etaJet1JetsEta2step4_4->Fill(eventexcl->GetLeadingJetEta(),totalweight);
                     h_etaJet2JetsEta2step4_4->Fill(eventexcl->GetSecondJetEta(),totalweight);
                     h_phiJet1JetsEta2step4_4->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
                     h_phiJet2JetsEta2step4_4->Fill(eventexcl->GetSecondJetPhi(),totalweight);
                     h_deltaEtaPFJetsEta2step4_4->Fill(deltaetapf_,totalweight);
                     h_absdeltaEtaPFJetsEta2step4_4->Fill(absdeltaetapf_,totalweight);
                     h_pfetaminJetsEta2step4_4->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
                     h_pfetamaxJetsEta2step4_4->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
                     h_puBx0JetsEta2step4_4->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     h_puBxm1JetsEta2step4_4->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
                     h_puBxp1JetsEta2step4_4->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
                     h_vertexJetsEta2step4_4->Fill(eventexcl->GetNVertex(), totalweight);
                     h_sumEHFplusJetsEta2step4_4->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
                     h_sumEHFminusJetsEta2step4_4->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
                     h_sumEHEplusJetsEta2step4_4->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
                     h_sumEHEminusJetsEta2step4_4->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
                     h_sumEHFpfplusJetsEta2step4_4->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
                     h_sumEHFpfminusJetsEta2step4_4->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);
		  }

		  if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){

                     counterJetsEta2step4_3+=totalweight;

		     h_RJJJetsEta2step4_3->Fill(eventexcl->GetRjjFromJets(),totalweight);
		     h_MultHFJetsEta2step4_3->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
		     h_ET_CalosJetsEta2step4_3->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
		     h_tracksJetsEta2step4_3->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
		     h_aSumEJetsEta2step4_3->Fill(aSumE_,totalweight);
		     h_deltaEtaJetsJetsEta2step4_3->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
		     h_deltaPhiJetsJetsEta2step4_3->Fill(deltaphi_,totalweight);
		     h_deltaPtJetsJetsEta2step4_3->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
		     h_dijetMassJetsEta2step4_3->Fill(eventexcl->GetMassDijets(),totalweight);
		     h_pTJet1JetsEta2step4_3->Fill(eventexcl->GetLeadingJetPt(),totalweight);
		     h_pTJet2JetsEta2step4_3->Fill(eventexcl->GetSecondJetPt(),totalweight);
                     h_etaJet1JetsEta2step4_3->Fill(eventexcl->GetLeadingJetEta(),totalweight);
                     h_etaJet2JetsEta2step4_3->Fill(eventexcl->GetSecondJetEta(),totalweight);
                     h_phiJet1JetsEta2step4_3->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
                     h_phiJet2JetsEta2step4_3->Fill(eventexcl->GetSecondJetPhi(),totalweight);
                     h_deltaEtaPFJetsEta2step4_3->Fill(deltaetapf_,totalweight);
                     h_absdeltaEtaPFJetsEta2step4_3->Fill(absdeltaetapf_,totalweight);
                     h_pfetaminJetsEta2step4_3->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
                     h_pfetamaxJetsEta2step4_3->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
                     h_puBx0JetsEta2step4_3->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     h_puBxm1JetsEta2step4_3->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
                     h_puBxp1JetsEta2step4_3->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
                     h_vertexJetsEta2step4_3->Fill(eventexcl->GetNVertex(), totalweight);
                     h_sumEHFplusJetsEta2step4_3->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
                     h_sumEHFminusJetsEta2step4_3->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
                     h_sumEHEplusJetsEta2step4_3->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
                     h_sumEHEminusJetsEta2step4_3->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
                     h_sumEHFpfplusJetsEta2step4_3->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
                     h_sumEHFpfminusJetsEta2step4_3->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);
		  }

		  if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){

                     counterJetsEta2step4_2+=totalweight;

		     h_RJJJetsEta2step4_2->Fill(eventexcl->GetRjjFromJets(),totalweight);
		     h_MultHFJetsEta2step4_2->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
		     h_ET_CalosJetsEta2step4_2->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
		     h_tracksJetsEta2step4_2->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
		     h_aSumEJetsEta2step4_2->Fill(aSumE_,totalweight);
		     h_deltaEtaJetsJetsEta2step4_2->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
		     h_deltaPhiJetsJetsEta2step4_2->Fill(deltaphi_,totalweight);
		     h_deltaPtJetsJetsEta2step4_2->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
		     h_dijetMassJetsEta2step4_2->Fill(eventexcl->GetMassDijets(),totalweight);
		     h_pTJet1JetsEta2step4_2->Fill(eventexcl->GetLeadingJetPt(),totalweight);
		     h_pTJet2JetsEta2step4_2->Fill(eventexcl->GetSecondJetPt(),totalweight);
                     h_etaJet1JetsEta2step4_2->Fill(eventexcl->GetLeadingJetEta(),totalweight);
                     h_etaJet2JetsEta2step4_2->Fill(eventexcl->GetSecondJetEta(),totalweight);
                     h_phiJet1JetsEta2step4_2->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
                     h_phiJet2JetsEta2step4_2->Fill(eventexcl->GetSecondJetPhi(),totalweight);
                     h_deltaEtaPFJetsEta2step4_2->Fill(deltaetapf_,totalweight);
                     h_absdeltaEtaPFJetsEta2step4_2->Fill(absdeltaetapf_,totalweight);
                     h_pfetaminJetsEta2step4_2->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
                     h_pfetamaxJetsEta2step4_2->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
                     h_puBx0JetsEta2step4_2->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     h_puBxm1JetsEta2step4_2->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
                     h_puBxp1JetsEta2step4_2->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
                     h_vertexJetsEta2step4_2->Fill(eventexcl->GetNVertex(), totalweight);
                     h_sumEHFplusJetsEta2step4_2->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
                     h_sumEHFminusJetsEta2step4_2->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
                     h_sumEHEplusJetsEta2step4_2->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
                     h_sumEHEminusJetsEta2step4_2->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
                     h_sumEHFpfplusJetsEta2step4_2->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
                     h_sumEHFpfminusJetsEta2step4_2->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);
		  }

		  if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){

                     counterJetsEta2step4_1+=totalweight;

		     h_RJJJetsEta2step4_1->Fill(eventexcl->GetRjjFromJets(),totalweight);
		     h_MultHFJetsEta2step4_1->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
		     h_ET_CalosJetsEta2step4_1->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),totalweight);
		     h_tracksJetsEta2step4_1->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
		     h_aSumEJetsEta2step4_1->Fill(aSumE_,totalweight);
		     h_deltaEtaJetsJetsEta2step4_1->Fill(eventexcl->GetJetsDeltaEta(),totalweight);
		     h_deltaPhiJetsJetsEta2step4_1->Fill(deltaphi_,totalweight);
		     h_deltaPtJetsJetsEta2step4_1->Fill(eventexcl->GetJetsDeltaPt(),totalweight);
		     h_dijetMassJetsEta2step4_1->Fill(eventexcl->GetMassDijets(),totalweight);
		     h_pTJet1JetsEta2step4_1->Fill(eventexcl->GetLeadingJetPt(),totalweight);
		     h_pTJet2JetsEta2step4_1->Fill(eventexcl->GetSecondJetPt(),totalweight);
                     h_etaJet1JetsEta2step4_1->Fill(eventexcl->GetLeadingJetEta(),totalweight);
                     h_etaJet2JetsEta2step4_1->Fill(eventexcl->GetSecondJetEta(),totalweight);
                     h_phiJet1JetsEta2step4_1->Fill(eventexcl->GetLeadingJetPhi(),totalweight);
                     h_phiJet2JetsEta2step4_1->Fill(eventexcl->GetSecondJetPhi(),totalweight);
                     h_deltaEtaPFJetsEta2step4_1->Fill(deltaetapf_,totalweight);
                     h_absdeltaEtaPFJetsEta2step4_1->Fill(absdeltaetapf_,totalweight);
                     h_pfetaminJetsEta2step4_1->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
                     h_pfetamaxJetsEta2step4_1->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
                     h_puBx0JetsEta2step4_1->Fill(eventexcl->GetNPileUpBx0(),totalweight);
                     h_puBxm1JetsEta2step4_1->Fill(eventexcl->GetNPileUpBxm1(),totalweightbxm1);
                     h_puBxp1JetsEta2step4_1->Fill(eventexcl->GetNPileUpBxp1(),totalweightbxp1);
                     h_vertexJetsEta2step4_1->Fill(eventexcl->GetNVertex(), totalweight);
                     h_sumEHFplusJetsEta2step4_1->Fill(eventdiff->GetSumEnergyHFPlus(), totalweight);
                     h_sumEHFminusJetsEta2step4_1->Fill(eventdiff->GetSumEnergyHFMinus(), totalweight);
                     h_sumEHEplusJetsEta2step4_1->Fill(eventdiff->GetSumEnergyHEPlus(), totalweight);
                     h_sumEHEminusJetsEta2step4_1->Fill(eventdiff->GetSumEnergyHEMinus(), totalweight);
                     h_sumEHFpfplusJetsEta2step4_1->Fill(eventexcl->GetSumEHFPFlowPlus(), totalweight);
                     h_sumEHFpfminusJetsEta2step4_1->Fill(eventexcl->GetSumEHFPFlowMinus(), totalweight);
                  }

	       } // end jets at JetsEta2

	    }// Jets Cuts

	 }// If nVertex

      } // Emule Trigger or Triggers

   }// Run All Events

     outstring << "" << std::endl;
     outstring << "<< INPUTS >>" << std::endl;
     outstring << " " << std::endl;
     outstring << "Input file: " << filein << std::endl;
     outstring << "Output file: " << savehistofile << std::endl;
     outstring << " " << std::endl;
     outstring << "pT(Jet1): " << jet1PT << "GeV" << std::endl;
     outstring << "pT(Jet2): " << jet2PT << "GeV" << std::endl;
     outstring << "# Vertex: " << optnVertex << std::endl;
     outstring << "Trigger Option: " << optTrigger << std::endl;
     outstring << " " << std::endl;
     outstring << "--> TRUE = 1 FALSE = 0" << std::endl;
     outstring << "PU Weight: " << switchWeightPU << std::endl;
     outstring << "Lumi. Weight: " << switchWeightLumi << std::endl;
     outstring << "Eff. Corr.: " << switchWeightEff << std::endl;
     outstring << "Evt. - Evt. Weight: " << switchWeightePw << std::endl;
     outstring << " " << std::endl;
     outstring << "--> Factors" << std::endl;
     outstring << "Lumi. Weight: " << weightlumipass << std::endl;
     outstring << "Trigger Eff.: " << triggereffpass << std::endl;
     outstring << "" << std::endl;
     outstring << "<< EVENT INFO >> " << std::endl;
     outstring << " " << std::endl;
     outstring << "Total # of Events without Weight: " << TotalE << std::endl;
     outstring << " " << std::endl;
     outstring << "Triggered: " << counterTrigger << std::endl;
     outstring << "STEP1: " << counterJetsstep1 << std::endl;
     outstring << "STEP2: " << counterJetsstep2 << std::endl;
     outstring << "STEP3 (CMS Acceptance): " << counterJetsAllstep3 << std::endl;
     outstring << "STEP4_4 (CMS Acceptance): " << counterJetsAllstep4_4 << std::endl;
     outstring << "STEP4_3 (CMS Acceptance): " << counterJetsAllstep4_3 << std::endl;
     outstring << "STEP4_2 (CMS Acceptance): " << counterJetsAllstep4_2 << std::endl;
     outstring << "STEP4_1 (CMS Acceptance): " << counterJetsAllstep4_1 << std::endl;
     outstring << " " << std::endl;
     outstring << "STEP3 (Tracker Acceptance): " << counterJetsTrackerstep3 << std::endl;
     outstring << "STEP4_4 (Tracker Acceptance): " << counterJetsTrackerstep4_4 << std::endl;
     outstring << "STEP4_3 (Tracker Acceptance): " << counterJetsTrackerstep4_3 << std::endl;
     outstring << "STEP4_2 (Tracker Acceptance): " << counterJetsTrackerstep4_2 << std::endl;
     outstring << "STEP4_1 (Tracker Acceptance): " << counterJetsTrackerstep4_1 << std::endl;
     outstring << " " << std::endl;
     outstring << "STEP3 (eta < 2 and eta > -2 Acceptance): " << counterJetsEta2step3 << std::endl;
     outstring << "STEP4_4 (eta < 2 and eta > -2 Acceptance): " << counterJetsEta2step4_4 << std::endl;
     outstring << "STEP4_3 (eta < 2 and eta > -2 Acceptance): " << counterJetsEta2step4_3 << std::endl;
     outstring << "STEP4_2 (eta < 2 and eta > -2 Acceptance): " << counterJetsEta2step4_2 << std::endl;
     outstring << "STEP4_1 (eta < 2 and eta > -2 Acceptance): " << counterJetsEta2step4_1 << std::endl;
     outstring << " " << std::endl;
     outstring << "<< LEGEND >> " << std::endl;
     outstring << "STEP1: # Vertex selection." << std::endl;
     outstring << "STEP2: # Vertex selection + Two Jets." << std::endl;
     outstring << "STEP3: # Vertex selection + Two Jets + Jets in Tracker Acceptance or All CMS." << std::endl;
     outstring << "STEP4_X: # Vertex selection + Two Jets + Jets in Tracker Acceptance or All CMS + Eta_max < X and Eta_min > X." << std::endl;
     outstring << " " << std::endl;

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
   double jet1PT_;
   double jet2PT_;
   int optnVertex_;
   int optTrigger_;
   bool switchWeightPU_;
   bool switchWeightLumi_;
   bool switchWeightEff_;
   bool switchWeightePw_;
   double weightlumipass_;
   double triggereffpass_;

   if (argc > 1 && strcmp(s1,argv[1]) != 0)  filein_ = argv[1];
   if (argc > 2 && strcmp(s1,argv[2]) != 0)  savehistofile_  = argv[2];
   if (argc > 3 && strcmp(s1,argv[3]) != 0)  jet1PT_  = atoi(argv[3]);
   if (argc > 4 && strcmp(s1,argv[4]) != 0)  jet2PT_ = atoi(argv[4]);
   if (argc > 5 && strcmp(s1,argv[5]) != 0)  optnVertex_ = atoi(argv[5]);
   if (argc > 6 && strcmp(s1,argv[6]) != 0)  optTrigger_   = atoi(argv[6]);
   if (argc > 7 && strcmp(s1,argv[7]) != 0)  switchWeightPU_  = atoi(argv[7]);
   if (argc > 8 && strcmp(s1,argv[8]) != 0)  switchWeightLumi_ = atoi(argv[8]);
   if (argc > 9 && strcmp(s1,argv[9]) != 0)  switchWeightEff_ = atoi(argv[9]);
   if (argc > 10 && strcmp(s1,argv[10]) != 0)  switchWeightePw_   = atoi(argv[10]);
   if (argc > 11 && strcmp(s1,argv[11]) != 0)  weightlumipass_  = atof(argv[11]);
   if (argc > 12 && strcmp(s1,argv[12]) != 0)  triggereffpass_ = atof(argv[12]);


   ExclDijetsComp* exclDijets = new ExclDijetsComp();   
   exclDijets->Run(filein_, savehistofile_, jet1PT_, jet2PT_, optnVertex_, optTrigger_, switchWeightPU_, switchWeightLumi_, switchWeightEff_, switchWeightePw_, weightlumipass_, triggereffpass_);

   return 0;
}
#endif
