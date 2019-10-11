#ifndef NWGLOBAL_H
#define NWGLOBAL_H


#include <fstream>
#include <iostream>

extern std::string simMode;
extern std::string analysisMode;

class NWGlobal {
private:
	NWGlobal();

	static NWGlobal* nWGlobalInstance;
public:
	~NWGlobal();
	NWGlobal(const NWGlobal &) = delete;
	NWGlobal operator = (const NWGlobal &) = delete;

	class NWGlobalGarbo {
	public:
		NWGlobalGarbo();
		~NWGlobalGarbo();
	};

	static NWGlobalGarbo nWGlobalGarbo;
public:
	static void relaseInstance();

	static NWGlobal* GetInstance();

public:
	std::string mode;

public:
	std::string OutPath;

	int BinNumberEachPower = 100;

	int OutWidth = 20;

public:
	std::fstream ofsSimRecord;

	int CurrentEventID;

	int EventLoopsNumber;

	double gunEnergy;

	std::string gunParticleName;

	std::string targetMaterial;

public:
	void PrintInfo();

	void InitialGlobal(const char* mode,const char* outPath);
};



#endif
