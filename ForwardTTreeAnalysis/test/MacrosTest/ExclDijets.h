#ifndef ExclDijets_h
#define ExclDijets_h

#include <string.h>
#include <TFile.h>
#include <TTree.h>

class loadFWLite {
    public:
       loadFWLite() {
            gSystem->Load("libFWCoreFWLite.so");
            gSystem->Load("libPhysicsToolsUtilities.so");
            AutoLibraryLoader::enable();
            gSystem->Load("libCintex.so");
            ROOT::Cintex::Cintex::Enable();
       }
};
 
static loadFWLite lfw;

class ExclDijets {

string filein;
string fileinput;
string savehistofile;
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

    void LoadFile(string);
    void ExclDijets(string, string, double, double, int, int, bool, bool, bool, bool, double, double);
 
};

