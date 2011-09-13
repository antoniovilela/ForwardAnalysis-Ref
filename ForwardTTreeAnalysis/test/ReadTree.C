///////////////////////////////////////////////////////////////////////
//Example of macro to read ROOT tree file which produced by ForwardAnalysisTree
//Author: Eliza Melo
//Date: 09/13/2011
////////////////////////////////////////////////////////////////////////////
void ReadTree()
{
  //------------ FWLite libraries ------
  gSystem->Load("libFWCoreFWLite.so");
  AutoLibraryLoader::enable();
  //------------ files -----------------
  TFile *inf  = TFile::Open("/storage2/eliza/MERGED_PAT/merged_JetMETTAU_0.root");
  TFile *outf = new TFile("JetMETTaucuts_0_Histos.root","RECREATE");
  TTree *tr = (TTree*)inf->Get("exclusiveDijetsTTreeAnalysis/ProcessedTree");
  //----------- define the tree branch -
  ExclusiveDijetsEvent *event = new ExclusiveDijetsEvent();
  TBranch *branch = tr->GetBranch("Events");
  branch->SetAddress(&event);
  //----------- settings ---------------
  int NEVENTS(2322766);
  //TString HLT("HLT_Jet370_v3");
  //double PTMIN(30); double ETAMAX(4.6)
  //--------- book histos ---------------------------------
  TH2F *hEtamaxVsNvertex = new TH2F("etaMaxFromPFCandsVsNVertex","etaMaxFromPFCandsVsNVertex",82,-7.,10.,82,0,10);
  //TH1F *hY     = new TH1F("JetRapidity","JetRapidity",100,-5,5);
  //TH1F *hMET   = new TH1F("METoverSUMET","METoverSUMET",100,0,1.0001);
  //TH1F *hNPV   = new TH1F("NumberOfVertices","NumberOfVertices",30,0,30);
  //TH1F *hRho   = new TH1F("PtDensityRho","PtDensityRho",50,0,50);
  //--------- trigger mapping ---------------------------------
  //--------- read the histogram with the trigger names -------
  //TH1F *hTrigNames = (TH1F*)inf->Get("ak7/TriggerNames");
  //int ihlt(-1);
  //cout<<"Finding trigger mapping: "<<endl;
  //----------- loop over the X-axis labels -----------------
  /*for(int ibin=0;ibin<hTrigNames->GetNbinsX();ibin++) {
    TString ss(hTrigNames->GetXaxis()->GetBinLabel(ibin+1));
    if (ss == HLT) {
      ihlt = ibin;
      continue;
    }
  }
  if (ihlt == -1) {
    cout<<"The requested trigger ("<<HLT<<") is not found ";
    break;
  }
  else {
    cout<<HLT<<" --> "<<ihlt<<endl;
  }*/
  //----------- counters -----------------------
  int counter_hlt(0),counter_pv(0),counter_hcal(0),counter_jet(0);
  //----------- tree reading -------------------
  unsigned NEntries = tr->GetEntries();
  cout<<"Reading TREE: "<<NEntries<<" events"<<endl;
  int decade = 0;
  for(unsigned i=0;i<NEVENTS;i++) {
    //----------- progress report -------------
    double progress = 10.0*i/(1.0*NEVENTS);
    int k = TMath::FloorNint(progress); 
    if (k > decade) 
      cout<<10*k<<" %"<<endl;
    decade = k;          
    //----------- read the event --------------
    tr->GetEntry(i);
    //cout<<"massDijets: " <<  event->GetMassDijets() << "etamax: "<< event->GetetaMaxFromPFCandsNew()  << endl;


   //----------- check if the trigger was fired ---
    //bool hltPass(false);
    //int prescale(1);
    //if (ihlt == -1)
      //hltPass = true; // no trigger set
    //else {
      //if (Event->fired(ihlt) > 0) {
        //hltPass = true;
        //prescale = Event->preL1(ihlt) * Event->preHLT(ihlt);
      //}
    //}
    //if (hltPass) {
      //counter_hlt++;
      //-------- check if the primary vertex is good ----
      /*if (Event->evtHdr().isPVgood() == 1) {
        counter_pv++; 
        //-------- check the loose HCAL noise filter ----------
        if (Event->evtHdr().looseHCALNoise()) {
          counter_hcal++;
          //------- fill the MET/SumET control histo ----------
          hMET->Fill(Event->pfmet().met_o_sumet());
          //------- fill the NPV histo ------------------------
          hNPV->Fill(Event->evtHdr().nVtxGood());          
          //------- fill the Rho histo ------------------------
          hRho->Fill(Event->evtHdr().pfRho());
          //------- loop over the PF jets ---------------------
          for(int j=0;j<Event->nPFJets();j++) { 
            //----- apply the pt and ID cuts ------------------
            if (Event->pfjet(j).ptCor() >= PTMIN && Event->pfjet(j).tightID()) {
              counter_jet++;*/
            if (event->GetLeadingJetP4().Pt() > 30. && event->GetSecondJetP4().Pt() > 30.){
             if (event->GetLeadingJetP4().Eta() < 4.6 && event->GetSecondJetP4().Eta() < 4.6){

               counter_jet++;
              //------- fill some histograms ------------------
              hEtamaxVsNvertex->Fill(event->GetetaMaxFromPFCandsNew(),event->GetNVertex());
              //hY->Fill(Event->pfjet(j).y()); 
           }  
          }// jet loop
        ///}// hcal noise filter    
      ////}// pv cut
    /////}// hlt
  }// tree loop
  //----------------- print out some information ---------------
  cout<<"Events read:                      "<<NEVENTS<<endl;
  //cout<<"Events after the trigger cut:     "<<counter_hlt<<endl;
  //cout<<"Events after the PV cut:          "<<counter_pv<<endl;
  //cout<<"Events after the HCAL filter cut: "<<counter_hcal<<endl;
  cout<<"Number of jets:                   "<<counter_jet<<endl;  
  //----------------- save the histos to the output file -------
  outf->Write();
}

