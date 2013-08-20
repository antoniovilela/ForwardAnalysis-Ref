//-------------------------------------------------------------------------------------------------------->>
// UNIVERSIDADE DO ESTADO DO RIO DE JANEIRO - CMS/Brasil
//-------------------------------------------------------------------------------------------------------->>
//
// Twiki: https://twiki.cern.ch/twiki/bin/view/CMS/FwdPhysicsDiffractiveZsAnalysis#Macro_Analysis
//

//#if !defined(__CINT__) || defined(__MAKECINT__)

#include <TROOT.h>
#include <TChain.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>
#include <TTree.h>
#include <TMath.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>

#include "DiffractiveZ.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/EventInfoEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveZEvent.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/DiffractiveEvent.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"

using namespace diffractiveAnalysis;
using namespace diffractiveZAnalysis;
using namespace eventInfo;
using namespace reweight;


static inline void loadBar(int x, int n, int r, int w)
{
  // Modified
  // http://www.rosshemsley.co.uk/2011/02/creating-a-progress-bar-in-c-or-any-other-console-app/

  // Only update r times.
  if ( x % (n/r) != 0 ) return;

  // Calculuate the correlation of complete-to-incomplete.
  double correlation = x/(double)n;
  int   c     = correlation * w;

  // Show the percentage complete.
  printf("%3d%%[", (int)(correlation*100) );

  // Show the load bar.
  for (int x=0; x<c; x++)
    printf("=");

  for (int x=c; x<w; x++)
    printf(" ");

  // ANSI Control codes to go back to the
  // previous line and clear it.
  // printf("]\n33[F33[J");

  printf("\r"); // Move to the first column
  fflush(stdout);
}


void DiffractiveZ::LoadFile(std::string fileinput, std::string processinput){

  inf = NULL;
  tr  = NULL;
  inf = TFile::Open(fileinput.c_str(),"read");
  std::string fdirectory = processinput + "/ProcessedTree";
  tr = (TTree*)inf->Get(fdirectory.c_str());
  eventdiff = new DiffractiveEvent();
  eventdiffZ = new DiffractiveZEvent();
  eventinfo = new EventInfoEvent();
  diff = tr->GetBranch("DiffractiveAnalysis");
  diffZ = tr->GetBranch("DiffractiveZAnalysis");
  info = tr->GetBranch("EventInfo");
  diff->SetAddress(&eventdiff);
  diffZ->SetAddress(&eventdiffZ);
  info->SetAddress(&eventinfo);

}

