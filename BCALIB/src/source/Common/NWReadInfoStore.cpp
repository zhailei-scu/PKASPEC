#include "NWReadInfoStore.h"
#include "NWGlobal.h"
#include <fstream>

using namespace std;

StepInfo::StepInfo(int theStepID) {
	this->stepID = theStepID;
}


StepInfo::~StepInfo() {
	this->stepID = -1;
	this->originPosition = G4ThreeVector(0, 0, 0);
	this->prePosition = G4ThreeVector(0, 0, 0);
	this->postPosition = G4ThreeVector(0, 0, 0);
	this->preEng = 0.0;
	this->postEng = 0.0;
	this->deltaEng = 0.0;
	this->deltaTime = 0.0;
	this->processName.clear();
	std::string().swap(this->processName);
}

QGSP_BIC_HP_Process StepInfo::ConvertToProcessID(std::string processName) {
	QGSP_BIC_HP_Process theResult;

	if (0 == processName.compare("nFission")) {
		theResult = QGSP_BIC_HP_Process(nFission);
	}
	else if (0 == processName.compare("nCapture")) {
		theResult = QGSP_BIC_HP_Process(nCapture);
	}
	else if (0 == processName.compare("neutronInelastic")) {
		theResult = QGSP_BIC_HP_Process(neutronInelastic);
	}
	else if (0 == processName.compare("hadElastic")) {
		theResult = QGSP_BIC_HP_Process(hadElastic);
	}
	else if (0 == processName.compare("Decay")) {
		theResult = QGSP_BIC_HP_Process(Decay);
	}
	else if (0 == processName.compare("Transportation")) {
		theResult = QGSP_BIC_HP_Process(Transportation);
	}
	else {
		std::cout << "Unkonown process name in QGSP_BIC_HP model: " << processName << std::endl;
		system("pause");
		exit(1);
	}

	return theResult;
}


std::string StepInfo::ConvertToProcessName(const QGSP_BIC_HP_Process processID) {
	std::string processName;

	if (processID == QGSP_BIC_HP_Process(nFission)) {
		processName = std::string("nFission");
	}
	else if (processID == QGSP_BIC_HP_Process(nCapture)) {
		processName = std::string("nCapture");
	}
	else if (processID == QGSP_BIC_HP_Process(neutronInelastic)) {
		processName = std::string("neutronInelastic");
	}
	else if (processID == QGSP_BIC_HP_Process(hadElastic)) {
		processName = std::string("hadElastic");
	}
	else if (processID == QGSP_BIC_HP_Process(Decay)) {
		processName = std::string("Decay");
	}
	else if (processID == QGSP_BIC_HP_Process(Transportation)) {
		processName = std::string("Transportation");
	}
	else {
		std::cout << "Unkonown process ID in QGSP_BIC_HP model: " << processID << std::endl;
		system("pause");
		exit(1);
	}

	return processName;
}


TrackInfo::TrackInfo(int theTrackID) {
	this->trackID = theTrackID;
	this->stepsInfo.clear();
	std::vector<StepInfo>().swap(this->stepsInfo);
}

TrackInfo::~TrackInfo() {
	this->trackID = -1;

	//note that clear function cannot release the memory 
	std::vector<StepInfo>().swap(this->stepsInfo);
}

EventInfo::EventInfo() {
	this->EventID = 0;
	this->tracksInfo.clear();
	std::vector<TrackInfo>().swap(this->tracksInfo);
}

EventInfo::EventInfo(int theEventID) {
	this->EventID = theEventID;
	this->tracksInfo.clear();
	std::vector<TrackInfo>().swap(this->tracksInfo);
}

EventInfo::~EventInfo() {
	this->EventID = -1;
	this->tracksInfo.clear();
	std::vector<TrackInfo>().swap(this->tracksInfo);
}



/*The singleton instance */
NWInfoStore* NWInfoStore::nWInfoStoreInstance = new NWInfoStore();


NWInfoStore::NWInfoStore() {
	this->EventsInfo.clear();
	std::map<int, std::vector<TrackInfo>>().swap(EventsInfo);
	this->nWInfoStoreInstance = NULL;
}


NWInfoStore::~NWInfoStore() {
	std::map<int, std::vector<TrackInfo>>().swap(EventsInfo);
}

NWInfoStore* NWInfoStore::GetInstance() {
	return nWInfoStoreInstance;
}

void NWInfoStore::ReleaseInstance() {

	if (NULL != nWInfoStoreInstance) {
		delete nWInfoStoreInstance;
		nWInfoStoreInstance = NULL;
	}
}


