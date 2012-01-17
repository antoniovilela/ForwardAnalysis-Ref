#ifndef ExclDijetsComp_h
#define ExclDijetsComp_h

#include <TFile.h>
#include <TTree.h>

#include <string>
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

   TTree* tr;
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

   public :
   ExclDijetsComp() {}
   ~ExclDijetsComp() {}
   
   void Run(std::string, std::string, double, double, int, int, bool, bool, bool, bool, double, double);
   void LoadFile(std::string);

};
#endif
