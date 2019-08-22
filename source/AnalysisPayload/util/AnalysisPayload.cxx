// stdlib functionality
#include <iostream>
// ROOT functionality
#include <TFile.h>
#include <TH1D.h>
// ATLAS EDM functionality
#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
// Add a submodule for JetSelectionHelper
#include "JetSelectionHelper/JetSelectionHelper.h"
// jet calibration
#include "AsgTools/AnaToolHandle.h"
#include "JetCalibTools/IJetCalibrationTool.h"
int main() {

  // initialize the xAOD EDM
  xAOD::Init();

  // open the input file
  TString inputFilePath = "/home/atlas/Bootcamp/data/DAOD_EXOT27.17882744._000026.pool.root.1";
  xAOD::TEvent event;
  std::unique_ptr< TFile > iFile ( TFile::Open(inputFilePath, "READ") );
  event.readFrom( iFile.get() );

  // make histograms for storage
  TH1D *h_njets_raw = new TH1D("h_njets_raw","",20,0,20);

  TH1D *h_mjj_raw = new TH1D("h_mjj_raw","",100,0,500);
  TH1D *h_mjj_raw1 = new TH1D("h_mjj_raw1","",100,0,500);

  asg::AnaToolHandle<IJetCalibrationTool> JetCalibrationTool_handle;
  JetCalibrationTool_handle.setTypeAndName("JetCalibrationTool/MyCalibrationTool");
  JetCalibrationTool_handle.setProperty("JetCollection","AntiKt4EMTopo"                                                  );
  JetCalibrationTool_handle.setProperty("ConfigFile"   ,"JES_MC16Recommendation_Consolidated_EMTopo_Apr2019_Rel21.config");
  JetCalibrationTool_handle.setProperty("CalibSequence","JetArea_Residual_EtaJES_GSC_Smear"                              );
  JetCalibrationTool_handle.setProperty("CalibArea"    ,"00-04-82"                                                       );
  JetCalibrationTool_handle.setProperty("IsData"       ,false                                                            );
  JetCalibrationTool_handle.retrieve();
  // for counting events
  unsigned count = 0;

  // get the number of events in the file to loop over
  const Long64_t numEntries = event.getEntries();

  // primary event loop
  for ( Long64_t i=0; i<numEntries; ++i ) {

    // Load the event
    event.getEntry( i );

    // Load xAOD::EventInfo and print the event info
    const xAOD::EventInfo * ei = nullptr;
    event.retrieve( ei, "EventInfo" );
    //std::cout << "Processing run # " << ei->runNumber() << ", event # " << ei->eventNumber() << std::endl;

    // retrieve the jet container from the event store
    const xAOD::JetContainer* jets = nullptr;
    event.retrieve(jets, "AntiKt4EMTopoJets");

    // make temporary vector of jets for those which pass selection
    std::vector<xAOD::Jet> jets_raw1;
    std::vector<xAOD::Jet> jets_raw;

    // loop through all of the jets and make selections with the helper
    // add jet selection helper
    JetSelectionHelper jet_selector;
    for(const xAOD::Jet* jet : *jets) {
      // print the kinematics of each jet in the event
      //std::cout << "Jet : pt=" << jet->pt() << "  eta=" << jet->eta() << "  phi=" << jet->phi() << "  m=" << jet->m() << std::endl;
      // calibrate the jet
      xAOD::Jet *calibratedjet;
      JetCalibrationTool_handle->calibratedCopy(*jet,calibratedjet);
      if (calibratedjet->pt()>30000&&jet_selector.isJetBFlavor(calibratedjet)) {
          jets_raw.push_back(*calibratedjet);
      }
      if (jet->pt()>30000&&jet_selector.isJetBFlavor(jet)) {
          jets_raw1.push_back(*jet);
      }
      // cleanup
      delete calibratedjet;
    }

    // fill the analysis histograms accordingly
    h_njets_raw->Fill( jets_raw.size() );

    if( jets_raw.size()>=2 ){
      h_mjj_raw->Fill( (jets_raw.at(0).p4()+jets_raw.at(1).p4()).M()/1000. );
    }
    if( jets_raw1.size()>=2 ){
      h_mjj_raw1->Fill( (jets_raw1.at(0).p4()+jets_raw1.at(1).p4()).M()/1000. );
    }
    // counter for the number of events analyzed thus far
    count += 1;
  }

  // open TFile to store the analysis histogram output
  TFile *fout = new TFile("raw.root","RECREATE");

  h_njets_raw->Write();

  h_mjj_raw->Write();

  fout->Close();

  TFile *fout1 = new TFile("calib.root","RECREATE");


  h_mjj_raw1->Write();

  fout1->Close();
  // exit from the main function cleanly
  return 0;
}
