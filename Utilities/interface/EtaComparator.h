#ifndef ForwardAnalysis_Utilities_EtaComparator_h
#define ForwardAnalysis_Utilities_EtaComparator_h

/** \class EtaComparator
 *
 * Compare by eta
 * 
 */

template<typename T>
struct LessByEta {
  typedef T first_argument_type;
  typedef T second_argument_type;
  bool operator()( const T & t1, const T & t2 ) const {
    return t1.eta() < t2.eta();
  }
};

template<typename T>
struct GreaterByEta {
  typedef T first_argument_type;
  typedef T second_argument_type;
  bool operator()( const T & t1, const T & t2 ) const {
    return t1.eta() > t2.eta();
  }
};

#endif
