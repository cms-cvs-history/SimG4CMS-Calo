///////////////////////////////////////////////////////////////////////////////
// File: HFShowerParam.cc
// Description: Parametrized version of HF hits
///////////////////////////////////////////////////////////////////////////////

#include "SimG4CMS/Calo/interface/HFShowerParam.h"
#include "DetectorDescription/Core/interface/DDFilter.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "G4VPhysicalVolume.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4NavigationHistory.hh"
#include "Randomize.hh"

#include "CLHEP/Units/GlobalPhysicalConstants.h"
#include "CLHEP/Units/GlobalSystemOfUnits.h"

#define DebugLog

HFShowerParam::HFShowerParam(std::string & name, const DDCompactView & cpv,
			     edm::ParameterSet const & p) : showerLibrary(0),
							    fibre(0), 
							    gflash(0),
							    fillHisto(false) {

  edm::ParameterSet m_HF  = p.getParameter<edm::ParameterSet>("HFShower");
  pePerGeV                = m_HF.getParameter<double>("PEPerGeV");
  trackEM                 = m_HF.getParameter<bool>("TrackEM");
  bool useShowerLibrary   = m_HF.getParameter<bool>("UseShowerLibrary");
  bool useGflash          = m_HF.getParameter<bool>("UseHFGflash");
  edMin                   = m_HF.getParameter<double>("EminLibrary");
  onlyLong                = m_HF.getParameter<bool>("OnlyLong");
  ref_index               = m_HF.getParameter<double>("RefIndex");
  parametrizeLast         = m_HF.getUntrackedParameter<bool>("ParametrizeLast",false);
  edm::LogInfo("HFShower") << "HFShowerParam::Use of shower library is set to "
			   << useShowerLibrary << " Use of Gflash is set to "
			   << useGflash << " P.E. per GeV " << pePerGeV
			   << ", ref. index of fibre " << ref_index 
			   << ", Track EM Flag " << trackEM << ", edMin "
			   << edMin << " GeV, use of Short fibre info in"
			   << " shower library set to " << !(onlyLong)
                           << ", use of parametrization for last part set to "
                           << parametrizeLast;

#ifdef DebugLog
  edm::Service<TFileService> tfile;
  if (tfile.isAvailable()) {
    fillHisto = true;
    edm::LogInfo("HFShower") << "HFShowerParam::Save histos in directory "
			     << "ProfileFromParam";
    TFileDirectory showerDir = tfile->mkdir("ProfileFromParam");
    em_2d         = showerDir.make<TH2F>("em_2d","Lateral Profile vs. Shower Depth;Radiation Length;Moliere Radius",800,800.0,1600.0,100,0.0,5.0);
    em_long       = showerDir.make<TH1F>("em_long","Longitudinal Profile;Radiation Length;Number of Spots",800,800.0,1600.0);
    em_lateral    = showerDir.make<TH1F>("em_lateral","Lateral Profile;Radiation Length;Moliere Radius",100,0.0,5.0);
  } else {
    fillHisto = false;
    edm::LogInfo("HFShower") << "HFShowerParam::No file is available for "
			     << "saving histos so the flag is set to false";
  }
#endif
  
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
  
  if (useShowerLibrary) showerLibrary = new HFShowerLibrary(name, cpv, p);
  if (useGflash)        gflash        = new HFGflash(p);
  fibre = new HFFibre(name, cpv, p);
}

HFShowerParam::~HFShowerParam() {
  if (fibre)         delete fibre;
  if (gflash)        delete gflash;
  if (showerLibrary) delete showerLibrary;
}

void HFShowerParam::initRun(G4ParticleTable * theParticleTable) {

  emPDG = theParticleTable->FindParticle("e-")->GetPDGEncoding();
  epPDG = theParticleTable->FindParticle("e+")->GetPDGEncoding();
  gammaPDG = theParticleTable->FindParticle("gamma")->GetPDGEncoding();
#ifdef DebugLog
  LogDebug("HFShower") << "HFShowerParam: Particle code for e- = " << emPDG
		       << " for e+ = " << epPDG << " for gamma = " << gammaPDG;
#endif
  if (showerLibrary) showerLibrary->initRun(theParticleTable);
}

