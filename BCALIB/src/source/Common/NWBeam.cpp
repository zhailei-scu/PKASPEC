#include "NWBeam.h"
#include <CLHEP/Random/Randomize.h>

NWBeam::NWBeam() {
	this->Clean();
}

NWBeam::~NWBeam() {
	this->Clean();
}

NWBeam::NWBeam(const NWBeam & r) {
	/*The operator = had been overload*/
	*this = r;

}



NWBeam& NWBeam::operator = (NWBeam const & r) {

	this->turnOnMode = r.turnOnMode;

	this->gunEnergy = r.gunEnergy;

	this->gunParticleName = r.gunParticleName;

	this->gunParticleStartDirection = r.gunParticleStartDirection;


	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {

			this->FluxRange[i][j] = r.FluxRange[i][j];

		}
	}

	return *this;

}

void NWBeam::Clean() {

	this->turnOnMode = BeamMode(Area_Random);

	this->gunEnergy = 0.e0;

	this->gunParticleName.clear();
	this->gunParticleName.swap(std::string(""));

	this->gunParticleStartDirection = G4ThreeVector(0, 0, -1);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {

			this->FluxRange[i][j] = 0;

		}
	}
}

void NWBeam::Print() const{

	std::cout << "The gun turnOn Mode is: " << turnOnMode << std::endl;

	std::cout << "The gun energy is: " << gunEnergy << std::endl;

	std::cout << "The gun particle name is: " << gunParticleName.c_str() << std::endl;

	std::cout << "The beam start direction: " << gunParticleStartDirection.getX() 
		<< " " << gunParticleStartDirection.getY()
		<< " " << gunParticleStartDirection.getZ()
		<< std::endl;

	std::cout << "The FluxRange x is from " << this->FluxRange[0][0] << " to " << this->FluxRange[0][1]
			  << " The FluxRange y is from " << this->FluxRange[1][0] << " to " << this->FluxRange[1][1]<<std::endl;

}

G4ThreeVector NWBeam::CalOriginPositionXY(int CurrentEventsIndex,int TotalEventsLoopNum, double ZPos) const {
	G4ThreeVector theResult(0,0, ZPos);
	double xLength;
	double yLength;
	double A;
	int xCeilNum;
	double Interval;

	xLength = abs(this->FluxRange[0][0] - this->FluxRange[0][1]);
	yLength = abs(this->FluxRange[1][0] - this->FluxRange[1][1]);

	switch (this->turnOnMode) {
		case BeamMode(Area_Random):
		{
			//CLHEP::HepRandom::getTheEngine()->flat() is  equal with G4UniformRand()
			theResult.setX(this->FluxRange[0][0] + xLength * CLHEP::HepRandom::getTheEngine()->flat());
			theResult.setY(this->FluxRange[1][0] + yLength * CLHEP::HepRandom::getTheEngine()->flat());
		}
		break;

		case BeamMode(Area_Uniform):
		{
			A = xLength*yLength/TotalEventsLoopNum;
			Interval = std::sqrt(A);
			xCeilNum = ceil(xLength / Interval);
			theResult.setX(Interval*(CurrentEventsIndex%xCeilNum + 0.5));
			theResult.setY(Interval*(floor(CurrentEventsIndex / xCeilNum)+0.5));
		}
		break;

		default:
		{
			std::cout << "Unknown turnon mode type: " << this->turnOnMode << std::endl;
			system("pause");
			exit(1);
		}
	}


	return theResult;
}