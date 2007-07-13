///////////////////////////////////////////////////////////////////////////////
// File: HcalNumberingScheme.cc
// Description: Numbering scheme for barrel electromagnetic calorimeter
///////////////////////////////////////////////////////////////////////////////
#include "SimG4CMS/Calo/interface/HcalNumberingScheme.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"

#include "CLHEP/Units/SystemOfUnits.h"
#include <iostream>

HcalNumberingScheme::HcalNumberingScheme() : CaloNumberingScheme(0) {
  edm::LogInfo("HcalSim") << "Creating HcalNumberingScheme";
}

HcalNumberingScheme::~HcalNumberingScheme() {
  edm::LogInfo("HcalSim") << "Deleting HcalNumberingScheme";
}

uint32_t HcalNumberingScheme::getUnitID(const HcalNumberingFromDDD::HcalID id){

  int zside = 2*(id.zside) - 1;
  int etaR  = zside*(id.etaR);
  int depth = id.depth;
  HcalSubdetector subdet =  (HcalSubdetector)(id.subdet);

  int flag = 0;
  if (subdet == HcalOuter) {
    flag = -1;
  } else if (subdet == HcalBarrel && id.lay == 1) {
    flag   = 1;
    subdet = HcalOuter;
    depth  = 4;
  }
  // if (subdet == HcalBarrel && id.lay == 1) { flag   = 1; depth = 3;}

  //pack it into an integer
  // to be consistent with HcalDetId convention
  uint32_t index = 0;
  if (flag >= 0) index = HcalDetId(subdet,etaR,id.phis,depth).rawId();

  LogDebug("HcalSim") << "HcalNumberingScheme flag = " << flag << " det = "
		      << id.subdet << " depth/lay = " << id.depth << "/" 
		      << id.lay << " zside = " << id.zside << " eta/R = " 
		      << id.etaR << " phi = " << id.phis << " oldphi = " 
		      << id.phi << " packed index = 0x" << std::hex << index 
		      << std::dec;

  return index;

}
