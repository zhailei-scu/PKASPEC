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

	this->OutPath = r.OutPath;

	this->OutWidth = r.OutWidth;

	this->flushFrequence = r.flushFrequence;

	this->EventLoopsNumber = r.EventLoopsNumber;

	this->targetMaterial = r.targetMaterial;

	this->beam = r.beam;

	return *this;
}

void NWSimParameters::Clean() {

	this->OutPath.clear();
	this->OutPath.swap(std::string(""));

	this->OutWidth = 20;

	this->flushFrequence = 100;

	this->EventLoopsNumber = 0;

	this->targetMaterial.clear();
	this->targetMaterial.swap(std::string(""));

	this->beam.Clean();
}


void NWSimParameters::ReadParametersFromFile(const char* filePath) {




}

void NWSimParameters::SetDefulatValue() {
	double FluxRange[2][2];

	this->OutPath.clear();
	this->OutPath.swap(std::string(""));
	this->OutPath = std::string("");

	this->OutWidth = 20;

	this->flushFrequence = 100;

	this->EventLoopsNumber = 1000;

	this->targetMaterial.clear();
	this->targetMaterial.swap(std::string(""));
	this->targetMaterial = std::string("G4_Zr");

	//beam
	this->beam.SetTurnOnMode(BeamMode(Area_Random));

	this->beam.SetGunEnergy(1.0);

	this->beam.SetGunParticleName(G4String("neutron"));

	this->beam.SetGunParticleStartDirection(G4ThreeVector(0, 0, -1));

	FluxRange[0][0] = -10;  //x0
	FluxRange[0][1] = 10;  //x1
	FluxRange[1][0] = -10;  //y0
	FluxRange[1][1] = 10;  //y1
	this->beam.SetFluxRange(FluxRange);
}

void NWSimParameters::PrintParameters() {

	std::cout << "###  The parameters summary ###" << std::endl;

	std::cout << "The event loops number is: " << this->EventLoopsNumber << std::endl;

	std::cout << "The target material is: " << this->targetMaterial.c_str() << std::endl;

	std::cout << "The out path is : " << OutPath.c_str() << std::endl;

	this->beam.Print();

	std::cout << "### End of the parameters summary ###" << std::endl;
}
