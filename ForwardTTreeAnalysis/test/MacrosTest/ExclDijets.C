//-------------------------------------------------------------------------------------------------------->>
// UNIVERSIDADE DO ESTADO DO RIO DE JANEIRO - CMS/Brasil
//-------------------------------------------------------------------------------------------------------->>
// Project: Exclusive Dijets Analysis
// Version: v1
//-------------------------------------------------------------------------------------------------------->>
//
// Instructions to run the code: (a) using script ro run multiple parameters or (b) bash command line. 
//
// (A) SCRIPT TO RUN MULTIPLE PARAMETERS
// -------------------------------------
// $> python RunExclAnalysis.py
//
//
// (B) COMMAND LINE
// ----------------
// $> root -l -b -q 'ExclDijets.C("inputfile.root","outputfile.root", pT(Jet1) Cut, pT(Jet2) Cut, Number of Vertex Cut, Trigger Option, Turn on(off) PU Reweight, Turn on(off) Luminosity Reweight, Turn on(off) Trigger Efficiency, Turn on(off) event-per-event Weight, Luminosity Weight Factor, Trigger Efficiency Factor)'
//
// TURN ON  = true
// TURN OFF = false
//
// I)   If you turn off PU reweight, the default weight will be 1;
// II)  If you turn off Luminosity reweight, the default weight will be 1;
// III) If you turn off Trigger Efficiency, the default weight will be 1;
// IV)  If you turn off event-per-event weight (some MC sample), the default weight will be 1;
//
// EXAMPLE: root -l -b -q 'ExclDijets.C("inputfile.root","outputfile.root", 60, 55, 2, 1, false, true, true, 0.0003, 2.3)'
//
// P.S.: DO NOT FORGET to set lines 383 or 384 for choose PU REWEIGHT METHOD: ITweight or weightOOT! You can use eventexcl->GetNPileUpBxm1() or eventexcl->GetNPileUpBxp1()
//       DO NOT FORGET to set lines 434 or 436 for choose SIMULATED TRIGGER OR TRIGGER!
//       

#include <string.h>
#include <stdio.h>
#include <TROOT.h>
#include <TChain.h>
#include <TString.h>
#include "LumiReweightingStandAlone.h"
#include "ExclDijets.h"

using namespace std;

void ExclDijets::LoadFile(string fileinput){

  TFile* inf = NULL;
  TTree* tr  = NULL;
  inf = TFile::Open(fileinput.c_str(),"read");
  tr = (TTree*)inf->Get("forwardQCDTTreeAnalysis/ProcessedTree");
  DiffractiveEvent *eventdiff = new DiffractiveEvent();
  ExclusiveDijetsEvent *eventexcl = new ExclusiveDijetsEvent();
  QCDEvent *eventqcd = new QCDEvent();
  TBranch *diff = tr->GetBranch("DiffractiveAnalysis");
  TBranch *excl = tr->GetBranch("ExclusiveDijetsAnalysis");
  TBranch *qcd = tr->GetBranch("QCDAnalysis");
  diff->SetAddress(&eventdiff);
  excl->SetAddress(&eventexcl);
  qcd->SetAddress(&eventqcd);
  
}


