///////////////////////////////////////////////////////////////////////////////
// File:  HFCherenkov.cc
// Description: Generate Cherenkov photons for HF
///////////////////////////////////////////////////////////////////////////////

#include "SimG4CMS/Calo/interface/HFCherenkov.h"

#include "G4Poisson.hh"
#include "G4ParticleDefinition.hh"

//#define DebugLog

HFCherenkov::HFCherenkov(edm::ParameterSet const & m_HF) {

  //Simple configurables
  //static SimpleConfigurable<double> p1(1.459, "HFCherenkov:RefIndex");
  //static SimpleConfigurable<double> p2(280.0, "HFCherenkov:Lambda1");
  //static SimpleConfigurable<double> p3(700.0, "HFCherenkov:Lambda2");
  //static SimpleConfigurable<double> p4(0.33,  "HFCherenkov:Aperture");
  //static SimpleConfigurable<double> p5(0.22,  "HFCherenkov:ApertureTrapped");
  //static SimpleConfigurable<double> p6(0.33,  "HFCherenkov:Gain");
  //static SimpleConfigurable<bool>   p7(false, "HFCherenkov:CheckSurvive");

  ref_index    = m_HF.getParameter<double>("RefIndex");
  lambda1      = ((m_HF.getParameter<double>("Lambda1"))/pow(double(10),7))*cm;
  lambda2      = ((m_HF.getParameter<double>("Lambda2"))/pow(double(10),7))*cm;
  aperture     = cos(asin(m_HF.getParameter<double>("Aperture")));
  apertureTrap = cos(asin(m_HF.getParameter<double>("ApertureTrapped")));
  gain         = m_HF.getParameter<double>("Gain");
  checkSurvive = m_HF.getParameter<bool>("CheckSurvive");

  edm::LogInfo("HFShower") << "HFCherenkov:: initialised with ref_index " 
			   << ref_index << " lambda1/lambda2 (cm) " 
			   << lambda1/cm << "/" << lambda2/cm
			   << " aperture(total/trapped) " << aperture << "/"
			   << apertureTrap << " Check photon survival in HF " 
			   << checkSurvive << " Gain " << gain;

  clearVectors();
}

HFCherenkov::~HFCherenkov() {}

int HFCherenkov::computeNPhTrapped(double pBeta, 
                                   double u, double v, double w, 
				   double step_length, double zFiber,
				   double dose, int npe_Dose) {

  if (pBeta < (1/ref_index) || step_length < 0.0001) {return 0;}

  double uv = sqrt(u*u + v*v);
  int nbOfPhotons = computeNbOfPhotons(pBeta, step_length);

  if (nbOfPhotons < 0) {
    return 0;
  } else if (nbOfPhotons > 0) {
    double u_ph, v_ph, w_ph=0;
    for (int i = 0; i < nbOfPhotons; i++) {
      double rand     = G4UniformRand();
      double theta_C  = acos(1./(pBeta*ref_index));
      double phi_C    = 2*M_PI*rand;
      double sinTheta = sin(theta_C);
      double cosTheta = cos(theta_C);
      double sinPhi   = sin(phi_C);
      double cosPhi   = cos(phi_C);
      //photon momentum
      if (uv < 0.001) { // aligned with z-axis
	u_ph = sinTheta * cosPhi;
	v_ph = sinTheta * sinPhi;
	w_ph = cosTheta;
      } else { // general case
	u_ph = u * cosTheta  + sinTheta * (v*sinPhi + u*w*cosPhi)/ uv;
	v_ph = v * cosTheta  + sinTheta * (-u*sinPhi + v*w*cosPhi)/ uv;
	w_ph = w * cosTheta  - sinTheta * cosPhi * uv;
      }
      if (w_ph > apertureTrap) { // phton trapped inside fiber
	npe_Dose += 1; 
      }
    }
  }
  int n_photons = npe_Dose;
  return n_photons;
}

