#ifndef NWGEOMETRY_H
#define NWGEOMETRY_H

#include "G4VUserDetectorConstruction.hh"

class NWGeometry : public G4VUserDetectorConstruction {
public:
	/*default constructor*/
	NWGeometry();

	/*Destroctor*/
	~NWGeometry();

	/*overload the pure virtual function in parent class*/
	G4VPhysicalVolume* Construct();


};

#endif
