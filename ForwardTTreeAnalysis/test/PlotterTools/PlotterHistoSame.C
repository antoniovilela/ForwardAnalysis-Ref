// Plotter
//////////

void setGraphTDR()
{

// TDR Style

TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); // Height of canvas
  tdrStyle->SetCanvasDefW(600); // Width of canvas
  tdrStyle->SetCanvasDefX(0);   // Position on screen
  tdrStyle->SetCanvasDefY(0);

// For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

// For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

// For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(1);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);
  tdrStyle->SetEndErrorSize(2);
  // remarked out DLE
  //tdrStyle->SetErrorMarker(20);
  tdrStyle->SetErrorX(0.);
  tdrStyle->SetMarkerStyle(20);

//For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(2);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

//For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
  //tdrStyle->SetOptFile(0);
  //tdrStyle->SetOptStat(1); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

// Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.16);
  tdrStyle->SetPadRightMargin(0.035);

// For the Global title:
  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

// For the axis titles:
  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(1.5); //0.9
  tdrStyle->SetTitleYOffset(1.5); //1.25
  //tdrStyle->SetTitleZOffset(1.5); //1.25
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

// For the axis labels:
  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

// For the axis:
  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(505, "XYZ");
//  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

// Change for log plots:
  //tdrStyle->SetOptLogx(0);
  //tdrStyle->SetOptLogy(0);
  //tdrStyle->SetOptLogz(0);

// Postscript options:
  tdrStyle->SetPaperSize(20.,20.);
  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);
  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);
  //tdrStyle->SetOptLogz(1);

tdrStyle->cd();

}


void PlotterHistoSame(){
cout << "\nRunning...\n" << endl;
Plotter ("data_2011_HFAND60.root","hltexcldijet60and-exume.root","hltexcldijet60and-pompty.root","hltexcldijet60and-pythia6.root","excldijet60And");
}

