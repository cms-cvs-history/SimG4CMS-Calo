/*
 * \file EcalSimHitsTask.cc
 *
 * $Date: 2006/04/10 08:20:02 $
 * $Revision: 1.13 $
 * \author F. Cossutti
 *
*/

#include <SimG4CMS/Calo/test/EcalSimHitsTask.h>
#include <DataFormats/EcalDetId/interface/EBDetId.h>
#include <DataFormats/EcalDetId/interface/EEDetId.h>
#include <DataFormats/EcalDetId/interface/ESDetId.h>

EcalSimHitsTask::EcalSimHitsTask(const ParameterSet& ps):
  HepMCLabel(ps.getUntrackedParameter("moduleLabelMC",string("PythiaSource"))),
  SimTkLabel(ps.getUntrackedParameter("moduleLabelTk",string("EmbdSimTrack"))),
  SimVtxLabel(ps.getUntrackedParameter("moduleLabelVtx",string("EmbdSimVertex"))){

 
  // DQM ROOT output
  outputFile_ = ps.getUntrackedParameter<string>("outputFile", "");
 
  if ( outputFile_.size() != 0 ) {
    LogInfo("OutputInfo") << " Ecal SimHits Task histograms will be saved to " << outputFile_.c_str();
  } else {
    LogInfo("OutputInfo") << " Ecal SimHits Task histograms will NOT be saved";
  }
 
  // verbosity switch
  verbose_ = ps.getUntrackedParameter<bool>("verbose", false);
 
  if ( verbose_ ) {
    cout << " verbose switch is ON" << endl;
  } else {
    cout << " verbose switch is OFF" << endl;
  }

  // DQMServices 
                                                          
  dbe_ = 0;

  // get hold of back-end interface

  dbe_ = Service<DaqMonitorBEInterface>().operator->();
                                                                                                                                          
  if ( dbe_ ) {
    if ( verbose_ ) {
      dbe_->setVerbose(1);
    } else {
      dbe_->setVerbose(0);
    }
  }
                                                                                                                                          
  if ( dbe_ ) {
    if ( verbose_ ) {
      sleep (3);
      dbe_->showDirStructure();
    }
  }
 
  meGunEnergy_ = 0;
  meGunEta_ = 0;   
  meGunPhi_ = 0;   

  menEBHits_ = 0;
  menEEHits_ = 0;

  meEBoccupancy_ = 0;
  meEEoccupancyzp_ = 0;
  meEEoccupancyzm_ = 0;
 
  meEBEnergyFraction_ = 0;
  meEEEnergyFraction_ = 0;
  meESEnergyFraction_ = 0;

  meEBLongitudinalShower_ = 0;
  meEELongitudinalShower_ = 0;

  meEBhitEnergy_ = 0;
  meEEhitEnergy_ = 0;

  meEBe1_ = 0;  
  meEBe4_ = 0;  
  meEBe9_ = 0;  
  meEBe16_ = 0; 
  meEBe25_ = 0; 

  meEBe1oe4_ = 0;  
  meEBe4oe9_ = 0;  
  meEBe9oe16_ = 0; 
  meEBe16oe25_ = 0;
  meEBe1oe25_ = 0; 
  meEBe9oe25_ = 0; 
                              
  meEEe1_ = 0;  
  meEEe4_ = 0;  
  meEEe9_ = 0;  
  meEEe16_ = 0; 
  meEEe25_ = 0; 

  menESHits1zp_ = 0;     
  menESHits2zp_ = 0;     

  menESHits1zm_ = 0;     
  menESHits2zm_ = 0;     
                                    
  meESEnergyHits1zp_ = 0;
  meESEnergyHits2zp_ = 0;
                                    
  meESEnergyHits1zm_ = 0;
  meESEnergyHits2zm_ = 0;

  meEEvsESEnergyzp_ = 0;
  meEEvsESEnergyzm_ = 0;

  Char_t histo[20];
 
  
  if ( dbe_ ) {
    dbe_->setCurrentFolder("EcalSimHitsTask");
  
    sprintf (histo, "EcalSimHitsTask Gun Momentum" ) ;
    meGunEnergy_ = dbe_->book1D(histo, histo, 100, 0., 1000.);
  
    sprintf (histo, "EcalSimHitsTask Gun Eta" ) ;
    meGunEta_ = dbe_->book1D(histo, histo, 700, -3.5, 3.5);
  
    sprintf (histo, "EcalSimHitsTask Gun Phi" ) ;
    meGunPhi_ = dbe_->book1D(histo, histo, 360, 0., 360.);

    sprintf (histo, "EcalSimHitsTask Barrel EB hits multiplicity" ) ;
    menEBHits_ = dbe_->book1D(histo, histo, 50, 0., 5000.) ;
 
    sprintf (histo, "EcalSimHitsTask Endcap EE hits multiplicity" ) ;
    menEEHits_ = dbe_->book1D(histo, histo, 50, 0., 5000.) ; 

    sprintf (histo, "EcalSimHitsTask Barrel occupancy" ) ;
    meEBoccupancy_ = dbe_->book2D(histo, histo, 360, 0., 360., 170, -85., 85.);
  
    sprintf (histo, "EcalSimHitsTask Endcap occupancy z+" ) ;
    meEEoccupancyzp_ = dbe_->book2D(histo, histo, 100, 0., 100., 100, 0., 100.);
  
    sprintf (histo, "EcalSimHitsTask Endcap occupancy z-" ) ;
    meEEoccupancyzm_ = dbe_->book2D(histo, histo, 100, 0., 100., 100, 0., 100.);
  
    sprintf (histo, "EcalSimHitsTask Barrel fraction of energy" ) ;
    meEBEnergyFraction_ = dbe_->book1D(histo, histo, 50 , 0.8 , 1.0);
  
    sprintf (histo, "EcalSimHitsTask Endcap fraction of energy" ) ;
    meEEEnergyFraction_ = dbe_->book1D(histo, histo, 50 , 0.8 , 1.0);
  
    sprintf (histo, "EcalSimHitsTask Preshower fraction of energy" ) ;
    meESEnergyFraction_ = dbe_->book1D(histo, histo, 50 , 0. , 0.01);

    sprintf (histo, "EcalSimHitsTask Barrel longitudinal shower profile" ) ;
    meEBLongitudinalShower_ = dbe_->bookProfile(histo, histo, 26,0,26, 100, 0, 3000);

    sprintf (histo, "EcalSimHitsTask Endcap longitudinal shower profile" ) ;
    meEELongitudinalShower_ = dbe_->bookProfile(histo, histo, 26,0,26, 100, 0, 3000);

    sprintf (histo, "EcalSimHitsTask Barrel EB hit energy spectrum" );
    meEBhitEnergy_ = dbe_->book1D(histo, histo, 4000, 0., 400.);

    sprintf (histo, "EcalSimHitsTask Endcap EE hit energy spectrum" );
    meEEhitEnergy_ = dbe_->book1D(histo, histo, 4000, 0., 400.);

    sprintf (histo, "EcalSimHitsTask Barrel E1" ) ;
    meEBe1_ = dbe_->book1D(histo, histo, 400, 0., 400.);

    sprintf (histo, "EcalSimHitsTask Barrel E4" ) ;
    meEBe4_ = dbe_->book1D(histo, histo, 400, 0., 400.);

    sprintf (histo, "EcalSimHitsTask Barrel E9" ) ;
    meEBe9_ = dbe_->book1D(histo, histo, 400, 0., 400.);

    sprintf (histo, "EcalSimHitsTask Barrel E16" ) ;
    meEBe16_ = dbe_->book1D(histo, histo, 400, 0., 400.);

    sprintf (histo, "EcalSimHitsTask Barrel E25" ) ;
    meEBe25_ = dbe_->book1D(histo, histo, 400, 0., 400.);

    sprintf (histo, "EcalSimHitsTask Barrel E1oE4" ) ;
    meEBe1oe4_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EcalSimHitsTask Barrel E4oE9" ) ;
    meEBe4oe9_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EcalSimHitsTask Barrel E9oE16" ) ;
    meEBe9oe16_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EcalSimHitsTask Barrel E16oE25" ) ;
    meEBe16oe25_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EcalSimHitsTask Barrel E1oE25" ) ;
    meEBe1oe25_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EcalSimHitsTask Barrel E9oE25" ) ;
    meEBe9oe25_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EcalSimHitsTask Endcap E1" ) ;
    meEEe1_ = dbe_->book1D(histo, histo, 400, 0., 400.);

    sprintf (histo, "EcalSimHitsTask Endcap E4" ) ;
    meEEe4_ = dbe_->book1D(histo, histo, 400, 0., 400.);

    sprintf (histo, "EcalSimHitsTask Endcap E9" ) ;
    meEEe9_ = dbe_->book1D(histo, histo, 400, 0., 400.);

    sprintf (histo, "EcalSimHitsTask Endcap E16" ) ;
    meEEe16_ = dbe_->book1D(histo, histo, 400, 0., 400.);

    sprintf (histo, "EcalSimHitsTask Endcap E25" ) ;
    meEEe25_ = dbe_->book1D(histo, histo, 400, 0., 400.);

    sprintf (histo, "EcalSimHitsTask Endcap E1oE4" ) ;
    meEEe1oe4_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EcalSimHitsTask Endcap E4oE9" ) ;
    meEEe4oe9_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EcalSimHitsTask Endcap E9oE16" ) ;
    meEEe9oe16_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EcalSimHitsTask Endcap E16oE25" ) ;
    meEEe16oe25_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EcalSimHitsTask Endcap E1oE25" ) ;
    meEEe1oe25_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EcalSimHitsTask Endcap E9oE25" ) ;
    meEEe9oe25_ = dbe_->book1D(histo, histo, 100, 0.4, 1.1);

    sprintf (histo, "EcalSimHitsTask Preshower ES hits layer 1 multiplicity z+" ) ;
    menESHits1zp_ = dbe_->book1D(histo, histo, 50, 0., 50. ) ;

    sprintf (histo, "EcalSimHitsTask Preshower ES hits layer 2 multiplicity z+" ) ;
    menESHits2zp_ = dbe_->book1D(histo, histo, 50, 0., 50. ) ;

    sprintf (histo, "EcalSimHitsTask Preshower ES hits layer 1 multiplicity z-" ) ;
    menESHits1zm_ = dbe_->book1D(histo, histo, 50, 0., 50. ) ;

    sprintf (histo, "EcalSimHitsTask Preshower ES hits layer 2 multiplicity z-" ) ;
    menESHits2zm_ = dbe_->book1D(histo, histo, 50, 0., 50. ) ;

    sprintf (histo, "EcalSimHitsTask Preshower energy ES hits layer 1 z+" ) ;
    meESEnergyHits1zp_ = dbe_->book1D(histo, histo, 100, 0., 0.001 ) ;

    sprintf (histo, "EcalSimHitsTask Preshower energy ES hits layer 2 z+" ) ;
    meESEnergyHits2zp_ = dbe_->book1D(histo, histo, 100, 0., 0.001 ) ;

    sprintf (histo, "EcalSimHitsTask Preshower energy ES hits layer 1 z-" ) ;
    meESEnergyHits1zm_ = dbe_->book1D(histo, histo, 100, 0., 0.001 ) ;

    sprintf (histo, "EcalSimHitsTask Preshower energy ES hits layer 2 z-" ) ;
    meESEnergyHits2zm_ = dbe_->book1D(histo, histo, 100, 0., 0.001 ) ;

    sprintf (histo, "EcalSimHitsTask Preshower EE vs ES energy z+" ) ;
    meEEvsESEnergyzp_ = dbe_->bookProfile(histo, histo, 150, 0., 300., 80, 0., 80.);

    sprintf (histo, "EcalSimHitsTask Preshower EE vs ES energy z-" ) ;
    meEEvsESEnergyzm_ = dbe_->bookProfile(histo, histo, 150, 0., 300., 80, 0., 80.);

  }
 
}

