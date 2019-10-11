#include "NWAnalysis.h"
#include "NWGlobal.h"
#include "stdio.h"
#include <fstream>
#include <iostream>
#include <iomanip>
/*The singleton instance*/
NWAnalysis* NWAnalysis::NWAnalysisInstance = new NWAnalysis();


NWAnalysis::NWAnalysis() {
	this->NWAnalysisInstance = NULL;
}

NWAnalysis::~NWAnalysis() {

}

NWAnalysis* NWAnalysis::GetInstance() {
	return NWAnalysisInstance;
}

void NWAnalysis::ReleaseInstance() {
	if (!NWAnalysisInstance) {
		delete NWAnalysisInstance;
		NWAnalysisInstance = NULL;
	}
}

void NWAnalysis::AnalysisResult(std::map<int, std::vector<TrackInfo>>* storedData) {
	std::fstream ofsOrignalDistance;
	std::fstream ofsAnalysis;
	std::string OrignalDistancePath;
	std::string AnalysisPath;
	std::vector<double> theDistance;
	double maxDistance = -1.0;
	double minDistance = 1E32;
	int minLog;
	int maxLog;
	int binNum;
	double *binEnds;
	double *score;
	std::stringstream ss;

	ss.clear();

	ss.str("");

	if(NWGlobal::GetInstance()->OutPath.length() >0){
		ss << NWGlobal::GetInstance()->OutPath.c_str() << "\\" << "DistanceResult_OriginDistance.txt";
	}
	else {
		ss << "DistanceResult_OriginDistance.txt";
	}

	ss >> OrignalDistancePath;

	ofsOrignalDistance.open(OrignalDistancePath, std::ios::out | std::ios::ate);

	ss.clear();

	ss.str("");

	if (NWGlobal::GetInstance()->OutPath.length() > 0) {
		ss << NWGlobal::GetInstance()->OutPath.c_str() << "\\" << "DistanceResult_Analysis.txt";
	}
	else {
		ss << "DistanceResult_Analysis.txt";
	}

	ss >> AnalysisPath;

	ofsAnalysis.open(AnalysisPath, std::ios::out | std::ios::ate);

	std::map<int, std::vector<TrackInfo>>::iterator it = storedData->begin();

	for (; it != storedData->end(); it++) {
		std::vector<TrackInfo>::iterator iteratorTrackInfo = it->second.begin();

		for (; iteratorTrackInfo != it->second.end(); iteratorTrackInfo++) {

			std::vector<StepInfo>::iterator iteratorStepInfo = iteratorTrackInfo->GetStepsInfo()->begin();

			if (iteratorTrackInfo->GetStepsInfo()->size() > 0) {

				int index = 0;

				G4ThreeVector prePKAPos;
				G4ThreeVector postPKAPos;

				for (; iteratorStepInfo != iteratorTrackInfo->GetStepsInfo()->end(); iteratorStepInfo++) {
					index++;

					if (index <= 1) {
						prePKAPos = iteratorStepInfo->GetpostPosition();
						postPKAPos = iteratorStepInfo->GetpostPosition();
					}
					else {
						postPKAPos = iteratorStepInfo->GetpostPosition();

						G4ThreeVector pKADist = prePKAPos - postPKAPos;

						double distance = pKADist.mag();

						ofsOrignalDistance << std::setiosflags(std::ios::scientific) << std::setprecision(7) << distance << std::endl;

						prePKAPos = postPKAPos;

						theDistance.push_back(distance);

						maxDistance = std::max(maxDistance, distance);
						minDistance = std::min(minDistance, distance);
					}
				}
			}

		}
	}

	std::cout << "minDistance: " << minDistance << std::endl;
	std::cout << "maxDistance: " << maxDistance << std::endl;


	minLog = std::floor(std::log10(minDistance));
	maxLog = std::ceil(std::log10(maxDistance));


	binNum = (maxLog - minLog)*NWGlobal::GetInstance()->BinNumberEachPower;

	binEnds = new double[binNum];
	score = new double[binNum];

	for (int i = 0; i < binNum; i++) {
		double base = pow(10, minLog + i / NWGlobal::GetInstance()->BinNumberEachPower);
		binEnds[i] = base + (i % NWGlobal::GetInstance()->BinNumberEachPower)*base * 9 / NWGlobal::GetInstance()->BinNumberEachPower;
		score[i] = 0;
	}


	std::vector<double>::iterator itDistance = theDistance.begin();


	for (; itDistance != theDistance.end(); itDistance++) {

		/*
		int  LogPos = std::floor(std::log10(*itDistance));

		int truePos = (LogPos - minLog)*NWGlobal::GetInstance()->BinNumberEachPower
			+ floor(NWGlobal::GetInstance()->BinNumberEachPower*((*itDistance) / pow(10, LogPos)) / 9);
		
		score[truePos]++;
		*/

		
		for (int i = 0; i < binNum; i++) {

			if (*itDistance < binEnds[i]) {
				score[i]++;
				break;
			}
		}
		
	}


	for (int i = 0; i < binNum; i++) {

		score[i] = score[i] / pow(10, i / NWGlobal::GetInstance()->BinNumberEachPower);

	}


	for (int i = 0; i < binNum; i++) {
		int outwidth = NWGlobal::GetInstance()->OutWidth;
		ofsAnalysis << std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << binEnds[i] 
					<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << score[i] << std::endl;
	}

	ofsAnalysis.close();

	ofsOrignalDistance.close();
}


NWAnalysis::NWAnalysisGarbo::NWAnalysisGarbo() {


}

NWAnalysis::NWAnalysisGarbo::~NWAnalysisGarbo() {

	ReleaseInstance();
}

