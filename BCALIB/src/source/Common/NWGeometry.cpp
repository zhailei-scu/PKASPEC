#include "NWGeometry.h"
#include "G4Box.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "NWGlobal.h"
#include "G4PhysicalConstants.hh"

NWGeometry::NWGeometry() {

}

NWGeometry::~NWGeometry() {

}

G4VPhysicalVolume* NWGeometry::Construct() {

	double halfWorld_x = NWGlobal::GetInstance()->GetSimParamters().GetHalfWorld_x();
	double halfWorld_y = NWGlobal::GetInstance()->GetSimParamters().GetHalfWorld_y();
	double halfWorld_z = NWGlobal::GetInstance()->GetSimParamters().GetHalfWorld_z();

	// Option to switch on/off checking of volumes overlaps
	G4bool checkOverlaps = true;

	G4NistManager * nist = G4NistManager::Instance();
	/*The world volum*/

	//G4Material* worldMaterial = nist->FindOrBuildMaterial("G4_AIR");
	G4Material* worldMaterial = new G4Material("world_Vacum",1.0, 1.01*g /mole, universe_mean_density, kStateGas, 0.1*kelvin,1.e-19*kelvin);
	//G4cout << worldMaterial << G4endl;

	G4Box* worldBoxShape = new G4Box("worldBoxShape", 1.0*halfWorld_x, 1.0*halfWorld_y, 1.0*halfWorld_z);
	G4LogicalVolume* worldBoxLogicalVolume = new G4LogicalVolume(worldBoxShape, worldMaterial,"worldBoxLogical");
	G4VPhysicalVolume*  worldBoxPhysicVolume = new G4PVPlacement(0,							//the rotation 
																 G4ThreeVector(0, 0, 0),	//the central position
																 worldBoxLogicalVolume, 
																 "worldBoxPhysicVolume", 
																 NULL,						//no mother is here
																 false,						//no boolean operation
																 0,							//copy number
																 checkOverlaps);			//overlapping checking


	/*The target volum zone*/
	G4Material *targetMaterial = NULL;
	switch (NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetTheModel())
	{
		case(MaterialModel(G4Default)):
		{
			targetMaterial = nist->FindOrBuildMaterial(NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetMaterialName().c_str());
		}
		break;


		case(MaterialModel(UserDef)):
		{
			G4Isotope *theIso = new G4Isotope(NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetMaterialName().c_str(),
											  NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetAtomNumber(),
											  NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetBaryonNumber(),
											  NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetMoleMass());

			G4Element *elemSingle = new G4Element(NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetMaterialName().c_str(),
												  NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetMaterialName().c_str(),
												  1);
			elemSingle->AddIsotope(theIso, 100 * perCent);

			targetMaterial = new G4Material(NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetMaterialName().c_str(),
											NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetDensity(),
											1);
			targetMaterial->AddElement(elemSingle, 1);

			/*
			targetMaterial = new G4Material(NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetMaterialName().c_str(),
											NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetAtomNumber(), 
											NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetMoleMass(),
											NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetDensity());
			*/
		}
		break;

		default:
		{
			std::cout << "Unknown material model: "<< NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetTheModel() <<std::endl;
		}
		break;
	}

	if (!targetMaterial) {
		std::cout << "The target materials is not defined in nist database: " << NWGlobal::GetInstance()->GetSimParamters().GetTargetMaterial().GetMaterialName() << std::endl;;
		getchar();
		exit(1);
	}

	G4cout << targetMaterial << G4endl;

	G4Box* targetBoxShape = new G4Box("targetBoxShape", 0.5*halfWorld_x, 0.5*halfWorld_y, 0.5*halfWorld_z);
	G4LogicalVolume* targetBoxLogicalVolume = new G4LogicalVolume(targetBoxShape, targetMaterial, "targetBoxLogical");
	new G4PVPlacement(0,                          //the rotation 
																 G4ThreeVector(0, 0, 0),     //the central position
																 targetBoxLogicalVolume,
																 "targetBoxPhysical",
																 worldBoxLogicalVolume,      //her mother is the worldvolum
																 false,                      //no boolean operation
																 0,							 //copy number
																 checkOverlaps);			 //overlapping checking

	return worldBoxPhysicVolume;
}