int HFCherenkov::computeNPE(G4ParticleDefinition* pDef, double pBeta,
                            double u, double v, double w,
			    double step_length, double zFiber,
			    double dose, int npe_Dose) {

  clearVectors();
  if (!isApplicable(pDef)) {return 0;}
  if (pBeta < (1/ref_index) || step_length < 0.0001) {
#ifdef DebugLog
    LogDebug("HFShower") << "HFCherenkov::computeNPE: pBeta " << pBeta 
			 << " 1/mu " << (1/ref_index) << " step_length " 
			 << step_length;
#endif
    return 0;
  }
   
  double uv = sqrt(u*u + v*v);
  int nbOfPhotons = computeNbOfPhotons(pBeta, step_length);
#ifdef DebugLog
  LogDebug("HFShower") << "HFCherenkov::computeNPE: pBeta " << pBeta 
		       << " u/v/w " << u << "/" << v << "/" << w 
		       << " step_length " << step_length << " zFib " << zFiber 
		       << " nbOfPhotons " << nbOfPhotons;
#endif
  if (nbOfPhotons < 0) {
    return 0;
  } else if (nbOfPhotons > 0) {
    double u_ph, v_ph, w_ph=0;
    for (int i = 0; i < nbOfPhotons; i++) {
      double rand     = G4UniformRand();
      double theta_C  = acos(1./(pBeta*ref_index));
      double phi_C    = 2*M_PI*rand;
      double sinTheta = sin(theta_C);
      double cosTheta = cos(theta_C);
      double sinPhi   = sin(phi_C);
      double cosPhi   = cos(phi_C);
      //photon momentum
      if (uv < 0.001) { // aligned with z-axis
	u_ph = sinTheta * cosPhi;
	v_ph = sinTheta * sinPhi;
	w_ph = cosTheta;
      } else { // general case
	u_ph = u * cosTheta  + sinTheta * (v*sinPhi + u*w*cosPhi)/ uv;
	v_ph = v * cosTheta  + sinTheta * (-u*sinPhi + v*w*cosPhi)/ uv;
	w_ph = w * cosTheta - sinTheta * cosPhi * uv;
      }
      double r_lambda = G4UniformRand();
      double lambda0 = (lambda1 * lambda2) / (lambda2 - r_lambda *
					      (lambda2 - lambda1));
      double lambda  = (lambda0/cm) * pow(double(10),7); // lambda is in nm
      wlini.push_back(lambda);
#ifdef DebugLog
      LogDebug("HFShower") << "HFCherenkov::computeNPE: " << i << " lambda "
			   << lambda << " w_ph " << w_ph << " aperture "
			   << aperture;
#endif
      if (w_ph > aperture) { // phton trapped inside fiber
	wltrap.push_back(lambda);
	double prob_HF  = 1.0; //photon survived in HF
	double a0_inv   = 0.1234;  //meter^-1
	double prob_MX  = exp( - 0.5 * a0_inv ); //light mixer
	if (checkSurvive) {
	  double a_inv = a0_inv + 0.14 * pow(dose,0.30);
	  double z_meters = zFiber;
	  prob_HF  = exp(-z_meters * a_inv ); //photon survived in HF
	}
	rand = G4UniformRand();
#ifdef DebugLog
	LogDebug("HFShower") << "HFCherenkov::computeNPE: probHF " << prob_HF
			     << " prob_MX " << prob_MX << " Random " << rand 
			     << " Survive? " << (rand < (prob_HF * prob_MX));
#endif
	if (rand < (prob_HF * prob_MX)) { // survived and sent to light mixer
	  wlatten.push_back(lambda);
	  rand = G4UniformRand();
	  double effHEM = computeHEMEff(lambda);
#ifdef DebugLog
	  LogDebug("HFShower") << "HFCherenkov::computeNPE: w_ph " << w_ph 
			       << " effHEM " << effHEM << " Random " << rand 
			       << " Survive? " << (w_ph>0.997||(rand<effHEM));
#endif
	  if (w_ph>0.997 || (rand<effHEM)) { // survived HEM
	    wlhem.push_back(lambda);
	    double qEffic = computeQEff(lambda);
	    rand = G4UniformRand();
#ifdef DebugLog
	    LogDebug("HFShower") << "HFCherenkov::computeNPE: qEffic "
				 << qEffic << " Random " << rand
				 << " Survive? " <<(rand < qEffic);
#endif
	    if (rand < qEffic) { // made photoelectron
	      npe_Dose += 1;
	      momZ.push_back(w_ph);
	      wl.push_back(lambda);
	      wlqeff.push_back(lambda);
	    } // made pe
	  } // passed HEM
	} // passed fiber
      } // end of  if(w_ph < w_aperture), trapped inside fiber
    }// end of ++NbOfPhotons
  } // end of if(NbOfPhotons)}
  int npe =  npe_Dose; // Nb of photoelectrons
#ifdef DebugLog
  LogDebug("HFShower") << "HFCherenkov::computeNPE: npe " << npe;
#endif
  return npe;
}

