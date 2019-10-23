#ifndef NWANALYSIS_H
#define NWANALYSIS_H

#include "NWInfoStore.h"
#include <fstream>

class NWAnalysis {
private:
	NWAnalysis();

	static NWAnalysis *NWAnalysisInstance;
public:
	~NWAnalysis();

	static NWAnalysis* GetInstance();

	static void ReleaseInstance();

	void AnalysisResult(std::map<int, std::vector<TrackInfo>>* storedData);

	void Cal_MinDist_LinkedCell(std::map<int, std::vector<TrackInfo>>* storedData,double boundary[][2], int ceilingNum_OneDim, std::fstream* ofsAnalysisPath_DistanceXYZ, fstream *ofsAnalysisPath_DistanceXYZ_ZONE);

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
