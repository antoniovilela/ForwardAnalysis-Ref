#include <stdio.h>

void NumberOfEvents(){

  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libPhysicsToolsUtilities.so");
  AutoLibraryLoader::enable();

  // Loading Files
  TFile *inf = TFile::Open("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_All_Pt-15to3000-Z2_PUS3.root");
  
  //2010 
  TTree *tr = (TTree*)inf->Get("forwardQCDTTreeAnalysis/ProcessedTree"); 

  //2011
  //TTree *tr = (TTree*)inf->Get("exclusiveDijetsTTreeAnalysis/ProcessedTree");

  // Define Branch Name
  DiffractiveEvent *eventdiff = new DiffractiveEvent();
  ExclusiveDijetsEvent *eventexcl = new ExclusiveDijetsEvent();

  TBranch *diff = tr->GetBranch("DiffractiveAnalysis");
  TBranch *excl = tr->GetBranch("ExclusiveDijetsAnalysis");

  diff->SetAddress(&eventdiff);
  excl->SetAddress(&eventexcl);

  int NEVENTS = tr->GetEntriesFast();

  TFile *out = new TFile("numberofevents.root","RECREATE");

  // Tree Reading 
  unsigned NEntries = tr->GetEntries();
  int decade = 0;

TH1D *h_events = new TH1D("events","Distribution of Cuts Evolution; Cuts; N events",100,0,100);
TH1D *h_eventspu0 = new TH1D("eventspu0","Distribution of Cuts Evolution Pile Up = 0; Cuts; N events",100,0,100);
TH1D *h_eventspu1 = new TH1D("eventspu1","Distribution of Cuts Evolution Pile Up = 1; Cuts; N events",100,0,100);
TH1D *h_eventspu2 = new TH1D("eventspu2","Distribution of Cuts Evolution Pile Up = 2; Cuts; N events",100,0,100);

TH1D *h_eventstracker = new TH1D("eventstracker","Distribution of Cuts Evolution; Cuts; N events",100,0,100);
TH1D *h_eventstrackerpu0 = new TH1D("eventstrackerpu0","Distribution of Cuts Evolution Pile Up = 0; Cuts; N events",100,0,100);
TH1D *h_eventstrackerpu1 = new TH1D("eventstrackerpu1","Distribution of Cuts Evolution Pile Up = 1; Cuts; N events",100,0,100);
TH1D *h_eventstrackerpu2 = new TH1D("eventstrackerpu2","Distribution of Cuts Evolution Pile Up = 2; Cuts; N events",100,0,100);

// Event by Event Analysis
//////////////////////////

int TotalE = 0;
int counterTotalEPU0 = 0;
int counterTotalEPU1 = 0;
int counterTotalEPU2 = 0;

int counterTrigger = 0;
int counterTriggerPU0 = 0;
int counterTriggerPU1 = 0;
int counterTriggerPU2 = 0;

int counterVertex = 0;
int counterVertexPU0 = 0;
int counterVertexPU1 = 0;
int counterVertexPU2 = 0;

int counterpTCut = 0;
int counterpTCutPU0 = 0;
int counterpTCutPU1 = 0;
int counterpTCutPU2 = 0;

int counterJetAcceptance = 0;
int counterJetAcceptancePU0 = 0;
int counterJetAcceptancePU1 = 0;
int counterJetAcceptancePU2 = 0;

int counterEta4 = 0;
int counterEta4PU0 = 0;
int counterEta4PU1 = 0;
int counterEta4PU2 = 0;

int counterEta3 = 0;
int counterEta3PU0 = 0;
int counterEta3PU1 = 0;
int counterEta3PU2 = 0;

int counterEta2 = 0;
int counterEta2PU0 = 0;
int counterEta2PU1 = 0;
int counterEta2PU2 = 0;

int counterEta1 = 0;
int counterEta1PU0 = 0;
int counterEta1PU1 = 0;
int counterEta1PU2 = 0;

int counterJetAcceptanceTracker = 0;
int counterJetAcceptanceTrackerPU0 = 0;
int counterJetAcceptanceTrackerPU1 = 0;
int counterJetAcceptanceTrackerPU2 = 0;

int counterEtaTracker4 = 0;
int counterEtaTracker4PU0 = 0;
int counterEtaTracker4PU1 = 0;
int counterEtaTracker4PU2 = 0;

int counterEtaTracker3 = 0;
int counterEtaTracker3PU0 = 0;
int counterEtaTracker3PU1 = 0;
int counterEtaTracker3PU2 = 0;

int counterEtaTracker2 = 0;
int counterEtaTracker2PU0 = 0;
int counterEtaTracker2PU1 = 0;
int counterEtaTracker2PU2 = 0;

int counterEtaTracker1 = 0;
int counterEtaTracker1PU0 = 0;
int counterEtaTracker1PU1 = 0;
int counterEtaTracker1PU2 = 0;

cout << " " << endl;
cout << "Starting... " << endl;

for(unsigned i=0;i<NEVENTS;i++) {

    //----------- Progress Report -------------
    double progress = 10.0*i/(1.0*NEVENTS);
    int k = TMath::FloorNint(progress); 
    if (k > decade) 
      cout<<10*k<<" %"<<endl;
    decade = k;          

      ++TotalE;
      if (eventexcl->GetNPileUpBx0() == 0) ++counterTotalEPU0;
      if (eventexcl->GetNPileUpBx0() == 1) ++counterTotalEPU1;
      if (eventexcl->GetNPileUpBx0() == 2) ++counterTotalEPU2;


// Emule Trigger
////////////////

tr->GetEntry(i);

if (eventdiff->GetSumEnergyHFMinus() < 5 && eventdiff->GetSumEnergyHFPlus() < 5){

      ++counterTrigger;
      if (eventexcl->GetNPileUpBx0() == 0) ++counterTriggerPU0;
      if (eventexcl->GetNPileUpBx0() == 1) ++counterTriggerPU1;
      if (eventexcl->GetNPileUpBx0() == 2) ++counterTriggerPU2;


        if(eventexcl->GetNVertex() > 0 && eventexcl->GetNVertex()<= 10){

           ++counterVertex;
           if (eventexcl->GetNPileUpBx0() == 0) ++counterVertexPU0;
           if (eventexcl->GetNPileUpBx0() == 1) ++counterVertexPU1;
           if (eventexcl->GetNPileUpBx0() == 2) ++counterVertexPU2;
        
             
            if (eventexcl->GetLeadingJetP4().Pt() > 60 && eventexcl->GetSecondJetP4().Pt() > 60 ){
            
              ++counterpTCut;
              if (eventexcl->GetNPileUpBx0() == 0) ++counterpTCutPU0;
              if (eventexcl->GetNPileUpBx0() == 1) ++counterpTCutPU1;
              if (eventexcl->GetNPileUpBx0() == 2) ++counterpTCutPU2;
                
                if (eventexcl->GetLeadingJetP4().Eta() < 5.2 && eventexcl->GetSecondJetP4().Eta() < 5.2 && eventexcl->GetLeadingJetP4().Eta() > -5.2 && eventexcl->GetSecondJetP4().Eta() > -5.2){
                
                     ++counterJetAcceptance;
                     if (eventexcl->GetNPileUpBx0() == 0) ++counterJetAcceptancePU0;
                     if (eventexcl->GetNPileUpBx0() == 1) ++counterJetAcceptancePU1;
                     if (eventexcl->GetNPileUpBx0() == 2) ++counterJetAcceptancePU2;
                

                      if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){
                
                         ++counterEta4;
                         if (eventexcl->GetNPileUpBx0() == 0) ++counterEta4PU0;
                         if (eventexcl->GetNPileUpBx0() == 1) ++counterEta4PU1;
                         if (eventexcl->GetNPileUpBx0() == 2) ++counterEta4PU2;
                  
                      
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){
                      
                         ++counterEta3;
                         if (eventexcl->GetNPileUpBx0() == 0) ++counterEta3PU0;
                         if (eventexcl->GetNPileUpBx0() == 1) ++counterEta3PU1;
                         if (eventexcl->GetNPileUpBx0() == 2) ++counterEta3PU2;
                      
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){
                      
                         ++counterEta2;
                         if (eventexcl->GetNPileUpBx0() == 0) ++counterEta2PU0;
                         if (eventexcl->GetNPileUpBx0() == 1) ++counterEta2PU1;
                         if (eventexcl->GetNPileUpBx0() == 2) ++counterEta2PU2;   
                      
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){
                      
                         ++counterEta1;
                         if (eventexcl->GetNPileUpBx0() == 0) ++counterEta1PU0;
                         if (eventexcl->GetNPileUpBx0() == 1) ++counterEta1PU1;
                         if (eventexcl->GetNPileUpBx0() == 2) ++counterEta1PU2; 
                      
                      }

                 }  

// Jets At Tracker!

                if (eventexcl->GetLeadingJetP4().Eta() < 2.9 && eventexcl->GetSecondJetP4().Eta() < 2.9 && eventexcl->GetLeadingJetP4().Eta() > -2.9 && eventexcl->GetSecondJetP4().Eta() > -2.9){
                
                    ++counterJetAcceptanceTracker;
                     if (eventexcl->GetNPileUpBx0() == 0) ++counterJetAcceptanceTrackerPU0;
                     if (eventexcl->GetNPileUpBx0() == 1) ++counterJetAcceptanceTrackerPU1;
                     if (eventexcl->GetNPileUpBx0() == 2) ++counterJetAcceptanceTrackerPU2;
                

                      if (eventdiff->GetEtaMinFromPFCands() > -4. && eventdiff->GetEtaMaxFromPFCands() < 4.){
                
                         ++counterEtaTracker4;
                         if (eventexcl->GetNPileUpBx0() == 0) ++counterEtaTracker4PU0;
                         if (eventexcl->GetNPileUpBx0() == 1) ++counterEtaTracker4PU1;
                         if (eventexcl->GetNPileUpBx0() == 2) ++counterEtaTracker4PU2;
                  
                      
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -3. && eventdiff->GetEtaMaxFromPFCands() < 3.){
                      
                         ++counterEtaTracker3;
                         if (eventexcl->GetNPileUpBx0() == 0) ++counterEtaTracker3PU0;
                         if (eventexcl->GetNPileUpBx0() == 1) ++counterEtaTracker3PU1;
                         if (eventexcl->GetNPileUpBx0() == 2) ++counterEtaTracker3PU2;
                      
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -2. && eventdiff->GetEtaMaxFromPFCands() < 2.){
                      
                         ++counterEtaTracker2;
                         if (eventexcl->GetNPileUpBx0() == 0) ++counterEtaTracker2PU0;
                         if (eventexcl->GetNPileUpBx0() == 1) ++counterEtaTracker2PU1;
                         if (eventexcl->GetNPileUpBx0() == 2) ++counterEtaTracker2PU2;   
                      
                      }

                      if (eventdiff->GetEtaMinFromPFCands() > -1. && eventdiff->GetEtaMaxFromPFCands() < 1.){
                      
                         ++counterEtaTracker1;
                         if (eventexcl->GetNPileUpBx0() == 0) ++counterEtaTracker1PU0;
                         if (eventexcl->GetNPileUpBx0() == 1) ++counterEtaTracker1PU1;
                         if (eventexcl->GetNPileUpBx0() == 2) ++counterEtaTracker1PU2; 
                      
                      }

                }  

           }// Jets Cuts

     }// If nVertex

   } // Emule Trigger

  }// Run All Events


