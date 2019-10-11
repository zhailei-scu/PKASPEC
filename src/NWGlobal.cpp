#include "NWGlobal.h"
#include <iomanip>
#include <sstream>

std::string simMode = std::string("sim");
std::string analysisMode = std::string("analysis");


/*The singleton*/

NWGlobal* NWGlobal::nWGlobalInstance = new NWGlobal();


NWGlobal::NWGlobal() {
	this->nWGlobalInstance = NULL;
}

NWGlobal::~NWGlobal() {
	this->ofsSimRecord.close();
}

void NWGlobal::relaseInstance() {
	if (NULL != nWGlobalInstance) {
		delete nWGlobalInstance;
		nWGlobalInstance = NULL;
	}
}

NWGlobal* NWGlobal::GetInstance() {
	return nWGlobalInstance;
}

void NWGlobal::PrintInfo() {

	if(0 == this->mode.compare(simMode)){
		std::cout << "###  The parameters summary ###" << std::endl;

		std::cout << "The event loops number is: " << this->EventLoopsNumber << std::endl;

		std::cout << "The particle gun name is: " << this->gunParticleName.c_str() << std::endl;

		std::cout << "The particle gun energy is: " << this->gunEnergy << std::endl;

		std::cout << "The target material is: " << this->targetMaterial.c_str() << std::endl;

		std::cout << "The target material is: " << this->targetMaterial.c_str() << std::endl;

		std::cout << "The out path is : " << OutPath.c_str() << std::endl;

		std::cout << "### End of the parameters summary ###" << std::endl;
	}
	else if (0 == this->mode.compare(analysisMode)) {

		std::cout << "###  The parameters summary ###" << std::endl;

		std::cout << "The out path is : " << OutPath.c_str() << std::endl;

		std::cout << "### End of the parameters summary ###" << std::endl;
	}


}

void NWGlobal::InitialGlobal(const char* mode,const  char* outPath) {

	this->mode = std::string(mode);

	this->OutPath = std::string(outPath);

	if (0 == this->mode.compare(simMode)) {
		std::stringstream ss;
		std::string outFile;
		ss.clear();
		ss.str("");

		if (this->OutPath.length() > 0) {
			ss << this->OutPath.c_str() << "\\" << "OutPutResult.txt";
		}
		else {
			ss<< "OutPutResult.txt";
		}

		ss >> outFile;

		ofsSimRecord.open(outFile, std::ios::out | std::ios::ate);

		ofsSimRecord << std::setw(OutWidth) << "EventID" << std::setw(OutWidth) << "TrackID" << std::setw(OutWidth) << "StepID"
			<< std::setw(OutWidth) << "PreEng" << std::setw(OutWidth) << "PostEng" << std::setw(OutWidth) << "DeltaEng"
			<< std::setw(OutWidth) << "PrePos(x)" << std::setw(OutWidth) << "PrePos(y)" << std::setw(OutWidth) << "PrePos(z)"
			<< std::setw(OutWidth) << "PostPos(x)" << std::setw(OutWidth) << "PostPos(y)" << std::setw(OutWidth) << "PostPos(z)" << std::endl;
	}
}


NWGlobal::NWGlobalGarbo::NWGlobalGarbo() {

}


NWGlobal::NWGlobalGarbo::~NWGlobalGarbo() {
	relaseInstance();
}


