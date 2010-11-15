#ifndef ForwardAnalysis_Utilities_OneToManySelector_h
#define ForwardAnalysis_Utilities_OneToManySelector_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "CommonTools/UtilAlgos/interface/AnyPairSelector.h"

namespace forwardAnalysis {

template <class Object, class Coll, class PairSelector=AnyPairSelector>
class OneToManySelector {
   public:
      OneToManySelector(edm::ParameterSet const& pset):
         src_(pset.template getParameter<edm::InputTag>("src")),
         selector_(reco::modules::make<PairSelector>(pset)) {}

      ~OneToManySelector() {}

      bool operator() (edm::Event const& event, Object const& obj){
         edm::Handle<Coll> source;
         event.getByLabel(src_,source);
         typename Coll::const_iterator cand = source->begin(), source_end = source->end();
         bool result = true;
         for(; cand != source_end; ++cand){
            if( !selector_(obj,*cand) ) { result = false; break; }
         }
         return result;
      }

   private: 
      edm::InputTag src_;
      PairSelector selector_;
};

} // namespace

#endif