// Checking Output!!!

cout << TotalE << endl;
cout << counterTrigger << endl;
cout << counterVertex << endl;
cout << counterpTCut << endl;
cout << counterJetAcceptance << endl;
cout << counterEta4 << endl;
cout << counterEta3 << endl;
cout << counterEta2 << endl;
cout << counterEta1 << endl;

cout << counterTotalEPU0 << endl;
cout << counterTriggerPU0 << endl;
cout << counterVertexPU0 << endl;
cout << counterpTCutPU0 << endl;
cout << counterJetAcceptancePU0 << endl;
cout << counterEta4PU0 << endl;
cout << counterEta3PU0 << endl;
cout << counterEta2PU0 << endl;
cout << counterEta1PU0 << endl;


h_events->AddBinContent(10,TotalE);
h_events->AddBinContent(20,counterTrigger);
h_events->AddBinContent(30,counterVertex);
h_events->AddBinContent(40,counterpTCut);
h_events->AddBinContent(50,counterJetAcceptance);
h_events->AddBinContent(60,counterEta4);
h_events->AddBinContent(70,counterEta3);
h_events->AddBinContent(80,counterEta2);
h_events->AddBinContent(90,counterEta1);

h_eventspu0->AddBinContent(10,counterTotalEPU0);
h_eventspu0->AddBinContent(20,counterTriggerPU0);
h_eventspu0->AddBinContent(30,counterVertexPU0);
h_eventspu0->AddBinContent(40,counterpTCutPU0);
h_eventspu0->AddBinContent(50,counterJetAcceptancePU0);
h_eventspu0->AddBinContent(60,counterEta4PU0);
h_eventspu0->AddBinContent(70,counterEta3PU0);
h_eventspu0->AddBinContent(80,counterEta2PU0);
h_eventspu0->AddBinContent(90,counterEta1PU0);

