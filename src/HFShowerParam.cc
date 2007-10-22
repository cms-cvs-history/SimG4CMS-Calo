///////////////////////////////////////////////////////////////////////////////
// File: HFShowerParam.cc
// Description: Parametrized version of HF hits
///////////////////////////////////////////////////////////////////////////////

#include "SimG4CMS/Calo/interface/HFShowerParam.h"
#include "DetectorDescription/Core/interface/DDFilter.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"

#include "FWCore/Utilities/interface/Exception.h"

#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Units/SystemOfUnits.h"

HFShowerParam::HFShowerParam(std::string & name, const DDCompactView & cpv,
			     edm::ParameterSet const & p) {

  edm::ParameterSet m_HF  = p.getParameter<edm::ParameterSet>("HFShower");
  cFibre                  = c_light*(m_HF.getParameter<double>("CFibre"));
  pePerGeV                = m_HF.getUntrackedParameter<double>("PEPerGeV",.25);
  
  G4String attribute = "ReadOutName";
  G4String value     = name;
  DDSpecificsFilter filter;
  DDValue           ddv(attribute,value,0);
  filter.setCriteria(ddv,DDSpecificsFilter::equals);
  DDFilteredView fv(cpv);
  fv.addFilter(filter);
  bool dodet = fv.firstChild();
  if (dodet) {
    DDsvalues_type sv(fv.mergedSpecifics());

    //Special Geometry parameters
    gpar      = getDDDArray("gparHF",sv);
    edm::LogInfo("HFShower") << "HFShowerParam: " <<gpar.size() <<" gpar (cm)";
    for (unsigned int ig=0; ig<gpar.size(); ig++)
      edm::LogInfo("HFShower") << "HFShowerParam: gpar[" << ig << "] = "
			       << gpar[ig]/cm << " cm";
  } else {
    edm::LogError("HFShower") << "HFShowerParam: cannot get filtered "
			      << " view for " << attribute << " matching "
			      << name;
    throw cms::Exception("Unknown", "HFShowerParam")
      << "cannot match " << attribute << " to " << name <<"\n";
  }
}

HFShowerParam::~HFShowerParam() {}

std::vector<double> HFShowerParam::getHits(G4Step * aStep) {

  std::vector<double> edeps;
  hits.clear();

  G4StepPoint * preStepPoint  = aStep->GetPreStepPoint(); 
  G4Track *     track    = aStep->GetTrack();   
  G4ThreeVector hitPoint = preStepPoint->GetPosition();   
  G4String      partType = track->GetDefinition()->GetParticleName();

  double pin    = (preStepPoint->GetTotalEnergy())/GeV;
  double zint   = hitPoint.z(); 
  double zz     = std::abs(zint) - gpar[4];
  
  LogDebug("HFShower") << "HFShowerParam: getHits " << partType
		       << " of energy " << pin << " GeV" 
                       << " Pos x,y,z = " << hitPoint.x() << "," 
		       << hitPoint.y() << "," << zint << " (" << zz << ")";

  HFShowerParam::Hit hit;
  hit.position = hitPoint;
  // take only e+-/gamma
  if (partType == "e-" || partType == "e+" || partType == "gamma" ) {
    // Leave out the last part
    double edep = 0.;
    if (zz < (gpar[1]-gpar[2])) edep = pin;
    else                        edep = (aStep->GetTotalEnergyDeposit())/GeV;
    if (edep > 0) {
      edep         *= 0.5*pePerGeV;
      double tSlice = (aStep->GetPostStepPoint()->GetGlobalTime());
      double zFibre = gpar[1]-zz;
      if (zFibre < 0) zFibre = 0;
      double time = zFibre / cFibre; // remaining part

      hit.depth = 1;
      hit.time  = tSlice+time;
      edeps.push_back(edep);
      hits.push_back(hit);
      if (zz >= gpar[0]) {
	edeps.push_back(edep);
	hit.depth = 2;
	hits.push_back(hit);
      }
      track->SetTrackStatus(fStopAndKill);
      LogDebug("HFShower") << "HFShowerParam: getHits kill track " 
			   << track->GetTrackID() << " and deposit "
			   << edep << " " << edeps.size() << " times";
    }
  }
    
  return edeps;

}
G4ThreeVector HFShowerParam::getPosHit(int i) {

  G4ThreeVector pos;
  if (i < static_cast<int>(hits.size())) pos = (hits[i].position);
  LogDebug("HFShower") << "HFShowerParam::getPosHit (" << i << "/" 
		       << hits.size() << ") " << pos;
  return pos;
}

int HFShowerParam::getDepth(int i) {

  int depth = 0;
  if (i < static_cast<int>(hits.size())) depth = (hits[i].depth);
  LogDebug("HFShower") << "HFShowerParam::getDepth (" << i << "/" 
		       << hits.size() << ") "  << depth;
  return depth;
}
 
double HFShowerParam::getTSlice(int i) {
   
  double tim = 0.;
  if (i < static_cast<int>(hits.size())) tim = (hits[i].time);
  LogDebug("HFShower") << "HFShowerParam::getTSlice (" << i << "/" 
		       << hits.size()<< ") " << tim;
  return tim;
}

std::vector<double> HFShowerParam::getDDDArray(const std::string & str, 
					       const DDsvalues_type & sv) {

  LogDebug("HFShower") << "HFShowerParam:getDDDArray called for " << str;

  DDValue value(str);
  if (DDfetch(&sv,value)) {
    LogDebug("HFShower") << value;
    const std::vector<double> & fvec = value.doubles();
    int nval = fvec.size();
    if (nval < 2) {
      edm::LogError("HFShower") << "HFShowerParam : # of " << str 
				<< " bins " << nval << " < 2 ==> illegal";
      throw cms::Exception("Unknown", "HFShowerParam")
	<< "nval < 2 for array " << str << "\n";
    }

    return fvec;
  } else {
    edm::LogError("HFShower") << "HFShowerParam : cannot get array " << str;
    throw cms::Exception("Unknown", "HFShowerParam") 
      << "cannot get array " << str << "\n";
  }
}
