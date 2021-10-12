#include "NWSimParameters.h"
#include <stdlib.h>
#include "G4SystemOfUnits.hh"
/*Singleton instance*/


std::string simMode = std::string("sim");
std::string analysisMode = std::string("analysis");




NWSimParameters::NWSimParameters() {
	this->Clean();
	SetDefulatValue();
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

	this->halfWorld_x = r.halfWorld_x;

	this->halfWorld_y = r.halfWorld_y;

	this->halfWorld_z = r.halfWorld_z;

	this->EventLoopsNumber = r.EventLoopsNumber;

	this->linkCellInterval_xy = r.linkCellInterval_xy;

	this->linkCellNum_z = r.linkCellNum_z;

	this->linkCellInterval_z = r.linkCellInterval_z;

	this->Analysis_CutEnergy = r.Analysis_CutEnergy;

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


	this->halfWorld_x = 10000 * cm;
	this->halfWorld_y = 10000 * cm;
	this->halfWorld_z = 10000 * cm;

	this->EventLoopsNumber = 0;

	this->linkCellInterval_xy = 20 * mm;

	this->linkCellNum_z = 10;

	this->linkCellInterval_z = 20 *mm;

	this->Analysis_CutEnergy = 5.5E-5*MeV;

	this->beam.Clean();

	this->targetMaterial.Clean();
}


void NWSimParameters::ReadParametersFromFile(const char*) {




}

void NWSimParameters::SetDefulatValue() {


	double FluxRange[2][2];

	this->OutPath.clear();
	std::string().swap(this->OutPath);
	this->OutPath = std::string("");

	this->OutWidth = 20;

	this->flushFrequence = 100;

	this->halfWorld_x = 10000 * cm;
	this->halfWorld_y = 10000 * cm;
	this->halfWorld_z = 10000 * cm;


	//-----Change below----

	this->theConcentReaction = ConcentReaction(MatrixAtom);

	this->linkCellInterval_xy = 20 * mm;

	this->linkCellInterval_z = 20 * mm;

	this->linkCellNum_z = 10;

	//this->Analysis_CutEnergy = 5.5E-5*MeV;
	this->Analysis_CutEnergy = 0.E0*MeV;

	//beam
	this->beam.SetTurnOnMode(BeamMode(Area_Random));

	this->beam.SetGunEnergy(14.4);

	this->beam.SetGunParticleName(G4String("neutron"));

	this->beam.SetGunParticleStartDirection(G4ThreeVector(0, 0, -1));

	FluxRange[0][0] = -10 * mm;  //x0
	FluxRange[0][1] = 10 * mm;  //x1
	FluxRange[1][0] = -10 * mm;  //y0
	FluxRange[1][1] = 10 * mm;  //y1
	this->beam.SetFluxRange(FluxRange);

	//Material
	this->targetMaterial.ConstructSimpleMaterial(MaterialType(G4_Fe));
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