int HFCherenkov::computeNPEinPMT(G4ParticleDefinition* pDef, double pBeta, 
                                 double u, double v, double w, 
                                 double step_length){
  clearVectors();
  int npe_ = 0;
  if (!isApplicable(pDef)) {return 0;}
  if (pBeta < (1/ref_index) || step_length < 0.0001) {
#ifdef DebugLog
    LogDebug("HFShower") << "HFCherenkov::computeNPEinPMT: pBeta " << pBeta 
			 << " 1/mu " << (1/ref_index) << " step_length " 
			 << step_length;
#endif
    return 0;
  }
   
  double uv = sqrt(u*u + v*v);
  int nbOfPhotons = computeNbOfPhotons(pBeta, step_length);
#ifdef DebugLog
  LogDebug("HFShower") << "HFCherenkov::computeNPEinPMT: pBeta " << pBeta 
		       << " u/v/w " << u << "/" << v << "/" << w 
		       << " step_length " << step_length  
		       << " nbOfPhotons " << nbOfPhotons;
#endif   
  if (nbOfPhotons < 0) {
    return 0;
  } else if (nbOfPhotons > 0) {
    double u_ph, v_ph, w_ph=0;
    for (int i = 0; i < nbOfPhotons; i++) {
      double rand     = G4UniformRand();
      double theta_C  = acos(1./(pBeta*ref_index));
      double phi_C    = 2*M_PI*rand;
      double sinTheta = sin(theta_C);
      double cosTheta = cos(theta_C);
      double sinPhi   = sin(phi_C);
      double cosPhi   = cos(phi_C); 
      //photon momentum
      if (uv < 0.001) { // aligned with z-axis
	u_ph = sinTheta * cosPhi;
	v_ph = sinTheta * sinPhi;
	w_ph = cosTheta;
      } else { // general case
	u_ph = u * cosTheta  + sinTheta * (v*sinPhi + u*w*cosPhi)/ uv;
	v_ph = v * cosTheta  + sinTheta * (-u*sinPhi + v*w*cosPhi)/ uv;
	w_ph = w * cosTheta - sinTheta * cosPhi * uv;
      }
      double r_lambda = G4UniformRand();
      double lambda0 = (lambda1 * lambda2) / (lambda2 - r_lambda *
					      (lambda2 - lambda1));
      double lambda  = (lambda0/cm) * pow(double(10),7); // lambda is in nm
      wlini.push_back(lambda);
#ifdef DebugLog
      LogDebug("HFShower") << "HFCherenkov::computeNPEinPMT: " <<i <<" lambda "
			   << lambda << " w_ph " << w_ph << " aperture " 
			   << aperture;
#endif
      if (w_ph > aperture) { // phton trapped inside PMT glass
	wltrap.push_back(lambda);
	rand = G4UniformRand();
#ifdef DebugLog
	LogDebug("HFShower") << "HFCherenkov::computeNPEinPMT: Random " << rand
			     << " Survive? " << (rand < 1.);
#endif
	if (rand < 1.0) { // survived all the times and sent to photo-cathode
	  wlatten.push_back(lambda);
	  rand = G4UniformRand();
	  double qEffic = computeQEff(lambda);//Quantum efficiency of the PMT
	  rand = G4UniformRand();
#ifdef DebugLog
	  LogDebug("HFShower") << "HFCherenkov::computeNPEinPMT: qEffic " 
		               << qEffic << " Random " << rand 
			       << " Survive? " <<(rand < qEffic);
#endif
	  if (rand < qEffic) { // made photoelectron
	    npe_ += 1;
	    momZ.push_back(w_ph);
	    wl.push_back(lambda);
	    wlqeff.push_back(lambda);
	  } // made pe
	} // accepted all Cherenkov photons
      } // end of  if(w_ph < w_aperture), trapped inside glass
    }// end of ++NbOfPhotons
  } // end of if(NbOfPhotons)}
#ifdef DebugLog
  LogDebug("HFShower") << "HFCherenkov::computeNPEinPMT: npe " << npe_;
#endif
  return npe_;
}

std::vector<double>  HFCherenkov::getWLIni() {
  std::vector<double> v = wlini;
  return v;
}

std::vector<double>  HFCherenkov::getWLTrap() {
  std::vector<double> v = wltrap;
  return v;
}

