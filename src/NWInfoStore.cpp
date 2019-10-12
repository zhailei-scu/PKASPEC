#include "NWInfoStore.h"
#include "NWGlobal.h"
#include <fstream>
#include <map>
#include <vector>

using namespace std;

StepInfo::StepInfo(int stepID) {
	this->stepID = stepID;
}


StepInfo::~StepInfo() {
	this->stepID = -1;
	this->prePosition = G4ThreeVector(0, 0, 0);
	this->postPosition = G4ThreeVector(0,0,0);
	this->preEng = 0.0;
	this->postEng = 0.0;
}


TrackInfo::TrackInfo(int trackID) {
	this->trackID = trackID;
	this->stepsInfo.clear();
	std::vector<StepInfo>().swap(this->stepsInfo);
}

TrackInfo::~TrackInfo() {
	this->trackID = -1;

	//note that clear function cannot release the memory 
	std::vector<StepInfo>().swap(this->stepsInfo);
}

EventInfo::EventInfo(int EventID) {
	this->EventID = EventID;
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
	EventsInfo.swap(std::map<int, std::vector<TrackInfo>>());
	this->nWInfoStoreInstance = NULL;
}


NWInfoStore::~NWInfoStore() {
	EventsInfo.swap(std::map<int, std::vector<TrackInfo>>());
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
	double prestep_x, prestep_y, prestep_z;
	double poststep_x, poststep_y, poststep_z;
	double preEng;
	double postEng;
	double deltaEng;
	std::stringstream ss;
	std::string tempBuf;

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

		ss >> EventID
			>> TrackID
			>> StepID
			>> preEng
			>> postEng
			>> deltaEng
			>> prestep_x>> prestep_y >> prestep_z
			>> poststep_x >> poststep_y >> poststep_z;


		std::cout << EventID << std::endl;

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
		tempStepInfo.SetprePosition(G4ThreeVector(prestep_x, prestep_y, prestep_z));
		tempStepInfo.SetpostPosition(G4ThreeVector(poststep_x, poststep_y, poststep_z));

		tempTrackInfo->GetStepsInfo()->push_back(tempStepInfo);



	}






	ofs.close();
}


NWInfoStore::NWInfoStoreGarbo::NWInfoStoreGarbo() {

}


NWInfoStore::NWInfoStoreGarbo::~NWInfoStoreGarbo() {

	ReleaseInstance();
}


