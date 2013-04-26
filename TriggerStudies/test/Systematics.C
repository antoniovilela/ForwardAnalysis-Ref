#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TKey.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <float.h>


//
// Efficiency and Plotter
//
////////////////////////////////////


void Systematics(){
  //PurityTriggerService("histo_purity_nopresel_jet2_pT10.root",0);
  //TriggerEfficiency("histo_effTriggerMultijetsRunB_RefDijetAve50U_And30U_270313pT5030.root","effTriggerMultijetsRunB_RefDijetAve50U_And30U_pT50_30.root");
  //EachCutEfficiency("histo_effCutsMinBias2010RunB_castor.root",0);
  //TriggerEfficiencyMerge("histo_effTriggerMultijetsRunB_RefDijetAve50U_Or30U_castor.root","histo_effTriggerMultijetsRunB_RefOr30U_And30U_castor.root","histo_effTriggerMultijetsRunB_RefDijetAve50U_And30U_castor.root");
  Systematic("histo_effTriggerMultijetsRunB_RefDijetAve50U_And30U_castor.root", "sigmaPlusHLTDijet50_And30U_pT60_castor.root","sigmaMinusHLTDijet50_And30U_pT60_castor.root");
}


void EachCutEfficiency(TString name, bool logscale){

  TFile *l1  = TFile::Open(name);
  gStyle->SetOptStat(0);

  TCanvas* c1 = new TCanvas("c1","c1",700,500);
  TLegend* leg = new TLegend(0.7597956,0.822335,0.9931857,0.9949239,NULL,"brNDC");

  if(logscale) c1->SetLogy(1);

  TH1F* h_without_cuts = (TH1F*)l1->Get("Events_without_cuts");
  TH1F* h_Trigger = (TH1F*)l1->Get("Events_with_trigger");
  TH1F* h_Triggerpresel = (TH1F*)l1->Get("Events_with_trigger_presel");
  TH1F* h_Triggerpreselvertex = (TH1F*)l1->Get("Events_with_trigger_presel_vertex");
  TH1F* h_step4_4 = (TH1F*)l1->Get("Events_All_step4_4");
  TH1F* h_step4_3 = (TH1F*)l1->Get("Events_All_step4_3");
  TH1F* h_step4_2 = (TH1F*)l1->Get("Events_All_step4_2");
  TH1F* h_step4_1 = (TH1F*)l1->Get("Events_All_step4_1");

  TH1F *ratiopresel = h_Trigger->Clone(); 
  ratiopresel->SetName("RatioPreSel");
  ratiopresel->Divide(h_Triggerpresel,h_Trigger,1.,1.,"B");
  ratiopresel->GetYaxis()->SetTitle("#frac{N^{pass,cut}_{ZeroBias}}{N^{total}_{ZeroBias}}");
  ratiopresel->GetYaxis()->SetTitleOffset(1.1);
  ratiopresel->GetYaxis()->SetTitleSize(0.03);
  ratiopresel->GetXaxis()->SetTitle("L_{Bunch} [#mub^{-1}s^{-1}]");
  ratiopresel->GetXaxis()->SetRangeUser(0,0.8);
  ratiopresel->SetTitle("Efficiency, Cut: #sum E_HF^{+}<30 GeV and #sum E_HF^{-}<30 GeV");
  ratiopresel->Draw();

  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_presel.png"));
  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_presel.C"));

  TH1F *ratiovertex = h_Trigger->Clone();
  ratiovertex->SetName("RatioVertex"); 
  ratiovertex->Divide(h_Triggerpreselvertex,h_Trigger,1.,1.,"B");
  ratiovertex->GetYaxis()->SetTitle("#frac{N^{pass,cut}_{ZeroBias}}{N^{total}_{ZeroBias}}");
  ratiovertex->GetYaxis()->SetTitleOffset(1.1);
  ratiovertex->GetYaxis()->SetTitleSize(0.03);
  ratiovertex->GetXaxis()->SetTitle("L_{Bunch} [#mub^{-1}s^{-1}]");
  ratiovertex->SetTitle("Efficiency, Cut: #sum E_HF^{+}< 30 GeV and #sum E_HF^{-}< 30 GeV and Vertex");
  ratiovertex->GetXaxis()->SetRangeUser(0,0.8);
  ratiovertex->Draw();

  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_vertex.png"));
  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_vertex.C"));

  TH1F *ratiostep4_4 = h_Trigger->Clone(); 
  ratiostep4_4->SetName("RatioStep4_4"); 
  ratiostep4_4->Divide(h_step4_4,h_Trigger,1.,1.,"B");
  ratiostep4_4->GetYaxis()->SetTitle("#frac{N^{pass,cut}_{ZeroBias}}{N^{total}_{ZeroBias}}");
  ratiostep4_4->GetYaxis()->SetTitleOffset(1.1);
  ratiostep4_4->GetYaxis()->SetTitleSize(0.03);
  ratiostep4_4->GetXaxis()->SetTitle("L_{Bunch} [#mub^{-1}s^{-1}]");
  ratiostep4_4->SetTitle("Efficiency, Cut: #sum E_HF^{+}< 30 GeV and #sum E_HF^{-}< 30 GeV and Vertex and |#eta_{pf(max,min)}|<4");
  ratiostep4_4->GetXaxis()->SetRangeUser(0,0.8);
  ratiostep4_4->Draw();

  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_step4_4.png"));
  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_step4_4.C"));

  TH1F *ratiostep4_3 = h_Trigger->Clone(); 
  ratiostep4_3->SetName("RatioStep4_3"); 
  ratiostep4_3->Divide(h_step4_3,h_Trigger,1.,1.,"B");
  ratiostep4_3->GetYaxis()->SetTitle("#frac{N^{pass,cut}_{ZeroBias}}{N^{total}_{ZeroBias}}");
  ratiostep4_3->GetYaxis()->SetTitleOffset(1.1);
  ratiostep4_3->GetYaxis()->SetTitleSize(0.03);
  ratiostep4_3->GetXaxis()->SetTitle("L_{Bunch} [#mub^{-1}s^{-1}]");
  ratiostep4_3->SetTitle("Efficiency, Cut: #sum E_HF^{+}< 30 GeV and #sum E_HF^{-}< 30 GeV and Vertex and |#eta_{pf(max,min)}|<3");
  ratiostep4_3->GetXaxis()->SetRangeUser(0,0.8);
  ratiostep4_3->Draw();

  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_step4_3.png"));
  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_step4_3.C"));

  TH1F *ratiostep4_2 = h_Trigger->Clone(); 
  ratiostep4_2->SetName("RatioStep4_2"); 
  ratiostep4_2->Divide(h_step4_2,h_Trigger,1.,1.,"B");
  ratiostep4_2->GetYaxis()->SetTitle("#frac{N^{pass,cut}_{ZeroBias}}{N^{total}_{ZeroBias}}");
  ratiostep4_2->GetYaxis()->SetTitleOffset(1.1);
  ratiostep4_2->GetYaxis()->SetTitleSize(0.03);
  ratiostep4_2->GetXaxis()->SetTitle("L_{Bunch} [#mub^{-1}s^{-1}]");
  ratiostep4_2->SetTitle("Efficiency, Cut: #sum E_HF^{+}< 30 GeV and #sum E_HF^{-}< 30 GeV and Vertex and |#eta_{pf(max,min)}|<2");
  ratiostep4_2->GetXaxis()->SetRangeUser(0,0.8);
  ratiostep4_2->Draw();

  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_step4_2.png"));
  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_step4_2.C"));

  TH1F *ratiostep4_1 = h_Trigger->Clone(); 
  ratiostep4_1->SetName("RatioStep4_1"); 
  ratiostep4_1->Divide(h_step4_1,h_Trigger,1.,1.,"B");
  ratiostep4_1->GetYaxis()->SetTitle("#frac{N^{pass,cut}_{ZeroBias}}{N^{total}_{ZeroBias}}");
  ratiostep4_1->GetYaxis()->SetTitleOffset(1.1);
  ratiostep4_1->GetYaxis()->SetTitleSize(0.03);
  ratiostep4_1->GetXaxis()->SetTitle("L_{Bunch} [#mub^{-1}s^{-1}]");
  ratiostep4_1->SetTitle("Efficiency, Cut: #sum E_HF^{+}< 30 GeV and #sum E_HF^{-}< 30 GeV and Vertex and |#eta_{pf(max,min)}|<1");
  ratiostep4_1->GetXaxis()->SetRangeUser(0,0.8);
  ratiostep4_1->Draw();

  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_step4_1.png"));
  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_step4_1.C"));

  TString filenameroot(name);
  File = new TFile(filenameroot.TString::ReplaceAll("histo_effCuts","eff"),"RECREATE");
  File->cd();

  // Protection
  setHBins(ratiopresel);
  setHBins(ratiovertex);
  setHBins(ratiostep4_4);
  setHBins(ratiostep4_3);
  setHBins(ratiostep4_2);
  setHBins(ratiostep4_1);

  ratiopresel->Write();
  ratiovertex->Write();
  ratiostep4_4->Write();
  ratiostep4_3->Write();
  ratiostep4_2->Write();
  ratiostep4_1->Write();
  File->Close();

}

