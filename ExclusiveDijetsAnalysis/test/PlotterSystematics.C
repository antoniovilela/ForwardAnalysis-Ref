#include <TH1F.h>
#include <TH2F.h>
#include <THStack.h>
//#include "Styles.C"
#include <TCanvas.h>
#include <TLegend.h>
#include <TRandom.h>
#include <TMath.h>
#include "tdrstyle.C" // from 

void PlotterSystematics(){
 
        setTDRStyle();
 	//MakePlot("pTJet1_PU_Complete_with_trigger",1);
	//MakePlot("pTJet1_PU_Complete_JetsAtEta2_step3",1);
        //MakePlot("pTJet1_PU_Complete_JetsAtEta2_step4_3",1);
        //MakePlot("pTJet1_PU_Complete_JetsAtEta2_step4_4",1);
        //MakePlot("pTJet1_PU_Complete_JetsAtEta2_step4_3",1);

/*
	MakePlot("sumEHFminus_PU_Complete_JetsAtEta2_step3",1);
	MakePlot("sumEHFplus_PU_Complete_JetsAtEta2_step3",1);
	MakePlot("sumEHFminus_PU_Complete_JetsAtEta2_step4_4",1);
	MakePlot("sumEHFplus_PU_Complete_JetsAtEta2_step4_4",1);
	MakePlot("sumEHFminus_PU_Complete_JetsAtEta2_step4_3",1);
	MakePlot("sumEHFplus_PU_Complete_JetsAtEta2_step4_3",1);
*/

	/*MakePlot("pTJet2_PU_Complete_JetsAtEta2_step3",1);
	MakePlot("etaJet1_PU_Complete_JetsAtEta2_step3",0);
	MakePlot("etaJet2_PU_Complete_JetsAtEta2_step3",0);
	MakePlot("phiJet1_PU_Complete_JetsAtEta2_step3",0);
	MakePlot("phiJet2_PU_Complete_JetsAtEta2_step3",0);
	MakePlot("deltaPtJets_PU_Complete_JetsAtEta2_step3",1);
	MakePlot("deltaPhiJets_PU_Complete_JetsAtEta2_step3",1);
	MakePlot("deltaEtaJets_PU_Complete_JetsAtEta2_step3",1);
	MakePlot("RJJ_PU_Complete_JetsAtEta2_step3",1);*/
	//MakePlot("RJJ_PU_Complete_JetsAtEta2_step4_2",1);
	//MakePlot("RJJ_PU_Complete_JetsAtEta2_step4_3",1);
	//MakePlot("deltaEtamaxminPF_PU_Complete_JetsAtEta2_step3",1);
	MakePlot("deltaEtamaxminPF_PU_Complete_JetsAtEta2_step4_4",1);
	//MakePlot("deltaEtamaxminPF_PU_Complete_JetsAtEta2_step4_3",1);

	//MakePlot("pTJet1_PU_Complete_JetsAtEta2_step4_4",1);
	//MakePlot("deltaPtJets_PU_Complete_JetsAtEta2_step4_3",1);
	//MakePlot("deltaPhiJets_PU_Complete_JetsAtEta2_step4_3",1);
	//MakePlot("deltaEtaJets_PU_Complete_JetsAtEta2_step4_3",1);
	//MakePlot("pfetamax_PU_Complete_JetsAtEta2_step3",1);
	//MakePlot("pfetamax_PU_Complete_JetsAtEta2_step4_4",1);
	////MakePlot("pfetamax_PU_Complete_JetsAtEta2_step4_3",1);
	//MakePlot("pfetamin_PU_Complete_JetsAtEta2_step3",1);
	//MakePlot("pfetamin_PU_Complete_JetsAtEta2_step4_4",1);
	/////MakePlot("pfetamin_PU_Complete_JetsAtEta2_step4_3",1);

}

