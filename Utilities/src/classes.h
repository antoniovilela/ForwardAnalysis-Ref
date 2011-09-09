#include "DataFormats/Common/interface/Wrapper.h"

#include <map>
#include <vector>

namespace {
  struct dictionary {
     std::map<unsigned int, std::vector<double> > dummymv1;
     std::map<int, std::vector<unsigned int> > dummymv2;
     std::map<int, std::vector<double> > dummymv3;
     edm::Wrapper<std::map<unsigned int, std::vector<double> > > dummywmv1;
     edm::Wrapper<std::map<int, std::vector<unsigned int> > > dummywmv2;
     edm::Wrapper<std::map<int, std::vector<double> > > dummywmv3;
  };
} 