void PurityTriggerService(TString name, bool logscale){

  TFile *l1  = TFile::Open(name);
  gStyle->SetOptStat(0);

  TCanvas* c1 = new TCanvas("c1","c1",700,500);
  TLegend* leg = new TLegend(0.7597956,0.822335,0.9931857,0.9949239,NULL,"brNDC");

  if(logscale) c1->SetLogy(1);

  TH1F* h_events_jet1pt_trigger = (TH1F*)l1->Get("Events_jet1pt_with_trigger");
  TH1F* h_events_jet1eta_trigger = (TH1F*)l1->Get("Events_jet1eta_with_trigger");
  TH1F* h_events_jet1phi_trigger = (TH1F*)l1->Get("Events_jet1phi_with_trigger");

  TH1F* h_events_jet2pt_trigger = (TH1F*)l1->Get("Events_jet2pt_with_trigger");
  TH1F* h_events_jet2eta_trigger = (TH1F*)l1->Get("Events_jet2eta_with_trigger");
  TH1F* h_events_jet2phi_trigger = (TH1F*)l1->Get("Events_jet2phi_with_trigger");

  TH1F* h_events_preminus_without_cuts = (TH1F*)l1->Get("Events_preminus_without_cuts");
  TH1F* h_events_preplus_without_cuts = (TH1F*)l1->Get("Events_preplus_without_cuts");

  TH1F* h_events_preminus_with_trigger = (TH1F*)l1->Get("Events_preminus_with_trigger");
  TH1F* h_events_preplus_with_trigger = (TH1F*)l1->Get("Events_preplus_with_trigger");

  TH1F* h_events_preminus_with_trigger_presel = (TH1F*)l1->Get("Events_preminus_with_trigger_presel");
  TH1F* h_events_preplus_with_trigger_presel = (TH1F*)l1->Get("Events_preplus_with_trigger_presel");

  TH1F* h_events_jet1pt_trigger_dijets = (TH1F*)l1->Get("Events_jet1pt_with_trigger_dijets");
  TH1F* h_events_jet1eta_trigger_dijets = (TH1F*)l1->Get("Events_jet1eta_with_trigger_dijets");
  TH1F* h_events_jet1phi_trigger_dijets = (TH1F*)l1->Get("Events_jet1phi_with_trigger_dijets");

  TH1F* h_events_jet2pt_trigger_dijets = (TH1F*)l1->Get("Events_jet2pt_with_trigger_dijets");
  TH1F* h_events_jet2eta_trigger_dijets = (TH1F*)l1->Get("Events_jet2eta_with_trigger_dijets");
  TH1F* h_events_jet2phi_trigger_dijets = (TH1F*)l1->Get("Events_jet2phi_with_trigger_dijets");

  TH1F *ratiojet1pt = h_events_jet1pt_trigger->Clone(); 
  ratiojet1pt->SetName("ratiojet1pt");
  ratiojet1pt->Divide(h_events_jet1pt_trigger_dijets,h_events_jet1pt_trigger,1.,1.,"B");
  ratiojet1pt->GetYaxis()->SetTitle("Purity");
  ratiojet1pt->GetYaxis()->SetTitleOffset(1.1);
  ratiojet1pt->GetYaxis()->SetTitleSize(0.03);
  ratiojet1pt->SetTitle("Leading Jet pT Distribution for Second Jet pT > 15 GeV");
  ratiojet1pt->Draw();

  TH1F *ratiojet1eta = h_events_jet1eta_trigger->Clone(); 
  ratiojet1eta->SetName("ratiojet1eta");
  ratiojet1eta->Divide(h_events_jet1eta_trigger_dijets,h_events_jet1eta_trigger,1.,1.,"B");
  ratiojet1eta->GetYaxis()->SetTitle("Purity");
  ratiojet1eta->GetYaxis()->SetTitleOffset(1.1);
  ratiojet1eta->SetTitle("Leading Jet pT Distribution for Second Jet pT > 15 GeV");
  ratiojet1eta->GetYaxis()->SetTitleSize(0.03);
  ratiojet1eta->Draw();

  TH1F *ratiojet1phi = h_events_jet1phi_trigger->Clone(); 
  ratiojet1phi->SetName("ratiojet1phi");
  ratiojet1phi->Divide(h_events_jet1phi_trigger_dijets,h_events_jet1phi_trigger,1.,1.,"B");
  ratiojet1phi->GetYaxis()->SetTitle("Purity");
  ratiojet1phi->GetYaxis()->SetTitleOffset(1.1);
  ratiojet1phi->GetYaxis()->SetTitleSize(0.03);
  ratiojet1phi->SetTitle("Leading Jet pT Distribution for Second Jet pT > 15 GeV");
  ratiojet1phi->Draw();

  TH1F *ratiojet2pt = h_events_jet2pt_trigger->Clone(); 
  ratiojet2pt->SetName("ratiojet2pt");
  ratiojet2pt->Divide(h_events_jet2pt_trigger_dijets,h_events_jet2pt_trigger,1.,1.,"B");
  ratiojet2pt->GetYaxis()->SetTitle("Purity");
  ratiojet2pt->GetYaxis()->SetTitleOffset(1.1);
  ratiojet2pt->GetYaxis()->SetTitleSize(0.03);
  ratiojet2pt->SetTitle("Second Jet pT Distribution for Second Jet pT > 15 GeV");
  ratiojet2pt->Draw();

  TH1F *ratiojet2eta = h_events_jet2eta_trigger->Clone(); 
  ratiojet2eta->SetName("ratiojet2eta");
  ratiojet2eta->Divide(h_events_jet2eta_trigger_dijets,h_events_jet2eta_trigger,1.,1.,"B");
  ratiojet2eta->GetYaxis()->SetTitle("Purity");
  ratiojet2eta->GetYaxis()->SetTitleOffset(1.1);
  ratiojet2eta->GetYaxis()->SetTitleSize(0.03);
  ratiojet2eta->SetTitle("Second Jet pT Distribution for Second Jet pT > 15 GeV");
  ratiojet2eta->Draw();

  TH1F *ratiojet2phi = h_events_jet2phi_trigger->Clone(); 
  ratiojet2phi->SetName("ratiojet2phi");
  ratiojet2phi->Divide(h_events_jet2phi_trigger_dijets,h_events_jet2phi_trigger,1.,1.,"B");
  ratiojet2phi->GetYaxis()->SetTitle("Purity");
  ratiojet2phi->GetYaxis()->SetTitleOffset(1.1);
  ratiojet2phi->GetYaxis()->SetTitleSize(0.03);
  ratiojet2phi->SetTitle("Second Jet pT Distribution for Second Jet pT > 15 GeV");
  ratiojet2phi->Draw();

  TH1F *ratiosumMwc = h_events_preminus_without_cuts->Clone(); 
  ratiosumMwc->SetName("ratiosumMinuswc");
  ratiosumMwc->Divide(h_events_preminus_with_trigger,h_events_preminus_without_cuts,1.,1.,"B");
  ratiosumMwc->GetYaxis()->SetTitle("Purity");
  ratiosumMwc->GetYaxis()->SetTitleOffset(1.1);
  ratiosumMwc->GetYaxis()->SetTitleSize(0.03);
  ratiosumMwc->SetTitle("Sum Minus HF, #frac{# triggered}{# All Events}");
  ratiosumMwc->Draw();

  TH1F *ratiosumPwc = h_events_preplus_without_cuts->Clone(); 
  ratiosumPwc->SetName("ratiosumPluswc");
  ratiosumPwc->Divide(h_events_preplus_with_trigger,h_events_preplus_without_cuts,1.,1.,"B");
  ratiosumPwc->GetYaxis()->SetTitle("Purity");
  ratiosumPwc->GetYaxis()->SetTitleOffset(1.1);
  ratiosumPwc->GetYaxis()->SetTitleSize(0.03);
  ratiosumPwc->SetTitle("Sum Plus HF, #frac{# triggered}{# All Events}");
  ratiosumPwc->Draw();

  TH1F *ratiosumMtriggerpre = h_events_preminus_with_trigger->Clone(); 
  ratiosumMtriggerpre->SetName("ratiosumMinusTriggerPre");
  ratiosumMtriggerpre->Divide(h_events_preminus_with_trigger_presel,h_events_preminus_with_trigger,1.,1.,"B");
  ratiosumMtriggerpre->GetYaxis()->SetTitle("Purity");
  ratiosumMtriggerpre->GetYaxis()->SetTitleOffset(1.1);
  ratiosumMtriggerpre->GetYaxis()->SetTitleSize(0.03);
  ratiosumMtriggerpre->SetTitle("Sum Minus HF, #frac{# triggered+presel}{# Triggered}");
  ratiosumMtriggerpre->Draw();

  TH1F *ratiosumPtriggerpre = h_events_preplus_with_trigger->Clone(); 
  ratiosumPtriggerpre->SetName("ratiosumPlusTriggerPre");
  ratiosumPtriggerpre->Divide(h_events_preplus_with_trigger_presel,h_events_preplus_with_trigger,1.,1.,"B");
  ratiosumPtriggerpre->GetYaxis()->SetTitle("Purity");
  ratiosumPtriggerpre->GetYaxis()->SetTitleOffset(1.1);
  ratiosumPtriggerpre->GetYaxis()->SetTitleSize(0.03);
  ratiosumPtriggerpre->SetTitle("Sum Plus HF, #frac{# triggered+presel}{# Triggered}");
  ratiosumPtriggerpre->Draw();

  TString filename(name);
  c1->SaveAs(filename.TString::ReplaceAll(".root","_presel.png"));
  //TString filename(name);
  //c1->SaveAs(filename.TString::ReplaceAll(".root","_presel.C"));

  // Protection
/*
  setHBins(ratiojet1pt);
  setHBins(ratiojet1eta);
  setHBins(ratiojet1phi);
  setHBins(ratiojet2pt);
  setHBins(ratiojet2eta);
  setHBins(ratiojet2phi);
*/

  TString filenameroot(name);
  File = new TFile(filenameroot.TString::ReplaceAll("histo_purity_","purity_pre"),"RECREATE");
  File->cd();
  ratiojet1pt->Write();
  ratiojet1eta->Write();
  ratiojet1phi->Write();
  ratiojet2pt->Write();
  ratiojet2eta->Write();
  ratiojet2phi->Write();
  ratiosumMwc->Write();
  ratiosumMtriggerpre->Write();
  ratiosumPwc->Write();
  ratiosumPtriggerpre->Write();

  File->Close();

}