std::vector<double>  HFCherenkov::getWLAtten() {
  std::vector<double> v = wlatten;
  return v;
}

std::vector<double>  HFCherenkov::getWLHEM() {
  std::vector<double> v  = wlhem;
  return v;
}

std::vector<double>  HFCherenkov::getWLQEff() {
  std::vector<double> v = wlqeff;
  return v;
}

std::vector<double>  HFCherenkov::getWL() {
  std::vector<double> v = wl;
  return v;
}

std::vector<double>  HFCherenkov::getMom() {
  std::vector<double> v = momZ;
  return v;
}

int HFCherenkov::computeNbOfPhotons(double beta, G4double stepL) {

  double pBeta = beta;
  double alpha = 0.0073;
  double step_length = stepL;
  double theta_C = acos(1./(pBeta*ref_index));
  double lambdaDiff = (1./lambda1 - 1./lambda2);
  double cherenPhPerLength = 2 * M_PI * alpha * lambdaDiff*cm;
  double d_NOfPhotons = cherenPhPerLength * sin(theta_C)*sin(theta_C) *  (step_length/cm);
  int nbOfPhotons = int(d_NOfPhotons);
#ifdef DebugLog
  LogDebug("HFShower") << "HFCherenkov::computeNbOfPhotons: StepLength " 
		       << step_length << " theta_C " << theta_C 
		       << " lambdaDiff " << lambdaDiff
		       << " cherenPhPerLength " << cherenPhPerLength 
		       << " Photons " << d_NOfPhotons << " " << nbOfPhotons;
#endif
  return nbOfPhotons;
}

double HFCherenkov::computeQEff(double wavelength) {

  double y        = (wavelength - 275.) /180.;
  double func     = 1. / (1. + 250.*pow((y/5.),4));
  double qE_R7525 = 0.77 * y * exp(-y) * func ;
  double qeff     = qE_R7525;
#ifdef DebugLog
  LogDebug("HFShower") << "HFCherenkov::computeQEff: wavelength " << wavelength
		       << " y/func " << y << "/" << func << " qeff " << qeff;
#endif
  return qeff;
}

double HFCherenkov::computeHEMEff(double wavelength) {

  double hEMEff = 0;
  if (wavelength < 400.) {
    hEMEff = 0.;
  } else if (wavelength >= 400. && wavelength < 410.) {
    //hEMEff = .99 * (wavelength - 400.) / 10.;
    hEMEff = (-1322.453 / wavelength) + 4.2056;
  } else if (wavelength >= 410.) {
    hEMEff = 0.99;
    if (wavelength > 415. && wavelength < 445.) {
      //abs(wavelength - 430.) < 15.
      //hEMEff = 0.95;
      hEMEff = 0.97;
    } else if (wavelength > 550. && wavelength < 600.) {
      // abs(wavelength - 575.) < 25.)
      //hEMEff = 0.96;
      hEMEff = 0.98;
    } else if (wavelength > 565. && wavelength <= 635.) { // added later
      // abs(wavelength - 600.) < 35.)
      hEMEff = (701.7268 / wavelength) - 0.186;
    }
  }
#ifdef DebugLog
  LogDebug("HFShower") << "HFCherenkov::computeHEMEff: wavelength "
		       << wavelength << " hEMEff " << hEMEff;
#endif
  return hEMEff;
}

double HFCherenkov::smearNPE(int npe) {

  double pe = 0.;
  if (npe > 0) {
    for (int i = 0; i < npe; ++i) {
      double val =  G4Poisson(gain);
      pe += (val/gain) + 0.001*G4UniformRand();
    }
  }
#ifdef DebugLog
  LogDebug("HFShower") << "HFCherenkov::smearNPE: npe " << npe << " pe " << pe;
#endif
  return pe;
}

void HFCherenkov::clearVectors() {

  wl.clear();
  wlini.clear();
  wltrap.clear();
  wlatten.clear();
  wlhem.clear();
  wlqeff.clear();
  momZ.clear();
}

bool HFCherenkov::isApplicable(const G4ParticleDefinition* aParticleType) {
  bool tmp = (aParticleType->GetPDGCharge() != 0);
#ifdef DebugLog
  LogDebug("HFShower") << "HFCherenkov::isApplicable: aParticleType " 
		       << aParticleType->GetParticleName() << " PDGCharge " 
		       << aParticleType->GetPDGCharge() << " Result " << tmp;
#endif
  return tmp;
}