h_eventspu1->AddBinContent(10,counterTotalEPU1);
h_eventspu1->AddBinContent(20,counterTriggerPU1);
h_eventspu1->AddBinContent(30,counterVertexPU1);
h_eventspu1->AddBinContent(40,counterpTCutPU1);
h_eventspu1->AddBinContent(50,counterJetAcceptancePU1);
h_eventspu1->AddBinContent(60,counterEta4PU1);
h_eventspu1->AddBinContent(70,counterEta3PU1);
h_eventspu1->AddBinContent(80,counterEta2PU1);
h_eventspu1->AddBinContent(90,counterEta1PU1);

h_eventspu2->AddBinContent(10,counterTotalEPU2);
h_eventspu2->AddBinContent(20,counterTriggerPU2);
h_eventspu2->AddBinContent(30,counterVertexPU2);
h_eventspu2->AddBinContent(40,counterpTCutPU2);
h_eventspu2->AddBinContent(50,counterJetAcceptancePU2);
h_eventspu2->AddBinContent(60,counterEta4PU2);
h_eventspu2->AddBinContent(70,counterEta3PU2);
h_eventspu2->AddBinContent(80,counterEta2PU2);
h_eventspu2->AddBinContent(90,counterEta1PU2);

// Tracker

h_eventstracker->AddBinContent(10,TotalE);
h_eventstracker->AddBinContent(20,counterTrigger);
h_eventstracker->AddBinContent(30,counterVertex);
h_eventstracker->AddBinContent(40,counterpTCut);
h_eventstracker->AddBinContent(50,counterJetAcceptanceTracker);
h_eventstracker->AddBinContent(60,counterEtaTracker4);
h_eventstracker->AddBinContent(70,counterEtaTracker3);
h_eventstracker->AddBinContent(80,counterEtaTracker2);
h_eventstracker->AddBinContent(90,counterEtaTracker1);

