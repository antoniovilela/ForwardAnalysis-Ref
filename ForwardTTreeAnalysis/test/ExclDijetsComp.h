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

class ExclDijetsComp {

   TFile* inf;
   TTree* tr;
   TBranch *diff;
   TBranch *excl;
   TBranch *qcd;
   DiffractiveEvent *eventdiff;
   ExclusiveDijetsEvent *eventexcl;
   QCDEvent *eventqcd;

   std::string filein;
   std::string fileinput;
   std::string processname;
   std::string processinput; 
   std::string savehistofile;
   double jet1PT;
   double jet2PT;
   int optnVertex;
   int optTrigger;
   bool switchWeightPU;
   bool switchWeightLumi;
   bool switchWeightEff;
   bool switchWeightePw;
   bool switchMultiple;
   bool switchPreSel;
   bool switchTrigger;
   double weightlumipass;
   double triggereffpass;
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
   std::vector<std::vector<TH1D*> > m_hVector_deltaEtaPF;
   std::vector<std::vector<TH1D*> > m_hVector_absdeltaEtaPF;
   std::vector<std::vector<TH1D*> > m_hVector_vertex;

   public :
   ExclDijetsComp() {}
   ~ExclDijetsComp() { inf->Close(); }
   
   void Run(std::string, std::string, std::string, double, double, int, int, bool, bool, bool, bool, bool, bool, bool, double, double);
   void LoadFile(std::string,std::string);
   void FillHistograms();

};
#endif
