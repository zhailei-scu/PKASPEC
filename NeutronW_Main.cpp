#include <iostream>
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "NWGeometry.h"
#include "NWActionInitialization.h"
#include "G4VModularPhysicsList.hh"
#include "QGSP_BIC_HP.hh"
#include "NWGlobal.h"
#include "NWInfoStore.h"
#include "NWAnalysis.h"
#include <sstream>

void RunTheSimulation(int argc, char* argv[]) {

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

	ss >> NWGlobal::GetInstance()->EventLoopsNumber;

	/*Particle name*/
	NWGlobal::GetInstance()->gunParticleName = std::string(argv[3]);

	/*Particle energy*/
	ss.clear();
	ss.str("");

	ss << argv[4];

	ss >> NWGlobal::GetInstance()->gunEnergy;

	/*Target Material*/
	NWGlobal::GetInstance()->targetMaterial = std::string(argv[5]);

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

	runManager->BeamOn(NWGlobal::GetInstance()->EventLoopsNumber);

	//if (myGeometry) delete myGeometry;

	NWGlobal::GetInstance()->ofsSimRecord.flush();

	//NWAnalysis::GetInstance()->AnalysisResult(NWInfoStore::GetInstance()->GetEventsInfo());

	if (runManager) delete runManager;
}


void AnalysisTheResult(int argc, char* argv[]) {

	if (argc < 5) {
		std::cout << "The argument number is: " << argc << std::endl;
		std::cout << "You must special the 1) model \n \
										   2) the original file path \n \
										   3) OutPath \n \
										   4) the bin number for each 10 power"  << std::endl;
		system("pause");
		exit(1);
	}
	std::string originalDataFilePath;

	std::stringstream ss;

	ss.clear();
	ss.str("");

	originalDataFilePath = std::string(argv[2]);

	NWGlobal::GetInstance()->InitialGlobal(argv[1], argv[3]);

	ss << argv[4];

	/*Here, we only care about hadElastic process*/
	NWAnalysis::GetInstance()->targetProcessName = std::string("hadElastic");

	ss >> NWAnalysis::GetInstance()->BinNumberEachPower;

	NWInfoStore::GetInstance()->ReadEventsInfo(originalDataFilePath);

	std::cout << "Read done..." << std::endl;

	NWAnalysis::GetInstance()->AnalysisResult(NWInfoStore::GetInstance()->GetEventsInfo());

}

int main(int argc, char* argv[]) {

	if (argc < 2) {
		std::cout << "The argument number is: " << argc << std::endl;
		std::cout << "You must special the running model " << std::endl;
		std::cout << "either 'sim' or 'analysis' " << std::endl;
		system("pause");
		exit(1);
	}

	if (0 == simMode.compare(argv[1])) {
		RunTheSimulation(argc, argv);
	}
	else if (0 == analysisMode.compare(argv[1])) {
		AnalysisTheResult(argc, argv);
	}
	else {
		std::cout << "Unknown mode: " << argv[1] << std::endl;
		system("pause");
		exit(1);
	}

	return 0;
}