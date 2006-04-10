#include "TFile.h"
#include "TTree.h"
#include "TText.h"

void SimHitsPlots()
{

 gROOT ->Reset();
 char*  rfilename = "EcalSimHitsTask.root";

 delete gROOT->GetListOfFiles()->FindObject(rfilename);

 TText* te = new TText();
 TFile * rfile = new TFile(rfilename);

 rfile->cd("DQMData/EcalSimHitsTask");
 gDirectory->ls();

 gStyle->SetOptStat("nemruoi");

//////////////////////////////////////////////////////////////

// Particle Gun
 
 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(1,3);
   
   TH1* meGunEnergy_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Gun Momentum;1",meGunEnergy_);
   meGunEnergy_;
   
   TH1* meGunEta_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Gun Eta;1",meGunEta_);
   meGunEta_;
   
   TH1* meGunPhi_; 
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Gun Phi;1",meGunPhi_);
   meGunPhi_; 
   
   
   Ecal->cd(1); 
   meGunEnergy_->Draw(); 
   Ecal->cd(2); 
   meGunEta_->Draw(); 
   Ecal->cd(3); 
   meGunPhi_->Draw(); 
   Ecal->Print("ParticleGun.eps"); 
 }

 // Multiplicities and energy fractions

 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(2,3);

   TH1 * menEBHits_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel EB hits multiplicity;1",menEBHits_);
   menEBHits_;

   TH1 * meEBEnergyFraction_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel fraction of energy;1",meEBEnergyFraction_);
   meEBEnergyFraction_;

   TH1 * menEEHits_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap EE hits multiplicity;1",menEEHits_);
   menEEHits_;

   TH1 * meEEEnergyFraction_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap fraction of energy;1",meEEEnergyFraction_);
   meEEEnergyFraction_;

   TH1 * meESEnergyFraction_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower fraction of energy;1",meESEnergyFraction_);
   meESEnergyFraction_;
   
   Ecal->cd(1); 
   menEBHits_->Draw(); 
   Ecal->cd(2);
   meEBEnergyFraction_->Draw();
   Ecal->cd(3); 
   menEEHits_->Draw(); 
   Ecal->cd(4);
   meEEEnergyFraction_->Draw();
   Ecal->cd(6);
   meESEnergyFraction_->Draw();
   Ecal->Print("EnergyandMultiplicity.eps");
 }
 
 // Barrel longitudinal profile and hit energy spectrum

 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(1,2);

   TProfile * meEBLongitudinalShower_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel longitudinal shower profile;1",meEBLongitudinalShower_);
   meEBLongitudinalShower_;

   TH1 * meEBhitEnergy_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel EB hit energy spectrum;1",meEBhitEnergy_);
   meEBhitEnergy_;

   Ecal->cd(1);
   meEBLongitudinalShower_->Draw();
   Ecal->cd(2);
   gPad->SetLogx(0);
   gPad->SetLogy(0);
   meEBhitEnergy_->Draw(); 
   gPad->SetLogx(1);
   gPad->SetLogy(1);
   Ecal->Print("Barrel_Longitudinal.eps");
 }
 
 // Barrel occupancy

 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);

   TH2 * meEBoccupancy_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel occupancy;1",meEBoccupancy_);
   meEBoccupancy_;

   Ecal->cd(1);
   meEBoccupancy_->Draw();
   Ecal->Print("Barrel_Occupancy.eps");
 }
 
 // Endcap longitudinal profile and hit energy spectrum

 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(1,2);

   TProfile * meEELongitudinalShower_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap longitudinal shower profile;1",meEELongitudinalShower_);
   meEELongitudinalShower_;

   TH1 * meEEhitEnergy_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap EE hit energy spectrum;1",meEEhitEnergy_);
   meEEhitEnergy_;

   Ecal->cd(1);
   meEELongitudinalShower_->Draw();
   Ecal->cd(2);
   gPad->SetLogx(0);
   gPad->SetLogy(0);
   meEEhitEnergy_->Draw();
   gPad->SetLogx(1);
   gPad->SetLogy(1);
   Ecal->Print("Endcap_Longitudinal.eps");
 }
 
 // Endcap occupancy

 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(1,2);

   TH2 * meEEoccupancyzp_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap occupancy z+;1",meEEoccupancyzp_);
   meEEoccupancyzp_;

   TH2 * meEEoccupancyzm_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap occupancy z-;1",meEEoccupancyzm_);
   meEEoccupancyzm_;

   Ecal->cd(1);
   meEEoccupancyzp_->Draw();
   Ecal->cd(2);
   meEEoccupancyzm_->Draw();
   Ecal->Print("Endcap_Occupancy.eps");
 }
 
 // Barrel transverse variables
 
 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(2,3);
   
   TH1* meEBe1_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E1;1",meEBe1_);
   meEBe1_;

   TH1* meEBe4_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E4;1",meEBe4_);
   meEBe4_;
   
   TH1* meEBe9_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E9;1",meEBe9_);
   meEBe9_;
   
   TH1* meEBe16_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E16;1",meEBe16_);
   meEBe16_;
   
   TH1* meEBe25_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E25;1",meEBe25_);
   meEBe25_;
   
   Ecal->cd(1); 
   meEBe1_->Draw(); 
   Ecal->cd(2); 
   meEBe4_->Draw(); 
   Ecal->cd(3); 
   meEBe9_->Draw(); 
   Ecal->cd(4); 
   meEBe16_->Draw(); 
   Ecal->cd(5); 
   meEBe25_->Draw(); 
   Ecal->Print("Barrel_Transverse.eps"); 
 }
 
 // Endcap transverse variables
 
 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(2,3);
   
   TH1* meEEe1_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E1;1",meEEe1_);
   meEEe1_;

   TH1* meEEe4_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E4;1",meEEe4_);
   meEEe4_;
   
   TH1* meEEe9_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E9;1",meEEe9_);
   meEEe9_;
   
   TH1* meEEe16_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E16;1",meEEe16_);
   meEEe16_;
   
   TH1* meEEe25_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E25;1",meEEe25_);
   meEEe25_;
   
   Ecal->cd(1); 
   meEEe1_->Draw(); 
   Ecal->cd(2); 
   meEEe4_->Draw(); 
   Ecal->cd(3); 
   meEEe9_->Draw(); 
   Ecal->cd(4); 
   meEEe16_->Draw(); 
   Ecal->cd(5); 
   meEEe25_->Draw(); 
   Ecal->Print("Endcap_Transverse.eps"); 
 }
 
 // Barrel cluster ratios

  if (1) { 
    TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000); 
    Ecal->Divide(2,3); 
   
    TH1* meEBe1oe4_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E1oE4;1",meEBe1oe4_); 
    meEBe1oe4_; 
   
    TH1* meEBe4oe9_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E4oE9;1",meEBe4oe9_); 
    meEBe4oe9_; 
   
    TH1* meEBe9oe16_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E9oE16;1",meEBe9oe16_); 
    meEBe9oe16_; 
   
    TH1* meEBe16oe25_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E16oE25;1",meEBe16oe25_); 
    meEBe16oe25_; 
   
    TH1* meEBe1oe25_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E1oE25;1",meEBe1oe25_); 
    meEBe1oe25_; 
   
    TH1* meEBe9oe25_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E9oE25;1",meEBe9oe25_); 
    meEBe9oe25_; 
   
    Ecal->cd(1); 
    meEBe1oe4_->Draw(); 
    Ecal->cd(2); 
    meEBe4oe9_->Draw(); 
    Ecal->cd(3); 
    meEBe9oe16_->Draw(); 
    Ecal->cd(4); 
    meEBe16oe25_->Draw(); 
    Ecal->cd(5); 
    meEBe1oe25_->Draw(); 
    Ecal->cd(6); 
    meEBe9oe25_->Draw(); 
    Ecal->Print("Barrel_ClusterRatio.eps");  
  } 
 
 // Endcap cluster ratios

  if (1) { 
    TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000); 
    Ecal->Divide(2,3); 
   
    TH1* meEEe1oe4_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E1oE4;1",meEEe1oe4_); 
    meEEe1oe4_; 
   
    TH1* meEEe4oe9_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E4oE9;1",meEEe4oe9_); 
    meEEe4oe9_; 
   
    TH1* meEEe9oe16_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E9oE16;1",meEEe9oe16_); 
    meEEe9oe16_; 
   
    TH1* meEEe16oe25_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E16oE25;1",meEEe16oe25_); 
    meEEe16oe25_; 
   
    TH1* meEEe1oe25_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E1oE25;1",meEEe1oe25_); 
    meEEe1oe25_; 
   
    TH1* meEEe9oe25_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E9oE25;1",meEEe9oe25_); 
    meEEe9oe25_; 
   
    Ecal->cd(1); 
    meEEe1oe4_->Draw(); 
    Ecal->cd(2); 
    meEEe4oe9_->Draw(); 
    Ecal->cd(3); 
    meEEe9oe16_->Draw(); 
    Ecal->cd(4); 
    meEEe16oe25_->Draw(); 
    Ecal->cd(5); 
    meEEe1oe25_->Draw(); 
    Ecal->cd(6); 
    meEEe9oe25_->Draw(); 
    Ecal->Print("Endcap_ClusterRatio.eps");  
  } 

 // Preshower hits multiplicity

 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(2,2);
   
   TH1* menESHits1zp_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower ES hits layer 1 multiplicity z+;1",menESHits1zp_);
   menESHits1zp_;
   
   TH1* menESHits2zp_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower ES hits layer 2 multiplicity z+;1",menESHits2zp_);
   menESHits2zp_;
   
   TH1* menESHits1zm_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower ES hits layer 1 multiplicity z-;1",menESHits1zm_);
   menESHits1zm_;
   
   TH1* menESHits2zm_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower ES hits layer 2 multiplicity z-;1",menESHits2zm_);
   menESHits2zm_;
   
   Ecal->cd(1);
   menESHits1zp_->Draw();
   Ecal->cd(2);
   menESHits2zp_->Draw();
   Ecal->cd(3);
   menESHits1zm_->Draw();
   Ecal->cd(4);
   menESHits2zm_->Draw();
   Ecal->Print("Preshower_multiplicity.eps"); 
 }

 // Preshower hits energy

 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(2,2);
   
   TH1* meESEnergyHits1zp_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower energy ES hits layer 1 z+;1",meESEnergyHits1zp_);
   meESEnergyHits1zp_;
   
   TH1* meESEnergyHits2zp_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower energy ES hits layer 2 z+;1",meESEnergyHits2zp_);
   meESEnergyHits2zp_;
   
   TH1* meESEnergyHits1zm_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower energy ES hits layer 1 z-;1",meESEnergyHits1zm_);
   meESEnergyHits1zm_;
   
   TH1* meESEnergyHits2zm_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower energy ES hits layer 2 z-;1",meESEnergyHits2zm_);
   meESEnergyHits2zm_;
   
   
   Ecal->cd(1);
   meESEnergyHits1zp_->Draw();
   Ecal->cd(2);
   meESEnergyHits2zp_->Draw();
   Ecal->cd(3);
   meESEnergyHits1zm_->Draw();
   Ecal->cd(4);
   meESEnergyHits2zm_->Draw();
   Ecal->Print("Preshower_energy.eps"); 
 }

 // Preshower energy vs Endcap one

 if (1) {
   
   TProfile* meEEvsESEnergyzp_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower EE vs ES energy z+;1",meEEvsESEnergyzp_);
   meEEvsESEnergyzp_;
   
   TProfile* meEEvsESEnergyzm_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower EE vs ES energy z-;1",meEEvsESEnergyzm_);
   meEEvsESEnergyzm_;

   meEEvsESEnergyzp_->Fit("pol1");
   meEEvsESEnergyzm_->Fit("pol1");

   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(1,2);

   gStyle->SetOptStat(11);
   gStyle->SetOptFit(1111);

   Ecal->cd(1);
   meEEvsESEnergyzp_->Draw();
   Ecal->cd(2);
   meEEvsESEnergyzm_->Draw();
   Ecal->Print("Preshower_EEvsESenergy.eps"); 
 }

}
