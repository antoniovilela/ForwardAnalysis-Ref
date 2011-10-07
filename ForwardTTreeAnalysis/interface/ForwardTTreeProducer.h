#ifndef ForwardTTreeProducer_h
#define ForwardTTreeProducer_h

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>
#include <boost/type_traits.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>

#include "TTree.h"

namespace forwardTTreeAnalysis {

namespace helper {
   struct none{};
 
   template <class T>
   struct event_type {
      typedef typename T::event_type value;
      typedef typename T::event_type* ptr_type;
   };

   template <>
   struct event_type<none> {
      typedef none value;
      typedef none ptr_type;
   };

   struct print {
      void operator()(none) const { }

      template <class T>
      void operator()(T const&) const { std::cout << T::name << std::endl; }
   };
   
   template <class T>
   struct initialize {
      T operator()(edm::ParameterSet const& pset) const { return T( pset.getParameter<edm::ParameterSet>(T::name) ); }
   };

   template <>
   struct initialize<none> { none operator()(edm::ParameterSet const& pset) const {return none();} };

   struct create_event {
      none operator()(none) { return none(); }
   
      template <class T>
      typename event_type<T>::ptr_type operator()(T const& t) const { return new typename event_type<T>::value; }
   };

   struct delete_event {
      void operator()(none) { }
   
      template <class E>
      void operator()(E& ev) const { delete ev; }
   };

   struct create_branch {
      void operator()(TTree*,none,none) { }

      template <class T>
      void operator()(TTree* data, T const& t, typename event_type<T>::ptr_type ev) const {
         data->Branch(T::name,event_type<T>::value::name,ev);
      }
   };

   struct begin_run {
      void operator()(none,edm::Run const &,edm::EventSetup const&) { }

      template <class T>
      void operator()(T& t, edm::Run const & run, edm::EventSetup const& setup) {
         t.setBeginRun(run,setup); 
      }
   };

   struct fill_event {
      void operator()(none,none,edm::Event const&, edm::EventSetup const&) { }

      template <class T>
      void operator()(T& t, typename event_type<T>::ptr_type ev, edm::Event const& event, edm::EventSetup const& setup) {
         t.fillEventData(*ev,event,setup); 
      }
   };
}

template <class T1,class T2=helper::none,class T3=helper::none,class T4=helper::none,class T5=helper::none>
class ForwardTTreeProducer : public edm::EDAnalyzer 
{
  public:
    explicit ForwardTTreeProducer(edm::ParameterSet const& pset):
       t1_(helper::initialize<T1>()(pset)),
       t2_(helper::initialize<T2>()(pset)),
       t3_(helper::initialize<T3>()(pset)),
       t4_(helper::initialize<T4>()(pset)),
       t5_(helper::initialize<T5>()(pset))
    {
       boost::mpl::for_each<seq_>(helper::print());
       boost::mpl::for_each<evtseq_>(helper::print());
    } 
    virtual ~ForwardTTreeProducer() {
       helper::delete_event delete_ev;
       delete_ev(ev1_);delete_ev(ev2_);delete_ev(ev3_);delete_ev(ev4_);delete_ev(ev5_);
    }

    virtual void beginJob();
    virtual void beginRun(edm::Run const&, edm::EventSetup const&);
    virtual void analyze(edm::Event const&, edm::EventSetup const&);
    virtual void endJob();
  private:
    //FIXME Try a generic container
    typedef boost::mpl::vector<T1,T2,T3,T4,T5> seq_;
    typedef boost::mpl::vector<typename helper::event_type<T1>::value,
                               typename helper::event_type<T2>::value,
                               typename helper::event_type<T3>::value,
                               typename helper::event_type<T4>::value,
                               typename helper::event_type<T5>::value> evtseq_;
    T1 t1_;T2 t2_;T3 t3_;T4 t4_;T5 t5_;
    typename helper::event_type<T1>::ptr_type ev1_; 
    typename helper::event_type<T2>::ptr_type ev2_; 
    typename helper::event_type<T3>::ptr_type ev3_; 
    typename helper::event_type<T4>::ptr_type ev4_; 
    typename helper::event_type<T5>::ptr_type ev5_; 

    TTree* data_;
};
} // namespace

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

namespace forwardTTreeAnalysis {

template <class T1,class T2,class T3,class T4,class T5>
void ForwardTTreeProducer<T1,T2,T3,T4,T5>::beginJob() 
{
  edm::Service<TFileService> fs;
  data_ = fs->make<TTree>("ProcessedTree","ProcessedTree");
  helper::create_event create;
  ev1_ = create(t1_); 
  ev2_ = create(t2_); 
  ev3_ = create(t3_); 
  ev4_ = create(t4_); 
  ev5_ = create(t5_);
  helper::create_branch branch;
  branch(data_,t1_,ev1_);branch(data_,t2_,ev2_);branch(data_,t3_,ev3_);branch(data_,t4_,ev4_);branch(data_,t5_,ev5_); 

  data_->Print();
}

template <class T1,class T2,class T3,class T4,class T5>
void ForwardTTreeProducer<T1,T2,T3,T4,T5>::endJob() {}

template <class T1,class T2,class T3,class T4,class T5>
void ForwardTTreeProducer<T1,T2,T3,T4,T5>::beginRun(edm::Run const & run, edm::EventSetup const& setup)
{
  helper::begin_run begin;
  begin(t1_,run,setup);begin(t2_,run,setup);begin(t3_,run,setup);begin(t4_,run,setup);begin(t5_,run,setup);
}

template <class T1,class T2,class T3,class T4,class T5>
void ForwardTTreeProducer<T1,T2,T3,T4,T5>::analyze(edm::Event const& event, edm::EventSetup const& setup) 
{
  helper::fill_event fill;
  fill(t1_,ev1_,event,setup);
  fill(t2_,ev2_,event,setup);
  fill(t3_,ev3_,event,setup);
  fill(t4_,ev4_,event,setup);
  fill(t5_,ev5_,event,setup); 

  data_->Fill();
}

} // namespace
#endif
