#ifndef NWANALYSIS_H
#define NWANALYSIS_H

#define MAXCELLNUM_X 885
#define MAXCELLNUM_Y 885
#define MAXCELLNUM_Z 10
#define MAXCELLNUM MAXCELLNUM_X*MAXCELLNUM_Y*MAXCELLNUM_Z
#define MAXLENONELINE 1024

#include "NWReadInfoStore.h"


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
		std::fstream* ofsAnalysisPath_DistanceXYZ, std::fstream *ofsAnalysisPath_linkedCellPosition,
		std::fstream* ofsAnalysisPath_ZoneCount, std::fstream* ofsAnalysisPath_CeilCount);


	void New_AnalysisResult(std::map<int, std::vector<TrackInfo>>* storedData);

	void New_Cal_MinDist_LinkedCell(std::map<int, std::vector<TrackInfo>>* storedData, double boundary[][2],
		std::fstream* ofsAnalysisPath_DistanceXYZ, std::fstream *ofsAnalysisPath_linkedCellPosition,
		std::fstream* ofsAnalysisPath_ZoneCount, std::fstream* ofsAnalysisPath_CeilCount);


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
