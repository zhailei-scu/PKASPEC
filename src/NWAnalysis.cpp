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
	std::fstream ofsAnalysis_EqualInterval;
	std::fstream ofsAnalysis_PowerInterval;
	std::string OrignalDistancePath;
	std::string AnalysisPath_EqualInterval;
	std::string AnalysisPath_PowerInterval;
	std::vector<double> theDistance;
	double maxDistance = -1.0;
	double minDistance = 1E32;
	int minLog;
	int maxLog;
	int binNum;
	double *binEnds;
	double *score;
	std::stringstream ss;
	int PowerInterval_BinNum = 200;
	double PowerInterval_Min = 0.01;
	double PowerInterval_Max = 200;
	double PowerInterval_DeltaLength;
	double *binEnds_PowerInterval;
	int *score_PowerInterval;


	binEnds_PowerInterval = new double[PowerInterval_BinNum];

	score_PowerInterval = new int[PowerInterval_BinNum];

	PowerInterval_DeltaLength = (std::log10(PowerInterval_Max) - std::log10(PowerInterval_Min)) / PowerInterval_BinNum;


	if(NWGlobal::GetInstance()->OutPath.length() >0){

		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->OutPath.c_str() << "\\" << "DistanceResult_OriginDistance.txt";

		ss >> OrignalDistancePath;

		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->OutPath.c_str() << "\\" << "DistanceResult_Analysis_EqualInterval.txt";

		ss >> AnalysisPath_EqualInterval;

		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->OutPath.c_str() << "\\" << "DistanceResult_Analysis_PowerInterval.txt";

		ss >> AnalysisPath_PowerInterval;
	}
	else {
		ss.clear();

		ss.str("");

		ss << "DistanceResult_OriginDistance.txt";

		ss >> OrignalDistancePath;

		ss.clear();

		ss.str("");

		ss << "DistanceResult_Analysis_EqualInterval.txt";

		ss >> AnalysisPath_EqualInterval;

		ss.clear();

		ss.str("");

		ss << "DistanceResult_Analysis_PowerInterval.txt";

		ss >> AnalysisPath_PowerInterval;
	}

	ofsOrignalDistance.open(OrignalDistancePath, std::ios::out | std::ios::ate);

	ofsAnalysis_EqualInterval.open(AnalysisPath_EqualInterval, std::ios::out | std::ios::ate);

	ofsAnalysis_PowerInterval.open(AnalysisPath_PowerInterval, std::ios::out | std::ios::ate);

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

	minDistance = std::max(minPrecision, minDistance);

	minLog = std::floor(std::log10(minDistance));
	maxLog = std::ceil(std::log10(maxDistance));


	binNum = (maxLog - minLog)*BinNumberEachPower;

	binEnds = new double[binNum];
	score = new double[binNum];

	for (int i = 0; i < binNum; i++) {
		double base = pow(10, minLog + i / BinNumberEachPower);
		binEnds[i] = base + (i % BinNumberEachPower)*base * 9 / BinNumberEachPower;
		score[i] = 0;
	}

	for (int i = 0; i < PowerInterval_BinNum; i++) {
		binEnds_PowerInterval[i] = PowerInterval_Min*std::pow(10, i*PowerInterval_DeltaLength);
		score_PowerInterval[i] = 0;
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

		if (*itDistance >= PowerInterval_Min && *itDistance <= PowerInterval_Max) {
			int binPos = std::floor((std::log10(*itDistance) - std::log10(PowerInterval_Min)) / PowerInterval_DeltaLength);
			score_PowerInterval[binPos]++;
		}
		
	}


	for (int i = 0; i < binNum; i++) {

		score[i] = score[i] / pow(10, std::max(i-1,0) / BinNumberEachPower);

	}


	int outwidth = NWGlobal::GetInstance()->OutWidth;

	for (int i = 0; i < binNum; i++) {
		ofsAnalysis_EqualInterval << std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << binEnds[i]
					<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << score[i] << std::endl;
	}


	for (int i = 0; i < PowerInterval_BinNum; i++) {
		ofsAnalysis_PowerInterval << std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << binEnds_PowerInterval[i]
			<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << score_PowerInterval[i] << std::endl;
	}


	ofsAnalysis_EqualInterval.close();

	ofsAnalysis_PowerInterval.close();

	ofsOrignalDistance.close();

	if (!binEnds) delete[] binEnds;
	if (!score) delete[] score;

	if (!binEnds_PowerInterval) delete[] binEnds_PowerInterval;
	if (!score_PowerInterval) delete[] score_PowerInterval;
}


NWAnalysis::NWAnalysisGarbo::NWAnalysisGarbo() {


}

NWAnalysis::NWAnalysisGarbo::~NWAnalysisGarbo() {

	ReleaseInstance();
}

