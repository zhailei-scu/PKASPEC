#include "NWGlobal.h"
#include <iomanip>
#include <sstream>

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

void NWGlobal::InitialGlobal(const std::string& mode) {

	int OutWidth;
	std::stringstream ss;
	std::string outFile;
	ss.clear();
	ss.str("");

	this->simParamters.SetDefulatValue();

	if (0 == mode.compare(simMode)) {
		if (this->simParamters.GetOutPath()->length() > 0) {
			ss << this->simParamters.GetOutPath()->c_str() << "\\" << "OutPutResult.txt";
		}
		else {
			ss << "OutPutResult.txt";
		}

		ss >> outFile;

		ofsSimRecord.open(outFile, std::ios::out | std::ios::ate);

		OutWidth = this->simParamters.GetOutWidth();

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
			<< std::setw(OutWidth) << "ParticleName"
			<< std::setw(OutWidth) << "TrackStatus"
			<< std::endl;
	}


}

void NWGlobal::PrintInfo(){
	this->simParamters.PrintParameters();
}

NWSimParameters * NWGlobal::GetSimParamters() {
	return &this->simParamters;
}


NWGlobal::NWGlobalGarbo::NWGlobalGarbo() {

}


NWGlobal::NWGlobalGarbo::~NWGlobalGarbo() {
	relaseInstance();
}


