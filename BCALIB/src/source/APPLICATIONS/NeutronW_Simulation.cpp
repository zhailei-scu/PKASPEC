#include <iostream>
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "NWGeometry.h"
#include "NWActionInitialization.h"
#include "G4VModularPhysicsList.hh"
#include "QGSP_BIC_HP.hh"
#include "NWGlobal.h"
#include "NWSimParameters.h"
#include <sstream>
#include "NWInfoStore.h"
#include "../../../../LIB/src/include/MiniUtilities/EXNUMB.h"

void RunTheSimulation(int EventLoops) {

	NWGlobal::GetInstance()->GetSimParamters()->SetEventLoopsNumber(EventLoops);

	/*Out Path*/
	NWGlobal::GetInstance()->InitialGlobal(simMode);

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

	runManager->BeamOn(NWGlobal::GetInstance()->GetSimParamters()->GetEventLoopsNumber());

	//if (myGeometry) delete myGeometry;

	NWGlobal::GetInstance()->ofsSimRecord.flush();

	//NWAnalysis::GetInstance()->AnalysisResult(NWInfoStore::GetInstance()->GetEventsInfo());

	if (runManager) delete runManager;
}

int main(int argc, char* argv[]) {

	int EventLoops;
	stringstream ss;

	if (argc < 2) {
		std::cout << "You must special the event loops that you run" << std::endl;
		system("pause");
		exit(1);
	}

	ss << argv[1];

	ss >> EventLoops;

	std::cout << "The event loops is: " << EventLoops <<std::endl;


	RunTheSimulation(EventLoops);

	return 0;
}