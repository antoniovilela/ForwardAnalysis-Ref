#ifndef UtilitiesPlottingTools_StdAllocatorAdaptor_h
#define UtilitiesPlottingTools_StdAllocatorAdaptor_h

class StdAllocatorAdaptor {
  public:
     template <class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
     T* make(const Arg1 & a1, const Arg2 & a2, const Arg3 & a3, const Arg4 & a4) const{
        return new T(a1,a2,a3,a4);
     }

     template <class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
     T* make(const Arg1 & a1, const Arg2 & a2, const Arg3 & a3, const Arg4 & a4, const Arg5 & a5) const{
        return new T(a1,a2,a3,a4,a5); 
     }

     template <class T, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
                        typename Arg5, typename Arg6, typename Arg7, typename Arg8>
     T* make(const Arg1 & a1, const Arg2 & a2, const Arg3 & a3, const Arg4 & a4, const Arg5 & a5, const Arg6 & a6, const Arg7 & a7, const Arg8 & a8) const{
        return new T(a1,a2,a3,a4,a5,a6,a7,a8);
     }
};

#endif
