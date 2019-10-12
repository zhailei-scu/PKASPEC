#ifndef NWINFOSTORE_H
#define NWINFOSTORE_H

#include <stdlib.h>
#include <map>
#include <vector>
#include "G4ThreeVector.hh"


#define MAXLENONELINE 1024

class StepInfo {
public:
	StepInfo() = delete;
	StepInfo(int stepID);
	~StepInfo();

private:
	int stepID;

	G4ThreeVector prePosition;
	G4ThreeVector postPosition;
	double preEng;
	double postEng;

public:

	inline int GetStepID(){
		return this->stepID;
	}

	inline void SetprePosition(G4ThreeVector prePosition) {
		this->prePosition = prePosition;
	}

	inline G4ThreeVector GetprePosition() {
		return this->prePosition;
	}

	inline void SetpostPosition(G4ThreeVector postPosition) {
		this->postPosition = postPosition;
	}

	inline G4ThreeVector GetpostPosition() {
		return this->postPosition;
	}

	inline void SetpreEng(double preEng) {
		this->preEng = preEng;
	}

	inline double GetpreEng() {
		return this->preEng;
	}

	inline void SetpostEng(double postEng) {
		this->postEng = postEng;
	}

	inline double GetpostEng() {
		return this->postEng;
	}
};


class TrackInfo {
public:
	TrackInfo() = delete;
	TrackInfo(int trackID);
	~TrackInfo();

private:
	int trackID;

	std::vector<StepInfo> stepsInfo;

public:
	inline int GetTrackID() {
		return this->trackID;
	}

	inline std::vector<StepInfo>* GetStepsInfo() {
		return &this->stepsInfo;
	}

};

class EventInfo {
public:
	EventInfo() = delete;
	EventInfo(int EventID);
	~EventInfo();
private:
	int EventID;
	std::vector<TrackInfo> tracksInfo;

public:

	inline int GetEventID() {
		return this->EventID;
	}

	inline std::vector<TrackInfo>* GetTracksInfo() {
		return &this->tracksInfo;
	}
};

class NWInfoStore {
private:
	NWInfoStore();

	NWInfoStore(const NWInfoStore &r) = delete;

	NWInfoStore operator = (const NWInfoStore &r) = delete;

	static NWInfoStore* nWInfoStoreInstance;
public:
	~NWInfoStore();

	static NWInfoStore* GetInstance();

	static void ReleaseInstance();

public:
	class NWInfoStoreGarbo {
	public:
		NWInfoStoreGarbo();
		~NWInfoStoreGarbo();
	};

	static NWInfoStoreGarbo nWInfoStoreGarboInstance;

private:
	std::map<int, std::vector<TrackInfo>> EventsInfo;

public:

	inline std::map<int, std::vector<TrackInfo>>* GetEventsInfo() {
		return &this->EventsInfo;
	}

	void ReadEventsInfo(std::string path);
};



#endif
