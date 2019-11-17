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


	G4double targetBox_x = 10000 * cm;
	G4double targetBox_y = 10000 * cm;
	G4double targetBox_z = 10000 * cm;

	// Option to switch on/off checking of volumes overlaps
	G4bool checkOverlaps = true;

	G4NistManager * nist = G4NistManager::Instance();
	/*The world volum*/

	//G4Material* worldMaterial = nist->FindOrBuildMaterial("G4_AIR");
	G4Material* worldMaterial = new G4Material("world_Vacum",1.0, 1.01*g /mole, universe_mean_density, kStateGas, 0.1*kelvin,1.e-19*kelvin);
	//G4cout << worldMaterial << G4endl;

	G4Box* worldBoxShape = new G4Box("worldBoxShape", 1.0*targetBox_x, 1.0*targetBox_y, 1.0*targetBox_z);
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
	//G4double w_density = 19.3*g / cm3;
	//G4double w_molemass = 183.84*g / mole;
	//G4Material* targetMaterial = new G4Material("ITER_W",74,w_molemass, w_density);
	G4Material *targetMaterial = nist->FindOrBuildMaterial(NWGlobal::GetInstance()->GetSimParamters()->GetTargetMaterial()->c_str());
	G4cout << targetMaterial << G4endl;


	if (!targetMaterial) {
		std::cout << "The target materials is not defined in nist database: " << NWGlobal::GetInstance()->GetSimParamters()->GetTargetMaterial()->c_str();
		system("pause");
		exit(1);
	}

	G4Box* targetBoxShape = new G4Box("targetBoxShape", 0.5*targetBox_x, 0.5*targetBox_y, 0.5*targetBox_z);
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