std::vector<HFShowerParam::Hit> HFShowerParam::getHits(G4Step * aStep) {

  G4StepPoint * preStepPoint  = aStep->GetPreStepPoint(); 
  G4Track *     track    = aStep->GetTrack();   
  G4ThreeVector hitPoint = preStepPoint->GetPosition();   
  G4int         particleCode = track->GetDefinition()->GetPDGEncoding();
  double        zv = std::abs(hitPoint.z()) - gpar[4] - 0.5*gpar[1];
  G4ThreeVector localPoint = G4ThreeVector(hitPoint.x(),hitPoint.y(),zv);

  double pin    = (preStepPoint->GetTotalEnergy())/GeV;
  double zint   = hitPoint.z(); 
  double zz     = std::abs(zint) - gpar[4];

#ifdef DebugLog
  LogDebug("HFShower") << "HFShowerParam: getHits " 
		       << track->GetDefinition()->GetParticleName()
		       << " of energy " << pin << " GeV" 
                       << " Pos x,y,z = " << hitPoint.x() << "," 
		       << hitPoint.y() << "," << zint << " (" << zz << ","
		       << localPoint.z() << ", " <<(localPoint.z()+0.5*gpar[1])
		       << ") Local " << localPoint;
#endif
  std::vector<HFShowerParam::Hit> hits;
  HFShowerParam::Hit hit;
  hit.position = hitPoint;

  // look for other charged particles
  bool   other = false;
  double pBeta = track->GetDynamicParticle()->GetTotalMomentum() / track->GetDynamicParticle()->GetTotalEnergy();
  if (particleCode != emPDG && particleCode != epPDG && particleCode != gammaPDG ) {
    if (track->GetDefinition()->GetPDGCharge() != 0 && pBeta > (1/ref_index) &&
	aStep->GetTotalEnergyDeposit() > 0) other = true;
  }

  // take only e+-/gamma/or special particles
  if (particleCode == emPDG || particleCode == epPDG ||
      particleCode == gammaPDG || other) {
    // Leave out the last part
    double edep = 0.;
    bool   kill = false;
    if ((!trackEM) && ((zz<(gpar[1]-gpar[2])) || parametrizeLast) && (!other)){
      edep = pin;
      kill = true;
    } else if (track->GetDefinition()->GetPDGCharge() != 0 && 
	       pBeta > (1/ref_index)) {
      edep = (aStep->GetTotalEnergyDeposit())/GeV;
    }
    std::string path = "ShowerLibrary";
    if (edep > 0) {
      if ((showerLibrary || gflash) && kill && pin > edMin && (!other)) {
	if (showerLibrary) {
	  std::vector<HFShowerLibrary::Hit> hitSL = showerLibrary->getHits(aStep,kill, onlyLong);
	  for (unsigned int i=0; i<hitSL.size(); i++) {
	    hit.position = hitSL[i].position;
	    hit.depth    = hitSL[i].depth;
	    hit.time     = hitSL[i].time;
	    hit.edep     = 1;
	    hits.push_back(hit);
#ifdef DebugLog

	    if (fillHisto) {
	      em_2d->Fill(hit.position.z()/cm, hit.position.r()/cm);
	      em_lateral->Fill(hit.position.r()/cm);
	      em_long->Fill(hit.position.z()/cm);
	    }

	    LogDebug("HFShower") << "HFShowerParam: Hit at depth " << hit.depth
				 << " with edep " << hit.edep << " Time " 
				 << hit.time;
#endif
	  }
	} else {
	  std::vector<HFGflash::Hit>  hitSL = gflash->gfParameterization(aStep,kill, onlyLong);
	  for (unsigned int i=0; i<hitSL.size(); i++) {
	    bool ok = true;
	    double zv  = std::abs(hitSL[i].position.z()) - gpar[4];
	    //depth
	    int depth    = 1;
	    if (G4UniformRand() > 0.5) depth = 2;
	    if (zv < 0 || zv > gpar[1])     ok = false;
	    if (depth == 2 && zv < gpar[0]) ok = false;
	    if (ok) {
	      //attenuation
	      double time = fibre->tShift(localPoint,depth,0); //remaining part

	      hit.position = hitSL[i].position;
	      hit.depth    = depth;
	      hit.time     = time + hitSL[i].time;
	      hit.edep     = 1;
	      hits.push_back(hit);
#ifdef DebugLog

	    if (fillHisto) {
	      em_2d->Fill(hit.position.z()/cm, hit.position.r()/cm);
	      em_lateral->Fill(hit.position.r()/cm);
	      em_long->Fill(hit.position.z()/cm);
	    }

	      LogDebug("HFShower") << "HFShowerParam: Hit at depth " << hit.depth
				   << " with edep " << hit.edep << " Time " 
				   << hit.time;
#endif
	    }
	  }
	}
      } else {
	path          = "Rest";
	edep         *= pePerGeV;
	double tSlice = (aStep->GetPostStepPoint()->GetGlobalTime());
	double time = fibre->tShift(localPoint,1,0); // remaining part
	hit.depth   = 1;
	hit.time    = tSlice+time;
	hit.edep    = edep;
	hits.push_back(hit);
#ifdef DebugLog
	LogDebug("HFShower") << "HFShowerParam: Hit at depth 1 with edep " 
			     << edep << " Time " << tSlice << ":" << time
			     << ":" << hit.time;
#endif
	if (zz >= gpar[0]) {
	  time      = fibre->tShift(localPoint,2,0);
	  hit.depth = 2;
	  hit.time  = tSlice+time;
	  hits.push_back(hit);
#ifdef DebugLog
	  LogDebug("HFShower") << "HFShowerParam: Hit at depth 2 with edep " 
			       << edep << " Time " << tSlice << ":" << time
			       << hit.time;
#endif
	}
      }
      for (unsigned int ii=0; ii<hits.size(); ii++) {
	double zv = std::abs(hits[ii].position.z());
	if (zv > 12790)
	edm::LogInfo("HFShower") << "HFShowerParam: Abnormal hit along " <<path
				 << " in " << preStepPoint->GetPhysicalVolume()->GetLogicalVolume()->GetName()
				 << " at " << hits[ii].position << " zz " 
				 << zv << " Edep " << edep << " due to " 
				 << track->GetDefinition()->GetParticleName()
				 << " time " << hit.time;
      }
      if (kill) {
	track->SetTrackStatus(fStopAndKill);
	G4TrackVector tv = *(aStep->GetSecondary());
	for (unsigned int kk=0; kk<tv.size(); kk++) {
	  if (tv[kk]->GetVolume() == preStepPoint->GetPhysicalVolume())
	    tv[kk]->SetTrackStatus(fStopAndKill);
	}
      }
#ifdef DebugLog
      LogDebug("HFShower") << "HFShowerParam: getHits kill (" << kill
			   << ") track " << track->GetTrackID() 
			   << " at " << hitPoint
			   << " and deposit " << edep << " " << hits.size()
			   << " times" << " ZZ " << zz << " " << gpar[0];
#endif
    }
  }
    
  return hits;
}

std::vector<double> HFShowerParam::getDDDArray(const std::string & str, 
					       const DDsvalues_type & sv) {

#ifdef DebugLog
  LogDebug("HFShower") << "HFShowerParam:getDDDArray called for " << str;
#endif
  DDValue value(str);
  if (DDfetch(&sv,value)) {
#ifdef DebugLog
    LogDebug("HFShower") << value;
#endif
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