EcalSimHitsTask::~EcalSimHitsTask(){
 
  if ( outputFile_.size() != 0 && dbe_ ) dbe_->save(outputFile_);

}

void EcalSimHitsTask::beginJob(const EventSetup& c){

}

void EcalSimHitsTask::endJob(){

}

void EcalSimHitsTask::analyze(const Event& e, const EventSetup& c){

  LogInfo("EventInfo") << " Run = " << e.id().run() << " Event = " << e.id().event();

  vector<PCaloHit> theEBCaloHits;
  vector<PCaloHit> theEECaloHits;
  vector<PCaloHit> theESCaloHits;
  vector<EmbdSimTrack> theSimTracks;
  vector<EmbdSimVertex> theSimVertexes;

  Handle<HepMCProduct> MCEvt;
  Handle<EmbdSimTrackContainer> SimTk;
  Handle<EmbdSimVertexContainer> SimVtx;
  Handle<PCaloHitContainer> EcalHitsEB;
  Handle<PCaloHitContainer> EcalHitsEE;
  Handle<PCaloHitContainer> EcalHitsES;

  Handle<PEcalValidInfo> MyPEcalValidInfo;

  e.getByLabel(HepMCLabel, MCEvt);
  e.getByLabel(SimTkLabel,SimTk);
  e.getByLabel(SimVtxLabel,SimVtx);
  e.getByLabel("SimG4Object","EcalHitsEB",EcalHitsEB);
  e.getByLabel("SimG4Object","EcalHitsEE",EcalHitsEE);
  e.getByLabel("SimG4Object","EcalHitsES",EcalHitsES);

  bool isLongitudinal = true;
  try {
    e.getByLabel("SimG4Object","EcalValidInfo",MyPEcalValidInfo);
  } catch ( cms::Exception &e ) { isLongitudinal = false; }

  theSimTracks.insert(theSimTracks.end(),SimTk->begin(),SimTk->end());
  theSimVertexes.insert(theSimVertexes.end(),SimVtx->begin(),SimVtx->end());
  theEBCaloHits.insert(theEBCaloHits.end(), EcalHitsEB->begin(), EcalHitsEB->end());
  theEECaloHits.insert(theEECaloHits.end(), EcalHitsEE->begin(), EcalHitsEE->end());
  theESCaloHits.insert(theESCaloHits.end(), EcalHitsES->begin(), EcalHitsES->end());

  for ( HepMC::GenEvent::particle_const_iterator p = MCEvt->GetEvent()->particles_begin();
        p != MCEvt->GetEvent()->particles_end(); ++p ) {

    Hep3Vector hmom = Hep3Vector((*p)->momentum().vect());
    double htheta = hmom.theta();
    double heta = -log(tan(htheta * 0.5));
    double hphi = hmom.phi();
    hphi = (hphi>=0) ? hphi : hphi+2*M_PI;
    hphi = hphi / M_PI * 180.;

    LogDebug("EventInfo") << "Particle gun type form MC = " << abs((*p)->pdg_id()) << "\n" << "Energy = "<< (*p)->momentum().e() << " Eta = " << heta << " Phi = " << hphi;

    if (meGunEnergy_) meGunEnergy_->Fill((*p)->momentum().e());
    if (meGunEta_) meGunEta_->Fill(heta);
    if (meGunPhi_) meGunPhi_->Fill(hphi);

  }

  int nvtx = 0;
  for (vector<EmbdSimVertex>::iterator isimvtx = theSimVertexes.begin();
       isimvtx != theSimVertexes.end(); ++isimvtx){
    nvtx++;
    LogDebug("EventInfo") <<" Vertex index = " << nvtx << " vertex dump: " << *isimvtx;
  }

  for (vector<EmbdSimTrack>::iterator isimtrk = theSimTracks.begin();
       isimtrk != theSimTracks.end(); ++isimtrk){
    LogDebug("EventInfo") <<" Starting vertex index = " <<isimtrk->vertIndex() << " track dump: " << *isimtrk ; 
  }

   std::map<unsigned int, std::vector<PCaloHit>,std::less<unsigned int> > CaloHitMap;

   double eb1 = 0.0;
   double eb4 = 0.0;
   double eb9 = 0.0;
   double eb16 = 0.0;
   double eb25 = 0.0;

   MapType ebmap;
   uint32_t nEBHits = 0;
   double EBEnergy_ = 0.;

   for (std::vector<PCaloHit>::iterator isim = theEBCaloHits.begin();
	isim != theEBCaloHits.end(); ++isim){
     CaloHitMap[(*isim).id()].push_back((*isim));
     
     EBDetId ebid (isim->id()) ;

     LogDebug("HitInfo") 
       <<" CaloHit " << isim->getName() << " DetID = "<<isim->id()<< "\n"	
       << "Energy = " << isim->energy() << " Time = " << isim->time() << "\n"
       << "EBDetId = " << ebid.ieta() << " " << ebid.iphi();

     if (meEBoccupancy_) meEBoccupancy_->Fill( ebid.iphi(), ebid.ieta() );

     uint32_t crystid = ebid.rawId();
     ebmap[crystid] += isim->energy();

     EBEnergy_ += isim->energy();
     nEBHits++;
     meEBhitEnergy_->Fill(isim->energy());

   }

   if (menEBHits_) menEBHits_->Fill(nEBHits);

   if (nEBHits > 0 ) {

     uint32_t  ebcenterid = getUnitWithMaxEnergy(ebmap);
     EBDetId myEBid(ebcenterid);
     int bx = myEBid.ietaAbs();
     int by = myEBid.iphi();
     int bz = myEBid.zside();
     eb1 =  energyInMatrixEB(1,1,bx,by,bz,ebmap);
     if (meEBe1_) meEBe1_->Fill(eb1);
     eb9 =  energyInMatrixEB(3,3,bx,by,bz,ebmap);
     if (meEBe9_) meEBe9_->Fill(eb9);
     eb25=  energyInMatrixEB(5,5,bx,by,bz,ebmap);
     if (meEBe25_) meEBe25_->Fill(eb25);
     
     MapType  newebmap;
     if( fillEBMatrix(3,3,bx,by,bz,newebmap, ebmap)){
       eb4 = eCluster2x2(newebmap);
       if (meEBe4_) meEBe4_->Fill(eb4);
     }
     if( fillEBMatrix(5,5,bx,by,bz,newebmap, ebmap)){
       eb16 = eCluster4x4(eb9,newebmap); 
       if (meEBe16_) meEBe16_->Fill(eb16);
     }
     
     if (meEBe1oe4_ && eb4 != 0. ) meEBe1oe4_->Fill(eb1/eb4);
     if (meEBe4oe9_ && eb9 != 0. ) meEBe4oe9_->Fill(eb4/eb9);
     if (meEBe9oe16_ && eb16 != 0. ) meEBe9oe16_->Fill(eb9/eb16);
     if (meEBe16oe25_ && eb25 != 0. ) meEBe16oe25_->Fill(eb16/eb25);
     if (meEBe1oe25_ && eb25 != 0. ) meEBe1oe25_->Fill(eb1/eb25);
     if (meEBe9oe25_ && eb25 != 0. ) meEBe9oe25_->Fill(eb9/eb25);
     
   }

   double ee1 = 0.0;
   double ee4 = 0.0;
   double ee9 = 0.0;
   double ee16 = 0.0;
   double ee25 = 0.0;

   MapType eemap;
   uint32_t nEEHits = 0;
   double EEEnergy_ = 0.;
   double EEetzp_ = 0.;
   double EEetzm_ = 0.;
   
   for (std::vector<PCaloHit>::iterator isim = theEECaloHits.begin();
	isim != theEECaloHits.end(); ++isim){
     CaloHitMap[(*isim).id()].push_back((*isim));
     
     EEDetId eeid (isim->id()) ;
     
     LogDebug("HitInfo")
       <<" CaloHit " << isim->getName() << " DetID = "<<isim->id()<< "\n"
       << "Energy = " << isim->energy() << " Time = " << isim->time() << "\n"
       << "EEDetId side " << eeid.zside() << " = " << eeid.ix() << " " << eeid.iy() ;
     
     if (eeid.zside() > 0 ) {
       EEetzp_ += isim->energy();
       if (meEEoccupancyzp_) meEEoccupancyzp_->Fill( eeid.ix(), eeid.iy() );
     }
     else if (eeid.zside() < 0 ) {
       EEetzm_ += isim->energy();
       if (meEEoccupancyzm_) meEEoccupancyzm_->Fill( eeid.ix(), eeid.iy() );
     }

     uint32_t crystid = eeid.rawId();
     eemap[crystid] += isim->energy();

     nEEHits++;
     meEEhitEnergy_->Fill(isim->energy());
     EEEnergy_ += isim->energy();

   }

   if (menEEHits_) menEEHits_->Fill(nEEHits);

   if (nEEHits > 0) {

     uint32_t  eecenterid = getUnitWithMaxEnergy(eemap);
     EEDetId myEEid(eecenterid);
     int bx = myEEid.ix();
     int by = myEEid.iy();
     int bz = myEEid.zside();
     ee1 =  energyInMatrixEE(1,1,bx,by,bz,eemap);
     if (meEEe1_) meEEe1_->Fill(ee1);
     ee9 =  energyInMatrixEE(3,3,bx,by,bz,eemap);
     if (meEEe9_) meEEe9_->Fill(ee9);
     ee25=  energyInMatrixEE(5,5,bx,by,bz,eemap);
     if (meEEe25_) meEEe25_->Fill(ee25);
     
     MapType  neweemap;
     if( fillEEMatrix(3,3,bx,by,bz,neweemap, eemap)){
       ee4 = eCluster2x2(neweemap);
       if (meEEe4_) meEEe4_->Fill(ee4);
     }
     if( fillEEMatrix(5,5,bx,by,bz,neweemap, eemap)){
       ee16 = eCluster4x4(ee9,neweemap); 
       if (meEEe16_) meEEe16_->Fill(ee16);
     }
     
     if (meEEe1oe4_ && ee4 != 0. ) meEEe1oe4_->Fill(ee1/ee4);
     if (meEEe4oe9_ && ee9 != 0. ) meEEe4oe9_->Fill(ee4/ee9);
     if (meEEe9oe16_ && ee16 != 0. ) meEEe9oe16_->Fill(ee9/ee16);
     if (meEEe16oe25_ && ee25 != 0. ) meEEe16oe25_->Fill(ee16/ee25);
     if (meEEe1oe25_ && ee25 != 0. ) meEEe1oe25_->Fill(ee1/ee25);
     if (meEEe9oe25_ && ee25 != 0. ) meEEe9oe25_->Fill(ee9/ee25);
     
   }

   uint32_t nESHits1zp = 0;
   uint32_t nESHits1zm = 0;
   uint32_t nESHits2zp = 0;
   uint32_t nESHits2zm = 0;
   double ESEnergy_ = 0.;
   double ESet1zp_ = 0.;
   double ESet2zp_ = 0.;
   double ESet1zm_ = 0.;
   double ESet2zm_ = 0.;
   
   for (std::vector<PCaloHit>::iterator isim = theESCaloHits.begin();
	isim != theESCaloHits.end(); ++isim){
     CaloHitMap[(*isim).id()].push_back((*isim));
     
     ESDetId esid (isim->id()) ;
     
     LogDebug("HitInfo")
       <<" CaloHit " << isim->getName() << " DetID = "<<isim->id()<< "\n"
       << "Energy = " << isim->energy() << " Time = " << isim->time() << "\n"
       << "ESDetId: z side " << esid.zside() << "  plane " << esid.plane() << esid.six() << ',' << esid.siy() << ':' << esid.strip();

     
     ESEnergy_ += isim->energy();
     
     if (esid.plane() == 1 ) { 
       if (esid.zside() > 0 ) {
         nESHits1zp++ ;
         ESet1zp_ += isim->energy();
         if (meESEnergyHits1zp_) meESEnergyHits1zp_->Fill(isim->energy()) ; 
       }
       else if (esid.zside() < 0 ) {
         nESHits1zm++ ; 
         ESet1zm_ += isim->energy();
         if (meESEnergyHits1zm_) meESEnergyHits1zm_->Fill(isim->energy()) ; 
       }
     }
     else if (esid.plane() == 2 ) {
       if (esid.zside() > 0 ) {
         nESHits2zp++ ; 
         ESet2zp_ += isim->energy();
         if (meESEnergyHits2zp_) meESEnergyHits2zp_->Fill(isim->energy()) ; 
       }
       else if (esid.zside() < 0 ) {
         nESHits2zm++ ; 
         ESet2zm_ += isim->energy();
         if (meESEnergyHits2zm_) meESEnergyHits2zm_->Fill(isim->energy()) ; 
       }
     }

   }

   if (menESHits1zp_) menESHits1zp_->Fill(nESHits1zp);
   if (menESHits1zm_) menESHits1zm_->Fill(nESHits1zm);

   if (menESHits2zp_) menESHits2zp_->Fill(nESHits2zp);
   if (menESHits2zm_) menESHits2zm_->Fill(nESHits2zm);

   // Fill the EE vs ES plots only for particles in the Endcap acceptance
   // assuming at most 2 particles back to back...

   for ( HepMC::GenEvent::particle_const_iterator p = MCEvt->GetEvent()->particles_begin();
         p != MCEvt->GetEvent()->particles_end(); ++p ) {
     
     Hep3Vector hmom = Hep3Vector((*p)->momentum().vect());
     double htheta = hmom.theta();
     double heta = -log(tan(htheta * 0.5));

     if ( heta > 1.653 && heta < 2.6 ) {
       if (meEEvsESEnergyzp_) meEEvsESEnergyzp_->Fill((ESet1zp_+0.7*ESet2zp_)/0.00009,EEetzp_);
     }
     if ( heta < -1.653 && heta > -2.6 ) {
       if (meEEvsESEnergyzm_) meEEvsESEnergyzm_->Fill((ESet1zm_+0.7*ESet2zm_)/0.00009,EEetzm_);
     }
   
   }

   double etot = EBEnergy_ + EEEnergy_ + ESEnergy_ ;
   double fracEB = 0.0;
   double fracEE = 0.0;
   double fracES = 0.0;

   if (etot>0.0) { 
     fracEB = EBEnergy_/etot; 
     fracEE = EEEnergy_/etot; 
     fracES = ESEnergy_/etot; 
   }

   if (meEBEnergyFraction_) meEBEnergyFraction_->Fill(fracEB);

   if (meEEEnergyFraction_) meEEEnergyFraction_->Fill(fracEE);

   if (meESEnergyFraction_) meESEnergyFraction_->Fill(fracES);

   if ( isLongitudinal ) {
     if ( MyPEcalValidInfo->eb1x1() > 0. ) {
       vector<float>  BX0 = MyPEcalValidInfo->bX0();
       for (int ii=0;ii< 26;ii++ ) {
         if (meEBLongitudinalShower_) meEBLongitudinalShower_->Fill(float(ii), BX0[ii]);
       }
     }
     if ( MyPEcalValidInfo->ee1x1() > 0. ) {
       vector<float>  EX0 = MyPEcalValidInfo->eX0();
       for (int ii=0;ii< 26;ii++ ) {
         if (meEELongitudinalShower_) meEELongitudinalShower_->Fill(float(ii), EX0[ii]);
       }
     }
   }
     
}

