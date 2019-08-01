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
// our library functionality
#include "JetSelectionHelper.h"

int main() {

  // initialize the xAOD EDM
  xAOD::Init();

  // open the input file
  TString inputFilePath = "/home/atlas/Bootcamp/Data/mc16_13TeV.345055.PowhegPythia8EvtGen_NNPDF3_AZNLO_ZH125J_MINLO_llbb_VpT.deriv.DAOD_EXOT27.e5706_s3126_r10724_p3840/DAOD_EXOT27.17882744._000026.pool.root.1";
  xAOD::TEvent event;
  std::unique_ptr< TFile > iFile ( TFile::Open(inputFilePath, "READ") );
  event.readFrom( iFile.get() );

  // make our jet selector tool
  JetSelectionHelper myJetTool;

  // make histograms for storage
  TH1D *h_njets_raw = new TH1D("h_njets_raw","",20,0,20);
  TH1D *h_njets_kin = new TH1D("h_njets_kin","",20,0,20);

  TH1D *h_mjj_raw = new TH1D("h_mjj_raw","",100,0,500);
  TH1D *h_mjj_kin = new TH1D("h_mjj_kin","",100,0,500);

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
    std::cout << "Processing run # " << ei->runNumber() << ", event # " << ei->eventNumber() << std::endl;

    // retrieve the jet container from the event store
    const xAOD::JetContainer* jets = nullptr;
    event.retrieve(jets, "AntiKt4EMTopoJets");

    // make temporary vector of jets for those which pass selection
    std::vector<xAOD::Jet> jets_raw;
    std::vector<xAOD::Jet> jets_kin;

    // loop through all of the jets and make selections with the helper
    for(const xAOD::Jet* jet : *jets) {
      // print the kinematics of each jet in the event
      std::cout << "Jet : " << jet->pt() << jet->eta() << jet->phi() << jet->m() << std::endl;

      jets_raw.push_back(*jet);

      // perform kinematic selections and store in vector of "selected jets"
      if( myJetTool.isJetGood(jet) ){
        jets_kin.push_back(*jet);
      }

    }

    // fill the analysis histograms accordingly
    h_njets_raw->Fill( jets_raw.size() );
    h_njets_kin->Fill( jets_kin.size() );

    if( jets_raw.size()>=2 ){
      h_mjj_raw->Fill( (jets_raw.at(0).p4()+jets_raw.at(1).p4()).M()/1000. );
    }

    if( jets_kin.size()>=2 ){
      h_mjj_kin->Fill( (jets_kin.at(0).p4()+jets_kin.at(1).p4()).M()/1000. );
    }

    // counter for the number of events analyzed thus far
    count += 1;
  }

  // open TFile to store the analysis histogram output
  TFile *fout = new TFile("myOutputFile.root","RECREATE");

  h_njets_raw->Write();
  h_njets_kin->Write();

  h_mjj_raw->Write();
  h_mjj_kin->Write();

  fout->Close();

  // exit from the main function cleanly
  return 0;
}
