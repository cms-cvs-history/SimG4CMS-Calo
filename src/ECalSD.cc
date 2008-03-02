///////////////////////////////////////////////////////////////////////////////
// File: ECalSD.cc
// Description: Sensitive Detector class for electromagnetic calorimeters
///////////////////////////////////////////////////////////////////////////////
#include "SimG4CMS/Calo/interface/ECalSD.h"
#include "SimG4Core/Notification/interface/TrackInformation.h"
#include "Geometry/EcalCommonData/interface/EcalBarrelNumberingScheme.h"
#include "Geometry/EcalCommonData/interface/EcalBaseNumber.h"
#include "Geometry/EcalCommonData/interface/EcalEndcapNumberingScheme.h"
#include "Geometry/EcalCommonData/interface/EcalPreshowerNumberingScheme.h"
#include "Geometry/EcalCommonData/interface/ESTBNumberingScheme.h"
#include "DetectorDescription/Core/interface/DDFilter.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "DetectorDescription/Core/interface/DDSolid.h"
#include "DetectorDescription/Core/interface/DDSplit.h"
#include "DetectorDescription/Core/interface/DDValue.h"

#include "Geometry/EcalCommonData/interface/EcalBaseNumber.h"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

ECalSD::ECalSD(G4String name, const DDCompactView & cpv,
	       SensitiveDetectorCatalog & clg, 
	       edm::ParameterSet const & p, const SimTrackManager* manager) : 
  CaloSD(name, cpv, clg, p, manager), numberingScheme(0) {
  
  //   static SimpleConfigurable<bool>   on1(false, "ECalSD:UseBirkLaw");
  //   static SimpleConfigurable<double> bk1(0.013, "ECalSD:BirkC1");
  //   static SimpleConfigurable<double> bk2(9.6e-6,"ECalSD:BirkC2");
  // Values from NIM 80 (1970) 239-244: as implemented in Geant3
  //   useBirk          = on1.value();
  //   birk1            = bk1.value()*(g/(MeV*cm2));
  //   birk2            = bk2.value()*(g/(MeV*cm2))*(g/(MeV*cm2));

  edm::ParameterSet m_EC = p.getParameter<edm::ParameterSet>("ECalSD");
  useBirk    = m_EC.getParameter<bool>("UseBirkLaw");
  useBirkL3  = m_EC.getParameter<bool>("BirkL3Parametrization");
  birk1      = m_EC.getParameter<double>("BirkC1")*(g/(MeV*cm2));
  birk2      = m_EC.getParameter<double>("BirkC2")*(g/(MeV*cm2))*(g/(MeV*cm2));
  birkSlope  = m_EC.getParameter<double>("BirkSlope");
  birkCut    = m_EC.getParameter<double>("BirkCut");
  slopeLY    = m_EC.getParameter<double>("SlopeLightYield");
  bool isItTB= m_EC.getUntrackedParameter<bool>("TestBeam", false);
  useWeight= true;

  EcalNumberingScheme* scheme=0;
  if      (name == "EcalHitsEB") scheme = dynamic_cast<EcalNumberingScheme*>(new EcalBarrelNumberingScheme());
  else if (name == "EcalHitsEE") scheme = dynamic_cast<EcalNumberingScheme*>(new EcalEndcapNumberingScheme());
  else if (name == "EcalHitsES") {
    if (isItTB) scheme = dynamic_cast<EcalNumberingScheme*>(new ESTBNumberingScheme());
    else        scheme = dynamic_cast<EcalNumberingScheme*>(new EcalPreshowerNumberingScheme());
    useWeight = false;
  } else {edm::LogWarning("EcalSim") << "ECalSD: ReadoutName not supported\n";}

  if (scheme)  setNumberingScheme(scheme);
  LogDebug("EcalSim") 
    << "Constructing a ECalSD  with name " << GetName() << "\n";
  edm::LogInfo("EcalSim")  << "ECalSD:: Use of Birks law is set to      " 
			   << useBirk << "        with the two constants C1 = "
			   << birk1 << ", C2 = " << birk2 << "\n"
			   << "         Use of L3 parametrization " 
			   << useBirkL3 << " with slope " << birkSlope
			   << " and cut off " << birkCut << "\n"
			   << "         Slope for Light yield is set to "
			   << slopeLY;

  edm::LogInfo("EcalSim") << "ECalSD:: Suppression Flag " << suppressHeavy
			  << " protons below " << kmaxProton << " MeV,"
			  << " neutrons below " << kmaxNeutron << " MeV and"
			  << " ions below " << kmaxIon << " MeV";

  if (useWeight) initMap(name,cpv);

}

