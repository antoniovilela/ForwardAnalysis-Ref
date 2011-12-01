void CreateHistoPUMC(){

  gSystem->Load("libFWCoreFWLite.so");
  gSystem->Load("libPhysicsToolsUtilities.so");
  AutoLibraryLoader::enable();

  // Loading Files
  TFile *inf = TFile::Open("/storage2/dmf/Common/OutputFiles/MCPythia6/QCD_Pt-170to300-Z2_PUS3.root");
  TTree *tr = (TTree*)inf->Get("forwardQCDTTreeAnalysis/ProcessedTree"); 

  // Define Branch Name
  DiffractiveEvent *eventdiff = new DiffractiveEvent();
  ExclusiveDijetsEvent *eventexcl = new ExclusiveDijetsEvent();

  TBranch *diff = tr->GetBranch("DiffractiveAnalysis");
  TBranch *excl = tr->GetBranch("ExclusiveDijetsAnalysis");

  diff->SetAddress(&eventdiff);
  excl->SetAddress(&eventexcl);

  TFile *outf = new TFile("pileup170to300mc.root","RECREATE");
  int NEVENTS = tr->GetEntriesFast();

TH1D *h_pu = new TH1D("pileupmc","PileUp Monte Carlo; # Pile Up; N events",25,0,25);

// Tree Reading 
unsigned NEntries = tr->GetEntries();
int decade = 0;


// Event by Event Analysis
//////////////////////////

for(unsigned i=0;i<NEVENTS;i++) {

    //----------- Progress Report -------------
    double progress = 10.0*i/(1.0*NEVENTS);
    int k = TMath::FloorNint(progress); 
    if (k > decade) 
      cout<<10*k<<" %"<<endl;
    decade = k;          

    //----------- Read the Event --------------
    tr->GetEntry(i);
    
    h_pu->Fill(eventexcl->GetNPileUpBx0());

}// Run All Events

outf->Write();

}