void ExclDijets::ExclDijets(string filein_, string savehistofile_, double jet1PT_, double jet2PT_, int optnVertex_, int optTrigger_, bool switchWeightPU_, bool switchWeightLumi_, bool switchWeightEff_, bool switchWeightePw_, double weightlumipass_, double triggereffpass_){

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

cout << "" << endl;
cout << "< INPUTS >" << endl;
cout << "" << endl;
cout << "filein: " << filein << endl;
cout << "savehistofile: " << savehistofile << endl;
cout << "jet1PT: " << jet1PT << endl;
cout << "jet2PT: " << jet2PT << endl;
cout << "optnVertex: " << optnVertex << endl;
cout << "optTrigger: " << optTrigger << endl;
cout << "switchWeightPU: " << switchWeightPU << endl;
cout << "switchWeightLumi: " << switchWeightLumi << endl;
cout << "switchWeightEff: " << switchWeightEff << endl;
cout << "weightlumipass: " << weightlumipass << endl;
cout << "triggereffpass: " << triggereffpass << endl;
cout << "" << endl;

ExclDijets::LoadFile(filein.c_str());  

using namespace reweight;
LumiReWeighting LumiWeights_;
LumiWeights_ = LumiReWeighting("147146-149711-pileup_2.root ","pileup15to3000_BXs_mc.root","pileup","pileupmcBx0");

if (optnVertex == 0){

cout << "---------------------------------------------------------------" << endl;
cout << "Please, restart your setup. Respect the Condition # Vertex > 0)" << endl;
cout << "---------------------------------------------------------------" << endl;
exit();

}

cout << " " << endl;
cout << "pT(jet1) > " << jet1PT << endl;
cout << "pT(jet2) > " << jet2PT << endl;
cout << " " << endl;

TFile *outf = new TFile(savehistofile.c_str(),"RECREATE");

//int NEVENTS = tr->GetEntriesFast();
int NEVENTS = tr->GetEntries();

TH1::SetDefaultSumw2(true);
TH2::SetDefaultSumw2(true);


// Control Plots A: without cuts
TH1D *h_DeltaEtaGenwc = new TH1D("DeltaEtaGen_without_cuts","#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
TH1D *h_MxGenRangewc = new TH1D("MxGenRange_without_cuts","Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
TH1D *h_RJJwc = new TH1D("RJJ_without_cuts","R_{jj} Distribution; R_{jj}; N events",50,0,1);
TH2F *h_MultHFwc = new TH2F( "mHF_without_cuts", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_Caloswc = new TH2F( "ETCalos_without_cuts", "HF^{+} and Castor; \sum Energy HF^{+}; log_{10} \sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
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
TH1D *h_deltaEtaPFwc = new TH1D("deltaEtaPF_without_cuts","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,-12,12);
TH1D *h_absdeltaEtaPFwc = new TH1D("absdeltaEtaPF_without_cuts","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0.0,12);
//------------>


// ControlPlots B: with trigger
TH1D *h_DeltaEtaGenwt = new TH1D("DeltaEtaGen_with_trigger","#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
TH1D *h_MxGenRangewt = new TH1D("MxGenRange_with_trigger","Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
TH1D *h_RJJwt = new TH1D("RJJ_with_trigger","R_{jj} Distribution; R_{jj}; N events",50,0,1);
TH2F *h_MultHFwt = new TH2F( "mHF_with_trigger", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_Caloswt = new TH2F( "ETCalos_with_trigger", "HF^{+} and Castor; \sum Energy HF^{+}; log_{10} \sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
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
TH1D *h_deltaEtaPFwt = new TH1D("deltaEtaPF_with_trigger","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,-12,12);
TH1D *h_absdeltaEtaPFwt = new TH1D("absdeltaEtaPF_with_trigger","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0.0,12);
//------------>


// STEP1: trigger + Vertex Cut
TH1D *h_DeltaEtaGenstep1 = new TH1D("DeltaEtaGen_step1","#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
TH1D *h_MxGenRangestep1 = new TH1D("MxGenRange_step1","Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
TH1D *h_RJJstep1 = new TH1D("RJJ_step1","R_{jj} Distribution; R_{jj}; N events",50,0,1);
TH2F *h_MultHFstep1 = new TH2F( "mHF_step1", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_Calosstep1 = new TH2F( "ETCalos_step1", "HF^{+} and Castor; \sum Energy HF^{+}; log_{10} \sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
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
TH1D *h_deltaEtaPFstep1 = new TH1D("deltaEtaPF_step1","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,-12,12);
TH1D *h_absdeltaEtaPFstep1 = new TH1D("absdeltaEtaPF_step1","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0.0,12);
//------------>


// STEP2: trigger + Vertex Cut + pT (Jet1 and Jet2) Cut 
TH1D *h_DeltaEtaGenstep2 = new TH1D("DeltaEtaGen_step2","#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
TH1D *h_MxGenRangestep2 = new TH1D("MxGenRange_step2","Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
TH1D *h_RJJstep2 = new TH1D("RJJ_step2","R_{jj} Distribution; R_{jj}; N events",50,0,1);
TH2F *h_MultHFstep2 = new TH2F( "mHF_step2", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_Calosstep2 = new TH2F( "ETCalos_step2", "HF^{+} and Castor; \sum Energy HF^{+}; log_{10} \sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
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
TH1D *h_deltaEtaPFstep2 = new TH1D("deltaEtaPF_step2","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,-12,12);
TH1D *h_absdeltaEtaPFstep2 = new TH1D("absdeltaEtaPF_step2","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0.0,12);
//------------>


// ALL STEP3: trigger + Vertex Cut + pT (Jet1 and Jet2) Cut + Jets All CMS Acceptance 
TH1D *h_DeltaEtaGenAllstep3 = new TH1D("DeltaEtaGenAll_step3","#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
TH1D *h_MxGenRangeAllstep3 = new TH1D("MxGenRangeAll_step3","Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
TH1D *h_RJJAllstep3 = new TH1D("RJJAll_step3","R_{jj} Distribution; R_{jj}; N events",50,0,1);
TH2F *h_MultHFAllstep3 = new TH2F( "mHFAll_step3", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_CalosAllstep3 = new TH2F( "ETCalosAll_step3", "HF^{+} and Castor; \sum Energy HF^{+}; log_{10} \sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
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
TH1D *h_deltaEtaPFAllstep3 = new TH1D("deltaEtaPFAll_step3","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,-12,12);
TH1D *h_absdeltaEtaPFAllstep3 = new TH1D("absdeltaEtaPFAll_step3","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0.0,12);
//------------>

// ALL STEP4-X: trigger + Vertex Cut + pT (Jet1 and Jet2) Cut + Jets All CMS Acceptance + |Eta Max and Eta Min| < X Cut 
// |Eta max and Eta min| < 4
TH1D *h_RJJAllstep4_4 = new TH1D("RJJAll_step4_4","R_{jj} Distribution; R_{jj}; N events",50,0,1);
TH2F *h_MultHFAllstep4_4 = new TH2F( "mHFAll_step4_4", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_CalosAllstep4_4 = new TH2F( "ETCalosAll_step4_4", "HF^{+} and Castor; \sum Energy HF^{+}; log_{10} \sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
TH1D *h_tracksAllstep4_4 = new TH1D("tracksAll_step4_4","Tracks Multiplicity; n Tracks; N events",50,0,150);
TH1D *h_aSumEAllstep4_4 = new TH1D("aEnergyAll_step4_4","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_deltaEtaJetsAllstep4_4 = new TH1D("deltaEtaJetsAll_step4_4","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJetsAllstep4_4 = new TH1D("deltaPhiJetsAll_step4_4","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJetsAllstep4_4 = new TH1D("deltaPtJetsAll_step4_4","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMassAllstep4_4 = new TH1D("dijetMassAll_step4_4","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
TH1D *h_pTJet1Allstep4_4 = new TH1D("pTJet1All_step4_4","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
TH1D *h_pTJet2Allstep4_4 = new TH1D("pTJet2All_step4_4","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);

// |Eta max and Eta min| < 3
TH1D *h_RJJAllstep4_3 = new TH1D("RJJAll_step4_3","R_{jj} Distribution; R_{jj}; N events",50,0,1);
TH2F *h_MultHFAllstep4_3 = new TH2F( "mHFAll_step4_3", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_CalosAllstep4_3 = new TH2F( "ETCalosAll_step4_3", "HF^{+} and Castor; \sum Energy HF^{+}; log_{10} \sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
TH1D *h_tracksAllstep4_3 = new TH1D("tracksAll_step4_3","Tracks Multiplicity; n Tracks; N events",50,0,150);
TH1D *h_aSumEAllstep4_3 = new TH1D("aEnergyAll_step4_3","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_deltaEtaJetsAllstep4_3 = new TH1D("deltaEtaJetsAll_step4_3","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJetsAllstep4_3 = new TH1D("deltaPhiJetsAll_step4_3","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJetsAllstep4_3 = new TH1D("deltaPtJetsAll_step4_3","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMassAllstep4_3 = new TH1D("dijetMassAll_step4_3","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
TH1D *h_pTJet1Allstep4_3 = new TH1D("pTJet1All_step4_3","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
TH1D *h_pTJet2Allstep4_3 = new TH1D("pTJet2All_step4_3","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);

// |Eta max and Eta min| < 2
TH1D *h_RJJAllstep4_2 = new TH1D("RJJAll_step4_2","R_{jj} Distribution; R_{jj}; N events",50,0,1);
TH2F *h_MultHFAllstep4_2 = new TH2F( "mHFAll_step4_2", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_CalosAllstep4_2 = new TH2F( "ETCalosAll_step4_2", "HF^{+} and Castor; \sum Energy HF^{+}; log_{10} \sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
TH1D *h_tracksAllstep4_2 = new TH1D("tracksAll_step4_2","Tracks Multiplicity; n Tracks; N events",50,0,150);
TH1D *h_aSumEAllstep4_2 = new TH1D("aEnergyAll_step4_2","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_deltaEtaJetsAllstep4_2 = new TH1D("deltaEtaJetsAll_step4_2","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJetsAllstep4_2 = new TH1D("deltaPhiJetsAll_step4_2","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJetsAllstep4_2 = new TH1D("deltaPtJetsAll_step4_2","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMassAllstep4_2 = new TH1D("dijetMassAll_step4_2","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
TH1D *h_pTJet1Allstep4_2 = new TH1D("pTJet1All_step4_2","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
TH1D *h_pTJet2Allstep4_2 = new TH1D("pTJet2All_step4_2","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);

// |Eta max and Eta min| < 1
TH1D *h_RJJAllstep4_1 = new TH1D("RJJAll_step4_1","R_{jj} Distribution; R_{jj}; N events",50,0,1);
TH2F *h_MultHFAllstep4_1 = new TH2F( "mHFAll_step4_1", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_CalosAllstep4_1 = new TH2F( "ETCalosAll_step4_1", "HF^{+} and Castor; \sum Energy HF^{+}; log_{10} \sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
TH1D *h_tracksAllstep4_1 = new TH1D("tracksAll_step4_1","Tracks Multiplicity; n Tracks; N events",50,0,150);
TH1D *h_aSumEAllstep4_1 = new TH1D("aEnergyAll_step4_1","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_deltaEtaJetsAllstep4_1 = new TH1D("deltaEtaJetsAll_step4_1","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJetsAllstep4_1 = new TH1D("deltaPhiJetsAll_step4_1","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJetsAllstep4_1 = new TH1D("deltaPtJetsAll_step4_1","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMassAllstep4_1 = new TH1D("dijetMassAll_step4_1","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
TH1D *h_pTJet1Allstep4_1 = new TH1D("pTJet1All_step4_1","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
TH1D *h_pTJet2Allstep4_1 = new TH1D("pTJet2All_step4_1","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);

//------------>

// Tracker STEP3: trigger + Vertex Cut + pT (Jet1 and Jet2) Cut + Jets All CMS Acceptance 
TH1D *h_DeltaEtaGenTrackerstep3 = new TH1D("DeltaEtaGenTracker_step3","#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
TH1D *h_MxGenRangeTrackerstep3 = new TH1D("MxGenRangeTracker_step3","Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
TH1D *h_RJJTrackerstep3 = new TH1D("RJJTracker_step3","R_{jj} Distribution; R_{jj}; N events",50,0,1);
TH2F *h_MultHFTrackerstep3 = new TH2F( "mHFTracker_step3", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_CalosTrackerstep3 = new TH2F( "ETCalosTracker_step3", "HF^{+} and Castor; \sum Energy HF^{+}; log_{10} \sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
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
TH1D *h_deltaEtaPFTrackerstep3 = new TH1D("deltaEtaPFTracker_step3","#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",20,-12,12);
TH1D *h_absdeltaEtaPFTrackerstep3 = new TH1D("absdeltaEtaPFTracker_step3","#Delta#eta_{PF} Distribution; |#eta_{max}-#eta_{min}|; N events",20,0.0,12);
//------------>

// Tracker STEP4-X: trigger + Vertex Cut + pT (Jet1 and Jet2) Cut + Jets at CMS Tracker Acceptance + |Eta Max and Eta Min| < X Cut 
// |Eta max and Eta min| < 4
TH1D *h_RJJTrackerstep4_4 = new TH1D("RJJTracker_step4_4","R_{jj} Distribution; R_{jj}; N events",50,0,1);
TH2F *h_MultHFTrackerstep4_4 = new TH2F( "mHFTracker_step4_4", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_CalosTrackerstep4_4 = new TH2F( "ETCalosTracker_step4_4", "HF^{+} and Castor; \sum Energy HF^{+}; log_{10} \sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
TH1D *h_tracksTrackerstep4_4 = new TH1D("tracksTracker_step4_4","Tracks Multiplicity; n Tracks; N events",50,0,150);
TH1D *h_aSumETrackerstep4_4 = new TH1D("aEnergyTracker_step4_4","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_deltaEtaJetsTrackerstep4_4 = new TH1D("deltaEtaJetsTracker_step4_4","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJetsTrackerstep4_4 = new TH1D("deltaPhiJetsTracker_step4_4","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJetsTrackerstep4_4 = new TH1D("deltaPtJetsTracker_step4_4","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMassTrackerstep4_4 = new TH1D("dijetMassTracker_step4_4","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
TH1D *h_pTJet1Trackerstep4_4 = new TH1D("pTJet1Tracker_step4_4","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
TH1D *h_pTJet2Trackerstep4_4 = new TH1D("pTJet2Tracker_step4_4","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);

// |Eta max and Eta min| < 3
TH1D *h_RJJTrackerstep4_3 = new TH1D("RJJTracker_step4_3","R_{jj} Distribution; R_{jj}; N events",50,0,1);
TH2F *h_MultHFTrackerstep4_3 = new TH2F( "mHFTracker_step4_3", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_CalosTrackerstep4_3 = new TH2F( "ETCalosTracker_step4_3", "HF^{+} and Castor; \sum Energy HF^{+}; log_{10} \sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
TH1D *h_tracksTrackerstep4_3 = new TH1D("tracksTracker_step4_3","Tracks Multiplicity; n Tracks; N events",50,0,150);
TH1D *h_aSumETrackerstep4_3 = new TH1D("aEnergyTracker_step4_3","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_deltaEtaJetsTrackerstep4_3 = new TH1D("deltaEtaJetsTracker_step4_3","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJetsTrackerstep4_3 = new TH1D("deltaPhiJetsTracker_step4_3","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJetsTrackerstep4_3 = new TH1D("deltaPtJetsTracker_step4_3","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMassTrackerstep4_3 = new TH1D("dijetMassTracker_step4_3","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
TH1D *h_pTJet1Trackerstep4_3 = new TH1D("pTJet1Tracker_step4_3","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
TH1D *h_pTJet2Trackerstep4_3 = new TH1D("pTJet2Tracker_step4_3","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);

// |Eta max and Eta min| < 2
TH1D *h_RJJTrackerstep4_2 = new TH1D("RJJTracker_step4_2","R_{jj} Distribution; R_{jj}; N events",50,0,1);
TH2F *h_MultHFTrackerstep4_2 = new TH2F( "mHFTracker_step4_2", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_CalosTrackerstep4_2 = new TH2F( "ETCalosTracker_step4_2", "HF^{+} and Castor; \sum Energy HF^{+}; log_{10} \sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
TH1D *h_tracksTrackerstep4_2 = new TH1D("tracksTracker_step4_2","Tracks Multiplicity; n Tracks; N events",50,0,150);
TH1D *h_aSumETrackerstep4_2 = new TH1D("aEnergyTracker_step4_2","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_deltaEtaJetsTrackerstep4_2 = new TH1D("deltaEtaJetsTracker_step4_2","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJetsTrackerstep4_2 = new TH1D("deltaPhiJetsTracker_step4_2","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJetsTrackerstep4_2 = new TH1D("deltaPtJetsTracker_step4_2","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMassTrackerstep4_2 = new TH1D("dijetMassTracker_step4_2","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
TH1D *h_pTJet1Trackerstep4_2 = new TH1D("pTJet1Tracker_step4_2","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
TH1D *h_pTJet2Trackerstep4_2 = new TH1D("pTJet2Tracker_step4_2","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);

// |Eta max and Eta min| < 1
TH1D *h_RJJTrackerstep4_1 = new TH1D("RJJTracker_step4_1","R_{jj} Distribution; R_{jj}; N events",50,0,1);
TH2F *h_MultHFTrackerstep4_1 = new TH2F( "mHFTracker_step4_1", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_CalosTrackerstep4_1 = new TH2F( "ETCalosTracker_step4_1", "HF^{+} and Castor; \sum Energy HF^{+}; log_{10} \sum Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
TH1D *h_tracksTrackerstep4_1 = new TH1D("tracksTracker_step4_1","Tracks Multiplicity; n Tracks; N events",50,0,150);
TH1D *h_aSumETrackerstep4_1 = new TH1D("aEnergyTracker_step4_1","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_deltaEtaJetsTrackerstep4_1 = new TH1D("deltaEtaJetsTracker_step4_1","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJetsTrackerstep4_1 = new TH1D("deltaPhiJetsTracker_step4_1","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJetsTrackerstep4_1 = new TH1D("deltaPtJetsTracker_step4_1","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMassTrackerstep4_1 = new TH1D("dijetMassTracker_step4_1","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
TH1D *h_pTJet1Trackerstep4_1 = new TH1D("pTJet1Tracker_step4_1","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
TH1D *h_pTJet2Trackerstep4_1 = new TH1D("pTJet2Tracker_step4_1","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);

//------------>

unsigned NEntries = tr->GetEntries();
cout << "" << endl;
cout<< "Reading Tree: "<< NEntries << " events"<<endl;
cout << "" << endl;

int decade = 0;


// Event by Event Analysis
//////////////////////////

int TotalE = 0;
int counterTrigger = 0;
int counterJetsAll = 0;
int counterJetsAtTracker = 0;
int counterJetsstep2 = 0;
int counterJetsAllstep3 = 0;
int counterJetsTrackerstep3 = 0;

double deltaphi_ = 0;
double aSumE_ = 0;
double absdeltaetapf_ = 0;
double deltaetapf_ = 0;

for(unsigned i=0;i<NEVENTS;i++) {
double totalweight = -999;
++TotalE;

    //----------- Progress Report -------------
    double progress = 10.0*i/(1.0*NEVENTS);
    int k = TMath::FloorNint(progress); 
    if (k > decade) 
      cout<<10*k<<" %"<<endl;
    decade = k;          

    //----------- Read the Event --------------
    tr->GetEntry(i);

//
// Define Weights
//

deltaphi_ = fabs(eventexcl->GetLeadingJetPhi()-eventexcl->GetSecondJetPhi());
aSumE_ = (eventdiff->GetSumEnergyHFPlus() - eventdiff->GetSumEnergyHFMinus())/(eventdiff->GetSumEnergyHFPlus() + eventdiff->GetSumEnergyHFMinus());
absdeltaetapf_ = fabs(eventdiff->GetEtaMaxFromPFCands()-eventdiff->GetEtaMinFromPFCands());
deltaetapf_ = (eventdiff->GetEtaMaxFromPFCands()-eventdiff->GetEtaMinFromPFCands());
 
             //------------------------------------------------------------------------------------------
             //
             // SET PILE-UP REWEIGHT METHOD!!!!
             // NOTE: you can use eventexcl->GetNPileUpBxm1() or eventexcl->GetNPileUpBxp1()
             if (switchWeightPU) {double weight = LumiWeights_.weightOOT(eventexcl->GetNPileUpBx0(),eventexcl->GetNPileUpBxm1());} 
//             if (switchWeightPU) { double weight = LumiWeights_.ITweight(eventexcl->GetNPileUpBx0());}
             else {double weight = 1;}
             //------------------------------------------------------------------------------------------

             if (switchWeightLumi) { double weightlumi = weightlumipass; }
             else {double weightlumi = 1;}

             if (switchWeightEff) { double triggereff = triggereffpass;}
             else {double triggereff = 1;}

             if (switchWeightePw) { double weightepw = eventqcd->evtHdr().weight();}
             else {double weightepw = 1;}

            // TEST 
            // cout << weightepw << endl;
            //---------->>

//---------->>
/*
             cout << "BEFORE TRIGGER" << endl;
             cout << "PileUp-1: " << eventexcl->GetNPileUpBxm1() << endl; 
             cout << "PileUp0: " << eventexcl->GetNPileUpBx0() << endl;
             cout << "PileUp+1: " << eventexcl->GetNPileUpBxp1() << endl;
             cout << "==============" << endl;   
*/
//---------->>

            totalweight = triggereff*weight*weightlumi*weightepw;

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
                 h_deltaEtaPFwc->Fill(deltaetapf_,totalweight);
                 h_absdeltaEtaPFwc->Fill(absdeltaetapf_,totalweight);
            //////////////////////////////////////////////////


//------------------------------------------------------------------------------------------
//
// SET TRIGGER OR TRIGGER EMULATOR!!!!
//
//
// SIMULATED TRIGGER
      if (eventdiff->GetSumEnergyHFMinus() < 50 && eventdiff->GetSumEnergyHFPlus() < 50){
// TRIGGER
//    if (eventexcl->GetHLTPath(optTrigger)) {
//
//------------------------------------------------------------------------------------------

    ++counterTrigger;

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
                 h_deltaEtaPFwt->Fill(deltaetapf_,totalweight);
                 h_absdeltaEtaPFwt->Fill(absdeltaetapf_,totalweight);
            //////////////////////////////////////////////////


        if(eventexcl->GetNVertex() > 0 && eventexcl->GetNVertex()<= optnVertex){


//---------->>
        
/*
             cout << "" << endl;
             cout << "PileUp-1: " << eventexcl->GetNPileUpBxm1() << endl;    
             cout << "PileUp0: " << eventexcl->GetNPileUpBx0() << endl;
             cout << "PileUp+1: " << eventexcl->GetNPileUpBxp1() << endl;
             cout << "Weigh["<<i<<"]: " << LumiWeights_.ITweight(eventexcl->GetNPileUpBx0()) << endl;
             cout << "" << endl;
*/

//cout << "" << endl;
//cout << "Weight["<<i<<"]: " << weight << endl;
//cout << "" << endl;

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
                 h_deltaEtaPFstep1->Fill(deltaetapf_,totalweight);
                 h_absdeltaEtaPFstep1->Fill(absdeltaetapf_,totalweight);
            //////////////////////////////////////////////////
          

            // Setting Analysis Cut
            if (eventexcl->GetLeadingJetP4().Pt() > jet1PT && eventexcl->GetSecondJetP4().Pt() > jet2PT ){

                 ++counterJetsstep2;

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
                 h_deltaEtaPFstep2->Fill(deltaetapf_,totalweight);
                 h_absdeltaEtaPFstep2->Fill(absdeltaetapf_,totalweight);
                 //////////////////////////////////////////////////
                
                if (eventexcl->GetLeadingJetP4().Eta() < 5.2 && eventexcl->GetSecondJetP4().Eta() < 5.2 && eventexcl->GetLeadingJetP4().Eta() > -5.2 && eventexcl->GetSecondJetP4().Eta() > -5.2){

                 ++counterJetsAllstep3;

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
                 h_deltaEtaPFAllstep3->Fill(deltaetapf_,totalweight);
                 h_absdeltaEtaPFAllstep3->Fill(absdeltaetapf_,totalweight);

                      // Eta max and Eta min cut
                      if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){
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
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){
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
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){
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
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){
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
                      }

                 }  
                 // end Jets at all CMS Acceptance

                if (eventexcl->GetLeadingJetP4().Eta() < 2.9 && eventexcl->GetSecondJetP4().Eta() < 2.9 && eventexcl->GetLeadingJetP4().Eta() > -2.9 && eventexcl->GetSecondJetP4().Eta() > -2.9){

                 ++counterJetsTrackerstep3;

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

                      // Eta max and Eta min cut
                      if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){
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
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){
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
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){
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
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){
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
                      }
 
                } // end jets at tracker

           }// Jets Cuts

     }// If nVertex

   } // Emule Trigger or Triggers

  }// Run All Events

 cout << "" << endl;
 cout << "Trigger Info" << endl;
 cout << "------------" << endl;
 cout << "" << endl;
 cout << "Total # of Events: " << TotalE << endl;
 cout << "Triggered: " << counterTrigger << endl;
 cout << "Selected " << counterJetsstep2 <<  " jets with pT(jet1) > " << jet1PT << " [GeV] " << "and pT(jet2) > " << jet2PT << " [GeV]." << endl;
 cout << "Selected " << counterJetsAllstep3 <<  " jets with pT(jet1) > " << jet1PT << " [GeV] " << "and pT(jet2) > " << jet2PT << " [GeV] in all CMS acceptance."<< endl;
 cout << "Selected " << counterJetsTrackerstep3 <<  " jets with pT(jet1) > " << jet1PT << " [GeV] " << "and pT(jet2) > " << jet2PT << " [GeV] in tracker acceptance."<< endl;
 cout << "" << endl;

outf->Write();
outf->Close();

~LumiWeights_;
gROOT->Reset();

}


