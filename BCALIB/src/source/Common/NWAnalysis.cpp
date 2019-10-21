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
	std::fstream ofsAnalysis_EndReason;
	std::fstream ofsAnalysis_DeviateAxesDistance;
	std::fstream ofsAnalysisPath_DistanceXYZ;
	std::string OrignalDistancePath;
	std::string AnalysisPath_EqualInterval;
	std::string AnalysisPath_PowerInterval;
	std::string AnalysisPath_EndReason;
	std::string AnalysisPath_DeviateAxesDistance;
	std::string AnalysisPath_DistanceXYZ;
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
	int outwidth;
	std::map<int, std::vector<TrackInfo>>::iterator it;
	std::vector<TrackInfo>::iterator iteratorTrackInfo;
	std::vector<StepInfo>::iterator iteratorStepInfo;
	G4ThreeVector prePKAPos;
	G4ThreeVector postPKAPos;
	int index;
	G4ThreeVector pKADist;
	double distance;
	double distanceToOriginAxe;
	G4ThreeVector vectorToOrigin;
	double vectorMultiple;
	double magOriginDirection;
	double magvectorToOrigin;
	std::vector<StepInfo*>* linkedCells;
	int ceilingNum;
	int ceilingNum_X;
	int ceilingNum_Y;
	int ceilingNum_Z;
	double minPos_X;
	double maxPos_X;
	double minPos_Y;
	double maxPos_Y;
	double minPos_Z;
	double maxPos_Z;
	double ceiling_Interval_X;
	double ceiling_Interval_Y;
	double ceiling_Interval_Z;
	int ceilIndex_X;
	int ceilIndex_Y;
	int ceilIndex_Z;
	int linkID;
	//---Body---

	minPos_X = 1.e32;
	minPos_Y = 1.e32;
	minPos_Z = 1.e32;
	maxPos_X = -1.e32;
	maxPos_Y = -1.e32;
	maxPos_Z = -1.e32;

	ceilingNum_X = 10;
	ceilingNum_Y = 10;
	ceilingNum_Z = 10;

	ceilingNum = ceilingNum_X * ceilingNum_Y*ceilingNum_Z;

	linkedCells = new std::vector<StepInfo*>[ceilingNum];

	for (int i = 0; i < ceilingNum; i++) {
		linkedCells[i].swap(std::vector<StepInfo*>());
	}

	outwidth = NWGlobal::GetInstance()->GetSimParamters()->GetOutWidth();

	binEnds_PowerInterval = new double[PowerInterval_BinNum];

	score_PowerInterval = new int[PowerInterval_BinNum];

	PowerInterval_DeltaLength = (std::log10(PowerInterval_Max) - std::log10(PowerInterval_Min)) / PowerInterval_BinNum;




	if(NWGlobal::GetInstance()->GetSimParamters()->GetOutPath()->length() >0){

		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters()->GetOutPath()->c_str() << "\\" << "DistanceResult_OriginDistance.txt";

		ss >> OrignalDistancePath;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters()->GetOutPath()->c_str() << "\\" << "DistanceResult_Analysis_EqualInterval.txt";

		ss >> AnalysisPath_EqualInterval;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters()->GetOutPath()->c_str() << "\\" << "DistanceResult_Analysis_PowerInterval.txt";

		ss >> AnalysisPath_PowerInterval;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters()->GetOutPath()->c_str() << "\\" << "DistanceResult_Analysis_EndReason.txt";

		ss >> AnalysisPath_EndReason;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters()->GetOutPath()->c_str() << "\\" << "DistanceResult_Analysis_DeviateAxesDistance.txt";

		ss >> AnalysisPath_DeviateAxesDistance;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters()->GetOutPath()->c_str() << "\\" << "DistanceResult_Analysis_DistanceXYZ.txt";

		ss >> AnalysisPath_DistanceXYZ;
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


		ss.clear();

		ss.str("");

		ss << "DistanceResult_Analysis_EndReason.txt";

		ss >> AnalysisPath_EndReason;


		ss.clear();

		ss.str("");

		ss << "DistanceResult_Analysis_DeviateAxesDistance.txt";

		ss >> AnalysisPath_DeviateAxesDistance;


		ss.clear();

		ss.str("");

		ss << "DistanceResult_Analysis_DistanceXYZ.txt";

		ss >> AnalysisPath_DistanceXYZ;
	}

	ofsOrignalDistance.open(OrignalDistancePath, std::ios::out | std::ios::ate);

	ofsAnalysis_EqualInterval.open(AnalysisPath_EqualInterval, std::ios::out | std::ios::ate);

	ofsAnalysis_PowerInterval.open(AnalysisPath_PowerInterval, std::ios::out | std::ios::ate);

	ofsAnalysis_EndReason.open(AnalysisPath_EndReason, std::ios::out | std::ios::ate);

	ofsAnalysis_EndReason << std::setw(outwidth) << "EventID:"
		<< std::setw(outwidth) << "EndReason" << std::setw(outwidth) << "EndEnergy" << std::endl;

	ofsAnalysis_DeviateAxesDistance.open(AnalysisPath_DeviateAxesDistance, std::ios::out | std::ios::ate);

	ofsAnalysis_DeviateAxesDistance << std::setw(outwidth) << "EventID:"
									<< std::setw(outwidth) << "ToOrgVector(mm)" << std::endl;


	ofsAnalysisPath_DistanceXYZ.open(AnalysisPath_DistanceXYZ, std::ios::out | std::ios::ate);

	ofsAnalysisPath_DistanceXYZ 
		<< std::setw(outwidth) << "SubjectEventID"
		<< std::setw(outwidth) << "SubjectTrackID"
		<< std::setw(outwidth) << "SubjectStepID"
		<< std::setw(outwidth) << "ObjectEventID"
		<< std::setw(outwidth) << "ObjectTrackID"
		<< std::setw(outwidth) << "ObjectStepID"
		<< std::setw(outwidth) << "DeltaX(mm)"
		<< std::setw(outwidth) << "DeltaY(mm)"
		<< std::setw(outwidth) << "DeltaZ(mm)" << std::endl;


	it = storedData->begin();

	for (; it != storedData->end(); it++) {
		iteratorTrackInfo = it->second.begin();

		for (; iteratorTrackInfo != it->second.end(); iteratorTrackInfo++) {

			iteratorStepInfo = iteratorTrackInfo->GetStepsInfo()->begin();

			if (iteratorTrackInfo->GetStepsInfo()->size() > 0) {

				index = 0;

				for (; iteratorStepInfo != iteratorTrackInfo->GetStepsInfo()->end(); iteratorStepInfo++) {

					if (0 != iteratorStepInfo->GetProcessName().compare(this->targetProcessName)) {

						ofsAnalysis_EndReason << std::setw(outwidth) << it->first
							<< std::setw(outwidth) << StepInfo::ConvertToProcessID(iteratorStepInfo->GetProcessName())
							<<std::setw(outwidth)  << std::setiosflags(std::ios::scientific)<< iteratorStepInfo->GetpreEng()
							<< std::endl;
						break;
					}

					index++;

					if (index <= 1) {
						prePKAPos = iteratorStepInfo->GetpostPosition();
						postPKAPos = iteratorStepInfo->GetpostPosition();
					}
					else {
						postPKAPos = iteratorStepInfo->GetpostPosition();

						pKADist = prePKAPos - postPKAPos;

						distance = pKADist.mag();

						ofsOrignalDistance << std::setiosflags(std::ios::scientific) << std::setprecision(7) << distance << std::endl;

						prePKAPos = postPKAPos;

						theDistance.push_back(distance);

						maxDistance = std::max(maxDistance, distance);
						minDistance = std::min(minDistance, distance);
					}


					vectorToOrigin = iteratorStepInfo->GetpostPosition() - iteratorStepInfo->GetOriginPosition();

					minPos_X = min(iteratorStepInfo->GetpostPosition().getX(), minPos_X);
					minPos_Y = min(iteratorStepInfo->GetpostPosition().getY(), minPos_Y);
					minPos_Z = min(iteratorStepInfo->GetpostPosition().getZ(), minPos_Z);
					maxPos_X = max(iteratorStepInfo->GetpostPosition().getX(), maxPos_X);
					maxPos_Y = max(iteratorStepInfo->GetpostPosition().getY(), maxPos_Y);
					maxPos_Z = max(iteratorStepInfo->GetpostPosition().getZ(), maxPos_Z);

					vectorMultiple = vectorToOrigin * iteratorStepInfo->GetOriginDirection();

					magvectorToOrigin = vectorToOrigin.mag();

					magOriginDirection = iteratorStepInfo->GetOriginDirection().mag();

					if (magvectorToOrigin <= 0 || magOriginDirection <= 0) {
						distanceToOriginAxe = 0.0;
					}
					else {
						distanceToOriginAxe = magvectorToOrigin * std::sqrt(1 - pow(std::fabs(vectorMultiple) / (magvectorToOrigin*magOriginDirection), 2));
					}

					ofsAnalysis_DeviateAxesDistance << std::setw(outwidth) << it->first
						<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << distanceToOriginAxe 
						<< std::endl;

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

	for (std::vector<double>::iterator itDistance = theDistance.begin(); itDistance != theDistance.end(); itDistance++) {

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


	for (int i = 0; i < binNum; i++) {
		ofsAnalysis_EqualInterval << std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << binEnds[i]
					<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << score[i] << std::endl;
	}


	for (int i = 0; i < PowerInterval_BinNum; i++) {
		ofsAnalysis_PowerInterval << std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << binEnds_PowerInterval[i]
			<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << score_PowerInterval[i] << std::endl;
	}

	ceiling_Interval_X = (maxPos_X - minPos_X) / ceilingNum_X;
	ceiling_Interval_Y = (maxPos_Y - minPos_Y) / ceilingNum_Y;
	ceiling_Interval_Z = (maxPos_Z - minPos_Z) / ceilingNum_Z;

	it = storedData->begin();

	for (; it != storedData->end(); it++) {
		iteratorTrackInfo = it->second.begin();

		for (; iteratorTrackInfo != it->second.end(); iteratorTrackInfo++) {

			iteratorStepInfo = iteratorTrackInfo->GetStepsInfo()->begin();

			if (iteratorTrackInfo->GetStepsInfo()->size() > 0) {

				for (; iteratorStepInfo != iteratorTrackInfo->GetStepsInfo()->end(); iteratorStepInfo++) {

					ceilIndex_X = min(int((iteratorStepInfo->GetpostPosition().getX() - minPos_X) / ceiling_Interval_X), ceilingNum_X - 1);
					ceilIndex_Y = min(int((iteratorStepInfo->GetpostPosition().getY() - minPos_Y) / ceiling_Interval_Y), ceilingNum_Y - 1);
					ceilIndex_Z = min(int((iteratorStepInfo->GetpostPosition().getZ() - minPos_Z) / ceiling_Interval_Z), ceilingNum_Z - 1);

					linkID = ceilIndex_Z * ceilingNum_Y*ceilingNum_Z + ceilIndex_Y * ceilingNum_X + ceilIndex_X;
					linkedCells[linkID].push_back(iteratorStepInfo._Ptr);

				}
			}

		}
	}





	ofsAnalysis_EqualInterval.close();

	ofsAnalysis_PowerInterval.close();

	ofsOrignalDistance.close();

	ofsAnalysis_EndReason.close();

	ofsAnalysis_DeviateAxesDistance.close();

	if (NULL != binEnds) delete[] binEnds;
	if (NULL != score) delete[] score;

	if (NULL != binEnds_PowerInterval) delete[] binEnds_PowerInterval;
	if (NULL != score_PowerInterval) delete[] score_PowerInterval;

	if (NULL != linkedCells) delete[] linkedCells;
}


NWAnalysis::NWAnalysisGarbo::NWAnalysisGarbo() {


}

NWAnalysis::NWAnalysisGarbo::~NWAnalysisGarbo() {

	ReleaseInstance();
}

