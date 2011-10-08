////////////////////////////////////////////////////////////////////////////////
//Example of macro to read ROOT tree file which produced by ForwardAnalysisTree
//Author: Eliza Melo
//Revisor: Diego Figueiredo - Modified: 09/21/2011
//Date: 09/13/2011
////////////////////////////////////////////////////////////////////////////////

// Castor in fC. log10(fabs(eventdiff->GetSumETotCastor())) Using fabs because there are some signals with -fC.
// Castor Energy log10 put just in Legend. This is not calculated running this code.

void MacroExclusiveThree(){

// FWLite libraries
  gSystem->Load("libFWCoreFWLite.so");
  AutoLibraryLoader::enable();

  // Loading Files
  TFile *inf  = TFile::Open("/storage2/dmf/Common/OutputFiles/PatSamples2010/PATTupleMultiJets2010B.root");
  TTree *tr = (TTree*)inf->Get("exclusiveDijetsTTreeAnalysis/ProcessedTree");

  // Define Branch Name
  DiffractiveEvent *eventdiff = new DiffractiveEvent();
  ExclusiveDijetsEvent *eventexcl = new ExclusiveDijetsEvent();

  TBranch *diff = tr->GetBranch("DiffractiveAnalysis");
  TBranch *excl = tr->GetBranch("ExclusiveDijetsAnalysis");

  diff->SetAddress(&eventdiff);
  excl->SetAddress(&eventexcl);

// TRIGGERS: ExclusiveDijets
// GetHLTPath(0) = HLT_ExclDiJet30U_HFAND_v*
// GetHLTPath(1) = HLT_ExclDiJet30U_HFOR_v*
// GetHLTPath(2) = HLT_Jet30U*

RunExclusive(60,60,10,1,0);

}

