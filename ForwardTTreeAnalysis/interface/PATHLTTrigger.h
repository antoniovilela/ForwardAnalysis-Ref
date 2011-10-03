#ifndef PATHLTTrigger_h
#define PATHLTTrigger_h

#include <string>

namespace patInfo {
  class PATInfo;
}

class PATHLTTrigger {
  public:

  PATHLTTrigger();
  ~PATHLTTrigger();
 //------------ Set FWD methods ------------------------------


      void SetHLTPrescale(unsigned fHLTPrescale)                                   { fHLTPrescale = HLTPrescale_;}
      void SetHLTName(std::string fHLTName)                                        { HLTName_ = fHLTName;}

      void SetHLTWasRun(bool fHLTWasRun)                                           { fHLTWasRun =  HLTWasRun_;}
      void SetHLTWasAccept(bool fHLTWasAccept)                                     { fHLTWasAccept =  HLTWasAccept_;}
      void SetHLTWasError(bool fHLTWasError)                                       { fHLTWasError =  HLTWasError_;}
     
      void SetAcceptedL1SeedName(std::string facceptedL1Seeds)                     { acceptedL1Seeds_ =  facceptedL1Seeds;}
      void SetFailedL1SeedName(std::string ffailedL1Seeds)                         { failedL1Seeds_ = ffailedL1Seeds; }

  //
  //------------ Get FWD methods ------------------------------

      unsigned GetHLTPrescale()                                  const { return HLTPrescale_;}
      std::string  GetHLTName()                                  const { return HLTName_;}

      bool GetHLTWasRun()                                        const { return HLTWasRun_;}
      bool GetHLTWasAccept()                                     const { return HLTWasAccept_;}
      bool GetHLTWasError()                                      const { return HLTWasError_;}
      std::string  GetAcceptedL1SeedName()                       const { return acceptedL1Seeds_;}
      std::string  GetFailedL1SeedName()                         const { return failedL1Seeds_;}

  private:
     friend class patInfo::PATInfo;
     std::string  HLTName_;
     
     unsigned HLTPrescale_;
      bool HLTWasRun_;
      bool HLTWasAccept_;
      bool HLTWasError_;
      std::string acceptedL1Seeds_;
      std::string failedL1Seeds_;
     
//////////////////////////////////
};//end code

#endif
