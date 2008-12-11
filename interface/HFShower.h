#ifndef SimG4CMS_HFShower_h
#define SimG4CMS_HFShower_h
///////////////////////////////////////////////////////////////////////////////
// File: HFShower.h
// Description: Generates hits for HF with Cerenkov photon code
///////////////////////////////////////////////////////////////////////////////

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "SimG4CMS/Calo/interface/HFCherenkov.h"
#include "SimG4CMS/Calo/interface/HFFibre.h"

#include "G4ThreeVector.hh"
#include "G4String.hh"

class DDCompactView;    
class G4Step;

#include <vector>
 
class HFShower {

public:    

  HFShower(std::string & name, const DDCompactView & cpv, 
	   edm::ParameterSet const & p, int chk=0);
  virtual ~HFShower();

public:

  struct Hit {
    Hit() {}
    double                   time;
    double                   wavelength;
    double                   momentum;
    G4ThreeVector            position;
  };

  std::vector<Hit>           getHits(G4Step * aStep);

private:    

  std::vector<double>        getDDDArray(const std::string&, 
					 const DDsvalues_type&,  int&);

private:    

  HFCherenkov*               cherenkov;
  HFFibre*                   fibre;

  int                        chkFibre;
  double                     probMax;
  std::vector<double>        gpar;

};

#endif // HFShower_h