void RunExclusive(double jet1PT, double jet2PT, double jet3PT, int optnVertex, int optTrigger)
{

if (optnVertex == 0){

cout << "---------------------------------------------------------------" << endl;
cout << "Please, restart your setup. Respect the Condition # Vertex > 0)" << endl;
cout << "---------------------------------------------------------------" << endl;
exit();

}

cout << " " << endl;
cout << "pT(jet1) > " << jet1PT << endl;
cout << "pT(jet2) > " << jet2PT << endl;
cout << "pT(jet3) > " << jet3PT << endl;
cout << " " << endl;

// Settings
///////////

char filename[250];
sprintf(filename,"exclusive_2010B_pTJ1-%d_pTJ2-%d_pTJ3-%d_nV-%d_Trigger-%d.root",jet1PT,jet2PT,jet3PT,optnVertex,optTrigger);

TFile *outf = new TFile(filename,"RECREATE");
int NEVENTS = tr->GetEntriesFast();


TH1D *h_RJJ = new TH1D("RJJ","R_{jj} Distribution - Jets in |#eta| < 5.2 ; R_{jj}; N events",50,0,1);
TH1D *h_RJJetsAtTracker = new TH1D("RJJetsAtTracker","R_{jj} Distribution - Jets in |#eta| < 2.9; R_{jj}; N events",50,0,1);

TH1D *h_aSumE = new TH1D("aEnergy","Forward Backward Asymmetry Distribution - Jets in |#eta| < 5.2; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);

TH1D *h_aSumEJetsAtTracker = new TH1D("aEnergyJetsAtTracker","Forward Backward Asymmetry Distribution - Jets in |#eta| < 2.9; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",50,-1,1);


TH1D *h_tracks = new TH1D("tracks","Tracks Multiplicity - Jets in |#eta| < 5.2 ; n Tracks; N events",50,0,150);
TH1D *h_tracksJetsAtTracker = new TH1D("tracksJetsAtTracker","Tracks Multiplicity - Jets in |#eta| < 2.9; nTracks; N events",50,0,150);

TH2F *h_MultHF = new TH2F( "mHF", "HF^{+} and HF^{-} Multiplicity: jets in |#eta| < 5.2; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );

TH2F *h_MultHFJetsAtTracker = new TH2F( "mHFJetsAtTracker", "HF^{+} and HF^{-} Multiplicity: jets in |#eta| < 2.9; n HF^{+}; n HF^{-}; N events", 10,  0., 10., 10,  0., 10. );

TH2F *h_ET_Calos = new TH2F( "ETCalos", "HF^{+} and Castor: jets in |#eta| < 5.2; Energy HF^{+}; log_{10} Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );

TH2F *h_ET_CalosJetsAtTracker = new TH2F( "ETCalosJetsAtTracker", "HF^{+} and Castor: jets in |#eta| < 2.9; Energy HF^{+}; log_{10} Castor Signal [fC]; N events", 10,  0., 10., 50,  0., 50. );

TH1D *h_pfetamax = new TH1D("pfetamax","Particle Flow #eta_{max} Distribution -  jets in |#eta| < 5.2;; #eta; N events",20,0,5.5);
TH1D *h_pfetamin = new TH1D("pfetamin","Particle Flow #eta_{min} Distribution -  jets in |#eta| < 5.2;; #eta; N events",20,-5.5,0);

TH1D *h_pfetamaxJetsAtTracker = new TH1D("pfetamaxJetsAtTracker","Particle Flow #eta_{max} Distribution - jets in |#eta| < 2.9; #eta; N events",20,0,5.5);
TH1D *h_pfetaminJetsAtTracker = new TH1D("pfetaminJetsAtTracker","Particle Flow #eta_{min} Distribution - jets in |#eta| < 2.9; #eta; N events",20,-5.5,0);


//
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

double aSumE;
int TotalE = 0;
int counterTrigger = 0;
int counterJetsAll = 0;
int counterJetsAtTracker = 0;


for(unsigned i=0;i<NEVENTS;i++) {
aSumE=0;
++TotalE;

    //----------- Progress Report -------------
    double progress = 10.0*i/(1.0*NEVENTS);
    int k = TMath::FloorNint(progress); 
    if (k > decade) 
      cout<<10*k<<" %"<<endl;
    decade = k;          

    //----------- Read the Event --------------
    tr->GetEntry(i);


    if (eventexcl->GetHLTPath(optTrigger)) {
    ++counterTrigger;

        if(eventexcl->GetNVertex() > 0 && eventexcl->GetNVertex()<= optnVertex){

            // Setting Analysis Cut
            if (eventexcl->GetLeadingJetP4().Pt() > jet1PT && eventexcl->GetSecondJetP4().Pt() > jet2PT && eventexcl->GetThirdJetP4().Pt() > jet3PT){
                
                if (eventexcl->GetLeadingJetP4().Eta() < 5.2 && eventexcl->GetSecondJetP4().Eta() < 5.2 && eventexcl->GetThirdJetP4().Eta() < 5.2 && eventexcl->GetLeadingJetP4().Eta() > -5.2 && eventexcl->GetSecondJetP4().Eta() > -5.2 && eventexcl->GetThirdJetP4().Eta() > -5.2){
                aSumE = (eventdiff->GetSumEnergyHFPlus() - eventdiff->GetSumEnergyHFMinus())/(eventdiff->GetSumEnergyHFPlus() + eventdiff->GetSumEnergyHFMinus());

                 ++counterJetsAll;
                 h_RJJ->Fill(eventexcl->GetRjjFromJets());
                 h_MultHF->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus());
                 h_ET_Calos->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())));
                 h_tracks->Fill(eventdiff->GetMultiplicityTracks());
                 h_aSumE->Fill(aSumE);
                 h_pfetamin->Fill(eventdiff->GetEtaMinFromPFCands());
                 h_pfetamax->Fill(eventdiff->GetEtaMaxFromPFCands());

                      if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){
                      h_RJJ4->Fill(eventexcl->GetRjjFromJets());
                      h_MultHF4->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus());
                      h_ET_Calos4->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())));
                      h_tracks4->Fill(eventdiff->GetMultiplicityTracks());
                      h_aSumE4->Fill(aSumE);
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){
                      h_RJJ3->Fill(eventexcl->GetRjjFromJets());
                      h_MultHF3->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus());
                      h_ET_Calos3->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())));
                      h_tracks3->Fill(eventdiff->GetMultiplicityTracks());
                      h_aSumE3->Fill(aSumE);
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){
                      h_RJJ2->Fill(eventexcl->GetRjjFromJets());
                      h_MultHF2->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus());
                      h_ET_Calos2->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())));
                      h_tracks2->Fill(eventdiff->GetMultiplicityTracks());
                      h_aSumE2->Fill(aSumE);
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){
                      h_RJJ1->Fill(eventexcl->GetRjjFromJets());
                      h_MultHF1->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus());
                      h_ET_Calos1->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())));
                      h_tracks1->Fill(eventdiff->GetMultiplicityTracks());
                      h_aSumE1->Fill(aSumE);
                      }

                 }  

                if (eventexcl->GetLeadingJetP4().Eta() < 2.9 && eventexcl->GetSecondJetP4().Eta() < 2.9 && eventexcl->GetThirdJetP4().Eta() < 2.9 && eventexcl->GetLeadingJetP4().Eta() > -2.9 && eventexcl->GetSecondJetP4().Eta() > -2.9 && eventexcl->GetThirdJetP4().Eta() > -2.9){
                aSumE = (eventdiff->GetSumEnergyHFPlus() - eventdiff->GetSumEnergyHFMinus())/(eventdiff->GetSumEnergyHFPlus() + eventdiff->GetSumEnergyHFMinus());
  
                ++counterJetsAtTracker;
                h_RJJetsAtTracker->Fill(eventexcl->GetRjjFromJets());
                h_MultHFJetsAtTracker->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus());
                h_ET_CalosJetsAtTracker->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())));
                h_tracksJetsAtTracker->Fill(eventdiff->GetMultiplicityTracks());
                h_aSumEJetsAtTracker->Fill(aSumE);
                h_pfetaminJetsAtTracker->Fill(eventdiff->GetEtaMinFromPFCands());
                h_pfetamaxJetsAtTracker->Fill(eventdiff->GetEtaMaxFromPFCands());

                      if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){
                      h_RJJetsAtTracker4->Fill(eventexcl->GetRjjFromJets());
                      h_MultHFJetsAtTracker4->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus());
                      h_ET_CalosJetsAtTracker4->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())));
                      h_tracksJetsAtTracker4->Fill(eventdiff->GetMultiplicityTracks());
                      h_aSumEJetsAtTracker4->Fill(aSumE);
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){
                      h_RJJetsAtTracker3->Fill(eventexcl->GetRjjFromJets());
                      h_MultHFJetsAtTracker3->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus());
                      h_ET_CalosJetsAtTracker3->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())));
                      h_tracksJetsAtTracker3->Fill(eventdiff->GetMultiplicityTracks());
                      h_aSumEJetsAtTracker3->Fill(aSumE);
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){
                      h_RJJetsAtTracker2->Fill(eventexcl->GetRjjFromJets());
                      h_MultHFJetsAtTracker2->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus());
                      h_ET_CalosJetsAtTracker2->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())));
                      h_tracksJetsAtTracker2->Fill(eventdiff->GetMultiplicityTracks());
                      h_aSumEJetsAtTracker2->Fill(aSumE);
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){
                      h_RJJetsAtTracker1->Fill(eventexcl->GetRjjFromJets());
                      h_MultHFJetsAtTracker1->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus());
                      h_ET_CalosJetsAtTracker1->Fill(eventdiff->GetSumEnergyHFPlus(),log10(fabs(eventdiff->GetSumETotCastor())));
                      h_tracksJetsAtTracker1->Fill(eventdiff->GetMultiplicityTracks());
                      h_aSumEJetsAtTracker1->Fill(aSumE);
                      }

                }  

           }// Jets Cuts

        }// Triggers

     }// If nVertex

  }// Run All Events

 cout << "" << endl;
 cout << "Trigger Info" << endl;
 cout << "------------" << endl;
 cout << "" << endl;
 cout << "Total # of Events: " << TotalE  << endl;
 cout << "Triggered: " << counterTrigger  << endl;
 cout << "Selected " << counterJetsAll <<  " jets with pT(jet1) > " << jet1PT << " [GeV], " << "pT(jet2) > " << jet2PT << " [GeV], " << "and pT(jet3) > " << jet3PT << " [GeV] in all CMS acceptance."<< endl;
 cout << "Selected " << counterJetsAtTracker <<  " jets with pT(jet1) > " << jet1PT << " [GeV], " << "pT(jet2) > " << jet2PT << " [GeV], " << "and pT(jet3) > " << jet3PT << " [GeV] in tracker acceptance."<< endl;
 cout << "" << endl;

  outf->Write();

}


