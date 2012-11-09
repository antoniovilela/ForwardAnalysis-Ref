{

  TFile* fileColl = TFile::Open("analysisHistos_CaloTowerAnalysis_0.root");
  TFile* fileNoColl = TFile::Open("analysisHistos_CaloTowerAnalysis_0.root");
  TFile* fileUnpaired = TFile::Open("analysisHistos_CaloTowerAnalysis_0.root");
 
  /*std::string labelColl = "p+p (7 TeV) Collision";
  std::string labelNoColl = "p+p (7 TeV) No Collision";
  std::string labelUnpaired = "p+p (7 TeV) Unpaired";*/

  TH1F* h_NEventsColl = static_cast<TH1F*>(fileColl->Get((std::string("caloTowerAnalysisZeroBias") + "/NEvents").c_str()));
  TH1F* h_NEventsNoColl = static_cast<TH1F*>(fileNoColl->Get((std::string("caloTowerAnalysisZeroBiasNoTrk") + "/NEvents").c_str()));
  TH1F* h_NEventsUnpaired =   static_cast<TH1F*>(fileUnpaired->Get((std::string("caloTowerAnalysisBPTXPlusOrMinusOnly") + "/NEvents").c_str()));

  TCanvas* canvasHF = new TCanvas("canvasHF","Energy HF");
  TLegend* leg = new TLegend(0.7597956,0.822335,0.9531857,0.9549239,NULL,"brNDC");
  canvasHF->cd();

  TH1F* h_energyHF_Unpaired = static_cast<TH1F*>(fileUnpaired->Get((std::string("caloTowerAnalysisBPTXPlusOrMinusOnly") + "/energyHF").c_str()));
  h_energyHF_Unpaired->Scale(1./h_NEventsUnpaired->GetBinContent(1));
  h_energyHF_Unpaired->SetMarkerColor(kRed);
  h_energyHF_Unpaired->SetMarkerStyle(20);
  h_energyHF_Unpaired->SetMarkerSize(1.1);
  h_energyHF_Unpaired->GetYaxis()->SetTitle("1/NEvents");
  h_energyHF_Unpaired->GetXaxis()->SetTitle("HF tower energy (GeV)");
  leg->AddEntry(h_energyHF_Unpaired,"p+p (7 TeV) Unpaired","p");

  
  TH1F* h_energyHF_NoColl = static_cast<TH1F*>(fileNoColl->Get((std::string("caloTowerAnalysisZeroBiasNoTrk") + "/energyHF").c_str()));
  h_energyHF_NoColl->Scale(1./h_NEventsNoColl->GetBinContent(1));
  h_energyHF_NoColl->SetMarkerColor(kBlue);
  h_energyHF_NoColl->SetMarkerStyle(24);
  h_energyHF_NoColl->SetMarkerSize(1.1);
  h_energyHF_NoColl->GetYaxis()->SetTitle("1/NEvents");
  h_energyHF_NoColl->GetXaxis()->SetTitle("HF tower energy (GeV)");
  leg->AddEntry(h_energyHF_NoColl,"p+p (7 TeV) No Collisions","p");


  TH1F* h_energyHF_Coll = static_cast<TH1F*>(fileColl->Get((std::string("caloTowerAnalysisZeroBias") + "/energyHF").c_str()));
  h_energyHF_Coll->Scale(1./h_NEventsColl->GetBinContent(1));
  h_energyHF_Coll->SetMarkerColor(kBlack);
  h_energyHF_Coll->SetMarkerStyle(28);
  h_energyHF_Coll->SetMarkerSize(1.1);
  h_energyHF_Coll->GetYaxis()->SetTitle("1/NEvents");
  h_energyHF_Coll->GetXaxis()->SetTitle("HF tower energy (GeV)");
  leg->AddEntry(h_energyHF_Coll,"p+p (7 TeV) Collisions","p");

  h_energyHF_Unpaired->Draw("p");
  h_energyHF_NoColl->Draw("psames");
  h_energyHF_Coll->Draw("psames");
  leg->Draw("psames");
//-----------------------------------------------

 //  Calculated of the threshold of the CaloTowers
 TH1F* h_energyHF_95 = static_cast<TH1F*>(fileUnpaired->Get((std::string("caloTowerAnalysisBPTXPlusOrMinusOnly") + "/energyHF").c_str()));


  Bool_t debug = false;

  Double_t xmin = h_energyHF_95->GetXaxis()->GetXmin();
  Double_t xmax = h_energyHF_95->GetXaxis()->GetXmax();

  TAxis *axis =  h_energyHF_95->GetXaxis();
  Int_t bin_X_min = axis->FindBin(xmin);
  Int_t bin_X_max = axis->FindBin(xmax);

  Double_t binCenter_x_min = h_energyHF_95->GetBinCenter(bin_X_min);
  Double_t binCenter_x_max = h_energyHF_95->GetBinCenter(bin_X_max-1);

  Double_t integral = h_energyHF_95->Integral(bin_X_min,bin_X_max-1);
  if(debug){
  cout << "xmin: "<< xmin <<endl;
  cout << "xmax: "<< xmax <<endl;
  cout << "bin_X_min: "<< bin_X_min<<" "<< "binCenter_x_min: "<< binCenter_x_min << endl;
  cout << "bin_X_max: "<< bin_X_max-1<<" "<< "binCenter_x_max: "<< binCenter_x_max <<  endl;
  cout << "Integral :" << integral<< endl;
  }
  Double_t choose,test;
  choose = integral*0.99;

//  Double_t test;
  for(Int_t i= 0;i <= bin_X_max-1;i++){

  test=0;
  test =  h_energyHF_95->Integral(0,i);

if(test > choose){

   Double_t value_limit_95 =  axis->GetBinCenter(i);
   Double_t error_limit_95 = h_energyHF_95->GetBinError(i);
  
if(debug){
    cout<< "binContent(bin number): "<<  value_limit_95 <<" "<<"bin number: "<< i << endl;
    cout<< "binwidth(bin number): "<<  error_limit_95 <<" "<<"bin number: "<< i << endl;
    cout<< "BinCenter(bin number): "<<  value_limit_95 <<" "<<"bin number: "<< i << endl;
    cout<< "BinError(bin number): "<<  error_limit_95 <<" "<<"bin number: "<< i << endl;

}
    break;
   }


}

//-----------------------------------------------
  Double_t maxvalue=h_energyHF_Unpaired->GetMaximum();
  cout<<"Threshold of the CaloTowers in HF using Unpaired and NoCollisions sel. (95%): "<< value_limit_95 <<"+/- "<< error_limit_95 <<" in GeV " << endl;
  // Show Thresold in histogram using HLT_ZeroBias
  cout<<"Drawing line in histogram using Unpaired events with 95 percent"<<endl;

  TLine *l=new TLine(value_limit_95,0,value_limit_95,maxvalue);  
  l->SetLineColor(6);
  l->SetLineStyle(7);
  l->SetLineWidth(2);
  l->Draw();
  canvasHF->SaveAs("h_energyHF_Unpaired.root");

//----------------------------------------------
  TCanvas* canvasHFPlus = new TCanvas("canvasHFPlus","Energy HF Plus");
  TLegend* leg = new TLegend(0.7597956,0.822335,0.9531857,0.9549239,NULL,"brNDC");
  canvasHFPlus->cd();

  TH1F* h_energyHFplus_Unpaired = static_cast<TH1F*>(fileUnpaired->Get((std::string("caloTowerAnalysisBPTXPlusOrMinusOnly") + "/energyHFplus").c_str()));
  h_energyHFplus_Unpaired->Scale(1./h_NEventsUnpaired->GetBinContent(1));
  h_energyHFplus_Unpaired->SetMarkerColor(kRed);
  h_energyHFplus_Unpaired->SetMarkerStyle(20);
  h_energyHFplus_Unpaired->SetMarkerSize(1.1);
  h_energyHFplus_Unpaired->GetYaxis()->SetTitle("1/NEvents");
  h_energyHFplus_Unpaired->GetXaxis()->SetTitle("HF(Plus) tower energy (GeV)");
  leg->AddEntry(h_energyHFplus_Unpaired,"p+p (7 TeV) Unpaired","p");

  
  TH1F* h_energyHFplus_NoColl = static_cast<TH1F*>(fileNoColl->Get((std::string("caloTowerAnalysisZeroBiasNoTrk") + "/energyHFplus").c_str()));
  h_energyHFplus_NoColl->Scale(1./h_NEventsNoColl->GetBinContent(1));
  h_energyHFplus_NoColl->SetMarkerColor(kBlue);
  h_energyHFplus_NoColl->SetMarkerStyle(24);
  h_energyHFplus_NoColl->SetMarkerSize(1.1);
  h_energyHFplus_NoColl->GetYaxis()->SetTitle("1/NEvents");
  h_energyHFplus_NoColl->GetXaxis()->SetTitle("HF(Plus) tower energy (GeV)");
  leg->AddEntry(h_energyHFplus_NoColl,"p+p (7 TeV) No Collisions","p");


  TH1F* h_energyHFplus_Coll = static_cast<TH1F*>(fileColl->Get((std::string("caloTowerAnalysisZeroBias") + "/energyHFplus").c_str()));
  h_energyHFplus_Coll->Scale(1./h_NEventsColl->GetBinContent(1));
  h_energyHFplus_Coll->SetMarkerColor(kBlack);
  h_energyHFplus_Coll->SetMarkerStyle(25);
  h_energyHFplus_Coll->SetMarkerSize(1.1);
  h_energyHFplus_Coll->GetYaxis()->SetTitle("1/NEvents");
  h_energyHFplus_Coll->GetXaxis()->SetTitle("HF(Plus) tower energy (GeV)");
  leg->AddEntry(h_energyHFplus_Coll,"p+p (7 TeV) Collisions","p");

  h_energyHFplus_Unpaired->Draw("p");
  h_energyHFplus_NoColl->Draw("psames");
  h_energyHFplus_Coll->Draw("psames");
  leg->Draw("psames");
//-----------------------------------------------
  TCanvas* canvasHFMinus = new TCanvas("canvasHFMinus","Energy HF Minus");
  TLegend* leg = new TLegend(0.7597956,0.822335,0.9531857,0.9549239,NULL,"brNDC");
  canvasHFMinus->cd();

  TH1F* h_energyHFminus_Unpaired = static_cast<TH1F*>(fileUnpaired->Get((std::string("caloTowerAnalysisBPTXPlusOrMinusOnly") + "/energyHFminus").c_str()));
  h_energyHFminus_Unpaired->Scale(1./h_NEventsUnpaired->GetBinContent(1));
  h_energyHFminus_Unpaired->SetMarkerColor(kRed);
  h_energyHFminus_Unpaired->SetMarkerStyle(20);
  h_energyHFminus_Unpaired->SetMarkerSize(1.1);
  h_energyHFminus_Unpaired->GetYaxis()->SetTitle("1/NEvents");
  h_energyHFminus_Unpaired->GetXaxis()->SetTitle("HF(Minus) tower energy (GeV)");
  leg->AddEntry(h_energyHFminus_Unpaired,"p+p (7 TeV) Unpaired","p");

  TH1F* h_energyHFminus_NoColl = static_cast<TH1F*>(fileNoColl->Get((std::string("caloTowerAnalysisZeroBiasNoTrk") + "/energyHFminus").c_str()));
  h_energyHFminus_NoColl->Scale(1./h_NEventsNoColl->GetBinContent(1));
  h_energyHFminus_NoColl->SetMarkerColor(kBlue);
  h_energyHFminus_NoColl->SetMarkerStyle(24);
  h_energyHFminus_NoColl->SetMarkerSize(1.1);
  h_energyHFminus_NoColl->GetYaxis()->SetTitle("1/NEvents");
  h_energyHFminus_NoColl->GetXaxis()->SetTitle("HF(Minus) tower energy (GeV)");
  leg->AddEntry(h_energyHFminus_NoColl,"p+p (7 TeV) No Collisions","p");

  TH1F* h_energyHFminus_Coll = static_cast<TH1F*>(fileColl->Get((std::string("caloTowerAnalysisZeroBias") + "/energyHFminus").c_str()));
  h_energyHFminus_Coll->Scale(1./h_NEventsColl->GetBinContent(1));
  h_energyHFminus_Coll->SetMarkerColor(kBlack);
  h_energyHFminus_Coll->SetMarkerStyle(25);
  h_energyHFminus_Coll->SetMarkerSize(1.1);
  h_energyHFminus_Coll->GetYaxis()->SetTitle("1/NEvents");
  h_energyHFminus_Coll->GetXaxis()->SetTitle("HF(Minus) tower energy (GeV)");
  leg->AddEntry(h_energyHFminus_Coll,"p+p (7 TeV) Collisions","p");

  h_energyHFminus_Unpaired->Draw("p");
  h_energyHFminus_NoColl->Draw("psames");
  h_energyHFminus_Coll->Draw("psames");
  leg->Draw("psames");




}
