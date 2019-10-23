#ifndef NWGLOBAL_H
#define NWGLOBAL_H


#include <fstream>
#include <iostream>
#include "G4ThreeVector.hh"
#include "NWSimParameters.h"

class NWGlobal {
private:
	NWGlobal();

	static NWGlobal* nWGlobalInstance;
public:

	~NWGlobal();
	NWGlobal(const NWGlobal &) = delete;
	NWGlobal operator = (const NWGlobal &) = delete;
public:

	static void relaseInstance();

	static NWGlobal* GetInstance();

private:
	NWSimParameters simParamters;

public:
	int CurrentEventID;

	G4ThreeVector particleOriginPos;

	G4ThreeVector particleOriginDirection;

	int flushRecord = 0;

	std::fstream ofsSimRecord;

public:
	void PrintInfo();

	void InitialGlobal(const std::string& mode);

	NWSimParameters * GetSimParamters();

public:
	class NWGlobalGarbo {
	public:
		NWGlobalGarbo();
		~NWGlobalGarbo();
	};

	static NWGlobalGarbo nWGlobalGarbo;
};



#endif
