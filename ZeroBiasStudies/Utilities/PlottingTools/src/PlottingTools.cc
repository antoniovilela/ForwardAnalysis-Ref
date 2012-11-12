#include "Utilities/PlottingTools/interface/PlottingTools.h"

#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include "TDirectory.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

double ConstNorm::normalization = 1.0;

void plot(std::map<std::string,std::vector<std::string> >& variablesMap, TDirectory* dir, bool Norm){
   std::map<std::string,TCanvas*> canvasesVar;
   std::map<std::string,TLegend*> legendsVar;
   for(std::map<std::string,std::vector<std::string> >::const_iterator cat = variablesMap.begin();
                                                                       cat != variablesMap.end(); ++cat){
      const std::string& name = cat->first;
      canvasesVar[name] = new TCanvas(name.c_str(),name.c_str());
      canvasesVar[name]->cd();

      legendsVar[name] = new TLegend(0.4,0.7,0.95,0.8);
    
      const std::vector<std::string>& vars = cat->second;
      size_t index = 0;
      for(std::vector<std::string>::const_iterator var = vars.begin(); var != vars.end(); ++var,++index){
         TH1F* histo = getHisto(dir,*var);

         scaleHisto(histo,1,1,(index + 1));
         legendsVar[name]->AddEntry(histo,var->c_str(),"L"); 
         if(Norm) histo->DrawNormalized("same");
         else histo->Draw("same");
      }
      legendsVar[name]->SetFillColor(0);
      legendsVar[name]->Draw("same");
   }
}

std::map<std::string,std::vector<std::string> > buildVarMap(const std::vector<std::string>& varNames,const std::vector<std::string>& triggerBits){

  std::map<std::string,std::vector<std::string> > varMap;

  std::vector<std::string> def;
   for(std::vector<std::string>::const_iterator var = varNames.begin(); var != varNames.end(); ++var){
      varMap[*var] = def;
      std::vector<std::string>& variables = varMap[*var];
      variables.push_back(*var);
      for(std::vector<std::string>::const_iterator trigBit = triggerBits.begin(); trigBit != triggerBits.end(); ++trigBit){
         variables.push_back(*var + "_" + *trigBit);
      }
   }

   return varMap;
}

/*TH1F* getHisto(TFile* file, const std::string& refVar){
   TH1F* hist = static_cast<TH1F*>(file->Get(refVar.c_str()));

   if(!hist) std::cout << ">>>> Error: histogram " << refVar.c_str() << " not found." << std::endl;

   return hist;
}*/ 

TH1F* getHisto(TDirectory const* dir, std::string const& refVar){
   TDirectory* mydir = const_cast<TDirectory*>(dir);
   TH1F* hist = static_cast<TH1F*>(mydir->Get(refVar.c_str()));

   //if(!hist) std::cout << ">>>> Error: histogram " << refVar.c_str() << " not found." << std::endl;
   if(!hist) throw RootException("ERROR: histogram not found: " + refVar);

   return hist;
}

void scaleHisto(TH1F* histo, double scale, int line, int color, int rebin){

   if(scale != 1.) {std::cout << "DEBUG: Calling scaleHisto..scaling histo by " << scale << std::endl; histo->Scale(scale);}

   if(rebin != 1) {std::cout << "DEBUG: Calling scaleHisto..re-bining histo by " << rebin << std::endl; histo->Rebin(rebin);}
   //histo->SetLineWidth(3);
   histo->SetLineStyle(line);
   histo->SetLineColor(color);

   //histo->GetYaxis()->SetTitle("a.u.");
   /*double binWidth = histo->GetBinWidth(1);
   char axisYTitle[50]; sprintf(axisYTitle,"/ (%f)",binWidth);
   histo->GetYaxis()->SetTitle(axisYTitle);*/
}

TH1F* rebinHisto(TH1F const* histo, std::vector<int> const& groups){
   //int nBins = histo->GetNbinsX();
   int nBins_new = groups.size(); 
 
   //int sum = std::for_each(groups.begin(),groups.end(),Sum<int>(0)).value();
   //int sum = std::accumulate(groups.begin(),groups.end(),0);

   // Create array with new (variable) binning
   double xbins[400];
   xbins[0] = histo->GetBinLowEdge(1);
   int bin = 1; 
   for(int k = 0; k < nBins_new; ++k){
      bin += groups[k];
      xbins[k+1] = histo->GetBinLowEdge(bin);
   }
   TH1F histo_temp("_temp","",nBins_new,xbins);

   int firstbin = 1;
   for(int k = 0; k <= nBins_new; ++k){
      histo_temp.SetBinContent((k + 1), histo->Integral(firstbin,firstbin + groups[k] - 1));
      firstbin += groups[k]; 
   } 

   std::string hname = histo->GetName();
   hname += "_rebinned";
   histo_temp.SetName(hname.c_str());
   TH1F* histo_new = new TH1F(histo_temp);
   
   return histo_new;
}
