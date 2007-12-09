#ifndef SimG4CMS_HFShowerLibrary_h
#define SimG4CMS_HFShowerLibrary_h 1
///////////////////////////////////////////////////////////////////////////////
// File: HFShowerLibrary.h
// Description: Gets information from a shower library
///////////////////////////////////////////////////////////////////////////////

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimG4CMS/Calo/interface/HFFibre.h"
#include "SimDataFormats/CaloHit/interface/HFShowerPhoton.h"
#include "DetectorDescription/Core/interface/DDsvalues.h"

#include "G4ParticleTable.hh"
#include "G4ThreeVector.hh"
 
//ROOT
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include <string>
#include <memory>

class DDCompactView;    
class G4Step;

class HFShowerLibrary {
  
public:
  
  //Constructor and Destructor
  HFShowerLibrary(std::string & name, const DDCompactView & cpv,
		  edm::ParameterSet const & p);
  ~HFShowerLibrary();

public:

  void                initRun(G4ParticleTable * theParticleTable);
  int                 getHits(G4Step * aStep);
  G4ThreeVector       getPosHit(int i);
  int                 getDepth(int i);
  double              getTSlice(int i);

protected:

  bool                rInside(double r);
  void                getRecord(int, int);
  void                loadPacking(TTree *);
  void                loadEventInfo(TTree *);
  void                loadEventInfo(TBranch *);
  void                interpolate(int, double);
  void                extrapolate(int, double);
  void                storePhoton(int j);
  std::vector<double> getDDDArray(const std::string&, const DDsvalues_type&,
				  int&);

  struct Hit {
    Hit() {}
    G4ThreeVector     position;
    int               depth;
    double            time;
  };

private:

  HFFibre *           fibre;
  TFile *             hf;
  TTree               *emTree, *hadTree;
  TBranch             *emBranch, *hadBranch;

  bool                readBranch, verbose;
  int                 nMomBin, totEvents, evtPerBin;
  float               libVers, listVersion; 
  std::vector<double> pmom;

  bool                format, packXYZ;
  int                 xOffset, xMultiplier, xScale;
  int                 yOffset, yMultiplier, yScale;
  int                 zOffset, zMultiplier, zScale;

  double              probMax, backProb;
  double              dphi, rMin, rMax;
  std::vector<double> gpar;

  int                 emPDG, epPDG, gammaPDG;
  int                 pi0PDG, etaPDG, nuePDG, numuPDG, nutauPDG;
  int                 anuePDG, anumuPDG, anutauPDG, geantinoPDG;

  int                 nHit;
  std::vector<Hit>    hit;

  int                 npe;
  std::vector<HFShowerPhoton> pe;
  std::vector<HFShowerPhoton> photon;

};
#endif
