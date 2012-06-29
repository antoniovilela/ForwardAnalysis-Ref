import FWCore.ParameterSet.Config as cms

def addCastorRecHitCorrector(process):
    process.castorRecHitCorrector = cms.EDProducer("RecHitCorrector",
	rechitLabel = cms.InputTag("castorreco","","RECO"),
	revertFactor = cms.double(1),
	doInterCalib = cms.bool(False)
    )

    process.load("CondCore.DBCommon.CondDBSetup_cfi")
    process.CastorDbProducer = cms.ESProducer("CastorDbProducer")
    process.es_castor_pool = cms.ESSource("PoolDBESSource",
	process.CondDBSetup,
	timetype = cms.string('runnumber'),
	connect = cms.string('frontier://cmsfrontier.cern.ch:8000/FrontierProd/CMS_COND_31X_HCAL'),
	authenticationMethod = cms.untracked.uint32(0),
	toGet = cms.VPSet(
	    cms.PSet(
		record = cms.string('CastorChannelQualityRcd'),  
		tag = cms.string('CastorChannelQuality_v2.2_offline')
	    )
	)
    )
    process.es_prefer_castor = cms.ESPrefer('PoolDBESSource','es_castor_pool')

    process.castorInvalidDataFilter = cms.EDFilter("CastorInvalidDataFilter")
    process.castorSequence = cms.Sequence(process.castorInvalidDataFilter+process.castorRecHitCorrector)
