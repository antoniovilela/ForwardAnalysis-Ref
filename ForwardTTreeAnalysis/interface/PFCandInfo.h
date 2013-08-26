#ifndef PFCandInfo_h
#define PFCandInfo_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "FWCore/Utilities/interface/InputTag.h"
#include "ForwardAnalysis/ForwardTTreeAnalysis/interface/PFCand.h"
#include <string>
#include <vector>
#include "DataFormats/Math/interface/LorentzVector.h"

class PFCandInfoEvent;

namespace pfCandInfo{

	class PFCandInfo {
		public:
			typedef PFCandInfoEvent event_type;
			static const char* name;

			PFCandInfo() {};
			PFCandInfo(const edm::ParameterSet&);
			~PFCandInfo();


			void begin();
			void begin(const edm::Run&, const edm::EventSetup&);
			void fill(PFCandInfoEvent&, const edm::Event&, const edm::EventSetup&);
			void end();

//			void fillPFFlowInfo(PFCandInfoEvent&, const edm::Event&, const edm::EventSetup&, std::vector<pfCandInfo::PFCandInfo>&);

		private:
			//void pfCandInfo::PFCandInfo::fillPFFlowInfo(PFCandInfoEvent&, const edm::Event&, const edm::EventSetup&, std::vector<pfCandInfo::PFCandInfo, std::allocator<pfCandInfo::PFCandInfo> >&)                 
			//void fillPFFlowInfo(PFCandInfoEvent&, const edm::Event&, const edm::EventSetup&, std::vector<pfCandInfo::PFCandInfo>&);
			void fillPFFlowInfo(PFCandInfoEvent&, const edm::Event&, const edm::EventSetup&);
			//std::vector<PFCandInfo>  PFCandVector;
			edm::InputTag particleTag_;

	};
} //namespace

#endif //end code






















