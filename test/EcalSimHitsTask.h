#ifndef EcalSimHitsTask_H
#define EcalSimHitsTask_H

/*
 * \file EcalSimHitsTask.h
 *
 * $Date: 2006/08/02 13:27:54 $
 * $Revision: 1.4 $
 * \author F. Cossutti
 *
*/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include "DQMServices/Daemon/interface/MonitorDaemon.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "SimDataFormats/EcalValidation/interface/PEcalValidInfo.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

using namespace cms;
using namespace edm;
using namespace std;

class EcalSimHitsTask: public EDAnalyzer{

    typedef map<uint32_t,float,less<uint32_t> >  MapType;

public:

/// Constructor
EcalSimHitsTask(const ParameterSet& ps);

/// Destructor
~EcalSimHitsTask();

protected:

/// Analyze
void analyze(const Event& e, const EventSetup& c);

// BeginJob
void beginJob(const EventSetup& c);

// EndJob
void endJob(void);

private:

 uint32_t getUnitWithMaxEnergy(MapType& themap);

 virtual float energyInMatrixEB(int nCellInEta, int nCellInPhi, 
                                int centralEta, int centralPhi, int centralZ,
                                MapType& themap); 
 
 virtual float energyInMatrixEE(int nCellInX, int nCellInY, 
                                int centralX, int centralY, int centralZ,
                                MapType& themap); 
 
 bool  fillEBMatrix(int nCellInEta, int nCellInPhi,
                    int CentralEta, int CentralPhi,int CentralZ,
                    MapType& fillmap, MapType&  themap);
 
 bool  fillEEMatrix(int nCellInX, int nCellInY,
                    int CentralX, int CentralY,int CentralZ,
                    MapType& fillmap, MapType&  themap);

 float eCluster2x2( MapType& themap);
 float eCluster4x4(float e33,MapType& themap);
 
 string HepMCLabel;
 string g4InfoLabel;
 string EBHitsCollection;
 string EEHitsCollection;
 string ESHitsCollection;
 string ValidationCollection;
 
 bool verbose_;
 
 DaqMonitorBEInterface* dbe_;
 
 string outputFile_;

 MonitorElement* meGunEnergy_;
 MonitorElement* meGunEta_;
 MonitorElement* meGunPhi_;   

 MonitorElement* menEBHits_;
 MonitorElement* menEEHits_;

 MonitorElement* meEBoccupancy_;
 MonitorElement* meEEoccupancyzp_;
 MonitorElement* meEEoccupancyzm_;

 MonitorElement* meEBEnergyFraction_;
 MonitorElement* meEEEnergyFraction_;
 MonitorElement* meESEnergyFraction_;

 MonitorElement* meEBLongitudinalShower_;
 MonitorElement* meEELongitudinalShower_;

 MonitorElement* meEBhitEnergy_;
 MonitorElement* meEEhitEnergy_;

 MonitorElement* meEBe1_; 
 MonitorElement* meEBe4_; 
 MonitorElement* meEBe9_; 
 MonitorElement* meEBe16_; 
 MonitorElement* meEBe25_; 

 MonitorElement* meEBe1oe4_;
 MonitorElement* meEBe4oe9_;
 MonitorElement* meEBe9oe16_;
 MonitorElement* meEBe16oe25_;
 MonitorElement* meEBe1oe25_;
 MonitorElement* meEBe9oe25_; 

 MonitorElement* meEEe1_; 
 MonitorElement* meEEe4_; 
 MonitorElement* meEEe9_; 
 MonitorElement* meEEe16_; 
 MonitorElement* meEEe25_; 

 MonitorElement* meEEe1oe4_;
 MonitorElement* meEEe4oe9_;
 MonitorElement* meEEe9oe16_;
 MonitorElement* meEEe16oe25_;
 MonitorElement* meEEe1oe25_;
 MonitorElement* meEEe9oe25_;

 MonitorElement* menESHits1zp_;
 MonitorElement* menESHits2zp_;

 MonitorElement* menESHits1zm_;
 MonitorElement* menESHits2zm_;

 MonitorElement* meESEnergyHits1zp_;
 MonitorElement* meESEnergyHits2zp_;

 MonitorElement* meESEnergyHits1zm_;
 MonitorElement* meESEnergyHits2zm_;

 MonitorElement* meEEvsESEnergyzp_;
 MonitorElement* meEEvsESEnergyzm_;

};

#endif
