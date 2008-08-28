import FWCore.ParameterSet.Config as cms

#
# Geometry master configuration
# for ECAL+HCAL
#
# Ideal geometry, needed for simulation
from SimG4CMS.Calo.testGeometryPMTXML_cfi import *
#  Calorimeters
from Geometry.CaloEventSetup.CaloGeometry_cff import *
from Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi import *
from Geometry.EcalMapping.EcalMapping_cfi import *
from Geometry.EcalMapping.EcalMappingRecord_cfi import *
