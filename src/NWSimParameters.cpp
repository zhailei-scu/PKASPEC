#include "NWSimParameters.h"
#include <stdlib.h>
/*Singleton instance*/


std::string simMode = std::string("sim");
std::string analysisMode = std::string("analysis");


NWSimParameters::NWSimParameters() {
	this->Clean();
}

NWSimParameters::~NWSimParameters() {
	this->Clean();
}

NWSimParameters::NWSimParameters(const NWSimParameters &r) {
	/*The operator = had been overload*/
	*this = r;
}

NWSimParameters& NWSimParameters::operator=(const NWSimParameters & r) {
	this->mode = r.mode;

	this->OutPath = r.OutPath;

	this->OutWidth = r.OutWidth;

	this->flushFrequence = r.flushFrequence;

	this->EventLoopsNumber = r.EventLoopsNumber;

	this->gunEnergy = r.gunEnergy;

	this->gunParticleName = r.gunParticleName;

	this->gunParticleStartPos = r.gunParticleStartPos;

	this->gunParticleStartDirection = r.gunParticleStartDirection;

	this->targetMaterial = r.targetMaterial;

	return *this;
}

void NWSimParameters::Clean() {
	this->mode.clear();
	this->mode.swap(std::string(""));

	this->OutPath.clear();
	this->OutPath.swap(std::string(""));

	this->OutWidth = 20;

	this->flushFrequence = 100;

	this->EventLoopsNumber = 0;

	this->gunEnergy = 0.e0;

	this->gunParticleName.clear();
	this->gunParticleName.swap(std::string(""));

	this->gunParticleStartPos = G4ThreeVector(0, 0, 0);

	this->gunParticleStartDirection = G4ThreeVector(0, 0, 0);

	this->targetMaterial.clear();
	this->targetMaterial.swap(std::string(""));
}


void NWSimParameters::ReadParametersFromFile(const char* filePath) {




}

void NWSimParameters::PrintParameters() {

	if (0 == this->mode.compare(simMode)) {
		std::cout << "###  The parameters summary ###" << std::endl;

		std::cout << "The event loops number is: " << this->EventLoopsNumber << std::endl;

		std::cout << "The particle gun name is: " << this->gunParticleName.c_str() << std::endl;

		std::cout << "The particle gun energy is: " << this->gunEnergy << std::endl;

		std::cout << "The target material is: " << this->targetMaterial.c_str() << std::endl;

		std::cout << "The target material is: " << this->targetMaterial.c_str() << std::endl;

		std::cout << "The out path is : " << OutPath.c_str() << std::endl;

		std::cout << "### End of the parameters summary ###" << std::endl;
	}
	else if (0 == this->mode.compare(analysisMode)) {

		std::cout << "###  The parameters summary ###" << std::endl;

		std::cout << "The out path is : " << OutPath.c_str() << std::endl;

		std::cout << "### End of the parameters summary ###" << std::endl;
	}


}
