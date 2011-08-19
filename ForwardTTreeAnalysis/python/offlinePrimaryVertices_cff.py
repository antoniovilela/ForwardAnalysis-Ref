import FWCore.ParameterSet.Config as cms

# Reco Vertex
# initialize magnetic field #########################
from Configuration.StandardSequences.MagneticField_38T_cff import *
from Configuration.StandardSequences.GeometryPilot2_cff import *
from Configuration.StandardSequences.FrontierConditions_GlobalTag_cff import *
GlobalTag.globaltag = 'IDEAL_V11::All'

from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *
import RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi
offlinePrimaryVerticesFromCTFTracksAVF = RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi.offlinePrimaryVertices.clone()
offlinePrimaryVerticesFromCTFTracksAVF.algorithm = 'AdaptiveVertexFitter'
offlinePrimaryVerticesFromCTFTracksKVF = RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi.offlinePrimaryVertices.clone()
offlinePrimaryVerticesFromCTFTracksKVF.algorithm = 'KalmanVertexFitter'
offlinePrimaryVerticesFromCTFTracksTKF = RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi.offlinePrimaryVertices.clone()
offlinePrimaryVerticesFromCTFTracksTKF.algorithm = 'TrimmedKalmanFinder'
"""
offlinePrimaryVerticesFromCTFTracksAVFModified = RecoVertex.PrimaryVertexProducer.OfflinePrimaryVertices_cfi.offlinePrimaryVertices.clone()
offlinePrimaryVerticesFromCTFTracksAVFModified.algorithm = 'AdaptiveVertexFitter'
offlinePrimaryVerticesFromCTFTracksAVFModified.TkFilterParameters.minSiliconHits = 4
offlinePrimaryVerticesFromCTFTracksAVFModified.TkFilterParameters.maxD0Significance = 10.0
offlinePrimaryVerticesFromCTFTracksAVFModified.TkClusParameters.zSeparation = 0.05
"""

vertexreco = cms.Sequence(offlinePrimaryVerticesFromCTFTracksAVF + 
                          offlinePrimaryVerticesFromCTFTracksKVF)
