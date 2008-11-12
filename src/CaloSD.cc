///////////////////////////////////////////////////////////////////////////////
// File: CaloSD.cc
// Description: Sensitive Detector class for calorimeters
///////////////////////////////////////////////////////////////////////////////

#include "SimG4CMS/Calo/interface/CaloSD.h"
#include "SimDataFormats/SimHitMaker/interface/CaloSlaveSD.h"
#include "SimG4Core/Notification/interface/TrackInformation.h"
#include "SimG4Core/Application/interface/EventAction.h"

#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4GFlashSpot.hh"
#include "G4ParticleTable.hh"

#define DebugLog

CaloSD::CaloSD(G4String name, const DDCompactView & cpv,
	       SensitiveDetectorCatalog & clg, 
	       edm::ParameterSet const & p, const SimTrackManager* manager) : 
  SensitiveCaloDetector(name, cpv, clg, p),
  G4VGFlashSensitiveDetector(), 
  theTrack(0), preStepPoint(0), eminHit(0), m_trackManager(manager), 
  currentHit(0), hcID(-1), theHC(0){

  //Add Hcal Sentitive Detector Names

  collectionName.insert(name);

  //Parameters
  //  static SimpleConfigurable<float> pEmin(1.0,"CaloSD:EminTrack");
  //   static SimpleConfigurable<int>   pcheckHit(25,"CaloSD:CheckHits");
  //   static SimpleConfigurable<bool>  pUseMap(false,"CaloSD:UseMap");
  edm::ParameterSet m_CaloSD = p.getParameter<edm::ParameterSet>("CaloSD");
  energyCut    = m_CaloSD.getParameter<double>("EminTrack")*GeV;
  tmaxHit      = m_CaloSD.getParameter<double>("TmaxHit")*ns;
  std::vector<double> eminHits = m_CaloSD.getParameter<std::vector<double> >("EminHits");
  std::vector<std::string> hcn = m_CaloSD.getParameter<std::vector<std::string> >("HCNames");
  //  eminHit      = m_CaloSD.getParameter<double>("EminHit")*MeV;
  suppressHeavy= m_CaloSD.getParameter<bool>("SuppressHeavy");
  kmaxIon      = m_CaloSD.getParameter<double>("IonThreshold")*MeV;
  kmaxProton   = m_CaloSD.getParameter<double>("ProtonThreshold")*MeV;
  kmaxNeutron  = m_CaloSD.getParameter<double>("NeutronThreshold")*MeV;
  checkHits    = m_CaloSD.getUntrackedParameter<int>("CheckHits", 25);
  useMap       = m_CaloSD.getUntrackedParameter<bool>("UseMap", true);
  int verbn    = m_CaloSD.getUntrackedParameter<int>("Verbosity", 0);
  bool on      = m_CaloSD.getUntrackedParameter<bool>("DetailedTiming");
  corrTOFBeam  = m_CaloSD.getUntrackedParameter<bool>("CorrectTOFBeam", false);
  double beamZ = m_CaloSD.getUntrackedParameter<double>("BeamPosition",0.0)*cm;
  correctT     = beamZ/c_light/nanosecond;

  SetVerboseLevel(verbn);
  for (unsigned int k=0; k<hcn.size(); k++) {
    if (name == (G4String)(hcn[k])) {
      if (k < eminHits.size()) eminHit = eminHits[k]*MeV;
      break;
    }
  }
#ifdef DebugLog
  LogDebug("CaloSim") << "***************************************************" 
		      << "\n"
		      << "*                                                 *" 
		      << "\n"
		      << "* Constructing a CaloSD  with name " << GetName()
		      << "\n"
		      << "*                                                 *" 
		      << "\n"
		      << "***************************************************";
#endif
  slave      = new CaloSlaveSD(name);
  currentID  = CaloHitID();
  previousID = CaloHitID();

  primAncestor = 0;
  cleanIndex = 0;

  //
  // Now attach the right detectors (LogicalVolumes) to me
  //
  std::vector<std::string> lvNames = clg.logicalNames(name);
  this->Register();
  for (std::vector<std::string>::iterator it=lvNames.begin();
       it !=lvNames.end(); it++){
    this->AssignSD(*it);
#ifdef DebugLog
    LogDebug("CaloSim") << "CaloSD : Assigns SD to LV " << (*it);
#endif
  }

  // timer initialization
  if (on) {
  //     string trname("CaloSD:");
  //     theHitTimer.init( trname + name + ":hits", true);
  //   }
  //   else {
  //     theHitTimer.init( "CaloSensitiveDetector:hits", true);
  }

  edm::LogInfo("CaloSim") << "CaloSD: Minimum energy of track for saving it " 
			  << energyCut/GeV  << " GeV" << "\n"
			  << "        Use of HitID Map " << useMap << "\n"
			  << "        Check last " << checkHits 
			  << " before saving the hit\n" 
			  << "        Correct TOF globally by " << correctT
			  << " ns (Flag =" << corrTOFBeam << ")\n"
			  << "        Save hits recorded before " << tmaxHit
			  << " ns and if energy is above " << eminHit/MeV
			  << " MeV";
}

