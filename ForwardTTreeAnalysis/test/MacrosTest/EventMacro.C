// State University of Rio de Janeiro - UERJ
// Authors: exclusive dijets subgroup
// contact: dmf@cern.ch
//
//------------------------------------------
// Version: 1.0
// 
// How Execute: root -l EventMacro.C
// 
// Functions: LoadFiles(i) - load a specific file (i) and run your analysis with your cuts defined in RunExclusive 
//            RunExclusive( pT[jet1], pT[jet2], Number of Vertex, Trigger Reference, switchWeightPU_, weight, triggereff)
//            
//            pT[jet1] = Jet1 pT cut;
//            pT[jet2] = Jet2 pT cut;
//            optnVertex = Selecting Events with number of vertex < optnVertex;
//            optTrigger = Trigger option (0,1,2...). Same order defined in your PF2PAT(...).py or forwardQCD(...).py which generate your Pattuple. See hltpath variable.
//            switchWeightPU_ = true (if is running data). This apply weightpu = 1, false (if is running mc);
//            weight = luminosity weight (L_data/L_MC) for MC;
//            triggereff = trigger efficiency;
//
// REMEMBER TO UNCOMMENT TRIGGER IN CASE YOU ARE RUNNING DATA AND UNCOMMENT TRIGGER EMULATION FOR MONTECARLO.
//
//


#include <string.h>
#include <stdio.h>
#include "LumiReweightingStandAlone.h"

//For 2010 Data: normalize MC 
//#include "EventMacro2010.h"
//#include "EventMacroMCDiff2010.h"

//For 2011 Data: normalize MC
//#include "EventMacro2011.h"

//For Data: 
//#include "EventMacro.h"
//#include "EventMacroData2010.h"
#include "EventMacroData2011.h"

//TEST
//#include "EventMacroTest.h"



// Castor in fC. log10(fabs(eventdiff->GetSumETotCastor())) Using fabs because there are some signals with -fC.

using namespace std;

