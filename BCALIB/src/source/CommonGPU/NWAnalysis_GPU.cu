#include "NWAnalysis_GPU.cuh"
#include "NWGlobal.h"
#include "stdio.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

__global__ void Kernel_Statistic(G4ThreeVector* Dev_ShiftPos,int *Dev_CeilZID, int *Dev_StartID,int *Dev_PKANumEachCeilZ,int *Dev_ResultInnerCeilZID, int *Dev_ResultCeilZID, int totalCeilZ,int NPKA) {
	int tid = threadIdx.y*blockDim.x + threadIdx.x;
	int bid = blockIdx.y*gridDim.x + blockIdx.x;
	int cid = bid * BLOCKSIZE + tid;
	int shellNum = 0;
	bool founded = false;
	G4ThreeVector subjectShiftPos;
	G4ThreeVector objectShiftPos;
	G4ThreeVector pKADist;
	double distance;
	double minDist;
	int resultInnerCeilZID;
	int resultCeilZID;
	/*Body*/

	minDist = 1.e32;

	if (cid < NPKA) {
		int CeilZID = Dev_CeilZID[cid];

		subjectShiftPos = Dev_ShiftPos[cid];

		while (!founded) {
			for (int z = std::max(CeilZID - shellNum,0); z < std::min(CeilZID + shellNum+1, totalCeilZ); z++) {

				for (int kk = Dev_StartID[z]; kk < Dev_StartID[z] + Dev_PKANumEachCeilZ[z]; kk++) {

					if (kk != cid) {
						objectShiftPos = Dev_ShiftPos[kk];

						pKADist = subjectShiftPos - objectShiftPos;
						distance = pKADist.mag();

						if (distance < minDist) {

							minDist = distance;

							founded = true;

							resultInnerCeilZID = kk;

							resultCeilZID = z;
						}


					}

				}

			}

			shellNum++;
		}

		Dev_ResultInnerCeilZID[cid] = resultInnerCeilZID;
		Dev_ResultCeilZID[cid] = resultCeilZID;
	}


}

