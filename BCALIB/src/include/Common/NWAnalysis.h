#ifndef NWANALYSIS_H
#define NWANALYSIS_H

#define MAXCELLNUM 626*626*20
#define MAXLENONELINE 1024

#include <fstream>
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

public:

	inline int GetStepID() const {
		return this->stepID;
	}

	inline void SetpreEng(double preEng) {
		this->preEng = preEng;
	}

	inline double GetpreEng() const {
		return this->preEng;
	}

	inline void SetpostEng(double postEng) {
		this->postEng = postEng;
	}

	inline double GetpostEng() const {
		return this->postEng;
	}

	inline void SetDeltaEng(double deltaEng) {
		this->deltaEng = deltaEng;
	}

	inline double GetDeltaEng() const {
		return this->deltaEng;
	}

	inline void SetDeltaTime(double deltaTime) {
		this->deltaTime = deltaTime;
	}

	inline double GetDeltaTime() const {
		return this->deltaTime;
	}

	inline void SetOriginDirection(G4ThreeVector originDirection) {
		this->originDirection = originDirection;
	}

	inline G4ThreeVector GetOriginDirection() const {
		return this->originDirection;
	}

	inline void SetOriginPosition(G4ThreeVector originPosition) {
		this->originPosition = originPosition;
	}

	inline G4ThreeVector GetOriginPosition() const {
		return this->originPosition;
	}

	inline void SetprePosition(G4ThreeVector prePosition) {
		this->prePosition = prePosition;
	}

	inline G4ThreeVector GetprePosition() const {
		return this->prePosition;
	}

	inline void SetpostPosition(G4ThreeVector postPosition) {
		this->postPosition = postPosition;
	}

	inline G4ThreeVector GetpostPosition() const {
		return this->postPosition;
	}


	inline void SetProcessName(std::string processName) {
		this->processName = processName;
	}

	inline std::string GetProcessName() const {
		return this->processName;
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






class NWAnalysis {
private:
	NWAnalysis();

	static NWAnalysis *NWAnalysisInstance;
public:
	~NWAnalysis();

	static NWAnalysis* GetInstance();

	static void ReleaseInstance();

	void AnalysisResult(std::map<int, std::vector<TrackInfo>>* storedData);

	void Cal_MinDist_LinkedCell(std::map<int, std::vector<TrackInfo>>* storedData,double boundary[][2], 
		std::fstream* ofsAnalysisPath_DistanceXYZ, std::fstream *ofsAnalysisPath_linkedCellPosition);

public:
	class NWAnalysisGarbo {
	public:
		NWAnalysisGarbo();
		~NWAnalysisGarbo();
	};

	static NWAnalysisGarbo NWAnalysisGarboInstance;

public:

	int BinNumberEachPower = 100;

	double minBin = 0.01;
	double maxBin = 200;

	double minPrecision = 1e-7;

public:
	std::string targetProcessName;

};



#endif