ECalSD::~ECalSD() {
  if (numberingScheme) delete numberingScheme;
}

double ECalSD::getEnergyDeposit(G4Step * aStep) {
  
  if (aStep == NULL) {
    return 0;
  } else {
    preStepPoint        = aStep->GetPreStepPoint();
    G4String nameVolume = preStepPoint->GetPhysicalVolume()->GetName();

    // take into account light collection curve for crystals
    double weight = 1.;
    if (suppressHeavy) {
      G4Track* theTrack = aStep->GetTrack();
      TrackInformation * trkInfo = (TrackInformation *)(theTrack->GetUserInformation());
      if (trkInfo) {
	int pdg = theTrack->GetDefinition()->GetPDGEncoding();
	if (!(trkInfo->isPrimary())) { // Only secondary particles
	  double ke = theTrack->GetKineticEnergy()/MeV;
	  if (((pdg/1000000000 == 1 && ((pdg/10000)%100) > 0 &&
		((pdg/10)%100) > 0)) && (ke<kmaxIon)) weight = 0;
	  if ((pdg == 2212) && (ke < kmaxProton))     weight = 0;
	  if ((pdg == 2112) && (ke < kmaxNeutron))    weight = 0;
	  if (weight == 0) {
	    LogDebug("EcalSim") << "Ignore Track " << theTrack->GetTrackID()
				<< " Type " << theTrack->GetDefinition()->GetParticleName()
				<< " Kinetic Energy " << ke << " MeV";
	  }
	}
      }
    }
    if (useWeight) {
      weight *= curve_LY(aStep);
      if (useBirk) {
	if (useBirkL3) weight *= getBirkL3(aStep);
	else           weight *= getAttenuation(aStep, birk1, birk2);
      }
    }
    double edep   = aStep->GetTotalEnergyDeposit() * weight;
    LogDebug("EcalSim") << "ECalSD:: " << nameVolume
			<<" Light Collection Efficiency " << weight 
			<< " Weighted Energy Deposit " << edep/MeV << " MeV";
    return edep;
  } 
}

int ECalSD::getRadiationLenght(G4Step * aStep) {
  
  if (aStep == NULL) {
    return 0;
  } else {
    
    G4ThreeVector hitPoint = aStep->GetPreStepPoint()->GetPosition();
    G4VPhysicalVolume* currentPV  = aStep->GetPreStepPoint()->GetPhysicalVolume();
    G4String name = currentPV->GetName();
    std::string crystal;
    crystal.assign(name,0,4);
    
    int thisX0 = 0;
    if (crystal == "EFRY"){
      float z = hitPoint.z();
      float detz = fabs(fabs(z)-3200);
      thisX0 = (int)floor( detz/8.9 );   
    } 
    if(crystal == "EBRY") {
      float x = hitPoint.x();
      float y = hitPoint.y();
      float r = sqrt(x*x +y*y);
      float detr = r -1290;
      thisX0 = (int)floor( detr/8.9);
    }
    
    return thisX0;
  }
}

uint32_t ECalSD::setDetUnitId(G4Step * aStep) { 
  getBaseNumber(aStep);
  return (numberingScheme == 0 ? 0 : numberingScheme->getUnitID(theBaseNumber));
}

void ECalSD::setNumberingScheme(EcalNumberingScheme* scheme) {
  if (scheme != 0) {
    edm::LogInfo("EcalSim") << "EcalSD: updates numbering scheme for " 
			    << GetName() << "\n";
    if (numberingScheme) delete numberingScheme;
    numberingScheme = scheme;
  }
}

