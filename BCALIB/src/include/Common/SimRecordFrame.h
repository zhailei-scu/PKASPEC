#ifndef SIMRECORDFRAME_H
#define SIMRECORDFRAME_H
//#include <windows.h>


/*This class is used to record the simulation events, is a singleton*/
class SimRecordFrame {

private:
	/*Default constructor*/
	SimRecordFrame();

	/*Copy Constructor*/
	SimRecordFrame(const SimRecordFrame &r) = delete;

	/*Reload the operator =*/
	SimRecordFrame operator = (const SimRecordFrame &r) = delete;

	static  SimRecordFrame *simRecordFrameInstance;
public:
	~SimRecordFrame();
	static void writeOneRecord(const char* _log);

	static void releaseInstance();


	/*The garbge auto release class*/
	class SimRecordFrameGarbo {
	public:
		SimRecordFrameGarbo();
		~SimRecordFrameGarbo();
	};

	static SimRecordFrameGarbo garbo;


private:


	//HANDLE mutex;


};





#endif
