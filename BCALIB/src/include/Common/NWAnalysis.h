#ifndef NWANALYSIS_H
#define NWANALYSIS_H

#define MAXCELLNUM_X 885
#define MAXCELLNUM_Y 885
#define MAXCELLNUM_Z 10
#define MAXCELLNUM MAXCELLNUM_X*MAXCELLNUM_Y*MAXCELLNUM_Z
#define MAXLENONELINE 1024

#include "NWReadInfoStore.h"


class NWAnalysis {
public:
	NWAnalysis();

	~NWAnalysis();

	virtual void AnalysisResult(std::map<int, std::vector<TrackInfo>>* storedData);

public:

	int BinNumberEachPower = 100;

	double minBin = 0.01;
	double maxBin = 200;

	double minPrecision = 1e-7;

};



#endif
