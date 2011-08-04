#ifndef ForwardAnalysis_Utilities_OneToManySelector_h
#define ForwardAnalysis_Utilities_OneToManySelector_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "CommonTools/UtilAlgos/interface/AnyPairSelector.h"
#include "CommonTools/UtilAlgos/interface/AnySelector.h"

namespace forwardAnalysis {

template <class Object, class Coll, 
          class PairSelector=AnyPairSelector, class S1=AnySelector, class S2=AnySelector>
class OneToManySelector {
   public:
      OneToManySelector(edm::ParameterSet const& pset):
         src_( pset.template getParameter<edm::InputTag>("src") ),
         selector_( reco::modules::make<PairSelector>(pset) ),
         s1_( reco::modules::make<S1>(pset) ),
         s2_( reco::modules::make<S2>(pset) ) {}

      ~OneToManySelector() {}

      bool operator() (Object const& obj, edm::Event const& event){
         edm::Handle<Coll> source;
         event.getByLabel(src_,source);
         typename Coll::const_iterator cand = source->begin(), source_end = source->end();
         bool result = true;
         for(; cand != source_end; ++cand){
            if( !s1_(obj) || !s2_(*cand) || !selector_(obj,*cand) ) { result = false; break; }
         }
         return result;
      }

   private: 
      edm::InputTag src_;
      PairSelector selector_;
      S1 s1_;
      S2 s2_;
};

} // namespace

#endif
