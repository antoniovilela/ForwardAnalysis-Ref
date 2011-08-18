#ifndef ForwardAnalysis_Utilities_ObjectToEventSelectorAdaptor_h
#define ForwardAnalysis_Utilities_ObjectToEventSelectorAdaptor_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace forwardAnalysis {
  template <class Object, class SingleObjectSelector> 
  class ObjectToEventSelectorAdaptor {
     public:
        ObjectToEventSelectorAdaptor(edm::ParameterSet const& pset): select_(pset) {}
        ~ObjectToEventSelectorAdaptor() {}

        bool operator() (Object const& obj, edm::Event const& event){
           return select_(obj);
        }
     private:
        SingleObjectSelector select_;  
  };

}
#endif
