#ifndef EcalSimHitsTask_H
#define EcalSimHitsTask_H

/*
 * \file EcalSimHitsTask.h
 *
 * $Date: 2007/02/16 14:07:51 $
 * $Revision: 1.7 $
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
#include "SimDataFormats/EncodedEventId/interface/EncodedEventId.h"
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"
#include "SimDataFormats/ValidationFormats/interface/PValidationFormats.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>


class EcalSimHitsTask: public edm::EDAnalyzer{

    typedef std::map<uint32_t,float,std::less<uint32_t> >  MapType;

public:

/// Constructor
EcalSimHitsTask(const edm::ParameterSet& ps);

/// Destructor
~EcalSimHitsTask();

protected:

/// Analyze
void analyze(const edm::Event& e, const edm::EventSetup& c);

// BeginJob
void beginJob(const edm::EventSetup& c);

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
 
 std::string HepMCLabel;
 std::string g4InfoLabel;
 std::string EBHitsCollection;
 std::string EEHitsCollection;
 std::string ESHitsCollection;
 std::string ValidationCollection;
 
 bool verbose_;
 
 DaqMonitorBEInterface* dbe_;
 
 std::string outputFile_;

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
