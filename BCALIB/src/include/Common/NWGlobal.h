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

	int EventLoops;

public:

	int CurrentEventID;

	G4ThreeVector particleOriginPos;

	G4ThreeVector particleOriginDirection;

	int flushRecord = 0;

	std::fstream ofsSimRecord;

	std::fstream ofs_Iso;

public:
	void PrintInfo();

	void InitialGlobal(const std::string& mode,const NWSimParameters & simParameters);

	NWSimParameters const & GetSimParamters() const;

	void NWGlobal::SetSimParamters(const NWSimParameters &simParamters);

public:
	class NWGlobalGarbo {
	public:
		NWGlobalGarbo();
		~NWGlobalGarbo();
	};

	static NWGlobalGarbo nWGlobalGarbo;

public:
	inline void SetEventLoops(int theEventLoops) {
		this->EventLoops = theEventLoops;
	}

	inline int GetEventLoops() {
		return this->EventLoops;
	}
};



#endif
