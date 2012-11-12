#ifndef UtilitiesPlottingTools_Plotter_h
#define UtilitiesPlottingTools_Plotter_h

#include "TH1F.h"
#include "TDirectory.h"
#include "THStack.h"

#include "Utilities/PlottingTools/interface/PlottingTools.h"

#include <vector>
#include <string> 
#include <map>

class VarDesc{
   public:
      VarDesc(const std::string& fname, TDirectory* fdir, const std::string& flabel, double fnorm): name_(fname),dir_(fdir),label_(flabel),norm_(fnorm) {}
      const std::string& name() const {return name_;}
      TDirectory* dir() const {return dir_;}
      const std::string& label() const {return label_;}
      double norm() const {return norm_;}
   private:
      std::string name_;
      TDirectory* dir_;
      std::string label_;
      double norm_;
};

template <class NormPolicy=DefaultNorm>
class Plotter{
   public:
      //typedef std::pair<std::string,std::pair<std::string,TDirectory*> > VarDesc;
      //typedef std::pair<std::string,std::pair<TDirectory*,std::pair<std::string,double> > > VarDesc;
      typedef std::map<std::string, std::vector<VarDesc> > VarMap;
      typedef std::pair<TH1F*,std::string> HistDesc;
      typedef std::map<std::string, std::vector<HistDesc> > HistMap;

      Plotter();
    
      void SetVerbose(bool set = true) { verbose_ = set; }
      void SetRebin(int rebin) { rebin_ = rebin; }
      void SetStats(bool stats) { histStats_ = stats; }
      void SetRatio(bool set) { useRatio_ = set; }
      void SetTitleSize(float size) { titleSize_ = size; }
      void SetAxisLabelSize(float size) { axisLabelSize_ = size; }
      void SetHeader(std::string const& header) { legendHeader_ = header; }
      void SetRefHisto(bool set = true) { firstAsRefHist_ = set; }

      void AddLabel(std::string const& label) { paveLabels_.push_back(label); }
      void AddObject(TObject* object) { drawObjects_.push_back(object); }

      void SetColors(std::vector<int> const& colors) {histLineColors_ = colors; histFillColors_ = colors;}
      void SetLineColors(std::vector<int> const& colors) {histLineColors_ = colors;}
      void SetFillColors(std::vector<int> const& colors) {histFillColors_ = colors;}
      void SetLineStyles(std::vector<int> const& linestyles) {histLineStyles_ = linestyles;}
      void SetFillStyles(std::vector<int> const& fillstyles) {histFillStyles_ = fillstyles;}
      void SetMarkerColors(std::vector<int> const& colors) {histMarkerColors_ = colors;}
      void SetMarkerStyles(std::vector<int> const& styles) {histMarkerStyles_ = styles;}
      void SetMarkerSizes(std::vector<float> const& sizes) {histMarkerSizes_ = sizes;}

      /*void SetTitlesX(std::vector<std::string> const& titles) {histTitlesX_ = titles;}
      void SetTitlesY(std::vector<std::string> const& titles) {histTitlesY_ = titles;}*/
      void SetTitleX(std::string const& var, std::string const& title) { histTitlesX[var] = title; }
      void SetTitleY(std::string const& var, std::string const& title) { histTitlesY[var] = title; } 

      void plot(std::vector<std::string>&, std::vector<std::pair<std::string,TDirectory*> >&, const char* drawOption = "");
      void plot(std::vector<std::string>&, std::vector<std::pair<std::string,TDirectory*> >&, const std::vector<double>&, const char* drawOption = "");

      void plot(std::map<std::string,std::vector<std::string> >&, TDirectory*, const char* drawOption = "");
      void plot(VarMap& variablesMap, const char* drawOption = "");

      void plotStack(VarMap const& variablesMap, const char* drawOption = "", bool firstAsRefHist = false);

      void plotStack(std::vector<std::string>& variables, std::vector<std::pair<std::string,TDirectory*> >& directories, const std::vector<double>& normFactors, const char* drawOption = "");
      void plotComponents(std::vector<std::string>& variables, std::vector<std::pair<std::string,TDirectory*> >& directories, const std::vector<double>& normFactors, const char* drawOption = "");
      //void plotComponents(VarMap const& variablesMap, const char* drawOption = "");

   private:
      TH1F* loadHisto(VarDesc const&,unsigned int);
      std::map<std::string,std::vector<HistDesc> > loadHistos(VarMap const&);
      THStack* buildTHStack(std::string const&, std::vector<HistDesc> const&);
      VarMap makeVarMap(std::vector<std::string>&, std::vector<std::pair<std::string,TDirectory*> >&, const std::vector<double>&);
      void plotHistos(std::vector<HistDesc> const&, std::string const&);

      bool verbose_, firstAsRefHist_, histStats_, useRatio_;
      int rebin_;
      float titleSize_, axisLabelSize_; 
      std::string legendHeader_;

      std::vector<int> histLineColors_;
      std::vector<int> histLineStyles_;
      std::vector<int> histFillColors_;
      std::vector<int> histFillStyles_;
      std::vector<int> histMarkerColors_;
      std::vector<int> histMarkerStyles_;
      std::vector<float> histMarkerSizes_;
      /*std::vector<std::string> histTitlesX_;
      std::vector<std::string> histTitlesY_;*/
      std::map<std::string, std::string> histTitlesX;
      std::map<std::string, std::string> histTitlesY;
      std::vector<std::string> paveLabels_;
      std::vector<TObject*> drawObjects_;
      //NormPolicy norm_;
};

#include "Plotter.icc"

#endif