void TriggerEfficiency(TString file1, TString output1){

  TFile *l1  = TFile::Open(file1);

  TH1F* h1_without_cuts = (TH1F*)l1->Get("Events_without_cuts");
  TH1F* h1_RefTrigger = (TH1F*)l1->Get("Events_with_RefTrigger");
  TH1F* h1_RefTriggerCutEta4 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLine_eta4");
  TH1F* h1_RefTriggerCutEta3 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLine_eta3");
  TH1F* h1_RefTriggerCutEta2 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLine_eta2");
  TH1F* h1_RefTriggerCutEta1 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLine_eta1");
  TH1F* h1_RefTriggerCutAndTriggerEta4 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta4");
  TH1F* h1_RefTriggerCutAndTriggerEta3 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta3");
  TH1F* h1_RefTriggerCutAndTriggerEta2 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta2");
  TH1F* h1_RefTriggerCutAndTriggerEta1 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta1");

  //Eff1
  TH1F *h1_ratiorefer = h1_RefTrigger->Clone(); 
  h1_ratiorefer->Divide(h1_RefTrigger,h1_RefTrigger,1.,1.,"B");

  TH1F *h1_ratioRefTriggerCutAndTriggerEta4 = h1_RefTriggerCutAndTriggerEta4->Clone(); 
  h1_ratioRefTriggerCutAndTriggerEta4->Divide(h1_RefTriggerCutAndTriggerEta4,h1_RefTriggerCutEta4,1.,1.,"B");

  TH1F *h1_ratioRefTriggerCutAndTriggerEta3 = h1_RefTriggerCutAndTriggerEta3->Clone(); 
  h1_ratioRefTriggerCutAndTriggerEta3->Divide(h1_RefTriggerCutAndTriggerEta3,h1_RefTriggerCutEta3,1.,1.,"B");

  TH1F *h1_ratioRefTriggerCutAndTriggerEta2 = h1_RefTriggerCutAndTriggerEta2->Clone(); 
  h1_ratioRefTriggerCutAndTriggerEta2->Divide(h1_RefTriggerCutAndTriggerEta2,h1_RefTriggerCutEta2,1.,1.,"B");

  TH1F *h1_ratioRefTriggerCutAndTriggerEta1 = h1_RefTriggerCutAndTriggerEta1->Clone(); 
  h1_ratioRefTriggerCutAndTriggerEta1->Divide(h1_RefTriggerCutAndTriggerEta1,h1_RefTriggerCutEta1,1.,1.,"B");

  setHBins(h1_ratiorefer);
  setHBins(h1_ratioRefTriggerCutAndTriggerEta4);
  setHBins(h1_ratioRefTriggerCutAndTriggerEta3);
  setHBins(h1_ratioRefTriggerCutAndTriggerEta2);
  setHBins(h1_ratioRefTriggerCutAndTriggerEta1);

  File = new TFile(output1,"RECREATE");
  File->cd();
  h1_ratiorefer->Write();
  h1_ratioRefTriggerCutAndTriggerEta4->Write();
  h1_ratioRefTriggerCutAndTriggerEta3->Write();
  h1_ratioRefTriggerCutAndTriggerEta2->Write();
  h1_ratioRefTriggerCutAndTriggerEta1->Write();
  File->Close();

}

