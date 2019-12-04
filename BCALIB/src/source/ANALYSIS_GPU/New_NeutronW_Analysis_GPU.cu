#include <iostream>
#include "NWGlobal.h"
#include "NWAnalysis_GPU.cuh"
#include "NWSimParameters.h"
#include <sstream>

void AnalysisTheResult(int argc, char* argv[]) {

	NWAnalysis_GPU analysis;

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

	analysis.AnalysisResult(NWInfoStore::GetInstance()->GetEventsInfo());

}

int main(int argc, char* argv[]) {

	AnalysisTheResult(argc, argv);

	return 0;
}
