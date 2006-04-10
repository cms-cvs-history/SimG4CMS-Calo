#include "TFile.h"
#include "TTree.h"
#include "TText.h"

void SimHitsPlotCompare()
{

 gROOT ->Reset();
 char*  rfilename = "EcalSimHitsTask_old.root";
 char*  sfilename = "EcalSimHitsTask_new.root";

 delete gROOT->GetListOfFiles()->FindObject(rfilename);
 delete gROOT->GetListOfFiles()->FindObject(sfilename);

 TText* te = new TText();
 TFile * rfile = new TFile(rfilename);
 TFile * sfile = new TFile(sfilename);

 rfile->cd("DQMData/EcalSimHitsTask");
 gDirectory->ls();

 sfile->cd("DQMData/EcalSimHitsTask");
 gDirectory->ls();

 gStyle->SetOptStat("n");

//////////////////////////////////////////////////////////////

// Particle Gun
 
 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(1,3);
   
   TH1* meGunEnergy_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Gun Momentum;1",meGunEnergy_);
   meGunEnergy_;
   meGunEnergy_->SetLineColor(4);
   
   TH1* meGunEta_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Gun Eta;1",meGunEta_);
   meGunEta_;
   meGunEta_->SetLineColor(4);
   
   TH1* meGunPhi_; 
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Gun Phi;1",meGunPhi_);
   meGunPhi_; 
   meGunPhi_->SetLineColor(4); 
   
   TH1* newmeGunEnergy_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Gun Momentum;1",newmeGunEnergy_);
   newmeGunEnergy_;
   newmeGunEnergy_->SetLineColor(2);
   
   TH1* newmeGunEta_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Gun Eta;1",newmeGunEta_);
   newmeGunEta_;
   newmeGunEta_->SetLineColor(2);
   
   TH1* newmeGunPhi_; 
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Gun Phi;1",newmeGunPhi_);
   newmeGunPhi_; 
   newmeGunPhi_->SetLineColor(2); 
   
   
   Ecal->cd(1); 
   meGunEnergy_->Draw(); 
   newmeGunEnergy_->Draw("same"); 
   Ecal->cd(2); 
   meGunEta_->Draw(); 
   newmeGunEta_->Draw("same"); 
   Ecal->cd(3); 
   meGunPhi_->Draw(); 
   newmeGunPhi_->Draw("same"); 
   Ecal->Print("ParticleGun_compare.eps"); 
 }

 // Multiplicities and energy fractions

 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(2,3);

   TH1 * menEBHits_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel EB hits multiplicity;1",menEBHits_);
   menEBHits_;
   menEBHits_->SetLineColor(4);

   TH1 * meEBEnergyFraction_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel fraction of energy;1",meEBEnergyFraction_);
   meEBEnergyFraction_;
   meEBEnergyFraction_->SetLineColor(4);

   TH1 * menEEHits_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap EE hits multiplicity;1",menEEHits_);
   menEEHits_;
   menEEHits_->SetLineColor(4);

   TH1 * meEEEnergyFraction_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap fraction of energy;1",meEEEnergyFraction_);
   meEEEnergyFraction_;
   meEEEnergyFraction_->SetLineColor(4);

   TH1 * meESEnergyFraction_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower fraction of energy;1",meESEnergyFraction_);
   meESEnergyFraction_;
   meESEnergyFraction_->SetLineColor(4);

   TH1 * newmenEBHits_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel EB hits multiplicity;1",newmenEBHits_);
   newmenEBHits_;
   newmenEBHits_->SetLineColor(2);

   TH1 * newmeEBEnergyFraction_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel fraction of energy;1",newmeEBEnergyFraction_);
   newmeEBEnergyFraction_;
   newmeEBEnergyFraction_->SetLineColor(2);

   TH1 * newmenEEHits_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap EE hits multiplicity;1",newmenEEHits_);
   newmenEEHits_;
   newmenEEHits_->SetLineColor(2);

   TH1 * newmeEEEnergyFraction_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap fraction of energy;1",newmeEEEnergyFraction_);
   newmeEEEnergyFraction_;
   newmeEEEnergyFraction_->SetLineColor(2);

   TH1 * newmeESEnergyFraction_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower fraction of energy;1",newmeESEnergyFraction_);
   newmeESEnergyFraction_;
   newmeESEnergyFraction_->SetLineColor(2);
   
   Ecal->cd(1); 
   menEBHits_->Draw(); 
   newmenEBHits_->Draw("same"); 
   Ecal->cd(2);
   meEBEnergyFraction_->Draw();
   newmeEBEnergyFraction_->Draw("same");
   Ecal->cd(3); 
   menEEHits_->Draw(); 
   newmenEEHits_->Draw("same"); 
   Ecal->cd(4);
   meEEEnergyFraction_->Draw();
   newmeEEEnergyFraction_->Draw("same");
   Ecal->cd(6);
   meESEnergyFraction_->Draw();
   newmeESEnergyFraction_->Draw("same");
   Ecal->Print("EnergyandMultiplicity_compare.eps");
 }
 
 // Barrel longitudinal profile and hit energy spectrum

 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(1,2);

   TProfile * meEBLongitudinalShower_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel longitudinal shower profile;1",meEBLongitudinalShower_);
   meEBLongitudinalShower_;
   meEBLongitudinalShower_->SetLineColor(4);

   TH1 * meEBhitEnergy_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel EB hit energy spectrum;1",meEBhitEnergy_);
   meEBhitEnergy_;
   meEBhitEnergy_->SetLineColor(4);

   TProfile * newmeEBLongitudinalShower_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel longitudinal shower profile;1",newmeEBLongitudinalShower_);
   newmeEBLongitudinalShower_;
   newmeEBLongitudinalShower_->SetLineColor(2);

   TH1 * newmeEBhitEnergy_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel EB hit energy spectrum;1",newmeEBhitEnergy_);
   newmeEBhitEnergy_;
   newmeEBhitEnergy_->SetLineColor(2);

   Ecal->cd(1);
   meEBLongitudinalShower_->Draw();
   newmeEBLongitudinalShower_->Draw("same");
   Ecal->cd(2);
   gPad->SetLogx(0);
   gPad->SetLogy(0);
   meEBhitEnergy_->Draw(); 
   newmeEBhitEnergy_->Draw("same"); 
   gPad->SetLogx(1);
   gPad->SetLogy(1);
   Ecal->Print("Barrel_Longitudinal_compare.eps");
 }
 
 // Barrel occupancy

 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(1,2);

   TH2 * meEBoccupancy_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel occupancy;1",meEBoccupancy_);
   meEBoccupancy_;
   meEBoccupancy_->SetLineColor(4);

   TH2 * newmeEBoccupancy_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel occupancy;1",newmeEBoccupancy_);
   newmeEBoccupancy_;
   newmeEBoccupancy_->SetLineColor(2);

   Ecal->cd(1);
   meEBoccupancy_->Draw();
   Ecal->cd(2);
   newmeEBoccupancy_->Draw();
   Ecal->Print("Barrel_Occupancy_compare.eps");
 }
 
 // Endcap longitudinal profile and hit energy spectrum

 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(1,2);

   TProfile * meEELongitudinalShower_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap longitudinal shower profile;1",meEELongitudinalShower_);
   meEELongitudinalShower_;
   meEELongitudinalShower_->SetLineColor(4);

   TH1 * meEEhitEnergy_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap EE hit energy spectrum;1",meEEhitEnergy_);
   meEEhitEnergy_;
   meEEhitEnergy_->SetLineColor(4);

   TProfile * newmeEELongitudinalShower_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap longitudinal shower profile;1",newmeEELongitudinalShower_);
   newmeEELongitudinalShower_;
   newmeEELongitudinalShower_->SetLineColor(2);

   TH1 * newmeEEhitEnergy_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap EE hit energy spectrum;1",newmeEEhitEnergy_);
   newmeEEhitEnergy_;
   newmeEEhitEnergy_->SetLineColor(2);

   Ecal->cd(1);
   meEELongitudinalShower_->Draw();
   newmeEELongitudinalShower_->Draw("same");
   Ecal->cd(2);
   gPad->SetLogx(0);
   gPad->SetLogy(0);
   meEEhitEnergy_->Draw();
   newmeEEhitEnergy_->Draw("same");
   gPad->SetLogx(1);
   gPad->SetLogy(1);
   Ecal->Print("Endcap_Longitudinal_compare.eps");
 }
 
 // Endcap occupancy

 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(2,2);

   TH2 * meEEoccupancyzp_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap occupancy z+;1",meEEoccupancyzp_);
   meEEoccupancyzp_;
   meEEoccupancyzp_->SetLineColor(4);

   TH2 * meEEoccupancyzm_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap occupancy z-;1",meEEoccupancyzm_);
   meEEoccupancyzm_;
   meEEoccupancyzm_->SetLineColor(4);

   TH2 * newmeEEoccupancyzp_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap occupancy z+;1",newmeEEoccupancyzp_);
   newmeEEoccupancyzp_;
   newmeEEoccupancyzp_->SetLineColor(2);

   TH2 * newmeEEoccupancyzm_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap occupancy z-;1",newmeEEoccupancyzm_);
   newmeEEoccupancyzm_;
   newmeEEoccupancyzm_->SetLineColor(2);

   Ecal->cd(1);
   meEEoccupancyzp_->Draw();
   Ecal->cd(2);
   meEEoccupancyzm_->Draw();
   Ecal->cd(3);
   newmeEEoccupancyzp_->Draw();
   Ecal->cd(4);
   newmeEEoccupancyzm_->Draw();
   Ecal->Print("Endcap_Occupancy_compare.eps");
 }
 
 // Barrel transverse variables
 
 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(2,3);
   
   TH1* meEBe1_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E1;1",meEBe1_);
   meEBe1_;
   meEBe1_->SetLineColor(4);

   TH1* meEBe4_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E4;1",meEBe4_);
   meEBe4_;
   meEBe4_->SetLineColor(4);
   
   TH1* meEBe9_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E9;1",meEBe9_);
   meEBe9_;
   meEBe9_->SetLineColor(4);
   
   TH1* meEBe16_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E16;1",meEBe16_);
   meEBe16_;
   meEBe16_->SetLineColor(4);
   
   TH1* meEBe25_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E25;1",meEBe25_);
   meEBe25_;
   meEBe25_->SetLineColor(4);
   
   TH1* newmeEBe1_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E1;1",newmeEBe1_);
   newmeEBe1_;
   newmeEBe1_->SetLineColor(2);

   TH1* newmeEBe4_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E4;1",newmeEBe4_);
   newmeEBe4_;
   newmeEBe4_->SetLineColor(2);
   
   TH1* newmeEBe9_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E9;1",newmeEBe9_);
   newmeEBe9_;
   newmeEBe9_->SetLineColor(2);
   
   TH1* newmeEBe16_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E16;1",newmeEBe16_);
   newmeEBe16_;
   newmeEBe16_->SetLineColor(2);
   
   TH1* newmeEBe25_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E25;1",newmeEBe25_);
   newmeEBe25_;
   newmeEBe25_->SetLineColor(2);
   
   Ecal->cd(1); 
   meEBe1_->Draw(); 
   newmeEBe1_->Draw("same"); 
   Ecal->cd(2); 
   meEBe4_->Draw(); 
   newmeEBe4_->Draw("same"); 
   Ecal->cd(3); 
   meEBe9_->Draw(); 
   newmeEBe9_->Draw("same"); 
   Ecal->cd(4); 
   meEBe16_->Draw(); 
   newmeEBe16_->Draw("same"); 
   Ecal->cd(5); 
   meEBe25_->Draw(); 
   newmeEBe25_->Draw("same"); 
   Ecal->Print("Barrel_Transverse_compare.eps"); 
 }
 
 // Endcap transverse variables
 
 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(2,3);
   
   TH1* meEEe1_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E1;1",meEEe1_);
   meEEe1_;
   meEEe1_->SetLineColor(4);

   TH1* meEEe4_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E4;1",meEEe4_);
   meEEe4_;
   meEEe4_->SetLineColor(4);
   
   TH1* meEEe9_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E9;1",meEEe9_);
   meEEe9_;
   meEEe9_->SetLineColor(4);
   
   TH1* meEEe16_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E16;1",meEEe16_);
   meEEe16_;
   meEEe16_->SetLineColor(4);
   
   TH1* meEEe25_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E25;1",meEEe25_);
   meEEe25_;
   meEEe25_->SetLineColor(4);
   
   TH1* newmeEEe1_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E1;1",newmeEEe1_);
   newmeEEe1_;
   newmeEEe1_->SetLineColor(2);

   TH1* newmeEEe4_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E4;1",newmeEEe4_);
   newmeEEe4_;
   newmeEEe4_->SetLineColor(2);
   
   TH1* newmeEEe9_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E9;1",newmeEEe9_);
   newmeEEe9_;
   newmeEEe9_->SetLineColor(2);
   
   TH1* newmeEEe16_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E16;1",newmeEEe16_);
   newmeEEe16_;
   newmeEEe16_->SetLineColor(2);
   
   TH1* newmeEEe25_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E25;1",newmeEEe25_);
   newmeEEe25_;
   newmeEEe25_->SetLineColor(2);
   
   Ecal->cd(1); 
   meEEe1_->Draw(); 
   newmeEEe1_->Draw("same"); 
   Ecal->cd(2); 
   meEEe4_->Draw(); 
   newmeEEe4_->Draw("same"); 
   Ecal->cd(3); 
   meEEe9_->Draw(); 
   newmeEEe9_->Draw("same"); 
   Ecal->cd(4); 
   meEEe16_->Draw(); 
   newmeEEe16_->Draw("same"); 
   Ecal->cd(5); 
   meEEe25_->Draw(); 
   newmeEEe25_->Draw("same"); 
   Ecal->Print("Endcap_Transverse_compare.eps"); 
 }
 
 // Barrel cluster ratios

  if (1) { 
    TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000); 
    Ecal->Divide(2,3); 
   
    TH1* meEBe1oe4_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E1oE4;1",meEBe1oe4_); 
    meEBe1oe4_; 
    meEBe1oe4_->SetLineColor(4); 
   
    TH1* meEBe4oe9_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E4oE9;1",meEBe4oe9_); 
    meEBe4oe9_; 
    meEBe4oe9_->SetLineColor(4); 
   
    TH1* meEBe9oe16_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E9oE16;1",meEBe9oe16_); 
    meEBe9oe16_; 
    meEBe9oe16_->SetLineColor(4); 
   
    TH1* meEBe16oe25_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E16oE25;1",meEBe16oe25_); 
    meEBe16oe25_; 
    meEBe16oe25_->SetLineColor(4); 
   
    TH1* meEBe1oe25_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E1oE25;1",meEBe1oe25_); 
    meEBe1oe25_; 
    meEBe1oe25_->SetLineColor(4); 
   
    TH1* meEBe9oe25_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E9oE25;1",meEBe9oe25_); 
    meEBe9oe25_; 
    meEBe9oe25_->SetLineColor(4); 
   
    TH1* newmeEBe1oe4_; 
    sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E1oE4;1",newmeEBe1oe4_); 
    newmeEBe1oe4_; 
    newmeEBe1oe4_->SetLineColor(2); 
   
    TH1* newmeEBe4oe9_; 
    sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E4oE9;1",newmeEBe4oe9_); 
    newmeEBe4oe9_; 
    newmeEBe4oe9_->SetLineColor(2); 
   
    TH1* newmeEBe9oe16_; 
    sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E9oE16;1",newmeEBe9oe16_); 
    newmeEBe9oe16_; 
    newmeEBe9oe16_->SetLineColor(2); 
   
    TH1* newmeEBe16oe25_; 
    sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E16oE25;1",newmeEBe16oe25_); 
    newmeEBe16oe25_; 
    newmeEBe16oe25_->SetLineColor(2); 
   
    TH1* newmeEBe1oe25_; 
    sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E1oE25;1",newmeEBe1oe25_); 
    newmeEBe1oe25_; 
    newmeEBe1oe25_->SetLineColor(2); 
   
    TH1* newmeEBe9oe25_; 
    sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Barrel E9oE25;1",newmeEBe9oe25_); 
    newmeEBe9oe25_; 
    newmeEBe9oe25_->SetLineColor(2); 
   
    Ecal->cd(1); 
    meEBe1oe4_->Draw(); 
    newmeEBe1oe4_->Draw("same"); 
    Ecal->cd(2); 
    meEBe4oe9_->Draw(); 
    newmeEBe4oe9_->Draw("same"); 
    Ecal->cd(3); 
    meEBe9oe16_->Draw(); 
    newmeEBe9oe16_->Draw("same"); 
    Ecal->cd(4); 
    meEBe16oe25_->Draw(); 
    newmeEBe16oe25_->Draw("same"); 
    Ecal->cd(5); 
    meEBe1oe25_->Draw(); 
    newmeEBe1oe25_->Draw("same"); 
    Ecal->cd(6); 
    meEBe9oe25_->Draw(); 
    newmeEBe9oe25_->Draw("same"); 
    Ecal->Print("Barrel_ClusterRatio_compare.eps");  
  } 
 
 // Endcap cluster ratios

  if (1) { 
    TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000); 
    Ecal->Divide(2,3); 
   
    TH1* meEEe1oe4_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E1oE4;1",meEEe1oe4_); 
    meEEe1oe4_; 
    meEEe1oe4_->SetLineColor(4); 
   
    TH1* meEEe4oe9_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E4oE9;1",meEEe4oe9_); 
    meEEe4oe9_; 
    meEEe4oe9_->SetLineColor(4); 
   
    TH1* meEEe9oe16_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E9oE16;1",meEEe9oe16_); 
    meEEe9oe16_; 
    meEEe9oe16_->SetLineColor(4); 
   
    TH1* meEEe16oe25_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E16oE25;1",meEEe16oe25_); 
    meEEe16oe25_; 
    meEEe16oe25_->SetLineColor(4); 
   
    TH1* meEEe1oe25_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E1oE25;1",meEEe1oe25_); 
    meEEe1oe25_; 
    meEEe1oe25_->SetLineColor(4);
   
    TH1* meEEe9oe25_; 
    rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E9oE25;1",meEEe9oe25_); 
    meEEe9oe25_; 
    meEEe9oe25_->SetLineColor(4); 
   
    TH1* newmeEEe1oe4_; 
    sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E1oE4;1",newmeEEe1oe4_); 
    newmeEEe1oe4_; 
    newmeEEe1oe4_->SetLineColor(2); 
   
    TH1* newmeEEe4oe9_; 
    sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E4oE9;1",newmeEEe4oe9_); 
    newmeEEe4oe9_; 
    newmeEEe4oe9_->SetLineColor(2); 
   
    TH1* newmeEEe9oe16_; 
    sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E9oE16;1",newmeEEe9oe16_); 
    newmeEEe9oe16_; 
    newmeEEe9oe16_->SetLineColor(2); 
   
    TH1* newmeEEe16oe25_; 
    sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E16oE25;1",newmeEEe16oe25_); 
    newmeEEe16oe25_; 
    newmeEEe16oe25_->SetLineColor(2); 
   
    TH1* newmeEEe1oe25_; 
    sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E1oE25;1",newmeEEe1oe25_); 
    newmeEEe1oe25_; 
    newmeEEe1oe25_->SetLineColor(2);
   
    TH1* newmeEEe9oe25_; 
    sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Endcap E9oE25;1",newmeEEe9oe25_); 
    newmeEEe9oe25_; 
    newmeEEe9oe25_->SetLineColor(2); 
   
    Ecal->cd(1); 
    meEEe1oe4_->Draw(); 
    newmeEEe1oe4_->Draw("same"); 
    Ecal->cd(2); 
    meEEe4oe9_->Draw(); 
    newmeEEe4oe9_->Draw("same"); 
    Ecal->cd(3); 
    meEEe9oe16_->Draw(); 
    newmeEEe9oe16_->Draw("same"); 
    Ecal->cd(4); 
    meEEe16oe25_->Draw(); 
    newmeEEe16oe25_->Draw("same"); 
    Ecal->cd(5); 
    meEEe1oe25_->Draw(); 
    newmeEEe1oe25_->Draw("same"); 
    Ecal->cd(6); 
    meEEe9oe25_->Draw(); 
    newmeEEe9oe25_->Draw("same"); 
    Ecal->Print("Endcap_ClusterRatio_compare.eps");  
  } 

 // Preshower hits multiplicity

 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(2,2);
   
   TH1* menESHits1zp_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower ES hits layer 1 multiplicity z+;1",menESHits1zp_);
   menESHits1zp_;
   menESHits1zp_->SetLineColor(4);
   
   TH1* menESHits2zp_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower ES hits layer 2 multiplicity z+;1",menESHits2zp_);
   menESHits2zp_;
   menESHits2zp_->SetLineColor(4);
   
   TH1* menESHits1zm_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower ES hits layer 1 multiplicity z-;1",menESHits1zm_);
   menESHits1zm_;
   menESHits1zm_->SetLineColor(4);
   
   TH1* menESHits2zm_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower ES hits layer 2 multiplicity z-;1",menESHits2zm_);
   menESHits2zm_;
   menESHits2zm_->SetLineColor(4);
   
   TH1* newmenESHits1zp_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower ES hits layer 1 multiplicity z+;1",newmenESHits1zp_);
   newmenESHits1zp_;
   newmenESHits1zp_->SetLineColor(2);
   
   TH1* newmenESHits2zp_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower ES hits layer 2 multiplicity z+;1",newmenESHits2zp_);
   newmenESHits2zp_;
   newmenESHits2zp_->SetLineColor(2);
   
   TH1* newmenESHits1zm_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower ES hits layer 1 multiplicity z-;1",newmenESHits1zm_);
   newmenESHits1zm_;
   newmenESHits1zm_->SetLineColor(2);
   
   TH1* newmenESHits2zm_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower ES hits layer 2 multiplicity z-;1",newmenESHits2zm_);
   newmenESHits2zm_;
   newmenESHits2zm_->SetLineColor(2);
   
   Ecal->cd(1);
   menESHits1zp_->Draw();
   newmenESHits1zp_->Draw("same");
   Ecal->cd(2);
   menESHits2zp_->Draw();
   newmenESHits2zp_->Draw("same");
   Ecal->cd(3);
   menESHits1zm_->Draw();
   newmenESHits1zm_->Draw("same");
   Ecal->cd(4);
   menESHits2zm_->Draw();
   newmenESHits2zm_->Draw("same");
   Ecal->Print("Preshower_multiplicity_compare.eps"); 
 }

 // Preshower hits energy

 if (1) {
   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(2,2);
   
   TH1* meESEnergyHits1zp_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower energy ES hits layer 1 z+;1",meESEnergyHits1zp_);
   meESEnergyHits1zp_;
   meESEnergyHits1zp_->SetLineColor(4);
   
   TH1* meESEnergyHits2zp_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower energy ES hits layer 2 z+;1",meESEnergyHits2zp_);
   meESEnergyHits2zp_;
   meESEnergyHits2zp_->SetLineColor(4);
   
   TH1* meESEnergyHits1zm_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower energy ES hits layer 1 z-;1",meESEnergyHits1zm_);
   meESEnergyHits1zm_;
   meESEnergyHits1zm_->SetLineColor(4);
   
   TH1* meESEnergyHits2zm_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower energy ES hits layer 2 z-;1",meESEnergyHits2zm_);
   meESEnergyHits2zm_;
   meESEnergyHits2zm_->SetLineColor(4);
   
   TH1* newmeESEnergyHits1zp_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower energy ES hits layer 1 z+;1",newmeESEnergyHits1zp_);
   newmeESEnergyHits1zp_;
   newmeESEnergyHits1zp_->SetLineColor(2);
   
   TH1* newmeESEnergyHits2zp_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower energy ES hits layer 2 z+;1",newmeESEnergyHits2zp_);
   newmeESEnergyHits2zp_;
   newmeESEnergyHits2zp_->SetLineColor(2);
   
   TH1* newmeESEnergyHits1zm_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower energy ES hits layer 1 z-;1",newmeESEnergyHits1zm_);
   newmeESEnergyHits1zm_;
   newmeESEnergyHits1zm_->SetLineColor(2);
   
   TH1* newmeESEnergyHits2zm_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower energy ES hits layer 2 z-;1",newmeESEnergyHits2zm_);
   newmeESEnergyHits2zm_;
   newmeESEnergyHits2zm_->SetLineColor(2);
   
   
   Ecal->cd(1);
   meESEnergyHits1zp_->Draw();
   newmeESEnergyHits1zp_->Draw("same");
   Ecal->cd(2);
   meESEnergyHits2zp_->Draw();
   newmeESEnergyHits2zp_->Draw("same");
   Ecal->cd(3);
   meESEnergyHits1zm_->Draw();
   newmeESEnergyHits1zm_->Draw("same");
   Ecal->cd(4);
   meESEnergyHits2zm_->Draw();
   newmeESEnergyHits2zm_->Draw("same");
   Ecal->Print("Preshower_energy_compare.eps"); 
 }

 // Preshower energy vs Endcap one

 if (1) {
   
   TProfile* meEEvsESEnergyzp_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower EE vs ES energy z+;1",meEEvsESEnergyzp_);
   meEEvsESEnergyzp_;
   meEEvsESEnergyzp_->SetLineColor(4);
   
   TProfile* meEEvsESEnergyzm_;
   rfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower EE vs ES energy z-;1",meEEvsESEnergyzm_);
   meEEvsESEnergyzm_;
   meEEvsESEnergyzm_->SetLineColor(4);

   meEEvsESEnergyzp_->Fit("pol1");
   meEEvsESEnergyzm_->Fit("pol1");
   
   TProfile* newmeEEvsESEnergyzp_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower EE vs ES energy z+;1",newmeEEvsESEnergyzp_);
   newmeEEvsESEnergyzp_;
   newmeEEvsESEnergyzp_->SetLineColor(2);
   
   TProfile* newmeEEvsESEnergyzm_;
   sfile->GetObject("DQMData/EcalSimHitsTask/EcalSimHitsTask Preshower EE vs ES energy z-;1",newmeEEvsESEnergyzm_);
   newmeEEvsESEnergyzm_;
   newmeEEvsESEnergyzm_->SetLineColor(2);

   newmeEEvsESEnergyzp_->Fit("pol1");
   newmeEEvsESEnergyzm_->Fit("pol1");

   TCanvas * Ecal = new TCanvas("Ecal","Ecal",800,1000);
   Ecal->Divide(1,2);

   gStyle->SetOptStat(11);
   gStyle->SetOptFit(1111);

   Ecal->cd(1);
   meEEvsESEnergyzp_->Draw();
   newmeEEvsESEnergyzp_->Draw("same");
   Ecal->cd(2);
   meEEvsESEnergyzm_->Draw();
   newmeEEvsESEnergyzm_->Draw("same");
   Ecal->Print("Preshower_EEvsESenergy_compare.eps"); 
 }

}
