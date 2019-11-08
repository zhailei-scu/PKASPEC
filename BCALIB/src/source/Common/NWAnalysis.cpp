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
	std::fstream ofsAnalysisPath_linkedCellPosition;
	std::string OrignalDistancePath;
	std::string AnalysisPath_EqualInterval;
	std::string AnalysisPath_PowerInterval;
	std::string AnalysisPath_EndReason;
	std::string AnalysisPath_DeviateAxesDistance;
	std::string AnalysisPath_DistanceXYZ;
	std::string AnalysisPath_linkedCellPosition;
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
	int ceilingNum_OneDim;
	double boundary[3][2];
	//---Body---

	boundary[0][0] = 1.e32;
	boundary[1][0] = 1.e32;
	boundary[2][0] = 1.e32;
	boundary[0][1] = -1.e32;
	boundary[1][1] = -1.e32;
	boundary[2][1] = -1.e32;

	ceilingNum_OneDim = 10;

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


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters()->GetOutPath()->c_str() << "\\" << "DistanceResult_Analysis_linkedCellPosition.txt";

		ss >> AnalysisPath_linkedCellPosition;

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


		ss.clear();

		ss.str("");

		ss << "DistanceResult_Analysis_linkedCellPosition.txt";

		ss >> AnalysisPath_linkedCellPosition;
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
		<< std::setw(outwidth) << "SubjectZONEID"
		<< std::setw(outwidth) << "SubjectLinkedID"
		<< std::setw(outwidth) << "SubjectEventID"
		<< std::setw(outwidth) << "SubjectTrackID"
		<< std::setw(outwidth) << "SubjectStepID"
		<< std::setw(outwidth) << "ObjectZONEID"
		<< std::setw(outwidth) << "ObjectLinkedID"
		<< std::setw(outwidth) << "ObjectEventID"
		<< std::setw(outwidth) << "ObjectTrackID"
		<< std::setw(outwidth) << "ObjectStepID"
		<< std::setw(outwidth) << "MinDeltaX(mm)"
		<< std::setw(outwidth) << "MinDeltaY(mm)"
		<< std::setw(outwidth) << "MinDeltaZ(mm)" 
		<< std::setw(outwidth) << "MinDeltaDist(mm)" << std::endl;


	ofsAnalysisPath_linkedCellPosition.open(AnalysisPath_linkedCellPosition, std::ios::out | std::ios::ate);

	ofsAnalysisPath_linkedCellPosition
		<< std::setw(outwidth) << "CellID"
		<< std::setw(outwidth) << "ZoneID"
		<< std::setw(outwidth) << "cent_x(mm)"
		<< std::setw(outwidth) << "cent_y(mm)"
		<< std::setw(outwidth) << "cent_z(mm)" << std::endl;


	it = storedData->begin();

	for (; it != storedData->end(); it++) {
		iteratorTrackInfo = it->second.begin();

		for (; iteratorTrackInfo != it->second.end(); iteratorTrackInfo++) {

			iteratorStepInfo = iteratorTrackInfo->GetStepsInfo()->begin();

			if (iteratorTrackInfo->GetStepsInfo()->size() > 0) {

				index = 0;

				for (; iteratorStepInfo != iteratorTrackInfo->GetStepsInfo()->end(); iteratorStepInfo++) {

					if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters()->GetTheConcentReaction()) {

						if (0 != iteratorStepInfo->GetProcessName().compare(std::string("hadElastic"))) {

							ofsAnalysis_EndReason << std::setw(outwidth) << it->first
								<< std::setw(outwidth) << StepInfo::ConvertToProcessID(iteratorStepInfo->GetProcessName())
								<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << iteratorStepInfo->GetpreEng()
								<< std::endl;
							break;
						}

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

	Cal_MinDist_LinkedCell(storedData,boundary, &ofsAnalysisPath_DistanceXYZ,&ofsAnalysisPath_linkedCellPosition);


	ofsAnalysis_EqualInterval.close();

	ofsAnalysis_PowerInterval.close();

	ofsOrignalDistance.close();

	ofsAnalysis_EndReason.close();

	ofsAnalysis_DeviateAxesDistance.close();

	ofsAnalysisPath_DistanceXYZ.close();

	ofsAnalysisPath_linkedCellPosition.close();

	if (NULL != binEnds) delete[] binEnds;
	if (NULL != score) delete[] score;

	if (NULL != binEnds_PowerInterval) delete[] binEnds_PowerInterval;
	if (NULL != score_PowerInterval) delete[] score_PowerInterval;
}


void NWAnalysis::Cal_MinDist_LinkedCell(std::map<int, std::vector<TrackInfo>>* storedData,double boundary[][2],
	fstream* ofsAnalysisPath_DistanceXYZ,fstream *ofsAnalysisPath_linkedCellPosition){
	/*Local Vars*/
	int ceilingNum;
	double ceil_Interval[3];
	int ceilingNum_OneDim[3];
	int ceilIndex[3];
	double beamCenter[2];
	double newBoundary[3][2];
	int linkID;
	int subjectLinkID;
	int objectLinkID;
	std::map<int, std::vector<TrackInfo>>::iterator it;
	std::vector<TrackInfo>::iterator iteratorTrackInfo;
	std::vector<int>* linkedCells_EventID = NULL;
	std::vector<int>* linkedCells_TrackID = NULL;
	std::vector<StepInfo*>* linkedCells_StepInfo = NULL;
	G4ThreeVector objectPostion;
	G4ThreeVector pKADist;
	double distance;
	int outwidth;
	int ZoneNum;
	int ZoneCenter[2];
	int SubjectZoneID;
	int SubjectZoneID_X;
	int SubjectZoneID_Y;
	int ObjectZoneID;
	int ObjectZoneID_X;
	int ObjectZoneID_Y;
	int theSize;
	int subjectEventID;
	int subjectTrackID;
	int subjectStepID;
	G4ThreeVector subjectPostion;
	double minDist;
	int minDist_ObjectLinkedID;
	int minDist_objectEventID;
	int minDist_objectTrackID;
	int minDist_objectStepID;
	double minDist_x;
	double minDist_y;
	double minDist_z;
	int shellNum;
	bool founded;
	/*Body*/

	ceil_Interval[0] = ceil_Interval[1] = NWGlobal::GetInstance()->GetSimParamters()->GetLinkCellInterval_xy();

	NWGlobal::GetInstance()->GetSimParamters()->GetNWBeam()->GetFluxCenter(beamCenter);

	for (int i = 0; i <= 1; i++) {
		ceilingNum_OneDim[i] = 2*ceil(max(beamCenter[i] - boundary[i][0], boundary[i][1] - beamCenter[i]) / ceil_Interval[i]);
		ceilingNum_OneDim[i] = max(ceilingNum_OneDim[i], 2);
	}

	ceilingNum_OneDim[0] = ceilingNum_OneDim[1] =  max(ceilingNum_OneDim[0], ceilingNum_OneDim[1]);

	ceilingNum_OneDim[2] = NWGlobal::GetInstance()->GetSimParamters()->GetLinkCellNum_z();
	ceil_Interval[2] = ceil((boundary[2][1] - boundary[2][0]) / ceilingNum_OneDim[2]);

	ceilingNum = ceilingNum_OneDim[0] * ceilingNum_OneDim[1] * ceilingNum_OneDim[2];

	if (ceilingNum > MAXCELLNUM) {
		std::cout << "The cell x y interval and z cell number exceed the memory limit." << std::endl;
		std::cout << "x cell number : " << ceilingNum_OneDim[0] << std::endl;
		std::cout << "y cell number : " << ceilingNum_OneDim[1] << std::endl;
		std::cout << "z cell number : " << ceilingNum_OneDim[2] << std::endl;
		std::cout << "the max cell number x*y*z : " << MAXCELLNUM << std::endl;
		system("pause");
		exit(1);
	}

	std::cout << "boundary_x " << boundary[0][0] << " " << boundary[0][1] << std::endl;
	std::cout << "boundary_y " << boundary[1][0] << " " << boundary[1][1] << std::endl;
	std::cout << "boundary_z " << boundary[2][0] << " " << boundary[2][1] << std::endl;


	std::cout << "cellnum_x " << ceilingNum_OneDim[0] << std::endl;
	std::cout << "cellnum_y " << ceilingNum_OneDim[1] << std::endl;
	std::cout << "cellnum_z " << ceilingNum_OneDim[2] << std::endl;


	/*Zone ID start from 0(Center zone)*/
	ZoneNum = ceilingNum_OneDim[0] / 2;
	ZoneCenter[0] = ceilingNum_OneDim[0] / 2 - 1;
	ZoneCenter[1] = ceilingNum_OneDim[0] / 2;

	/*new boundary*/
	for (int i = 0; i <= 1; i++ ) {
		newBoundary[i][0] = beamCenter[i] - ceil_Interval[i] * ZoneNum;
		newBoundary[i][1] = beamCenter[i] + ceil_Interval[i] * ZoneNum;
	}
	newBoundary[2][0] = boundary[2][0];
	newBoundary[2][1] = boundary[2][1];

	outwidth = NWGlobal::GetInstance()->GetSimParamters()->GetOutWidth();

	

	linkedCells_EventID = new std::vector<int>[ceilingNum];
	linkedCells_TrackID = new std::vector<int>[ceilingNum];
	linkedCells_StepInfo = new std::vector<StepInfo*>[ceilingNum];

	for (int i = 0; i < ceilingNum; i++) {
		std::vector<int>().swap(linkedCells_EventID[i]);
		std::vector<int>().swap(linkedCells_TrackID[i]);
		std::vector<StepInfo*>().swap(linkedCells_StepInfo[i]);
	}


	for (int k = 0; k < ceilingNum_OneDim[2]; k++) {
		for (int j = 0; j < ceilingNum_OneDim[1]; j++) {
			SubjectZoneID_Y = max(ZoneCenter[0] - j, j - ZoneCenter[1]);

			for (int i = 0; i < ceilingNum_OneDim[0]; i++) {

				SubjectZoneID_X = max(ZoneCenter[0] - i, i - ZoneCenter[1]);

				SubjectZoneID = max(SubjectZoneID_X, SubjectZoneID_Y);

				*ofsAnalysisPath_linkedCellPosition
					<< std::setw(outwidth) << k * ceilingNum_OneDim[0] * ceilingNum_OneDim[1] + j * ceilingNum_OneDim[0] + i
					<< std::setw(outwidth) << SubjectZoneID
					<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << newBoundary[0][0] + (i + 0.5)*ceil_Interval[0]
					<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << newBoundary[1][0] + (j + 0.5)*ceil_Interval[1]
					<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << newBoundary[2][0] + (k + 0.5)*ceil_Interval[2]
					<< std::endl;
			}
		}
	}

	it= storedData->begin();

	for (; it != storedData->end(); it++) {
		iteratorTrackInfo = it->second.begin();

		for (; iteratorTrackInfo != it->second.end(); iteratorTrackInfo++) {

			theSize = iteratorTrackInfo->GetStepsInfo()->size();

			for (int index = 0; index < theSize; index++) {
				if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters()->GetTheConcentReaction()) {
					if (0 != iteratorTrackInfo->GetStepsInfo()->at(index).GetProcessName().compare(std::string("hadElastic"))) {
						break;
					}
				}

				ceilIndex[0] = min(int((iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getX() - newBoundary[0][0]) / ceil_Interval[0]), ceilingNum_OneDim[0] - 1);
				ceilIndex[1] = min(int((iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getY() - newBoundary[1][0]) / ceil_Interval[1]), ceilingNum_OneDim[1] - 1);
				ceilIndex[2] = min(int((iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getZ() - newBoundary[2][0]) / ceil_Interval[2]), ceilingNum_OneDim[2] - 1);

				ceilIndex[0] = max(ceilIndex[0], 0);
				ceilIndex[1] = max(ceilIndex[1], 0);
				ceilIndex[2] = max(ceilIndex[2], 0);

				linkID = ceilIndex[2] * ceilingNum_OneDim[0] * ceilingNum_OneDim[1] + ceilIndex[1] * ceilingNum_OneDim[0] + ceilIndex[0];
				linkedCells_EventID[linkID].push_back(it->first);
				linkedCells_TrackID[linkID].push_back(iteratorTrackInfo->GetTrackID());
				linkedCells_StepInfo[linkID].push_back(&iteratorTrackInfo->GetStepsInfo()->at(index));
			}

		}
	}


	for (int k = 0; k < ceilingNum_OneDim[2]; k++) {
		for (int j = 0; j < ceilingNum_OneDim[1]; j++) {

			SubjectZoneID_Y = max(ZoneCenter[0] - j,j - ZoneCenter[1]);

			for (int i = 0; i < ceilingNum_OneDim[0]; i++) {

				SubjectZoneID_X = max(ZoneCenter[0] - i, i - ZoneCenter[1]);

				SubjectZoneID = max(SubjectZoneID_X, SubjectZoneID_Y);

				subjectLinkID = k * ceilingNum_OneDim[0]*ceilingNum_OneDim[1] + j * ceilingNum_OneDim[0] + i;

				std::cout <<"cell ID: "<< subjectLinkID << " of total Cell Number: " << ceilingNum << std::endl;

				for (size_t l = 0; l < linkedCells_StepInfo[subjectLinkID].size(); l++) {

					subjectEventID = linkedCells_EventID[subjectLinkID].at(l);
					subjectTrackID = linkedCells_TrackID[subjectLinkID].at(l);
					subjectStepID = linkedCells_StepInfo[subjectLinkID].at(l)->GetStepID();
					subjectPostion = linkedCells_StepInfo[subjectLinkID].at(l)->GetpostPosition();

					minDist = 1.e32;
					shellNum = 1;
					founded = false;

					minDist_objectEventID = -1;
					minDist_objectTrackID = -1;
					minDist_objectStepID = -1;
					minDist_ObjectLinkedID = -1;

					ObjectZoneID = -1;

					while (!founded) {

						for (int kk = max(k - shellNum, 0); kk < min(k + shellNum + 1, ceilingNum_OneDim[2]); kk++) {
							for (int jj = max(j - shellNum, 0); jj < min(j + shellNum + 1, ceilingNum_OneDim[1]); jj++) {

								for (int ii = max(i - shellNum, 0); ii < min(i + shellNum + 1, ceilingNum_OneDim[0]); ii++) {

									objectLinkID = kk * ceilingNum_OneDim[0] * ceilingNum_OneDim[1] + jj * ceilingNum_OneDim[0] + ii;

									for (size_t m = 0; m < linkedCells_StepInfo[objectLinkID].size(); m++) {

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

												minDist_ObjectLinkedID = objectLinkID;

												minDist_objectEventID = objectEventID;
												minDist_objectTrackID = objectTrackID;
												minDist_objectStepID = objectStepID;

												ObjectZoneID_X = max(ZoneCenter[0] - ii, ii - ZoneCenter[1]);
												ObjectZoneID_Y = max(ZoneCenter[0] - jj, jj - ZoneCenter[1]);
												ObjectZoneID = max(ObjectZoneID_X, ObjectZoneID_Y);

												founded = true;
											}

										}

									}

								}

							}
						}

						shellNum = shellNum+1;
					}


					*ofsAnalysisPath_DistanceXYZ
						<< std::setw(outwidth) << SubjectZoneID
						<< std::setw(outwidth) << subjectLinkID
						<< std::setw(outwidth) << subjectEventID
						<< std::setw(outwidth) << subjectTrackID
						<< std::setw(outwidth) << subjectStepID
						<< std::setw(outwidth) << ObjectZoneID
						<< std::setw(outwidth) << minDist_ObjectLinkedID
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

