#ifndef NWSIMPARAMETERS_H
#define NWSIMPARAMETERS_H

#include <string>
#include <fstream>
#include "G4ThreeVector.hh"

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
	std::string mode;

private:
	std::string OutPath;

	int OutWidth = 20;

private:
	int flushFrequence = 100;

	int EventLoopsNumber;

	double gunEnergy;

	std::string gunParticleName;

	G4ThreeVector gunParticleStartPos;

	G4ThreeVector gunParticleStartDirection;

	std::string targetMaterial;

public:

	void ReadParametersFromFile(const char* filePath);

	void PrintParameters();

public:



	inline void SetMode(const std::string &mode) {
		this->mode = mode;
	}

	inline const std::string * GetMode() {
		return &this->mode;
	}

	inline void SetOutPath(const std::string &OutPath) {
		this->OutPath = OutPath;
	}

	inline const std::string * GetOutPath() {
		return &this->OutPath;
	}

	inline void SetOutWidth(const int & OutWidth) {
		this->OutWidth = OutWidth;
	}

	inline const int GetOutWidth() {
		return this->OutWidth;
	}

	inline void SetFlushFrequence(const int & flushFrequence) {
		this->flushFrequence = flushFrequence;
	}

	inline const int GetFlushFrequence() {
		return this->flushFrequence;
	}

	inline void SetEventLoopsNumber(const int & EventLoopsNumber) {
		this->EventLoopsNumber = EventLoopsNumber;
	}

	inline const int GetEventLoopsNumber() {
		return this->EventLoopsNumber;
	}

	inline void SetGunEnergy(const double & gunEnergy) {
		this->gunEnergy = gunEnergy;
	}

	inline const double GetGunEnergy() {
		return this->gunEnergy;
	}

	inline void SetGunParticleName(const std::string &gunParticleName) {
		this->gunParticleName = gunParticleName;
	}

	inline const std::string * GetGunParticleName() {
		return &this->gunParticleName;
	}

	inline void SetGunParticleStartPos(const G4ThreeVector &gunParticleStartPos) {
		this->gunParticleStartPos = gunParticleStartPos;
	}

	inline const G4ThreeVector * GetGunParticleStartPos() {
		return &this->gunParticleStartPos;
	}

	inline void SetGunParticleStartDirection(const G4ThreeVector &gunParticleStartDirection) {
		this->gunParticleStartDirection = gunParticleStartDirection;
	}

	inline const G4ThreeVector * GetGunParticleStartDirection() {
		return &this->gunParticleStartDirection;
	}

	inline void SetTargetMaterial(const std::string &targetMaterial) {
		this->targetMaterial = targetMaterial;
	}

	inline const std::string * GetTargetMaterial() {
		return &this->targetMaterial;
	}

};



#endif