#include "NWMaterials.h"
#include <iostream>
#include "G4SystemOfUnits.hh"

NWMaterial::NWMaterial() {
	Clean();
}


NWMaterial::~NWMaterial() {
	Clean();
}

void NWMaterial::Clean() {
	ConstructMaterial(MaterialType(User_W));
}

NWMaterial::NWMaterial(const NWMaterial & r) {
	this->theModel = r.theModel;

	this->name = r.name;

	this->atomNumber = r.atomNumber;

	this->baryonNumber = r.baryonNumber;

	this->moleMass = r.moleMass;
	this->density = r.density;
}

NWMaterial NWMaterial::operator = (const NWMaterial & r) {
	this->theModel = r.theModel;

	this->name = r.name;

	this->atomNumber = r.atomNumber;

	this->baryonNumber = r.baryonNumber;

	this->moleMass = r.moleMass;
	this->density = r.density;

	return *this;
}


void NWMaterial::ConstructMaterial(MaterialType type) {
	switch (type)
	{
	case G4_W:
	{
		this->theModel = MaterialModel(G4Default);
		this->name = "G4_W";
		this->atomNumber = 74;
	}
	break;

	case G4_Zr:
	{
		this->theModel = MaterialModel(G4Default);
		this->name = "G4_Zr";
		this->atomNumber = 40;
	}
	break;

	case User_W:
	{
		this->theModel = MaterialModel(UserDef);
		this->name = "W";
		this->atomNumber = 74;
		this->baryonNumber = 184;
		this->density = 19.3*g / cm3;
		this->moleMass = 183.84*g / mole;
	}
	break;

	case User_Zr:
	{
		this->theModel = MaterialModel(UserDef);
		this->name = "Zr";
		this->atomNumber = 40;
		this->baryonNumber = 90;
		this->density = 6.506*g / cm3;
		this->moleMass = 91.224*g / mole;
	}
	break;

	default:
	{
		std::cout << "Unkonwn material type "<< type << std::endl;
		system("pause");
		exit(1);
	}
	break;
	}
}