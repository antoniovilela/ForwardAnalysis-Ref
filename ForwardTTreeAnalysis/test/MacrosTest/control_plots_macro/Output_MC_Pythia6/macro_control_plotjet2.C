{

  gROOT->Reset();
  //Normalization example:

  //http://root.cern.ch/root/roottalk/roottalk00/0154.html
  /////////////////////////////////////////////////////////////////// 
  TFile *a = new TFile("hltexcld30U-qcd_15to30.root");
  TFile *b = new TFile("/storage2/sfonseca/CMSSW/pompty_exhume_plots/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/MacrosTest/exHume/hltexcldijet30Uand-exHume.root");
  TFile *c = new TFile("/storage2/sfonseca/CMSSW/pompty_exhume_plots/CMSSW_4_2_8/src/ForwardAnalysis/ForwardTTreeAnalysis/test/MacrosTest/pompty/hltexcldijet30Uand-pomptyPlusMinus.root");
  TFile *d = new TFile("/storage2/dmf/Common/OutputFiles/MacroOutPut/data2010/data_2010_HFAND30.root");  

  TCanvas *c1 = new TCanvas("c1","Validation", 0, 0,700,700);
  c1->Clear();
  c1->cd(0); 
  c1->SetLogy();

 
  //  /////////////////////////////////////////////////////
   a.cd("");
   TH1F *hptDiffa = (TH1F*)pTJet2wc->Clone();
 
   hptDiffa->SetName("pTJet2wc");
   hptDiffa->SetMarkerColor(kBlue);
   hptDiffa->SetLineColor(kBlue);
   hptDiffa->SetMarkerStyle(20);
   hptDiffa->SetMarkerSize(1);
   c1->cd();hptDiffa->Draw("");


 //  /////////////////////////////////////////////////////
  b.cd("");
  TH1F *hptDiffb = (TH1F*)pTJet2wc->Clone();
 
  hptDiffb->SetName("pTJet2wc");
  hptDiffb->SetMarkerColor(kMagenta);
  hptDiffb->SetLineColor(kMagenta);
  hptDiffb->SetMarkerStyle(20);
  hptDiffb->SetMarkerSize(1);
  c1->cd();hptDiffb->Draw("SAME");

//////////////////////////////////////////////	

 /////////////////////////////////////////////////////
  c.cd("");
  TH1F *hptDiffc = (TH1F*)pTJet2wc->Clone();
  hptDiffc->SetName("pTJet2wc");
  hptDiffc->SetMarkerColor(kRed);
  hptDiffc->SetLineColor(kRed);
  hptDiffc->SetMarkerStyle(20);
  hptDiffc->SetMarkerSize(1);
  c1->cd();hptDiffc->Draw("SAME");

///////////////////////////////////////////
 /////////////////////////////////////////////////////
  d.cd("");
 TH1F *hptDiffd = (TH1F*)pTJet2wc->Clone();
 hptDiffd->SetName("pTJet2wc");
 hptDiffd->SetMarkerColor(kBlack);
 hptDiffd->SetLineColor(kBlack);
 hptDiffd->SetMarkerStyle(20);
 hptDiffd->SetMarkerSize(1);
 c1->cd();hptDiffd->Draw("ELPSAME");

///////////////////////////////////////////


  // legend
  c1->cd();
  TLegend *leg = new TLegend(0.40,0.49,0.589,0.589);
  leg->AddEntry(hptDiffa,"QCD Pythia 6","f");
  leg->AddEntry(hptDiffb,"Pompty","f");
  leg->AddEntry(hptDiffc,"exHume","f");
  leg->AddEntry(hptDiffd,"data 2010","f");


  leg->Draw();
  c1->SaveAs("PtJet2_wc.png");

  ////////////////////////////////////////////////////////////////////////////


}


