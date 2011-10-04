#ifndef DijetsTriggerEvent_h
#define DijetsTriggerEvent_h

#include <string>
#include <vector>

namespace dijetsTriggerAnalysis {
   class DijetsTriggerAnalysis;
}

class DijetsTriggerEvent {
   public:
      typedef dijetsTriggerAnalysis::DijetsTriggerAnalysis type;

      DijetsTriggerEvent();
      ~DijetsTriggerEvent();
      //------------ Set FWD methods ------------------------------

      void SetNBits(int nBits) { nL1Bits_ = nBits; }
      void SetL1Bit(int idx, int decision) { l1Decision_[idx] = decision; }
      void SetL1Prescale(int idx, int prescale) { l1Prescale_[idx] = prescale; }
      void SetL1AlgoName(int idx, std::string const& name) { l1AlgoName_[idx] = name; }

      void SetEtSum(int ring,int etSum) { hfRingEtSum_[ring] = etSum;}
      void SetBitCount(int ring,int bitCount) { hfRingBitCount_[ring] = bitCount;}
    
      //
      //------------ Get FWD methods ------------------------------

      int GetNBits() const { return nL1Bits_; }
      int GetL1Bit(int idx) const { return l1Decision_[idx]; }
      int GetL1Prescale(int idx) const { return l1Prescale_[idx]; }
      std::string GetL1AlgoName(int idx) const { return l1AlgoName_[idx]; }

      int GetEtSum(int ring) const { return hfRingEtSum_[ring]; }
      int GetBitCount(int ring) const { return hfRingBitCount_[ring]; }
   private:
      friend class dijetsTriggerAnalysis::DijetsTriggerAnalysis;
      void reset();

      int nL1Bits_;
      std::vector<int> l1Decision_;
      std::vector<int> l1Prescale_;
      std::vector<std::string> l1AlgoName_;
      std::vector<int> hfRingEtSum_;
      std::vector<int> hfRingBitCount_;
};//end code

#endif
