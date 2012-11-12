#ifndef Utilities_AnalysisTools_ScaledObjectProducer_h
#define Utilities_AnalysisTools_ScaledObjectProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CommonTools/UtilAlgos/interface/SelectedOutputCollectionTrait.h"

namespace analysisTools {

   namespace helper {
     template<typename Collection>
     struct ObjectConverter {
        typedef typename Collection::value_type value_type;
        template<typename T>
        static value_type convert( const T & obj ) {
           return value_type( obj );
        }
     };
   }

template <class InputCollection,
          class OutputCollection = typename ::helper::SelectedOutputCollectionTrait<InputCollection>::type,
          class ClonePolicy = helper::ObjectConverter<OutputCollection> >
class ScaledObjectProducer: public edm::EDProducer{
  public:
     explicit ScaledObjectProducer(const edm::ParameterSet&);
     ~ScaledObjectProducer() {}

     virtual void produce(edm::Event&,const edm::EventSetup&);
  private:
     edm::InputTag srcTag_;
     double scale_;  
};

} // namespace

#include "ScaledObjectProducer.icc"

#endif