void TriggerEfficiencyMerge(TString file1, TString file2, TString output){

  TFile *l1  = TFile::Open(file1);
  TFile *l2  = TFile::Open(file2);

  TH1F* h1_without_cuts = (TH1F*)l1->Get("Events_without_cuts");
  TH1F* h1_RefTrigger = (TH1F*)l1->Get("Events_with_RefTrigger");
  TH1F* h1_RefTriggerCutEta4 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLine_eta4");
  TH1F* h1_RefTriggerCutEta3 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLine_eta3");
  TH1F* h1_RefTriggerCutEta2 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLine_eta2");
  TH1F* h1_RefTriggerCutEta1 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLine_eta1");
  TH1F* h1_RefTriggerCutAndTriggerEta4 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta4");
  TH1F* h1_RefTriggerCutAndTriggerEta3 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta3");
  TH1F* h1_RefTriggerCutAndTriggerEta2 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta2");
  TH1F* h1_RefTriggerCutAndTriggerEta1 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta1");

  TH1F* h2_without_cuts = (TH1F*)l2->Get("Events_without_cuts");
  TH1F* h2_RefTrigger = (TH1F*)l2->Get("Events_with_RefTrigger");
  TH1F* h2_RefTriggerCutEta4 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLine_eta4");
  TH1F* h2_RefTriggerCutEta3 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLine_eta3");
  TH1F* h2_RefTriggerCutEta2 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLine_eta2");
  TH1F* h2_RefTriggerCutEta1 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLine_eta1");
  TH1F* h2_RefTriggerCutAndTriggerEta4 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta4");
  TH1F* h2_RefTriggerCutAndTriggerEta3 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta3");
  TH1F* h2_RefTriggerCutAndTriggerEta2 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta2");
  TH1F* h2_RefTriggerCutAndTriggerEta1 = (TH1F*)l2->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta1");

  //Efficiency File1 Calculation
  TH1F *h1_ratiorefer = h1_RefTrigger->Clone(); 
  h1_ratiorefer->Divide(h1_RefTrigger,h1_RefTrigger,1.,1.,"B");

  TH1F *h1_ratioRefTriggerCutAndTriggerEta4 = h1_RefTriggerCutAndTriggerEta4->Clone(); 
  h1_ratioRefTriggerCutAndTriggerEta4->Divide(h1_RefTriggerCutAndTriggerEta4,h1_RefTriggerCutEta4,1.,1.,"B");

  TH1F *h1_ratioRefTriggerCutAndTriggerEta3 = h1_RefTriggerCutAndTriggerEta3->Clone(); 
  h1_ratioRefTriggerCutAndTriggerEta3->Divide(h1_RefTriggerCutAndTriggerEta3,h1_RefTriggerCutEta3,1.,1.,"B");

  TH1F *h1_ratioRefTriggerCutAndTriggerEta2 = h1_RefTriggerCutAndTriggerEta2->Clone(); 
  h1_ratioRefTriggerCutAndTriggerEta2->Divide(h1_RefTriggerCutAndTriggerEta2,h1_RefTriggerCutEta2,1.,1.,"B");

  TH1F *h1_ratioRefTriggerCutAndTriggerEta1 = h1_RefTriggerCutAndTriggerEta1->Clone(); 
  h1_ratioRefTriggerCutAndTriggerEta1->Divide(h1_RefTriggerCutAndTriggerEta1,h1_RefTriggerCutEta1,1.,1.,"B");


  //Efficiency File2 Calculation
  TH1F *h2_ratiorefer = h2_RefTrigger->Clone(); 
  h2_ratiorefer->Divide(h2_RefTrigger,h2_RefTrigger,1.,1.,"B");

  TH1F *h2_ratioRefTriggerCutAndTriggerEta4 = h2_RefTriggerCutAndTriggerEta4->Clone(); 
  h2_ratioRefTriggerCutAndTriggerEta4->Divide(h2_RefTriggerCutAndTriggerEta4,h2_RefTriggerCutEta4,1.,1.,"B");

  TH1F *h2_ratioRefTriggerCutAndTriggerEta3 = h2_RefTriggerCutAndTriggerEta3->Clone(); 
  h2_ratioRefTriggerCutAndTriggerEta3->Divide(h2_RefTriggerCutAndTriggerEta3,h2_RefTriggerCutEta3,1.,1.,"B");

  TH1F *h2_ratioRefTriggerCutAndTriggerEta2 = h2_RefTriggerCutAndTriggerEta2->Clone(); 
  h2_ratioRefTriggerCutAndTriggerEta2->Divide(h2_RefTriggerCutAndTriggerEta2,h2_RefTriggerCutEta2,1.,1.,"B");

  TH1F *h2_ratioRefTriggerCutAndTriggerEta1 = h2_RefTriggerCutAndTriggerEta1->Clone(); 
  h2_ratioRefTriggerCutAndTriggerEta1->Divide(h2_RefTriggerCutAndTriggerEta1,h2_RefTriggerCutEta1,1.,1.,"B");

  // Efficiency1*Efficiency2 Calculation
  h1_ratiorefer->Multiply(h2_ratiorefer);
  h1_ratioRefTriggerCutAndTriggerEta4->Multiply(h2_ratioRefTriggerCutAndTriggerEta4);
  h1_ratioRefTriggerCutAndTriggerEta3->Multiply(h2_ratioRefTriggerCutAndTriggerEta3);
  h1_ratioRefTriggerCutAndTriggerEta2->Multiply(h2_ratioRefTriggerCutAndTriggerEta2);
  h1_ratioRefTriggerCutAndTriggerEta1->Multiply(h2_ratioRefTriggerCutAndTriggerEta1);

  // Protection
  setHBins(h1_ratiorefer);
  setHBins(h1_ratioRefTriggerCutAndTriggerEta4);
  setHBins(h1_ratioRefTriggerCutAndTriggerEta3);
  setHBins(h1_ratioRefTriggerCutAndTriggerEta2);
  setHBins(h1_ratioRefTriggerCutAndTriggerEta1);

  File = new TFile(output,"RECREATE");
  File->cd();
  h1_ratiorefer->Write();
  h1_ratioRefTriggerCutAndTriggerEta4->Write();
  h1_ratioRefTriggerCutAndTriggerEta3->Write();
  h1_ratioRefTriggerCutAndTriggerEta2->Write();
  h1_ratioRefTriggerCutAndTriggerEta1->Write();
  File->Close();

}