float EcalSimHitsTask::energyInMatrixEB(int nCellInEta, int nCellInPhi,
                                        int centralEta, int centralPhi,
                                        int centralZ, MapType& themap){

  int   ncristals   = 0;
  float totalEnergy = 0.;
  
  int goBackInEta = nCellInEta/2;
  int goBackInPhi = nCellInPhi/2;
  int startEta    = centralZ*centralEta-goBackInEta;
  int startPhi    = centralPhi-goBackInPhi;
  
  for (int ieta=startEta; ieta<startEta+nCellInEta; ieta++) {
    for (int iphi=startPhi; iphi<startPhi+nCellInPhi; iphi++) {
      
      uint32_t index ;
      if (abs(ieta) > 85 || abs(ieta)<1 ) { continue; }
      if (iphi< 1)      { index = EBDetId(ieta,iphi+360).rawId(); }
      else if(iphi>360) { index = EBDetId(ieta,iphi-360).rawId(); }
      else              { index = EBDetId(ieta,iphi).rawId();     }

      totalEnergy   += themap[index];
      ncristals     += 1;
    }
  }
  
  LogDebug("GeomInfo")
    << nCellInEta << " x " << nCellInPhi 
    << " EB matrix energy = " << totalEnergy
    << " for " << ncristals << " crystals" ;
  return totalEnergy;
  
}   

