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

		ofsSimRecord 
			<< std::setw(OutWidth) << "EventID"    
			<< std::setw(OutWidth) << "TrackID"    
			<< std::setw(OutWidth) << "StepID"
			<< std::setw(OutWidth) << "PreEng(MeV)"     
			<< std::setw(OutWidth) << "PostEng(MeV)"   
			<< std::setw(OutWidth) << "DeltaEng(MeV)"
			<< std::setw(OutWidth) << "DeltaTime(ns)"
			<< std::setw(OutWidth) << "OriginDirection_x"
			<< std::setw(OutWidth) << "OriginDirection_y"
			<< std::setw(OutWidth) << "OriginDirection_z"
			<< std::setw(OutWidth) << "OriginPos_x(mm)"
			<< std::setw(OutWidth) << "OriginPos_y(mm)"
			<< std::setw(OutWidth) << "OriginPos_z(mm)"
			<< std::setw(OutWidth) << "PrePos_x(mm)"  
			<< std::setw(OutWidth) << "PrePos_y(mm)"  
			<< std::setw(OutWidth) << "PrePos_z(mm)"
			<< std::setw(OutWidth) << "PostPos_x(mm)" 
			<< std::setw(OutWidth) << "PostPos_y(mm)" 
			<< std::setw(OutWidth) << "PostPos_z(mm)" 
			<< std::setw(OutWidth) << "ProcessName"
			<< std::setw(OutWidth) << "TrackStatus"
			<< std::endl;
	}
}


NWGlobal::NWGlobalGarbo::NWGlobalGarbo() {

}


NWGlobal::NWGlobalGarbo::~NWGlobalGarbo() {
	relaseInstance();
}


