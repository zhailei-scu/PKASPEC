#ifndef NWBEAM_H
#define NWBEAM_H

#include <string>
#include "G4ThreeVector.hh"

using namespace std;

enum BeamMode {
	Area_Random,
	Area_Uniform
};


class NWBeam {
public:
	NWBeam();
	~NWBeam();
	NWBeam(const NWBeam & r);
	NWBeam& operator = (NWBeam const & r);

private:
	BeamMode turnOnMode;

	double gunEnergy;

	string gunParticleName;

	G4ThreeVector gunParticleStartDirection;

	double FluxRange[2][2];

public:

	void Clean();

	void Print() const;

	G4ThreeVector CalOriginPositionXY(int CurrentEventsIndex, int TotalEventsLoopNum, double ZPos)const;

	inline void SetTurnOnMode(BeamMode theMode) {
		this->turnOnMode = theMode;
	}

	inline BeamMode GetTurnOnMode() const{
		return this->turnOnMode;
	}


	inline void SetGunEnergy(const double & theGunEnergy) {
		this->gunEnergy = theGunEnergy;
	}

	inline double GetGunEnergy() const{
		return this->gunEnergy;
	}

	inline void SetGunParticleName(const std::string &theGunParticleName) {
		this->gunParticleName = theGunParticleName;
	}

	inline const std::string * GetGunParticleName() const {
		return &this->gunParticleName;
	}

	inline void SetGunParticleStartDirection(const G4ThreeVector &theGunParticleStartDirection) {
		this->gunParticleStartDirection = theGunParticleStartDirection;
	}

	inline const G4ThreeVector * GetGunParticleStartDirection() const{
		return &this->gunParticleStartDirection;
	}

	inline void SetFluxRange(double theFluxRange[2][2]) {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				this->FluxRange[i][j] = theFluxRange[i][j];
			}
		}
	}

	inline void GetFluxRange( double (*ptr)[2]){
		ptr = FluxRange;
	}

	inline void GetFluxCenter( double Center[2]) const {
		for (int i = 0; i < 2; i++) {
			Center[i] = 0.5*(this->FluxRange[i][0] + this->FluxRange[i][1]);
		}
	}

};

#endif