CaloSD::~CaloSD() { 
  if (slave)           delete slave; 
  if (theHC)           delete theHC;
}

bool CaloSD::ProcessHits(G4Step * aStep, G4TouchableHistory * ) {
  //  TimeMe t1( theHitTimer, false);
  
  NaNTrap( aStep ) ;
  
  if (aStep == NULL) {
    return true;
  } else {
    if (getStepInfo(aStep)) {
      if (hitExists() == false && edepositEM+edepositHAD>0.) 
	currentHit = createNewHit();
    }
  }
  return true;
} 

bool CaloSD::ProcessHits(G4GFlashSpot* aSpot, G4TouchableHistory*) {
	
  if (aSpot != NULL) {
		
    theTrack = const_cast<G4Track *>(aSpot->GetOriginatorTrack()->GetPrimaryTrack());
    G4int particleCode = theTrack->GetDefinition()->GetPDGEncoding();
    TrackInformation * trkInfo = (TrackInformation *)(theTrack->GetUserInformation());
		
    if (particleCode == emPDG ||
	particleCode == epPDG ||
	particleCode == gammaPDG ) {
      edepositEM  = aSpot->GetEnergySpot()->GetEnergy(); edepositHAD = 0.;
    } else {
      edepositEM  = 0.;  edepositHAD = 0.;
    }
	
    if (edepositEM>0.)  {
      G4Step *      fFakeStep          = new G4Step();
      G4StepPoint * fFakePreStepPoint  = fFakeStep->GetPreStepPoint();
      G4StepPoint * fFakePostStepPoint = fFakeStep->GetPostStepPoint();
      fFakePreStepPoint->SetPosition(aSpot->GetPosition());
      fFakePostStepPoint->SetPosition(aSpot->GetPosition());
      
      G4TouchableHandle fTouchableHandle   = aSpot->GetTouchableHandle();
      fFakePreStepPoint->SetTouchableHandle(fTouchableHandle);
      fFakeStep->SetTotalEnergyDeposit(aSpot->GetEnergySpot()->GetEnergy());
      
      double       time   = 0;
      unsigned int unitID = setDetUnitId(fFakeStep);
      int      primaryID;

      if (trkInfo)
	primaryID  = trkInfo->getIDonCaloSurface();
      else
	primaryID = 0;

      if (primaryID == 0) {
        edm::LogWarning("CaloSim") << "CaloSD: Spot Problem with primaryID "
				   << "**** set by force to  **** " 
				   << theTrack->GetTrackID(); 
        primaryID = theTrack->GetTrackID();
      }
      if (unitID > 0) {
	currentID.setID(unitID, time, primaryID, 0);
#ifdef DebugLog
	LogDebug("CaloSim") << "CaloSD:: GetSpotInfo for"
			    << " Unit 0x" << std::hex << currentID.unitID() 
			    << std::dec << " Edeposit = " << edepositEM << " " 
			    << edepositHAD;
#endif
	// Update if in the same detector, time-slice and for same track   
	if (currentID == previousID) {
	  updateHit(currentHit);
	} else {
        
	  posGlobal = aSpot->GetEnergySpot()->GetPosition();
	  // Reset entry point for new primary
	  if (currentID.trackID() != previousID.trackID()) {
	    entrancePoint  = aSpot->GetPosition();
	    entranceLocal  = aSpot->GetTouchableHandle()->GetHistory()->
	      GetTopTransform().TransformPoint(entrancePoint);
	    incidentEnergy = theTrack->GetKineticEnergy();
#ifdef DebugLog
	    LogDebug("CaloSim") << "CaloSD: Incident energy " 
				<< incidentEnergy/GeV << " GeV and" 
				<< " entrance point " << entrancePoint 
				<< " (Global) " << entranceLocal << " (Local)";
#endif
	  }
	
	  if (checkHit() == false) currentHit = createNewHit();
	}
      }
      
      delete 	fFakeStep;
    }
    return true;
    
  } 
  return false;
}                                   