void DiffractiveZ::CreateHistos(std::string type){

  double binarrayplus[18] = {0.,0.25,0.5,0.75,1.,1.25,1.5,1.75,2.,2.25,2.5,2.75,3.,3.25,3.5,3.75,4.,5.2};
  double binarrayminus[18] = {-5.2,-4.,-3.75,-3.5,-3.25,-3.,-2.75,-2.5,-2.25,-2.,-1.75,-1.5,-1.25,-1.,-0.75,-0.5,-0.25,0.};
  double binarraydelta[15] = {0.,0.5,1.,1.5,2.,2.5,3.,3.5,4.,4.5,5.,5.5,6.,7.,9.};

  std::string step0 = "without_cuts"; 
  std::string step1 = "with_trigger"; 
  std::string step2 = "step2"; 
  std::string step3 = "step3"; 
  std::string step4 = "step4"; 
  std::string step5 = "step5"; 
  std::string step6 = "step6"; 
  std::string step7 = "step7"; 
  std::string step8 = "GapHFMinus";
  std::string step9 = "GapHFPlus";
  std::string step10 = "GapHFMinusAndCastor";
  std::string step11 = "GapHFPlusAndCastorActivity";
  std::string step12 = "GapHFMinusAndCastorZKinPositive";
  std::string step13 = "GapHFPlusAndCastorActivityZKinNegative";


  Folders.push_back(step0);
  Folders.push_back(step1);
  Folders.push_back(step2);
  Folders.push_back(step3);
  Folders.push_back(step4);
  Folders.push_back(step5);
  Folders.push_back(step6);
  Folders.push_back(step7);
  Folders.push_back(step8);
  Folders.push_back(step9);
  Folders.push_back(step10);
  Folders.push_back(step11);
  Folders.push_back(step12);
  Folders.push_back(step13);

  int nloop=-999;

  if (type=="multiple_pileup") nloop=21;
  else if (type=="no_multiple_pileup") nloop=1;

  char tag[300];

  for (std::vector<std::string>::size_type j=0; j<Folders.size(); j++){

    m_hVector_DiElectronMass.push_back( std::vector<TH1F*>() );
    m_hVector_DiElectronEta.push_back( std::vector<TH1F*>() );
    m_hVector_DiElectronPt.push_back( std::vector<TH1F*>() );
    m_hVector_DiElectronPhi.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronPt.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronEta.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronPhi.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronCharge.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronPt.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronEta.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronPhi.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronCharge.push_back( std::vector<TH1F*>() );
    m_hVector_ElectronsN.push_back( std::vector<TH1F*>() );
    m_hVector_DiMuonMass.push_back( std::vector<TH1F*>() );
    m_hVector_DiMuonEta.push_back( std::vector<TH1F*>() );
    m_hVector_DiMuonPt.push_back( std::vector<TH1F*>() );
    m_hVector_DiMuonPhi.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingMuonPt.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingMuonEta.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingMuonPhi.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingMuonCharge.push_back( std::vector<TH1F*>() );
    m_hVector_SecondMuonPt.push_back( std::vector<TH1F*>() );
    m_hVector_SecondMuonEta.push_back( std::vector<TH1F*>() );
    m_hVector_SecondMuonPhi.push_back( std::vector<TH1F*>() );
    m_hVector_SecondMuonCharge.push_back( std::vector<TH1F*>() );
    m_hVector_MuonsN.push_back( std::vector<TH1F*>() );
    m_hVector_sumEHFplus.push_back( std::vector<TH1F*>() );
    m_hVector_sumEHFminus.push_back( std::vector<TH1F*>() );
    m_hVector_sumEHEplus.push_back( std::vector<TH1F*>() );
    m_hVector_sumEHEminus.push_back( std::vector<TH1F*>() );
    m_hVector_lumi.push_back( std::vector<TH1F*>() );
    m_hVector_asumE.push_back( std::vector<TH1F*>() );
    m_hVector_multhf.push_back( std::vector<TH2D*>() );
    m_hVector_etcalos_p.push_back( std::vector<TH2D*>() );
    m_hVector_etcalos_n.push_back( std::vector<TH2D*>() );
    m_hVector_tracks.push_back( std::vector<TH1F*>() );
    m_hVector_pfetamax.push_back( std::vector<TH1F*>() );
    m_hVector_pfetamin.push_back( std::vector<TH1F*>() );
    m_hVector_asumE.push_back( std::vector<TH1F*>() );
    m_hVector_vertex.push_back( std::vector<TH1F*>() );
    m_hVector_deltaphielectrons.push_back( std::vector<TH1F*>() );
    m_hVector_deltaphimuons.push_back( std::vector<TH1F*>() );
    m_hVector_deltapTelectrons.push_back( std::vector<TH1F*>() );
    m_hVector_deltapTmuons.push_back( std::vector<TH1F*>() );
    m_hVector_deltaetaelectrons.push_back( std::vector<TH1F*>() );
    m_hVector_deltaetamuons.push_back( std::vector<TH1F*>() );
    m_hVector_vertexvslumi.push_back( std::vector<TH2D*>() );
    m_hVector_patNElectron.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron1Pt.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron1Eta.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron1Phi.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron1Charge.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron1Et.push_back( std::vector<TH1F*>() );
    m_hVector_patNMuon.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon1Pt.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon1Eta.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon1Phi.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon1Charge.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon1Et.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron2Pt.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron2Eta.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron2Phi.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron2Charge.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron2Et.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon2Pt.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon2Eta.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon2Phi.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon2Charge.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon2Et.push_back( std::vector<TH1F*>() );
    m_hVector_patDiElectronMass.push_back( std::vector<TH1F*>() );
    m_hVector_patDiMuonMass.push_back( std::vector<TH1F*>() );
    m_hVector_patDiElectronPt.push_back( std::vector<TH1F*>() );
    m_hVector_patDiMuonPt.push_back( std::vector<TH1F*>() );
    m_hVector_patDiElectronEta.push_back( std::vector<TH1F*>() );
    m_hVector_patDiMuonEta.push_back( std::vector<TH1F*>() );
    m_hVector_patDiElectronPhi.push_back( std::vector<TH1F*>() );
    m_hVector_patDiMuonPhi.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron1TkDr03.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron1TkDr04.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron1EcalDr03.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron1EcalDr04.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron1HcalDr03.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron1HcalDr04.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron2TkDr03.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron2TkDr04.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron2EcalDr03.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron2EcalDr04.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron2HcalDr03.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron2HcalDr04.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron1relIsoDr03.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron1relIsoDr04.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron2relIsoDr03.push_back( std::vector<TH1F*>() );
    m_hVector_patElectron2relIsoDr04.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon1SumPtR03.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon1SumPtR05.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon1EmEtR03.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon1EmEtR05.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon1HadEtR03.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon1HadEtR05.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon2SumPtR03.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon2SumPtR05.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon2EmEtR03.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon2EmEtR05.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon2HadEtR03.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon2HadEtR05.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon1relIsoDr03.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon1relIsoDr05.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon2relIsoDr03.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon2relIsoDr05.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon1relIso.push_back( std::vector<TH1F*>() );
    m_hVector_patMuon2relIso.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronTkDr03.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronTkDr04.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronEcalDr03.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronEcalDr04.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronHcalDr03.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronHcalDr04.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronTkDr03.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronTkDr04.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronEcalDr03.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronEcalDr04.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronHcalDr03.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronHcalDr04.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronrelIsoDr03.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronrelIsoDr04.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronrelIsoDr03.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronrelIsoDr04.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingMuonSumPtR03.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingMuonSumPtR05.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingMuonEmEtR03.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingMuonEmEtR05.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingMuonHadEtR03.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingMuonHadEtR05.push_back( std::vector<TH1F*>() );
    m_hVector_SecondMuonSumPtR03.push_back( std::vector<TH1F*>() );
    m_hVector_SecondMuonSumPtR05.push_back( std::vector<TH1F*>() );
    m_hVector_SecondMuonEmEtR03.push_back( std::vector<TH1F*>() );
    m_hVector_SecondMuonEmEtR05.push_back( std::vector<TH1F*>() );
    m_hVector_SecondMuonHadEtR03.push_back( std::vector<TH1F*>() );
    m_hVector_SecondMuonHadEtR05.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingMuonrelIsoDr03.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingMuonrelIsoDr05.push_back( std::vector<TH1F*>() );
    m_hVector_SecondMuonrelIsoDr03.push_back( std::vector<TH1F*>() );
    m_hVector_SecondMuonrelIsoDr05.push_back( std::vector<TH1F*>() );

    m_hVector_correlationElectron1Pt.push_back( std::vector<TH2D*>() );
    m_hVector_correlationElectron1Eta.push_back( std::vector<TH2D*>() );
    m_hVector_correlationElectron1Phi.push_back( std::vector<TH2D*>() );
    m_hVector_correlationElectron1Iso.push_back( std::vector<TH2D*>() );
    m_hVector_correlationMuon1Pt.push_back( std::vector<TH2D*>() );
    m_hVector_correlationMuon1Eta.push_back( std::vector<TH2D*>() );
    m_hVector_correlationMuon1Phi.push_back( std::vector<TH2D*>() );
    m_hVector_correlationMuon1Iso.push_back( std::vector<TH2D*>() );

    m_hVector_correlationElectron2Pt.push_back( std::vector<TH2D*>() );
    m_hVector_correlationElectron2Eta.push_back( std::vector<TH2D*>() );
    m_hVector_correlationElectron2Phi.push_back( std::vector<TH2D*>() );
    m_hVector_correlationElectron2Iso.push_back( std::vector<TH2D*>() );
    m_hVector_correlationMuon2Pt.push_back( std::vector<TH2D*>() );
    m_hVector_correlationMuon2Eta.push_back( std::vector<TH2D*>() );
    m_hVector_correlationMuon2Phi.push_back( std::vector<TH2D*>() );
    m_hVector_correlationMuon2Iso.push_back( std::vector<TH2D*>() );

    m_hVector_sumECastorMinus.push_back( std::vector<TH1F*>() );
    m_hVector_ECastorSector.push_back( std::vector<TH2D*>() );
    m_hVector_ECaloVsEta.push_back( std::vector<TH2D*>() );
    m_hVector_ECastorSectorTProf.push_back( std::vector<TProfile*>() );
    m_hVector_ECaloVsEtaTProf.push_back( std::vector<TProfile*>() );

    m_hVector_tracksOutMuonsCone03.push_back( std::vector<TH1F*>() );
    m_hVector_tracksOutElectronsCone03.push_back( std::vector<TH1F*>() );
    m_hVector_tracksOutpatMuonsCone03.push_back( std::vector<TH1F*>() );
    m_hVector_tracksOutpatElectronsCone03.push_back( std::vector<TH1F*>() );

    m_hVector_tracksOutMuonsCone04.push_back( std::vector<TH1F*>() );
    m_hVector_tracksOutElectronsCone04.push_back( std::vector<TH1F*>() );
    m_hVector_tracksOutpatMuonsCone04.push_back( std::vector<TH1F*>() );
    m_hVector_tracksOutpatElectronsCone04.push_back( std::vector<TH1F*>() );

    m_hVector_tracksOutMuonsCone05.push_back( std::vector<TH1F*>() );
    m_hVector_tracksOutElectronsCone05.push_back( std::vector<TH1F*>() );
    m_hVector_tracksOutpatMuonsCone05.push_back( std::vector<TH1F*>() );
    m_hVector_tracksOutpatElectronsCone05.push_back( std::vector<TH1F*>() );

    m_hVector_LeadingElectronInnerHits.push_back( std::vector<TH1I*>() );
    m_hVector_LeadingElectronDCot.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronDist.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronDeltaEtaTkClu.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronDeltaPhiTkClu.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronSigmaIeIe.push_back( std::vector<TH1F*>() );
    m_hVector_LeadingElectronHE.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronInnerHits.push_back( std::vector<TH1I*>() );
    m_hVector_SecondElectronDCot.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronDist.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronDeltaEtaTkClu.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronDeltaPhiTkClu.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronSigmaIeIe.push_back( std::vector<TH1F*>() );
    m_hVector_SecondElectronHE.push_back( std::vector<TH1F*>() );

    m_hVector_SumEHFplus_S.push_back( std::vector<TH1F*>() );
    m_hVector_SumEHFminus_S.push_back( std::vector<TH1F*>() );
    m_hVector_SumEHFplus_L.push_back( std::vector<TH1F*>() );
    m_hVector_SumEHFminus_L.push_back( std::vector<TH1F*>() );

    m_hVector_maxetagap.push_back( std::vector<TH1F*>() );

    m_hVector_ElectronsPt.push_back( std::vector<TH1F*>() );
    m_hVector_ElectronsEta.push_back( std::vector<TH1F*>() );
    m_hVector_ElectronsPhi.push_back( std::vector<TH1F*>() );
    m_hVector_MuonsPt.push_back( std::vector<TH1F*>() );
    m_hVector_MuonsEta.push_back( std::vector<TH1F*>() );
    m_hVector_MuonsPhi.push_back( std::vector<TH1F*>() );
    m_hVector_patElectronsPt.push_back( std::vector<TH1F*>() );
    m_hVector_patElectronsEta.push_back( std::vector<TH1F*>() );
    m_hVector_patElectronsPhi.push_back( std::vector<TH1F*>() );
    m_hVector_patMuonsPt.push_back( std::vector<TH1F*>() );
    m_hVector_patMuonsEta.push_back( std::vector<TH1F*>() );
    m_hVector_patMuonsPhi.push_back( std::vector<TH1F*>() );

    m_hVector_SumEHFMax.push_back( std::vector<TH1F*>() );
    m_hVector_SumEHFMin.push_back( std::vector<TH1F*>() );

    m_hVector_EnergyVsEtaBin1D.push_back( std::vector<TH1F*>() );

    m_hVector_absdeltaEtaPF.push_back( std::vector<TH1F*>() );
    m_hVector_deltaEtaPF.push_back( std::vector<TH1F*>() );

    m_hVector_ECastorSectorBin1D.push_back( std::vector<TH1F*>() );

    m_hVector_TotalEnergyCastor_sector1.push_back( std::vector<TH1F*>() );
    m_hVector_TotalEnergyCastor_sector2.push_back( std::vector<TH1F*>() );
    m_hVector_TotalEnergyCastor_sector3.push_back( std::vector<TH1F*>() );
    m_hVector_TotalEnergyCastor_sector4.push_back( std::vector<TH1F*>() );
    m_hVector_TotalEnergyCastor_sector5.push_back( std::vector<TH1F*>() );
    m_hVector_TotalEnergyCastor_sector6.push_back( std::vector<TH1F*>() );
    m_hVector_TotalEnergyCastor_sector7.push_back( std::vector<TH1F*>() );
    m_hVector_TotalEnergyCastor_sector8.push_back( std::vector<TH1F*>() );
    m_hVector_TotalEnergyCastor_sector9.push_back( std::vector<TH1F*>() );
    m_hVector_TotalEnergyCastor_sector10.push_back( std::vector<TH1F*>() );
    m_hVector_TotalEnergyCastor_sector11.push_back( std::vector<TH1F*>() );
    m_hVector_TotalEnergyCastor_sector12.push_back( std::vector<TH1F*>() );
    m_hVector_TotalEnergyCastor_sector13.push_back( std::vector<TH1F*>() );
    m_hVector_TotalEnergyCastor_sector14.push_back( std::vector<TH1F*>() );
    m_hVector_TotalEnergyCastor_sector15.push_back( std::vector<TH1F*>() );
    m_hVector_TotalEnergyCastor_sector16.push_back( std::vector<TH1F*>() );

    m_hVector_EnergyHFMinusVsCastorTProf.push_back( std::vector<TProfile*>() );
    m_hVector_EnergyHFPlusVsCastorTProf.push_back( std::vector<TProfile*>() );

    m_hVector_sumECastorAndHFMinus.push_back( std::vector<TH1F*>() );
    m_hVector_CastorMultiplicity.push_back( std::vector<TH1I*>() );
    m_hVector_CastorMultiplicityVsLumi.push_back( std::vector<TH2D*>() );

    m_hVector_RunNumber.push_back( std::vector<TH1I*>() );
    m_hVector_RunNumberZeroCastor.push_back( std::vector<TH1I*>() );
    m_hVector_RunNumberHighCastor.push_back( std::vector<TH1I*>() );

    for (int k=0;k<nloop;k++){

      if (type=="multiple_pileup"){
	sprintf(tag,"multiple_pileup_%i",k);
      }
      else {
	sprintf(tag,"single");
      }

      char name1[300];
      sprintf(name1,"DiElectronMass_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_DiElectronMass = new TH1F(name1,"Dielectron Invariant Mass Distribution; M_{ee} [GeV]; N events",500,0,500);
      m_hVector_DiElectronMass[j].push_back(histo_DiElectronMass);

      char name2[300];
      sprintf(name2,"LeadingElectronPt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronPt = new TH1F(name2,"Leading Electron - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",500,0,500);
      m_hVector_LeadingElectronPt[j].push_back(histo_LeadingElectronPt);

      char name3[300];
      sprintf(name3,"LeadingElectronEta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronEta = new TH1F(name3,"Leading Electron - #eta Distribution; #eta; N events",60,-6,6);
      m_hVector_LeadingElectronEta[j].push_back(histo_LeadingElectronEta);

      char name4[300];
      sprintf(name4,"LeadingElectronPhi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronPhi = new TH1F(name4,"Leading Electron - #phi Distribution; #phi [rad]; N events",16,-3.2,3.2);
      m_hVector_LeadingElectronPhi[j].push_back(histo_LeadingElectronPhi);

      char name5[300];
      sprintf(name5,"LeadingElectronCharge_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronCharge = new TH1F(name5,"Leading Electron - Charge Distribution; Charge; N events",6,-3,3);
      m_hVector_LeadingElectronCharge[j].push_back(histo_LeadingElectronCharge);

      char name6[300];
      sprintf(name6,"SecondElectronPt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronPt = new TH1F(name6,"Second Electron - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",500,0,500);
      m_hVector_SecondElectronPt[j].push_back(histo_SecondElectronPt);

      char name7[300];
      sprintf(name7,"SecondElectronEta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronEta = new TH1F(name7,"Second Electron - #eta Distribution; #eta; N events",60,-6,6);
      m_hVector_SecondElectronEta[j].push_back(histo_SecondElectronEta);

      char name8[300];
      sprintf(name8,"SecondElectronPhi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronPhi = new TH1F(name8,"Second Electron - #phi Distribution; #phi [rad]; N events",16,-3.2,3.2);
      m_hVector_SecondElectronPhi[j].push_back(histo_SecondElectronPhi);

      char name9[300];
      sprintf(name9,"SecondElectronCharge_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronCharge = new TH1F(name9,"Second Electron - Charge Distribution; Charge; N events",6,-3,3);
      m_hVector_SecondElectronCharge[j].push_back(histo_SecondElectronCharge);

      char name10[300];
      sprintf(name10,"ElectronsN_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_ElectronsN = new TH1F(name10,"Electrons per Event Distribution; Number of Electrons; N events",100,0,100);
      m_hVector_ElectronsN[j].push_back(histo_ElectronsN);

      char name11[300];
      sprintf(name11,"DiMuonMass_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_DiMuonMass = new TH1F(name11,"DiMuon Invariant Mass Distribution; M_{#mu#mu} [GeV]; N events",500,0,500);
      m_hVector_DiMuonMass[j].push_back(histo_DiMuonMass);

      char name12[300];
      sprintf(name12,"LeadingMuonPt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingMuonPt = new TH1F(name12,"Leading Muon - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",500,0,500);
      m_hVector_LeadingMuonPt[j].push_back(histo_LeadingMuonPt);

      char name13[300];
      sprintf(name13,"LeadingMuonEta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingMuonEta = new TH1F(name13,"Leading Muon - #eta Distribution; #eta; N events",60,-6,6);
      m_hVector_LeadingMuonEta[j].push_back(histo_LeadingMuonEta);

      char name14[300];
      sprintf(name14,"LeadingMuonPhi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingMuonPhi = new TH1F(name14,"Leading Muon - #phi Distribution; #phi [rad]; N events",16,-3.2,3.2);
      m_hVector_LeadingMuonPhi[j].push_back(histo_LeadingMuonPhi);

      char name15[300];
      sprintf(name15,"LeadingMuonCharge_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingMuonCharge = new TH1F(name15,"Leading Muon - Charge Distribution; Charge; N events",6,-3,3);
      m_hVector_LeadingMuonCharge[j].push_back(histo_LeadingMuonCharge);

      char name16[300];
      sprintf(name16,"SecondMuonPt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondMuonPt = new TH1F(name16,"Second Muon - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",500,0,500);
      m_hVector_SecondMuonPt[j].push_back(histo_SecondMuonPt);

      char name17[300];
      sprintf(name17,"SecondMuonEta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondMuonEta = new TH1F(name17,"Second Muon - #eta Distribution; #eta; N events",60,-6,6);
      m_hVector_SecondMuonEta[j].push_back(histo_SecondMuonEta);

      char name18[300];
      sprintf(name18,"SecondMuonPhi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondMuonPhi = new TH1F(name18,"Second Muon - #phi Distribution; #phi [rad]; N events",16,-3.2,3.2);
      m_hVector_SecondMuonPhi[j].push_back(histo_SecondMuonPhi);

      char name19[300];
      sprintf(name19,"SecondMuonCharge_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondMuonCharge = new TH1F(name19,"Second Muon - Charge Distribution; Charge; N events",6,-3,3);
      m_hVector_SecondMuonCharge[j].push_back(histo_SecondMuonCharge);

      char name20[300];
      sprintf(name20,"MuonsN_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_MuonsN = new TH1F(name20,"Muons per Event Distribution; Number of Muons; N events",100,0,100);
      m_hVector_MuonsN[j].push_back(histo_MuonsN);

      char name21[300];
      sprintf(name21,"sumEHFplus_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_sumEHFplus = new TH1F(name21,"HF^{+} - Sum of Energy; #sum E_{HF^{+}} [GeV]; N events",2000,0,2000);
      m_hVector_sumEHFplus[j].push_back(histo_sumEHFplus);

      char name22[300];
      sprintf(name22,"sumEHFminus_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_sumEHFminus = new TH1F(name22,"HF^{-} - Sum of Energy; #sum E_{HF^{-}} [GeV]; N events",2000,0,2000);
      m_hVector_sumEHFminus[j].push_back(histo_sumEHFminus);

      char name23[300];
      sprintf(name23,"sumEHEplus_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_sumEHEplus = new TH1F(name23,"HE^{+} - Sum of Energy; #sum E_{HE^{+}} [GeV]; N events",2000,0,2000);
      m_hVector_sumEHEplus[j].push_back(histo_sumEHEplus);

      char name24[300];
      sprintf(name24,"sumEHEminus_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_sumEHEminus = new TH1F(name24,"HE^{-} - Sum of Energy; #sum E_{HE^{-}} [GeV]; N events",2000,0,2000);
      m_hVector_sumEHEminus[j].push_back(histo_sumEHEminus);

      char name25[300];
      sprintf(name25,"lumi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_lumi = new TH1F(name25,"Luminosity per Bunch; L_{Bunch} [#mub^{-1}s^{-1}]; N events",25,0,2);
      m_hVector_lumi[j].push_back(histo_lumi);      

      char name26[300];
      sprintf(name26,"aEnergy_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_aSumE = new TH1F(name26,"Forward Backward Asymmetry Distribution ; (#sum HF^{+} - #sum HF^{-})x(#sum HF^{+} + #sum HF^{-})^{-1}; N events",100,-2,2);
      m_hVector_asumE[j].push_back(histo_aSumE);

      char name27[300];
      sprintf(name27,"mHF_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_MultHF = new TH2D(name27,"HF^{+} and HF^{-} Multiplicity; n HF^{+}; n HF^{-}; N events", 100,  0., 100., 100,  0., 100. );
      m_hVector_multhf[j].push_back(histo_MultHF);

      char name28[300];
      sprintf(name28,"EnergyHFPlusVsCastor_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_ET_Calos_p = new TH2D(name28,"HF^{+} and Castor; #sum Energy HF^{+}; #sum Energy Castor [GeV]; N events", 1000,  0., 1000., 6000,  0., 3000. );
      m_hVector_etcalos_p[j].push_back(histo_ET_Calos_p);

      char name29[300];
      sprintf(name29,"Tracks_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_Tracks = new TH1F(name29,"Tracks Multiplicity; n Tracks; N events",150,0,150);
      m_hVector_tracks[j].push_back(histo_Tracks);

      char name30[300];
      sprintf(name30,"pfetamax_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_PFEtamax = new TH1F(name30,"Particle Flow #eta_{max} Distribution; #eta; N events",17,binarrayplus);
      m_hVector_pfetamax[j].push_back(histo_PFEtamax);

      char name31[300];
      sprintf(name31,"pfetamin_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_PFEtamin = new TH1F(name31,"Particle Flow #eta_{min} Distribution; #eta; N events",17,binarrayminus);
      m_hVector_pfetamin[j].push_back(histo_PFEtamin);

      char name32[300];
      sprintf(name32,"vertex_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_vertex = new TH1F(name32,"Number of Vertex; # Vertex; N events",25,0,25);
      m_hVector_vertex[j].push_back(histo_vertex);

      char name33[300];
      sprintf(name33,"deltaphiElectrons_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_deltaphielectrons = new TH1F(name33,"#Delta#phi_{ee} Distribution; #Delta#phi_{ee}; N events",20,0.0,3.2);
      m_hVector_deltaphielectrons[j].push_back(histo_deltaphielectrons);

      char name34[300];
      sprintf(name34,"deltaphiMuons_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_deltaphimuons = new TH1F(name34,"#Delta#phi_{#mu#mu} Distribution; #Delta#phi_{#mu#mu}; N events",20,0.0,3.2);
      m_hVector_deltaphimuons[j].push_back(histo_deltaphimuons);

      char name35[300];
      sprintf(name35,"deltapTElectrons_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_deltapTelectrons = new TH1F(name35,"#Delta pT_{ee} Distribution; #Delta pT_{ee} [GeV.c^{-1}]; N events",50,0.0,150);
      m_hVector_deltapTelectrons[j].push_back(histo_deltapTelectrons);

      char name36[300];
      sprintf(name36,"deltapTMuons_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_deltapTmuons = new TH1F(name36,"#Delta pT_{#mu#mu} Distribution; #Delta pT_{#mu#mu} [GeV.c^{-1}]; N events",50,0.0,150);
      m_hVector_deltapTmuons[j].push_back(histo_deltapTmuons);

      char name37[300];
      sprintf(name37,"deltaetaElectrons_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_deltaetaelectrons = new TH1F(name37,"#Delta#eta_{ee} Distribution; #Delta#eta_{ee}; N events",50,-11,11);
      m_hVector_deltaetaelectrons[j].push_back(histo_deltaetaelectrons);

      char name38[300];
      sprintf(name38,"deltaetaMuons_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_deltaetamuons = new TH1F(name38,"#Delta#eta_{#mu#mu} Distribution; #Delta#eta_{#mu#mu}; N events",50,-11,11);
      m_hVector_deltaetamuons[j].push_back(histo_deltaetamuons);

      char name39[300];
      sprintf(name39,"VertexVsLuminosity_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_vertexvslumi = new TH2D(name39,"Vertex vs Luminosity; # Vertex; Luminosity per Bunch [#mub^{-1}s^{-1}]", 25.,  0., 25., 25,  0., 2.);
      m_hVector_vertexvslumi[j].push_back(histo_vertexvslumi);

      char name40[300];
      sprintf(name40,"patNElectron_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patNElectron = new TH1F(name40,"Number of Electrons; # Electrons; Multiplicity", 100,  0., 100.);
      m_hVector_patNElectron[j].push_back(histo_patNElectron);

      char name41[300];
      sprintf(name41,"patElectron1Pt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1Pt = new TH1F(name41,"Leading Electron - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",500,0,500);
      m_hVector_patElectron1Pt[j].push_back(histo_patElectron1Pt);

      char name42[300];
      sprintf(name42,"patElectron1Eta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1Eta = new TH1F(name42,"Leading Electron - #eta Distribution; #eta; N events",60,-6,6);
      m_hVector_patElectron1Eta[j].push_back(histo_patElectron1Eta);

      char name43[300];
      sprintf(name43,"patElectron1Phi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1Phi = new TH1F(name43,"Leading Electron - #phi Distribution; #phi [rad]; N events",16,-3.2,3.2);
      m_hVector_patElectron1Phi[j].push_back(histo_patElectron1Phi);

      char name44[300];
      sprintf(name44,"patElectron1Charge_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1Charge= new TH1F(name44,"Leading Electron - Charge Distribution; Charge; N events",6,-3,3);
      m_hVector_patElectron1Charge[j].push_back(histo_patElectron1Charge);

      char name45[300];
      sprintf(name45,"patElectron1Et_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1Et = new TH1F(name45,"Leading Electron - E_{T} Distribution; E_{T} [GeV]; N events",500,0,500);
      m_hVector_patElectron1Et[j].push_back(histo_patElectron1Et);

      char name46[300];
      sprintf(name46,"patNMuon_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patNMuon = new TH1F(name46,"Number of Muons; # Muons; Multiplicity", 100,  0., 100.);
      m_hVector_patNMuon[j].push_back(histo_patNMuon);

      char name47[300];
      sprintf(name47,"patMuon1Pt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1Pt = new TH1F(name47,"Leading Muon - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",500,0,500);
      m_hVector_patMuon1Pt[j].push_back(histo_patMuon1Pt);

      char name48[300];
      sprintf(name48,"patMuon1Eta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1Eta = new TH1F(name48,"Leading Muon - #eta Distribution; #eta; N events",60,-6,6);
      m_hVector_patMuon1Eta[j].push_back(histo_patMuon1Eta);

      char name49[300];
      sprintf(name49,"patMuon1Phi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1Phi = new TH1F(name49,"Leading Muon - #phi Distribution; #phi [rad]; N events",16,-3.2,3.2);
      m_hVector_patMuon1Phi[j].push_back(histo_patMuon1Phi);

      char name50[300];
      sprintf(name50,"patMuon1Charge_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1Charge= new TH1F(name50,"Leading Muon - Charge Distribution; Charge; N events",6,-3,3);
      m_hVector_patMuon1Charge[j].push_back(histo_patMuon1Charge);

      char name51[300];
      sprintf(name51,"patMuon1Et_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1Et = new TH1F(name51,"Leading Muon - E_{T} Distribution; E_{T} [GeV]; N events",500,0,500);
      m_hVector_patMuon1Et[j].push_back(histo_patMuon1Et);

      char name52[300];
      sprintf(name52,"patElectron2Pt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2Pt = new TH1F(name52,"Second Electron - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",500,0,500);
      m_hVector_patElectron2Pt[j].push_back(histo_patElectron2Pt);

      char name53[300];
      sprintf(name53,"patElectron2Eta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2Eta = new TH1F(name53,"Second Electron - #eta Distribution; #eta; N events",60,-6,6);
      m_hVector_patElectron2Eta[j].push_back(histo_patElectron2Eta);

      char name54[300];
      sprintf(name54,"patElectron2Phi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2Phi = new TH1F(name54,"Second Electron - #phi Distribution; #phi [rad]; N events",16,-3.2,3.2);
      m_hVector_patElectron2Phi[j].push_back(histo_patElectron2Phi);

      char name55[300];
      sprintf(name55,"patElectron2Charge_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2Charge= new TH1F(name55,"Second Electron - Charge Distribution; Charge; N events",6,-3,3);
      m_hVector_patElectron2Charge[j].push_back(histo_patElectron2Charge);

      char name56[300];
      sprintf(name56,"patElectron2Et_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2Et = new TH1F(name56,"Second Electron - E_{T} Distribution; E_{T} [GeV]; N events",500,0,500);
      m_hVector_patElectron2Et[j].push_back(histo_patElectron2Et);

      char name57[300];
      sprintf(name57,"patMuon2Pt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2Pt = new TH1F(name57,"Second Muon - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",500,0,500);
      m_hVector_patMuon2Pt[j].push_back(histo_patMuon2Pt);

      char name58[300];
      sprintf(name58,"patMuon2Eta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2Eta = new TH1F(name58,"Second Muon - #eta Distribution; #eta; N events",60,-6,6);
      m_hVector_patMuon2Eta[j].push_back(histo_patMuon2Eta);

      char name59[300];
      sprintf(name59,"patMuon2Phi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2Phi = new TH1F(name59,"Second Muon - #phi Distribution; #phi [rad]; N events",16,-3.2,3.2);
      m_hVector_patMuon2Phi[j].push_back(histo_patMuon2Phi);

      char name60[300];
      sprintf(name60,"patMuon2Charge_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2Charge= new TH1F(name60,"Second Muon - Charge Distribution; Charge; N events",6,-3,3);
      m_hVector_patMuon2Charge[j].push_back(histo_patMuon2Charge);

      char name61[300];
      sprintf(name61,"patMuon2Et_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2Et = new TH1F(name61,"Second Muon - E_{T} Distribution; E_{T} [GeV]; N events",500,0,500);
      m_hVector_patMuon2Et[j].push_back(histo_patMuon2Et);

      char name62[300];
      sprintf(name62,"patDiElectronMass_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patDiElectronMass = new TH1F(name62,"Dielectron Invariant Mass Distribution; M_{ee} [GeV]; N events",500,0,500);
      m_hVector_patDiElectronMass[j].push_back(histo_patDiElectronMass);

      char name63[300];
      sprintf(name63,"patDiMuonMass_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patDiMuonMass = new TH1F(name63,"DiMuon Invariant Mass Distribution; M_{#mu#mu} [GeV]; N events",500,0,500);
      m_hVector_patDiMuonMass[j].push_back(histo_patDiMuonMass);

      char name64[300];
      sprintf(name64,"patElectron1TkDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1TkDr03 = new TH1F(name64,"Leading Electron: Tracker Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patElectron1TkDr03[j].push_back(histo_patElectron1TkDr03);

      char name65[300];
      sprintf(name65,"patElectron1TkDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1TkDr04 = new TH1F(name65,"Leading Electron: Tracker Isolation DR04; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patElectron1TkDr04[j].push_back(histo_patElectron1TkDr04);

      char name66[300];
      sprintf(name66,"patElectron1EcalDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1EcalDr03 = new TH1F(name66,"Leading Electron: ECAL Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patElectron1EcalDr03[j].push_back(histo_patElectron1EcalDr03);

      char name67[300];
      sprintf(name67,"patElectron1EcalDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1EcalDr04 = new TH1F(name67,"Leading Electron: ECAL Isolation DR04; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patElectron1EcalDr04[j].push_back(histo_patElectron1EcalDr04);

      char name68[300];
      sprintf(name68,"patElectron1HcalDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1HcalDr03 = new TH1F(name68,"Leading Electron: HCAL Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patElectron1HcalDr03[j].push_back(histo_patElectron1HcalDr03);

      char name69[300];
      sprintf(name69,"patElectron1HcalDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1HcalDr04 = new TH1F(name69,"Leading Electron: HCAL Isolation DR04; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patElectron1HcalDr04[j].push_back(histo_patElectron1HcalDr04);

      char name70[300];
      sprintf(name70,"patElectron2TkDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2TkDr03 = new TH1F(name70,"Second Electron: Tracker Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patElectron2TkDr03[j].push_back(histo_patElectron2TkDr03);

      char name71[300];
      sprintf(name71,"patElectron2TkDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2TkDr04 = new TH1F(name71,"Second Electron: Tracker Isolation DR04; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patElectron2TkDr04[j].push_back(histo_patElectron2TkDr04);

      char name72[300];
      sprintf(name72,"patElectron2EcalDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2EcalDr03 = new TH1F(name72,"Second Electron: ECAL Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patElectron2EcalDr03[j].push_back(histo_patElectron2EcalDr03);

      char name73[300];
      sprintf(name73,"patElectron2EcalDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2EcalDr04 = new TH1F(name73,"Second Electron: ECAL Isolation DR04; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patElectron2EcalDr04[j].push_back(histo_patElectron2EcalDr04);

      char name74[300];
      sprintf(name74,"patElectron2HcalDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2HcalDr03 = new TH1F(name74,"Second Electron: HCAL Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patElectron2HcalDr03[j].push_back(histo_patElectron2HcalDr03);

      char name75[300];
      sprintf(name75,"patElectron2HcalDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2HcalDr04 = new TH1F(name75,"Second Electron: HCAL Isolation DR04; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patElectron2HcalDr04[j].push_back(histo_patElectron2HcalDr04);

      char name76[300];
      sprintf(name76,"patElectron1relIsoDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1relIsoDr03 = new TH1F(name76,"Leading Electron: Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patElectron1relIsoDr03[j].push_back(histo_patElectron1relIsoDr03);

      char name77[300];
      sprintf(name77,"patElectron1relIsoDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron1relIsoDr04 = new TH1F(name77,"Leading Electron: Isolation DR04; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patElectron1relIsoDr04[j].push_back(histo_patElectron1relIsoDr04);

      char name78[300];
      sprintf(name78,"patElectron2relIsoDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2relIsoDr03 = new TH1F(name78,"Second Electron: Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patElectron2relIsoDr03[j].push_back(histo_patElectron2relIsoDr03);

      char name79[300];
      sprintf(name79,"patElectron2relIsoDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectron2relIsoDr04 = new TH1F(name79,"Second Electron: Isolation DR04; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patElectron2relIsoDr04[j].push_back(histo_patElectron2relIsoDr04);

      char name80[300];
      sprintf(name80,"patMuon1SumPtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1SumPtR03 = new TH1F(name80,"Leading Muon: Tracker Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon1SumPtR03[j].push_back(histo_patMuon1SumPtR03);

      char name81[300];
      sprintf(name81,"patMuon1SumPtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1SumPtR05 = new TH1F(name81,"Leading Muon: Tracker Isolation DR05; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon1SumPtR05[j].push_back(histo_patMuon1SumPtR05);

      char name82[300];
      sprintf(name82,"patMuon1EmEtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1EmEtR03 = new TH1F(name82,"Leading Muon: ECAL Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon1EmEtR03[j].push_back(histo_patMuon1EmEtR03);

      char name83[300];
      sprintf(name83,"patMuon1EmEtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1EmEtR05 = new TH1F(name83,"Leading Muon: ECAL Isolation DR05; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon1EmEtR05[j].push_back(histo_patMuon1EmEtR05);

      char name84[300];
      sprintf(name84,"patMuon1HadEtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1HadEtR03 = new TH1F(name84,"Leading Muon: HCAL Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon1HadEtR03[j].push_back(histo_patMuon1HadEtR03);

      char name85[300];
      sprintf(name85,"patMuon1HadEtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1HadEtR05 = new TH1F(name85,"Leading Muon: HCAL Isolation DR05; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon1HadEtR05[j].push_back(histo_patMuon1HadEtR05);

      char name86[300];
      sprintf(name86,"patMuon2SumPtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2SumPtR03 = new TH1F(name86,"Second Muon: Tracker Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon2SumPtR03[j].push_back(histo_patMuon2SumPtR03);

      char name87[300];
      sprintf(name87,"patMuon2SumPtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2SumPtR05 = new TH1F(name87,"Second Muon: Tracker Isolation DR05; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon2SumPtR05[j].push_back(histo_patMuon2SumPtR05);

      char name88[300];
      sprintf(name88,"patMuon2EmEtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2EmEtR03 = new TH1F(name88,"Second Muon: ECAL Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon2EmEtR03[j].push_back(histo_patMuon2EmEtR03);

      char name89[300];
      sprintf(name89,"patMuon2EmEtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2EmEtR05 = new TH1F(name89,"Second Muon: ECAL Isolation DR05; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon2EmEtR05[j].push_back(histo_patMuon2EmEtR05);

      char name90[300];
      sprintf(name90,"patMuon2HadEtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2HadEtR03 = new TH1F(name90,"Second Muon: HCAL Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon2HadEtR03[j].push_back(histo_patMuon2HadEtR03);

      char name91[300];
      sprintf(name91,"patMuon2HadEtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2HadEtR05 = new TH1F(name91,"Second Muon: HCAL Isolation DR05; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon2HadEtR05[j].push_back(histo_patMuon2HadEtR05);

      char name92[300];
      sprintf(name92,"patMuon1relIsoDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1relIsoDr03 = new TH1F(name92,"Leading Muon: Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon1relIsoDr03[j].push_back(histo_patMuon1relIsoDr03);

      char name93[300];
      sprintf(name93,"patMuon1relIsoDr05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1relIsoDr05 = new TH1F(name93,"Leading Muon: Isolation DR05; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon1relIsoDr05[j].push_back(histo_patMuon1relIsoDr05);

      char name94[300];
      sprintf(name94,"patMuon2relIsoDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2relIsoDr03 = new TH1F(name94,"Second Muon: Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon2relIsoDr03[j].push_back(histo_patMuon2relIsoDr03);

      char name95[300];
      sprintf(name95,"patMuon2relIsoDr05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2relIsoDr05 = new TH1F(name95,"Second Muon: Isolation DR05; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon2relIsoDr05[j].push_back(histo_patMuon2relIsoDr05);

      char name96[300];
      sprintf(name96,"patMuon1relIso_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon1relIso = new TH1F(name96,"Leading Muon: Isolation; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon1relIso[j].push_back(histo_patMuon1relIso);

      char name97[300];
      sprintf(name97,"patMuon2relIso_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuon2relIso = new TH1F(name97,"Second Muon: Isolation; # Isolation; [u]", 100,  0., 1.);
      m_hVector_patMuon2relIso[j].push_back(histo_patMuon2relIso);

      char name98[300];
      sprintf(name98,"LeadingElectronTkDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronTkDr03 = new TH1F(name98,"Leading Electron: Tracker Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_LeadingElectronTkDr03[j].push_back(histo_LeadingElectronTkDr03);

      char name99[300];
      sprintf(name99,"LeadingElectronTkDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronTkDr04 = new TH1F(name99,"Leading Electron: Tracker Isolation DR04; # Isolation; [u]", 100,  0., 1.);
      m_hVector_LeadingElectronTkDr04[j].push_back(histo_LeadingElectronTkDr04);

      char name100[300];
      sprintf(name100,"LeadingElectronEcalDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronEcalDr03 = new TH1F(name100,"Leading Electron: ECAL Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_LeadingElectronEcalDr03[j].push_back(histo_LeadingElectronEcalDr03);

      char name101[300];
      sprintf(name101,"LeadingElectronEcalDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronEcalDr04 = new TH1F(name101,"Leading Electron: ECAL Isolation DR04; # Isolation; [u]", 100,  0., 1.);
      m_hVector_LeadingElectronEcalDr04[j].push_back(histo_LeadingElectronEcalDr04);

      char name102[300];
      sprintf(name102,"LeadingElectronHcalDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronHcalDr03 = new TH1F(name102,"Leading Electron: HCAL Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_LeadingElectronHcalDr03[j].push_back(histo_LeadingElectronHcalDr03);

      char name103[300];
      sprintf(name103,"LeadingElectronHcalDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronHcalDr04 = new TH1F(name103,"Leading Electron: HCAL Isolation DR04; # Isolation; [u]", 100,  0., 1.);
      m_hVector_LeadingElectronHcalDr04[j].push_back(histo_LeadingElectronHcalDr04);

      char name104[300];
      sprintf(name104,"SecondElectronTkDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronTkDr03 = new TH1F(name104,"Second Electron: Tracker Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_SecondElectronTkDr03[j].push_back(histo_SecondElectronTkDr03);

      char name105[300];
      sprintf(name105,"SecondElectronTkDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronTkDr04 = new TH1F(name105,"Second Electron: Tracker Isolation DR04; # Isolation; [u]", 100,  0., 1.);
      m_hVector_SecondElectronTkDr04[j].push_back(histo_SecondElectronTkDr04);

      char name106[300];
      sprintf(name106,"SecondElectronEcalDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronEcalDr03 = new TH1F(name106,"Second Electron: ECAL Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_SecondElectronEcalDr03[j].push_back(histo_SecondElectronEcalDr03);

      char name107[300];
      sprintf(name107,"SecondElectronEcalDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronEcalDr04 = new TH1F(name107,"Second Electron: ECAL Isolation DR04; # Isolation; [u]", 100,  0., 1.);
      m_hVector_SecondElectronEcalDr04[j].push_back(histo_SecondElectronEcalDr04);

      char name108[300];
      sprintf(name108,"SecondElectronHcalDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronHcalDr03 = new TH1F(name108,"Second Electron: HCAL Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_SecondElectronHcalDr03[j].push_back(histo_SecondElectronHcalDr03);

      char name109[300];
      sprintf(name109,"SecondElectronHcalDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronHcalDr04 = new TH1F(name109,"Second Electron: HCAL Isolation DR04; # Isolation; [u]", 100,  0., 1.);
      m_hVector_SecondElectronHcalDr04[j].push_back(histo_SecondElectronHcalDr04);

      char name110[300];
      sprintf(name110,"LeadingElectronrelIsoDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronrelIsoDr03 = new TH1F(name110,"Leading Electron: Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_LeadingElectronrelIsoDr03[j].push_back(histo_LeadingElectronrelIsoDr03);

      char name111[300];
      sprintf(name111,"LeadingElectronrelIsoDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronrelIsoDr04 = new TH1F(name111,"Leading Electron: Isolation DR04; # Isolation; [u]", 100,  0., 1.);
      m_hVector_LeadingElectronrelIsoDr04[j].push_back(histo_LeadingElectronrelIsoDr04);

      char name112[300];
      sprintf(name112,"SecondElectronrelIsoDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronrelIsoDr03 = new TH1F(name112,"Second Electron: Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_SecondElectronrelIsoDr03[j].push_back(histo_SecondElectronrelIsoDr03);

      char name113[300];
      sprintf(name113,"SecondElectronrelIsoDr04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronrelIsoDr04 = new TH1F(name113,"Second Electron: Isolation DR04; # Isolation; [u]", 100,  0., 1.);
      m_hVector_SecondElectronrelIsoDr04[j].push_back(histo_SecondElectronrelIsoDr04);

      char name114[300];
      sprintf(name114,"LeadingMuonSumPtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingMuonSumPtR03 = new TH1F(name114,"Leading Muon: Tracker Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_LeadingMuonSumPtR03[j].push_back(histo_LeadingMuonSumPtR03);

      char name115[300];
      sprintf(name115,"LeadingMuonSumPtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingMuonSumPtR05 = new TH1F(name115,"Leading Muon: Tracker Isolation DR05; # Isolation; [u]", 100,  0., 1.);
      m_hVector_LeadingMuonSumPtR05[j].push_back(histo_LeadingMuonSumPtR05);

      char name116[300];
      sprintf(name116,"LeadingMuonEmEtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingMuonEmEtR03 = new TH1F(name116,"Leading Muon: ECAL Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_LeadingMuonEmEtR03[j].push_back(histo_LeadingMuonEmEtR03);

      char name117[300];
      sprintf(name117,"LeadingMuonEmEtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingMuonEmEtR05 = new TH1F(name117,"Leading Muon: ECAL Isolation DR05; # Isolation; [u]", 100,  0., 1.);
      m_hVector_LeadingMuonEmEtR05[j].push_back(histo_LeadingMuonEmEtR05);

      char name118[300];
      sprintf(name118,"LeadingMuonHadEtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingMuonHadEtR03 = new TH1F(name118,"Leading Muon: HCAL Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_LeadingMuonHadEtR03[j].push_back(histo_LeadingMuonHadEtR03);

      char name119[300];
      sprintf(name119,"LeadingMuonHadEtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingMuonHadEtR05 = new TH1F(name119,"Leading Muon: HCAL Isolation DR05; # Isolation; [u]", 100,  0., 1.);
      m_hVector_LeadingMuonHadEtR05[j].push_back(histo_LeadingMuonHadEtR05);

      char name120[300];
      sprintf(name120,"SecondMuonSumPtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondMuonSumPtR03 = new TH1F(name120,"Second Muon: Tracker Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_SecondMuonSumPtR03[j].push_back(histo_SecondMuonSumPtR03);

      char name121[300];
      sprintf(name121,"SecondMuonSumPtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondMuonSumPtR05 = new TH1F(name121,"Second Muon: Tracker Isolation DR05; # Isolation; [u]", 100,  0., 1.);
      m_hVector_SecondMuonSumPtR05[j].push_back(histo_SecondMuonSumPtR05);

      char name122[300];
      sprintf(name122,"SecondMuonEmEtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondMuonEmEtR03 = new TH1F(name122,"Second Muon: ECAL Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_SecondMuonEmEtR03[j].push_back(histo_SecondMuonEmEtR03);

      char name123[300];
      sprintf(name123,"SecondMuonEmEtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondMuonEmEtR05 = new TH1F(name123,"Second Muon: ECAL Isolation DR05; # Isolation; [u]", 100,  0., 1.);
      m_hVector_SecondMuonEmEtR05[j].push_back(histo_SecondMuonEmEtR05);

      char name124[300];
      sprintf(name124,"SecondMuonHadEtR03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondMuonHadEtR03 = new TH1F(name124,"Second Muon: HCAL Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_SecondMuonHadEtR03[j].push_back(histo_SecondMuonHadEtR03);

      char name125[300];
      sprintf(name125,"SecondMuonHadEtR05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondMuonHadEtR05 = new TH1F(name125,"Second Muon: HCAL Isolation DR05; # Isolation; [u]", 100,  0., 1.);
      m_hVector_SecondMuonHadEtR05[j].push_back(histo_SecondMuonHadEtR05);

      char name126[300];
      sprintf(name126,"LeadingMuonrelIsoDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingMuonrelIsoDr03 = new TH1F(name126,"Leading Muon: Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_LeadingMuonrelIsoDr03[j].push_back(histo_LeadingMuonrelIsoDr03);

      char name127[300];
      sprintf(name127,"LeadingMuonrelIsoDr05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingMuonrelIsoDr05 = new TH1F(name127,"Leading Muon: Isolation DR05; # Isolation; [u]", 100,  0., 1.);
      m_hVector_LeadingMuonrelIsoDr05[j].push_back(histo_LeadingMuonrelIsoDr05);

      char name128[300];
      sprintf(name128,"SecondMuonrelIsoDr03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondMuonrelIsoDr03 = new TH1F(name128,"Second Muon: Isolation DR03; # Isolation; [u]", 100,  0., 1.);
      m_hVector_SecondMuonrelIsoDr03[j].push_back(histo_SecondMuonrelIsoDr03);

      char name129[300];
      sprintf(name129,"SecondMuonrelIsoDr05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondMuonrelIsoDr05 = new TH1F(name129,"Second Muon: Isolation DR05; # Isolation; [u]", 100,  0., 1.);
      m_hVector_SecondMuonrelIsoDr05[j].push_back(histo_SecondMuonrelIsoDr05);

      char name130[300];
      sprintf(name130,"correlationElectron1Pt_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_correlationElectron1Pt = new TH2D(name130,"Correlation Leading Electron pT; Reco::Electron P_{T} [GeV.c^{-1}] ; Pat::Electron P_{T} [GeV.c^{-1}] ", 100,  0., 800., 100, 0., 800.);
      m_hVector_correlationElectron1Pt[j].push_back(histo_correlationElectron1Pt);

      char name131[300];
      sprintf(name131,"correlationElectron1Eta_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_correlationElectron1Eta = new TH2D(name131,"Correlation Leading Electron #eta; Reco::Electron #eta; Pat::Electron #eta", 50,  -5.5, 5.5, 50, -5.5, 5.5);
      m_hVector_correlationElectron1Eta[j].push_back(histo_correlationElectron1Eta);

      char name132[300];
      sprintf(name132,"correlationElectron1Phi_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_correlationElectron1Phi = new TH2D(name132,"Correlation Leading Electron #phi; Reco::Electron #phi; Pat::Electron #phi", 50,  -3.3, 3.3, 50, -3.3, 3.3);
      m_hVector_correlationElectron1Phi[j].push_back(histo_correlationElectron1Phi);

      char name133[300];
      sprintf(name133,"correlationElectron1Iso_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_correlationElectron1Iso = new TH2D(name133,"Correlation Leading Electron Isolation; Reco::Electron Iso; Pat::Electron Iso", 100,  0., 1., 100, 0., 1.);
      m_hVector_correlationElectron1Iso[j].push_back(histo_correlationElectron1Iso);

      char name134[300];
      sprintf(name134,"correlationElectron2Pt_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_correlationElectron2Pt = new TH2D(name134,"Correlation Second Electron pT; Reco::Electron P_{T} [GeV.c^{-1}]; Pat::Electron P_{T} [GeV.c^{-1}]", 100,  0., 800., 100, 0., 800.);
      m_hVector_correlationElectron2Pt[j].push_back(histo_correlationElectron2Pt);

      char name135[300];
      sprintf(name135,"correlationElectron2Eta_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_correlationElectron2Eta = new TH2D(name135,"Correlation Second Electron #eta; Reco::Electron #eta; Pat::Electron #eta", 50,  -5.5, 5.5, 50, -5.5, 5.5);
      m_hVector_correlationElectron2Eta[j].push_back(histo_correlationElectron2Eta);

      char name136[300];
      sprintf(name136,"correlationElectron2Phi_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_correlationElectron2Phi = new TH2D(name136,"Correlation Second Electron #phi; Reco::Electron #phi; Pat::Electron #phi", 50,  -3.3, 3.3, 50, -3.3, 3.3);
      m_hVector_correlationElectron2Phi[j].push_back(histo_correlationElectron2Phi);

      char name137[300];
      sprintf(name137,"correlationElectron2Iso_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_correlationElectron2Iso = new TH2D(name137,"Correlation Second Electron Isolation; Reco::Electron Iso; Pat::Electron Iso", 100,  0., 1., 100, 0., 1.);
      m_hVector_correlationElectron2Iso[j].push_back(histo_correlationElectron2Iso);

      char name138[300];
      sprintf(name138,"correlationMuon1Pt_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_correlationMuon1Pt = new TH2D(name138,"Correlation Leading Muon pT; Reco::Muon P_{T} [GeV.c^{-1}] ; Pat::Muon P_{T} [GeV.c^{-1}]", 100,  0., 800., 100, 0., 800.);
      m_hVector_correlationMuon1Pt[j].push_back(histo_correlationMuon1Pt);

      char name139[300];
      sprintf(name139,"correlationMuon1Eta_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_correlationMuon1Eta = new TH2D(name139,"Correlation Leading Muon #eta; Reco::Muon #eta; Pat::Muon #eta", 50,  -5.5, 5.5, 50, -5.5, 5.5);
      m_hVector_correlationMuon1Eta[j].push_back(histo_correlationMuon1Eta);

      char name140[300];
      sprintf(name140,"correlationMuon1Phi_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_correlationMuon1Phi = new TH2D(name140,"Correlation Leading Muon #phi; Reco::Muon #phi; Pat::Muon #phi", 50,  -3.3, 3.3, 50, -3.3, 3.3);
      m_hVector_correlationMuon1Phi[j].push_back(histo_correlationMuon1Phi);

      char name141[300];
      sprintf(name141,"correlationMuon1Iso_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_correlationMuon1Iso = new TH2D(name141,"Correlation Leading Muon Isolation; Reco::Muon Iso; Pat::Muon Iso", 100,  0., 1., 100, 0., 1.);
      m_hVector_correlationMuon1Iso[j].push_back(histo_correlationMuon1Iso);

      char name142[300];
      sprintf(name142,"correlationMuon2Pt_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_correlationMuon2Pt = new TH2D(name142,"Correlation Second Muon pT; Reco::Muon P_{T} [GeV.c^{-1}] ; Pat::Muon P_{T} [GeV.c^{-1}]", 100,  0., 800., 100, 0., 800.);
      m_hVector_correlationMuon2Pt[j].push_back(histo_correlationMuon2Pt);

      char name143[300];
      sprintf(name143,"correlationMuon2Eta_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_correlationMuon2Eta = new TH2D(name143,"Correlation Second Muon #eta; Reco::Muon #eta; Pat::Muon #eta", 50,  -5.5, 5.5, 50, -5.5, 5.5);
      m_hVector_correlationMuon2Eta[j].push_back(histo_correlationMuon2Eta);

      char name144[300];
      sprintf(name144,"correlationMuon2Phi_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_correlationMuon2Phi = new TH2D(name144,"Correlation Second Muon #phi; Reco::Muon #phi; Pat::Muon #phi", 50,  -3.3, 3.3, 50, -3.3, 3.3);
      m_hVector_correlationMuon2Phi[j].push_back(histo_correlationMuon2Phi);

      char name145[300];
      sprintf(name145,"correlationMuon2Iso_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_correlationMuon2Iso = new TH2D(name145,"Correlation Second Muon Isolation; Reco::Muon Iso; Pat::Muon Iso", 100,  0., 1., 100, 0., 1.);
      m_hVector_correlationMuon2Iso[j].push_back(histo_correlationMuon2Iso);

      char name146[300];
      sprintf(name146,"DiElectronPt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_DiElectronPt = new TH1F(name146,"Dielectron Pt Distribution; P_{T} [GeV.c^{-1}]; N events",200,0,1000);
      m_hVector_DiElectronPt[j].push_back(histo_DiElectronPt);

      char name147[300];
      sprintf(name147,"DiElectronEta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_DiElectronEta = new TH1F(name147,"Dielectron #eta Distribution; #eta; N events",50,-5.2,5.2);
      m_hVector_DiElectronEta[j].push_back(histo_DiElectronEta);

      char name148[300];
      sprintf(name148,"DiElectronPhi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_DiElectronPhi = new TH1F(name148,"Dielectron #phi Distribution; #phi [rad]; N events",60,-3.3,3.3);
      m_hVector_DiElectronPhi[j].push_back(histo_DiElectronPhi);

      char name149[300];
      sprintf(name149,"DiMuonPt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_DiMuonPt = new TH1F(name149,"DiMuon Pt Distribution; P_{T} [GeV.c^{-1}]; N events",200,0,1000);
      m_hVector_DiMuonPt[j].push_back(histo_DiMuonPt);

      char name150[300];
      sprintf(name150,"DiMuonEta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_DiMuonEta = new TH1F(name150,"DiMuon #eta Distribution; #eta; N events",50,-5.2,5.2);
      m_hVector_DiMuonEta[j].push_back(histo_DiMuonEta);

      char name151[300];
      sprintf(name151,"DiMuonPhi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_DiMuonPhi = new TH1F(name151,"DiMuon #phi Distribution; #phi [rad]; N events",60,-3.3,3.3);
      m_hVector_DiMuonPhi[j].push_back(histo_DiMuonPhi);

      char name152[300];
      sprintf(name152,"patDiMuonPt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patDiMuonPt = new TH1F(name152,"DiMuon Pt Distribution; P_{T} [GeV.c^{-1}]; N events",200,0,1000);
      m_hVector_patDiMuonPt[j].push_back(histo_patDiMuonPt);

      char name153[300];
      sprintf(name153,"patDiMuonEta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patDiMuonEta = new TH1F(name153,"DiMuon #eta Distribution; #eta; N events",50,-5.2,5.2);
      m_hVector_patDiMuonEta[j].push_back(histo_patDiMuonEta);

      char name154[300];
      sprintf(name154,"patDiMuonPhi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patDiMuonPhi = new TH1F(name154,"DiMuon #phi Distribution; #phi [rad]; N events",60,-3.3,3.3);
      m_hVector_patDiMuonPhi[j].push_back(histo_patDiMuonPhi);

      char name155[300];
      sprintf(name155,"patDiElectronPt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patDiElectronPt = new TH1F(name155,"DiElectron Pt Distribution; P_{T} [GeV.c^{-1}]; N events",200,0,1000);
      m_hVector_patDiElectronPt[j].push_back(histo_patDiElectronPt);

      char name156[300];
      sprintf(name156,"patDiElectronEta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patDiElectronEta = new TH1F(name156,"DiElectron #eta Distribution; #eta; N events",50,-5.2,5.2);
      m_hVector_patDiElectronEta[j].push_back(histo_patDiElectronEta);

      char name157[300];
      sprintf(name157,"patDiElectronPhi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patDiElectronPhi = new TH1F(name157,"DiElectron #phi Distribution; #phi [rad]; N events",60,-3.3,3.3);
      m_hVector_patDiElectronPhi[j].push_back(histo_patDiElectronPhi);

      char name158[300];
      sprintf(name158,"ECaloVsEta_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_ECaloVsEta = new TH2D(name158,"Calorimeter Energy X #eta; #eta; Energy [GeV]", 500,  -6, 6, 100, 0., 1000.);
      m_hVector_ECaloVsEta[j].push_back(histo_ECaloVsEta);

      char name159[300];
      sprintf(name159,"sumECastorMinus_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_sumECastorMinus = new TH1F(name159,"Castor Sum of Energy; Energy [GeV]; N events",6000,0,3000);
      m_hVector_sumECastorMinus[j].push_back(histo_sumECastorMinus);

      char name160[300];
      sprintf(name160,"ECastorSector_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_ECastorSector = new TH2D(name160,"Castor Energy X Sector; Sector; Energy [GeV]", 17,  0, 17, 44, 0., 220.);
      m_hVector_ECastorSector[j].push_back(histo_ECastorSector);

      char name161[300];
      sprintf(name161,"ECaloVsEtaTProf_%s_%s",tag,Folders.at(j).c_str());
      TProfile *histo_ECaloVsEtaTProf = new TProfile(name161,"Calorimeter Energy X #eta; #eta; <Energy> [GeV]", 100,  -6, 6, 0., 1000.);
      m_hVector_ECaloVsEtaTProf[j].push_back(histo_ECaloVsEtaTProf);

      char name162[300];
      sprintf(name162,"ECastorSectorTProf_%s_%s",tag,Folders.at(j).c_str());
      TProfile *histo_ECastorSectorTProf = new TProfile(name162,"Castor Energy X Sector; Sector; <Energy> [GeV]", 100,  0, 17, 0., 7000.);
      m_hVector_ECastorSectorTProf[j].push_back(histo_ECastorSectorTProf); 

      char name163[300]; 
      sprintf(name163,"tracksOutMuonsCone03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_tracksOutMuonsCone03 = new TH1F(name163,"Tracks Outside Leading and Second Muon Cone; Number of Tracks, #DeltaR>0.3; N events",500,0,500);
      m_hVector_tracksOutMuonsCone03[j].push_back(histo_tracksOutMuonsCone03);

      char name164[300];
      sprintf(name164,"tracksOutElectronsCone03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_tracksOutElectronsCone03 = new TH1F(name164,"Tracks Outside Leading and Second Electron Cone; Number of Tracks, #DeltaR>0.3; N events",500,0,500);
      m_hVector_tracksOutElectronsCone03[j].push_back(histo_tracksOutElectronsCone03);

      char name165[300];
      sprintf(name165,"tracksOutpatMuonsCone03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_tracksOutpatMuonsCone03 = new TH1F(name165,"Tracks Outside Leading and Second Muon Cone; Number of Tracks, #DeltaR>0.3; N events",500,0,500);
      m_hVector_tracksOutpatMuonsCone03[j].push_back(histo_tracksOutpatMuonsCone03);

      char name166[300];
      sprintf(name166,"tracksOutpatElectronsCone03_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_tracksOutpatElectronsCone03 = new TH1F(name166,"Tracks Outside Leading and Second Electron Cone; Number of Tracks, #DeltaR>0.3; N events",500,0,500);
      m_hVector_tracksOutpatElectronsCone03[j].push_back(histo_tracksOutpatElectronsCone03);

      char name167[300]; 
      sprintf(name167,"tracksOutMuonsCone04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_tracksOutMuonsCone04 = new TH1F(name167,"Tracks Outside Leading and Second Muon Cone; Number of Tracks, #DeltaR>0.4; N events",500,0,500);
      m_hVector_tracksOutMuonsCone04[j].push_back(histo_tracksOutMuonsCone04);

      char name168[300];
      sprintf(name168,"tracksOutElectronsCone04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_tracksOutElectronsCone04 = new TH1F(name168,"Tracks Outside Leading and Second Electron Cone; Number of Tracks, #DeltaR>0.4; N events",500,0,500);
      m_hVector_tracksOutElectronsCone04[j].push_back(histo_tracksOutElectronsCone04);

      char name169[300];
      sprintf(name169,"tracksOutpatMuonsCone04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_tracksOutpatMuonsCone04 = new TH1F(name169,"Tracks Outside Leading and Second Muon Cone; Number of Tracks, #DeltaR>0.4; N events",500,0,500);
      m_hVector_tracksOutpatMuonsCone04[j].push_back(histo_tracksOutpatMuonsCone04);

      char name170[300];
      sprintf(name170,"tracksOutpatElectronsCone04_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_tracksOutpatElectronsCone04 = new TH1F(name170,"Tracks Outside Leading and Second Electron Cone; Number of Tracks, #DeltaR>0.4; N events",500,0,500);
      m_hVector_tracksOutpatElectronsCone04[j].push_back(histo_tracksOutpatElectronsCone04);

      char name171[300]; 
      sprintf(name171,"tracksOutMuonsCone05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_tracksOutMuonsCone05 = new TH1F(name171,"Tracks Outside Leading and Second Muon Cone; Number of Tracks, #DeltaR>0.5; N events",500,0,500);
      m_hVector_tracksOutMuonsCone05[j].push_back(histo_tracksOutMuonsCone05);

      char name172[300];
      sprintf(name172,"tracksOutElectronsCone05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_tracksOutElectronsCone05 = new TH1F(name172,"Tracks Outside Leading and Second Electron Cone; Number of Tracks, #DeltaR>0.5; N events",500,0,500);
      m_hVector_tracksOutElectronsCone05[j].push_back(histo_tracksOutElectronsCone05);

      char name173[300];
      sprintf(name173,"tracksOutpatMuonsCone05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_tracksOutpatMuonsCone05 = new TH1F(name173,"Tracks Outside Leading and Second Muon Cone; Number of Tracks, #DeltaR>0.5; N events",500,0,500);
      m_hVector_tracksOutpatMuonsCone05[j].push_back(histo_tracksOutpatMuonsCone05);

      char name174[300];
      sprintf(name174,"tracksOutpatElectronsCone05_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_tracksOutpatElectronsCone05 = new TH1F(name174,"Tracks Outside Leading and Second Electron Cone; Number of Tracks, #DeltaR>0.5; N events",500,0,500);
      m_hVector_tracksOutpatElectronsCone05[j].push_back(histo_tracksOutpatElectronsCone05);

      char name175[300];
      sprintf(name175,"LeadingElectronInnerHits_%s_%s",tag,Folders.at(j).c_str());
      TH1I *histo_LeadingElectronInnerHits = new TH1I(name175,"Leading Electron; Number Of Expected Inner Hits; N events",500,0,500);
      m_hVector_LeadingElectronInnerHits[j].push_back(histo_LeadingElectronInnerHits);

      char name176[300];
      sprintf(name176,"LeadingElectronDCot_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronDCot = new TH1F(name176,"Leading Electron; DCot [a.u.]; N events",500,0,500);
      m_hVector_LeadingElectronDCot[j].push_back(histo_LeadingElectronDCot);

      char name177[300];
      sprintf(name177,"LeadingElectronDist_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronDist = new TH1F(name177,"Leading Electron; Dist [a.u.]; N events",500,0,500);
      m_hVector_LeadingElectronDist[j].push_back(histo_LeadingElectronDist);

      char name178[300];
      sprintf(name178,"LeadingElectronDeltaEtaTkClu_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronDeltaEtaTkClu = new TH1F(name178,"Leading Electron; #Delta#eta_{TK, Cluster} [a.u.]; N events",500,0,500);
      m_hVector_LeadingElectronDeltaEtaTkClu[j].push_back(histo_LeadingElectronDeltaEtaTkClu);

      char name179[300];
      sprintf(name179,"LeadingElectronDeltaPhiTkClu_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronDeltaPhiTkClu = new TH1F(name179,"Leading Electron; #Delta#phi_{TK, Cluster} [a.u.]; N events",500,0,500);
      m_hVector_LeadingElectronDeltaPhiTkClu[j].push_back(histo_LeadingElectronDeltaPhiTkClu);

      char name180[300];
      sprintf(name180,"LeadingElectronSigmaIeIe_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronSigmaIeIe = new TH1F(name180,"Leading Electron; #sigma_{i#etai#eta} [a.u.]; N events",500,0,500);
      m_hVector_LeadingElectronSigmaIeIe[j].push_back(histo_LeadingElectronSigmaIeIe);

      char name181[300];
      sprintf(name181,"LeadingElectronHE_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_LeadingElectronHE = new TH1F(name181,"Leading Electron; HE; N events",500,0,500);
      m_hVector_LeadingElectronHE[j].push_back(histo_LeadingElectronHE);

      char name182[300];
      sprintf(name182,"SecondElectronInnerHits_%s_%s",tag,Folders.at(j).c_str());
      TH1I *histo_SecondElectronInnerHits = new TH1I(name182,"Second Electron; Number Of Expected Inner Hits; N events",500,0,500);
      m_hVector_SecondElectronInnerHits[j].push_back(histo_SecondElectronInnerHits);

      char name183[300];
      sprintf(name183,"SecondElectronDCot_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronDCot = new TH1F(name183,"Second Electron; DCot [a.u.]; N events",500,0,500);
      m_hVector_SecondElectronDCot[j].push_back(histo_SecondElectronDCot);

      char name184[300];
      sprintf(name184,"SecondElectronDist_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronDist = new TH1F(name184,"Second Electron; Dist [a.u.]; N events",500,0,500);
      m_hVector_SecondElectronDist[j].push_back(histo_SecondElectronDist);

      char name185[300];
      sprintf(name185,"SecondElectronDeltaEtaTkClu_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronDeltaEtaTkClu = new TH1F(name185,"Second Electron; #Delta#eta_{TK, Cluster} [a.u.]; N events",500,0,500);
      m_hVector_SecondElectronDeltaEtaTkClu[j].push_back(histo_SecondElectronDeltaEtaTkClu);

      char name186[300];
      sprintf(name186,"SecondElectronDeltaPhiTkClu_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronDeltaPhiTkClu = new TH1F(name186,"Second Electron; #Delta#phi_{TK, Cluster} [a.u.]; N events",500,0,500);
      m_hVector_SecondElectronDeltaPhiTkClu[j].push_back(histo_SecondElectronDeltaPhiTkClu);

      char name187[300];
      sprintf(name187,"SecondElectronSigmaIeIe_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronSigmaIeIe = new TH1F(name187,"Second Electron; #sigma_{i#etai#eta} [a.u.]; N events",500,0,500);
      m_hVector_SecondElectronSigmaIeIe[j].push_back(histo_SecondElectronSigmaIeIe);

      char name188[300];
      sprintf(name188,"SecondElectronHE_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SecondElectronHE = new TH1F(name188,"Second Electron; HE; N events",500,0,500);
      m_hVector_SecondElectronHE[j].push_back(histo_SecondElectronHE);

      char name189[300];
      sprintf(name189,"sumEHFplus_S_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SumEHFplus_S = new TH1F(name189,"HF^{+} - Sum of Energy, Short Fibers; #sum E_{HF^{+},Short} [GeV]; N events",2000,0,2000);
      m_hVector_SumEHFplus_S[j].push_back(histo_SumEHFplus_S);

      char name190[300];
      sprintf(name190,"sumEHFminus_S_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SumEHFminus_S = new TH1F(name190,"HF^{-} - Sum of Energy, Short Fibers; #sum E_{HF^{-},Short} [GeV]; N events",2000,0,2000);
      m_hVector_SumEHFminus_S[j].push_back(histo_SumEHFminus_S);

      char name191[300];
      sprintf(name191,"sumEHFplus_L_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SumEHFplus_L = new TH1F(name191,"HF^{+} - Sum of Energy, Long Fibers; #sum E_{HF^{+},Long} [GeV]; N events",2000,0,2000);
      m_hVector_SumEHFplus_L[j].push_back(histo_SumEHFplus_L);

      char name192[300];
      sprintf(name192,"sumEHFminus_L_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SumEHFminus_L = new TH1F(name192,"HF^{-} - Sum of Energy, Long Fibers; #sum E_{HF^{-},Long} [GeV]; N events",2000,0,2000);
      m_hVector_SumEHFminus_L[j].push_back(histo_SumEHFminus_L);

      char name193[300];
      sprintf(name193,"maxetagap_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_maxetagap = new TH1F(name193,"Particle Flow #eta_{max} Gap Distribution; #eta_{max, gap}; N events",17,binarrayminus);
      m_hVector_maxetagap[j].push_back(histo_maxetagap);

      char name194[300];
      sprintf(name194,"ElectronsPt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_ElectronsPt = new TH1F(name194,"Electrons - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",500,0,500);
      m_hVector_ElectronsPt[j].push_back(histo_ElectronsPt);

      char name195[300];
      sprintf(name195,"ElectronsEta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_ElectronsEta = new TH1F(name195,"Electrons - #eta Distribution; #eta; N events",60,-6,6);
      m_hVector_ElectronsEta[j].push_back(histo_ElectronsEta);

      char name196[300];
      sprintf(name196,"ElectronsPhi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_ElectronsPhi = new TH1F(name196,"Electrons - #phi Distribution; #phi [rad]; N events",16,-3.2,3.2);
      m_hVector_ElectronsPhi[j].push_back(histo_ElectronsPhi);

      char name197[300];
      sprintf(name197,"MuonsPt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_MuonsPt = new TH1F(name197,"Muons - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",500,0,500);
      m_hVector_MuonsPt[j].push_back(histo_MuonsPt);

      char name198[300];
      sprintf(name198,"MuonsEta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_MuonsEta = new TH1F(name198,"Muons - #eta Distribution; #eta; N events",60,-6,6);
      m_hVector_MuonsEta[j].push_back(histo_MuonsEta);

      char name199[300];
      sprintf(name199,"MuonsPhi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_MuonsPhi = new TH1F(name199,"Muons - #phi Distribution; #phi [rad]; N events",16,-3.2,3.2);
      m_hVector_MuonsPhi[j].push_back(histo_MuonsPhi);

      char name200[300];
      sprintf(name200,"patElectronsPt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectronsPt = new TH1F(name200,"patElectrons - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",500,0,500);
      m_hVector_patElectronsPt[j].push_back(histo_patElectronsPt);

      char name201[300];
      sprintf(name201,"patElectronsEta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectronsEta = new TH1F(name201,"patElectrons - #eta Distribution; #eta; N events",60,-6,6);
      m_hVector_patElectronsEta[j].push_back(histo_patElectronsEta);

      char name202[300];
      sprintf(name202,"patElectronsPhi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patElectronsPhi = new TH1F(name202,"patElectrons - #phi Distribution; #phi [rad]; N events",16,-3.2,3.2);
      m_hVector_patElectronsPhi[j].push_back(histo_patElectronsPhi);

      char name203[300];
      sprintf(name203,"patMuonsPt_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuonsPt = new TH1F(name203,"patMuons - P_{T} Distribution; P_{T} [GeV.c^{-1}]; N events",500,0,500);
      m_hVector_patMuonsPt[j].push_back(histo_patMuonsPt);

      char name204[300];
      sprintf(name204,"patMuonsEta_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuonsEta = new TH1F(name204,"patMuons - #eta Distribution; #eta; N events",60,-6,6);
      m_hVector_patMuonsEta[j].push_back(histo_patMuonsEta);

      char name205[300];
      sprintf(name205,"patMuonsPhi_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_patMuonsPhi = new TH1F(name205,"patMuons - #phi Distribution; #phi [rad]; N events",16,-3.2,3.2);
      m_hVector_patMuonsPhi[j].push_back(histo_patMuonsPhi);

      char name206[300];
      sprintf(name206,"sumEHFMax_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SumEHFMax = new TH1F(name206,"HF - Sum of Energy; #sum E_{HF,Max} [GeV]; N events",2000,0,2000);
      m_hVector_SumEHFMax[j].push_back(histo_SumEHFMax);

      char name207[300];
      sprintf(name207,"sumEHFMin_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_SumEHFMin = new TH1F(name207,"HF - Sum of Energy; #sum E_{HF,Min} [GeV]; N events",2000,0,2000);
      m_hVector_SumEHFMin[j].push_back(histo_SumEHFMin);

      char name208[300];
      sprintf(name208,"EnergyVsEtaBin1D_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_EnergyVsEtaBin1D = new TH1F(name208,"Calorimeter Energy X #eta; #eta; #sum Energy_{calotower} [GeV]", 500,  -6, 6);
      m_hVector_EnergyVsEtaBin1D[j].push_back(histo_EnergyVsEtaBin1D);

      char name209[300];
      sprintf(name209,"deltaEtamaxminPF_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_deltaEtaPF = new TH1F(name209,"#Delta#eta_{PF} Distribution; #eta_{max}-#eta_{min}; N events",14,binarraydelta);
      m_hVector_deltaEtaPF[j].push_back(histo_deltaEtaPF);

      char name210[300];
      sprintf(name210,"absdeltaEtamaxminPF_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_absdeltaEtaPF = new TH1F(name210,"|#Delta#eta_{PF}| Distribution; |#eta_{max}-#eta_{min}|; N events",14,binarraydelta);
      m_hVector_absdeltaEtaPF[j].push_back(histo_absdeltaEtaPF);

      char name211[300];
      sprintf(name211,"ECastorSectorBin1D_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_ECastorSectorBin1D = new TH1F(name211,"Castor Energy X Sector; Sector; Energy [GeV]", 17,  0, 17);
      m_hVector_ECastorSectorBin1D[j].push_back(histo_ECastorSectorBin1D);

      char name212[300];
      sprintf(name212,"EnergyHFMinusVsCastor_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_ET_Calos_n = new TH2D(name212,"HF^{-} and Castor; #sum Energy HF^{-}; #sum Energy Castor [GeV]; N events", 1000,  0., 1000., 6000,  0., 3000. );
      m_hVector_etcalos_n[j].push_back(histo_ET_Calos_n);

      char name213[300];
      sprintf(name213,"EnergyHFPlusVsCastorTProf_%s_%s",tag,Folders.at(j).c_str());
      TProfile *histo_EnergyHFPlusVsCastorTProf = new TProfile(name213,"HF^{+} and Castor; #sum Energy HF^{+}; #sum Energy Castor [GeV]; N events", 1000,  0., 1000., 0., 3000. );
      m_hVector_EnergyHFPlusVsCastorTProf[j].push_back(histo_EnergyHFPlusVsCastorTProf);

      char name214[300];
      sprintf(name214,"EnergyHFMinusVsCastorTProf_%s_%s",tag,Folders.at(j).c_str());
      TProfile *histo_EnergyHFMinusVsCastorTProf = new TProfile(name214,"HF^{-} and Castor; #sum Energy HF^{-}; #sum Energy Castor [GeV]; N events", 1000,  0., 1000., 0., 3000. );
      m_hVector_EnergyHFMinusVsCastorTProf[j].push_back(histo_EnergyHFMinusVsCastorTProf);

      char name215[300];
      sprintf(name215,"sumECastorAndSumHFMinus_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_sumECastorAndHFMinus = new TH1F(name215,"HF^{-} and Castor Sum of Energy; Energy [GeV]; N events",6000,0,3000);
      m_hVector_sumECastorAndHFMinus[j].push_back(histo_sumECastorAndHFMinus);

      char name216[300];
      sprintf(name216,"CastorMultiplicity_%s_%s",tag,Folders.at(j).c_str());
      TH1I *histo_CastorMultiplicity = new TH1I(name216,"Castor: number of sectors with activity; #Sectors; N events",16,0,16);
      m_hVector_CastorMultiplicity[j].push_back(histo_CastorMultiplicity);

      char name217[300];
      sprintf(name217,"CastorMultiplicityVsLumi_%s_%s",tag,Folders.at(j).c_str());
      TH2D *histo_CastorMultiplicityVsLumi = new TH2D(name217,"CastorMultiplicity Vs Luminosity; Luminosity per Bunch [#mub^{-1}s^{-1}]; Castor Multiplicity",5000,0,2,16,0,16);
      m_hVector_CastorMultiplicityVsLumi[j].push_back(histo_CastorMultiplicityVsLumi);

      char name218[300];
      sprintf(name218,"RunNumber_%s_%s",tag,Folders.at(j).c_str());
      TH1I *histo_RunNumber = new TH1I(name218,"Run Number; Run Number; N Event",16000,134000,150000);
      m_hVector_RunNumber[j].push_back(histo_RunNumber);

      char name219[300];
      sprintf(name219,"RunNumberZeroCastor_%s_%s",tag,Folders.at(j).c_str());
      TH1I *histo_RunNumberZeroCastor = new TH1I(name219,"Run Number; Run Number; N Event",16000,134000,150000);
      m_hVector_RunNumberZeroCastor[j].push_back(histo_RunNumberZeroCastor);

      char name220[300];
      sprintf(name220,"RunNumberHighCastor_%s_%s",tag,Folders.at(j).c_str());
      TH1I *histo_RunNumberHighCastor = new TH1I(name220,"Run Number; Run Number; N Event",16000,134000,150000);
      m_hVector_RunNumberHighCastor[j].push_back(histo_RunNumberHighCastor);

      // Castor Sector Plots
      char name_sector1[300];
      sprintf(name_sector1,"TotalEnergyCastor_sector1_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_TotalEnergyCastor_sector1 = new TH1F(name_sector1,"#sum Energy, Castor Sector 1; #sum E_{modules 1,2,3,4,5} [GeV]; N events",1000,0,500);
      m_hVector_TotalEnergyCastor_sector1[j].push_back(histo_TotalEnergyCastor_sector1);

      char name_sector2[300];
      sprintf(name_sector2,"TotalEnergyCastor_sector2_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_TotalEnergyCastor_sector2 = new TH1F(name_sector2,"#sum Energy, Castor Sector 2; #sum E_{modules 1,2,3,4,5} [GeV]; N events",1000,0,500);
      m_hVector_TotalEnergyCastor_sector2[j].push_back(histo_TotalEnergyCastor_sector2);

      char name_sector3[300];
      sprintf(name_sector3,"TotalEnergyCastor_sector3_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_TotalEnergyCastor_sector3 = new TH1F(name_sector3,"#sum Energy, Castor Sector 3; #sum E_{modules 1,2,3,4,5} [GeV]; N events",1000,0,500);
      m_hVector_TotalEnergyCastor_sector3[j].push_back(histo_TotalEnergyCastor_sector3);

      char name_sector4[300];
      sprintf(name_sector4,"TotalEnergyCastor_sector4_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_TotalEnergyCastor_sector4 = new TH1F(name_sector4,"#sum Energy, Castor Sector 4; #sum E_{modules 1,2,3,4,5} [GeV]; N events",1000,0,500);
      m_hVector_TotalEnergyCastor_sector4[j].push_back(histo_TotalEnergyCastor_sector4);

      char name_sector5[300];
      sprintf(name_sector5,"TotalEnergyCastor_sector5_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_TotalEnergyCastor_sector5 = new TH1F(name_sector5,"#sum Energy, Castor Sector 5; #sum E_{modules 1,2,3,4,5} [GeV]; N events",1000,0,500);
      m_hVector_TotalEnergyCastor_sector5[j].push_back(histo_TotalEnergyCastor_sector5);

      char name_sector6[300];
      sprintf(name_sector6,"TotalEnergyCastor_sector6_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_TotalEnergyCastor_sector6 = new TH1F(name_sector6,"#sum Energy, Castor Sector 6; #sum E_{modules 1,2,3,4,5} [GeV]; N events",1000,0,500);
      m_hVector_TotalEnergyCastor_sector6[j].push_back(histo_TotalEnergyCastor_sector6);

      char name_sector7[300];
      sprintf(name_sector7,"TotalEnergyCastor_sector7_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_TotalEnergyCastor_sector7 = new TH1F(name_sector7,"#sum Energy, Castor Sector 7; #sum E_{modules 1,2,3,4,5} [GeV]; N events",1000,0,500);
      m_hVector_TotalEnergyCastor_sector7[j].push_back(histo_TotalEnergyCastor_sector7);

      char name_sector8[300];
      sprintf(name_sector8,"TotalEnergyCastor_sector8_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_TotalEnergyCastor_sector8 = new TH1F(name_sector8,"#sum Energy, Castor Sector 8; #sum E_{modules 1,2,3,4,5} [GeV]; N events",1000,0,500);
      m_hVector_TotalEnergyCastor_sector8[j].push_back(histo_TotalEnergyCastor_sector8);

      char name_sector9[300];
      sprintf(name_sector9,"TotalEnergyCastor_sector9_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_TotalEnergyCastor_sector9 = new TH1F(name_sector9,"#sum Energy, Castor Sector 9; #sum E_{modules 1,2,3,4,5} [GeV]; N events",1000,0,500);
      m_hVector_TotalEnergyCastor_sector9[j].push_back(histo_TotalEnergyCastor_sector9);

      char name_sector10[300];
      sprintf(name_sector10,"TotalEnergyCastor_sector10_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_TotalEnergyCastor_sector10 = new TH1F(name_sector10,"#sum Energy, Castor Sector 10; #sum E_{modules 1,2,3,4,5} [GeV]; N events",1000,0,500);
      m_hVector_TotalEnergyCastor_sector10[j].push_back(histo_TotalEnergyCastor_sector10);

      char name_sector11[300];
      sprintf(name_sector11,"TotalEnergyCastor_sector11_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_TotalEnergyCastor_sector11 = new TH1F(name_sector11,"#sum Energy, Castor Sector 11; #sum E_{modules 1,2,3,4,5} [GeV]; N events",1000,0,500);
      m_hVector_TotalEnergyCastor_sector11[j].push_back(histo_TotalEnergyCastor_sector11);

      char name_sector12[300];
      sprintf(name_sector12,"TotalEnergyCastor_sector12_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_TotalEnergyCastor_sector12 = new TH1F(name_sector12,"#sum Energy, Castor Sector 12; #sum E_{modules 1,2,3,4,5} [GeV]; N events",1000,0,500);
      m_hVector_TotalEnergyCastor_sector12[j].push_back(histo_TotalEnergyCastor_sector12);

      char name_sector13[300];
      sprintf(name_sector13,"TotalEnergyCastor_sector13_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_TotalEnergyCastor_sector13 = new TH1F(name_sector13,"#sum Energy, Castor Sector 13; #sum E_{modules 1,2,3,4,5} [GeV]; N events",1000,0,500);
      m_hVector_TotalEnergyCastor_sector13[j].push_back(histo_TotalEnergyCastor_sector13);

      char name_sector14[300];
      sprintf(name_sector14,"TotalEnergyCastor_sector14_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_TotalEnergyCastor_sector14 = new TH1F(name_sector14,"#sum Energy, Castor Sector 14; #sum E_{modules 1,2,3,4,5} [GeV]; N events",1000,0,500);
      m_hVector_TotalEnergyCastor_sector14[j].push_back(histo_TotalEnergyCastor_sector14);

      char name_sector15[300];
      sprintf(name_sector15,"TotalEnergyCastor_sector15_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_TotalEnergyCastor_sector15 = new TH1F(name_sector15,"#sum Energy, Castor Sector 15; #sum E_{modules 1,2,3,4,5} [GeV]; N events",1000,0,500);
      m_hVector_TotalEnergyCastor_sector15[j].push_back(histo_TotalEnergyCastor_sector15);

      char name_sector16[300];
      sprintf(name_sector16,"TotalEnergyCastor_sector16_%s_%s",tag,Folders.at(j).c_str());
      TH1F *histo_TotalEnergyCastor_sector16 = new TH1F(name_sector16,"#sum Energy, Castor Sector 16; #sum E_{modules 1,2,3,4,5} [GeV]; N events",1000,0,500);
      m_hVector_TotalEnergyCastor_sector16[j].push_back(histo_TotalEnergyCastor_sector16);

    }
  }
}

void DiffractiveZ::FillHistos(int index, int pileup, double totalweight){
  m_hVector_DiElectronMass[index].at(pileup)->Fill(eventdiffZ->GetDiElectronMass(),totalweight);
  m_hVector_DiElectronEta[index].at(pileup)->Fill(eventdiffZ->GetDiElectronEta(),totalweight);
  m_hVector_DiElectronPhi[index].at(pileup)->Fill(eventdiffZ->GetDiElectronPhi(),totalweight);
  m_hVector_DiElectronPt[index].at(pileup)->Fill(eventdiffZ->GetDiElectronPt(),totalweight);
  m_hVector_LeadingElectronPt[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronPt(),totalweight);
  m_hVector_LeadingElectronEta[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronEta(),totalweight);
  m_hVector_LeadingElectronPhi[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronPhi(),totalweight);
  m_hVector_LeadingElectronCharge[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronCharge(),totalweight);
  m_hVector_SecondElectronPt[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronPt(),totalweight);
  m_hVector_SecondElectronEta[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronEta(),totalweight);
  m_hVector_SecondElectronPhi[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronPhi(),totalweight);
  m_hVector_SecondElectronCharge[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronCharge(),totalweight);
  m_hVector_ElectronsN[index].at(pileup)->Fill(eventdiffZ->GetElectronsN(),totalweight);
  m_hVector_DiMuonMass[index].at(pileup)->Fill(eventdiffZ->GetDiMuonMass(),totalweight);
  m_hVector_DiMuonEta[index].at(pileup)->Fill(eventdiffZ->GetDiMuonEta(),totalweight);
  m_hVector_DiMuonPhi[index].at(pileup)->Fill(eventdiffZ->GetDiMuonPhi(),totalweight);
  m_hVector_DiMuonPt[index].at(pileup)->Fill(eventdiffZ->GetDiMuonPt(),totalweight);
  m_hVector_LeadingMuonPt[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonPt(),totalweight);
  m_hVector_LeadingMuonEta[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonEta(),totalweight);
  m_hVector_LeadingMuonPhi[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonPhi(),totalweight);
  m_hVector_LeadingMuonCharge[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonCharge(),totalweight);
  m_hVector_SecondMuonPt[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonPt(),totalweight);
  m_hVector_SecondMuonEta[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonEta(),totalweight);
  m_hVector_SecondMuonPhi[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonPhi(),totalweight);
  m_hVector_SecondMuonCharge[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonCharge(),totalweight);
  m_hVector_MuonsN[index].at(pileup)->Fill(eventdiffZ->GetMuonsN(),totalweight);
  m_hVector_sumEHFplus[index].at(pileup)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight);
  m_hVector_sumEHFminus[index].at(pileup)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight);
  m_hVector_sumEHEplus[index].at(pileup)->Fill(eventdiff->GetSumEnergyHEPlus(),totalweight);
  m_hVector_sumEHEminus[index].at(pileup)->Fill(eventdiff->GetSumEnergyHEMinus(),totalweight);
  m_hVector_lumi[index].at(pileup)->Fill(eventinfo->GetInstLumiBunch(),totalweight);
  m_hVector_asumE[index].at(pileup)->Fill(aSumE,totalweight);
  m_hVector_multhf[index].at(pileup)->Fill(eventdiff->GetMultiplicityHFPlus(),eventdiff->GetMultiplicityHFMinus(),totalweight);
  m_hVector_tracks[index].at(pileup)->Fill(eventdiff->GetMultiplicityTracks(),totalweight);
  m_hVector_pfetamax[index].at(pileup)->Fill(eventdiff->GetEtaMaxFromPFCands(),totalweight);
  m_hVector_pfetamin[index].at(pileup)->Fill(eventdiff->GetEtaMinFromPFCands(),totalweight);
  m_hVector_vertex[index].at(pileup)->Fill(eventdiff->GetNVertex(),totalweight);
  m_hVector_deltaphielectrons[index].at(pileup)->Fill(deltaphielectrons,totalweight);
  m_hVector_deltaphimuons[index].at(pileup)->Fill(deltaphimuons,totalweight);
  m_hVector_deltaetaelectrons[index].at(pileup)->Fill(deltaetaelectrons,totalweight);
  m_hVector_deltaetamuons[index].at(pileup)->Fill(deltaetamuons,totalweight);
  m_hVector_deltapTelectrons[index].at(pileup)->Fill(deltapTelectrons,totalweight);
  m_hVector_deltapTmuons[index].at(pileup)->Fill(deltapTmuons,totalweight);
  m_hVector_vertexvslumi[index].at(pileup)->Fill(eventdiff->GetNVertex(),eventinfo->GetInstLumiBunch(),totalweight);
  m_hVector_patNElectron[index].at(pileup)->Fill(eventdiffZ->GetPatNElectron(),totalweight);
  m_hVector_patElectron1Pt[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1Pt(),totalweight);
  m_hVector_patElectron1Eta[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1Eta(),totalweight);
  m_hVector_patElectron1Phi[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1Phi(),totalweight);
  m_hVector_patElectron1Charge[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1Charge(),totalweight);
  m_hVector_patElectron1Et[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1Et(),totalweight);
  m_hVector_patNMuon[index].at(pileup)->Fill(eventdiffZ->GetPatNMuon(),totalweight);
  m_hVector_patMuon1Pt[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1Pt(),totalweight);
  m_hVector_patMuon1Eta[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1Eta(),totalweight);
  m_hVector_patMuon1Phi[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1Phi(),totalweight);
  m_hVector_patMuon1Charge[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1Charge(),totalweight);
  m_hVector_patMuon1Et[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1Et(),totalweight);
  m_hVector_patElectron2Pt[index].at(pileup)->Fill(eventdiffZ->GetPatElectron2Pt(),totalweight);
  m_hVector_patElectron2Eta[index].at(pileup)->Fill(eventdiffZ->GetPatElectron2Eta(),totalweight);
  m_hVector_patElectron2Phi[index].at(pileup)->Fill(eventdiffZ->GetPatElectron2Phi(),totalweight);
  m_hVector_patElectron2Charge[index].at(pileup)->Fill(eventdiffZ->GetPatElectron2Charge(),totalweight);
  m_hVector_patElectron2Et[index].at(pileup)->Fill(eventdiffZ->GetPatElectron2Et(),totalweight);
  m_hVector_patMuon2Pt[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2Pt(),totalweight);
  m_hVector_patMuon2Eta[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2Eta(),totalweight);
  m_hVector_patMuon2Phi[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2Phi(),totalweight);
  m_hVector_patMuon2Charge[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2Charge(),totalweight);
  m_hVector_patMuon2Et[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2Et(),totalweight);
  m_hVector_patDiElectronMass[index].at(pileup)->Fill(eventdiffZ->GetPatDiElectronMass(),totalweight);
  m_hVector_patDiMuonMass[index].at(pileup)->Fill(eventdiffZ->GetPatDiMuonMass(),totalweight);
  m_hVector_patDiElectronEta[index].at(pileup)->Fill(eventdiffZ->GetPatDiElectronEta(),totalweight);
  m_hVector_patDiMuonEta[index].at(pileup)->Fill(eventdiffZ->GetPatDiMuonEta(),totalweight);
  m_hVector_patDiElectronPhi[index].at(pileup)->Fill(eventdiffZ->GetPatDiElectronPhi(),totalweight);
  m_hVector_patDiMuonPhi[index].at(pileup)->Fill(eventdiffZ->GetPatDiMuonPhi(),totalweight);
  m_hVector_patDiElectronPt[index].at(pileup)->Fill(eventdiffZ->GetPatDiElectronPt(),totalweight);
  m_hVector_patDiMuonPt[index].at(pileup)->Fill(eventdiffZ->GetPatDiMuonPt(),totalweight);
  m_hVector_patElectron1TkDr03[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1TkDr03(),totalweight);
  m_hVector_patElectron1TkDr04[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1TkDr04(),totalweight);
  m_hVector_patElectron1EcalDr03[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1EcalDr03(),totalweight);
  m_hVector_patElectron1EcalDr04[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1EcalDr04(),totalweight);
  m_hVector_patElectron1HcalDr03[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1HcalDr03(),totalweight);
  m_hVector_patElectron1HcalDr04[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1HcalDr04(),totalweight);
  m_hVector_patElectron2TkDr03[index].at(pileup)->Fill(eventdiffZ->GetPatElectron2TkDr03(),totalweight);
  m_hVector_patElectron2TkDr04[index].at(pileup)->Fill(eventdiffZ->GetPatElectron2TkDr04(),totalweight);
  m_hVector_patElectron2EcalDr03[index].at(pileup)->Fill(eventdiffZ->GetPatElectron2EcalDr03(),totalweight);
  m_hVector_patElectron2EcalDr04[index].at(pileup)->Fill(eventdiffZ->GetPatElectron2EcalDr04(),totalweight);
  m_hVector_patElectron2HcalDr03[index].at(pileup)->Fill(eventdiffZ->GetPatElectron2HcalDr03(),totalweight);
  m_hVector_patElectron2HcalDr04[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1HcalDr04(),totalweight);
  m_hVector_patElectron1relIsoDr03[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1relIsoDr03(),totalweight);
  m_hVector_patElectron1relIsoDr04[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1relIsoDr04(),totalweight);
  m_hVector_patElectron2relIsoDr03[index].at(pileup)->Fill(eventdiffZ->GetPatElectron2relIsoDr03(),totalweight);
  m_hVector_patElectron2relIsoDr04[index].at(pileup)->Fill(eventdiffZ->GetPatElectron2relIsoDr04(),totalweight);
  m_hVector_patMuon1SumPtR03[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1SumPtR03(),totalweight);
  m_hVector_patMuon1SumPtR05[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1SumPtR05(),totalweight);
  m_hVector_patMuon1EmEtR03[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1EmEtR03(),totalweight);
  m_hVector_patMuon1EmEtR05[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1EmEtR05(),totalweight);
  m_hVector_patMuon1HadEtR03[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1HadEtR03(),totalweight);
  m_hVector_patMuon1HadEtR05[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1HadEtR05(),totalweight);
  m_hVector_patMuon2SumPtR03[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2SumPtR03(),totalweight);
  m_hVector_patMuon2SumPtR05[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2SumPtR05(),totalweight);
  m_hVector_patMuon2EmEtR03[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2EmEtR03(),totalweight);
  m_hVector_patMuon2EmEtR05[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2EmEtR05(),totalweight);
  m_hVector_patMuon2HadEtR03[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2HadEtR03(),totalweight);
  m_hVector_patMuon2HadEtR05[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2HadEtR05(),totalweight);
  m_hVector_patMuon1relIsoDr03[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1relIsoDr03(),totalweight);
  m_hVector_patMuon1relIsoDr05[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1relIsoDr05(),totalweight);
  m_hVector_patMuon2relIsoDr03[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2relIsoDr03(),totalweight);
  m_hVector_patMuon2relIsoDr05[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2relIsoDr05(),totalweight);
  m_hVector_patMuon1relIso[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1relIso(),totalweight);
  m_hVector_patMuon2relIso[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2relIso(),totalweight);
  m_hVector_LeadingElectronTkDr03[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronTkDr03(),totalweight);
  m_hVector_LeadingElectronTkDr04[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronTkDr04(),totalweight);
  m_hVector_LeadingElectronEcalDr03[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronEcalDr03(),totalweight);
  m_hVector_LeadingElectronEcalDr04[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronEcalDr04(),totalweight);
  m_hVector_LeadingElectronHcalDr03[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronHcalDr03(),totalweight);
  m_hVector_LeadingElectronHcalDr04[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronHcalDr04(),totalweight);
  m_hVector_SecondElectronTkDr03[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronTkDr03(),totalweight);
  m_hVector_SecondElectronTkDr04[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronTkDr04(),totalweight);
  m_hVector_SecondElectronEcalDr03[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronEcalDr03(),totalweight);
  m_hVector_SecondElectronEcalDr04[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronEcalDr04(),totalweight);
  m_hVector_SecondElectronHcalDr03[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronHcalDr03(),totalweight);
  m_hVector_SecondElectronHcalDr04[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronHcalDr04(),totalweight);
  m_hVector_LeadingElectronrelIsoDr03[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronrelIsoDr03(),totalweight);
  m_hVector_LeadingElectronrelIsoDr04[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronrelIsoDr04(),totalweight);
  m_hVector_SecondElectronrelIsoDr03[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronrelIsoDr03(),totalweight);
  m_hVector_SecondElectronrelIsoDr04[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronrelIsoDr04(),totalweight);
  m_hVector_LeadingMuonSumPtR03[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonSumPtR03(),totalweight);
  m_hVector_LeadingMuonSumPtR05[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonSumPtR05(),totalweight);
  m_hVector_LeadingMuonEmEtR03[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonEmEtR03(),totalweight);
  m_hVector_LeadingMuonEmEtR05[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonEmEtR05(),totalweight);
  m_hVector_LeadingMuonHadEtR03[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonHadEtR03(),totalweight);
  m_hVector_LeadingMuonHadEtR05[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonHadEtR05(),totalweight);
  m_hVector_SecondMuonSumPtR03[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonSumPtR03(),totalweight);
  m_hVector_SecondMuonSumPtR05[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonSumPtR05(),totalweight);
  m_hVector_SecondMuonEmEtR03[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonEmEtR03(),totalweight);
  m_hVector_SecondMuonEmEtR05[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonEmEtR05(),totalweight);
  m_hVector_SecondMuonHadEtR03[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonHadEtR03(),totalweight);
  m_hVector_SecondMuonHadEtR05[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonHadEtR05(),totalweight);
  m_hVector_LeadingMuonrelIsoDr03[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonrelIsoDr03(),totalweight);
  m_hVector_LeadingMuonrelIsoDr05[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonrelIsoDr05(),totalweight);
  m_hVector_SecondMuonrelIsoDr03[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonrelIsoDr03(),totalweight);
  m_hVector_SecondMuonrelIsoDr05[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonrelIsoDr05(),totalweight);

  m_hVector_correlationElectron1Pt[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronPt(),eventdiffZ->GetPatElectron1Pt());
  m_hVector_correlationElectron1Eta[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronEta(),eventdiffZ->GetPatElectron1Eta());
  m_hVector_correlationElectron1Phi[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronPhi(),eventdiffZ->GetPatElectron1Phi());
  m_hVector_correlationElectron1Iso[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronrelIsoDr03(),eventdiffZ->GetPatElectron1relIsoDr03());
  m_hVector_correlationMuon1Pt[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonPt(),eventdiffZ->GetPatMuon1Pt());
  m_hVector_correlationMuon1Eta[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonEta(),eventdiffZ->GetPatMuon1Eta());
  m_hVector_correlationMuon1Phi[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonPhi(),eventdiffZ->GetPatMuon1Phi());
  m_hVector_correlationMuon1Iso[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonrelIsoDr03(),eventdiffZ->GetPatMuon1relIsoDr03());
  m_hVector_correlationElectron2Pt[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronPt(),eventdiffZ->GetPatElectron2Pt());
  m_hVector_correlationElectron2Eta[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronEta(),eventdiffZ->GetPatElectron2Eta());
  m_hVector_correlationElectron2Phi[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronPhi(),eventdiffZ->GetPatElectron2Phi());
  m_hVector_correlationElectron2Iso[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronrelIsoDr03(),eventdiffZ->GetPatElectron2relIsoDr03());
  m_hVector_correlationMuon2Pt[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonPt(),eventdiffZ->GetPatMuon2Pt());
  m_hVector_correlationMuon2Eta[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonEta(),eventdiffZ->GetPatMuon2Eta());
  m_hVector_correlationMuon2Phi[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonPhi(),eventdiffZ->GetPatMuon2Phi());
  m_hVector_correlationMuon2Iso[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonrelIsoDr03(),eventdiffZ->GetPatMuon2relIsoDr03());

  // Castor Energy per Sector
  if (eventdiffZ->GetCastorTowerEnergy(0) > 4.*0.364){
    m_hVector_TotalEnergyCastor_sector1[index].at(pileup)->Fill(eventdiffZ->GetCastorTowerEnergy(0),totalweight);
  }
  else{
    m_hVector_TotalEnergyCastor_sector1[index].at(pileup)->Fill(0);
  }

  if (eventdiffZ->GetCastorTowerEnergy(1) > 4.*0.364){
    m_hVector_TotalEnergyCastor_sector2[index].at(pileup)->Fill(eventdiffZ->GetCastorTowerEnergy(1),totalweight);
  }
  else{
    m_hVector_TotalEnergyCastor_sector2[index].at(pileup)->Fill(0);
  }

  if (eventdiffZ->GetCastorTowerEnergy(2) > 4.*0.364){
    m_hVector_TotalEnergyCastor_sector3[index].at(pileup)->Fill(eventdiffZ->GetCastorTowerEnergy(2),totalweight);
  } 
  else{
    m_hVector_TotalEnergyCastor_sector3[index].at(pileup)->Fill(0);
  }

  if (eventdiffZ->GetCastorTowerEnergy(3) > 4.*0.364){
    m_hVector_TotalEnergyCastor_sector4[index].at(pileup)->Fill(eventdiffZ->GetCastorTowerEnergy(3),totalweight);
  }
  else{
    m_hVector_TotalEnergyCastor_sector4[index].at(pileup)->Fill(0);
  }

  if (eventdiffZ->GetCastorTowerEnergy(4) > 4.*0.364){
    m_hVector_TotalEnergyCastor_sector5[index].at(pileup)->Fill(eventdiffZ->GetCastorTowerEnergy(4),totalweight);
  }
  else {
    m_hVector_TotalEnergyCastor_sector5[index].at(pileup)->Fill(0);
  }

  if (eventdiffZ->GetCastorTowerEnergy(5) > 4.*0.364){
    m_hVector_TotalEnergyCastor_sector6[index].at(pileup)->Fill(eventdiffZ->GetCastorTowerEnergy(5),totalweight);
  } 
  else {
    m_hVector_TotalEnergyCastor_sector6[index].at(pileup)->Fill(0);
  }

  if (eventdiffZ->GetCastorTowerEnergy(6) > 4.*0.364){
    m_hVector_TotalEnergyCastor_sector7[index].at(pileup)->Fill(eventdiffZ->GetCastorTowerEnergy(6),totalweight);
  }
  else{
    m_hVector_TotalEnergyCastor_sector7[index].at(pileup)->Fill(0);
  }

  if (eventdiffZ->GetCastorTowerEnergy(7) > 4.*0.364){
    m_hVector_TotalEnergyCastor_sector8[index].at(pileup)->Fill(eventdiffZ->GetCastorTowerEnergy(7),totalweight);
  }
  else {
    m_hVector_TotalEnergyCastor_sector8[index].at(pileup)->Fill(0);
  }

  if (eventdiffZ->GetCastorTowerEnergy(8) > 4.*0.364){
    m_hVector_TotalEnergyCastor_sector9[index].at(pileup)->Fill(eventdiffZ->GetCastorTowerEnergy(8),totalweight);
  }
  else{
    m_hVector_TotalEnergyCastor_sector9[index].at(pileup)->Fill(0);
  }

  if (eventdiffZ->GetCastorTowerEnergy(9) > 4.*0.364){
    m_hVector_TotalEnergyCastor_sector10[index].at(pileup)->Fill(eventdiffZ->GetCastorTowerEnergy(9),totalweight);
  }
  else{
    m_hVector_TotalEnergyCastor_sector10[index].at(pileup)->Fill(0);
  }

  if (eventdiffZ->GetCastorTowerEnergy(10) > 4.*0.364){
    m_hVector_TotalEnergyCastor_sector11[index].at(pileup)->Fill(eventdiffZ->GetCastorTowerEnergy(10),totalweight);
  } 
  else{
    m_hVector_TotalEnergyCastor_sector11[index].at(pileup)->Fill(0);
  }

  if (eventdiffZ->GetCastorTowerEnergy(11) > 4.*0.364){
    m_hVector_TotalEnergyCastor_sector12[index].at(pileup)->Fill(eventdiffZ->GetCastorTowerEnergy(11),totalweight);
  }
  else {
    m_hVector_TotalEnergyCastor_sector12[index].at(pileup)->Fill(0);
  }

  if (eventdiffZ->GetCastorTowerEnergy(12) > 4.*0.364){
    m_hVector_TotalEnergyCastor_sector13[index].at(pileup)->Fill(eventdiffZ->GetCastorTowerEnergy(12),totalweight);
  }
  else { 
    m_hVector_TotalEnergyCastor_sector13[index].at(pileup)->Fill(0);
  } 

  if (eventdiffZ->GetCastorTowerEnergy(13) > 4.*0.364){
    m_hVector_TotalEnergyCastor_sector14[index].at(pileup)->Fill(eventdiffZ->GetCastorTowerEnergy(13),totalweight);
  } 
  else {
    m_hVector_TotalEnergyCastor_sector14[index].at(pileup)->Fill(0);
  }  

  if (eventdiffZ->GetCastorTowerEnergy(14) > 4.*0.364){
    m_hVector_TotalEnergyCastor_sector15[index].at(pileup)->Fill(eventdiffZ->GetCastorTowerEnergy(14),totalweight);
  }
  else {
    m_hVector_TotalEnergyCastor_sector15[index].at(pileup)->Fill(0);
  }

  if (eventdiffZ->GetCastorTowerEnergy(15) > 4.*0.364){
    m_hVector_TotalEnergyCastor_sector16[index].at(pileup)->Fill(eventdiffZ->GetCastorTowerEnergy(15),totalweight);
  } 
  else {
    m_hVector_TotalEnergyCastor_sector16[index].at(pileup)->Fill(0);
  }

  sumCastorEnergy = 0.;
  sumCastorAndHFMinusEnergy = 0.;
  SectorCastorHit = 0.;
  SectorZeroCastorCounter = 0.;
  for (l=0; l<16;l++){
    if (eventdiffZ->GetCastorTowerEnergy(l) > 4.*0.364){
      ++SectorCastorHit;
      m_hVector_ECastorSector[index].at(pileup)->Fill(l+1,eventdiffZ->GetCastorTowerEnergy(l),totalweight);
      m_hVector_ECastorSectorTProf[index].at(pileup)->Fill(l+1,eventdiffZ->GetCastorTowerEnergy(l),totalweight);
      m_hVector_ECastorSectorBin1D[index].at(pileup)->Fill(l+1,eventdiffZ->GetCastorTowerEnergy(l)*totalweight);
      sumCastorEnergy+=eventdiffZ->GetCastorTowerEnergy(l);
    }
    else {
      m_hVector_ECastorSector[index].at(pileup)->Fill(l+1,0);
      m_hVector_ECastorSectorTProf[index].at(pileup)->Fill(l+1,0);
      m_hVector_ECastorSectorBin1D[index].at(pileup)->Fill(l+1,0);
      sumCastorEnergy+=0;
      ++SectorZeroCastorCounter;
    }
  }
  m_hVector_sumECastorMinus[index].at(pileup)->Fill(sumCastorEnergy,totalweight);
  sumCastorAndHFMinusEnergy = sumCastorEnergy+eventdiff->GetSumEnergyHFMinus();
  m_hVector_sumECastorAndHFMinus[index].at(pileup)->Fill(sumCastorAndHFMinusEnergy,totalweight);
  m_hVector_etcalos_p[index].at(pileup)->Fill(eventdiff->GetSumEnergyHFPlus(),sumCastorEnergy,totalweight);
  m_hVector_etcalos_n[index].at(pileup)->Fill(eventdiff->GetSumEnergyHFMinus(),sumCastorEnergy,totalweight);
  m_hVector_EnergyHFMinusVsCastorTProf[index].at(pileup)->Fill(eventdiff->GetSumEnergyHFMinus(),sumCastorEnergy,totalweight);
  m_hVector_EnergyHFPlusVsCastorTProf[index].at(pileup)->Fill(eventdiff->GetSumEnergyHFPlus(),sumCastorEnergy,totalweight);
  m_hVector_CastorMultiplicity[index].at(pileup)->Fill(SectorCastorHit,totalweight);
  m_hVector_CastorMultiplicityVsLumi[index].at(pileup)->Fill(eventinfo->GetInstLumiBunch(),SectorCastorHit,totalweight);
  if (SectorCastorHit < 1) m_hVector_RunNumberZeroCastor[index].at(pileup)->Fill(eventdiff->GetRunNumber());
  if (SectorCastorHit > 15) m_hVector_RunNumberHighCastor[index].at(pileup)->Fill(eventdiff->GetRunNumber());
  m_hVector_RunNumber[index].at(pileup)->Fill(eventdiff->GetRunNumber());

  for (k=0; k<eventdiffZ->GetEachTowerCounter();k++){
    m_hVector_ECaloVsEta[index].at(pileup)->Fill(eventdiffZ->GetEachTowerEta(k),eventdiffZ->GetEachTowerEnergy(k),totalweight);
    m_hVector_ECaloVsEtaTProf[index].at(pileup)->Fill(eventdiffZ->GetEachTowerEta(k),eventdiffZ->GetEachTowerEnergy(k),totalweight);
    m_hVector_EnergyVsEtaBin1D[index].at(pileup)->Fill(eventdiffZ->GetEachTowerEta(k),eventdiffZ->GetEachTowerEnergy(k)*totalweight);
  }

  if (eventdiff->GetSumEnergyHEPlus() > eventdiff->GetSumEnergyHEMinus()){
    m_hVector_SumEHFMax[index].at(pileup)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight);
    m_hVector_SumEHFMin[index].at(pileup)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight);
  }

  else {
    m_hVector_SumEHFMax[index].at(pileup)->Fill(eventdiff->GetSumEnergyHFMinus(),totalweight);
    m_hVector_SumEHFMin[index].at(pileup)->Fill(eventdiff->GetSumEnergyHFPlus(),totalweight);
  }

  m_hVector_tracksOutMuonsCone03[index].at(pileup)->Fill(eventdiffZ->GetTracksNonConeMuon03(),totalweight);  
  m_hVector_tracksOutElectronsCone03[index].at(pileup)->Fill(eventdiffZ->GetTracksNonConeElectron03(),totalweight);   
  m_hVector_tracksOutpatMuonsCone03[index].at(pileup)->Fill(eventdiffZ->GetTracksNonConepatMuon03(),totalweight);
  m_hVector_tracksOutpatElectronsCone03[index].at(pileup)->Fill(eventdiffZ->GetTracksNonConepatElectron03(),totalweight);

  m_hVector_tracksOutMuonsCone04[index].at(pileup)->Fill(eventdiffZ->GetTracksNonConeMuon04(),totalweight);
  m_hVector_tracksOutElectronsCone04[index].at(pileup)->Fill(eventdiffZ->GetTracksNonConeElectron04(),totalweight);
  m_hVector_tracksOutpatMuonsCone04[index].at(pileup)->Fill(eventdiffZ->GetTracksNonConepatMuon04(),totalweight);
  m_hVector_tracksOutpatElectronsCone04[index].at(pileup)->Fill(eventdiffZ->GetTracksNonConepatElectron04(),totalweight);

  m_hVector_tracksOutMuonsCone05[index].at(pileup)->Fill(eventdiffZ->GetTracksNonConeMuon05(),totalweight);
  m_hVector_tracksOutElectronsCone05[index].at(pileup)->Fill(eventdiffZ->GetTracksNonConeElectron05(),totalweight);
  m_hVector_tracksOutpatMuonsCone05[index].at(pileup)->Fill(eventdiffZ->GetTracksNonConepatMuon05(),totalweight);
  m_hVector_tracksOutpatElectronsCone05[index].at(pileup)->Fill(eventdiffZ->GetTracksNonConepatElectron05(),totalweight);

  m_hVector_LeadingElectronInnerHits[index].at(pileup)->Fill(innerHits1,totalweight);
  m_hVector_LeadingElectronDCot[index].at(pileup)->Fill(Dcot1,totalweight);
  m_hVector_LeadingElectronDist[index].at(pileup)->Fill(Dist1,totalweight);
  m_hVector_LeadingElectronDeltaEtaTkClu[index].at(pileup)->Fill(DeltaEtaTkClu1,totalweight);
  m_hVector_LeadingElectronDeltaPhiTkClu[index].at(pileup)->Fill(DeltaPhiTkClu1,totalweight);
  m_hVector_LeadingElectronSigmaIeIe[index].at(pileup)->Fill(sigmaIeIe1,totalweight);
  m_hVector_LeadingElectronHE[index].at(pileup)->Fill(HE1,totalweight);
  m_hVector_SecondElectronInnerHits[index].at(pileup)->Fill(innerHits2,totalweight);
  m_hVector_SecondElectronDCot[index].at(pileup)->Fill(Dcot2,totalweight);
  m_hVector_SecondElectronDist[index].at(pileup)->Fill(Dist2,totalweight);
  m_hVector_SecondElectronDeltaEtaTkClu[index].at(pileup)->Fill(DeltaEtaTkClu2,totalweight);
  m_hVector_SecondElectronDeltaPhiTkClu[index].at(pileup)->Fill(DeltaPhiTkClu2,totalweight);
  m_hVector_SecondElectronSigmaIeIe[index].at(pileup)->Fill(sigmaIeIe2,totalweight);
  m_hVector_SecondElectronHE[index].at(pileup)->Fill(HE2,totalweight);
  m_hVector_SumEHFplus_S[index].at(pileup)->Fill(eventdiffZ->GetSumEHF_SPlus(),totalweight);
  m_hVector_SumEHFminus_S[index].at(pileup)->Fill(eventdiffZ->GetSumEHF_SMinus(),totalweight);
  m_hVector_SumEHFplus_L[index].at(pileup)->Fill(eventdiffZ->GetSumEHF_LPlus(),totalweight);
  m_hVector_SumEHFminus_L[index].at(pileup)->Fill(eventdiffZ->GetSumEHF_LMinus(),totalweight);
  m_hVector_maxetagap[index].at(pileup)->Fill(eventdiffZ->GetMaxGapPF(),totalweight);

  m_hVector_ElectronsPt[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronPt(),totalweight);
  m_hVector_ElectronsEta[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronEta(),totalweight);
  m_hVector_ElectronsPhi[index].at(pileup)->Fill(eventdiffZ->GetLeadingElectronPhi(),totalweight);
  m_hVector_MuonsPt[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonPt(),totalweight);
  m_hVector_MuonsEta[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonEta(),totalweight);
  m_hVector_MuonsPhi[index].at(pileup)->Fill(eventdiffZ->GetLeadingMuonPhi(),totalweight);
  m_hVector_patElectronsPt[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1Pt(),totalweight);
  m_hVector_patElectronsEta[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1Eta(),totalweight);
  m_hVector_patElectronsPhi[index].at(pileup)->Fill(eventdiffZ->GetPatElectron1Phi(),totalweight);
  m_hVector_patMuonsPt[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1Pt(),totalweight);
  m_hVector_patMuonsEta[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1Eta(),totalweight);
  m_hVector_patMuonsPhi[index].at(pileup)->Fill(eventdiffZ->GetPatMuon1Phi(),totalweight);
  m_hVector_ElectronsPt[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronPt(),totalweight);
  m_hVector_ElectronsEta[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronEta(),totalweight);
  m_hVector_ElectronsPhi[index].at(pileup)->Fill(eventdiffZ->GetSecondElectronPhi(),totalweight);
  m_hVector_MuonsPt[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonPt(),totalweight);
  m_hVector_MuonsEta[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonEta(),totalweight);
  m_hVector_MuonsPhi[index].at(pileup)->Fill(eventdiffZ->GetSecondMuonPhi(),totalweight);
  m_hVector_patElectronsPt[index].at(pileup)->Fill(eventdiffZ->GetPatElectron2Pt(),totalweight);
  m_hVector_patElectronsEta[index].at(pileup)->Fill(eventdiffZ->GetPatElectron2Eta(),totalweight);
  m_hVector_patElectronsPhi[index].at(pileup)->Fill(eventdiffZ->GetPatElectron2Phi(),totalweight);
  m_hVector_patMuonsPt[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2Pt(),totalweight);
  m_hVector_patMuonsEta[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2Eta(),totalweight);
  m_hVector_patMuonsPhi[index].at(pileup)->Fill(eventdiffZ->GetPatMuon2Phi(),totalweight);

  m_hVector_absdeltaEtaPF[index].at(pileup)->Fill(absdeltaetapf,totalweight);
  m_hVector_deltaEtaPF[index].at(pileup)->Fill(deltaetapf,totalweight);

}

void DiffractiveZ::SaveHistos(std::string type){

  int ipileup;

  if (type=="multiple_pileup") ipileup=21;
  else ipileup=1;

  for (int i = 0; i < ipileup; i++){
    for (std::vector<std::string>::size_type j=0; j<Folders.size(); j++){

      m_hVector_DiElectronMass[j].at(i)->Write();
      m_hVector_LeadingElectronPt[j].at(i)->Write();
      m_hVector_LeadingElectronEta[j].at(i)->Write();
      m_hVector_LeadingElectronPhi[j].at(i)->Write();
      m_hVector_LeadingElectronCharge[j].at(i)->Write();
      m_hVector_SecondElectronPt[j].at(i)->Write();
      m_hVector_SecondElectronEta[j].at(i)->Write();
      m_hVector_SecondElectronPhi[j].at(i)->Write();
      m_hVector_SecondElectronCharge[j].at(i)->Write();
      m_hVector_ElectronsN[j].at(i)->Write();
      m_hVector_DiMuonMass[j].at(i)->Write();
      m_hVector_LeadingMuonPt[j].at(i)->Write();
      m_hVector_LeadingMuonEta[j].at(i)->Write();
      m_hVector_LeadingMuonPhi[j].at(i)->Write();
      m_hVector_LeadingMuonCharge[j].at(i)->Write();
      m_hVector_SecondMuonPt[j].at(i)->Write();
      m_hVector_SecondMuonEta[j].at(i)->Write();
      m_hVector_SecondMuonPhi[j].at(i)->Write();
      m_hVector_SecondMuonCharge[j].at(i)->Write();
      m_hVector_MuonsN[j].at(i)->Write();
      m_hVector_sumEHFplus[j].at(i)->Write();
      m_hVector_sumEHFminus[j].at(i)->Write();
      m_hVector_sumEHEplus[j].at(i)->Write();
      m_hVector_sumEHEminus[j].at(i)->Write();
      m_hVector_lumi[j].at(i)->Write();
      m_hVector_asumE[j].at(i)->Write();
      m_hVector_multhf[j].at(i)->Write();
      m_hVector_etcalos_p[j].at(i)->Write();
      m_hVector_etcalos_n[j].at(i)->Write();
      m_hVector_tracks[j].at(i)->Write();
      m_hVector_pfetamax[j].at(i)->Write();
      m_hVector_pfetamin[j].at(i)->Write();
      m_hVector_vertex[j].at(i)->Write();
      m_hVector_deltaphielectrons[j].at(i)->Write();
      m_hVector_deltaphimuons[j].at(i)->Write();
      m_hVector_deltaetaelectrons[j].at(i)->Write();
      m_hVector_deltaetamuons[j].at(i)->Write();
      m_hVector_deltapTelectrons[j].at(i)->Write();
      m_hVector_deltapTmuons[j].at(i)->Write();
      m_hVector_vertexvslumi[j].at(i)->Write();
      m_hVector_patNElectron[j].at(i)->Write();
      m_hVector_patElectron1Pt[j].at(i)->Write();
      m_hVector_patElectron1Eta[j].at(i)->Write();
      m_hVector_patElectron1Phi[j].at(i)->Write();
      m_hVector_patElectron1Charge[j].at(i)->Write();
      m_hVector_patElectron1Et[j].at(i)->Write();
      m_hVector_patNMuon[j].at(i)->Write();
      m_hVector_patMuon1Pt[j].at(i)->Write();
      m_hVector_patMuon1Eta[j].at(i)->Write();
      m_hVector_patMuon1Phi[j].at(i)->Write();
      m_hVector_patMuon1Charge[j].at(i)->Write();
      m_hVector_patMuon1Et[j].at(i)->Write();
      m_hVector_patElectron2Pt[j].at(i)->Write();
      m_hVector_patElectron2Eta[j].at(i)->Write();
      m_hVector_patElectron2Phi[j].at(i)->Write();
      m_hVector_patElectron2Charge[j].at(i)->Write();
      m_hVector_patElectron2Et[j].at(i)->Write();
      m_hVector_patMuon2Pt[j].at(i)->Write();
      m_hVector_patMuon2Eta[j].at(i)->Write();
      m_hVector_patMuon2Phi[j].at(i)->Write();
      m_hVector_patMuon2Charge[j].at(i)->Write();
      m_hVector_patMuon2Et[j].at(i)->Write();
      m_hVector_patDiElectronMass[j].at(i)->Write();
      m_hVector_patDiMuonMass[j].at(i)->Write();
      m_hVector_patElectron1TkDr03[j].at(i)->Write();
      m_hVector_patElectron1TkDr04[j].at(i)->Write();
      m_hVector_patElectron1EcalDr03[j].at(i)->Write();
      m_hVector_patElectron1EcalDr04[j].at(i)->Write();
      m_hVector_patElectron1HcalDr03[j].at(i)->Write();
      m_hVector_patElectron1HcalDr04[j].at(i)->Write();
      m_hVector_patElectron2TkDr03[j].at(i)->Write();
      m_hVector_patElectron2TkDr04[j].at(i)->Write();
      m_hVector_patElectron2EcalDr03[j].at(i)->Write();
      m_hVector_patElectron2EcalDr04[j].at(i)->Write();
      m_hVector_patElectron2HcalDr03[j].at(i)->Write();
      m_hVector_patElectron2HcalDr04[j].at(i)->Write();
      m_hVector_patElectron1relIsoDr03[j].at(i)->Write();
      m_hVector_patElectron1relIsoDr04[j].at(i)->Write();
      m_hVector_patElectron2relIsoDr03[j].at(i)->Write();
      m_hVector_patElectron2relIsoDr04[j].at(i)->Write();
      m_hVector_patMuon1SumPtR03[j].at(i)->Write();
      m_hVector_patMuon1SumPtR05[j].at(i)->Write();
      m_hVector_patMuon1EmEtR03[j].at(i)->Write();
      m_hVector_patMuon1EmEtR05[j].at(i)->Write();
      m_hVector_patMuon1HadEtR03[j].at(i)->Write();
      m_hVector_patMuon1HadEtR05[j].at(i)->Write();
      m_hVector_patMuon2SumPtR03[j].at(i)->Write();
      m_hVector_patMuon2SumPtR05[j].at(i)->Write();
      m_hVector_patMuon2EmEtR03[j].at(i)->Write();
      m_hVector_patMuon2EmEtR05[j].at(i)->Write();
      m_hVector_patMuon2HadEtR03[j].at(i)->Write();
      m_hVector_patMuon2HadEtR05[j].at(i)->Write();
      m_hVector_patMuon1relIsoDr03[j].at(i)->Write();
      m_hVector_patMuon1relIsoDr05[j].at(i)->Write();
      m_hVector_patMuon2relIsoDr03[j].at(i)->Write();
      m_hVector_patMuon2relIsoDr05[j].at(i)->Write();
      m_hVector_patMuon1relIso[j].at(i)->Write();
      m_hVector_patMuon2relIso[j].at(i)->Write();
      m_hVector_LeadingElectronTkDr03[j].at(i)->Write();
      m_hVector_LeadingElectronTkDr04[j].at(i)->Write();
      m_hVector_LeadingElectronEcalDr03[j].at(i)->Write();
      m_hVector_LeadingElectronEcalDr04[j].at(i)->Write();
      m_hVector_LeadingElectronHcalDr03[j].at(i)->Write();
      m_hVector_LeadingElectronHcalDr04[j].at(i)->Write();
      m_hVector_SecondElectronTkDr03[j].at(i)->Write();
      m_hVector_SecondElectronTkDr04[j].at(i)->Write();
      m_hVector_SecondElectronEcalDr03[j].at(i)->Write();
      m_hVector_SecondElectronEcalDr04[j].at(i)->Write();
      m_hVector_SecondElectronHcalDr03[j].at(i)->Write();
      m_hVector_SecondElectronHcalDr04[j].at(i)->Write();
      m_hVector_LeadingElectronrelIsoDr03[j].at(i)->Write();
      m_hVector_LeadingElectronrelIsoDr04[j].at(i)->Write();
      m_hVector_SecondElectronrelIsoDr03[j].at(i)->Write();
      m_hVector_SecondElectronrelIsoDr04[j].at(i)->Write();
      m_hVector_LeadingMuonSumPtR03[j].at(i)->Write();
      m_hVector_LeadingMuonSumPtR05[j].at(i)->Write();
      m_hVector_LeadingMuonEmEtR03[j].at(i)->Write();
      m_hVector_LeadingMuonEmEtR05[j].at(i)->Write();
      m_hVector_LeadingMuonHadEtR03[j].at(i)->Write();
      m_hVector_LeadingMuonHadEtR05[j].at(i)->Write();
      m_hVector_SecondMuonSumPtR03[j].at(i)->Write();
      m_hVector_SecondMuonSumPtR05[j].at(i)->Write();
      m_hVector_SecondMuonEmEtR03[j].at(i)->Write();
      m_hVector_SecondMuonEmEtR05[j].at(i)->Write();
      m_hVector_SecondMuonHadEtR03[j].at(i)->Write();
      m_hVector_SecondMuonHadEtR05[j].at(i)->Write();
      m_hVector_LeadingMuonrelIsoDr03[j].at(i)->Write();
      m_hVector_LeadingMuonrelIsoDr05[j].at(i)->Write();
      m_hVector_SecondMuonrelIsoDr03[j].at(i)->Write();
      m_hVector_SecondMuonrelIsoDr05[j].at(i)->Write();
      m_hVector_correlationElectron1Pt[j].at(i)->Write();
      m_hVector_correlationElectron1Eta[j].at(i)->Write();
      m_hVector_correlationElectron1Phi[j].at(i)->Write();
      m_hVector_correlationElectron1Iso[j].at(i)->Write();
      m_hVector_correlationMuon1Pt[j].at(i)->Write();
      m_hVector_correlationMuon1Eta[j].at(i)->Write();
      m_hVector_correlationMuon1Phi[j].at(i)->Write();
      m_hVector_correlationMuon1Iso[j].at(i)->Write();
      m_hVector_correlationElectron2Pt[j].at(i)->Write();
      m_hVector_correlationElectron2Eta[j].at(i)->Write();
      m_hVector_correlationElectron2Phi[j].at(i)->Write();
      m_hVector_correlationElectron2Iso[j].at(i)->Write();
      m_hVector_correlationMuon2Pt[j].at(i)->Write();
      m_hVector_correlationMuon2Eta[j].at(i)->Write();
      m_hVector_correlationMuon2Phi[j].at(i)->Write();
      m_hVector_correlationMuon2Iso[j].at(i)->Write();
      m_hVector_DiElectronEta[j].at(i)->Write();
      m_hVector_DiElectronPhi[j].at(i)->Write();
      m_hVector_DiElectronPt[j].at(i)->Write();
      m_hVector_DiMuonEta[j].at(i)->Write();
      m_hVector_DiMuonPhi[j].at(i)->Write();
      m_hVector_DiMuonPt[j].at(i)->Write();
      m_hVector_patDiElectronEta[j].at(i)->Write();
      m_hVector_patDiMuonEta[j].at(i)->Write();
      m_hVector_patDiElectronPhi[j].at(i)->Write();
      m_hVector_patDiMuonPhi[j].at(i)->Write();
      m_hVector_patDiElectronPt[j].at(i)->Write();
      m_hVector_patDiMuonPt[j].at(i)->Write();
      m_hVector_ECaloVsEta[j].at(i)->Write();
      m_hVector_ECaloVsEtaTProf[j].at(i)->Write();
      m_hVector_sumECastorMinus[j].at(i)->Write();
      m_hVector_ECastorSector[j].at(i)->Write();
      m_hVector_ECastorSectorTProf[j].at(i)->Write();
      m_hVector_tracksOutMuonsCone03[j].at(i)->Write();
      m_hVector_tracksOutElectronsCone03[j].at(i)->Write();
      m_hVector_tracksOutpatMuonsCone03[j].at(i)->Write();
      m_hVector_tracksOutpatElectronsCone03[j].at(i)->Write();
      m_hVector_tracksOutMuonsCone04[j].at(i)->Write();
      m_hVector_tracksOutElectronsCone04[j].at(i)->Write();
      m_hVector_tracksOutpatMuonsCone04[j].at(i)->Write();
      m_hVector_tracksOutpatElectronsCone04[j].at(i)->Write();
      m_hVector_tracksOutMuonsCone05[j].at(i)->Write();
      m_hVector_tracksOutElectronsCone05[j].at(i)->Write();
      m_hVector_tracksOutpatMuonsCone05[j].at(i)->Write();
      m_hVector_tracksOutpatElectronsCone05[j].at(i)->Write();
      m_hVector_LeadingElectronInnerHits[j].at(i)->Write();
      m_hVector_LeadingElectronDCot[j].at(i)->Write();
      m_hVector_LeadingElectronDist[j].at(i)->Write();
      m_hVector_LeadingElectronDeltaEtaTkClu[j].at(i)->Write();
      m_hVector_LeadingElectronDeltaPhiTkClu[j].at(i)->Write();
      m_hVector_LeadingElectronSigmaIeIe[j].at(i)->Write();
      m_hVector_LeadingElectronHE[j].at(i)->Write();
      m_hVector_SecondElectronInnerHits[j].at(i)->Write();
      m_hVector_SecondElectronDCot[j].at(i)->Write();
      m_hVector_SecondElectronDist[j].at(i)->Write();
      m_hVector_SecondElectronDeltaEtaTkClu[j].at(i)->Write();
      m_hVector_SecondElectronDeltaPhiTkClu[j].at(i)->Write();
      m_hVector_SecondElectronSigmaIeIe[j].at(i)->Write();
      m_hVector_SecondElectronHE[j].at(i)->Write();   
      m_hVector_SumEHFplus_S[j].at(i)->Write();
      m_hVector_SumEHFminus_S[j].at(i)->Write();
      m_hVector_SumEHFplus_L[j].at(i)->Write();
      m_hVector_SumEHFminus_L[j].at(i)->Write();
      m_hVector_maxetagap[j].at(i)->Write();
      m_hVector_ElectronsPt[j].at(i)->Write();
      m_hVector_ElectronsEta[j].at(i)->Write();
      m_hVector_ElectronsPhi[j].at(i)->Write();
      m_hVector_MuonsPt[j].at(i)->Write();
      m_hVector_MuonsEta[j].at(i)->Write();
      m_hVector_MuonsPhi[j].at(i)->Write();
      m_hVector_patElectronsPt[j].at(i)->Write();
      m_hVector_patElectronsEta[j].at(i)->Write();
      m_hVector_patElectronsPhi[j].at(i)->Write();
      m_hVector_patMuonsPt[j].at(i)->Write();
      m_hVector_patMuonsEta[j].at(i)->Write();
      m_hVector_patMuonsPhi[j].at(i)->Write();

      m_hVector_SumEHFMax[j].at(i)->Write();
      m_hVector_SumEHFMin[j].at(i)->Write();

      m_hVector_EnergyVsEtaBin1D[j].at(i)->Write();

      m_hVector_absdeltaEtaPF[j].at(i)->Write();
      m_hVector_deltaEtaPF[j].at(i)->Write();
      m_hVector_ECastorSectorBin1D[j].at(i)->Write();

      m_hVector_TotalEnergyCastor_sector1[j].at(i)->Write();
      m_hVector_TotalEnergyCastor_sector2[j].at(i)->Write();
      m_hVector_TotalEnergyCastor_sector3[j].at(i)->Write();
      m_hVector_TotalEnergyCastor_sector4[j].at(i)->Write();
      m_hVector_TotalEnergyCastor_sector5[j].at(i)->Write();
      m_hVector_TotalEnergyCastor_sector6[j].at(i)->Write();
      m_hVector_TotalEnergyCastor_sector7[j].at(i)->Write();
      m_hVector_TotalEnergyCastor_sector8[j].at(i)->Write();
      m_hVector_TotalEnergyCastor_sector9[j].at(i)->Write();
      m_hVector_TotalEnergyCastor_sector10[j].at(i)->Write();
      m_hVector_TotalEnergyCastor_sector11[j].at(i)->Write();
      m_hVector_TotalEnergyCastor_sector12[j].at(i)->Write();
      m_hVector_TotalEnergyCastor_sector13[j].at(i)->Write();
      m_hVector_TotalEnergyCastor_sector14[j].at(i)->Write();
      m_hVector_TotalEnergyCastor_sector15[j].at(i)->Write();
      m_hVector_TotalEnergyCastor_sector16[j].at(i)->Write();

      m_hVector_EnergyHFMinusVsCastorTProf[j].at(i)->Write();
      m_hVector_EnergyHFPlusVsCastorTProf[j].at(i)->Write();

      m_hVector_sumECastorAndHFMinus[j].at(i)->Write();
      m_hVector_CastorMultiplicity[j].at(i)->Write();
      m_hVector_CastorMultiplicityVsLumi[j].at(i)->Write();

      m_hVector_RunNumberHighCastor[j].at(i)->Write();
      m_hVector_RunNumberZeroCastor[j].at(i)->Write();
      m_hVector_RunNumber[j].at(i)->Write();

    }
  }

}

void DiffractiveZ::Run(std::string filein_, std::string processname_, std::string savehistofile_, std::string switchtrigger_, int optTrigger_, double lepton1pt_, double lepton2pt_, int nVertex_, std::string type_, std::string switchlumiweight_, double mcweight_, std::string typesel_){

  bool debug = false;

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  filein = filein_;
  savehistofile = savehistofile_;
  processname = processname_;
  type = type_;
  switchtrigger = switchtrigger_;
  switchlumiweight = switchlumiweight_;
  mcweight = mcweight_;
  nVertex = nVertex_;
  optTrigger = optTrigger_;
  lepton1pt = lepton1pt_;
  lepton2pt = lepton2pt_;
  typesel = typesel_;

  std::string selStatus;
  std::string TriggerStatus;

  TFile check1(filein.c_str());

  if (check1.IsZombie()){

    std::cout << "------------------------------------------------" << std::endl;
    std::cout << " There is no the file " << filein << " or the"   << std::endl;
    std::cout << " path is not correct." << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    return;

  }

  if (check1.GetDirectory(processname.c_str())){
    LoadFile(filein,processname);
  }

  else {
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << " There is no directory/path " << processname << std::endl;
    std::cout << " in the file." << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;
    return;
  }

  TFile *outf = new TFile(savehistofile.c_str(),"RECREATE");
  TString outtxt = savehistofile;
  outtxt.ReplaceAll("root","txt");
  std::ofstream outstring(outtxt);

  outstring << "" << std::endl;
  outstring << "<< Gold Events >>" << std::endl;
  outstring << "" << std::endl;
  outstring << "Please, insert this events in another text file to be used by PickEvent Tool. " << std::endl;
  outstring << "Twiki: https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookPickEvents " << std::endl;
  outstring << ">>---------------------------------------------------------------------- " << std::endl;

  int NEVENTS = tr->GetEntries();
  int pileup = -999;
  int triggercounter[20]={0};
  int totalT=0;

  aSumE = -999.;
  deltaphielectrons = -999.;
  deltaphimuons = -999.;
  deltaetaelectrons = -999;
  deltaetamuons = -999;
  deltapTelectrons = -999;
  deltapTmuons = -999;   

  isoTk1 = -999.;
  isoTk2 = -999.;
  isoEcal1 = -999.;
  isoEcal2 = -999.;
  isoHcal1 = -999.;
  isoHcal2 = -999.;  
  innerHits1 = -999;
  Dcot1 = -999.;
  Dist1 = -999.;
  DeltaEtaTkClu1 = -999.;
  DeltaPhiTkClu1 = -999.;
  sigmaIeIe1 = -999.;
  HE1 = -999.;
  innerHits2 = -999;
  Dcot2 = -999.;
  Dist2 = -999.;
  DeltaEtaTkClu2 = -999.;
  DeltaPhiTkClu2 = -999.;
  sigmaIeIe2 = -999.;
  HE2 = -999.;

  TH1::SetDefaultSumw2(true);
  TH2::SetDefaultSumw2(true);

  unsigned NEntries = tr->GetEntries();
  std::cout << "" << std::endl;
  std::cout<< "Reading Tree: "<< NEntries << " events"<<std::endl;
  std::cout << "" << std::endl;

  std::string status;  

  for(int i=0;i<NEVENTS;i++){

    tr->GetEntry(i);

    if ( type=="multiple_pileup" && (eventinfo->GetNPileUpBx0()==-1 && eventinfo->GetNPileUpBxm1()==-1 && eventinfo->GetNPileUpBxp1()==-1 )){
      std::cout << " " << std::endl; 
      std::cout << "--------------------------------------------------------------" << std::endl;
      std::cout << " There is no pile-up TTree information in your PATTuplefile."   << std::endl;
      std::cout << " Please, use another PATTuple with PU information to run mul- " << std::endl;
      std::cout << " tiple PU option." << std::endl;
      std::cout << "--------------------------------------------------------------" << std::endl;
      return;
    }

    if (type=="multiple_pileup"){
      pileup = eventinfo->GetNPileUpBx0();
    }
    else{
      pileup = 0;
    }

    for (int nt=0;nt<20;nt++){
      if(eventdiffZ->GetHLTPath(nt)){
	triggercounter[nt]++;
      }
    }

    aSumE = (eventdiff->GetSumEnergyHFPlus() - eventdiff->GetSumEnergyHFMinus())/(eventdiff->GetSumEnergyHFPlus() + eventdiff->GetSumEnergyHFMinus());
    deltaphielectrons = fabs(eventdiffZ->GetLeadingElectronPhi() - eventdiffZ->GetSecondElectronPhi());
    deltaphimuons = fabs(eventdiffZ->GetLeadingMuonPhi() - eventdiffZ->GetSecondMuonPhi());
    deltaetaelectrons = eventdiffZ->GetLeadingElectronEta() - eventdiffZ->GetSecondElectronEta();
    deltaetamuons = eventdiffZ->GetLeadingMuonEta() - eventdiffZ->GetSecondMuonEta();
    deltapTelectrons = eventdiffZ->GetLeadingElectronPt() - eventdiffZ->GetSecondElectronPt();
    deltapTmuons = eventdiffZ->GetLeadingMuonPt() - eventdiffZ->GetSecondMuonPt();
    deltaetapf = eventdiff->GetEtaMaxFromPFCands() - eventdiff->GetEtaMinFromPFCands();
    absdeltaetapf = fabs(eventdiff->GetEtaMaxFromPFCands() - eventdiff->GetEtaMinFromPFCands()); 

    double totalcommon = 1.;
    double mclumiweight = 1.;

    if (switchlumiweight == "mc_lumi_weight"){
      mclumiweight = mcweight;
    }

    totalcommon = mclumiweight;

    if (!debug){
      if (i==0) {
	std::cout << "" << std::endl;
	std::cout<< "Status Bar" << std::endl;
	std::cout << "" << std::endl;
      }
      loadBar(i,NEVENTS,100,100);
    }

    if (debug){
      std::cout << " " << std::endl;
      std::cout << "# of active towers: " << eventdiffZ->GetEachTowerCounter() << std::endl;
      std::cout << " " <<std::endl;
      for (int ic=0; ic<eventdiffZ->GetEachTowerCounter(); ic++){
	std::cout << "Each Tower Energy: " << eventdiffZ->GetEachTowerEnergy(ic) << std::endl;
      }
      std::cout << " " <<std::endl;
    }

    if (debug){
      if( i % 1000 == 0 ){
	std::cout << "\nEvent " << i << std::endl;
	std::cout << "Nr. events Bx 0: " << eventinfo->GetNPileUpBx0() << std::endl;
	std::cout << "Lumi per Bunch: " << eventinfo->GetInstLumiBunch() << std::endl;
	std::cout << "Luminosity weight (a): " << mclumiweight <<std::endl;
      }
    }

    bool trigger = false;
    bool vertex = false;
    bool diffseln = false;
    bool diffselp = false;
    int castorcount = 0;
    bool castorgap = false;
    bool castoractivity = false;

    bool presel = false;
    bool charge = false;
    bool dimass = false;
    bool nSel = false;
    bool eleBarrel1 = false;
    bool eleEndCap1 = false;
    bool eleBarrel2 = false;
    bool eleEndCap2 = false; 
    bool candSel = false;
    bool isoBarrel1 = false;
    bool isoEndCap1 = false;
    bool isoBarrel2 = false;
    bool isoEndCap2 = false;
    bool isolation = false;
    bool ZKinN = false;
    bool ZKinP = false;

    //if(eventdiff->GetRunNumber() == 146709 || eventdiff->GetRunNumber() == 146710 || eventdiff->GetRunNumber() == 146711 || eventdiff->GetRunNumber() == 146715 || eventdiff->GetRunNumber() == 146726) continue;

    if (switchtrigger == "trigger_all_electron"){
      if (eventdiffZ->GetHLTPath(0) || eventdiffZ->GetHLTPath(1) || eventdiffZ->GetHLTPath(2) || eventdiffZ->GetHLTPath(3) || eventdiffZ->GetHLTPath(4) || eventdiffZ->GetHLTPath(5) || eventdiffZ->GetHLTPath(6)) trigger = true;
      if (debug) std::cout << "\nTrigger Status: " << trigger << ", Trigger All Electron accepted." << std::endl;
      TriggerStatus = "trigger_all_electron";
    }
    else if (switchtrigger == "trigger"){
      if (eventdiffZ->GetHLTPath(optTrigger)) trigger = true;
      if (debug) std::cout << "\nTrigger Status: " << trigger <<", trigger accepted." << std::endl;
      TriggerStatus = "trigger";
    }
    else if (switchtrigger == "no_trigger") {
      trigger = true;
      if (debug) std::cout << "\nTrigger Status: " << trigger << ", no trigger." << std::endl; 
      TriggerStatus = "no_trigger";
    }
    else{
      exit(EXIT_FAILURE);
    }

    if (eventdiff->GetNVertex() == nVertex) vertex = true;
    if ((eventdiff->GetEtaMinFromPFCands() > -3.)) diffseln = true;
    if ((eventdiff->GetEtaMaxFromPFCands() <  3.)) diffselp = true;

    for(int sec=0; sec<16; sec++){
      if (eventdiffZ->GetCastorTowerEnergy(sec) > 4.*0.364) ++castorcount; 
    }

    if (castorcount > 0){
      castoractivity = true;
    }else{
      castorgap = true;
    } 

    if (typesel == "RecoElectron"){
      selStatus = "Reco::Electron";
      isoTk1 = eventdiffZ->GetLeadingElectronTkDr03()/eventdiffZ->GetLeadingElectronPt();
      isoEcal1 = eventdiffZ->GetLeadingElectronEcalDr03()/eventdiffZ->GetLeadingElectronPt();
      isoHcal1 = eventdiffZ->GetLeadingElectronHcalDr03()/eventdiffZ->GetLeadingElectronPt();
      isoTk2 = eventdiffZ->GetSecondElectronTkDr03()/eventdiffZ->GetSecondElectronPt();
      isoEcal2 = eventdiffZ->GetSecondElectronEcalDr03()/eventdiffZ->GetSecondElectronPt();
      isoHcal2 = eventdiffZ->GetSecondElectronHcalDr03()/eventdiffZ->GetSecondElectronPt();
      innerHits1 = eventdiffZ->GetLeadingElectronInnerHits();
      Dcot1 = eventdiffZ->GetLeadingElectronDCot();
      Dist1 = eventdiffZ->GetLeadingElectronDist();
      DeltaEtaTkClu1 = eventdiffZ->GetLeadingElectronDeltaEtaTkClu();
      DeltaPhiTkClu1 = eventdiffZ->GetLeadingElectronDeltaPhiTkClu();
      sigmaIeIe1 = eventdiffZ->GetLeadingElectronSigmaIeIe();
      HE1 = eventdiffZ->GetLeadingElectronHE();
      innerHits2 = eventdiffZ->GetSecondElectronInnerHits();
      Dcot2 = eventdiffZ->GetSecondElectronDCot();
      Dist2 = eventdiffZ->GetSecondElectronDist();
      DeltaEtaTkClu2 = eventdiffZ->GetSecondElectronDeltaEtaTkClu();
      DeltaPhiTkClu2 = eventdiffZ->GetSecondElectronDeltaPhiTkClu();
      sigmaIeIe2 = eventdiffZ->GetSecondElectronSigmaIeIe();
      HE2 = eventdiffZ->GetSecondElectronHE();

      if (eventdiffZ->GetLeadingElectronPt() > lepton1pt && eventdiffZ->GetSecondElectronPt() > lepton2pt) presel = true;
      if (eventdiffZ->GetLeadingElectronCharge()*eventdiffZ->GetSecondElectronCharge()==-1) charge = true;
      if (eventdiffZ->GetDiElectronMass() > 60. && eventdiffZ->GetDiElectronMass() < 110.) dimass = true;
      if (eventdiffZ->GetElectronsN() > 1) nSel = true;

      //Isolation Electron
      if ((fabs (eventdiffZ->GetLeadingElectronEta()) <= 1.4442) ){
	if (isoTk1<0.09 && isoEcal1<0.07 && isoHcal1<0.10) isoBarrel1 = true;
      }

      if ((fabs (eventdiffZ->GetLeadingElectronEta()) >= 1.5660) && (fabs (eventdiffZ->GetLeadingElectronEta()) <= 2.5)){
	if (isoTk1<0.04 && isoEcal1<0.05 && isoHcal1<0.025) isoEndCap1 = true;
      }

      if ((fabs (eventdiffZ->GetSecondElectronEta()) <= 1.4442) ){
	if (isoTk2<0.09 && isoEcal2<0.07 && isoHcal2<0.10) isoBarrel2 = true;
      }

      if ((fabs (eventdiffZ->GetSecondElectronEta()) >= 1.5660) && (fabs (eventdiffZ->GetSecondElectronEta()) <= 2.5)){
	if (isoTk2<0.04 && isoEcal2<0.05 && isoHcal2<0.025) isoEndCap2 = true;
      }

      if ((isoEndCap1 || isoBarrel1) && (isoEndCap2 || isoBarrel2)) isolation = true;

      //Electron Candidate Selection
      if ((fabs (eventdiffZ->GetLeadingElectronEta()) <= 1.4442) ){
	if (innerHits1 == 0 && (fabs (Dcot1) >= 0.02 || fabs (Dist1) >= 0.02 ) && fabs (DeltaEtaTkClu1) < 0.004 && fabs (DeltaPhiTkClu1) < 0.06 && sigmaIeIe1 < 0.01 && HE1 < 0.04 ) eleBarrel1 = true;
      }

      if ((fabs (eventdiffZ->GetLeadingElectronEta()) >= 1.5660) && (fabs (eventdiffZ->GetLeadingElectronEta()) <= 2.5)){
	if (innerHits1 == 0 && (fabs (Dcot1) >= 0.02 || fabs (Dist1) >= 0.02 ) && fabs (DeltaEtaTkClu1) < 0.007 && fabs (DeltaPhiTkClu1) < 0.03 && sigmaIeIe1 < 0.03 && HE1 < 0.025) eleEndCap1 = true;
      }

      if ((fabs (eventdiffZ->GetSecondElectronEta()) <= 1.4442) ){
	if (innerHits2 == 0 && (fabs (Dcot2) >= 0.02 || fabs (Dist2) >= 0.02 ) && fabs (DeltaEtaTkClu2) < 0.004 && fabs (DeltaPhiTkClu2) < 0.06 && sigmaIeIe2 < 0.01 && HE2 < 0.04 ) eleBarrel2 = true;
      }

      if ((fabs (eventdiffZ->GetSecondElectronEta()) >= 1.5660) && (fabs (eventdiffZ->GetSecondElectronEta()) <= 2.5)){
	if (innerHits2 == 0 && (fabs (Dcot2) >= 0.02 || fabs (Dist2) >= 0.02 ) && fabs (DeltaEtaTkClu2) < 0.007 && fabs (DeltaPhiTkClu2) < 0.03 && sigmaIeIe2 < 0.03 && HE2 < 0.025) eleEndCap2 = true;
      }

      if ((eleEndCap1 || eleBarrel1) && (eleEndCap2 || eleBarrel2)) candSel = true;

      if (eventdiffZ->GetDiElectronEta()>0) ZKinP = true;
      if (eventdiffZ->GetDiElectronEta()<0) ZKinN = true;

    }

    else if (typesel == "RecoMuon"){
      selStatus = "Reco::Muon";
      if (eventdiffZ->GetLeadingMuonPt() > lepton1pt && eventdiffZ->GetSecondMuonPt() > lepton2pt) presel = true;
      if (eventdiffZ->GetLeadingMuonCharge()*eventdiffZ->GetSecondMuonCharge()==-1) charge = true;
      if (eventdiffZ->GetDiMuonMass() > 60. && eventdiffZ->GetDiMuonMass() < 110.) dimass = true;
      if (eventdiffZ->GetMuonsN() > 1) nSel = true;
      if (eventdiffZ->GetLeadingMuonSumPtR03() < 3 && eventdiffZ->GetSecondMuonSumPtR03() < 3 ) { 
	isolation = true;
	candSel = true;
      }

      if (eventdiffZ->GetDiMuonEta()>0) ZKinP = true;
      if (eventdiffZ->GetDiMuonEta()<0) ZKinN = true;

    }

    else if (typesel == "PatElectron"){
      selStatus = "Pat::Electron";
      isoTk1 = eventdiffZ->GetPatElectron1TkDr03()/eventdiffZ->GetPatElectron1Pt();
      isoEcal1 = eventdiffZ->GetPatElectron1EcalDr03()/eventdiffZ->GetPatElectron1Pt();
      isoHcal1 = eventdiffZ->GetPatElectron1HcalDr03()/eventdiffZ->GetPatElectron1Pt();
      isoTk2 = eventdiffZ->GetPatElectron2TkDr03()/eventdiffZ->GetPatElectron2Pt();
      isoEcal2 = eventdiffZ->GetPatElectron2EcalDr03()/eventdiffZ->GetPatElectron2Pt();
      isoHcal2 = eventdiffZ->GetPatElectron2HcalDr03()/eventdiffZ->GetPatElectron2Pt();
      innerHits1 = eventdiffZ->GetPatElectron1InnerHits();
      Dcot1 = eventdiffZ->GetPatElectron1DCot();
      Dist1 = eventdiffZ->GetPatElectron1Dist();
      DeltaEtaTkClu1 = eventdiffZ->GetPatElectron1DeltaEtaTkClu();
      DeltaPhiTkClu1 = eventdiffZ->GetPatElectron1DeltaPhiTkClu();
      sigmaIeIe1 = eventdiffZ->GetPatElectron1SigmaIeIe();
      HE1 = eventdiffZ->GetPatElectron1HE();
      innerHits2 = eventdiffZ->GetPatElectron2InnerHits();
      Dcot2 = eventdiffZ->GetPatElectron2DCot();
      Dist2 = eventdiffZ->GetPatElectron2Dist();
      DeltaEtaTkClu2 = eventdiffZ->GetPatElectron2DeltaEtaTkClu();
      DeltaPhiTkClu2 = eventdiffZ->GetPatElectron2DeltaPhiTkClu();
      sigmaIeIe2 = eventdiffZ->GetPatElectron2SigmaIeIe();
      HE2 = eventdiffZ->GetPatElectron2HE();

      if (eventdiffZ->GetPatElectron1Pt() > lepton1pt && eventdiffZ->GetPatElectron2Pt() > lepton2pt) presel = true;
      if (eventdiffZ->GetPatElectron1Charge()*eventdiffZ->GetPatElectron2Charge()==-1) charge = true;
      if (eventdiffZ->GetPatDiElectronMass() > 60. && eventdiffZ->GetPatDiElectronMass() < 110.) dimass = true;
      if (eventdiffZ->GetPatNElectron() > 1) nSel = true;


      //Isolation Electron
      if ((fabs (eventdiffZ->GetPatElectron1Eta()) <= 1.4442) ){
	if (isoTk1<0.09 && isoEcal1<0.07 && isoHcal1<0.10) isoBarrel1 = true;
      }

      if ((fabs (eventdiffZ->GetPatElectron1Eta()) >= 1.5660) && (fabs (eventdiffZ->GetPatElectron1Eta()) <= 2.5)){
	if (isoTk1<0.04 && isoEcal1<0.05 && isoHcal1<0.025) isoEndCap1 = true;
      }

      if ((fabs (eventdiffZ->GetPatElectron2Eta()) <= 1.4442) ){
	if (isoTk2<0.09 && isoEcal2<0.07 && isoHcal2<0.10) isoBarrel2 = true;
      }

      if ((fabs (eventdiffZ->GetPatElectron2Eta()) >= 1.5660) && (fabs (eventdiffZ->GetPatElectron2Eta()) <= 2.5)){
	if (isoTk2<0.04 && isoEcal2<0.05 && isoHcal2<0.025) isoEndCap2 = true;
      }

      if ((isoEndCap1 || isoBarrel1) && (isoEndCap2 || isoBarrel2)) isolation = true;

      //Electron Candidate Selection
      if ((fabs (eventdiffZ->GetPatElectron1Eta()) <= 1.4442) ){
	if (innerHits1 == 0 && (fabs (Dcot1) >= 0.02 || fabs (Dist1) >= 0.02 ) && fabs (DeltaEtaTkClu1) < 0.004 && fabs (DeltaPhiTkClu1) < 0.06 && sigmaIeIe1 < 0.01 && HE1 < 0.04 ) eleBarrel1 = true;
      }

      if ((fabs (eventdiffZ->GetPatElectron1Eta()) >= 1.5660) && (fabs (eventdiffZ->GetPatElectron1Eta()) <= 2.5)){
	if (innerHits1 == 0 && (fabs (Dcot1) >= 0.02 || fabs (Dist1) >= 0.02 ) && fabs (DeltaEtaTkClu1) < 0.007 && fabs (DeltaPhiTkClu1) < 0.03 && sigmaIeIe1 < 0.03 && HE1 < 0.025) eleEndCap1 = true;
      }

      if ((fabs (eventdiffZ->GetPatElectron2Eta()) <= 1.4442) ){
	if (innerHits2 == 0 && (fabs (Dcot2) >= 0.02 || fabs (Dist2) >= 0.02 ) && fabs (DeltaEtaTkClu2) < 0.004 && fabs (DeltaPhiTkClu2) < 0.06 && sigmaIeIe2 < 0.01 && HE2 < 0.04 ) eleBarrel2 = true;
      }

      if ((fabs (eventdiffZ->GetPatElectron2Eta()) >= 1.5660) && (fabs (eventdiffZ->GetPatElectron2Eta()) <= 2.5)){
	if (innerHits2 == 0 && (fabs (Dcot2) >= 0.02 || fabs (Dist2) >= 0.02 ) && fabs (DeltaEtaTkClu2) < 0.007 && fabs (DeltaPhiTkClu2) < 0.03 && sigmaIeIe2 < 0.03 && HE2 < 0.025) eleEndCap2 = true;
      }

      if ((eleEndCap1 || eleBarrel1) && (eleEndCap2 || eleBarrel2)) candSel = true;

      if (eventdiffZ->GetPatDiElectronEta()>0) ZKinP = true;
      if (eventdiffZ->GetPatDiElectronEta()<0) ZKinN = true;


    }

    else if(typesel == "PatMuon"){
      selStatus = "Pat::Muon";
      if (eventdiffZ->GetPatMuon1Pt() > lepton1pt && eventdiffZ->GetPatMuon2Pt() > lepton2pt) presel = true;
      if (eventdiffZ->GetPatMuon1Charge()*eventdiffZ->GetPatMuon2Charge()==-1) charge = true;
      if (eventdiffZ->GetPatDiMuonMass() > 60. && eventdiffZ->GetPatDiMuonMass() < 110.) dimass = true;
      if (eventdiffZ->GetPatNMuon() > 1) nSel = true; 
      if (eventdiffZ->GetPatMuon1SumPtR03() < 3 && eventdiffZ->GetPatMuon2SumPtR03() < 3 ) {
	candSel = true;
	isolation = true;
      }

      if (eventdiffZ->GetPatDiMuonEta()>0) ZKinP = true;
      if (eventdiffZ->GetPatDiMuonEta()<0) ZKinN = true;

    }

    else{
      exit(EXIT_FAILURE);
    }

    if(pileup < 21){ // Never comment this line. It is the program defense.

      if(switchtrigger == "trigger" || switchtrigger == "trigger_all_electron"){ 
	FillHistos(0,pileup,totalcommon); 
	if(trigger) {
	  ++totalT;
	  FillHistos(1,pileup,totalcommon);
	} 
	if(trigger && vertex && presel) FillHistos(2,pileup,totalcommon);
	if(trigger && vertex && presel && nSel) FillHistos(3,pileup,totalcommon);
	if(trigger && vertex && presel && nSel && charge) FillHistos(4,pileup,totalcommon);
	if(trigger && vertex && presel && nSel && charge && dimass) FillHistos(5,pileup,totalcommon);
	if(trigger && vertex && presel && nSel && charge && dimass && isolation) FillHistos(6,pileup,totalcommon);
	if(trigger && vertex && presel && nSel && charge && dimass && isolation && candSel) FillHistos(7,pileup,totalcommon);
	if(trigger && vertex && presel && nSel && charge && dimass && isolation && candSel && diffseln) FillHistos(8,pileup,totalcommon);
	if(trigger && vertex && presel && nSel && charge && dimass && isolation && candSel && diffselp) FillHistos(9,pileup,totalcommon);
	if(trigger && vertex && presel && nSel && charge && dimass && isolation && candSel && diffseln && castorgap) FillHistos(10,pileup,totalcommon);
	if(trigger && vertex && presel && nSel && charge && dimass && isolation && candSel && diffselp && castoractivity) FillHistos(11,pileup,totalcommon);
	if(trigger && vertex && presel && nSel && charge && dimass && isolation && candSel && diffseln && castorgap && ZKinP){
	  FillHistos(12,pileup,totalcommon);
	  outstring << "Positive Z Candidate: " << eventdiff->GetRunNumber() << ":" << eventdiff->GetLumiSection() << ":" << eventdiff->GetEventNumber() << std::endl;
	}
	if(trigger && vertex && presel && nSel && charge && dimass && isolation && candSel && diffselp && castoractivity && ZKinN){
	  FillHistos(13,pileup,totalcommon);
	  outstring << "Negative Z Candidate: " << eventdiff->GetRunNumber() << ":" << eventdiff->GetLumiSection() << ":" << eventdiff->GetEventNumber() << std::endl;
	}

      }

      else if (switchtrigger =="no_trigger"){
	--totalT;
	FillHistos(0,pileup,totalcommon);
	if(vertex && presel) FillHistos(2,pileup,totalcommon);
	if(vertex && presel && nSel) FillHistos(3,pileup,totalcommon);
	if(vertex && presel && nSel && charge) FillHistos(4,pileup,totalcommon);
	if(vertex && presel && nSel && charge && dimass) FillHistos(5,pileup,totalcommon);
	if(vertex && presel && nSel && charge && dimass && isolation) FillHistos(6,pileup,totalcommon);
	if(vertex && presel && nSel && charge && dimass && isolation && candSel) FillHistos(7,pileup,totalcommon);
	if(vertex && presel && nSel && charge && dimass && isolation && candSel && diffseln) FillHistos(8,pileup,totalcommon);
	if(vertex && presel && nSel && charge && dimass && isolation && candSel && diffselp) FillHistos(9,pileup,totalcommon);
	if(vertex && presel && nSel && charge && dimass && isolation && candSel && diffseln && castorgap) FillHistos(10,pileup,totalcommon);
	if(vertex && presel && nSel && charge && dimass && isolation && candSel && diffselp && castoractivity) FillHistos(11,pileup,totalcommon);
	if(vertex && presel && nSel && charge && dimass && isolation && candSel && diffseln && castorgap && ZKinP) FillHistos(12,pileup,totalcommon);
	if(vertex && presel && nSel && charge && dimass && isolation && candSel && diffselp && castoractivity && ZKinN) FillHistos(13,pileup,totalcommon);
      }

      else {
	exit(EXIT_FAILURE);
      }
    }  

  }

  outstring << "" << std::endl;
  outstring << "<< INPUTS >>" << std::endl;
  outstring << " " << std::endl;
  outstring << ">> Input file: " << filein << std::endl;
  outstring << ">> Output file: " << savehistofile << std::endl;
  outstring << ">> TTree Name: " << processname << std::endl;
  outstring << " " << std::endl;
  outstring << "<< OPTIONS >>" << std::endl; 
  outstring << " " << std::endl;
  outstring << ">> Trigger: " << TriggerStatus << std::endl;
  outstring << ">> # Vertex: " << nVertex << std::endl;
  outstring << ">> Lumi. Weight: " << switchlumiweight << " | Weight: " << mcweight << std::endl;
  outstring << ">> Lepton1(pT) > " << lepton1pt <<std::endl;
  outstring << ">> Lepton2(pT) > " << lepton2pt <<std::endl;
  outstring << ">> Type of Selection: " << selStatus << std::endl;
  outstring << " " << std::endl;
  outstring << "<< TRIGGER >> " << std::endl;
  outstring << " " << std::endl;
  outstring << "Total Number of Events: " <<  NEntries << std::endl;
  outstring << "Number of Triggered Events: " << totalT << " | Option: " << optTrigger << std::endl;
  outstring << "(if negative, trigger option is off)" << std::endl;
  outstring << "" << std::endl; 
  outstring << "Each Trigger Fired: " <<  std::endl;
  outstring << "Trigger 0: " << triggercounter[0] << std::endl;
  outstring << "Trigger 1: " << triggercounter[1] << std::endl;
  outstring << "Trigger 2: " << triggercounter[2] << std::endl;
  outstring << "Trigger 3: " << triggercounter[3] << std::endl;
  outstring << "Trigger 4: " << triggercounter[4] << std::endl;
  outstring << "Trigger 5: " << triggercounter[5] << std::endl;
  outstring << "Trigger 6: " << triggercounter[6] << std::endl;
  outstring << "Trigger 7: " << triggercounter[7] << std::endl;
  outstring << "Trigger 8: " << triggercounter[8] << std::endl;
  outstring << "Trigger 9: " << triggercounter[9] << std::endl;
  outstring << "Trigger 10: " << triggercounter[10] << std::endl;
  outstring << "Trigger 11: " << triggercounter[11] << std::endl;
  outstring << "Trigger 12: " << triggercounter[12] << std::endl;
  outstring << "Trigger 13: " << triggercounter[13] << std::endl;
  outstring << "Trigger 14: " << triggercounter[14] << std::endl;
  outstring << "Trigger 15: " << triggercounter[15] << std::endl;
  outstring << "Trigger 16: " << triggercounter[16] << std::endl;
  outstring << "Trigger 17: " << triggercounter[17] << std::endl;
  outstring << "Trigger 18: " << triggercounter[18] << std::endl;
  outstring << "Trigger 19: " << triggercounter[19] << std::endl;
  outstring << "" << std::endl;

  outf->cd();
  SaveHistos(type);
  outf->Close();
  std::cout << "\n" << std::endl;
}

#if !defined(__CINT__) || defined(__MAKECINT__)
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

int main(int argc, char **argv)
{

  AutoLibraryLoader::enable();

  const char *s1="*";
  std::string filein_;
  std::string processname_;
  std::string savehistofile_;
  double lepton1pt_;
  double lepton2pt_;
  int nVertex_;
  int optTrigger_;
  std::string switchtrigger_;
  double mcweight_;
  std::string switchlumiweight_;
  std::string type_;
  std::string typesel_;

  if (argc > 1 && strcmp(s1,argv[1]) != 0) filein_ = argv[1];
  if (argc > 2 && strcmp(s1,argv[2]) != 0) processname_ = argv[2];
  if (argc > 3 && strcmp(s1,argv[3]) != 0) savehistofile_  = argv[3];
  if (argc > 4 && strcmp(s1,argv[4]) != 0) switchtrigger_ = argv[4];
  if (argc > 5 && strcmp(s1,argv[5]) != 0) optTrigger_   = atoi(argv[5]);
  if (argc > 6 && strcmp(s1,argv[6]) != 0) lepton1pt_ = atof(argv[6]);
  if (argc > 7 && strcmp(s1,argv[7]) != 0) lepton2pt_ = atof(argv[7]);
  if (argc > 8 && strcmp(s1,argv[8]) != 0) nVertex_ = atoi(argv[8]);
  if (argc > 9 && strcmp(s1,argv[9]) != 0) type_ = argv[9];
  if (argc > 10 && strcmp(s1,argv[10]) != 0) switchlumiweight_ = argv[10];
  if (argc > 11 && strcmp(s1,argv[11]) != 0) mcweight_ = atof(argv[11]);
  if (argc > 12 && strcmp(s1,argv[12]) != 0) typesel_ = argv[12];

  std::cout << " " << std::endl;
  std::cout << ">>>>> Run with the Options <<<<< " << std::endl;
  std::cout << "Filename, PATTuple: " << filein_ << std::endl;
  std::cout << "ProcessName, PATTuple: " << processname_ << std::endl;
  std::cout << "Output name, histograms: " << savehistofile_ << std::endl;
  std::cout << "Trigger Switch: " << switchtrigger_ << std::endl;
  std::cout << "Trigger Path Option: " << optTrigger_ << std::endl;
  std::cout << "Lepton 1, pT [GeV]: " << lepton1pt_ << std::endl;
  std::cout << "Lepton 2, pT [GeV]: " << lepton2pt_ << std::endl;
  std::cout << "# Vertex: " << nVertex_ << std::endl;
  std::cout << "Multiple Pile-Up Option: " << type_ << std::endl;
  std::cout << "Luminosity Weight Option: " << switchlumiweight_ << std::endl;
  std::cout << "MC Weight: " << mcweight_ << std::endl;
  std::cout << "Type of Selection: " << typesel_ << std::endl;
  std::cout << "" << std::endl;

  if (type_=="multiple_pileup" || type_=="no_multiple_pileup") {

    if (switchtrigger_=="trigger" || switchtrigger_=="no_trigger" || switchtrigger_=="trigger_all_electron") {}
    else{
      std::cout << "Please Insert type of Swithtrigger: " << std::endl;
      std::cout << "1) trigger: run with trigger. Need optTrigger >=0;" << std::endl;
      std::cout << "2) trigger_all_electron: all trigger electron path will be accepted. Do not require optTrigger." << std::endl;
      std::cout << "3) no_trigger: run without trigger." << std::endl;
      return 0;
    }

    if (typesel_=="RecoMuon" || typesel_=="RecoElectron" || typesel_=="PatElectron" || typesel_=="PatMuon" ) {}
    else{
      std::cout << "Please Insert type of Selections: " << std::endl;
      std::cout << "1) RecoMuon: selections with Reco::Muon." << std::endl;
      std::cout << "2) RecoElectron: selections with Reco::Electron." << std::endl;
      std::cout << "3) PatMuon: selections with Pat::Muon." << std::endl;
      std::cout << "4) PatElectron: selections with Pat::Electron." << std::endl;
      return 0;
    }

    if (nVertex_ <= 0 || optTrigger_ < 0 || mcweight_ <= 0 || lepton1pt_ < 0 || lepton2pt_ < 0 ){
      std::cout << "----------------------------------------------" << std::endl;
      std::cout << " Pay attention on the input numbers parameters" << std::endl;
      std::cout << "----------------------------------------------" << std::endl;
      std::cout << ">> Requirements:                             " << std::endl;
      std::cout << "I)   nVertex_ > 0 " << std::endl;
      std::cout << "II)  optTrigger >= 0" << std::endl;
      std::cout << "III) mcweight_ > 0" << std::endl;
      std::cout << "IV)  Lepton1pt_ and Lepton2pt_ >= 0" << std::endl;  
      std::cout << "----------------------------------------------" << std::endl; 
      return 0;
    }

    DiffractiveZ* diffZRun = new DiffractiveZ();
    diffZRun->CreateHistos(type_);
    diffZRun->Run(filein_, processname_, savehistofile_, switchtrigger_, optTrigger_, lepton1pt_, lepton2pt_, nVertex_, type_, switchlumiweight_, mcweight_, typesel_);
    return 0;
  }

  else{
    std::cout << "Please Insert type of histograms: " << std::endl;
    std::cout << "1) multiple_pileup: create histograms for each pile-up event. It works only for MC with pile-up." << std::endl;
    std::cout << "2) no_multiple_pileup: create histograms without each pile-up event. It works for data and MC with/without pile-up." << std::endl;
    return 0;
  }
}

#endif
