#include <iostream>
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "NWGeometry.h"
#include "NWActionInitialization.h"
#include "G4VModularPhysicsList.hh"
#include "QGSP_BIC_HP.hh"
#include "NWGlobal.h"
#include <sstream>

void RunTheSimulation(int argc, char* argv[]) {

	int EventLoopsNumber;
	double gunEnergy;

	if (argc < 7) {
		std::cout << "The argument number is: " << argc << std::endl;
		std::cout << "You must special the 1) model \n \
										   2) number of Events \n  \
										   3) particle name,  \n  \
										   4) particle energy \n  \
										   5) target materials \n \
										   6) OutPath ." << std::endl;
		system("pause");
		exit(1);
	}

	std::stringstream ss;

	/*Event number*/
	ss.clear();
	ss.str("");
	ss << argv[2];

	ss >> EventLoopsNumber;
	NWGlobal::GetInstance()->GetSimParamters()->SetEventLoopsNumber(EventLoopsNumber);

	/*Particle name*/
	NWGlobal::GetInstance()->GetSimParamters()->SetGunParticleName(std::string(argv[3]));

	/*Particle energy*/
	ss.clear();
	ss.str("");

	ss << argv[4];

	ss >> gunEnergy;
	NWGlobal::GetInstance()->GetSimParamters()->SetGunEnergy(gunEnergy);

	/*Target Material*/
	NWGlobal::GetInstance()->GetSimParamters()->SetTargetMaterial(std::string(argv[5]));

	/*Out Path*/
	NWGlobal::GetInstance()->InitialGlobal(argv[1],argv[6]);

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

	RunTheSimulation(argc, argv);

	return 0;
}