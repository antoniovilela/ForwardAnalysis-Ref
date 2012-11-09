 {


  //  gStyle->SetOptStats(000);
  TFile* fileColl_HLT_ZB = TFile::Open("analysisHistos_CaloTowerAnalysis_0.root");


  TFile* fileColl_HLT_PHY = TFile::Open("analysisHistos_CaloTowerAnalysis_0.root");



  TH1F* nEvents_HLT_ZB = static_cast<TH1F*>(fileColl_HLT_ZB->Get((std::string("caloTowerAnalysisZeroBiasNoTrk") + "/NEvents").c_str()));
  TH1F* nEvents_HLT_PHY = static_cast<TH1F*>(fileColl_HLT_PHY->Get((std::string("caloTowerAnalysisBPTXPlusOrMinusOnly") + "/NEvents").c_str()));


  // double 
  TCanvas* canvasHF = new TCanvas("canvasHF","Energy HF");
  TLegend* leg = new TLegend(0.7597956,0.822335,0.9531857,0.9549239,NULL,"brNDC");
  canvasHF->cd();

  HLT_ZB NonCollisions
  TH1F* h_energyHF_Coll_HLT_ZB = static_cast<TH1F*>(fileColl_HLT_ZB->Get((std::string("caloTowerAnalysisZeroBiasNoTrk") + "/energyHF").c_str()));
  h_energyHF_Coll_HLT_ZB->Scale(1./nEvents_HLT_ZB->GetBinContent(1));
  h_energyHF_Coll_HLT_ZB->SetMarkerColor(kBlack);
  h_energyHF_Coll_HLT_ZB->SetMarkerStyle(20);
  h_energyHF_Coll_HLT_ZB->SetMarkerSize(1.1);
  h_energyHF_Coll_HLT_ZB->SetStats(kFALSE);	
  h_energyHF_Coll_HLT_ZB->GetYaxis()->SetTitle("1/NEvents");
  h_energyHF_Coll_HLT_ZB->GetXaxis()->SetTitle("HF tower energy (GeV)");
  leg->AddEntry(h_energyHF_Coll_HLT_ZB,"p+p (7 TeV) Non Collisions using HLT_ZeroBias","p");


  //HLT Unpaired

  TH1F* h_energyHF_Coll_HLT_PHY = static_cast<TH1F*>(fileColl_HLT_PHY->Get((std::string("caloTowerAnalysisBPTXPlusOrMinusOnly") + "/energyHF").c_str()));
  h_energyHF_Coll_HLT_PHY->Scale(1./nEvents_HLT_PHY->GetBinContent(1));
  h_energyHF_Coll_HLT_PHY->SetMarkerColor(kBlue);
  h_energyHF_Coll_HLT_PHY->SetMarkerStyle(24);
  h_energyHF_Coll_HLT_PHY->SetMarkerSize(1.1);
  h_energyHF_Coll_HLT_PHY->GetYaxis()->SetTitle("1/NEvents");
  h_energyHF_Coll_HLT_PHY->GetXaxis()->SetTitle("HF tower energy (GeV)");
  leg->AddEntry(h_energyHF_Coll_HLT_PHY,"p+p (7 TeV) Unpaired","p");


  h_energyHF_Coll_HLT_ZB->Draw("p");
  h_energyHF_Coll_HLT_PHY->Draw("p");
  leg->Draw("p");
  canvasHF->Update();

  //Calculated of threshold of the CaloTowers - Non-Collisions
  Int_t binMin_ZB = h_energyHF_Coll_HLT_ZB->GetMinimumBin();
  Int_t binMax_ZB = h_energyHF_Coll_HLT_ZB->GetMaximumBin();
  TAxis *axis = h_energyHF_Coll_HLT_ZB->GetXaxis();
  Int_t binMin_ZB = axis->FindBin(0.0375);
  Int_t binMax_ZB = axis->FindBin(14.9625);
  Double_t integral_ZB = h_energyHF_Coll_HLT_ZB->Integral(binMin_ZB,binMax_ZB);
  Double_t value_limits_ZB_95 = 0.95*integral_ZB;
  Double_t value_limits_ZB_99 = 0.99*integral_ZB;  
  Double_t maxvalue=h_energyHF_Coll_HLT_ZB->GetMaximum();
  cout<<"Threshold of the CaloTowers in HF using HLT_ZeroBias (95%): "<< value_limits_ZB_95 <<" in GeV " <<" >>> binMin: "<< binMin_ZB <<" >>> binMax: "<< binMax_ZB <<endl;
  cout<<"Threshold of the CaloTowers in HF using HLT_ZeroBias (99%): "<< value_limits_ZB_99 <<" in GeV " <<" >>> binMin: "<< binMin_ZB <<" >>> binMax: "<< binMax_ZB <<endl;
  // Show Thresold in histogram

  TLine *l=new TLine(value_limits_ZB_99,0,value_limits_ZB_99,maxvalue);  
  l->SetLineColor(2);
  l->SetLineStyle(7);
  l->SetLineWidth(2);
  l->Draw();
  canvasHF->SaveAs("h_energyHF_NonColl.root");

  //  Calculated of threshold of the CaloTowers - Unpaired
  Int_t binMin_ = h_energyHF_Coll_HLT_PHY->GetMinimumBin();
  Int_t binMax_ = h_energyHF_Coll_HLT_PHY->GetMaximumBin();
  TAxis *axiss = h_energyHF_Coll_HLT_PHY->GetXaxis();
  Int_t binMin_ = axiss->FindBin(0.0375);
  Int_t binMax_ = axiss->FindBin(14.9625);
  Double_t integral_ = h_energyHF_Coll_HLT_PHY->Integral(binMin_,binMax_);
  Double_t value_limits_95 = 0.95*integral_;
  Double_t value_limits_99 = 0.99*integral_;  
  Double_t maxvalue=h_energyHF_Coll_HLT_PHY->GetMaximum();
  cout<<"Threshold of the CaloTowers in HF using Unpaired (95%): "<< value_limits_95 <<" in GeV "<<" >>> binMin: "<< binMin_ <<" >>> binMax: "<< binMax_ << endl;
  cout<<"Threshold of the CaloTowers in HF using Unpaired (99%) "<< value_limits_99 <<" in GeV " <<" >>> binMin: "<< binMin_ <<" >>> binMax: "<< binMax_ << endl;
  // Show Thresold in histogram

  TLine *l=new TLine(value_limits_99,0,value_limits_99,maxvalue);  
  l->SetLineColor(14);
  l->SetLineStyle(7);
  l->SetLineWidth(2);
  l->Draw();
  canvasHF->SaveAs("h_energyHF_Unpaired.root");


//----------------------------------------------
  TCanvas* canvasHFPlus = new TCanvas("canvasHFPlus","Energy HF Plus");
  TLegend* leg = new TLegend(0.7597956,0.822335,0.9531857,0.9549239,NULL,"brNDC");

  canvasHFPlus->cd();


  TH1F* h_energyHFplus_Coll_HLT_ZB = static_cast<TH1F*>(fileColl_HLT_ZB->Get((std::string("caloTowerAnalysisZeroBiasNoTrk") + "/energyHFplus").c_str()));
  h_energyHFplus_Coll_HLT_ZB->Scale(1./nEvents_HLT_ZB->GetBinContent(1));
  h_energyHFplus_Coll_HLT_ZB->SetMarkerColor(kBlack);
  h_energyHFplus_Coll_HLT_ZB->SetMarkerStyle(25);
  h_energyHFplus_Coll_HLT_ZB->SetMarkerSize(1.1);
  h_energyHFplus_Coll_HLT_ZB->GetYaxis()->SetTitle("1/NEvents");
  h_energyHFplus_Coll_HLT_ZB->GetXaxis()->SetTitle("HF(Plus) tower energy (GeV)");
  leg->AddEntry(h_energyHFplus_Coll_HLT_ZB,"p+p (7 TeV) Non Collisions using HLT_ZeroBias","p");


 TH1F* h_energyHFplus_Coll_HLT_PHY = static_cast<TH1F*>(fileColl_HLT_PHY->Get((std::string("caloTowerAnalysisBPTXPlusOrMinusOnly") + "/energyHFplus").c_str()));
  h_energyHFplus_Coll_HLT_PHY->Scale(1./nEvents_HLT_PHY->GetBinContent(1));
  h_energyHFplus_Coll_HLT_PHY->SetMarkerColor(kBlue);
  h_energyHFplus_Coll_HLT_PHY->SetMarkerStyle(24);
  h_energyHFplus_Coll_HLT_PHY->SetMarkerSize(1.1);
  h_energyHFplus_Coll_HLT_PHY->GetYaxis()->SetTitle("1/NEvents");
  h_energyHFplus_Coll_HLT_PHY->GetXaxis()->SetTitle("HF(Plus) tower energy (GeV)");
  leg->AddEntry(h_energyHFplus_Coll_HLT_PHY,"p+p (7 TeV) Unpaired","p");


  h_energyHFplus_Coll_HLT_ZB->Draw("p");
  h_energyHFplus_Coll_HLT_PHY->Draw("psames");
  leg->Draw("psames");
  canvasHFPlus->SaveAs("h_energyHFplus.root");

//-----------------------------------------------
  TCanvas* canvasHFMinus = new TCanvas("canvasHFMinus","Energy HF Minus");
  TLegend* leg = new TLegend(0.7597956,0.822335,0.9531857,0.9549239,NULL,"brNDC");
  canvasHFMinus->cd();
//-----------------------------------------------*/

  TH1F* h_energyHFminus_Coll_HLT_ZB = static_cast<TH1F*>(fileColl_HLT_ZB->Get((std::string("caloTowerAnalysisZeroBiasNoTrk") + "/energyHFminus").c_str()));
  h_energyHFminus_Coll_HLT_ZB->Scale(1./nEvents_HLT_ZB->GetBinContent(1));
  h_energyHFminus_Coll_HLT_ZB->SetMarkerColor(kBlack);
  h_energyHFminus_Coll_HLT_ZB->SetMarkerStyle(25);
  h_energyHFminus_Coll_HLT_ZB->SetMarkerSize(1.1);
  h_energyHFminus_Coll_HLT_ZB->GetYaxis()->SetTitle("1/NEvents");
  h_energyHFminus_Coll_HLT_ZB->GetXaxis()->SetTitle("HF(Minus) tower energy (GeV)");
  leg->AddEntry(h_energyHFminus_Coll_HLT_ZB,"p+p (7 TeV) Non Collisions using HLT_ZeroBias","p");


 TH1F* h_energyHFminus_Coll_HLT_PHY = static_cast<TH1F*>(fileColl_HLT_PHY->Get((std::string("caloTowerAnalysisBPTXPlusOrMinusOnly") + "/energyHFminus").c_str()));
  h_energyHFminus_Coll_HLT_PHY->Scale(1./nEvents_HLT_PHY->GetBinContent(1));
  h_energyHFminus_Coll_HLT_PHY->SetMarkerColor(kBlue);
  h_energyHFminus_Coll_HLT_PHY->SetMarkerStyle(24);
  h_energyHFminus_Coll_HLT_PHY->SetMarkerSize(1.1);
  h_energyHFminus_Coll_HLT_PHY->GetYaxis()->SetTitle("1/NEvents");
  h_energyHFminus_Coll_HLT_PHY->GetXaxis()->SetTitle("HF(Minus) tower energy (GeV)");
  leg->AddEntry(h_energyHFminus_Coll_HLT_PHY,"p+p (7 TeV) Unpaired","p");


  //  h_energyHFplus_Unpaired->Draw("p");
  //  h_energyHFplus_NoColl->Draw("psames");

  h_energyHFminus_Coll_HLT_ZB->Draw("p");
  h_energyHFminus_Coll_HLT_PHY->Draw("psames");
  leg->Draw("psames");
  canvasHFMinus->SaveAs("h_energyHFminus.root");


}