void Dev_Statistic(std::vector<int>* linkedCells_EventID,
					std::vector<int>* linkedCells_TrackID,
					std::vector<int>* linkedCells_ZoneID,
					std::vector<StepInfo*>* linkedCells_StepInfo,
					std::vector<G4ThreeVector>* linkedCells_ShiftPos,
					int totalCeilZ, int totalSize,std::fstream* ofsAnalysisPath_DistanceXYZ) {

	/*Local Vars*/
	G4ThreeVector* ShiftPos;
	G4ThreeVector* Dev_ShiftPos;
	int *CeilZID;
	int *Dev_CeilZID;
	int *ResultInnerCeilZID;
	int *Dev_ResultInnerCeilZID;
	int *ResultCeilZID;
	int *Dev_ResultCeilZID;
	int *StartID;
	int *Dev_StartID;
	int *PKANumEachCeilZ;
	int *Dev_PKANumEachCeilZ;
	G4ThreeVector subjectTruePosition;
	G4ThreeVector objectTruePosition;
	G4ThreeVector subjectShifPos;
	G4ThreeVector objectShifPos;
	G4ThreeVector pKADist;
	cudaError_t cudaStatus;
	int index;
	dim3 blocks;
	dim3 threads;
	int NB;
	int NBX;
	int NBY;
	int outwidth;
	/*Body*/

	outwidth = NWGlobal::GetInstance()->GetSimParamters().GetOutWidth();

	ShiftPos = new G4ThreeVector[totalSize];

	ResultInnerCeilZID = new int[totalSize];

	ResultCeilZID = new int[totalSize];

	CeilZID = new int[totalSize];

	StartID = new int[totalCeilZ];

	PKANumEachCeilZ = new int[totalCeilZ];
	index = 0;

	for (int i = 0; i < totalCeilZ; i++) {

		StartID[i] = index;

		for (std::vector<G4ThreeVector>::iterator it = linkedCells_ShiftPos[i].begin; it != linkedCells_ShiftPos[i].end; it++) {
			ShiftPos[index] = *it;
			CeilZID[index] = i;

			index++;
		}

		PKANumEachCeilZ[i] = linkedCells_ShiftPos[i].size();
	}

	cudaStatus = cudaMalloc((void**)&Dev_ShiftPos, sizeof(G4ThreeVector)*totalSize);
	if (cudaStatus != cudaSuccess) {
		std::cout << "The memory allocate not right !" << std::endl;
		system("pause");
		exit(1);
	}

	cudaStatus = cudaMalloc((void**)&Dev_ResultInnerCeilZID, sizeof(int)*totalSize);
	if (cudaStatus != cudaSuccess) {
		std::cout << "The memory allocate not right !" << std::endl;
		system("pause");
		exit(1);
	}

	cudaStatus = cudaMalloc((void**)&Dev_ResultCeilZID, sizeof(int)*totalSize);
	if (cudaStatus != cudaSuccess) {
		std::cout << "The memory allocate not right !" << std::endl;
		system("pause");
		exit(1);
	}

	cudaStatus = cudaMalloc((void**)&Dev_CeilZID, sizeof(int)*totalSize);
	if (cudaStatus != cudaSuccess) {
		std::cout << "The memory allocate not right !" << std::endl;
		system("pause");
		exit(1);
	}


	cudaStatus = cudaMalloc((void**)&Dev_StartID, sizeof(int)*totalCeilZ);
	if (cudaStatus != cudaSuccess) {
		std::cout << "The memory allocate not right !" << std::endl;
		system("pause");
		exit(1);
	}

	cudaStatus = cudaMalloc((void**)&Dev_PKANumEachCeilZ, sizeof(int)*totalCeilZ);
	if (cudaStatus != cudaSuccess) {
		std::cout << "The memory allocate not right !" << std::endl;
		system("pause");
		exit(1);
	}

	cudaStatus = cudaMemcpy(Dev_ShiftPos, ShiftPos, sizeof(G4ThreeVector)*totalSize, cudaMemcpyHostToDevice);

	cudaStatus = cudaMemcpy(Dev_CeilZID, CeilZID, sizeof(int)*totalSize, cudaMemcpyHostToDevice);

	cudaStatus = cudaMemcpy(Dev_StartID, StartID, sizeof(int)*totalCeilZ, cudaMemcpyHostToDevice);

	cudaStatus = cudaMemcpy(Dev_PKANumEachCeilZ, PKANumEachCeilZ, sizeof(int)*totalCeilZ, cudaMemcpyHostToDevice);

	NB = (totalSize - 1) / BLOCKSIZE + 1;

	NBX = GRIDDIMX;
	NBY = (NB - 1) / NBX + 1;

	blocks = dim3(NBX, NBY);
	threads = dim3(BLOCKSIZE, 1);

	Kernel_Statistic<<<blocks, threads >>>(Dev_ShiftPos, Dev_CeilZID, Dev_StartID, Dev_PKANumEachCeilZ,Dev_ResultInnerCeilZID, Dev_ResultCeilZID,totalCeilZ, totalSize);

	cudaStatus = cudaMemcpy(ResultInnerCeilZID, Dev_ResultInnerCeilZID, sizeof(int)*totalSize, cudaMemcpyDeviceToHost);
	cudaStatus = cudaMemcpy(ResultCeilZID, Dev_ResultCeilZID, sizeof(int)*totalSize, cudaMemcpyDeviceToHost);
	/*OutPut*/

	index = 0;

	for (int i = 0; i < totalCeilZ; i++) {

		for (int j = 0; j<linkedCells_ShiftPos[i].size(); j++) {
			subjectShifPos = linkedCells_ShiftPos[i].at(j);
			objectShifPos = linkedCells_ShiftPos[ResultCeilZID[index]].at(ResultInnerCeilZID[index]);
			pKADist = subjectShifPos - objectShifPos;

			if (ConcentReaction(InletToLastEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {
				subjectTruePosition = linkedCells_StepInfo[i].at(j)->GetpostPosition();
				objectTruePosition = linkedCells_StepInfo[ResultCeilZID[index]].at(ResultInnerCeilZID[index])->GetpostPosition();
			}
			else if (ConcentReaction(InletToFirstNonEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
				ConcentReaction(InletEstAndInEstTillEnd) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
				ConcentReaction(MatrixAtom) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
				ConcentReaction(Iso) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction()) {

				subjectTruePosition = linkedCells_StepInfo[i].at(j)->GetprePosition();
				objectTruePosition = linkedCells_StepInfo[ResultCeilZID[index]].at(ResultInnerCeilZID[index])->GetprePosition();
			}


			*ofsAnalysisPath_DistanceXYZ
				<< std::setw(outwidth) << linkedCells_ZoneID[i].at(j)
				<< std::setw(outwidth) << i
				<< std::setw(outwidth) << linkedCells_EventID[i].at(j)
				<< std::setw(outwidth) << linkedCells_TrackID[i].at(j)
				<< std::setw(outwidth) << linkedCells_StepInfo[i].at(i)->GetStepID()
				<< std::setw(outwidth) << linkedCells_ZoneID[ResultCeilZID[index]].at[ResultInnerCeilZID[index]]
				<< std::setw(outwidth) << j
				<< std::setw(outwidth) << linkedCells_EventID[ResultCeilZID[index]].at[ResultInnerCeilZID[index]]
				<< std::setw(outwidth) << linkedCells_TrackID[ResultCeilZID[index]].at[ResultInnerCeilZID[index]]
				<< std::setw(outwidth) << linkedCells_StepInfo[ResultCeilZID[index]].at[ResultInnerCeilZID[index]]->GetStepID()
				<< std::setw(outwidth) << subjectTruePosition.getX()
				<< std::setw(outwidth) << subjectTruePosition.getY()
				<< std::setw(outwidth) << subjectTruePosition.getZ()
				<< std::setw(outwidth) << subjectShifPos.getX()
				<< std::setw(outwidth) << subjectShifPos.getY()
				<< std::setw(outwidth) << subjectShifPos.getZ()
				<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << abs(pKADist.getX())
				<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << abs(pKADist.getY())
				<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << abs(pKADist.getZ())
				<< std::setw(outwidth) << std::setiosflags(std::ios::scientific) << std::setprecision(7) << pKADist.mag() << std::endl;


			index++;
		}
	}


	/*Memory Free*/
	if (NULL != ShiftPos) delete[] ShiftPos;
	if (NULL != ShiftPos) delete[] ResultInnerCeilZID;
	if (NULL != ShiftPos) delete[] ResultCeilZID;
	if (NULL != ShiftPos) delete[] StartID;
	if (NULL != ShiftPos) delete[] PKANumEachCeilZ;

	cudaFree(Dev_ShiftPos);
	cudaFree(Dev_CeilZID);
	cudaFree(Dev_ResultInnerCeilZID);
	cudaFree(Dev_ResultCeilZID);
	cudaFree(Dev_StartID);
	cudaFree(Dev_PKANumEachCeilZ);

}


void Dev_Cal_MinDist_LinkedCell(std::map<int, std::vector<TrackInfo>>* storedData, double boundary[][2],
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
	std::vector<int>* linkedCells_ZoneID = NULL;
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
	int totalSize;
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
	newBoundary[2][1] = boundary[2][0] + ceilingNum_OneDim[2] * ceil_Interval[2];


	std::cout << "new_boundary_x " << newBoundary[0][0] << " " << newBoundary[0][1] << std::endl;
	std::cout << "new_boundary_y " << newBoundary[1][0] << " " << newBoundary[1][1] << std::endl;
	std::cout << "new_boundary_z " << newBoundary[2][0] << " " << newBoundary[2][1] << std::endl;


	linkedCells_EventID = new std::vector<int>[ceilingNum_OneDim[2]];
	linkedCells_TrackID = new std::vector<int>[ceilingNum_OneDim[2]];
	linkedCells_ZoneID = new std::vector<int>[ceilingNum_OneDim[2]];
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
		std::vector<int>().swap(linkedCells_ZoneID[i]);
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


	totalSize = 0;
	it = storedData->begin();

	for (; it != storedData->end(); it++) {
		iteratorTrackInfo = it->second.begin();

		for (; iteratorTrackInfo != it->second.end(); iteratorTrackInfo++) {

			theSize = iteratorTrackInfo->GetStepsInfo()->size();

			for (int index = 0; index < theSize; index++) {

				totalSize++;

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

				}
				else if (ConcentReaction(InletToFirstNonEst) == NWGlobal::GetInstance()->GetSimParamters().GetTheConcentReaction() ||
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

				SubjectZoneID_X = max(ZoneCenter[0] - ceilIndex[0], ceilIndex[0] - ZoneCenter[0]);
				SubjectZoneID_Y = max(ZoneCenter[1] - ceilIndex[1], ceilIndex[1] - ZoneCenter[1]);
				SubjectZoneID = max(SubjectZoneID_X, SubjectZoneID_Y);


				ceilXYCount[ceilIndex[0]][ceilIndex[1]] += 1;

				shiftPosition[0] = shiftPosition[0] - (ceilIndex[0] - ZoneCenter[0])*ceil_Interval[0];
				shiftPosition[1] = shiftPosition[1] - (ceilIndex[1] - ZoneCenter[1])*ceil_Interval[1];

				linkID = ceilIndex[2];
				linkedCells_ZoneID[linkID].push_back(SubjectZoneID);
				linkedCells_EventID[linkID].push_back(it->first);
				linkedCells_TrackID[linkID].push_back(iteratorTrackInfo->GetTrackID());
				linkedCells_StepInfo[linkID].push_back(&iteratorTrackInfo->GetStepsInfo()->at(index));
				linkedCells_ShiftPos[linkID].push_back(shiftPosition);
			}

		}
	}


	Dev_Statistic(linkedCells_EventID,
					linkedCells_TrackID,
					linkedCells_ZoneID,
					linkedCells_StepInfo,
					linkedCells_ShiftPos,
					ceilingNum_OneDim[2], totalSize,ofsAnalysisPath_DistanceXYZ);


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
	if (NULL != linkedCells_ZoneID) delete[] linkedCells_ZoneID;
	if (NULL != linkedCells_StepInfo) delete[] linkedCells_StepInfo;

	for (int i = 0; i < ceilingNum_OneDim[0]; i++) {
		delete[] ceilXYCount[i];
		ceilXYCount[i] = NULL;
	}
	delete[] ceilXYCount;

}


NWAnalysis_GPU::NWAnalysis_GPU() {

}

NWAnalysis_GPU::~NWAnalysis_GPU() {
}

void NWAnalysis_GPU::AnalysisResult(std::map<int, std::vector<TrackInfo>>* storedData) {
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

	Dev_Cal_MinDist_LinkedCell(storedData, boundary, &ofsAnalysisPath_DistanceXYZ, &ofsAnalysisPath_linkedCellPosition, &ofsAnalysisPath_ZoneCount, &ofsAnalysisPath_CeilXYCount);


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