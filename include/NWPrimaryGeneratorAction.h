#ifndef NWPrimaryGeneratorAction_H
#define NWPrimaryGeneratorAction_H

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "NWInfoStore.h"

class NWPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
	/*Default constructor*/
	NWPrimaryGeneratorAction();

	/*Destructor*/
	~NWPrimaryGeneratorAction();


	/*Copy constructor*/
	NWPrimaryGeneratorAction(const NWPrimaryGeneratorAction &r);

	/*reload the operation = */
	NWPrimaryGeneratorAction operator = (const NWPrimaryGeneratorAction &r);
public:
	/*override*/
	virtual void GeneratePrimaries(G4Event* anEvent);

private:
	G4ParticleGun* particleGun;

	G4ParticleDefinition *particleDef;
};


#endif