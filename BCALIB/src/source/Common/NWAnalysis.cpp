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
	int ceilingNum[3];
	double boundary[3][2];
	//---Body---

	boundary[0][0] = 1.e32;
	boundary[1][0] = 1.e32;
	boundary[2][0] = 1.e32;
	boundary[0][1] = -1.e32;
	boundary[1][1] = -1.e32;
	boundary[2][1] = -1.e32;

	ceilingNum[0] = 10;
	ceilingNum[1] = 10;
	ceilingNum[2] = 10;

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
		<< std::setw(outwidth) << "MinDeltaX(mm)"
		<< std::setw(outwidth) << "MinDeltaY(mm)"
		<< std::setw(outwidth) << "MinDeltaZ(mm)" 
		<< std::setw(outwidth) << "MinDeltaDist(mm)" << std::endl;


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

					boundary[0][0] = min(iteratorStepInfo->GetpostPosition().getX(), boundary[0][0]);
					boundary[1][0] = min(iteratorStepInfo->GetpostPosition().getY(), boundary[1][0]);
					boundary[2][0] = min(iteratorStepInfo->GetpostPosition().getZ(), boundary[2][0]);
					boundary[0][1] = max(iteratorStepInfo->GetpostPosition().getX(), boundary[0][1]);
					boundary[1][1] = max(iteratorStepInfo->GetpostPosition().getY(), boundary[1][1]);
					boundary[2][1] = max(iteratorStepInfo->GetpostPosition().getZ(), boundary[2][1]);

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

	Cal_MinDist_LinkedCell(storedData,boundary,ceilingNum, &ofsAnalysisPath_DistanceXYZ);


	ofsAnalysis_EqualInterval.close();

	ofsAnalysis_PowerInterval.close();

	ofsOrignalDistance.close();

	ofsAnalysis_EndReason.close();

	ofsAnalysis_DeviateAxesDistance.close();

	ofsAnalysisPath_DistanceXYZ.close();

	if (NULL != binEnds) delete[] binEnds;
	if (NULL != score) delete[] score;

	if (NULL != binEnds_PowerInterval) delete[] binEnds_PowerInterval;
	if (NULL != score_PowerInterval) delete[] score_PowerInterval;
}