h_eventstrackerpu0->AddBinContent(10,counterTotalEPU0);
h_eventstrackerpu0->AddBinContent(20,counterTriggerPU0);
h_eventstrackerpu0->AddBinContent(30,counterVertexPU0);
h_eventstrackerpu0->AddBinContent(40,counterpTCutPU0);
h_eventstrackerpu0->AddBinContent(50,counterJetAcceptanceTrackerPU0);
h_eventstrackerpu0->AddBinContent(60,counterEtaTracker4PU0);
h_eventstrackerpu0->AddBinContent(70,counterEtaTracker3PU0);
h_eventstrackerpu0->AddBinContent(80,counterEtaTracker2PU0);
h_eventstrackerpu0->AddBinContent(90,counterEtaTracker1PU0);

h_eventstrackerpu1->AddBinContent(10,counterTotalEPU1);
h_eventstrackerpu1->AddBinContent(20,counterTriggerPU1);
h_eventstrackerpu1->AddBinContent(30,counterVertexPU1);
h_eventstrackerpu1->AddBinContent(40,counterpTCutPU1);
h_eventstrackerpu1->AddBinContent(50,counterJetAcceptanceTrackerPU1);
h_eventstrackerpu1->AddBinContent(60,counterEtaTracker4PU1);
h_eventstrackerpu1->AddBinContent(70,counterEtaTracker3PU1);
h_eventstrackerpu1->AddBinContent(80,counterEtaTracker2PU1);
h_eventstrackerpu1->AddBinContent(90,counterEtaTracker1PU1);

h_eventstrackerpu2->AddBinContent(10,counterTotalEPU2);
h_eventstrackerpu2->AddBinContent(20,counterTriggerPU2);
h_eventstrackerpu2->AddBinContent(30,counterVertexPU2);
h_eventstrackerpu2->AddBinContent(40,counterpTCutPU2);
h_eventstrackerpu2->AddBinContent(50,counterJetAcceptanceTrackerPU2);
h_eventstrackerpu2->AddBinContent(60,counterEtaTracker4PU2);
h_eventstrackerpu2->AddBinContent(70,counterEtaTracker3PU2);
h_eventstrackerpu2->AddBinContent(80,counterEtaTracker2PU2);
h_eventstrackerpu2->AddBinContent(90,counterEtaTracker1PU2);

cout << " " << endl;
cout << "Saving file..." << endl;
out->Write();

cout << " " << endl;
cout << "Finish" << endl;
cout << " " << endl;
}
