#include <iostream>
#include "NWGlobal.h"
#include "NWAnalysis.h"
#include "NWSimParameters.h"
#include <sstream>

void AnalysisTheResult(int argc, char* argv[]) {

	NWAnalysis analysis;
	double ParticleEnergy;
	stringstream ss;

	if (argc < 3) {
		std::cout << "The argument number is: " << argc << std::endl;
		std::cout << "You must special the 1) the original file path and particle energy" << std::endl;
		getchar();
		exit(1);
	}
	std::string originalDataFilePath;

	originalDataFilePath = std::string(argv[1]);

	stringstream().swap(ss);
	ss.clear();

	ss << argv[2];
	ss >> ParticleEnergy;

	/*Parameters*/
	NWSimParameters* theSimParemeters = new NWSimParameters();

	theSimParemeters->SetDefulatValue();

	NWBeam *theBeam = new NWBeam();

	theBeam->SetGunEnergy(ParticleEnergy*MeV);

	theSimParemeters->SetBeam(*theBeam);

	NWGlobal::GetInstance()->InitialGlobal(analysisMode,*theSimParemeters);

	NWInfoStore::GetInstance()->ReadEventsInfo(originalDataFilePath);

	std::cout << "Read done..." << std::endl;

	analysis.AnalysisResult(NWInfoStore::GetInstance()->GetEventsInfo());

	delete theSimParemeters;
	delete theBeam;
}

int main(int argc, char* argv[]) {

	AnalysisTheResult(argc, argv);
	
	return 0;
}