void MakePlot(TString name,bool logscale){

        // Data
        TFile *l1  = TFile::Open("nonehisto_multijet_HLTExclDiJet30U_AND_HFPreSel_GoodVertex_triggerRefDijet50And30_triggercorr_effcorr_68mb_24bin_pT60_60.root");
        TFile *sp  = TFile::Open("plushisto_multijet_HLTExclDiJet30U_AND_HFPreSel_GoodVertex_triggerRefDijet50And30_triggercorr_effcorr_68mb_24bin_pT60_60.root");
        TFile *sm  = TFile::Open("minushisto_multijet_HLTExclDiJet30U_AND_HFPreSel_GoodVertex_triggerRefDijet50And30_triggercorr_effcorr_68mb_24bin_pT60_60.root");

        // MC                              
        TFile *l2  = TFile::Open("histo_pythia6pu_pT60_60_multiple.root");
        TFile *l3  = TFile::Open("histo_herwigpu_pt60_60_multiple.root");
        TFile *l4 = TFile::Open("histo_pompytPLUSEMINUS60_0304.root");
        TFile *l5 = TFile::Open("histo_exhumepT60_0304.root");
        TFile *l6 = TFile::Open("histo_pomwigpT60_0304.root");

	TCanvas* c1 = new TCanvas(name,name);
	 c1->cd()->SetRightMargin(0.04);

        TLegend* leg = new TLegend(0.696, 0.35, 0.94, 0.92);
        leg->SetBorderSize(0);
        leg->SetLineStyle(0);
        leg->SetTextFont(42);
        leg->SetFillStyle(0);
        leg->SetFillColor(0);

	if(logscale) c1->SetLogy(1);

	TH1F* h_1 = (TH1F*)l1->Get(name);
	//h_1->SetLineColor(kRed);
	h_1->SetMarkerStyle(20);
	h_1->SetMarkerSize(1.3);
	h_1->GetYaxis()->SetTitle("N Events");
	leg->AddEntry(h_1,"Data 2010 Corrected","P");

        TH1F* h_sp = (TH1F*)sp->Get(name);
	h_sp->SetFillColor(10);
	h_sp->SetLineColor(10);
	h_sp->GetYaxis()->SetTitle("N Events");

	TH1F* h_sm = (TH1F*)sm->Get(name);
        h_sm->SetFillColor(5);
	h_sm->SetLineColor(0);
	h_sm->GetYaxis()->SetTitle("N Events");

	TH1F* h_2 = (TH1F*)l2->Get("deltaEtamaxminPF_PU_0_JetsAtEta2_step4_4");

        double factor = h_1->Integral()/h_2->Integral();

	h_2->SetFillColor(kRed);
        //h_2->Scale(factor);
        h_2->Scale(6.825);
        h_2->Scale(1./3.);
	//h_2->SetLineWidth(2);
	h_2->SetFillStyle(3004);
	h_2->GetYaxis()->SetTitle("N Events");
	leg->AddEntry(h_2,"Pythia 6, PU[0]","LFP");

	TH1F* h_3 = (TH1F*)l3->Get("deltaEtamaxminPF_PU_0_JetsAtEta2_step4_4");
	h_3->SetFillColor(kGreen-7);
	//h_3->SetLineWidth(2);
	h_3->SetFillStyle(3005);
	h_3->GetYaxis()->SetTitle("N Events");
	leg->AddEntry(h_3,"Herwig, PU[0]","LFP");


	TH1F* h_4 = (TH1F*)l4->Get(name);
	h_4->SetFillColor(kAzure-2);
	//h_4->SetLineWidth(2);
	h_4->SetFillStyle(3006);
	h_4->GetYaxis()->SetTitle("N Events");
	//leg->AddEntry(h_4,"Pompyt Plus and Minus","LFP");

	TH1F* h_5 = (TH1F*)l5->Get(name);
	h_5->SetFillColor(27);
        h_5->Scale(0.05);
	//h_5->SetLineWidth(2);
	h_5->SetFillStyle(3013);
	h_5->GetYaxis()->SetTitle("N Events");
	leg->AddEntry(h_5,"ExHume","LFP");	

	TH1F* h_6 = (TH1F*)l6->Get(name);
	h_6->SetFillColor(35);
	//h_6->SetLineWidth(2);
	h_6->SetFillStyle(3017);
	h_6->GetYaxis()->SetTitle("N Events");
	leg->AddEntry(h_6,"Pomwig DPE","LFP");

        TList *list = new TList;
        list->Add(h_2);
        list->Add(h_5);
        list->Add(h_6);

        TH1F *MCtogether = (TH1F*)h_6->Clone("MCtogether");
        MCtogether->Reset();
	MCtogether->SetFillColor(40);
        MCtogether->Merge(list);
        leg->AddEntry(MCtogether,"All MC","LFP");


     /*   h_sm->Rebin(2);
        h_sp->Rebin(2);
        h_2->Rebin(2);
        h_3->Rebin(2);
        h_1->Rebin(2);*/


        h_sm->Draw("histo");
        h_sp->Draw("histosame");
	h_2->Draw("histosame");
	h_3->Draw("histosame");
	//h_4->Draw("histosame");
	h_5->Draw("histosame");
	h_6->Draw("histosame");
        MCtogether->Draw("histosame");
        h_1->Draw("psame");
	leg->Draw("same");
        gPad->RedrawAxis();

        text1 = new TLatex(3.570061,23.08044,"Work in Progress");
        text1->SetNDC();
        text1->SetTextAlign(13);
        text1->SetX(0.184);
        text1->SetY(0.928);
        text1->SetTextFont(42);
        text1->SetTextSizePixels(24);// dflt=28
        text1->Draw();

        text2 = new TLatex(3.570061,23.08044,"24.51 pb^{-1} at #sqrt{s} = 7 TeV");
        text2->SetNDC();
        text2->SetTextAlign(13);
        text2->SetX(0.184);
        text2->SetY(0.88);
        text2->SetTextFont(42);
        text2->SetTextSizePixels(24);// dflt=28
        text2->Draw();

	c1->SaveAs(name+TString(".png"));
	//c1->SaveAs(name+TString(".C"));

}


