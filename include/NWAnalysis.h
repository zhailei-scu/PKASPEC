#ifndef NWANALYSIS_H
#define NWANALYSIS_H

#include "NWInfoStore.h"

class NWAnalysis {
private:
	NWAnalysis();

	static NWAnalysis *NWAnalysisInstance;
public:
	~NWAnalysis();

	static NWAnalysis* GetInstance();

	static void ReleaseInstance();

	void AnalysisResult(std::map<int, std::vector<TrackInfo>>* storedData);

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

};



#endif
