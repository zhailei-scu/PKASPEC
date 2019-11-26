#include "NWGlobal.h"
#include <iomanip>
#include <sstream>

/*The singleton*/

NWGlobal* NWGlobal::nWGlobalInstance = new NWGlobal();


NWGlobal::NWGlobal() {
	this->nWGlobalInstance = NULL;
}

NWGlobal::~NWGlobal() {

	if(ofsSimRecord.is_open()) this->ofsSimRecord.close();
	if(ofs_Iso.is_open()) this->ofs_Iso.close();
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
	std::string outFile_Iso;
	ss.clear();
	ss.str("");

	this->simParamters.SetDefulatValue();

	OutWidth = this->simParamters.GetOutWidth();

	if (0 == mode.compare(simMode)) {

		if (this->simParamters.GetTheConcentReaction() == ConcentReaction(InletToLastEst) ||
			this->simParamters.GetTheConcentReaction() == ConcentReaction(InletToFirstNonEst) ||
			this->simParamters.GetTheConcentReaction() == ConcentReaction(InletEstAndInEstTillEnd) ||
			this->simParamters.GetTheConcentReaction() == ConcentReaction(MatrixAtom)) {

			if (this->simParamters.GetOutPath()->length() > 0) {
				ss << this->simParamters.GetOutPath()->c_str() << "\\" << "OutPutResult.txt";
			}
			else {
				ss << "OutPutResult.txt";
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
				<< std::setw(OutWidth) << "ParticleName"
				<< std::setw(OutWidth) << "TrackStatus"
				<< std::endl;

		}

		if (this->simParamters.GetTheConcentReaction() == ConcentReaction(Iso)) {

			ss.clear();
			ss.str("");

			if (this->simParamters.GetOutPath()->length() > 0) {
				ss << this->simParamters.GetOutPath()->c_str() << "\\" << "OutIso.txt";
			}
			else {
				ss << "OutIso.txt";
			}

			ss >> outFile_Iso;

			ofs_Iso.open(outFile_Iso, std::ios::out | std::ios::ate);



			ofs_Iso
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
				<< std::setw(OutWidth) << "AtomNumber"
				<< std::setw(OutWidth) << "BaryonNum"
				<< std::setw(OutWidth) << "TrackStatus"
				<< std::endl;
		}


	}


}

void NWGlobal::PrintInfo(){
	this->simParamters.PrintParameters();
}

NWSimParameters const & NWGlobal::GetSimParamters() const{
	return this->simParamters;
}


NWGlobal::NWGlobalGarbo::NWGlobalGarbo() {

}


NWGlobal::NWGlobalGarbo::~NWGlobalGarbo() {
	relaseInstance();
}


