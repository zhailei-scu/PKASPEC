#ifndef NWREADINFOSTIRE_H
#define NWREADINFOSTIRE_H


#include <stdlib.h>
#include <map>
#include <vector>
#include "G4ThreeVector.hh"

enum QGSP_BIC_HP_Process
{
	nFission,
	nCapture,
	neutronInelastic,
	hadElastic,
	Decay,
	Transportation
};

class StepInfo {
public:
	StepInfo() = delete;
	StepInfo(int stepID);
	~StepInfo();

private:
	int stepID;

	double preEng;
	double postEng;
	double deltaEng;
	double deltaTime;
	G4ThreeVector originDirection;
	G4ThreeVector originPosition;
	G4ThreeVector prePosition;
	G4ThreeVector postPosition;
	std::string processName;
	std::string particleName;
	int AtomNum;
	int BaryonNum;

public:

	inline int GetStepID() const {
		return this->stepID;
	}

	inline void SetpreEng(double thePreEng) {
		this->preEng = thePreEng;
	}

	inline double GetpreEng() const {
		return this->preEng;
	}

	inline void SetpostEng(double thePostEng) {
		this->postEng = thePostEng;
	}

	inline double GetpostEng() const {
		return this->postEng;
	}

	inline void SetDeltaEng(double theDeltaEng) {
		this->deltaEng = theDeltaEng;
	}

	inline double GetDeltaEng() const {
		return this->deltaEng;
	}

	inline void SetDeltaTime(double theDeltaTime) {
		this->deltaTime = theDeltaTime;
	}

	inline double GetDeltaTime() const {
		return this->deltaTime;
	}

	inline void SetOriginDirection(G4ThreeVector theOriginDirection) {
		this->originDirection = theOriginDirection;
	}

	inline G4ThreeVector GetOriginDirection() const {
		return this->originDirection;
	}

	inline void SetOriginPosition(G4ThreeVector theOriginPosition) {
		this->originPosition = theOriginPosition;
	}

	inline G4ThreeVector GetOriginPosition() const {
		return this->originPosition;
	}

	inline void SetprePosition(G4ThreeVector thePrePosition) {
		this->prePosition = thePrePosition;
	}

	inline G4ThreeVector GetprePosition() const {
		return this->prePosition;
	}

	inline void SetpostPosition(G4ThreeVector thePostPosition) {
		this->postPosition = thePostPosition;
	}

	inline G4ThreeVector GetpostPosition() const {
		return this->postPosition;
	}


	inline void SetProcessName(std::string theProcessName) {
		this->processName = theProcessName;
	}

	inline std::string GetProcessName() const {
		return this->processName;
	}


	inline void SetParticleName(std::string theParticleName) {
		this->particleName = theParticleName;
	}

	inline std::string GetParticleName() const {
		return this->particleName;
	}

	inline int GetAtomNum() {
		return this->AtomNum;
	}

	inline void SetAtomNum(int theAtomNum) {
		this->AtomNum = theAtomNum;
	}

	inline int GetBaryonNum() {
		return this->BaryonNum;
	}

	inline void SetBaryonNum(int theBaryonNum) {
		this->BaryonNum = theBaryonNum;
	}


	static QGSP_BIC_HP_Process ConvertToProcessID(std::string processName);

	static std::string  ConvertToProcessName(const QGSP_BIC_HP_Process processID);
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
	EventInfo();
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