void Systematic(TString filein, TString outplus, TString outminus){

  TFile *l1  = TFile::Open(filein);

  //Load Histograms
  TH1F *heff_trigger4 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta4");
  TH1F *heff_trigger4plus = heff_trigger4->Clone();
  TH1F *heff_trigger4minus = heff_trigger4->Clone(); 

  TH1F *heff_trigger3 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta3");
  TH1F *heff_trigger3plus = heff_trigger3->Clone();
  TH1F *heff_trigger3minus = heff_trigger3->Clone(); 

  TH1F *heff_trigger2 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta2");
  TH1F *heff_trigger2plus = heff_trigger2->Clone();
  TH1F *heff_trigger2minus = heff_trigger2->Clone(); 

  TH1F *heff_trigger1 = (TH1F*)l1->Get("Events_with_RefTriggerCutsOffLineAndTrigger_eta1");
  TH1F *heff_trigger1plus = heff_trigger1->Clone();
  TH1F *heff_trigger1minus = heff_trigger1->Clone();  

  int nBins = heff_trigger4->GetNbinsX();
  int binFirst = heff_trigger4->FindFirstBinAbove(0,1);
  int binLast = heff_trigger4->FindLastBinAbove(0,1);

  for(Int_t ibin = 0; ibin <= nBins; ++ibin){
    float binError4 = heff_trigger4->GetBinError(ibin);
    float binContent4 = heff_trigger4->GetBinContent(ibin);
    float sigmaplus4 = binContent4 + binError4;
    float sigmaminus4 = binContent4 - binError4;
    heff_trigger4plus->SetBinContent(ibin,sigmaplus4);
    heff_trigger4minus->SetBinContent(ibin,sigmaminus4);

    float binError3 = heff_trigger3->GetBinError(ibin);
    float binContent3 = heff_trigger3->GetBinContent(ibin);
    float sigmaplus3 = binContent3 + binError3;
    float sigmaminus3 = binContent3 - binError3;
    heff_trigger3plus->SetBinContent(ibin,sigmaplus3);
    heff_trigger3minus->SetBinContent(ibin,sigmaminus3);

    float binError2 = heff_trigger2->GetBinError(ibin);
    float binContent2 = heff_trigger2->GetBinContent(ibin);
    float sigmaplus2 = binContent2 + binError2;
    float sigmaminus2 = binContent2 - binError2;
    heff_trigger2plus->SetBinContent(ibin,sigmaplus2);
    heff_trigger2minus->SetBinContent(ibin,sigmaminus2);

    float binError1 = heff_trigger1->GetBinError(ibin);
    float binContent1 = heff_trigger1->GetBinContent(ibin);
    float sigmaplus1 = binContent1 + binError1;
    float sigmaminus1 = binContent1 - binError1;
    heff_trigger1plus->SetBinContent(ibin,sigmaplus1);
    heff_trigger1minus->SetBinContent(ibin,sigmaminus1);
  }

  // Protection
  setHBins(heff_trigger4plus);
  setHBins(heff_trigger3plus);
  setHBins(heff_trigger2plus);
  setHBins(heff_trigger1plus);
  setHBins(heff_trigger4minus);
  setHBins(heff_trigger3minus);
  setHBins(heff_trigger2minus);
  setHBins(heff_trigger1minus);


  File1 = new TFile(outplus,"RECREATE");
  File1->cd();
  heff_trigger4plus->Write();
  heff_trigger3plus->Write();
  heff_trigger2plus->Write();
  heff_trigger1plus->Write();
  File1->Close();

  File2 = new TFile(outminus,"RECREATE");
  File2->cd();
  heff_trigger4minus->Write();
  heff_trigger3minus->Write();
  heff_trigger2minus->Write();
  heff_trigger1minus->Write();
  File2->Close();

}

