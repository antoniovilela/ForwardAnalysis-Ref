#ifndef DiffractiveZComp_h
#define DiffractiveZComp_h

#include <TFile.h>
#include <TTree.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

class DiffractiveEvent;
class DiffractiveZEvent;
class EventInfoEvent;

class DiffractiveZComp {

   TFile* inf;
   TTree* tr;
   TBranch *diff;
   TBranch *diffZ;
   TBranch *info;
   DiffractiveEvent *eventdiff;
   DiffractiveZEvent *eventdiffZ;
   EventInfoEvent *eventinfo;

   std::string filein;
   std::string savehistofile;
   std::string ttreename;
   int nVertex;
   int optTrigger;
   bool switchPUMultiple;
   bool switchPreSel;
   bool switchTrigger;
   bool switchmcweight;
   float mcweight;

   std::vector<std::vector<TH1D*> > m_hVector_DiElectron;
   std::vector<std::vector<TH1D*> > m_hVector_LeadingElectronPt;
   std::vector<std::vector<TH1D*> > m_hVector_LeadingElectronEta;
   std::vector<std::vector<TH1D*> > m_hVector_LeadingElectronPhi;
   std::vector<std::vector<TH1D*> > m_hVector_LeadingElectronCharge;
   std::vector<std::vector<TH1D*> > m_hVector_SecondElectronPt;
   std::vector<std::vector<TH1D*> > m_hVector_SecondElectronEta;
   std::vector<std::vector<TH1D*> > m_hVector_SecondElectronPhi;
   std::vector<std::vector<TH1D*> > m_hVector_SecondElectronCharge;
   std::vector<std::vector<TH1D*> > m_hVector_ElectronsN;

   std::vector<std::vector<TH1D*> > m_hVector_DiMuon;
   std::vector<std::vector<TH1D*> > m_hVector_LeadingMuonPt;
   std::vector<std::vector<TH1D*> > m_hVector_LeadingMuonEta;
   std::vector<std::vector<TH1D*> > m_hVector_LeadingMuonPhi;
   std::vector<std::vector<TH1D*> > m_hVector_LeadingMuonCharge;
   std::vector<std::vector<TH1D*> > m_hVector_SecondMuonPt;
   std::vector<std::vector<TH1D*> > m_hVector_SecondMuonEta;
   std::vector<std::vector<TH1D*> > m_hVector_SecondMuonPhi;
   std::vector<std::vector<TH1D*> > m_hVector_SecondMuonCharge;
   std::vector<std::vector<TH1D*> > m_hVector_MuonsN;
 
   std::vector<std::vector<TH1D*> > m_hVector_sumEHFplus;
   std::vector<std::vector<TH1D*> > m_hVector_sumEHFminus;
   std::vector<std::vector<TH1D*> > m_hVector_sumEHEplus;
   std::vector<std::vector<TH1D*> > m_hVector_sumEHEminus;
 
   std::vector<std::vector<TH1D*> > m_hVector_lumi;
   std::vector<std::vector<TH1D*> > m_hVector_asumE;
   std::vector<std::vector<TH2F*> > m_hVector_multhf;
   std::vector<std::vector<TH2F*> > m_hVector_etcalos;
   std::vector<std::vector<TH1D*> > m_hVector_tracks;
   std::vector<std::vector<TH1D*> > m_hVector_pfetamax;
   std::vector<std::vector<TH1D*> > m_hVector_pfetamin;
   std::vector<std::vector<TH1D*> > m_hVector_vertex;
 
   public :
   DiffractiveZComp() {}
   ~DiffractiveZComp() { inf->Close(); }
   
   void Run(std::string, std::string, std::string, bool, int, bool, int, bool, bool, float);
   void LoadFile(std::string,std::string);
   void FillHistograms();

};
#endif
