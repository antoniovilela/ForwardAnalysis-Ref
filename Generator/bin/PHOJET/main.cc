#include "Phojet.h"

#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <iomanip>

#include <boost/lexical_cast.hpp>

#include "HepMC/IO_GenEvent.h"

using namespace std;

void print_help(vector<string> const& parameters){

   stringstream oss;
   oss << "Usage: phojet [options]" << endl;

   vector<string>::const_iterator it_par = parameters.begin();
   vector<string>::const_iterator pars_end = parameters.end();
   for(; it_par != pars_end; ++it_par){
      oss << "   " << "--" <<  *it_par << endl;
   }
   cout << oss.str(); 
}

int main(int argc, char **argv)
{
   vector<string> required_parameters_;
   required_parameters_.push_back("process");
   required_parameters_.push_back("seed");
   required_parameters_.push_back("nevents");
   required_parameters_.push_back("comenergy");
   
   // Read command line parameters 
   /*std::string line_;
   if (argc > 1 && strcmp(s1,argv[1]) != 0)  line_ = argv[1];*/
   vector<string> command_line_parameters_;
   if(argc > 1){
      command_line_parameters_.resize(argc - 1);
      copy(argv + 1, argv + argc, command_line_parameters_.begin());
   }

   // Help option
   vector<string> help_str; help_str.push_back("-h"); help_str.push_back("--help");
   for(vector<string>::const_iterator it_help = help_str.begin();
                                      it_help != help_str.end(); ++it_help){
      if( find(command_line_parameters_.begin(), command_line_parameters_.end(), *it_help) != command_line_parameters_.end() ) { print_help(required_parameters_); return 0; }
   }
 
   // Read required parameters
   map<string,string> required_parameters_map_;
   for(vector<string>::const_iterator it_par = required_parameters_.begin();
                                      it_par != required_parameters_.end(); ++it_par){
      stringstream par_ss; par_ss << "--"; par_ss << *it_par;
      vector<string>::const_iterator it_par_key = find(command_line_parameters_.begin(), command_line_parameters_.end(), par_ss.str());
      if( it_par_key == command_line_parameters_.end() ){
	 stringstream oss;
	 oss << "ERROR: The following parameter was not set: " << *it_par << endl;      
	 throw runtime_error( oss.str() );
      }
      vector<string>::const_iterator it_par_value = it_par_key + 1;
      if(  it_par_value == command_line_parameters_.end() ||
           find(required_parameters_.begin(), required_parameters_.end(), *it_par_value) != required_parameters_.end() ){
	 stringstream oss;
	 oss << "ERROR: Invalid value for parameter: " << *it_par << endl;      
	 throw runtime_error( oss.str() );
      }

      required_parameters_map_[*it_par] = *it_par_value;
      //cout << "  " << setw(15) << *it_par << " = " << required_parameters_map_[*it_par] << endl;  
   }
   
   //----------------------------
   /*std::string line_ = "PROCESS     1  0  1  1  1  1  1  1  1  1  1";
   unsigned int maxEvents_ = 1000;
   double comEnergy_ = 7000;
   long seed_ = 123456789;*/
   string process_ = required_parameters_map_["process"];
   unsigned int maxEvents_ = boost::lexical_cast<unsigned int>( required_parameters_map_["nevents"] );
   long int seed_ = boost::lexical_cast<long int>( required_parameters_map_["seed"] );
   double comEnergy_ = boost::lexical_cast<double>( required_parameters_map_["comenergy"] );
   
   vector<string> phojet_params_;
   phojet_params_.push_back(process_);
   stringstream oss;
   oss  << "=========================================================" << endl
        << "PHOJET will initialize with parameters: " << endl
        << "  PROCESS:    " << process_ << endl
        << "  N events:   " << maxEvents_ << endl
        << "  Seed:       " << seed_ << endl
        << "  COM energy: " << comEnergy_ << endl
        << "=========================================================" << endl;
   cout << oss.str();

   phojet::Phojet* generator = new phojet::Phojet(comEnergy_,seed_,phojet_params_);
   generator->begin();

   //ofstream output("phojet.hepmc");
   HepMC::IO_GenEvent* output = new HepMC::IO_GenEvent("phojet.hepmc",ios::out);
   for(unsigned int evt = 0; evt < maxEvents_; ++evt){
      cout << "\nProcessing event " << (evt + 1) << endl;
      bool success = generator->run();
      if(!success){
         cout << "WARNING: Event " << (evt + 1) << " failed." << endl;
         continue;
      }
      //generator->write(output);
      output->write_event( generator->event() );
   }    
   generator->end();
   //output.close();

   return 0;
}