double CaloSD::getEnergyDeposit(G4Step* aStep) {
  return aStep->GetTotalEnergyDeposit();
}

void CaloSD::Initialize(G4HCofThisEvent * HCE) { 

#ifdef DebugLog
  LogDebug("CaloSim") << "CaloSD : Initialize called for " << GetName(); 
#endif

  //This initialization is performed at the beginning of an event
  //------------------------------------------------------------
  theHC = new CaloG4HitCollection(GetName(), collectionName[0]);

  if (hcID<0) 
    hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  HCE->AddHitsCollection(hcID, theHC);
}

void CaloSD::EndOfEvent(G4HCofThisEvent* ) {

  // clean the hits for the last tracks

  cleanHitCollection();

#ifdef DebugLog
  LogDebug("CaloSim") << "CaloSD: EndofEvent entered with " << theHC->entries()
		      << " entries";
#endif
  //  TimeMe("CaloSD:sortAndMergeHits",false);

  hitvec.reserve(theHC->entries());

  // here we loop over transient hits and make them persistent
  for (int ii=0; ii<theHC->entries(); ii++) {
    CaloG4Hit* aHit = (*theHC)[ii];
#ifdef DebugLog
    LogDebug("CaloSim") << "CaloHit " << ii << " " << *aHit; 
#endif
    hitvec.push_back(aHit);
  }

#ifdef DebugLog
  LogDebug("CaloSim") << "CaloSD: EndofEvent transfer " << hitvec.size() 
		      << " hits to vector" << " for " << GetName();
#endif
}

void CaloSD::clear() {} 

void CaloSD::DrawAll() {} 

void CaloSD::PrintAll() {
#ifdef DebugLog
  LogDebug("CaloSim") << "CaloSD: Collection " << theHC->GetName();
#endif
  theHC->PrintAllHits();
} 

void CaloSD::fillHits(edm::PCaloHitContainer& c, std::string n){
  if (slave->name() == n) c=slave->hits();
}

bool CaloSD::getStepInfo(G4Step* aStep) {
  
  preStepPoint = aStep->GetPreStepPoint(); 
  theTrack     = aStep->GetTrack();   

  G4int particleCode = theTrack->GetDefinition()->GetPDGEncoding();
  if (particleCode == emPDG ||
      particleCode == epPDG ||
      particleCode == gammaPDG ) {
    edepositEM  = getEnergyDeposit(aStep); edepositHAD = 0.;
  } else {
    edepositEM  = 0.; edepositHAD = getEnergyDeposit(aStep);
  }

  double       time  = (aStep->GetPostStepPoint()->GetGlobalTime())/nanosecond;
  unsigned int unitID= setDetUnitId(aStep);
  TrackInformation * trkInfo = (TrackInformation *)(theTrack->GetUserInformation());
  int      primaryID;

  if (trkInfo)
    primaryID = trkInfo->getIDonCaloSurface();	
  else
    primaryID = 0;

#ifdef DebugLog
  if (trkInfo) LogDebug("CaloSim") << "CaloSD: hit update from track Id on Calo Surface " << trkInfo->getIDonCaloSurface();
#endif   

  if (primaryID == 0) {
    edm::LogWarning("CaloSim") << "CaloSD: Problem with primaryID **** set by "
			       << "force to TkID **** " 
			       << theTrack->GetTrackID() << " in Volume "
			       << preStepPoint->GetTouchable()->GetVolume(0)->GetName();
    primaryID = theTrack->GetTrackID();
  }

  bool flag = (unitID > 0);
  if (flag) {
    currentID.setID(unitID, time, primaryID, 0);
#ifdef DebugLog
    G4TouchableHistory* touch =(G4TouchableHistory*)(theTrack->GetTouchable());
    LogDebug("CaloSim") << "CaloSD:: GetStepInfo for"
			<< " PV "     << touch->GetVolume(0)->GetName()
			<< " PVid = " << touch->GetReplicaNumber(0)
			<< " MVid = " << touch->GetReplicaNumber(1)
			<< " Unit   " << currentID.unitID() 
			<< " Edeposit = " << edepositEM << " " << edepositHAD;
  } else {
    G4TouchableHistory* touch =(G4TouchableHistory*)(theTrack->GetTouchable());
    LogDebug("CaloSim") << "CaloSD:: GetStepInfo for"
			<< " PV "     << touch->GetVolume(0)->GetName()
			<< " PVid = " << touch->GetReplicaNumber(0)
			<< " MVid = " << touch->GetReplicaNumber(1)
			<< " Unit   " << std::hex << unitID << std::dec 
			<< " Edeposit = " << edepositEM << " " << edepositHAD;
#endif
  }
  return flag;
}

