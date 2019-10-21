#ifndef NWSIMPARAMETERS_H
#define NWSIMPARAMETERS_H

#include <string>
#include <fstream>
#include "G4ThreeVector.hh"
#include "NWBeam.h"

extern std::string simMode;
extern std::string analysisMode;


/*Singleton class*/
class NWSimParameters {

public:
	/*Default constructor*/
	NWSimParameters();
	/*Copy Constructor*/
	NWSimParameters(const NWSimParameters &r);
	/*Reload the default operator =*/
	NWSimParameters& operator = (const NWSimParameters &r);

	/*Destructor*/
	~NWSimParameters();

	void Clean();


private:
	std::string OutPath;

	int OutWidth = 20;

private:
	int flushFrequence = 100;

	int EventLoopsNumber;

	std::string targetMaterial;

private:
	NWBeam beam;

public:

	void ReadParametersFromFile(const char* filePath);

	void PrintParameters();

	void SetDefulatValue();

public:

	inline void SetOutPath(const std::string &OutPath) {
		this->OutPath = OutPath;
	}

	inline const std::string * GetOutPath() const{
		return &this->OutPath;
	}

	inline void SetOutWidth(const int & OutWidth) {
		this->OutWidth = OutWidth;
	}

	inline const int GetOutWidth() const{
		return this->OutWidth;
	}

	inline void SetFlushFrequence(const int & flushFrequence) {
		this->flushFrequence = flushFrequence;
	}

	inline const int GetFlushFrequence() const{
		return this->flushFrequence;
	}

	inline void SetEventLoopsNumber(const int & EventLoopsNumber) {
		this->EventLoopsNumber = EventLoopsNumber;
	}

	inline const int GetEventLoopsNumber() const{
		return this->EventLoopsNumber;
	}

	inline void SetTargetMaterial(const std::string &targetMaterial) {
		this->targetMaterial = targetMaterial;
	}

	inline const std::string * GetTargetMaterial() const{
		return &this->targetMaterial;
	}

	inline void SetBeam(const NWBeam & beam) {
		this->beam = beam;
	}

	inline NWBeam * GetNWBeam(){
		return &this->beam;
	}

};



#endif