void Plotter(char* name1, char* name2, char* name3, char* name4, char* titlename){

setGraphTDR();

if(name1==NULL){

	printf("\nWarning: ");
	printf("the file ");
	printf(name1);
	printf(" does not exist \n\n");
	return;
	
}

else if(name2==NULL){

	printf("\nWarning: ");
	printf("the file ");
	printf(name2);
	printf(" does not exist \n\n");
	return;
}

else if(name3==NULL){

	printf("\nWarning: ");
	printf("the file ");
	printf(name3);
	printf(" does not exist \n\n");
	return;
}

else if(name4==NULL){

	printf("\nWarning: ");
	printf("the file ");
	printf(name4);
	printf(" does not exist \n\n");
	return;
	
}

else{

	TFile *f1= new TFile(name1);
	TFile *f2= new TFile(name2);
        TFile *f3= new TFile(name3);
        TFile *f4= new TFile(name4);

}

char *hName1 = new char [80];
char *hName2 = new char [80]; 
char *hName3 = new char [80];
char *hName4 = new char [80]; 	
char *filesave = new char[80];
char *filesave1 = new char[80];
char *filesave2 = new char[80];
char *filesave3 = new char[80];
char *filesave4 = new char[80];

f1->cd();
TIter nextkey1( f1->GetListOfKeys() );
TKey *key1;

f2->cd();
TIter nextkey2( f2->GetListOfKeys() );
TKey *key2;

f3->cd();
TIter nextkey3( f3->GetListOfKeys() );
TKey *key3;

f4->cd();
TIter nextkey4( f4->GetListOfKeys() );
TKey *key4;

int i=0;

    while ((key1 = (TKey*)nextkey1())) {

       key2 = (TKey*)nextkey2();
       key3 = (TKey*)nextkey3();
       key4 = (TKey*)nextkey4();

	// read object from source file
	TObject *obj1 = key1->ReadObj();
	TH1D *h1 = (TH1D*)obj1;

	// read object from source file
	TObject *obj2 = key2->ReadObj();
	TH1D *h2 = (TH1D*)obj2;

	// read object from source file
	TObject *obj3 = key3->ReadObj();
	TH1D *h3 = (TH1D*)obj3;

	// read object from source file
	TObject *obj4 = key4->ReadObj();
	TH1D *h4 = (TH1D*)obj4;

	hName1=h1->GetName();
      	hName2=h2->GetName();
       hName3=h3->GetName();
       hName4=h4->GetName();

        // General Style
        
        h1->GetXaxis()->SetTitleOffset(1.6);
        h1->GetYaxis()->SetTitleOffset(1.6);
        h1->GetZaxis()->SetTitleOffset(2.);
        h2->GetXaxis()->SetTitleOffset(1.6);
        h2->GetYaxis()->SetTitleOffset(1.6);
        h2->GetZaxis()->SetTitleOffset(2.);
        h3->GetXaxis()->SetTitleOffset(1.6);
        h3->GetYaxis()->SetTitleOffset(1.6);
        h3->GetZaxis()->SetTitleOffset(2.);
        h4->GetXaxis()->SetTitleOffset(1.6);
        h4->GetYaxis()->SetTitleOffset(1.6);
        h4->GetZaxis()->SetTitleOffset(2.);
        

        sprintf(filesave,"%s%s.png",h1->GetName(),titlename);
        std::string checkLog(obj1->GetName());


   if (h1->GetEntries() != 0 || h2->GetEntries() != 0 || h3->GetEntries() != 0 || h4->GetEntries() != 0){ // Defense

        if (obj1->IsA()->InheritsFrom("TH1") && obj2->IsA()->InheritsFrom("TH1") && obj3->IsA()->InheritsFrom("TH1") && obj4->IsA()->InheritsFrom("TH1")){
        
           TCanvas *togetherlog = new TCanvas("canvasSamelog","canvasSamelog");
           togetherlog->cd();     

           if ( checkLog == "pTJet1All" || checkLog == "pTJet2All" || checkLog == "pfetamaxAll" || checkLog == "pfetaminAll" || checkLog == "deltaEtaJetsAll" || checkLog == "deltaPhiJetsAll" || checkLog == "deltaPtJetsAll"  || checkLog == "dijetMassAll" || checkLog == "pfetamax" || checkLog == "pfetamin" || checkLog == "deltaEtaJets" || checkLog == "deltaPhiJets" || checkLog == "deltaPtJets"  || checkLog == "dijetMass" || checkLog == "pfetamax4" || checkLog == "pfetamin4" || checkLog == "deltaEtaJets4" || checkLog == "deltaPhiJets4" || checkLog == "deltaPtJets4" || checkLog == "dijetMass4" || checkLog == "RJJAll" || checkLog == "RJJ" || checkLog == "RJJ4" || checkLog == "tracksAll" || checkLog == "tracks" || checkLog == "tracks4") {
           togetherlog->SetLogy();
           }           


           setGraphTDR();

           h1->SetMarkerStyle(20);
           h1->SetMarkerSize(1);
    
           h2->SetMarkerStyle(20);
           h2->SetMarkerSize(1);
           h2->SetMarkerColor(2);
        
           h3->SetMarkerStyle(20);
           h3->SetMarkerSize(1);
           h3->SetMarkerColor(3);

           h4->SetMarkerStyle(20);
           h4->SetMarkerSize(1);
           h4->SetMarkerColor(4);
           
           leg = new TLegend(0.71,0.89,0.989,0.989);
           leg->SetTextSize(0.020);
           leg->SetFillColor(0);
           gStyle->SetOptStat(0);
           leg->AddEntry(h1,"Data 2011","p");
           leg->AddEntry(h2,"Exume","p");
           leg->AddEntry(h3,"Pompty","p");
           leg->AddEntry(h4,"Pythia6 Z2","p");
                   
           //h1->SetFillColor(1);
           //h1->Sumw2();
           //h1->GetYaxis()->SetTitle("Probability");
           h1->Draw("PE");
           //h1->DrawNormalized("PE");

           //h2->SetFillColor(2);
           //h2->Sumw2();
           //h2->GetYaxis()->SetTitle("Probability");
           h2->Draw("SAMEPE");
           //h2->DrawNormalized("SAMEPE");

           //h3->SetFillColor(3);
           //h3->Sumw2();
           //h3->GetYaxis()->SetTitle("Probability");
           h3->Draw("SAMEPE");
           //h3->DrawNormalized("SAMEPE");
           leg->Draw();

           //h4->SetFillColor(3);
           //h4->Sumw2();
           //h4->GetYaxis()->SetTitle("Probability");
           h4->Draw("SAMEPE");
           //h4->DrawNormalized("SAMEPE");
           leg->Draw();
           
           togetherlog->Update();
           togetherlog->SaveAs(filesave);
           delete togetherlog;

        }

        if (obj1->IsA ()->InheritsFrom ("TH2") && obj2->IsA ()->InheritsFrom ("TH2") && obj3->IsA ()->InheritsFrom ("TH2") && obj4->IsA ()->InheritsFrom ("TH2")){
     

          if ( checkLog == "ETCalos" || checkLog == "ETCalosJetsAtTracker" || checkLog == "ETCalos1" || checkLog == "ETCalosJetsAtTracker1" || checkLog == "ETCalos2" || checkLog == "ETCalosJetsAtTracker2" || checkLog == "ETCalos3" || checkLog == "ETCalosJetsAtTracker3" || checkLog == "ETCalos4" || checkLog == "ETCalosJetsAtTracker4" ){

               TCanvas *togetherLego = new TCanvas("canvasLego","canvasLego");
               gStyle->SetOptStat(1);

               h1->SetMarkerSize(0);
               h2->SetMarkerSize(0);
               h3->SetMarkerSize(0);
               h4->SetMarkerSize(0);

               //h1->GetZaxis()->SetTitle("Probability");
               //h2->GetZaxis()->SetTitle("Probability");
               //h3->GetZaxis()->SetTitle("Probability");
               //h4->GetZaxis()->SetTitle("Probability");

               h1->GetYaxis()->SetTitle("log_{10} Castor Signal [fC]");
               h2->GetYaxis()->SetTitle("log_{10} Castor Signal [fC]");
               h3->GetYaxis()->SetTitle("log_{10} Castor Signal [fC]");
               h4->GetYaxis()->SetTitle("log_{10} Castor Signal [fC]");

               togetherLego->cd();
               togetherLego->SetLogy(1);
               //h1->SetLineColor(1);
               h1->SetFillColor(1);
               h1->Draw("LEGO1E0");
               togetherLego->Update();
               sprintf(filesave1,"%s%s-data2011.png",h1->GetName(),titlename);
               togetherLego->SaveAs(filesave1);

               togetherLego->cd();
               togetherLego->SetLogy(1);
               //h2->SetLineColor(2);
               h2->SetFillColor(2);
               h2->Draw("LEGO1E0");
               togetherLego->Update();
               sprintf(filesave2,"%s%s-exume2011.png",h2->GetName(),titlename);
               togetherLego->SaveAs(filesave2);  

               togetherLego->cd();
               togetherLego->SetLogy(1);
               //h3->SetLineColor(3);
               h3->SetFillColor(3);
               h3->Draw("LEGO1E0");
               togetherLego->Update();
               sprintf(filesave3,"%s%s-pompty2011.png",h3->GetName(),titlename);
               togetherLego->SaveAs(filesave3);   

               togetherLego->cd();
               togetherLego->SetLogy(1);
               //h4->SetLineColor(3);
               h4->SetFillColor(4);
               h4->Draw("LEGO1E0");
               togetherLego->Update();
               sprintf(filesave4,"%s%s-pythia62011.png",h4->GetName(),titlename);
               togetherLego->SaveAs(filesave4);           

               delete togetherLego;

          }
        
          else {

          TCanvas *togetherLegoAll = new TCanvas("canvasLegoAll","canvasLegoAll",1200,400);
          togetherLegoAll->Divide(4,1);

          setGraphTDR();
          //setTH2();   
          h1->SetMarkerSize(0);
          h2->SetMarkerSize(0);
          h3->SetMarkerSize(0);
          h4->SetMarkerSize(0);       

          leg2 = new TLegend(0.71,0.89,0.989,0.989);
          leg2->SetTextSize(0.020);
          leg2->SetFillColor(0);
          leg2->AddEntry(h1,"Data 2011","f");
          leg2->AddEntry(h2,"Exume","f");
          leg2->AddEntry(h3,"Pompty","f");
          leg2->AddEntry(h4,"Pythia6 Z2","f");

          //h1->GetZaxis()->SetTitle("Probability");
          //h2->GetZaxis()->SetTitle("Probability");
          //h3->GetZaxis()->SetTitle("Probability");
          //h4->GetZaxis()->SetTitle("Probability");

          h1->GetYaxis()->SetTitle("nHF^{-}");
          h2->GetYaxis()->SetTitle("nHF^{-}");
          h3->GetYaxis()->SetTitle("nHF^{-}");
          h4->GetYaxis()->SetTitle("nHF^{-}");

          togetherLegoAll->cd(1);
          h1->SetFillColor(1);
          h1->Draw("LEGO1E0");

          togetherLegoAll->cd(2);
          h2->SetFillColor(2);
          h2->Draw("LEGO1E0"); 

          togetherLegoAll->cd(3);
          h3->SetFillColor(3);
          h3->Draw("LEGO1E0");

          togetherLegoAll->cd(4);
          h4->SetFillColor(4);
          h4->Draw("LEGO1E0");

          //leg2->Draw();
          togetherLegoAll->Update();
          togetherLegoAll->SaveAs(filesave);
          delete togetherLegoAll;

          }
 
       }

       i++;

   } // if defense
       
  }

printf("\nNumber of Plots: %d\n\n",i);
       

}


// STUFF
// Double_t scale2 = 1./h2->Integral();
// h2->Scale(scale2);