void NWInfoStore::ReadEventsInfo(std::string path) {

	int EventID;
	int TrackID;
	int StepID;
	double preEng;
	double postEng;
	double deltaEng;
	double deltaTime;
	double originVector_x, originVector_y, originVector_z;
	double originstep_x, originstep_y, originstep_z;
	double prestep_x, prestep_y, prestep_z;
	double poststep_x, poststep_y, poststep_z;
	std::string processname;
	std::string particleName;
	std::stringstream ss;
	std::string tempBuf;
	int trackStatus;
	int AtomNumber;
	int BaryonNum;
	int trackIndex;
	int dumpCountTrackInfo;

	std::fstream ofs;

	ofs.open(path, std::ios::in);

	getline(ofs, tempBuf); // the first line

	while (getline(ofs, tempBuf)) {

		ss.clear();
		ss.str("");

		ss << tempBuf;

		if (tempBuf.size() < 1) {
			continue;
		}

		processname.clear();
		std::string().swap(processname);

		if (ConcentReaction(Iso) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

			ss >> EventID
				>> TrackID
				>> StepID
				>> preEng
				>> postEng
				>> deltaEng
				>> deltaTime
				>> originVector_x >> originVector_y >> originVector_z
				>> originstep_x >> originstep_y >> originstep_z
				>> prestep_x >> prestep_y >> prestep_z
				>> poststep_x >> poststep_y >> poststep_z
				>> processname
				>> particleName
				>> AtomNumber
				>> BaryonNum
				>> trackStatus;
		}
		else {
			ss >> EventID
				>> TrackID
				>> StepID
				>> preEng
				>> postEng
				>> deltaEng
				>> deltaTime
				>> originVector_x >> originVector_y >> originVector_z
				>> originstep_x >> originstep_y >> originstep_z
				>> prestep_x >> prestep_y >> prestep_z
				>> poststep_x >> poststep_y >> poststep_z
				>> processname
				>> particleName
				>> trackStatus;
		}


		if (0 == EventID % 100) {
			std::cout << EventID << std::endl;
		}

		if (0 == NWInfoStore::GetInstance()->GetEventsInfo()->count(EventID)) {

			NWInfoStore::GetInstance()->GetEventsInfo()->insert(map<int, vector<TrackInfo>>::value_type(EventID, vector<TrackInfo>()));
		}

		map<int, vector<TrackInfo>>::iterator it = NWInfoStore::GetInstance()->GetEventsInfo()->find(EventID);

		vector<TrackInfo>* trackVector = &(it->second);

		trackIndex = -1;

		dumpCountTrackInfo = 0;

		for (vector<TrackInfo>::iterator itera = trackVector->begin(); itera != trackVector->end(); itera++) {
			if (itera->GetTrackID() == TrackID) {
				dumpCountTrackInfo++;

				trackIndex++;

				if (dumpCountTrackInfo > 1) {
					cout << "There are dumplicate case for event: " << EventID
						<< "track : " << TrackID << endl;
					system("pause");
				}
			}
		}

		TrackInfo *tempTrackInfo = NULL;

		if (trackIndex < 0) {
			trackVector->push_back(TrackInfo(TrackID));

			trackIndex = trackVector->size() - 1;
		}
		tempTrackInfo = &trackVector->at(trackIndex);

		if (tempTrackInfo->GetStepsInfo()->size()) {
			for (vector<StepInfo>::iterator itStep = tempTrackInfo->GetStepsInfo()->begin(); itStep != tempTrackInfo->GetStepsInfo()->end(); itStep++) {
				if (itStep->GetStepID() == StepID) {
					cout << "There are dumplicate case for event: " << EventID
						<< " track : " << TrackID
						<< " step : " << StepID << endl;
					system("pause");
				}
			}
		}

		StepInfo tempStepInfo(StepID);

		tempStepInfo.SetpreEng(preEng);
		tempStepInfo.SetpostEng(postEng);
		tempStepInfo.SetDeltaEng(deltaEng);
		tempStepInfo.SetDeltaTime(deltaTime);
		tempStepInfo.SetOriginDirection(G4ThreeVector(originVector_x, originVector_y, originVector_z));
		tempStepInfo.SetOriginPosition(G4ThreeVector(originstep_x, originstep_y, originstep_z));
		tempStepInfo.SetprePosition(G4ThreeVector(prestep_x, prestep_y, prestep_z));
		tempStepInfo.SetpostPosition(G4ThreeVector(poststep_x, poststep_y, poststep_z));
		tempStepInfo.SetProcessName(processname);
		tempStepInfo.SetParticleName(particleName);
		tempStepInfo.SetAtomNum(AtomNumber);
		tempStepInfo.SetBaryonNum(BaryonNum);

		tempTrackInfo->GetStepsInfo()->push_back(tempStepInfo);
	}

	ofs.close();
}


NWInfoStore::NWInfoStoreGarbo::NWInfoStoreGarbo() {

}


NWInfoStore::NWInfoStoreGarbo::~NWInfoStoreGarbo() {

	ReleaseInstance();
}