#include <iostream>
#include "NWGlobal.h"
#include "NWInfoStore.h"
#include "NWAnalysis.h"
#include <sstream>


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

	AnalysisTheResult(argc, argv);
	
	return 0;
}