void NWAnalysis::Cal_MinDist_LinkedCell(std::map<int, std::vector<TrackInfo>>* storedData,double boundary[][2],int ceilNum[3],fstream* ofsAnalysisPath_DistanceXYZ){
	/*Local Vars*/
	int ceilingNum;
	double ceiling_Interval[3];
	int ceilIndex[3];
	int linkID;
	int subjectLinkID;
	int objectLinkID;
	std::map<int, std::vector<TrackInfo>>::iterator it;
	std::vector<TrackInfo>::iterator iteratorTrackInfo;
	std::vector<StepInfo>::iterator iteratorStepInfo;
	std::vector<int>* linkedCells_EventID;
	std::vector<int>* linkedCells_TrackID;
	std::vector<StepInfo*>* linkedCells_StepInfo;
	G4ThreeVector objectPostion;
	G4ThreeVector pKADist;
	double distance;
	int outwidth;
	/*Body*/
	outwidth = NWGlobal::GetInstance()->GetSimParamters()->GetOutWidth();

	ceilingNum = ceilNum[0] * ceilNum[1] * ceilNum[2];

	linkedCells_EventID = new std::vector<int>[ceilingNum];
	linkedCells_TrackID = new std::vector<int>[ceilingNum];
	linkedCells_StepInfo = new std::vector<StepInfo*>[ceilingNum];

	for (int i = 0; i < ceilingNum; i++) {
		linkedCells_EventID[i].swap(std::vector<int>());
		linkedCells_TrackID[i].swap(std::vector<int>());
		linkedCells_StepInfo[i].swap(std::vector<StepInfo*>());
	}

	ceiling_Interval[0] = (boundary[0][1] - boundary[0][0]) / ceilNum[0];
	ceiling_Interval[1] = (boundary[1][1] - boundary[1][0]) / ceilNum[1];
	ceiling_Interval[2] = (boundary[2][1] - boundary[2][0]) / ceilNum[2];

	it= storedData->begin();

	for (; it != storedData->end(); it++) {
		iteratorTrackInfo = it->second.begin();

		for (; iteratorTrackInfo != it->second.end(); iteratorTrackInfo++) {

			iteratorStepInfo = iteratorTrackInfo->GetStepsInfo()->begin();

			if (iteratorTrackInfo->GetStepsInfo()->size() > 0) {

				for (; iteratorStepInfo != iteratorTrackInfo->GetStepsInfo()->end(); iteratorStepInfo++) {

					if (0 != iteratorStepInfo->GetProcessName().compare(this->targetProcessName)) {
						break;
					}

					ceilIndex[0] = min(int((iteratorStepInfo->GetpostPosition().getX() - boundary[0][0]) / ceiling_Interval[0]), ceilNum[0] - 1);
					ceilIndex[1] = min(int((iteratorStepInfo->GetpostPosition().getY() - boundary[1][0]) / ceiling_Interval[1]), ceilNum[1] - 1);
					ceilIndex[2] = min(int((iteratorStepInfo->GetpostPosition().getZ() - boundary[2][0]) / ceiling_Interval[2]), ceilNum[2] - 1);

					ceilIndex[0] = max(ceilIndex[0], 0);
					ceilIndex[1] = max(ceilIndex[1], 0);
					ceilIndex[2] = max(ceilIndex[2], 0);

					linkID = ceilIndex[2]*ceilNum[0]*ceilNum[1] + ceilIndex[1]*ceilNum[0] + ceilIndex[0];
					linkedCells_EventID[linkID].push_back(it->first);
					linkedCells_TrackID[linkID].push_back(iteratorTrackInfo->GetTrackID());
					linkedCells_StepInfo[linkID].push_back(iteratorStepInfo._Ptr);
				}
			}

		}
	}


	for (int k = 0; k < ceilNum[2]; k++) {
		for (int j = 0; j < ceilNum[1]; j++) {
			for (int i = 0; i < ceilNum[0]; i++) {

				subjectLinkID = k * ceilNum[0]* ceilNum[1] + j * ceilNum[0] + i;

				for (int l = 0; l < linkedCells_StepInfo[subjectLinkID].size(); l++) {

					int subjectEventID = linkedCells_EventID[subjectLinkID].at(l);
					int subjectTrackID = linkedCells_TrackID[subjectLinkID].at(l);
					int subjectStepID = linkedCells_StepInfo[subjectLinkID].at(l)->GetStepID();
					G4ThreeVector subjectPostion = linkedCells_StepInfo[subjectLinkID].at(l)->GetpostPosition();

					double minDist = 1.e32;
					int minDist_objectEventID;
					int minDist_objectTrackID;
					int minDist_objectStepID;
					double minDist_x;
					double minDist_y;
					double minDist_z;

					for (int kk = max(k - 1, 0); kk < min(k + 2, ceilNum[2]); kk++) {
						for (int jj = max(j - 1, 0); jj < min(j + 2, ceilNum[1]); jj++) {
							for (int ii = max(i - 1, 0); ii < min(i + 2, ceilNum[0]); ii++) {
								objectLinkID = kk * ceilNum[0]* ceilNum[1] + jj * ceilNum[0] + ii;

								for (int m = 0; m < linkedCells_StepInfo[objectLinkID].size(); m++) {

									int objectEventID = linkedCells_EventID[objectLinkID].at(m);
									int objectTrackID = linkedCells_TrackID[objectLinkID].at(m);
									int objectStepID = linkedCells_StepInfo[objectLinkID].at(m)->GetStepID();

									if (subjectEventID != objectEventID || subjectTrackID != objectTrackID || subjectStepID != objectStepID) {
										objectPostion = linkedCells_StepInfo[objectLinkID].at(m)->GetpostPosition();
										pKADist = subjectPostion - objectPostion;
										distance = pKADist.mag();

										if (distance < minDist) {

											minDist = distance;

											minDist_x = abs(pKADist.getX());
											minDist_y = abs(pKADist.getY());
											minDist_z = abs(pKADist.getZ());

											minDist_objectEventID = objectEventID;
											minDist_objectTrackID = objectTrackID;
											minDist_objectStepID = objectStepID;
										}

									}

								}

							}

						}
					}


					*ofsAnalysisPath_DistanceXYZ
						<< std::setw(outwidth) << subjectEventID
						<< std::setw(outwidth) << subjectTrackID
						<< std::setw(outwidth) << subjectStepID
						<< std::setw(outwidth) << minDist_objectEventID
						<< std::setw(outwidth) << minDist_objectTrackID
						<< std::setw(outwidth) << minDist_objectStepID
						<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << minDist_x
						<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << minDist_y
						<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << minDist_z
						<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << minDist << std::endl;
				}

			}
		}
	}

	if (NULL != linkedCells_EventID) delete[] linkedCells_EventID;
	if (NULL != linkedCells_TrackID) delete[] linkedCells_TrackID;
	if (NULL != linkedCells_StepInfo) delete[] linkedCells_StepInfo;

}


NWAnalysis::NWAnalysisGarbo::NWAnalysisGarbo() {


}

NWAnalysis::NWAnalysisGarbo::~NWAnalysisGarbo() {

	ReleaseInstance();
}

