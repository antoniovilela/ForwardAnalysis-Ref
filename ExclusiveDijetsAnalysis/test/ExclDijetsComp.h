#ifndef ExclDijetsComp_h
#define ExclDijetsComp_h

#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

class DiffractiveEvent;
class ExclusiveDijetsEvent;
class QCDEvent;
class EventInfoEvent;

class ExclDijetsComp {

   TFile* inf;
   TTree* tr;
   TBranch *diff;
   TBranch *excl;
   TBranch *info;
   DiffractiveEvent *eventdiff;
   ExclusiveDijetsEvent *eventexcl;
   EventInfoEvent *eventinfo;

   std::string pudatafile;
   std::string pumcfile;
   std::string filein;
   std::string filetrigger;
   std::string fileinput;
   std::string processname;
   std::string processinput; 
   std::string savehistofile;
   std::string filecor;
   std::string JetUn;
   double jet1PT;
   double jet2PT;
   int optnVertex;
   int optTrigger;
   bool switchWeightPU;
   bool switchWeightLumi;
   bool switchWeightEff;
   bool switchTriggerEff;
   bool switchWeightePw;
   bool switchMultiple;
   bool switchPreSel;
   bool switchTrigger;
   double weightlumipass;

   std::vector<std::vector<TH1D*> > m_hVector_rjj;
   std::vector<std::vector<TH1D*> > m_hVector_detagen;
   std::vector<std::vector<TH1D*> > m_hVector_mxGen;
   std::vector<std::vector<TH2F*> > m_hVector_multhf;
   std::vector<std::vector<TH2F*> > m_hVector_etcalos;
   std::vector<std::vector<TH1D*> > m_hVector_tracks;
   std::vector<std::vector<TH1D*> > m_hVector_pfetamax;
   std::vector<std::vector<TH1D*> > m_hVector_pfetamin;
   std::vector<std::vector<TH1D*> > m_hVector_asumE;
   std::vector<std::vector<TH1D*> > m_hVector_deltaetajets;
   std::vector<std::vector<TH1D*> > m_hVector_deltaphijets;
   std::vector<std::vector<TH1D*> > m_hVector_deltaptjets;
   std::vector<std::vector<TH1D*> > m_hVector_dijetmass;
   std::vector<std::vector<TH1D*> > m_hVector_ptjet1;
   std::vector<std::vector<TH1D*> > m_hVector_ptjet2;
   std::vector<std::vector<TH1D*> > m_hVector_etajet1;
   std::vector<std::vector<TH1D*> > m_hVector_etajet2;
   std::vector<std::vector<TH1D*> > m_hVector_phijet1;
   std::vector<std::vector<TH1D*> > m_hVector_phijet2;
   std::vector<std::vector<TH1D*> > m_hVector_sumEHFplus;
   std::vector<std::vector<TH1D*> > m_hVector_sumEHFminus;
   std::vector<std::vector<TH1D*> > m_hVector_sumEHEplus;
   std::vector<std::vector<TH1D*> > m_hVector_sumEHEminus;
   std::vector<std::vector<TH1D*> > m_hVector_sumEHFpfplus;
   std::vector<std::vector<TH1D*> > m_hVector_sumEHFpfminus;
   std::vector<std::vector<TH1D*> > m_hVector_sumECastor;
   std::vector<std::vector<TH1D*> > m_hVector_deltaEtaPF;
   std::vector<std::vector<TH1D*> > m_hVector_absdeltaEtaPF;
   std::vector<std::vector<TH1D*> > m_hVector_vertex;
   std::vector<std::vector<TH1D*> > m_hVector_sumEHFplusVsiEta;
   std::vector<std::vector<TH1D*> > m_hVector_sumEHFminusVsiEta;
   std::vector<std::vector<TH1D*> > m_hVector_lumi;
   std::vector<std::vector<TH2D*> > m_hVector_sumEHFplusVsetaMax;
   std::vector<std::vector<TH2D*> > m_hVector_sumEHFminusVsetaMin;
   std::vector<std::vector<TH2D*> > m_hVector_sumEHFpfplusVsetaMax;
   std::vector<std::vector<TH2D*> > m_hVector_sumEHFpfminusVsetaMin;

   //
   public :
   ExclDijetsComp() {}
   ~ExclDijetsComp() { inf->Close(); }
   
   void Run(std::string, std::string, std::string, std::string, std::string, std::string, std::string, double, double, std::string, int, int, bool, bool, bool, bool, bool, bool, bool, bool, double);
   void LoadFile(std::string,std::string);
   void FillHistograms();

};
#endif