float EcalSimHitsTask::energyInMatrixEE(int nCellInX, int nCellInY,
                                        int centralX, int centralY,
                                        int centralZ, MapType& themap){
  
  int   ncristals   = 0;
  float totalEnergy = 0.;
        
  int goBackInX = nCellInX/2;
  int goBackInY = nCellInY/2;
  int startX    = centralX-goBackInX;
  int startY    = centralY-goBackInY;
  
  for (int ix=startX; ix<startX+nCellInX; ix++) {
    for (int iy=startY; iy<startY+nCellInY; iy++) {
      
      uint32_t index ;
      try {
        index = EEDetId(ix,iy,centralZ).rawId();
      } catch ( std::runtime_error &e ) { continue ; }
      totalEnergy   += themap[index];
      ncristals     += 1;
    }
  }
  
  LogDebug("GeomInfo")
    << nCellInX << " x " << nCellInY 
    << " EE matrix energy = " << totalEnergy
    << " for " << ncristals << " crystals";
  return totalEnergy;
  
}

bool  EcalSimHitsTask::fillEBMatrix(int nCellInEta, int nCellInPhi,
                                    int CentralEta, int CentralPhi,int CentralZ,
                                    MapType& fillmap, MapType&  themap)
{
  int goBackInEta = nCellInEta/2;
  int goBackInPhi = nCellInPhi/2;
  
  int startEta  =  CentralZ*CentralEta - goBackInEta;
  int startPhi  =  CentralPhi - goBackInPhi;
  
  int i = 0 ;
  for ( int ieta = startEta; ieta < startEta+nCellInEta; ieta ++ ) {
    for( int iphi = startPhi; iphi < startPhi + nCellInPhi; iphi++ ) {
      uint32_t  index;
      if (abs(ieta) > 85 || abs(ieta)<1 ) { continue; }
      if (iphi< 1)      { index = EBDetId(ieta,iphi+360).rawId(); }
      else if(iphi>360) { index = EBDetId(ieta,iphi-360).rawId(); }
      else              { index = EBDetId(ieta,iphi).rawId();     }
      fillmap[i++] = themap[index];
    }
  }
  
  uint32_t  ebcenterid = getUnitWithMaxEnergy(themap);
  
  if ( fillmap[i/2] == themap[ebcenterid] ) 
    return true;
  else 
    return false;
} 