G4ThreeVector CaloSD::setToLocal(G4ThreeVector global, 
				 const G4VTouchable* touch) {

  G4ThreeVector localPoint = 
    touch->GetHistory()->GetTopTransform().TransformPoint(global);
  
  return localPoint;  
}

G4bool CaloSD::hitExists() {
   
  if (currentID.trackID()<1) {
    edm::LogWarning("CaloSim") << "***** CaloSD error: primaryID = " 
			       << currentID.trackID()
			       << " maybe detector name changed";
  }
  
  // Update if in the same detector, time-slice and for same track   
  if (currentID == previousID) {
    updateHit(currentHit);
    return true;
  }
   
  // Reset entry point for new primary
  posGlobal = preStepPoint->GetPosition();
  if (currentID.trackID() != previousID.trackID()) 
    resetForNewPrimary(preStepPoint->GetPosition(),
		       preStepPoint->GetKineticEnergy());

  return checkHit();
}

G4bool CaloSD::checkHit() {

  //look in the HitContainer whether a hit with the same ID already exists:
  bool       found = false;
  if (useMap) {
    std::map<CaloHitID,CaloG4Hit*>::const_iterator it = hitMap.find(currentID);
    if (it != hitMap.end()) {
      currentHit = it->second;
      found      = true;
    }
  } else {
    if (checkHits <= 0) return false;
    int  minhit= (theHC->entries()>checkHits ? theHC->entries()-checkHits : 0);
    int  maxhit= theHC->entries()-1;

    for (int j=maxhit; j>minhit&&!found; j--) {
      CaloG4Hit* aPreviousHit = (*theHC)[j];
      if (aPreviousHit->getID() == currentID) {
	currentHit = aPreviousHit;
	found      = true;
      }
    }          
  }

  if (found) {
    updateHit(currentHit);
    return true;
  } else {
    return false;
  }    

}

CaloG4Hit* CaloSD::createNewHit() {

#ifdef DebugLog
  LogDebug("CaloSim") << "CaloSD::CreateNewHit for"
		      << " Unit " << currentID.unitID() 
		      << " " << currentID.depth()
		      << " Edeposit = " << edepositEM << " " << edepositHAD;
  LogDebug("CaloSim") << " primary "    << currentID.trackID()
		      << " time slice " << currentID.timeSliceID()
		      << " For Track  " << theTrack->GetTrackID()
		      << " which is a " <<theTrack->GetDefinition()->GetParticleName()
		      << " of energy "  << theTrack->GetKineticEnergy()/GeV
		      << " " << theTrack->GetMomentum().mag()/GeV
		      << " daughter of part. " << theTrack->GetParentID()
		      << " and created by " ;
  
  if (theTrack->GetCreatorProcess()!=NULL)
    LogDebug("CaloSim") << theTrack->GetCreatorProcess()->GetProcessName() ;
  else 
    LogDebug("CaloSim") << "NO process";
#endif  
  
  CaloG4Hit* aHit;
  if (reusehit.size() > 0) {
    aHit = reusehit[0];
    aHit->setEM(0.);
    aHit->setHadr(0.);
    reusehit.erase(reusehit.begin());
  }  else { 
    aHit = new CaloG4Hit;
  }

  aHit->setID(currentID);
  aHit->setEntry(entrancePoint.x(),entrancePoint.y(),entrancePoint.z());
  aHit->setEntryLocal(entranceLocal.x(),entranceLocal.y(),entranceLocal.z());
  aHit->setPosition(posGlobal.x(),posGlobal.y(),posGlobal.z());
  aHit->setIncidentEnergy(incidentEnergy);
  updateHit(aHit);
  
  storeHit(aHit);
  double etrack = 0;
  if (currentID.trackID() == primIDSaved) { // The track is saved; nothing to be done
  } else if (currentID.trackID() == theTrack->GetTrackID()) {
    etrack= theTrack->GetKineticEnergy();
    if (etrack >= energyCut) {
      TrackInformation * trkInfo = 
	(TrackInformation *)(theTrack->GetUserInformation());
      trkInfo->storeTrack(true);
      trkInfo->putInHistory();
#ifdef DebugLog
      LogDebug("CaloSim") << "CaloSD: set save the track " 
			  << currentID.trackID() << " with Hit";
#endif
    }
  } else {
    TrackWithHistory * trkh = tkMap[currentID.trackID()];
#ifdef DebugLog
    LogDebug("CaloSim") << "CaloSD : TrackwithHistory pointer for " 
			<< currentID.trackID() << " is " << trkh;
#endif
    if (trkh != NULL) {
      etrack = sqrt(trkh->momentum().Mag2());
      if (etrack >= energyCut) {
	trkh->save();
#ifdef DebugLog
	LogDebug("CaloSim") << "CaloSD: set save the track " 
			    << currentID.trackID() << " with Hit";
#endif
      }
    }
  }
  primIDSaved = currentID.trackID();
  return aHit;
}	 

