// main41.cc is a part of the PYTHIA event generator.
// Copyright (C) 2019 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL v2 or later, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// Author: Mikhail Kirsanov, Mikhail.Kirsanov@cern.ch, based on main01.cc.
// This program illustrates how HepMC can be interfaced to Pythia8.
// It studies the charged multiplicity distribution at the LHC.
// HepMC events are output to the hepmcout41.dat file.

// WARNING: typically one needs 25 MB/100 events at the LHC.
// Therefore large event samples may be impractical.

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"

using namespace Pythia8;



int main() {

  // Interface for conversion from Pythia8::Event to HepMC event.
  HepMC::Pythia8ToHepMC ToHepMC;
  
  // Specify file where HepMC events will be stored.
  HepMC::IO_GenEvent ascii_io("hepmcoutmesons.dat", std::ios::out);
  // Generator. Process selection. LHC initialization. Histogram.
  Pythia pythia;
  pythia.readString("SoftQCD:all = on");
  //pythia.readString("HardQCD:all = on");
  // Used for fixed target
  pythia.readString("Beams:frameType = 2");
  pythia.readString("Beams:idA = 2212");
  pythia.readString("Beams:idB = 2212");
  pythia.readString("Beams:eA = 120.0");
  pythia.readString("Beams:eB = 1.0");
  //pythia.readString("Beams:eCM = 120");
  // Neutral pions are not allowed to decay
  pythia.readString("111:mayDecay = off");
  // Etas are not allowed to decay
  pythia.readString("221:mayDecay = off");
  // Omegas are not allowed to decay
  pythia.readString("223:mayDecay = off");
  pythia.init();
  double nPions = 0;
  double nEtas = 0;
  double nOmegas = 0;

    // Number of events
  int nEvents=5000;
  //  double ratio = nPions/nEvents;
  
  // Begin event loop. Generate event. Skip if error.
  for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
    if (!pythia.next()) continue;

    //Find number of all final charged particles and fill histogram.
    for (int i = 0; i < pythia.event.size(); ++i)
      if (pythia.event[i].isFinal() && pythia.event[i].isNeutral() && (pythia.event[i].idAbs() == 111)){
        ++nPions;
      }
    for (int i = 0; i < pythia.event.size(); ++i)
      if (pythia.event[i].isFinal() && pythia.event[i].isNeutral() && (pythia.event[i].idAbs() == 221)){
        ++nEtas;
     }
    for (int i = 0; i < pythia.event.size(); ++i)
      if (pythia.event[i].isFinal() && pythia.event[i].isNeutral() && (pythia.event[i].idAbs() == 223)){
        ++nOmegas;
     }



    
    
    HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
    //ToHepMC.fill_next_event( pythia, hepmcevt  );
    ToHepMC.fill_next_event( pythia, hepmcevt  );
    //ToHepMC.set_store_pdf(true);
    //ToHepMC.set_store_proc(true);
    //ToHepMC.set_store_xsec(true);
    
    // Write the HepMC event to file. Done with it.
    ascii_io << hepmcevt;
    delete hepmcevt;
    
  // End of event loop. Statistics. Histogram.

  }
  pythia.stat();
  // Done.

  std::cout << "Total number of pions " <<  nPions << "\n" ;
  std::cout << "Total number of pions per POT " <<  nPions/nEvents << "\n" ;
  std::cout << "---------------------------------------------------------------------" << "\n" ;
  std::cout << "Total number of Etas " <<  nEtas << "\n" ;
  std::cout << "Total number of Etas per POT " <<  nEtas/nEvents << "\n" ;
  std::cout << "---------------------------------------------------------------------" << "\n" ;
  std::cout << "Total number of Omegas " <<  nOmegas << "\n" ;
  std::cout << "Total number of Omegas per POT " <<  nOmegas/nEvents << "\n" ;
  std::cout << "---------------------------------------------------------------------" << "\n" ;
  // the 0.084 is the branching ratio for the process omega -> pion + gamma
  std::cout << "Total number of Omegas decaying to pions + photons " <<  0.084*nOmegas << "\n" ;
  std::cout << "Total number of Omegas decaying  to pions + photons per POT " <<  0.084*nOmegas/nEvents << "\n" ;
  return 0;
}
