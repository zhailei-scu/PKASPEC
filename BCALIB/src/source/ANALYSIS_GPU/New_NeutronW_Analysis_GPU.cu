#include <iostream>
#include "NWGlobal.h"
#include "NWAnalysis_GPU.cuh"
#include "NWSimParameters.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <sstream>

void AnalysisTheResult(int argc, char* argv[]) {
	/*Local Vars*/
	stringstream ss;
	int deviceCount;
	int currentDevice;
	int selectedDeviceStart;
	int numberOfSelectedDevice;
	cudaError theError;
	/*Body*/

	cudaGetDeviceCount(&deviceCount);

	cudaGetDevice(&currentDevice);

	NWAnalysis_GPU analysis;

	if (argc < 3) {
		std::cout << "The argument number is: " << argc << std::endl;
		std::cout << "You must special the 1) the original file path" << std::endl;
		std::cout << "You must special the 2) the start of selectted GPU ID" << std::endl;
		std::cout << "You must special the 3) the number of selectted GPU ID" << std::endl;
		system("pause");
		exit(1);
	}
	std::string originalDataFilePath;

	originalDataFilePath = std::string(argv[1]);

	ss.clear();
	ss.str("");
	ss << argv[2];
	ss >> selectedDeviceStart;

	ss.clear();
	ss.str("");
	ss << argv[3];
	ss >> numberOfSelectedDevice;

	if ((selectedDeviceStart + numberOfSelectedDevice) < 0 || (selectedDeviceStart + numberOfSelectedDevice) >= deviceCount) {
		std::cout << "You can only use the device from 0 to "<< numberOfSelectedDevice-1 <<std::endl;
		getchar();
		exit(1);
	}

	theError = cudaSetDevice(selectedDeviceStart);
	if (theError != cudaSuccess) {
		std::cout << "Set device "<< selectedDeviceStart << " error " << std::endl;
		getchar();
		exit(1);
	}

	NWGlobal::GetInstance()->InitialGlobal(analysisMode);

	NWInfoStore::GetInstance()->ReadEventsInfo(originalDataFilePath);

	std::cout << "Read done, execute the new analysis..." << std::endl;

	analysis.AnalysisResult(NWInfoStore::GetInstance()->GetEventsInfo());
}

int main(int argc, char* argv[]) {

	AnalysisTheResult(argc, argv);

	return 0;
}
