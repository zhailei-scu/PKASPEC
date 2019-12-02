#include <iostream>
#include "NWGlobal.h"
#include "NWAnalysis.h"
#include "NWSimParameters.h"
#include <sstream>

void AnalysisTheResult(int argc, char* argv[]) {

	if (argc < 2) {
		std::cout << "The argument number is: " << argc << std::endl;
		std::cout << "You must special the 1) the original file path" << std::endl;
		system("pause");
		exit(1);
	}
	std::string originalDataFilePath;

	originalDataFilePath = std::string(argv[1]);

	NWGlobal::GetInstance()->InitialGlobal(analysisMode);

	NWInfoStore::GetInstance()->ReadEventsInfo(originalDataFilePath);

	std::cout << "Read done, execute the new analysis..." << std::endl;

	NWAnalysis::GetInstance()->New_AnalysisResult(NWInfoStore::GetInstance()->GetEventsInfo());

}

int main(int argc, char* argv[]) {

	AnalysisTheResult(argc, argv);

	return 0;
}