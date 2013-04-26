#include <TH1F.h>
#include <TH2F.h>
#include <THStack.h>
//#include "Styles.C"
#include <TCanvas.h>
#include <TLegend.h>
#include <TRandom.h>
#include <TMath.h>
#include "tdrstyle.C" // from 

void Compare(){
 
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
        MakePlot("pTJet1_no_multiple_pileup_step4_3", "pTJet1_multiple_pileup_0_step4_3",1);
	//MakePlot("deltaEtamaxminPF_no_multiple_pileup_step4_4",1);
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

void MakePlot(TString name, TString name2, bool logscale){


        // Data
        TFile *l1  = TFile::Open("nonehisto_multijet_HLTExclDiJet30U_AND_HFPreSel_GoodVertex_triggerRefDijet50And30_triggercorr_effcorr_68mb_24bin_pT60_60.root");
        TFile *l2  = TFile::Open("nonehisto_multijet_HLTExclDiJet30U_AND_HFPreSel_GoodVertex_triggerRefDijet50And30_triggercorr_68mb_24bin_pT60_60.root");
        TFile *l3  = TFile::Open("nonehisto_multijet_HLTExclDiJet30U_AND_HFPreSel_GoodVertex_triggerRefDijet50And30_effcorr_68mb_24bin_pT60_60.root");
        TFile *l4  = TFile::Open("nonehisto_multijet_HLTExclDiJet30U_AND_HFPreSel_GoodVertex_triggerRefDijet50And30_68mb_24bin_pT60_60.root");

        // MC                              
        TFile *l5  = TFile::Open("histo_pythia6pu_pT60_60_pu0_castor.root");
        TFile *l6  = TFile::Open("histo_herwigpu_pT60_60_pu0_castor.root");

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
	h_1->SetMarkerStyle(20);
	h_1->SetMarkerSize(1.3);
	h_1->GetYaxis()->SetTitle("N Events");
	leg->AddEntry(h_1,"Data 2010 Corrected (Trigger and Cuts)","P");

	TH1F* h_2 = (TH1F*)l2->Get(name);
	h_2->SetMarkerStyle(20);
	h_2->SetMarkerSize(1.3);
	h_2->SetMarkerColor(kAzure);
	h_2->GetYaxis()->SetTitle("N Events");
	leg->AddEntry(h_2,"Data 2010 Corrected (Trigger)","P");

	TH1F* h_3 = (TH1F*)l3->Get(name);
	h_3->SetMarkerStyle(20);
	h_3->SetMarkerSize(1.3);
	h_3->SetMarkerColor(kGreen);
	h_3->GetYaxis()->SetTitle("N Events");
	leg->AddEntry(h_3,"Data 2010 Corrected (Cuts)","P");

	TH1F* h_4 = (TH1F*)l4->Get(name);
	h_4->SetMarkerStyle(20);
	h_4->SetMarkerSize(1.3);
      	h_4->SetMarkerColor(kAzure+15);
	h_4->GetYaxis()->SetTitle("N Events");
	leg->AddEntry(h_4,"Data 2010","P");


	TH1F* h_5 = (TH1F*)l5->Get(name2);
        //double factor = h_1->Integral()/h_2->Integral();
	h_5->SetFillColor(kRed);
        //h_2->Scale(factor);
        //h_2->Scale(6.825);
        //h_2->Scale(1./3.);
	h_5->SetLineWidth(2);
	h_5->SetFillStyle(3004);
	h_5->GetYaxis()->SetTitle("N Events");
	leg->AddEntry(h_5,"Pythia 6, PU[0]","LFP");

	TH1F* h_6 = (TH1F*)l6->Get(name2);
	h_6->SetFillColor(kGreen-7);
	//h_3->SetLineWidth(2);
	h_6->SetFillStyle(3005);
	h_6->GetYaxis()->SetTitle("N Events");
	leg->AddEntry(h_6,"Herwig, PU[0]","LFP");

        h_4->Draw("p");
        h_3->Draw("psame");
	h_2->Draw("psame");
	h_1->Draw("psame");
	h_5->Draw("histosame");
	h_6->Draw("histosame");
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