bool  EcalSimHitsTask::fillEEMatrix(int nCellInX, int nCellInY,
                                    int CentralX, int CentralY,int CentralZ,
                                    MapType& fillmap, MapType&  themap)
{
   int goBackInX = nCellInX/2;
   int goBackInY = nCellInY/2;

   int startX  =  CentralX - goBackInX;
   int startY  =  CentralY - goBackInY;

   int i = 0 ;
   for ( int ix = startX; ix < startX+nCellInX; ix ++ ) {

      for( int iy = startY; iy < startY + nCellInY; iy++ ) {

        uint32_t index ;
        try {
          index = EEDetId(ix,iy,CentralZ).rawId();
        } catch ( std::runtime_error &e ) { continue ; }
        fillmap[i++] = themap[index];
      }
   }
   uint32_t  centerid = getUnitWithMaxEnergy(themap);

   if ( fillmap[i/2] == themap[centerid] ) 
        return true;
   else
        return false;
}


float EcalSimHitsTask::eCluster2x2( MapType& themap){
  float  E22=0.;
  float e012  = themap[0]+themap[1]+themap[2];
  float e036  = themap[0]+themap[3]+themap[6];
  float e678  = themap[6]+themap[7]+themap[8];
  float e258  = themap[2]+themap[5]+themap[8];

  if ( (e012>e678 || e012==e678) && (e036>e258  || e036==e258))
    return     E22=themap[0]+themap[1]+themap[3]+themap[4];
  else if ( (e012>e678 || e012==e678)  && (e036<e258 || e036==e258) )
    return    E22=themap[1]+themap[2]+themap[4]+themap[5];
  else if ( (e012<e678 || e012==e678) && (e036>e258 || e036==e258))
    return     E22=themap[3]+themap[4]+themap[6]+themap[7];
  else if ( (e012<e678|| e012==e678)  && (e036<e258|| e036==e258) )
    return    E22=themap[4]+themap[5]+themap[7]+themap[8];
  else {
    return E22;
  }
}

