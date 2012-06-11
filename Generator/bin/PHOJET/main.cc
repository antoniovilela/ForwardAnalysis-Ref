#include "Phojet.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "HepMC/IO_GenEvent.h"

int main(int argc, char **argv)
{
   /*std::string line_;
   if (argc > 1 && strcmp(s1,argv[1]) != 0)  line_ = argv[1];*/

   std::string line_ = "PROCESS     1  0  1  1  1  1  1  1  1  1  1";
   unsigned int maxEvents_ = 1000;

   double comEnergy_ = 7000;
   long seed_ = 123456789;
   std::vector<std::string> params_;
   params_.push_back(line_);

   //std::ofstream output("phojet.hepmc");
   HepMC::IO_GenEvent* output = new HepMC::IO_GenEvent("phojet.hepmc",std::ios::out);

   phojet::Phojet* generator = new phojet::Phojet(comEnergy_,seed_,params_);
   generator->begin();
   for(unsigned int evt = 0; evt < maxEvents_; ++evt){
      std::cout << "Processing event " << (evt + 1) << std::endl;
      bool success = generator->run();
      if(!success){
         std::cout << "WARNING: Event " << (evt + 1) << " failed." << std::endl;
         continue;
      }
      //generator->write(output);
      output->write_event( generator->event() );
   }    
   generator->end();
   //output.close();

   return 0;
}
