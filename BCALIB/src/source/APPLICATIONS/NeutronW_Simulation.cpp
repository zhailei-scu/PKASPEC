#include <iostream>
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "NWGeometry.h"
#include "NWActionInitialization.h"
#include "G4VModularPhysicsList.hh"
#include "QGSP_BIC_HP.hh"
#include "FTFP_BERT.hh"
#include "NWGlobal.h"
#include "NWSimParameters.h"
#include <sstream>
#include "../../../../LIB/src/include/MiniUtilities/EXNUMB.h"

void RunTheSimulation(int EventLoops,double ParticleEnergy) {

	NWGlobal::GetInstance()->SetEventLoops(EventLoops);

	/*Parameters*/
	NWSimParameters* theSimParemeters = new NWSimParameters();

	theSimParemeters->SetDefulatValue();

	NWBeam *theBeam = new NWBeam();

	theBeam->SetGunEnergy(ParticleEnergy*MeV);

	theSimParemeters->SetBeam(*theBeam);

	/*Out Path*/
	NWGlobal::GetInstance()->InitialGlobal(simMode,*theSimParemeters);

	NWGlobal::GetInstance()->PrintInfo();

	/*Random number*/
	G4Random::setTheEngine(new CLHEP::RanecuEngine);

	G4RunManager* runManager = new G4RunManager();

	/*Geomerty*/
	NWGeometry* myGeometry = new NWGeometry();

	runManager->SetUserInitialization(myGeometry);

	G4VModularPhysicsList* physicList = new QGSP_BIC_HP;

	runManager->SetUserInitialization(physicList);

	runManager->SetUserInitialization(new NWActionInitialization());

	runManager->Initialize();

	runManager->BeamOn(NWGlobal::GetInstance()->GetEventLoops());

	//if (myGeometry) delete myGeometry;

	NWGlobal::GetInstance()->ofsSimRecord.flush();

	//NWAnalysis::GetInstance()->AnalysisResult(NWInfoStore::GetInstance()->GetEventsInfo());

	if (runManager) delete runManager;

	delete theSimParemeters;

	delete theBeam;
}

int main(int argc, char* argv[]) {

	int EventLoops;
	double ParticleEnergy;
	stringstream ss;

	if (argc < 3) {
		std::cout << "You must special the event loops that you run and the particle energy" << std::endl;
		getchar();
		exit(1);
	}

	ss << argv[1];

	ss >> EventLoops;

	std::cout << "The event loops is: " << EventLoops <<std::endl;
	
	stringstream().swap(ss);
	ss.clear();

	ss << argv[2];

	ss >> ParticleEnergy;

	std::cout << "The particle energy is: " << ParticleEnergy <<" MeV"<< std::endl;

	stringstream().swap(ss);
	ss.clear();

	RunTheSimulation(EventLoops, ParticleEnergy);

	return 0;
}