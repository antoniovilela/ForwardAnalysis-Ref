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

   struct begin_job {
      void operator()(none) { }

      template <class T>
      void operator()(T& t) {
         t.begin(); 
      }
   };

   struct begin_run {
      void operator()(none,edm::Run const &,edm::EventSetup const&) { }

      template <class T>
      void operator()(T& t, edm::Run const & run, edm::EventSetup const& setup) {
         t.begin(run,setup); 
      }
   };

   struct end_job {
      void operator()(none) { }

      template <class T>
      void operator()(T& t) {
         t.end(); 
      }
   };

   struct fill_event {
      void operator()(none,none,edm::Event const&, edm::EventSetup const&) { }

      template <class T>
      void operator()(T& t, typename event_type<T>::ptr_type ev, edm::Event const& event, edm::EventSetup const& setup) {
         t.fill(*ev,event,setup); 
      }
   };
}

template <class T1,class T2=helper::none,class T3=helper::none,class T4=helper::none,
                   class T5=helper::none,class T6=helper::none,class T7=helper::none,
                   class T8=helper::none,class T9=helper::none,class T10=helper::none>
class ForwardTTreeProducer : public edm::EDAnalyzer 
{
  public:
    explicit ForwardTTreeProducer(edm::ParameterSet const& pset):
       t1_(helper::initialize<T1>()(pset)),
       t2_(helper::initialize<T2>()(pset)),
       t3_(helper::initialize<T3>()(pset)),
       t4_(helper::initialize<T4>()(pset)),
       t5_(helper::initialize<T5>()(pset)),
       t6_(helper::initialize<T6>()(pset)),
       t7_(helper::initialize<T7>()(pset)),
       t8_(helper::initialize<T8>()(pset)),
       t9_(helper::initialize<T9>()(pset)),
       t10_(helper::initialize<T10>()(pset))
    {
       boost::mpl::for_each<seq_>(helper::print());
       boost::mpl::for_each<evtseq_>(helper::print());
    } 
    virtual ~ForwardTTreeProducer() {
       helper::delete_event delete_ev;
       delete_ev(ev1_);delete_ev(ev2_);delete_ev(ev3_);delete_ev(ev4_);delete_ev(ev5_);
       delete_ev(ev6_);delete_ev(ev7_);delete_ev(ev8_);delete_ev(ev9_);delete_ev(ev10_);
    }

    virtual void beginJob();
    virtual void beginRun(edm::Run const&, edm::EventSetup const&);
    virtual void analyze(edm::Event const&, edm::EventSetup const&);
    virtual void endJob();
  private:
    //FIXME Try a generic container
    typedef boost::mpl::vector<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> seq_;
    typedef boost::mpl::vector<typename helper::event_type<T1>::value,
                               typename helper::event_type<T2>::value,
                               typename helper::event_type<T3>::value,
                               typename helper::event_type<T4>::value,
                               typename helper::event_type<T5>::value,
                               typename helper::event_type<T6>::value,
                               typename helper::event_type<T7>::value,
                               typename helper::event_type<T8>::value,
                               typename helper::event_type<T9>::value,
                               typename helper::event_type<T10>::value> evtseq_;

    T1 t1_;T2 t2_;T3 t3_;T4 t4_;T5 t5_;
    T6 t6_;T7 t7_;T8 t8_;T9 t9_;T10 t10_;
    typename helper::event_type<T1>::ptr_type ev1_; 
    typename helper::event_type<T2>::ptr_type ev2_; 
    typename helper::event_type<T3>::ptr_type ev3_; 
    typename helper::event_type<T4>::ptr_type ev4_; 
    typename helper::event_type<T5>::ptr_type ev5_; 
    typename helper::event_type<T6>::ptr_type ev6_; 
    typename helper::event_type<T7>::ptr_type ev7_; 
    typename helper::event_type<T8>::ptr_type ev8_; 
    typename helper::event_type<T9>::ptr_type ev9_; 
    typename helper::event_type<T10>::ptr_type ev10_; 

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

template <class T1,class T2,class T3,class T4,class T5,
          class T6,class T7,class T8,class T9,class T10>
void ForwardTTreeProducer<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10>::beginJob()
{
  edm::Service<TFileService> fs;
  data_ = fs->make<TTree>("ProcessedTree","ProcessedTree");
  helper::create_event create;
  ev1_ = create(t1_); ev2_ = create(t2_); ev3_ = create(t3_); ev4_ = create(t4_); ev5_ = create(t5_);
  ev6_ = create(t6_); ev7_ = create(t7_); ev8_ = create(t8_); ev9_ = create(t9_); ev10_ = create(t10_);
  helper::create_branch branch;
  branch(data_,t1_,ev1_);branch(data_,t2_,ev2_);branch(data_,t3_,ev3_);
  branch(data_,t4_,ev4_);branch(data_,t5_,ev5_);branch(data_,t6_,ev6_);
  branch(data_,t7_,ev7_);branch(data_,t8_,ev8_);branch(data_,t9_,ev9_);branch(data_,t10_,ev10_); 

  data_->Print();

  helper::begin_job begin;
  begin(t1_);begin(t2_);begin(t3_);begin(t4_);begin(t5_);
  begin(t6_);begin(t7_);begin(t8_);begin(t9_);begin(t10_);
}

template <class T1,class T2,class T3,class T4,class T5,
          class T6,class T7,class T8,class T9,class T10>
void ForwardTTreeProducer<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10>::endJob()
{
  helper::end_job end;
  end(t1_);end(t2_);end(t3_);end(t4_);end(t5_);
  end(t6_);end(t7_);end(t8_);end(t9_);end(t10_);
}

template <class T1,class T2,class T3,class T4,class T5,
          class T6,class T7,class T8,class T9,class T10>
void ForwardTTreeProducer<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10>::beginRun(edm::Run const & run, edm::EventSetup const& setup)
{
  helper::begin_run begin;
  begin(t1_,run,setup);begin(t2_,run,setup);begin(t3_,run,setup);begin(t4_,run,setup);begin(t5_,run,setup);
  begin(t6_,run,setup);begin(t7_,run,setup);begin(t8_,run,setup);begin(t9_,run,setup);begin(t10_,run,setup);
}

template <class T1,class T2,class T3,class T4,class T5,
          class T6,class T7,class T8,class T9,class T10>
void ForwardTTreeProducer<T1,T2,T3,T4,T5,T6,T7,T8,T9,T10>::analyze(edm::Event const& event, edm::EventSetup const& setup) 
{
  helper::fill_event fill;
  fill(t1_,ev1_,event,setup); fill(t2_,ev2_,event,setup); fill(t3_,ev3_,event,setup);
  fill(t4_,ev4_,event,setup); fill(t5_,ev5_,event,setup); fill(t6_,ev6_,event,setup);
  fill(t7_,ev7_,event,setup); fill(t8_,ev8_,event,setup); fill(t9_,ev9_,event,setup);
  fill(t10_,ev10_,event,setup); 

  data_->Fill();
}

} // namespace
#endif
