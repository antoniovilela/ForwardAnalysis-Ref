#ifndef ForwardEDMProducer_h
#define ForwardEDMProducer_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>
#include <boost/type_traits.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/for_each.hpp>

namespace forwardEDMAnalysis {

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

template <class Analysis>
class ForwardEDMProducer : public edm::EDProducer 
{
  public:
    explicit ForwardEDMProducer(edm::ParameterSet const& pset):
       analysis_(helper::initialize<Analysis>()(pset))
    {
       boost::mpl::for_each<seq_>(helper::print());
       boost::mpl::for_each<evtseq_>(helper::print());

       produces<event_type>();
    } 
    virtual ~ForwardEDMProducer() {
       //helper::delete_event delete_ev;
       //delete_ev(event_);
    }

  private:
    typedef typename helper::event_type<Analysis>::value event_type;
    typedef typename helper::event_type<Analysis>::ptr_type event_ptr_type;

    virtual void beginJob();
    virtual void beginRun(edm::Run const&, edm::EventSetup const&);
    virtual void produce(edm::Event&, edm::EventSetup const&);
    virtual void endJob();

    //FIXME Try a generic container
    typedef boost::mpl::vector<Analysis> seq_;
    typedef boost::mpl::vector<event_type> evtseq_;

    Analysis analysis_;
    //event_ptr_type event_; 
};
} // namespace

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"

namespace forwardEDMAnalysis {

template <class Analysis>
void ForwardEDMProducer<Analysis>::beginJob()
{
  //helper::create_event create;
  //event_ = create(analysis_);

  helper::begin_job begin;
  begin(analysis_);
}

template <class Analysis>
void ForwardEDMProducer<Analysis>::endJob()
{
  helper::end_job end;
  end(analysis_);
}

template <class Analysis>
void ForwardEDMProducer<Analysis>::beginRun(edm::Run const & run, edm::EventSetup const& setup)
{
  helper::begin_run begin;
  begin(analysis_,run,setup);
}

template <class Analysis>
void ForwardEDMProducer<Analysis>::produce(edm::Event& event, edm::EventSetup const& setup)
{
  helper::create_event create;
  std::auto_ptr<event_type> event_ptr( create(analysis_) );

  helper::fill_event fill;
  fill(analysis_,event_ptr.get(),event,setup);

  event.put( event_ptr );  
}

} // namespace
#endif
