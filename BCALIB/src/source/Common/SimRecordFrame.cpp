#include "SimRecordFrame.h"
#include "stdlib.h"

//The singleton
SimRecordFrame* SimRecordFrame::simRecordFrameInstance = new SimRecordFrame();


SimRecordFrame::SimRecordFrame() {
	this->simRecordFrameInstance = NULL;

	/*
	this->mutex = CreateMutex(NULL, FALSE, NULL);

	ReleaseMutex(this->mutex);

	*/
}


SimRecordFrame::~SimRecordFrame() {

	/*
	WaitForSingleObject(SimRecordFrame::simRecordFrameInstance->mutex,INFINITE);


	ReleaseMutex(this->mutex);
	CloseHandle(this->mutex);
	this->mutex = NULL;

	*/
}


void SimRecordFrame::releaseInstance() {
	if (NULL != simRecordFrameInstance) {
		delete simRecordFrameInstance;
		simRecordFrameInstance = NULL;
	}

}


void SimRecordFrame::writeOneRecord(const char* _log) {

}


SimRecordFrame::SimRecordFrameGarbo::SimRecordFrameGarbo() {

}

SimRecordFrame::SimRecordFrameGarbo::~SimRecordFrameGarbo() {
	releaseInstance();
}