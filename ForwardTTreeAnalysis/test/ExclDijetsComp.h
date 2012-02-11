#ifndef ExclDijetsComp_h
#define ExclDijetsComp_h

#include <TFile.h>
#include <TTree.h>

#include <string>
#include <vector>

class DiffractiveEvent;
class ExclusiveDijetsEvent;
class QCDEvent;

//class loadFWLite {
/*    public:
       loadFWLite() {
            gSystem->Load("libFWCoreFWLite.so");
            gSystem->Load("libPhysicsToolsUtilities.so");
            AutoLibraryLoader::enable();
            gSystem->Load("libCintex.so");
            ROOT::Cintex::Cintex::Enable();
       }
};
 
static loadFWLite lfw;
*/

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
   std::string savehistofile;
   double jet1PT;
   double jet2PT;
   int optnVertex;
   int optTrigger;
   bool switchWeightPU;
   bool switchWeightLumi;
   bool switchWeightEff;
   bool switchWeightePw;
   double weightlumipass;
   double triggereffpass;
   //std::vector<TH1D*> m_hVector[1];
   std::vector<std::vector<TH1D*> > m_hVector;

   public :
   ExclDijetsComp() {}
   ~ExclDijetsComp() { 

       //for (std::vector<TH1D*>::const_iterator i(m_hVector.begin()); i != m_hVector.end(); ++i) {
       //  delete (*i);
       // }

      //m_hVector.resize(0);
      inf->Close(); 

   }
   
   void Run(std::string, std::string, double, double, int, int, bool, bool, bool, bool, double, double);
   void LoadFile(std::string);

};
#endif
