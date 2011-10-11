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
  tdrStyle->SetOptFile(0);
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

tdrStyle->cd();

}


void PlotterHisto(){
cout << "\nRunning...\n" << endl;
Plotter ("exclusive_pTJ1-60_pTJ2-60_nV-1_Trigger-0.root","Triggers");
}

void Plotter(char* name1, char* titlename){

setGraphTDR();

if(name1==NULL){

	printf("\nWarning: ");
	printf("the file ");
	printf(name1);
	printf(" does not exist \n\n");
	return;
	
}

else{

	TFile *f1= new TFile(name1);

}

char *hName1 = new char [80];
char *filesave = new char[80];
char *filesave1 = new char[80];

f1->cd();
TIter nextkey1( f1->GetListOfKeys() );
TKey *key1;


int i=0;

    while ((key1 = (TKey*)nextkey1())) {


	// read object from source file
	TObject *obj1 = key1->ReadObj();
	TH1D *h1 = (TH1D*)obj1;
	hName1=h1->GetName();


        // General Style
        
        h1->GetXaxis()->SetTitleOffset(1.6);
        h1->GetYaxis()->SetTitleOffset(1.6);
        h1->GetZaxis()->SetTitleOffset(2.);
      

        sprintf(filesave,"%s%s.png",h1->GetName(),titlename);
        std::string checkLog(obj1->GetName());


   if (h1->GetEntries() != 0){ // Defense

        if (obj1->IsA()->InheritsFrom("TH1")){
        
           TCanvas *singleCanvas = new TCanvas("canvas","canvas");
           singleCanvas->cd();     

           setGraphTDR();
           //setTH1();

           h1->SetMarkerStyle(20);
           h1->SetMarkerSize(1);
           gStyle->SetOptStat(0);
    
           //h1->SetFillColor(1);
           h1->Sumw2();
           h1->GetYaxis()->SetTitle("Probability");
           h1->DrawNormalized("PE");
      
           singleCanvas->Update();
           singleCanvas->SaveAs(filesave);
           delete singleCanvas;


        }

        if (obj1->IsA ()->InheritsFrom ("TH2")){
     

          if ( checkLog == "ETCalos" || checkLog == "ETCalosJetsAtTracker" || checkLog == "ETCalos1" || checkLog == "ETCalosJetsAtTracker1" || checkLog == "ETCalos2" || checkLog == "ETCalosJetsAtTracker2" || checkLog == "ETCalos3" || checkLog == "ETCalosJetsAtTracker3" || checkLog == "ETCalos4" || checkLog == "ETCalosJetsAtTracker4" ){

               TCanvas *LegoCanvas = new TCanvas("canvasLego","canvasLego");
               gStyle->SetOptStat(1);

               h1->SetMarkerSize(0);
 
               //h1->GetZaxis()->SetTitle("Probability");


               h1->GetYaxis()->SetTitle("log_{10} Castor Signal [fC]");
   
               LegoCanvas->cd();
               LegoCanvas->SetLogy();
               //h1->SetLineColor(1);
               h1->SetFillColor(1);
               h1->Draw("LEGO1E0");
               LegoCanvas->Update();
               sprintf(filesave1,"%s%s-trigger0.png",h1->GetName(),titlename);
               LegoCanvas->SaveAs(filesave1);

               delete LegoCanvas;

          }
        
          else {

          TCanvas *LegoCanvas = new TCanvas("canvasLego","canvasLego");

          setGraphTDR();
          //setTH2();   
          h1->SetMarkerSize(0);
 
 
          //h1->GetZaxis()->SetTitle("Probability");

          h1->GetYaxis()->SetTitle("nHF^{-}");
 
          LegoCanvas->cd();
          h1->SetFillColor(1);
          h1->Draw("LEGO1E0");

          LegoCanvas->Update();
          LegoCanvas->SaveAs(filesave);
          delete LegoCanvas;

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