void setHBins(TH1F* h)
{

  // function to set all bins in a histogram to non empty values
  // this is needed to ensure proper working of the likelihood ratio 
  // minimizations inside the >> theta-framework.org
  // http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/UserCode/SchieferD/ThetaTools/bin/theta_mcuncertainty_x.cc?revision=1.2&view=markup
  // MODIFIED.

  float lumiminl1=0.;
  float lumimaxl1=0.;

  lumiminl1 = h->GetXaxis()->GetBinCenter(h->FindFirstBinAbove(0,1));
  lumimaxl1 = h->GetXaxis()->GetBinCenter(h->FindLastBinAbove(0,1));
  
  /*
  cout << "LumiMinl1: " << lumiminl1 << " | LumiMaxl1: " << lumimaxl1 << endl;
  cout << "GetBinMin: " << h->GetXaxis()->FindBin(lumiminl1) << endl;
  cout << "GetBinMax: " << h->GetXaxis()->FindBin(lumimaxl1) << endl;
  */

  int minBin = h->GetXaxis()->FindBin(lumiminl1);
  int maxBin = h->GetXaxis()->FindBin(lumimaxl1);

  if (0==h || h->GetEntries() <= 0.) return;


  if (h->GetNbinsX()<=4){

    for (Int_t i=1;i<=h->GetNbinsX();i++) {

      if (h->GetBinContent(i)<FLT_EPSILON) {
	h->SetBinContent(i,h->GetBinContent(i-1)); 
	h->SetBinError(i,0.05*h->GetBinContent(i-1)); 
      }

      if (h->GetBinContent(1)<FLT_EPSILON) {
	h->SetBinContent(1,h->GetBinContent(2)); 
	h->SetBinError(1,0.05*h->GetBinContent(2)); 
      }

      if (h->GetBinContent(h->GetNbinsX())<FLT_EPSILON) {
	h->SetBinContent(h->GetNbinsX(),h->GetBinContent(h->GetNbinsX()-1)); 
	h->SetBinError(h->GetNbinsX(),0.05*h->GetBinContent(h->GetNbinsX()-1)); 
      }

    }

  }


  else{

    for (Int_t i=minBin;i>=1;i--) {
      h->SetBinContent(i,h->GetBinContent(i+1)); 
      h->SetBinError(i,0.05*h->GetBinContent(i+1)); 
    }

    for (Int_t i=maxBin;i<=h->GetNbinsX();i++){
      h->SetBinContent(i,h->GetBinContent(i-1)); 
      h->SetBinError(i,0.05*h->GetBinContent(i-1)); 
    }

    for (Int_t i=minBin;i<=maxBin;i++) {

      if (h->GetBinContent(i)<FLT_EPSILON) {
	h->SetBinContent(i,h->GetBinContent(i-1)); 
	h->SetBinError(i,0.05*h->GetBinContent(i-1)); 
      }

    }


  }

}

