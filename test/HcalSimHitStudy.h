#ifndef SimG4CMS_HcalSimHitStudy_H
#define SimG4CMS_HcalSimHitStudy_H

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

#include "SimDataFormats/CaloHit/interface/PCaloHit.h"
#include "SimDataFormats/CaloHit/interface/PCaloHitContainer.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

class HcalSimHitStudy: public edm::EDAnalyzer{
public:

  HcalSimHitStudy(const edm::ParameterSet& ps);
  ~HcalSimHitStudy();

protected:

  void beginJob (const edm::EventSetup& c);
  void endJob   ();
  void analyze  (const edm::Event& e, const edm::EventSetup& c);

  void analyzeHits  (std::vector<PCaloHit> &);

private:

  std::string            g4Label, hcalHits, outFile_;
  bool                   verbose_, checkHit_;
  DaqMonitorBEInterface* dbe_;

  MonitorElement *meAllNHit_, *meBadDetHit_, *meBadSubHit_, *meBadIdHit_;
  MonitorElement *meHBNHit_, *meHENHit_, *meHONHit_, *meHFNHit_;
  MonitorElement *meDetectHit_, *meSubdetHit_, *meDepthHit_, *meEtaHit_;
  MonitorElement *mePhiHit_, *meEnergyHit_, *meTimeHit_, *meTimeWHit_;
  MonitorElement *meHBDepHit_, *meHEDepHit_, *meHODepHit_, *meHFDepHit_;
  MonitorElement *meHBEtaHit_, *meHEEtaHit_, *meHOEtaHit_, *meHFEtaHit_;
  MonitorElement *meHBPhiHit_, *meHEPhiHit_, *meHOPhiHit_, *meHFPhiHit_;
  MonitorElement *meHBEneHit_, *meHEEneHit_, *meHOEneHit_, *meHFEneHit_;
  MonitorElement *meHBTimHit_, *meHETimHit_, *meHOTimHit_, *meHFTimHit_;
};

#endif