void RunExclusive(string savehistofile, double jet1PT, double jet2PT, int optnVertex, int optTrigger, bool switchWeightPU_, bool switchWeightLumi_, bool switchWeightEff_, double weightlumi_, double triggereff_){

using namespace reweight;
LumiReWeighting LumiWeights_;
LumiWeights_ = LumiReWeighting("pu2010B.root","pileupmc.root","pileup","pileupmc");


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

// Settings
///////////

//char fileout[250];
//sprintf(fileout,"exclusive_pTJ1-%d_pTJ2-%d_nV-%d_Trigger-%d.root",jet1PT,jet2PT,optnVertex,optTrigger);

TFile *outf = new TFile(savehistofile.c_str(),"RECREATE");
int NEVENTS = tr->GetEntriesFast();

TH1::SetDefaultSumw2(true);
TH2::SetDefaultSumw2(true);

TH1D *h_DeltaEtaGenwc = new TH1D("DeltaEtaGenwc","#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
TH1D *h_MxGenRangewc = new TH1D("MxGenRangewc","Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
TH1D *h_RJJwc = new TH1D("RJJwc","R_{jj} Distribution; R_{jj}; N events",50,0,1);
TH2F *h_MultHFwc = new TH2F( "mHFwc", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_Caloswc = new TH2F( "ETCaloswc", "HF^{+} and Castor; Energy HF^{+}; log_{10} Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
TH1D *h_trackswc = new TH1D("trackswc","Tracks Multiplicity; n Tracks; N events",50,0,150);
TH1D *h_pfetamaxwc = new TH1D("pfetamaxwc","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
TH1D *h_pfetaminwc = new TH1D("pfetaminwc","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
TH1D *h_aSumEwc = new TH1D("aEnergywc","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_deltaEtaJetswc = new TH1D("deltaEtaJetswc","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJetswc = new TH1D("deltaPhiJetswc","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJetswc = new TH1D("deltaPtJetswc","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMasswc = new TH1D("dijetMasswc","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
TH1D *h_pTJet1wc = new TH1D("pTJet1wc","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
TH1D *h_pTJet2wc = new TH1D("pTJet2wc","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);

TH1D *h_DeltaEtaGenAll = new TH1D("DeltaEtaGenAll","#Delta#eta_{Gen} Distribution; #Delta#eta_{Gen}; N events",20,0.0,5.2);
TH1D *h_MxGenRangeAll = new TH1D("MxGenRangeAll","Mass_{X} Gen Distribution; M_{x} Gen [GeV]; N events",50,0,5000);
TH1D *h_RJJAll = new TH1D("RJJAll","R_{jj} Distribution; R_{jj}; N events",50,0,1);
TH2F *h_MultHFAll = new TH2F( "mHFAll", "HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_CalosAll = new TH2F( "ETCalosAll", "HF^{+} and Castor; Energy HF^{+}; log_{10} Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
TH1D *h_tracksAll = new TH1D("tracksAll","Tracks Multiplicity; n Tracks; N events",50,0,150);
TH1D *h_pfetamaxAll = new TH1D("pfetamaxAll","Particle Flow #eta_{max} Distribution; #eta; N events",20,0,5.5);
TH1D *h_pfetaminAll = new TH1D("pfetaminAll","Particle Flow #eta_{min} Distribution; #eta; N events",20,-5.5,0);
TH1D *h_aSumEAll = new TH1D("aEnergyAll","Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_deltaEtaJetsAll = new TH1D("deltaEtaJetsAll","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJetsAll = new TH1D("deltaPhiJetsAll","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJetsAll = new TH1D("deltaPtJetsAll","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMassAll = new TH1D("dijetMassAll","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);
TH1D *h_pTJet1 = new TH1D("pTJet1All","Leading Jet - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);
TH1D *h_pTJet2 = new TH1D("pTJet2All","Second Jet  - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",100,0,2000);

TH1D *h_RJJ = new TH1D("RJJ","R_{jj} Distribution - Jets in |#eta| < 5.2 ; R_{jj}; N events",50,0,1);
TH1D *h_RJJetsAtTracker = new TH1D("RJJetsAtTracker","R_{jj} Distribution - Jets in |#eta| < 2.9; R_{jj}; N events",50,0,1);

TH1D *h_deltaEtaJets = new TH1D("deltaEtaJets","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJets = new TH1D("deltaPhiJets","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJets = new TH1D("deltaPtJets","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMass = new TH1D("dijetMass","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);

TH1D *h_deltaEtaJets4 = new TH1D("deltaEtaJets4","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJets4 = new TH1D("deltaPhiJets4","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJets4 = new TH1D("deltaPtJets4","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMass4 = new TH1D("dijetMass4","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);

TH1D *h_deltaEtaJets3 = new TH1D("deltaEtaJets3","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJets3 = new TH1D("deltaPhiJets3","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJets3 = new TH1D("deltaPtJets3","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMass3 = new TH1D("dijetMass3","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);

TH1D *h_deltaEtaJets2 = new TH1D("deltaEtaJets2","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJets2 = new TH1D("deltaPhiJets2","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJets2 = new TH1D("deltaPtJets2","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMass2 = new TH1D("dijetMass2","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);

TH1D *h_deltaEtaJets1 = new TH1D("deltaEtaJets1","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJets1 = new TH1D("deltaPhiJets1","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJets1 = new TH1D("deltaPtJets1","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMass1 = new TH1D("dijetMass1","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);

TH1D *h_deltaEtaJetsAtTracker = new TH1D("deltaEtaJetsAtTracker","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJetsAtTracker = new TH1D("deltaPhiJetsAtTracker","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJetsAtTracker = new TH1D("deltaPtJetsAtTracker","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMassAtTracker = new TH1D("dijetMassAtTracker","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);

TH1D *h_deltaEtaJetsAtTracker4 = new TH1D("deltaEtaJetsAtTracker4","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJetsAtTracker4 = new TH1D("deltaPhiJetsAtTracker4","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJetsAtTracker4 = new TH1D("deltaPtJetsAtTracker4","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMassAtTracker4 = new TH1D("dijetMassAtTracker4","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);

TH1D *h_deltaEtaJetsAtTracker3 = new TH1D("deltaEtaJetsAtTracker3","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJetsAtTracker3 = new TH1D("deltaPhiJetsAtTracker3","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJetsAtTracker3 = new TH1D("deltaPtJetsAtTracker3","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMassAtTracker3 = new TH1D("dijetMassAtTracker3","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);

TH1D *h_deltaEtaJetsAtTracker2 = new TH1D("deltaEtaJetsAtTracker2","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJetsAtTracker2 = new TH1D("deltaPhiJetsAtTracker2","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJetsAtTracker2 = new TH1D("deltaPtJetsAtTracker2","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMassAtTracker2 = new TH1D("dijetMassAtTracker2","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);

TH1D *h_deltaEtaJetsAtTracker1 = new TH1D("deltaEtaJets1AtTracker1","#Delta#eta_{jj} Distribution; #Delta#eta_{jj}; N events",20,0.0,5.2);
TH1D *h_deltaPhiJetsAtTracker1 = new TH1D("deltaPhiJets1AtTracker1","#Delta#phi_{jj} Distribution; #Delta#phi_{jj}; N events",20,0.0,3.2);
TH1D *h_deltaPtJetsAtTracker1 = new TH1D("deltaPtJetsAtTracker1","#Delta P_{T} Distribution; #Delta P_{T} [GeV.c^{-1}]; N events",40,0,400);
TH1D *h_dijetMassAtTracker1 = new TH1D("dijetMassAtTracker1","Dijet Invariant Mass Distribution; M_{jj} [GeV]; N events",40,0,400);

TH1D *h_aSumE = new TH1D("aEnergy","Forward Backward Asymmetry Distribution - Jets in |#eta| < 5.2; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_aSumEJetsAtTracker = new TH1D("aEnergyJetsAtTracker","Forward Backward Asymmetry Distribution - Jets in |#eta| < 2.9; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_tracks = new TH1D("tracks","Tracks Multiplicity - Jets in |#eta| < 5.2 ; n Tracks; N events",50,0,150);
TH1D *h_tracksJetsAtTracker = new TH1D("tracksJetsAtTracker","Tracks Multiplicity - Jets in |#eta| < 2.9; nTracks; N events",50,0,150);
TH2F *h_MultHF = new TH2F( "mHF", "HF^{+} and HF^{-} Multiplicity: jets in |#eta| < 5.2; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_MultHFJetsAtTracker = new TH2F( "mHFJetsAtTracker", "HF^{+} and HF^{-} Multiplicity: jets in |#eta| < 2.9; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_Calos = new TH2F( "ETCalos", "HF^{+} and Castor: jets in |#eta| < 5.2; Energy HF^{+}; log_{10} Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
TH2F *h_ET_CalosJetsAtTracker = new TH2F( "ETCalosJetsAtTracker", "HF^{+} and Castor: jets in |#eta| < 2.9; Energy HF^{+}; log_{10} Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
TH1D *h_pfetamax = new TH1D("pfetamax","Particle Flow #eta_{max} Distribution -  jets in |#eta| < 5.2; #eta; N events",20,0,5.5);
TH1D *h_pfetamin = new TH1D("pfetamin","Particle Flow #eta_{min} Distribution -  jets in |#eta| < 5.2; #eta; N events",20,-5.5,0);
TH1D *h_pfetamaxJetsAtTracker = new TH1D("pfetamaxJetsAtTracker","Particle Flow #eta_{max} Distribution - jets in |#eta| < 2.9; #eta; N events",20,0,5.5);
TH1D *h_pfetaminJetsAtTracker = new TH1D("pfetaminJetsAtTracker","Particle Flow #eta_{min} Distribution - jets in |#eta| < 2.9; #eta; N events",20,-5.5,0);

TH1D *h_RJJ4 = new TH1D("RJJ4","R_{jj} Distribution - Jets in |#eta| < 5.2 ; R_{jj}; N events",50,0,1);
TH1D *h_RJJetsAtTracker4 = new TH1D("RJJetsAtTracker4","R_{jj} Distribution - Jets in |#eta| < 2.9; R_{jj}; N events",50,0,1);
TH1D *h_aSumE4 = new TH1D("aEnergy4","Forward Backward Asymmetry Distribution - Jets in |#eta| < 5.2; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_aSumEJetsAtTracker4 = new TH1D("aEnergyJetsAtTracker4","Forward Backward Asymmetry Distribution - Jets in |#eta| < 2.9; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_tracks4 = new TH1D("tracks4","Tracks Multiplicity - Jets in |#eta| < 5.2 ; n Tracks; N events",50,0,150);
TH1D *h_tracksJetsAtTracker4 = new TH1D("tracksJetsAtTracker4","Tracks Multiplicity - Jets in |#eta| < 2.9; nTracks; N events",50,0,150);
TH2F *h_MultHF4 = new TH2F( "mHF4", "HF^{+} and HF^{-} Multiplicity: jets in |#eta| < 5.2; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_MultHFJetsAtTracker4 = new TH2F( "mHFJetsAtTracker4", "HF^{+} and HF^{-} Multiplicity: jets in |#eta| < 2.9; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_Calos4 = new TH2F( "ETCalos4", "HF^{+} and Castor: jets in |#eta| < 5.2; Energy HF^{+}; log_{10} Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
TH2F *h_ET_CalosJetsAtTracker4 = new TH2F( "ETCalosJetsAtTracker4", "HF^{+} and Castor: jets in |#eta| < 2.9; Energy HF^{+}; log_{10} Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );

//
TH1D *h_RJJ3 = new TH1D("RJJ3","R_{jj} Distribution - Jets in |#eta| < 5.2 ; R_{jj}; N events",50,0,1);
TH1D *h_RJJetsAtTracker3 = new TH1D("RJJetsAtTracker3","R_{jj} Distribution - Jets in |#eta| < 2.9; R_{jj}; N events",50,0,1);
TH1D *h_aSumE3 = new TH1D("aEnergy3","Forward Backward Asymmetry Distribution - Jets in |#eta| < 5.2; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_aSumEJetsAtTracker3 = new TH1D("aEnergyJetsAtTracker3","Forward Backward Asymmetry Distribution - Jets in |#eta| < 2.9; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_tracks3 = new TH1D("tracks3","Tracks Multiplicity - Jets in |#eta| < 5.2 ; n Tracks; N events",50,0,150);
TH1D *h_tracksJetsAtTracker3 = new TH1D("tracksJetsAtTracker3","Tracks Multiplicity - Jets in |#eta| < 2.9; nTracks; N events",50,0,150);
TH2F *h_MultHF3 = new TH2F( "mHF3", "HF^{+} and HF^{-} Multiplicity: jets in |#eta| < 5.2; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_MultHFJetsAtTracker3 = new TH2F( "mHFJetsAtTracker3", "HF^{+} and HF^{-} Multiplicity: jets in |#eta| < 2.9; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_Calos3 = new TH2F( "ETCalos3", "HF^{+} and Castor: jets in |#eta| < 5.2; Energy HF^{+}; log_{10} Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );
TH2F *h_ET_CalosJetsAtTracker3 = new TH2F( "ETCalosJetsAtTracker3", "HF^{+} and Castor: jets in |#eta| < 2.9; Energy HF^{+}; log_{10} Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );

//
TH1D *h_RJJ2 = new TH1D("RJJ2","R_{jj} Distribution - Jets in |#eta| < 5.2 ; R_{jj}; N events",50,0,1);
TH1D *h_RJJetsAtTracker2 = new TH1D("RJJetsAtTracker2","R_{jj} Distribution - Jets in |#eta| < 2.9; R_{jj}; N events",50,0,1);
TH1D *h_aSumE2 = new TH1D("aEnergy2","Forward Backward Asymmetry Distribution - Jets in |#eta| < 5.2; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_aSumEJetsAtTracker2 = new TH1D("aEnergyJetsAtTracker2","Forward Backward Asymmetry Distribution - Jets in |#eta| < 2.9; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_tracks2 = new TH1D("tracks2","Tracks Multiplicity - Jets in |#eta| < 5.2 ; n Tracks; N events",50,0,150);
TH1D *h_tracksJetsAtTracker2 = new TH1D("tracksJetsAtTracker2","Tracks Multiplicity - Jets in |#eta| < 2.9; nTracks; N events",50,0,150);
TH2F *h_MultHF2 = new TH2F( "mHF2", "HF^{+} and HF^{-} Multiplicity: jets in |#eta| < 5.2; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_MultHFJetsAtTracker2 = new TH2F( "mHFJetsAtTracker2", "HF^{+} and HF^{-} Multiplicity: jets in |#eta| < 2.9; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_Calos2 = new TH2F( "ETCalos2", "HF^{+} and Castor: jets in |#eta| < 5.2; Energy HF^{+}; log_{10} Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50.);
TH2F *h_ET_CalosJetsAtTracker2 = new TH2F( "ETCalosJetsAtTracker2", "HF^{+} and Castor: jets in |#eta| < 2.9; Energy HF^{+}; log_{10} Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );

//
TH1D *h_RJJ1 = new TH1D("RJJ1","R_{jj} Distribution - Jets in |#eta| < 5.2 ; R_{jj}; N events",50,0,1);
TH1D *h_RJJetsAtTracker1 = new TH1D("RJJetsAtTracker1","R_{jj} Distribution - Jets in |#eta| < 2.9; R_{jj}; N events",50,0,1);
TH1D *h_aSumE1 = new TH1D("aEnergy1","Forward Backward Asymmetry Distribution - Jets in |#eta| < 5.2; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_aSumEJetsAtTracker1 = new TH1D("aEnergyJetsAtTracker1","Forward Backward Asymmetry Distribution - Jets in |#eta| < 2.9; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);
TH1D *h_tracks1 = new TH1D("tracks1","Tracks Multiplicity - Jets in |#eta| < 5.2 ; n Tracks; N events",50,0,150);
TH1D *h_tracksJetsAtTracker1 = new TH1D("tracksJetsAtTracker1","Tracks Multiplicity - Jets in |#eta| < 2.9; nTracks; N events",50,0,150);
TH2F *h_MultHF1 = new TH2F( "mHF1", "HF^{+} and HF^{-} Multiplicity: jets in |#eta| < 5.2; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_MultHFJetsAtTracker1 = new TH2F( "mHFJetsAtTracker1", "HF^{+} and HF^{-} Multiplicity: jets in |#eta| < 2.9; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );
TH2F *h_ET_Calos1 = new TH2F( "ETCalos1", "HF^{+} and Castor: jets in |#eta| < 5.2; Energy HF^{+}; log_{10} Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50.);
TH2F *h_ET_CalosJetsAtTracker1 = new TH2F( "ETCalosJetsAtTracker1", "HF^{+} and Castor: jets in |#eta| < 2.9; Energy HF^{+}; log_{10} Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50.);


// Tree Reading 
unsigned NEntries = tr->GetEntries();
cout<< "Reading Tree: "<< NEntries << " events"<<endl;
int decade = 0;


// Event by Event Analysis
//////////////////////////

int TotalE = 0;
int counterTrigger = 0;
int counterJetsAll = 0;
int counterJetsAtTracker = 0;
double deltaphi_ = 0;
double aSumE_ = 0;


for(unsigned i=0;i<NEVENTS;i++) {
++TotalE;

    //----------- Progress Report -------------
    double progress = 10.0*i/(1.0*NEVENTS);
    int k = TMath::FloorNint(progress); 
    if (k > decade) 
      cout<<10*k<<" %"<<endl;
    decade = k;          

    //----------- Read the Event --------------
    tr->GetEntry(i);

/*
             cout << "BEFORE TRIGGER" << endl;
             cout << "PileUp-1: " << eventexcl->GetNPileUpBxm1() << endl; 
             cout << "PileUp0: " << eventexcl->GetNPileUpBx0() << endl;
             cout << "PileUp+1: " << eventexcl->GetNPileUpBxp1() << endl;
             cout << "==============" << endl;   
*/


// Emule Trigger
////////////////
if (eventdiff->GetSumEnergyHFMinus() < 5 && eventdiff->GetSumEnergyHFPlus() < 5){

// TRIGGER
///////////////
//    if (eventexcl->GetHLTPath(optTrigger)) {
    ++counterTrigger;
    
deltaphi_ = fabs(eventexcl->GetLeadingJetPhi()-eventexcl->GetSecondJetPhi());
aSumE_ = (eventdiff->GetSumEnergyHFPlus() - eventdiff->GetSumEnergyHFMinus())/(eventdiff->GetSumEnergyHFPlus() + eventdiff->GetSumEnergyHFMinus());

             if (switchWeightPU_) { double weight = LumiWeights_.ITweight(eventexcl->GetNPileUpBx0());}
             else {double weight = 1;}

             if (switchWeightLumi_) { double weightlumi = weightlumi_; }
             else {double weightlumi = 1;}

             if (switchWeightEff_) { double triggereff = triggereff_;}
             else {double triggereff = 1;}


            // Control Histograms          
            ////////////////////////////////////////////////
                 h_DeltaEtaGenwc->Fill(eventexcl->GetDeltaEtaGen(),triggereff*weight*weightlumi);
                 h_MxGenRangewc->Fill(eventexcl->GetMxGenRange(),triggereff*weight*weightlumi);
                 h_RJJwc->Fill(eventexcl->GetRjjFromJets(),triggereff*weight*weightlumi);
                 h_MultHFwc->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),triggereff*weight*weightlumi);
                 h_ET_Caloswc->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),triggereff*weight*weightlumi);
                 h_trackswc->Fill(eventdiff->GetMultiplicityTracks(),triggereff*weight*weightlumi);
                 h_aSumEwc->Fill(aSumE_,triggereff*weight*weightlumi);
                 h_pfetaminwc->Fill(eventdiff->GetEtaMinFromPFCands(),triggereff*weight*weightlumi);
                 h_pfetamaxwc->Fill(eventdiff->GetEtaMaxFromPFCands(),triggereff*weight*weightlumi);
                 h_deltaEtaJetswc->Fill(eventexcl->GetJetsDeltaEta(),triggereff*weight*weightlumi);
                 h_deltaPhiJetswc->Fill(deltaphi_,triggereff*weight*weightlumi);
                 h_deltaPtJetswc->Fill(eventexcl->GetJetsDeltaPt(),triggereff*weight*weightlumi);
                 h_dijetMasswc->Fill(eventexcl->GetMassDijets(),triggereff*weight*weightlumi);
                 h_pTJet1wc->Fill(eventexcl->GetLeadingJetPt(),triggereff*weight*weightlumi);
                 h_pTJet2wc->Fill(eventexcl->GetSecondJetPt(),triggereff*weight*weightlumi);
            //////////////////////////////////////////////////


        if(eventexcl->GetNVertex() > 0 && eventexcl->GetNVertex()<= optnVertex){
        
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

            // Vertex Cut Histograms          
            ////////////////////////////////////////////////
                 h_DeltaEtaGenAll->Fill(eventexcl->GetDeltaEtaGen(),triggereff*weight*weightlumi);
                 h_MxGenRangeAll->Fill(eventexcl->GetMxGenRange(),triggereff*weight*weightlumi);
                 h_RJJAll->Fill(eventexcl->GetRjjFromJets(),triggereff*weight*weightlumi);
                 h_MultHFAll->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),triggereff*weight*weightlumi);
                 h_ET_CalosAll->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),triggereff*weight*weightlumi);
                 h_tracksAll->Fill(eventdiff->GetMultiplicityTracks(),triggereff*weight*weightlumi);
                 h_aSumEAll->Fill(aSumE_,triggereff*weight*weightlumi);
                 h_pfetaminAll->Fill(eventdiff->GetEtaMinFromPFCands(),triggereff*weight*weightlumi);
                 h_pfetamaxAll->Fill(eventdiff->GetEtaMaxFromPFCands(),triggereff*weight*weightlumi);
                 h_deltaEtaJetsAll->Fill(eventexcl->GetJetsDeltaEta(),triggereff*weight*weightlumi);
                 h_deltaPhiJetsAll->Fill(deltaphi_,triggereff*weight*weightlumi);
                 h_deltaPtJetsAll->Fill(eventexcl->GetJetsDeltaPt(),triggereff*weight*weightlumi);
                 h_dijetMassAll->Fill(eventexcl->GetMassDijets(),triggereff*weight*weightlumi);
                 h_pTJet1->Fill(eventexcl->GetLeadingJetPt(),triggereff*weight*weightlumi);
                 h_pTJet2->Fill(eventexcl->GetSecondJetPt(),triggereff*weight*weightlumi);
            //////////////////////////////////////////////////
          

 // Setting Analysis Cut
            if (eventexcl->GetLeadingJetP4().Pt() > jet1PT && eventexcl->GetSecondJetP4().Pt() > jet2PT ){
                
                if (eventexcl->GetLeadingJetP4().Eta() < 5.2 && eventexcl->GetSecondJetP4().Eta() < 5.2 && eventexcl->GetLeadingJetP4().Eta() > -5.2 && eventexcl->GetSecondJetP4().Eta() > -5.2){

                 ++counterJetsAll;
                 h_RJJ->Fill(eventexcl->GetRjjFromJets(),triggereff*weight*weightlumi);
                 h_MultHF->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),triggereff*weight*weightlumi);
                 h_ET_Calos->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),triggereff*weight*weightlumi);
                 h_tracks->Fill(eventdiff->GetMultiplicityTracks(),triggereff*weight*weightlumi);
                 h_aSumE->Fill(aSumE_,triggereff*weight*weightlumi);
                 h_pfetamin->Fill(eventdiff->GetEtaMinFromPFCands(),triggereff*weight*weightlumi);
                 h_pfetamax->Fill(eventdiff->GetEtaMaxFromPFCands(),triggereff*weight*weightlumi);
                 h_deltaEtaJets->Fill(eventexcl->GetJetsDeltaEta(),triggereff*weight*weightlumi);
                 h_deltaPhiJets->Fill(deltaphi_,triggereff*weight*weightlumi);
                 h_deltaPtJets->Fill(eventexcl->GetJetsDeltaPt(),triggereff*weight*weightlumi);
                 h_dijetMass->Fill(eventexcl->GetMassDijets(),triggereff*weight*weightlumi);


                      if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){
                      h_RJJ4->Fill(eventexcl->GetRjjFromJets(),triggereff*weight*weightlumi);
                      h_MultHF4->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),triggereff*weight*weightlumi);
                      h_ET_Calos4->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),triggereff*weight*weightlumi);
                      h_tracks4->Fill(eventdiff->GetMultiplicityTracks(),triggereff*weight*weightlumi);
                      h_aSumE4->Fill(aSumE_,triggereff*weight*weightlumi);
                      h_deltaEtaJets4->Fill(eventexcl->GetJetsDeltaEta(),triggereff*weight*weightlumi);
                      h_deltaPhiJets4->Fill(deltaphi_,triggereff*weight*weightlumi);
                      h_deltaPtJets4->Fill(eventexcl->GetJetsDeltaPt(),triggereff*weight*weightlumi);
                      h_dijetMass4->Fill(eventexcl->GetMassDijets(),triggereff*weight*weightlumi);
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){
                      h_RJJ3->Fill(eventexcl->GetRjjFromJets(),triggereff*weight*weightlumi);
                      h_MultHF3->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),triggereff*weight*weightlumi);
                      h_ET_Calos3->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),triggereff*weight*weightlumi);
                      h_tracks3->Fill(eventdiff->GetMultiplicityTracks(),triggereff*weight*weightlumi);
                      h_aSumE3->Fill(aSumE_,triggereff*weight*weightlumi);
                      h_deltaEtaJets3->Fill(eventexcl->GetJetsDeltaEta(),triggereff*weight*weightlumi);
                      h_deltaPhiJets3->Fill(deltaphi_,triggereff*weight*weightlumi);
                      h_deltaPtJets3->Fill(eventexcl->GetJetsDeltaPt(),triggereff*weight*weightlumi);
                      h_dijetMass3->Fill(eventexcl->GetMassDijets(),triggereff*weight*weightlumi);
                      
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){
                      h_RJJ2->Fill(eventexcl->GetRjjFromJets(),triggereff*weight*weightlumi);
                      h_MultHF2->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),triggereff*weight*weightlumi);
                      h_ET_Calos2->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),triggereff*weight*weightlumi);
                      h_tracks2->Fill(eventdiff->GetMultiplicityTracks(),triggereff*weight*weightlumi);
                      h_aSumE2->Fill(aSumE_,triggereff*weight*weightlumi);
                      h_deltaEtaJets2->Fill(eventexcl->GetJetsDeltaEta(),triggereff*weight*weightlumi);
                      h_deltaPhiJets2->Fill(deltaphi_,triggereff*weight*weightlumi);
                      h_deltaPtJets2->Fill(eventexcl->GetJetsDeltaPt(),triggereff*weight*weightlumi);
                      h_dijetMass2->Fill(eventexcl->GetMassDijets(),triggereff*weight*weightlumi);
                      
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){
                      h_RJJ1->Fill(eventexcl->GetRjjFromJets(),triggereff*weight*weightlumi);
                      h_MultHF1->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),triggereff*weight*weightlumi);
                      h_ET_Calos1->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),triggereff*weight*weightlumi);
                      h_tracks1->Fill(eventdiff->GetMultiplicityTracks(),triggereff*weight*weightlumi);
                      h_aSumE1->Fill(aSumE_,triggereff*weight*weightlumi);
                      h_deltaEtaJets1->Fill(eventexcl->GetJetsDeltaEta(),triggereff*weight*weightlumi);
                      h_deltaPhiJets1->Fill(deltaphi_,triggereff*weight*weightlumi);
                      h_deltaPtJets1->Fill(eventexcl->GetJetsDeltaPt(),triggereff*weight*weightlumi);
                      h_dijetMass1->Fill(eventexcl->GetMassDijets(),triggereff*weight*weightlumi);
                      
                      }

                 }  

                if (eventexcl->GetLeadingJetP4().Eta() < 2.9 && eventexcl->GetSecondJetP4().Eta() < 2.9 && eventexcl->GetLeadingJetP4().Eta() > -2.9 && eventexcl->GetSecondJetP4().Eta() > -2.9){

                ++counterJetsAtTracker;
                h_RJJetsAtTracker->Fill(eventexcl->GetRjjFromJets(),triggereff*weight*weightlumi);
                h_MultHFJetsAtTracker->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),triggereff*weight*weightlumi);
                h_ET_CalosJetsAtTracker->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),triggereff*weight*weightlumi);
                h_tracksJetsAtTracker->Fill(eventdiff->GetMultiplicityTracks(),triggereff*weight*weightlumi);
                h_aSumEJetsAtTracker->Fill(aSumE_,triggereff*weight*weightlumi);
                h_pfetaminJetsAtTracker->Fill(eventdiff->GetEtaMinFromPFCands(),triggereff*weight*weightlumi);
                h_pfetamaxJetsAtTracker->Fill(eventdiff->GetEtaMaxFromPFCands(),triggereff*weight*weightlumi);
                h_deltaEtaJetsAtTracker->Fill(eventexcl->GetJetsDeltaEta(),triggereff*weight*weightlumi);
                h_deltaPhiJetsAtTracker->Fill(deltaphi_,triggereff*weight*weightlumi);
                h_deltaPtJetsAtTracker->Fill(eventexcl->GetJetsDeltaPt(),triggereff*weight*weightlumi);
                h_dijetMassAtTracker->Fill(eventexcl->GetMassDijets(),triggereff*weight*weightlumi);
                      

                      if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){
                      h_RJJetsAtTracker4->Fill(eventexcl->GetRjjFromJets(),triggereff*weight*weightlumi);
                      h_MultHFJetsAtTracker4->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),triggereff*weight*weightlumi);
                      h_ET_CalosJetsAtTracker4->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),triggereff*weight*weightlumi);
                      h_tracksJetsAtTracker4->Fill(eventdiff->GetMultiplicityTracks(),triggereff*weight*weightlumi);
                      h_aSumEJetsAtTracker4->Fill(aSumE_,triggereff*weight*weightlumi);
                      h_deltaEtaJetsAtTracker4->Fill(eventexcl->GetJetsDeltaEta(),triggereff*weight*weightlumi);
                      h_deltaPhiJetsAtTracker4->Fill(deltaphi_,triggereff*weight*weightlumi);
                      h_deltaPtJetsAtTracker4->Fill(eventexcl->GetJetsDeltaPt(),triggereff*weight*weightlumi);
                      h_dijetMassAtTracker4->Fill(eventexcl->GetMassDijets(),triggereff*weight*weightlumi);
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){
                      h_RJJetsAtTracker3->Fill(eventexcl->GetRjjFromJets(),triggereff*weight*weightlumi);
                      h_MultHFJetsAtTracker3->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),triggereff*weight*weightlumi);
                      h_ET_CalosJetsAtTracker3->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),triggereff*weight*weightlumi);
                      h_tracksJetsAtTracker3->Fill(eventdiff->GetMultiplicityTracks(),triggereff*weight*weightlumi);
                      h_aSumEJetsAtTracker3->Fill(aSumE_,triggereff*weight*weightlumi);
                      h_deltaEtaJetsAtTracker3->Fill(eventexcl->GetJetsDeltaEta(),triggereff*weight*weightlumi);
                      h_deltaPhiJetsAtTracker3->Fill(deltaphi_,triggereff*weight*weightlumi);
                      h_deltaPtJetsAtTracker3->Fill(eventexcl->GetJetsDeltaPt(),triggereff*weight*weightlumi);
                      h_dijetMassAtTracker3->Fill(eventexcl->GetMassDijets(),triggereff*weight*weightlumi);
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){
                      h_RJJetsAtTracker2->Fill(eventexcl->GetRjjFromJets(),triggereff*weight*weightlumi);
                      h_MultHFJetsAtTracker2->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),triggereff*weight*weightlumi);
                      h_ET_CalosJetsAtTracker2->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),triggereff*weight*weightlumi);
                      h_tracksJetsAtTracker2->Fill(eventdiff->GetMultiplicityTracks(),triggereff*weight*weightlumi);
                      h_aSumEJetsAtTracker2->Fill(aSumE_,triggereff*weight*weightlumi);
                      h_deltaEtaJetsAtTracker2->Fill(eventexcl->GetJetsDeltaEta(),triggereff*weight*weightlumi);
                      h_deltaPhiJetsAtTracker2->Fill(deltaphi_,triggereff*weight*weightlumi);
                      h_deltaPtJetsAtTracker2->Fill(eventexcl->GetJetsDeltaPt(),triggereff*weight*weightlumi);
                      h_dijetMassAtTracker2->Fill(eventexcl->GetMassDijets(),triggereff*weight*weightlumi);
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){
                      h_RJJetsAtTracker1->Fill(eventexcl->GetRjjFromJets(),triggereff*weight*weightlumi);
                      h_MultHFJetsAtTracker1->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),triggereff*weight*weightlumi);
                      h_ET_CalosJetsAtTracker1->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())),triggereff*weight*weightlumi);
                      h_tracksJetsAtTracker1->Fill(eventdiff->GetMultiplicityTracks(),triggereff*weight*weightlumi);
                      h_aSumEJetsAtTracker1->Fill(aSumE_,triggereff*weight*weightlumi);
                      h_deltaEtaJetsAtTracker1->Fill(eventexcl->GetJetsDeltaEta(),triggereff*weight*weightlumi);
                      h_deltaPhiJetsAtTracker1->Fill(deltaphi_,triggereff*weight*weightlumi);
                      h_deltaPtJetsAtTracker1->Fill(eventexcl->GetJetsDeltaPt(),triggereff*weight*weightlumi);
                      h_dijetMassAtTracker1->Fill(eventexcl->GetMassDijets(),triggereff*weight*weightlumi);
                      }

                }  

           }// Jets Cuts

  //      }// Triggers

     }// If nVertex

   } // Emule Trigger

  }// Run All Events

 cout << "" << endl;
 cout << "Trigger Info" << endl;
 cout << "------------" << endl;
 cout << "" << endl;
 cout << "Total # of Events: " << TotalE  << endl;
 cout << "Triggered: " << counterTrigger  << endl;
 cout << "Selected " << counterJetsAll <<  " jets with pT(jet1) > " << jet1PT << " [GeV] " << "and pT(jet2) > " << jet2PT << " [GeV] in all CMS acceptance."<< endl;
 cout << "Selected " << counterJetsAtTracker <<  " jets with pT(jet1) > " << jet1PT << " [GeV] " << "and pT(jet2) > " << jet2PT << " [GeV] in tracker acceptance."<< endl;
 cout << "" << endl;

  outf->Write();

~LumiWeights_;

}

