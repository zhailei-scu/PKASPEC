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
	std::fstream ofsAnalysisPath_ZoneCount;
	std::fstream ofsAnalysisPath_CeilXYCount;
	std::string OrignalDistancePath;
	std::string AnalysisPath_EqualInterval;
	std::string AnalysisPath_PowerInterval;
	std::string AnalysisPath_EndReason;
	std::string AnalysisPath_DeviateAxesDistance;
	std::string AnalysisPath_DistanceXYZ;
	std::string AnalysisPath_linkedCellPosition;
	std::string AnalysisPath_ZoneCount;
	std::string AnalysisPath_CeilXYCount;
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
	double boundary[3][2];
	//---Body---

	boundary[0][0] = 1.e32;
	boundary[1][0] = 1.e32;
	boundary[2][0] = 1.e32;
	boundary[0][1] = -1.e32;
	boundary[1][1] = -1.e32;
	boundary[2][1] = -1.e32;

	outwidth = NWGlobal::GetInstance()->GetSimParamters().GetOutWidth();

	binEnds_PowerInterval = new double[PowerInterval_BinNum];

	score_PowerInterval = new int[PowerInterval_BinNum];

	PowerInterval_DeltaLength = (std::log10(PowerInterval_Max) - std::log10(PowerInterval_Min)) / PowerInterval_BinNum;

	score = NULL;
	binEnds = NULL;


	if(NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->length() >0){

		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "DistanceResult_OriginDistance.txt";

		ss >> OrignalDistancePath;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "DistanceResult_Analysis_EqualInterval.txt";

		ss >> AnalysisPath_EqualInterval;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "DistanceResult_Analysis_PowerInterval.txt";

		ss >> AnalysisPath_PowerInterval;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "DistanceResult_Analysis_EndReason.txt";

		ss >> AnalysisPath_EndReason;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "DistanceResult_Analysis_DeviateAxesDistance.txt";

		ss >> AnalysisPath_DeviateAxesDistance;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "DistanceResult_Analysis_DistanceXYZ.txt";

		ss >> AnalysisPath_DistanceXYZ;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "DistanceResult_Analysis_linkedCellPosition.txt";

		ss >> AnalysisPath_linkedCellPosition;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "ZoneCount.txt";

		ss >> AnalysisPath_ZoneCount;



		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "CeilCount.txt";

		ss >> AnalysisPath_CeilXYCount;

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



		ss.clear();

		ss.str("");

		ss << "ZoneCount.txt";

		ss >> AnalysisPath_ZoneCount;


		ss.clear();

		ss.str("");

		ss << "CeilCount.txt";

		ss >> AnalysisPath_CeilXYCount;

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


	ofsAnalysisPath_ZoneCount.open(AnalysisPath_ZoneCount,std::ios::out | std::ios::ate);

	ofsAnalysisPath_ZoneCount
		<< std::setw(outwidth) << "ZoneID"
		<< std::setw(outwidth) << "Count" << std::endl;



	ofsAnalysisPath_CeilXYCount.open(AnalysisPath_CeilXYCount, std::ios::out | std::ios::ate);

	ofsAnalysisPath_CeilXYCount
		<< std::setw(outwidth) << "ZoneID"
		<< std::setw(outwidth) << "CeilXYID"
		<< std::setw(outwidth) << "ZoneX"
		<< std::setw(outwidth) << "ZoneY"
		<< std::setw(outwidth) << "Count" << std::endl;


	it = storedData->begin();

	for (; it != storedData->end(); it++) {
		iteratorTrackInfo = it->second.begin();

		index = 0;

		for (; iteratorTrackInfo != it->second.end(); iteratorTrackInfo++) {

			iteratorStepInfo = iteratorTrackInfo->GetStepsInfo()->begin();

			if (iteratorTrackInfo->GetStepsInfo()->size() > 0) {

				for (; iteratorStepInfo != iteratorTrackInfo->GetStepsInfo()->end(); iteratorStepInfo++) {

					if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

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

						if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {
							prePKAPos = iteratorStepInfo->GetpostPosition();
							postPKAPos = iteratorStepInfo->GetpostPosition();

						}else if (ConcentReaction(InletToFirstNonEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
								 ConcentReaction(InletEstAndInEstTillEnd) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() || 
							     ConcentReaction(MatrixAtom) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
							     ConcentReaction(Iso) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

							prePKAPos = iteratorStepInfo->GetprePosition();
							postPKAPos = iteratorStepInfo->GetprePosition();
						}
					}
					else {

						if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {
							postPKAPos = iteratorStepInfo->GetpostPosition();

						}else if (ConcentReaction(InletToFirstNonEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
							ConcentReaction(InletEstAndInEstTillEnd) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
							ConcentReaction(MatrixAtom) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() || 
							ConcentReaction(Iso) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

							postPKAPos = iteratorStepInfo->GetprePosition();
						}

						pKADist = prePKAPos - postPKAPos;

						distance = pKADist.mag();

						ofsOrignalDistance << std::setiosflags(std::ios::scientific) << std::setprecision(7) << distance << std::endl;

						prePKAPos = postPKAPos;

						theDistance.push_back(distance);

						maxDistance = std::max(maxDistance, distance);
						minDistance = std::min(minDistance, distance);
					}

					if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {
						vectorToOrigin = iteratorStepInfo->GetpostPosition() - iteratorStepInfo->GetOriginPosition();

						boundary[0][0] = min(iteratorStepInfo->GetpostPosition().getX(), boundary[0][0]);
						boundary[1][0] = min(iteratorStepInfo->GetpostPosition().getY(), boundary[1][0]);
						boundary[2][0] = min(iteratorStepInfo->GetpostPosition().getZ(), boundary[2][0]);
						boundary[0][1] = max(iteratorStepInfo->GetpostPosition().getX(), boundary[0][1]);
						boundary[1][1] = max(iteratorStepInfo->GetpostPosition().getY(), boundary[1][1]);
						boundary[2][1] = max(iteratorStepInfo->GetpostPosition().getZ(), boundary[2][1]);

					}else if (ConcentReaction(InletToFirstNonEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
						ConcentReaction(InletEstAndInEstTillEnd) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
						ConcentReaction(MatrixAtom) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() || 
						ConcentReaction(Iso) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

						vectorToOrigin = iteratorStepInfo->GetprePosition() - iteratorStepInfo->GetOriginPosition();

						boundary[0][0] = min(iteratorStepInfo->GetprePosition().getX(), boundary[0][0]);
						boundary[1][0] = min(iteratorStepInfo->GetprePosition().getY(), boundary[1][0]);
						boundary[2][0] = min(iteratorStepInfo->GetprePosition().getZ(), boundary[2][0]);
						boundary[0][1] = max(iteratorStepInfo->GetprePosition().getX(), boundary[0][1]);
						boundary[1][1] = max(iteratorStepInfo->GetprePosition().getY(), boundary[1][1]);
						boundary[2][1] = max(iteratorStepInfo->GetprePosition().getZ(), boundary[2][1]);

					}

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

	if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
		ConcentReaction(InletToFirstNonEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
		ConcentReaction(InletEstAndInEstTillEnd) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
		ConcentReaction(MatrixAtom) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

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
			binEnds_PowerInterval[i] = PowerInterval_Min * std::pow(10, i*PowerInterval_DeltaLength);
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

			score[i] = score[i] / pow(10, std::max(i - 1, 0) / BinNumberEachPower);

		}


		for (int i = 0; i < binNum; i++) {
			ofsAnalysis_EqualInterval << std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << binEnds[i]
				<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << score[i] << std::endl;
		}


		for (int i = 0; i < PowerInterval_BinNum; i++) {
			ofsAnalysis_PowerInterval << std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << binEnds_PowerInterval[i]
				<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << score_PowerInterval[i] << std::endl;
		}

	}

	Cal_MinDist_LinkedCell(storedData,boundary, &ofsAnalysisPath_DistanceXYZ,&ofsAnalysisPath_linkedCellPosition,&ofsAnalysisPath_ZoneCount,&ofsAnalysisPath_CeilXYCount);


	ofsAnalysis_EqualInterval.close();

	ofsAnalysis_PowerInterval.close();

	ofsOrignalDistance.close();

	ofsAnalysis_EndReason.close();

	ofsAnalysis_DeviateAxesDistance.close();

	ofsAnalysisPath_DistanceXYZ.close();

	ofsAnalysisPath_linkedCellPosition.close();

	ofsAnalysisPath_ZoneCount.close();

	ofsAnalysisPath_CeilXYCount.close();

	if (NULL != binEnds) delete[] binEnds;
	if (NULL != score) delete[] score;

	if (NULL != binEnds_PowerInterval) delete[] binEnds_PowerInterval;
	if (NULL != score_PowerInterval) delete[] score_PowerInterval;
}


void NWAnalysis::Cal_MinDist_LinkedCell(std::map<int, std::vector<TrackInfo>>* storedData,double boundary[][2],
	fstream* ofsAnalysisPath_DistanceXYZ,fstream *ofsAnalysisPath_linkedCellPosition,
	fstream* ofsAnalysisPath_ZoneCount,fstream* ofsAnalysisPath_CeilCount){
	/*Local Vars*/
	int ceilingNum;
	double ceil_Interval[3];
	int ceilingNum_OneDim[3];
	int ceilIndex[3];
	double newBoundary[3][2];
	double beamCenter[2];
	int linkID;
	int subjectLinkID;
	int objectLinkID;
	std::map<int, std::vector<TrackInfo>>::iterator it;
	std::vector<TrackInfo>::iterator iteratorTrackInfo;
	std::vector<int>* linkedCells_EventID = NULL;
	std::vector<int>* linkedCells_TrackID = NULL;
	std::vector<StepInfo*>* linkedCells_StepInfo = NULL;
	int** ceilXYCount;
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
	int iInterval;
	int ZoneCount;
	int corr_i;
	int corr_j;
	/*Body*/

	ceil_Interval[0] = ceil_Interval[1] = NWGlobal::GetInstance()->GetSimParamters().GetLinkCellInterval_xy();

	NWGlobal::GetInstance()->GetSimParamters().GetNWBeam().GetFluxCenter(beamCenter);

	for (int i = 0; i <= 1; i++) {
		ceilingNum_OneDim[i] = 2*ceil(max(beamCenter[i] - boundary[i][0], boundary[i][1] - beamCenter[i]) / ceil_Interval[i]) + 1;
		ceilingNum_OneDim[i] = max(ceilingNum_OneDim[i], 3);
	}

	ceilingNum_OneDim[0] = ceilingNum_OneDim[1] =  max(ceilingNum_OneDim[0], ceilingNum_OneDim[1]);

	ceilingNum_OneDim[2] = NWGlobal::GetInstance()->GetSimParamters().GetLinkCellNum_z();
	ceil_Interval[2] = ceil((boundary[2][1] - boundary[2][0]) / ceilingNum_OneDim[2]);

	ceilingNum = ceilingNum_OneDim[0] * ceilingNum_OneDim[1] * ceilingNum_OneDim[2];

	for (int i = 0; i < 3; i++) {
		ceilIndex[i] = 0;
	}


	std::cout << "boundary_x " << boundary[0][0] << " " << boundary[0][1] << std::endl;
	std::cout << "boundary_y " << boundary[1][0] << " " << boundary[1][1] << std::endl;
	std::cout << "boundary_z " << boundary[2][0] << " " << boundary[2][1] << std::endl;

	if (ceilingNum > MAXCELLNUM) {
		std::cout << "The cell x y interval and z cell number exceed the memory limit." << std::endl;
		std::cout << "x cell number : " << ceilingNum_OneDim[0] << std::endl;
		std::cout << "y cell number : " << ceilingNum_OneDim[1] << std::endl;
		std::cout << "z cell number : " << ceilingNum_OneDim[2] << std::endl;
		std::cout << "the max cell number x*y*z : " << MAXCELLNUM << std::endl;
		std::cout << "So we would only consider a little zone" << std::endl;
		std::cout << "The x bin number in x would be :" << MAXCELLNUM_X << std::endl;
		std::cout << "The x bin number in y would be :" << MAXCELLNUM_Y << std::endl;
		std::cout << "The x bin number in z would be :" << MAXCELLNUM_Z << std::endl;

		ceilingNum_OneDim[0] = MAXCELLNUM_X;
		ceilingNum_OneDim[1] = MAXCELLNUM_Y;
		ceilingNum_OneDim[2] = MAXCELLNUM_Z;

		ceil_Interval[2] = ceil((boundary[2][1] - boundary[2][0]) / ceilingNum_OneDim[2]);

		ceilingNum = ceilingNum_OneDim[0] * ceilingNum_OneDim[1] * ceilingNum_OneDim[2];
	}


	std::cout << "cellnum_x " << ceilingNum_OneDim[0] << std::endl;
	std::cout << "cellnum_y " << ceilingNum_OneDim[1] << std::endl;
	std::cout << "cellnum_z " << ceilingNum_OneDim[2] << std::endl;


	/*Zone ID start from 0(Center zone)*/
	ZoneNum = ceilingNum_OneDim[0] / 2  + 1;
	ZoneCenter[0] = ZoneNum - 1;
	ZoneCenter[1] = ZoneCenter[0];

	outwidth = NWGlobal::GetInstance()->GetSimParamters().GetOutWidth();


	for (int i = 0; i <= 1; i++) {
		newBoundary[i][0] = (beamCenter[i] - (ZoneNum - 1 + 0.50)*ceil_Interval[i]);
		newBoundary[i][1] = (beamCenter[i] + (ZoneNum - 1 + 0.50)*ceil_Interval[i]);
	}
	newBoundary[2][0] = boundary[2][0];
	newBoundary[2][1] = boundary[2][1];


	std::cout << "new_boundary_x " << newBoundary[0][0] << " " << newBoundary[0][1] << std::endl;
	std::cout << "new_boundary_y " << newBoundary[1][0] << " " << newBoundary[1][1] << std::endl;
	std::cout << "new_boundary_z " << newBoundary[2][0] << " " << newBoundary[2][1] << std::endl;

	

	linkedCells_EventID = new std::vector<int>[ceilingNum];
	linkedCells_TrackID = new std::vector<int>[ceilingNum];
	linkedCells_StepInfo = new std::vector<StepInfo*>[ceilingNum];


	ceilXYCount = new int*[ceilingNum_OneDim[0]];
	for (int i = 0; i < ceilingNum_OneDim[0];i++) {
		ceilXYCount[i] = new int[ceilingNum_OneDim[1]];

		for (int j = 0; j < ceilingNum_OneDim[1]; j++) {
			ceilXYCount[i][j] = 0;
		}
	}


	for (int i = 0; i < ceilingNum; i++) {
		std::vector<int>().swap(linkedCells_EventID[i]);
		std::vector<int>().swap(linkedCells_TrackID[i]);
		std::vector<StepInfo*>().swap(linkedCells_StepInfo[i]);
	}


	for (int k = 0; k < ceilingNum_OneDim[2]; k++) {
		for (int j = 0; j < ceilingNum_OneDim[1]; j++) {
			SubjectZoneID_Y = max(ZoneCenter[1] - j,j - ZoneCenter[1]);

			for (int i = 0; i < ceilingNum_OneDim[0]; i++) {

				SubjectZoneID_X = max(ZoneCenter[0] - i, i - ZoneCenter[0]);

				SubjectZoneID = max(SubjectZoneID_X, SubjectZoneID_Y);

				*ofsAnalysisPath_linkedCellPosition
					<< std::setw(outwidth) << k * ceilingNum_OneDim[0] * ceilingNum_OneDim[1] + j * ceilingNum_OneDim[0] + i
					<< std::setw(outwidth) << SubjectZoneID
					<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << (i - ZoneCenter[0])*ceil_Interval[0]
					<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << (j - ZoneCenter[1])*ceil_Interval[1]
					<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << newBoundary[2][0] + k*ceil_Interval[2]
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
				if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {
					if (0 != iteratorTrackInfo->GetStepsInfo()->at(index).GetProcessName().compare(std::string("hadElastic"))) {
						break;
					}
				}

				if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

					if (iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getX() < newBoundary[0][0] || 
						iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getX() > newBoundary[0][1] || 
						iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getY() < newBoundary[1][0] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getY() > newBoundary[1][1] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getZ() < newBoundary[2][0] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getZ() > newBoundary[2][1]) {
						continue;
					}

					ceilIndex[0] = min(int((iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getX() - newBoundary[0][0]) / ceil_Interval[0]), ceilingNum_OneDim[0] - 1);
					ceilIndex[1] = min(int((iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getY() - newBoundary[1][0]) / ceil_Interval[1]), ceilingNum_OneDim[1] - 1);
					ceilIndex[2] = min(int((iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getZ() - newBoundary[2][0]) / ceil_Interval[2]), ceilingNum_OneDim[2] - 1);

				}else if (ConcentReaction(InletToFirstNonEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
					ConcentReaction(InletEstAndInEstTillEnd) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
					ConcentReaction(MatrixAtom) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
					ConcentReaction(Iso) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {


					if (iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getX() < newBoundary[0][0] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getX() > newBoundary[0][1] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getY() < newBoundary[1][0] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getY() > newBoundary[1][1] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getZ() < newBoundary[2][0] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getZ() > newBoundary[2][1]) {

						continue;
					}



					ceilIndex[0] = min(int((iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getX() - newBoundary[0][0]) / ceil_Interval[0]), ceilingNum_OneDim[0] - 1);
					ceilIndex[1] = min(int((iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getY() - newBoundary[1][0]) / ceil_Interval[1]), ceilingNum_OneDim[1] - 1);
					ceilIndex[2] = min(int((iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getZ() - newBoundary[2][0]) / ceil_Interval[2]), ceilingNum_OneDim[2] - 1);

				}

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

	for (int j = 0; j < ceilingNum_OneDim[1]; j++) {

		SubjectZoneID_Y = max(ZoneCenter[1] - j,j - ZoneCenter[1]);

		for (int i = 0; i < ceilingNum_OneDim[0]; i++) {

			SubjectZoneID_X = max(ZoneCenter[0] - i, i - ZoneCenter[0]);

			SubjectZoneID = max(SubjectZoneID_X, SubjectZoneID_Y);

			for (int k = 0; k < ceilingNum_OneDim[2]; k++) {

				subjectLinkID = k * ceilingNum_OneDim[0]*ceilingNum_OneDim[1] + j * ceilingNum_OneDim[0] + i;


				if (i == ceilingNum_OneDim[0]  && k == ceilingNum_OneDim[2]) {
					std::cout << "cell ID: " << subjectLinkID << " of total Cell Number: " << ceilingNum << std::endl;
				}

				for (size_t l = 0; l < linkedCells_StepInfo[subjectLinkID].size(); l++) {

					ceilXYCount[i][j]++;

					subjectEventID = linkedCells_EventID[subjectLinkID].at(l);
					subjectTrackID = linkedCells_TrackID[subjectLinkID].at(l);
					subjectStepID = linkedCells_StepInfo[subjectLinkID].at(l)->GetStepID();
					if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {
						subjectPostion = linkedCells_StepInfo[subjectLinkID].at(l)->GetpostPosition();
					}
					else if (ConcentReaction(InletToFirstNonEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
						ConcentReaction(InletEstAndInEstTillEnd) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
						ConcentReaction(MatrixAtom) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
						ConcentReaction(Iso) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

						subjectPostion = linkedCells_StepInfo[subjectLinkID].at(l)->GetprePosition();
					}

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

											if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {
												objectPostion = linkedCells_StepInfo[objectLinkID].at(m)->GetpostPosition();
											}
											else if (ConcentReaction(InletToFirstNonEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
												ConcentReaction(InletEstAndInEstTillEnd) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
												ConcentReaction(MatrixAtom) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
												ConcentReaction(Iso) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

												objectPostion = linkedCells_StepInfo[objectLinkID].at(m)->GetprePosition();
											}

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

												ObjectZoneID_X = max(ZoneCenter[0] - ii, ii - ZoneCenter[0]);
												ObjectZoneID_Y = max(ZoneCenter[1] - jj, jj - ZoneCenter[1]);
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

	for (int ZoneID = 0; ZoneID < ZoneNum; ZoneID++) {

		ZoneCount = 0;

		for (int j = -ZoneID; j <= ZoneID; j = j+1) {

			if (j == -ZoneID || j == ZoneID) {
				iInterval = 1;
			}
			else {
				iInterval = 2 * ZoneID;
			}

			corr_j = ZoneCenter[1] + j;


			for (int i = -ZoneID; i <= ZoneID; i = i + iInterval) {

				corr_i = ZoneCenter[0] + i;

				ZoneCount += ceilXYCount[corr_i][corr_j];

				*ofsAnalysisPath_CeilCount
					<< std::setw(outwidth) << ZoneID
					<< std::setw(outwidth) << corr_j*ceilingNum_OneDim[0] + corr_i
					<< std::setw(outwidth) << i
					<< std::setw(outwidth) << j
					<< std::setw(outwidth) << ceilXYCount[corr_i][corr_j] << std::endl;

			}
		}


		*ofsAnalysisPath_ZoneCount
			<< std::setw(outwidth) << ZoneID
			<< std::setw(outwidth) << ZoneCount << std::endl;

	}



	if (NULL != linkedCells_EventID) delete[] linkedCells_EventID;
	if (NULL != linkedCells_TrackID) delete[] linkedCells_TrackID;
	if (NULL != linkedCells_StepInfo) delete[] linkedCells_StepInfo;

	for (int i = 0; i < ceilingNum_OneDim[0]; i++) {
		delete[] ceilXYCount[i];
		ceilXYCount[i] = NULL;
	}
	delete[] ceilXYCount;

}


void NWAnalysis::New_AnalysisResult(std::map<int, std::vector<TrackInfo>>* storedData) {
	std::fstream ofsOrignalDistance;
	std::fstream ofsAnalysis_EqualInterval;
	std::fstream ofsAnalysis_PowerInterval;
	std::fstream ofsAnalysis_EndReason;
	std::fstream ofsAnalysis_DeviateAxesDistance;
	std::fstream ofsAnalysisPath_DistanceXYZ;
	std::fstream ofsAnalysisPath_linkedCellPosition;
	std::fstream ofsAnalysisPath_ZoneCount;
	std::fstream ofsAnalysisPath_CeilXYCount;
	std::string OrignalDistancePath;
	std::string AnalysisPath_EqualInterval;
	std::string AnalysisPath_PowerInterval;
	std::string AnalysisPath_EndReason;
	std::string AnalysisPath_DeviateAxesDistance;
	std::string AnalysisPath_DistanceXYZ;
	std::string AnalysisPath_linkedCellPosition;
	std::string AnalysisPath_ZoneCount;
	std::string AnalysisPath_CeilXYCount;
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
	double boundary[3][2];
	//---Body---

	boundary[0][0] = 1.e32;
	boundary[1][0] = 1.e32;
	boundary[2][0] = 1.e32;
	boundary[0][1] = -1.e32;
	boundary[1][1] = -1.e32;
	boundary[2][1] = -1.e32;

	outwidth = NWGlobal::GetInstance()->GetSimParamters().GetOutWidth();

	binEnds_PowerInterval = new double[PowerInterval_BinNum];

	score_PowerInterval = new int[PowerInterval_BinNum];

	PowerInterval_DeltaLength = (std::log10(PowerInterval_Max) - std::log10(PowerInterval_Min)) / PowerInterval_BinNum;

	score = NULL;
	binEnds = NULL;


	if (NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->length() > 0) {

		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "New_DistanceResult_OriginDistance.txt";

		ss >> OrignalDistancePath;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "New_DistanceResult_Analysis_EqualInterval.txt";

		ss >> AnalysisPath_EqualInterval;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "New_DistanceResult_Analysis_PowerInterval.txt";

		ss >> AnalysisPath_PowerInterval;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "New_DistanceResult_Analysis_EndReason.txt";

		ss >> AnalysisPath_EndReason;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "New_DistanceResult_Analysis_DeviateAxesDistance.txt";

		ss >> AnalysisPath_DeviateAxesDistance;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "New_DistanceResult_Analysis_DistanceXYZ.txt";

		ss >> AnalysisPath_DistanceXYZ;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "New_DistanceResult_Analysis_linkedCellPosition.txt";

		ss >> AnalysisPath_linkedCellPosition;


		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "New_ZoneCount.txt";

		ss >> AnalysisPath_ZoneCount;



		ss.clear();

		ss.str("");

		ss << NWGlobal::GetInstance()->GetSimParamters().GetOutPath()->c_str() << "\\" << "New_CeilCount.txt";

		ss >> AnalysisPath_CeilXYCount;

	}
	else {
		ss.clear();

		ss.str("");

		ss << "New_DistanceResult_OriginDistance.txt";

		ss >> OrignalDistancePath;


		ss.clear();

		ss.str("");

		ss << "New_DistanceResult_Analysis_EqualInterval.txt";

		ss >> AnalysisPath_EqualInterval;


		ss.clear();

		ss.str("");

		ss << "New_DistanceResult_Analysis_PowerInterval.txt";

		ss >> AnalysisPath_PowerInterval;


		ss.clear();

		ss.str("");

		ss << "New_DistanceResult_Analysis_EndReason.txt";

		ss >> AnalysisPath_EndReason;


		ss.clear();

		ss.str("");

		ss << "New_DistanceResult_Analysis_DeviateAxesDistance.txt";

		ss >> AnalysisPath_DeviateAxesDistance;


		ss.clear();

		ss.str("");

		ss << "New_DistanceResult_Analysis_DistanceXYZ.txt";

		ss >> AnalysisPath_DistanceXYZ;


		ss.clear();

		ss.str("");

		ss << "New_DistanceResult_Analysis_linkedCellPosition.txt";

		ss >> AnalysisPath_linkedCellPosition;



		ss.clear();

		ss.str("");

		ss << "New_ZoneCount.txt";

		ss >> AnalysisPath_ZoneCount;


		ss.clear();

		ss.str("");

		ss << "New_CeilCount.txt";

		ss >> AnalysisPath_CeilXYCount;

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
		<< std::setw(outwidth) << "TruePos_X(mm)"
		<< std::setw(outwidth) << "TruePos_Y(mm)"
		<< std::setw(outwidth) << "TruePos_Z(mm)"
		<< std::setw(outwidth) << "ShiftPos_X(mm)"
		<< std::setw(outwidth) << "ShiftPos_Y(mm)"
		<< std::setw(outwidth) << "ShiftPos_Z(mm)"
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


	ofsAnalysisPath_ZoneCount.open(AnalysisPath_ZoneCount, std::ios::out | std::ios::ate);

	ofsAnalysisPath_ZoneCount
		<< std::setw(outwidth) << "ZoneID"
		<< std::setw(outwidth) << "Count" << std::endl;



	ofsAnalysisPath_CeilXYCount.open(AnalysisPath_CeilXYCount, std::ios::out | std::ios::ate);

	ofsAnalysisPath_CeilXYCount
		<< std::setw(outwidth) << "ZoneID"
		<< std::setw(outwidth) << "CeilXYID"
		<< std::setw(outwidth) << "ZoneX"
		<< std::setw(outwidth) << "ZoneY"
		<< std::setw(outwidth) << "Count" << std::endl;


	it = storedData->begin();

	for (; it != storedData->end(); it++) {
		iteratorTrackInfo = it->second.begin();

		index = 0;

		for (; iteratorTrackInfo != it->second.end(); iteratorTrackInfo++) {

			iteratorStepInfo = iteratorTrackInfo->GetStepsInfo()->begin();

			if (iteratorTrackInfo->GetStepsInfo()->size() > 0) {

				for (; iteratorStepInfo != iteratorTrackInfo->GetStepsInfo()->end(); iteratorStepInfo++) {

					if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

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

						if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {
							prePKAPos = iteratorStepInfo->GetpostPosition();
							postPKAPos = iteratorStepInfo->GetpostPosition();

						}
						else if (ConcentReaction(InletToFirstNonEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
							ConcentReaction(InletEstAndInEstTillEnd) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
							ConcentReaction(MatrixAtom) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
							ConcentReaction(Iso) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

							prePKAPos = iteratorStepInfo->GetprePosition();
							postPKAPos = iteratorStepInfo->GetprePosition();
						}
					}
					else {

						if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {
							postPKAPos = iteratorStepInfo->GetpostPosition();

						}
						else if (ConcentReaction(InletToFirstNonEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
							ConcentReaction(InletEstAndInEstTillEnd) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
							ConcentReaction(MatrixAtom) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
							ConcentReaction(Iso) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

							postPKAPos = iteratorStepInfo->GetprePosition();
						}

						pKADist = prePKAPos - postPKAPos;

						distance = pKADist.mag();

						ofsOrignalDistance << std::setiosflags(std::ios::scientific) << std::setprecision(7) << distance << std::endl;

						prePKAPos = postPKAPos;

						theDistance.push_back(distance);

						maxDistance = std::max(maxDistance, distance);
						minDistance = std::min(minDistance, distance);
					}

					if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {
						vectorToOrigin = iteratorStepInfo->GetpostPosition() - iteratorStepInfo->GetOriginPosition();

						boundary[0][0] = min(iteratorStepInfo->GetpostPosition().getX(), boundary[0][0]);
						boundary[1][0] = min(iteratorStepInfo->GetpostPosition().getY(), boundary[1][0]);
						boundary[2][0] = min(iteratorStepInfo->GetpostPosition().getZ(), boundary[2][0]);
						boundary[0][1] = max(iteratorStepInfo->GetpostPosition().getX(), boundary[0][1]);
						boundary[1][1] = max(iteratorStepInfo->GetpostPosition().getY(), boundary[1][1]);
						boundary[2][1] = max(iteratorStepInfo->GetpostPosition().getZ(), boundary[2][1]);

					}
					else if (ConcentReaction(InletToFirstNonEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
						ConcentReaction(InletEstAndInEstTillEnd) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
						ConcentReaction(MatrixAtom) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
						ConcentReaction(Iso) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

						vectorToOrigin = iteratorStepInfo->GetprePosition() - iteratorStepInfo->GetOriginPosition();

						boundary[0][0] = min(iteratorStepInfo->GetprePosition().getX(), boundary[0][0]);
						boundary[1][0] = min(iteratorStepInfo->GetprePosition().getY(), boundary[1][0]);
						boundary[2][0] = min(iteratorStepInfo->GetprePosition().getZ(), boundary[2][0]);
						boundary[0][1] = max(iteratorStepInfo->GetprePosition().getX(), boundary[0][1]);
						boundary[1][1] = max(iteratorStepInfo->GetprePosition().getY(), boundary[1][1]);
						boundary[2][1] = max(iteratorStepInfo->GetprePosition().getZ(), boundary[2][1]);

					}

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

	if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
		ConcentReaction(InletToFirstNonEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
		ConcentReaction(InletEstAndInEstTillEnd) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
		ConcentReaction(MatrixAtom) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

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
			binEnds_PowerInterval[i] = PowerInterval_Min * std::pow(10, i*PowerInterval_DeltaLength);
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

			score[i] = score[i] / pow(10, std::max(i - 1, 0) / BinNumberEachPower);

		}


		for (int i = 0; i < binNum; i++) {
			ofsAnalysis_EqualInterval << std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << binEnds[i]
				<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << score[i] << std::endl;
		}


		for (int i = 0; i < PowerInterval_BinNum; i++) {
			ofsAnalysis_PowerInterval << std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << binEnds_PowerInterval[i]
				<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << score_PowerInterval[i] << std::endl;
		}

	}

	New_Cal_MinDist_LinkedCell(storedData, boundary, &ofsAnalysisPath_DistanceXYZ, &ofsAnalysisPath_linkedCellPosition, &ofsAnalysisPath_ZoneCount, &ofsAnalysisPath_CeilXYCount);


	ofsAnalysis_EqualInterval.close();

	ofsAnalysis_PowerInterval.close();

	ofsOrignalDistance.close();

	ofsAnalysis_EndReason.close();

	ofsAnalysis_DeviateAxesDistance.close();

	ofsAnalysisPath_DistanceXYZ.close();

	ofsAnalysisPath_linkedCellPosition.close();

	ofsAnalysisPath_ZoneCount.close();

	ofsAnalysisPath_CeilXYCount.close();

	if (NULL != binEnds) delete[] binEnds;
	if (NULL != score) delete[] score;

	if (NULL != binEnds_PowerInterval) delete[] binEnds_PowerInterval;
	if (NULL != score_PowerInterval) delete[] score_PowerInterval;
}


void NWAnalysis::New_Cal_MinDist_LinkedCell(std::map<int, std::vector<TrackInfo>>* storedData, double boundary[][2],
	fstream* ofsAnalysisPath_DistanceXYZ, fstream *ofsAnalysisPath_linkedCellPosition,
	fstream* ofsAnalysisPath_ZoneCount, fstream* ofsAnalysisPath_CeilCount) {
	/*Local Vars*/
	double ceil_Interval[3];
	int ceilingNum_OneDim[3];
	int ceilIndex[3];
	double newBoundary[3][2];
	double beamCenter[2];
	int linkID;
	int subjectLinkID;
	int objectLinkID;
	std::map<int, std::vector<TrackInfo>>::iterator it;
	std::vector<TrackInfo>::iterator iteratorTrackInfo;
	std::vector<int>* linkedCells_EventID = NULL;
	std::vector<int>* linkedCells_TrackID = NULL;
	std::vector<StepInfo*>* linkedCells_StepInfo = NULL;
	std::vector<G4ThreeVector>* linkedCells_ShiftPos = NULL;
	int** ceilXYCount;
	G4ThreeVector shiftPosition;
	G4ThreeVector subjectTruePosition;
	G4ThreeVector objectTruePosition;
	G4ThreeVector subjectShiftPostion;
	G4ThreeVector objectShiftPostion;
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
	int iInterval;
	int ZoneCount;
	int corr_i;
	int corr_j;
	/*Body*/

	ceil_Interval[0] = ceil_Interval[1] = NWGlobal::GetInstance()->GetSimParamters().GetLinkCellInterval_xy();

	ceil_Interval[2] = NWGlobal::GetInstance()->GetSimParamters().GetLinkCellInterval_z();

	NWGlobal::GetInstance()->GetSimParamters().GetNWBeam().GetFluxCenter(beamCenter);

	for (int i = 0; i <= 1; i++) {
		ceilingNum_OneDim[i] = 2 * ceil(max(beamCenter[i] - boundary[i][0], boundary[i][1] - beamCenter[i]) / ceil_Interval[i]) + 1;
		ceilingNum_OneDim[i] = max(ceilingNum_OneDim[i], 3);
	}

	ceilingNum_OneDim[0] = ceilingNum_OneDim[1] = max(ceilingNum_OneDim[0], ceilingNum_OneDim[1]);

	ceilingNum_OneDim[2] = ceil((boundary[2][1] - boundary[2][0]) / ceil_Interval[2]);
	ceilingNum_OneDim[2] = max(ceilingNum_OneDim[2], 1);

	for (int i = 0; i < 3; i++) {
		ceilIndex[i] = 0;
	}

	std::cout << "boundary_x " << boundary[0][0] << " " << boundary[0][1] << std::endl;
	std::cout << "boundary_y " << boundary[1][0] << " " << boundary[1][1] << std::endl;
	std::cout << "boundary_z " << boundary[2][0] << " " << boundary[2][1] << std::endl;

	std::cout << "cellnum_x " << ceilingNum_OneDim[0] << std::endl;
	std::cout << "cellnum_y " << ceilingNum_OneDim[1] << std::endl;
	std::cout << "cellnum_z " << ceilingNum_OneDim[2] << std::endl;


	/*Zone ID start from 0(Center zone)*/
	ZoneNum = ceilingNum_OneDim[0] / 2 + 1;
	ZoneCenter[0] = ZoneNum - 1;
	ZoneCenter[1] = ZoneCenter[0];

	outwidth = NWGlobal::GetInstance()->GetSimParamters().GetOutWidth();


	for (int i = 0; i <= 1; i++) {
		newBoundary[i][0] = (beamCenter[i] - (ZoneNum - 1 + 0.50)*ceil_Interval[i]);
		newBoundary[i][1] = (beamCenter[i] + (ZoneNum - 1 + 0.50)*ceil_Interval[i]);
	}
	newBoundary[2][0] = boundary[2][0];
	newBoundary[2][1] = boundary[2][0] + ceilingNum_OneDim[2]*ceil_Interval[2];


	std::cout << "new_boundary_x " << newBoundary[0][0] << " " << newBoundary[0][1] << std::endl;
	std::cout << "new_boundary_y " << newBoundary[1][0] << " " << newBoundary[1][1] << std::endl;
	std::cout << "new_boundary_z " << newBoundary[2][0] << " " << newBoundary[2][1] << std::endl;


	linkedCells_EventID = new std::vector<int>[ceilingNum_OneDim[2]];
	linkedCells_TrackID = new std::vector<int>[ceilingNum_OneDim[2]];
	linkedCells_StepInfo = new std::vector<StepInfo*>[ceilingNum_OneDim[2]];
	linkedCells_ShiftPos = new std::vector<G4ThreeVector>[ceilingNum_OneDim[2]];


	ceilXYCount = new int*[ceilingNum_OneDim[0]];
	for (int i = 0; i < ceilingNum_OneDim[0]; i++) {
		ceilXYCount[i] = new int[ceilingNum_OneDim[1]];

		for (int j = 0; j < ceilingNum_OneDim[1]; j++) {
			ceilXYCount[i][j] = 0;
		}
	}


	for (int i = 0; i < ceilingNum_OneDim[2]; i++) {
		std::vector<int>().swap(linkedCells_EventID[i]);
		std::vector<int>().swap(linkedCells_TrackID[i]);
		std::vector<StepInfo*>().swap(linkedCells_StepInfo[i]);
	}


	for (int k = 0; k < ceilingNum_OneDim[2]; k++) {
		for (int j = 0; j < ceilingNum_OneDim[1]; j++) {
			SubjectZoneID_Y = max(ZoneCenter[1] - j, j - ZoneCenter[1]);

			for (int i = 0; i < ceilingNum_OneDim[0]; i++) {

				SubjectZoneID_X = max(ZoneCenter[0] - i, i - ZoneCenter[0]);

				SubjectZoneID = max(SubjectZoneID_X, SubjectZoneID_Y);

				*ofsAnalysisPath_linkedCellPosition
					<< std::setw(outwidth) << k * ceilingNum_OneDim[0] * ceilingNum_OneDim[1] + j * ceilingNum_OneDim[0] + i
					<< std::setw(outwidth) << SubjectZoneID
					<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << (i - ZoneCenter[0])*ceil_Interval[0]
					<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << (j - ZoneCenter[1])*ceil_Interval[1]
					<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << newBoundary[2][0] + k * ceil_Interval[2]
					<< std::endl;
			}
		}
	}

	it = storedData->begin();

	for (; it != storedData->end(); it++) {
		iteratorTrackInfo = it->second.begin();

		for (; iteratorTrackInfo != it->second.end(); iteratorTrackInfo++) {

			theSize = iteratorTrackInfo->GetStepsInfo()->size();

			for (int index = 0; index < theSize; index++) {
				if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {
					if (0 != iteratorTrackInfo->GetStepsInfo()->at(index).GetProcessName().compare(std::string("hadElastic"))) {
						break;
					}
				}

				if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

					if (iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getX() < newBoundary[0][0] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getX() > newBoundary[0][1] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getY() < newBoundary[1][0] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getY() > newBoundary[1][1] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getZ() < newBoundary[2][0] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getZ() > newBoundary[2][1]) {
						continue;
					}

					ceilIndex[0] = min(int((iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getX() - newBoundary[0][0]) / ceil_Interval[0]), ceilingNum_OneDim[0] - 1);
					ceilIndex[1] = min(int((iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getY() - newBoundary[1][0]) / ceil_Interval[1]), ceilingNum_OneDim[1] - 1);
					ceilIndex[2] = min(int((iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition().getZ() - newBoundary[2][0]) / ceil_Interval[2]), ceilingNum_OneDim[2] - 1);

					shiftPosition = iteratorTrackInfo->GetStepsInfo()->at(index).GetpostPosition();

				}else if (ConcentReaction(InletToFirstNonEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
					ConcentReaction(InletEstAndInEstTillEnd) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
					ConcentReaction(MatrixAtom) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
					ConcentReaction(Iso) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {


					if (iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getX() < newBoundary[0][0] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getX() > newBoundary[0][1] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getY() < newBoundary[1][0] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getY() > newBoundary[1][1] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getZ() < newBoundary[2][0] ||
						iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getZ() > newBoundary[2][1]) {

						continue;
					}

					ceilIndex[0] = min(int((iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getX() - newBoundary[0][0]) / ceil_Interval[0]), ceilingNum_OneDim[0] - 1);
					ceilIndex[1] = min(int((iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getY() - newBoundary[1][0]) / ceil_Interval[1]), ceilingNum_OneDim[1] - 1);
					ceilIndex[2] = min(int((iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition().getZ() - newBoundary[2][0]) / ceil_Interval[2]), ceilingNum_OneDim[2] - 1);

					shiftPosition = iteratorTrackInfo->GetStepsInfo()->at(index).GetprePosition();
				}

				ceilIndex[0] = max(ceilIndex[0], 0);
				ceilIndex[1] = max(ceilIndex[1], 0);
				ceilIndex[2] = max(ceilIndex[2], 0);


				ceilXYCount[ceilIndex[0]][ceilIndex[1]] += 1;

				shiftPosition[0] = shiftPosition[0] - (ceilIndex[0] - ZoneCenter[0])*ceil_Interval[0];
				shiftPosition[1] = shiftPosition[1] - (ceilIndex[1] - ZoneCenter[1])*ceil_Interval[1];

				linkID = ceilIndex[2];
				linkedCells_EventID[linkID].push_back(it->first);
				linkedCells_TrackID[linkID].push_back(iteratorTrackInfo->GetTrackID());
				linkedCells_StepInfo[linkID].push_back(&iteratorTrackInfo->GetStepsInfo()->at(index));
				linkedCells_ShiftPos[linkID].push_back(shiftPosition);
			}

		}
	}


	for (int k = 0; k < ceilingNum_OneDim[2]; k++) {

		subjectLinkID = k;

		for (size_t l = 0; l < linkedCells_StepInfo[subjectLinkID].size(); l++) {

			subjectEventID = linkedCells_EventID[subjectLinkID].at(l);
			subjectTrackID = linkedCells_TrackID[subjectLinkID].at(l);
			subjectStepID = linkedCells_StepInfo[subjectLinkID].at(l)->GetStepID();

			if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {
				subjectTruePosition = linkedCells_StepInfo[subjectLinkID].at(l)->GetpostPosition();
			}else if (ConcentReaction(InletToFirstNonEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
					  ConcentReaction(InletEstAndInEstTillEnd) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
					  ConcentReaction(MatrixAtom) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
					  ConcentReaction(Iso) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

				subjectTruePosition = linkedCells_StepInfo[subjectLinkID].at(l)->GetprePosition();
			}

			subjectShiftPostion = linkedCells_ShiftPos[subjectLinkID].at(l);

			ceilIndex[0] = min(int((subjectTruePosition.getX() - newBoundary[0][0]) / ceil_Interval[0]), ceilingNum_OneDim[0] - 1);
			ceilIndex[1] = min(int((subjectTruePosition.getY() - newBoundary[1][0]) / ceil_Interval[1]), ceilingNum_OneDim[1] - 1);

			SubjectZoneID_X = max(ZoneCenter[0] - ceilIndex[0], ceilIndex[0] - ZoneCenter[0]);
			SubjectZoneID_Y = max(ZoneCenter[1] - ceilIndex[1], ceilIndex[1] - ZoneCenter[1]);
			SubjectZoneID = max(SubjectZoneID_X, SubjectZoneID_Y);

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

					objectLinkID = kk;

					for (size_t m = 0; m < linkedCells_StepInfo[objectLinkID].size(); m++) {

						int objectEventID = linkedCells_EventID[objectLinkID].at(m);
						int objectTrackID = linkedCells_TrackID[objectLinkID].at(m);
						int objectStepID = linkedCells_StepInfo[objectLinkID].at(m)->GetStepID();

						if (subjectEventID != objectEventID || subjectTrackID != objectTrackID || subjectStepID != objectStepID) {

							if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {
								objectTruePosition = linkedCells_StepInfo[objectLinkID].at(m)->GetpostPosition();
							}
							else if (ConcentReaction(InletToFirstNonEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
									ConcentReaction(InletEstAndInEstTillEnd) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
									ConcentReaction(MatrixAtom) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
									ConcentReaction(Iso) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

								objectTruePosition = linkedCells_StepInfo[objectLinkID].at(m)->GetprePosition();
							}

							objectShiftPostion = linkedCells_ShiftPos[objectLinkID].at(m);

							pKADist = subjectShiftPostion - objectShiftPostion;
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


								ceilIndex[0] = min(int((objectTruePosition.getX() - newBoundary[0][0]) / ceil_Interval[0]), ceilingNum_OneDim[0] - 1);
								ceilIndex[1] = min(int((objectTruePosition.getY() - newBoundary[1][0]) / ceil_Interval[1]), ceilingNum_OneDim[1] - 1);

								ObjectZoneID_X = max(ZoneCenter[0] - ceilIndex[0], ceilIndex[0] - ZoneCenter[0]);
								ObjectZoneID_Y = max(ZoneCenter[1] - ceilIndex[1], ceilIndex[1] - ZoneCenter[1]);
								ObjectZoneID = max(ObjectZoneID_X, ObjectZoneID_Y);

								founded = true;
							}

						}

					}

				}

				shellNum = shellNum + 1;
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
						<< std::setw(outwidth) << subjectTruePosition.getX()
						<< std::setw(outwidth) << subjectTruePosition.getY()
						<< std::setw(outwidth) << subjectTruePosition.getZ()
						<< std::setw(outwidth) << subjectShiftPostion.getX()
						<< std::setw(outwidth) << subjectShiftPostion.getY()
						<< std::setw(outwidth) << subjectShiftPostion.getZ()
						<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << minDist_x
						<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << minDist_y
						<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << minDist_z
						<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << minDist << std::endl;
		}

	}


	for (int ZoneID = 0; ZoneID < ZoneNum; ZoneID++) {

		ZoneCount = 0;

		for (int j = -ZoneID; j <= ZoneID; j = j + 1) {

			if (j == -ZoneID || j == ZoneID) {
				iInterval = 1;
			}
			else {
				iInterval = 2 * ZoneID;
			}

			corr_j = ZoneCenter[1] + j;


			for (int i = -ZoneID; i <= ZoneID; i = i + iInterval) {

				corr_i = ZoneCenter[0] + i;

				ZoneCount += ceilXYCount[corr_i][corr_j];

				*ofsAnalysisPath_CeilCount
					<< std::setw(outwidth) << ZoneID
					<< std::setw(outwidth) << corr_j * ceilingNum_OneDim[0] + corr_i
					<< std::setw(outwidth) << i
					<< std::setw(outwidth) << j
					<< std::setw(outwidth) << ceilXYCount[corr_i][corr_j] << std::endl;

			}
		}


		*ofsAnalysisPath_ZoneCount
			<< std::setw(outwidth) << ZoneID
			<< std::setw(outwidth) << ZoneCount << std::endl;

	}



	if (NULL != linkedCells_EventID) delete[] linkedCells_EventID;
	if (NULL != linkedCells_TrackID) delete[] linkedCells_TrackID;
	if (NULL != linkedCells_StepInfo) delete[] linkedCells_StepInfo;

	for (int i = 0; i < ceilingNum_OneDim[0]; i++) {
		delete[] ceilXYCount[i];
		ceilXYCount[i] = NULL;
	}
	delete[] ceilXYCount;

}





NWAnalysis::NWAnalysisGarbo::NWAnalysisGarbo() {


}

NWAnalysis::NWAnalysisGarbo::~NWAnalysisGarbo() {

	ReleaseInstance();
}