void CaloSD::updateHit(CaloG4Hit* aHit) {

  if (edepositEM+edepositHAD != 0) {
    aHit->addEnergyDeposit(edepositEM,edepositHAD);
#ifdef DebugLog
    LogDebug("CaloSim") << "CaloSD: Add energy deposit in " << currentID 
			<< " em " << edepositEM/MeV << " hadronic " 
			<< edepositHAD/MeV << " MeV"; 
#endif
  }

  // buffer for next steps:
  previousID = currentID;
}

void CaloSD::resetForNewPrimary(G4ThreeVector point, double energy) {
  
  entrancePoint  = point;
  entranceLocal  = setToLocal(entrancePoint, preStepPoint->GetTouchable());
  incidentEnergy = energy;
#ifdef DebugLog
  LogDebug("CaloSim") << "CaloSD: Incident energy " << incidentEnergy/GeV 
		      << " GeV and" << " entrance point " << entrancePoint 
		      << " (Global) " << entranceLocal << " (Local)";
#endif
}

double CaloSD::getAttenuation(G4Step* aStep, double birk1, double birk2,
			      double birk3) {

  double weight = 1.;
  double charge = aStep->GetPreStepPoint()->GetCharge();

  if (charge != 0. && aStep->GetStepLength() > 0) {
    G4Material* mat = aStep->GetPreStepPoint()->GetMaterial();
    double density = mat->GetDensity();
    double dedx    = aStep->GetTotalEnergyDeposit()/aStep->GetStepLength();
    double rkb     = birk1/density;
    double c       = birk2*rkb*rkb;
    if (std::abs(charge) >= 2.) rkb /= birk3; // based on alpha particle data
    weight = 1./(1.+rkb*dedx+c*dedx*dedx);
#ifdef DebugLog
    LogDebug("CaloSim") << "CaloSD::getAttenuation in " << mat->GetName() 
			<< " Charge " << charge << " dE/dx " << dedx 
			<< " Birk Const " << rkb << ", " << c << " Weight = " 
			<< weight << " dE " << aStep->GetTotalEnergyDeposit();
#endif
  }
  return weight;
}

void CaloSD::update(const BeginOfRun *) {

  G4ParticleTable * theParticleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  emPDG = theParticleTable->FindParticle(particleName="e-")->GetPDGEncoding();
  epPDG = theParticleTable->FindParticle(particleName="e+")->GetPDGEncoding();
  gammaPDG = theParticleTable->FindParticle(particleName="gamma")->GetPDGEncoding();
#ifdef DebugLog
  LogDebug("CaloSim") << "CaloSD: Particle code for e- = " << emPDG
		      << " for e+ = " << epPDG << " for gamma = " << gammaPDG;
#endif
  initRun();
} 

