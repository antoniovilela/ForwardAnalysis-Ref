#ifndef PATL1Trigger_h
#define PATL1Trigger_h

#include <string>

namespace patInfo {
  class PATInfo;
}

class PATL1Trigger {
  public:

  PATL1Trigger();
  ~PATL1Trigger();
 //------------ Set FWD methods ------------------------------
  void SetTechL1Bit(bool fL1bit)                                         { TechL1bit_  =  fL1bit;}

  void SetGTL1Results(bool fGTL1Results )                                { GTL1Results_ = fGTL1Results; }
  void SetL1TriggerDecision(bool fL1TriggerDecision )                     { L1TriggerDecision_ = fL1TriggerDecision;}
  void SetL1TriggerDecisionBeforeMask(bool fL1TriggerDecisionBeforeMask ) { L1TriggerDecisionBeforeMask_ = fL1TriggerDecisionBeforeMask; }
  void SetL1TriggerDecisionAfterMask(bool fL1TriggerDecisionAfterMask )   { L1TriggerDecisionAfterMask_ = fL1TriggerDecisionAfterMask; }
  void SetL1TriggerName(std::string fTriggerName)                              { TriggerName_ = fTriggerName;}
  void SetL1LogicalExpression(std::string fL1LogicalExpression)                { L1LogicalExpression_ = fL1LogicalExpression;}
  void SetL1Prescale(unsigned fL1Prescale)                                     { fL1Prescale  = L1Prescale_;}

  //
  //------------ Get FWD methods ------------------------------
  bool GetTechL1Bit()                  const{ return TechL1bit_;}

  bool GetGTL1Results()              const { return GTL1Results_; }
  bool GetL1TriggerDecision()             const { return L1TriggerDecision_ ;}
  bool GetL1TriggerDecisionBeforeMask()   const { return L1TriggerDecisionBeforeMask_; }
  bool GetL1TriggerDecisionAfterMask()    const { return L1TriggerDecisionBeforeMask_; }
  std::string GetL1TriggerName()                 const { return TriggerName_ ;}
  std::string GetL1LogicalExpression()           const { return L1LogicalExpression_;}
  unsigned GetL1Prescale()                       const { return  L1Prescale_;}

  private:
     friend class patInfo::PATInfo;

     bool TechL1bit_;

     bool GTL1Results_;
     bool L1TriggerDecisionBeforeMask_;
     bool L1TriggerDecisionAfterMask_;
     bool L1TriggerDecision_;
     std::string TriggerName_ ;
     std::string L1LogicalExpression_ ;

     unsigned L1Prescale_;
     
//////////////////////////////////
};//end code

#endif
