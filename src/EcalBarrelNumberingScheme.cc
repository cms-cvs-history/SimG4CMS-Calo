///////////////////////////////////////////////////////////////////////////////
// File: EcalBarrelNumberingScheme.cc
// Description: Numbering scheme for barrel electromagnetic calorimeter
///////////////////////////////////////////////////////////////////////////////
#include "SimG4CMS/Calo/interface/EcalBarrelNumberingScheme.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"

#include "CLHEP/Units/SystemOfUnits.h"
#include <iostream>

#define debug

EcalBarrelNumberingScheme::EcalBarrelNumberingScheme(int iv) : 
  EcalNumberingScheme(iv) {
  if (verbosity>0) 
    std::cout << "Creating EcalBarrelNumberingScheme" << std::endl;
}

EcalBarrelNumberingScheme::~EcalBarrelNumberingScheme() {
  if (verbosity>0) 
    std::cout << "Deleting EcalBarrelNumberingScheme" << std::endl;
}

uint32_t EcalBarrelNumberingScheme::getUnitID(const G4Step* aStep) const {

  G4StepPoint* PreStepPoint = aStep->GetPreStepPoint(); 
  const G4VTouchable* touch = aStep->GetPreStepPoint()->GetTouchable();
  G4ThreeVector HitPoint    = PreStepPoint->GetPosition();	
  int PVid  = touch->GetReplicaNumber(0);
  int MVid  = 1; 
  int MMVid = 1;
  if (touch->GetHistoryDepth() > 0) {
    MVid = touch->GetReplicaNumber(1);
  } else { 
    if (verbosity>0) 
      std::cout << "ECalBarrelNumberingScheme::getUnitID: Null pointer to "
		<< "alveole ! Use default id=1 " << std::endl;
  }
  if (touch->GetHistoryDepth() > 1) { 
    MMVid = touch->GetReplicaNumber(2);
  } else { 
    if (verbosity>0) 
      std::cout << "ECalBarrelNumberingScheme::getUnitID: Null pointer to "
		<< "module ! Use default id=1 " << std::endl;
  }

  // z side 
  int zside=HitPoint.z()>0 ? 1: -1;

  // eta index of in Lyon geometry
  int ieta = PVid%5;
  if( ieta == 0) {ieta = 5;}
  int eta = 5 * (int) ((float)(PVid - 1)/10.) + ieta;

  // phi index in Lyon geometry
  int isubm = 1 + (int) ((float)(PVid - 1)/5.);
  int iphi  = (isubm%2) == 0 ? 2: 1;
  int phi   = 20*(18-MMVid) + 2*(10-MVid) + iphi;

  //pack it into an integer
  // to be consistent with EBDetId convention
  //  zside=2*(1-zside)+1;
  uint32_t intindex = EBDetId(zside*eta,phi).rawId();
#ifdef debug
  if (verbosity>1) 
    std::cout << "EcalBarrelNumberingScheme zside = "  << zside << " eta = " 
	      << eta << " phi = " << phi << " packed index = 0x" << std::hex 
	      << intindex << std::dec << std::endl;
#endif
  return intindex;

}