void CaloSD::update(const BeginOfEvent *) {
#ifdef DebugLog
  LogDebug("CaloSim")  << "CaloSD: Dispatched BeginOfEvent for " << GetName() 
		       << " !" ;
#endif
  clearHits();
  tkMap.erase (tkMap.begin(), tkMap.end());
}

void CaloSD::update(const EndOfTrack * trk) {

  int id = (*trk)()->GetTrackID();
  TrackInformation *trkI =(TrackInformation *)((*trk)()->GetUserInformation());
  int lastTrackID = -1;
  if (trkI) lastTrackID = trkI->getIDonCaloSurface();
  if (id == lastTrackID) {
    const TrackContainer * trksForThisEvent = m_trackManager->trackContainer();
    if (trksForThisEvent != NULL) {
      int it = (int)(trksForThisEvent->size()) - 1;
      if (it >= 0) {
        TrackWithHistory * trkH = (*trksForThisEvent)[it];
        if (trkH->trackID() == (unsigned int)(id)) tkMap[id] = trkH;
#ifdef DebugLog
        LogDebug("CaloSim") << "CaloSD: get track " << it << " from "
			    << "Container of size " << trksForThisEvent->size()
			    << " with ID " << trkH->trackID();
      } else {
	LogDebug("CaloSim") << "CaloSD: get track " << it << " from "
			    << "Container of size " << trksForThisEvent->size()
			    << " with no ID";
#endif
      }
    }
  }
}

void CaloSD::update(const ::EndOfEvent * ) {
  
#ifdef DebugLog
  LogDebug("CaloSim") << "CaloSD::update: Start saving hits for " << GetName()
		      << " with " << hitvec.size() << " hits";
#endif
  int kount = 0, count = 0, wrong = 0, tcut = 0;
  std::vector<CaloG4Hit*>::iterator i;
  std::pair<bool,bool> ok;

  if (useMap) {
    for (i=hitvec.begin(); i!=hitvec.end(); i++) {
      ok = saveHit(*i);
      if ((ok.second)) {
	count++;
	if (!(ok.first))  wrong++;
      } else {
	tcut++;
      }
      kount++;
    }
  } else {
    sort(hitvec.begin(), hitvec.end(), CaloG4HitLess());
#ifdef DebugLog
    LogDebug("CaloSim") << "CaloSD: EndofEvent sort the hits in buffer ";
#endif
    std::vector<CaloG4Hit*>::iterator j;
    CaloG4HitEqual equal;
    for (i=hitvec.begin(); i!=hitvec.end(); i++) {
      int jump = 0;
      for (j = i+1; j<hitvec.end() && equal(*i, *j); j++) {
	jump++;
	// merge j to i
	(**i).addEnergyDeposit(**j);
	double em = 0.;
	double eh = 0.;
	(**j).setEM(em);
	(**j).setHadr(eh);
      }

      kount++;
#ifdef DebugLog
      LogDebug("CaloSim") << "CaloSD: Merge " << jump << " hits to hit " 
			  << kount;
#endif
      ok = saveHit(*i);
      if ((ok.second)) {
	count++;
	if (!(ok.first))  wrong++;
      } else {
	tcut++;
      }
      i+=jump;
      kount += jump;
    }
  }
  
  edm::LogInfo("CaloSim") << "CaloSD: " << GetName() << " store " << count
			  << " hits out of " << kount << " recorded with " 
			  << wrong << " track IDs not given properly and "
			  << tcut << " hits with time above cutoff";
  summarize();
}

void CaloSD::clearHits() {

  hitvec.erase (hitvec.begin(), hitvec.end()); 
  hitMap.erase (hitMap.begin(), hitMap.end());
  //for (unsigned int i = 0; i<reusehit.size(); i++) { delete reusehit[i]; }
  reusehit.clear();
  cleanIndex = 0;
  previousID.reset();
  primIDSaved    = -99;
#ifdef DebugLog
  LogDebug("CaloSim") << "CaloSD: Clears hit vector for " << GetName() << " " 
		      << slave;
#endif
  slave->Initialize();
#ifdef DebugLog
  LogDebug("CaloSim") << "CaloSD: Initialises slave SD for " << GetName();
#endif
}

void CaloSD::initRun() {}

bool CaloSD::filterHit(CaloG4Hit* hit, double time) {
  return ((time <= tmaxHit) && (hit->getEnergyDeposit() > eminHit));
}

