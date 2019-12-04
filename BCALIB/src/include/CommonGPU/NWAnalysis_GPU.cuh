#ifndef NWANALYSIS_GPU_H
#define NWANALYSIS_GPU_H

#define BLOCKSIZE 512
#define GRIDDIMX 64;

#include "NWAnalysis.h"

struct ThreeVector
{
	double x;
	double y;
	double z;
};


class NWAnalysis_GPU : public NWAnalysis {
public:
	NWAnalysis_GPU();
public:
	~NWAnalysis_GPU();

	void AnalysisResult(std::map<int, std::vector<TrackInfo>>* storedData);
};



#endif
