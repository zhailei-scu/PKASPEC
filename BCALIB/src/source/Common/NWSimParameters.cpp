#include "NWSimParameters.h"
#include <stdlib.h>
#include "G4SystemOfUnits.hh"
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

	this->theConcentReaction = r.theConcentReaction;

	this->OutPath = r.OutPath;

	this->OutWidth = r.OutWidth;

	this->flushFrequence = r.flushFrequence;

	this->EventLoopsNumber = r.EventLoopsNumber;

	this->linkCellInterval_xy = r.linkCellInterval_xy;

	this->linkCellNum_z = r.linkCellNum_z;

	this->targetMaterial = r.targetMaterial;

	this->beam = r.beam;

	this->targetMaterial = r.targetMaterial;

	return *this;
}

void NWSimParameters::Clean() {


	this->theConcentReaction = ConcentReaction(InletToLastEst);

	this->OutPath.clear();
	std::string().swap(this->OutPath);

	this->OutWidth = 20;

	this->flushFrequence = 100;

	this->EventLoopsNumber = 0;

	this->linkCellInterval_xy = 10 * mm;

	this->linkCellNum_z = 10;

	this->beam.Clean();

	this->targetMaterial.Clean();
}


void NWSimParameters::ReadParametersFromFile(const char*) {




}

void NWSimParameters::SetDefulatValue() {

	this->theConcentReaction = ConcentReaction(Iso);

	double FluxRange[2][2];

	this->OutPath.clear();
	std::string().swap(this->OutPath);
	this->OutPath = std::string("");

	this->OutWidth = 20;

	this->flushFrequence = 100;

	this->linkCellInterval_xy = 10 * mm;

	this->linkCellNum_z = 10;

	//beam
	this->beam.SetTurnOnMode(BeamMode(Area_Random));

	this->beam.SetGunEnergy(15);

	this->beam.SetGunParticleName(G4String("neutron"));

	this->beam.SetGunParticleStartDirection(G4ThreeVector(0, 0, -1));

	FluxRange[0][0] = -10 * mm;  //x0
	FluxRange[0][1] = 10 * mm;  //x1
	FluxRange[1][0] = -10 * mm;  //y0
	FluxRange[1][1] = 10 * mm;  //y1
	this->beam.SetFluxRange(FluxRange);

	//Material
	this->targetMaterial.ConstructMaterial(MaterialType(User_W));
}

void NWSimParameters::PrintParameters() {

	std::cout << "###  The parameters summary ###" << std::endl;

	std::cout << "The event loops number is: " << this->EventLoopsNumber << std::endl;

	std::cout << "The target material is: " << this->targetMaterial.GetMaterialName().c_str() << std::endl;

	std::cout << "The target atom number is: " << this->targetMaterial.GetAtomNumber() << std::endl;

	std::cout << "The out path is : " << OutPath.c_str() << std::endl;

	this->beam.Print();

	std::cout << "### End of the parameters summary ###" << std::endl;
}
