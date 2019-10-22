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

void RunTheSimulation() {

	/*Out Path*/
	NWGlobal::GetInstance()->InitialGlobal();

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

	RunTheSimulation();

	return 0;
}