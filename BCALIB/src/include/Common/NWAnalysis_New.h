#ifndef NWANALYSIS_NEW_H
#define NWANALYSIS_NEW_H


#include "NWAnalysis.h"


class NWAnalysis_New : public NWAnalysis{
public:
	NWAnalysis_New();
public:
	~NWAnalysis_New();

	void AnalysisResult(std::map<int, std::vector<TrackInfo>>* storedData);
};



#endif
