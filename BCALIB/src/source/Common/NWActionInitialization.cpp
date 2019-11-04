#include "NWActionInitialization.h"
#include "NWPrimaryGeneratorAction.h"
#include "NWSteppingAction.h"
#include "NWSteppingAction_MatrixAtoms.h"

NWActionInitialization::NWActionInitialization() {

}


NWActionInitialization::~NWActionInitialization() {

}


void NWActionInitialization::Build() const{
	SetUserAction(new NWPrimaryGeneratorAction());

	/*
	NWSteppingAction *theNWSteppingAction = new NWSteppingAction();

	theNWSteppingAction->SetTargetTrackID(1);
	*/

	NWSteppingAction_MatrixAtoms *theNWSteppingAction = new NWSteppingAction_MatrixAtoms();
	theNWSteppingAction->SetTargetAtomNumber(40);

	SetUserAction(theNWSteppingAction);
}