float EcalSimHitsTask::eCluster4x4(float e33,  MapType&  themap){
  float E44=0.;
  float e0_4   = themap[0]+themap[1]+themap[2]+themap[3]+themap[4];
  float e0_20  = themap[0]+themap[5]+themap[10]+themap[15]+themap[20];
  float e4_24  = themap[4]+themap[9]+themap[14]+themap[19]+themap[24];
  float e0_24  = themap[20]+themap[21]+themap[22]+themap[23]+themap[24];
  
  if ((e0_4>e0_24 || e0_4==e0_24) && (e0_20>e4_24|| e0_20==e4_24))
    return E44=e33+themap[0]+themap[1]+themap[2]+themap[3]+themap[5]+themap[10]+themap[15];
  else if ((e0_4>e0_24 || e0_4==e0_24)  && (e0_20<e4_24 || e0_20==e4_24))
    return E44=e33+themap[1]+themap[2]+themap[3]+themap[4]+themap[9]+themap[14]+themap[19];
  else if ((e0_4<e0_24|| e0_4==e0_24) && (e0_20>e4_24 || e0_20==e4_24))
    return E44=e33+themap[5]+themap[10]+themap[15]+themap[20]+themap[21]+themap[22]+themap[23];
  else if ((e0_4<e0_24|| e0_4==e0_24) && (e0_20<e4_24 || e0_20==e4_24))
    return E44=e33+themap[21]+themap[22]+themap[23]+themap[24]+themap[9]+themap[14]+themap[19];
  else{
    return E44;
  }
}

uint32_t EcalSimHitsTask::getUnitWithMaxEnergy(MapType& themap) {
  
  //look for max
  uint32_t unitWithMaxEnergy = 0;
  float    maxEnergy = 0.;
  
  MapType::iterator iter;
  for (iter = themap.begin(); iter != themap.end(); iter++) {
    
    if (maxEnergy < (*iter).second) {
      maxEnergy = (*iter).second;       
      unitWithMaxEnergy = (*iter).first;
    }                           
  }
  
  LogDebug("GeomInfo")
    << " max energy of " << maxEnergy 
    << " GeV in Unit id " << unitWithMaxEnergy;
  return unitWithMaxEnergy;
}


//define this as a plug-in
DEFINE_FWK_MODULE(EcalSimHitsTask)