void ECalSD::initMap(G4String sd, const DDCompactView & cpv) {

  G4String attribute = "ReadOutName";
  DDSpecificsFilter filter;
  DDValue           ddv(attribute,sd,0);
  filter.setCriteria(ddv,DDSpecificsFilter::equals);
  DDFilteredView fv(cpv);
  fv.addFilter(filter);
  fv.firstChild();

  const G4LogicalVolumeStore * lvs = G4LogicalVolumeStore::GetInstance();
  std::vector<G4LogicalVolume *>::const_iterator lvcite;
  bool dodet=true;
  while (dodet) {
    const DDSolid & sol  = fv.logicalPart().solid();
    const std::vector<double> & paras = sol.parameters();
    G4String name = DDSplit(sol.name()).first;
    G4LogicalVolume* lv=0;
    for (lvcite = lvs->begin(); lvcite != lvs->end(); lvcite++) 
      if ((*lvcite)->GetName() == name) {
	lv = (*lvcite);
	break;
      }
    LogDebug("EcalSim") << "ECalSD::initMap (for " << sd << "): Solid " << name
			<< " Shape " << sol.shape() << " Parameter 0 = " 
			<< paras[0] << " Logical Volume " << lv;
    if (sol.shape() == ddtrap) {
      double dz = 2*paras[0];
      xtalLMap.insert(std::pair<G4LogicalVolume*,double>(lv,dz));
    }
    dodet = fv.next();
  }
  LogDebug("EcalSim") << "ECalSD: Length Table for " << attribute << " = " 
		      << sd << ":";   
  std::map<G4LogicalVolume*,double>::const_iterator ite = xtalLMap.begin();
  int i=0;
  for (; ite != xtalLMap.end(); ite++, i++) {
    G4String name = "Unknown";
    if (ite->first != 0) name = (ite->first)->GetName();
    LogDebug("EcalSim") << " " << i << " " << ite->first << " " << name 
			<< " L = " << ite->second;
  }
}

double ECalSD::curve_LY(G4Step* aStep) {

  G4StepPoint*     stepPoint = aStep->GetPreStepPoint();
  G4LogicalVolume* lv        = stepPoint->GetTouchable()->GetVolume(0)->GetLogicalVolume();
  G4String         nameVolume= lv->GetName();

  double weight = 1.;
  G4ThreeVector  localPoint = setToLocal(stepPoint->GetPosition(),
					 stepPoint->GetTouchable());
  double crlength = crystalLength(lv);
  double dapd = 0.5 * crlength - localPoint.z();
  if (dapd >= -0.1 || dapd <= crlength+0.1) {
    if (dapd <= 100.)
      weight = 1.0 + slopeLY - dapd * 0.01 * slopeLY;
  } else {
    edm::LogWarning("EcalSim") << "ECalSD: light coll curve : wrong distance "
			       << "to APD " << dapd << " crlength = " 
			       << crlength << " crystal name = " << nameVolume 
			       << " z of localPoint = " << localPoint.z() 
			       << " take weight = " << weight;
  }
  LogDebug("EcalSim") << "ECalSD, light coll curve : " << dapd 
		      << " crlength = " << crlength
		      << " crystal name = " << nameVolume 
		      << " z of localPoint = " << localPoint.z() 
		      << " take weight = " << weight;
  return weight;
}

double ECalSD::crystalLength(G4LogicalVolume* lv) {

  double length= 230.;
  std::map<G4LogicalVolume*,double>::const_iterator ite = xtalLMap.find(lv);
  if (ite != xtalLMap.end()) length = ite->second;
  return length;
}

void ECalSD::getBaseNumber(const G4Step* aStep) {

  theBaseNumber.reset();
  const G4VTouchable* touch = aStep->GetPreStepPoint()->GetTouchable();
  int theSize = touch->GetHistoryDepth()+1;
  if ( theBaseNumber.getCapacity() < theSize ) theBaseNumber.setSize(theSize);
  //Get name and copy numbers
  if ( theSize > 1 ) {
    for (int ii = 0; ii < theSize ; ii++) {
      theBaseNumber.addLevel(touch->GetVolume(ii)->GetName(),touch->GetReplicaNumber(ii));
      LogDebug("EcalSim") << "ECalSD::getBaseNumber(): Adding level " << ii
                          << ": " << touch->GetVolume(ii)->GetName() << "["
                          << touch->GetReplicaNumber(ii) << "]";
    }
  }

}

double ECalSD::getBirkL3(G4Step* aStep) {

  double weight = 1.;
  double charge = aStep->GetPreStepPoint()->GetCharge();

  if (charge != 0. && aStep->GetStepLength() > 0) {
    G4Material* mat = aStep->GetPreStepPoint()->GetMaterial();
    double density = mat->GetDensity();
    double dedx    = aStep->GetTotalEnergyDeposit()/aStep->GetStepLength();
    double rkb     = birk1/density;
    weight         = 1. - birkSlope*log(rkb*dedx);
    if (weight < birkCut) weight = birkCut;
    LogDebug("EcalSim") << "ECalSD::getBirkL3 in " << mat->GetName()
                        << " Charge " << charge << " dE/dx " << dedx
                        << " Birk Const " << rkb << " Weight = " << weight 
			<< " dE " << aStep->GetTotalEnergyDeposit();
  }
  return weight;

}