void CaloSD::storeHit(CaloG4Hit* hit) {

  if (previousID.trackID()<0) return;
  if (hit == 0) {
    edm::LogWarning("CaloSim") << "CaloSD: hit to be stored is NULL !!";
    return;
  }

  theHC->insert(hit);
  if (useMap) hitMap.insert(std::pair<CaloHitID,CaloG4Hit*>(previousID,hit));

}

std::pair<bool,bool> CaloSD::saveHit(CaloG4Hit* aHit) {

  int tkID;
  bool ok   = true;
  bool save = false;
  if (m_trackManager) {
    tkID = m_trackManager->idSavedTrack(aHit->getTrackID());
    if (tkID == 0) ok = false;
  } else {
    tkID = aHit->getTrackID();
    ok = false;
  }
#ifdef DebugLog
  LogDebug("CaloSim") << "CalosD: Track ID " << aHit->getTrackID() 
		      << " changed to " << tkID << " by SimTrackManager"
		      << " Status " << ok;
#endif
  double time = aHit->getTimeSlice();
  if (corrTOFBeam) time += correctT;
  //if (filterHit(aHit,time)) {
    save = true;
    slave->processHits(aHit->getUnitID(), aHit->getEM()/GeV, 
		       aHit->getHadr()/GeV, time, tkID, aHit->getDepth());
#ifdef DebugLog
    LogDebug("CaloSim") << "CaloSD: Store Hit at " << std::hex 
			<< aHit->getUnitID() << std::dec << " " 
			<< aHit->getDepth() << " due to " << tkID 
			<< " in time " << time << " of energy " 
			<< aHit->getEM()/GeV << " GeV (EM) and " 
			<< aHit->getHadr()/GeV << " GeV (Hadr)";
#endif
    //  }
  return std::pair<bool,bool>(ok,save);
}

void CaloSD::summarize() {}

void CaloSD::update(const BeginOfTrack * trk) {

  int primary = -1;
  TrackInformation * trkInfo = (TrackInformation *)((*trk)()->GetUserInformation());
  if ( trkInfo->isPrimary() ) { primary = (*trk)()->GetTrackID(); }

#ifdef DebugLog
  LogDebug("CaloSim") << "New track: isPrimary " << trkInfo->isPrimary() 
                      << " primary ID = " << primary 
                      << " primary ancestor ID " << primAncestor;
#endif

  // update the information if a different primary track ID 
  
  if (primary > 0 && primary != primAncestor) {
    primAncestor = primary;

    // clean the hits information
    
    if (theHC->entries()>0) {
      cleanHitCollection();
    }

  }
}

void CaloSD::cleanHitCollection() {

  if ( reusehit.size() == 0 ) { 
    reusehit.reserve(theHC->entries()); 
    itervec.reserve(theHC->entries());
  }
  std::vector<CaloG4Hit*>* theCollection = theHC->GetVector();
  int addhit = 0;
#ifdef DebugLog
  LogDebug("CaloSim") << "CaloSD: Starting reusehit filling from index = " << cleanIndex;
#endif

  for (std::vector<CaloG4Hit*>::iterator iter=(theCollection->begin()+cleanIndex); iter<theCollection->end(); iter++) {
    
    CaloG4Hit* aHit = (*iter);
    
    // selection
    
    double time = aHit->getTimeSlice();
    if (corrTOFBeam) time += correctT;
    if (!filterHit(aHit,time)) {
#ifdef DebugLog
      LogDebug("CaloSim") << "CaloSD: dropped CaloG4Hit " << " " << *aHit; 
#endif
      
      // create the list of hits to be reused
      
      reusehit.push_back(*iter);
      itervec.push_back(iter);
      addhit++;
    }
  }

#ifdef DebugLog
  LogDebug("CaloSim") << "CaloSD: Size of reusehit    = " << reusehit.size()  
                      << " Number of added hit = " << addhit;
#endif
  if ( addhit>0 ) {
    int offset = reusehit.size()-addhit;
    for (int ii = addhit-1; ii>=0; ii--) {
      if (useMap) {
        CaloHitID theID = reusehit[offset+ii]->getID();
        hitMap.erase(theID);
      }
      theCollection->erase(itervec[ii]);
    }
  }
  itervec.clear();
  cleanIndex = theHC->entries();
  
}
