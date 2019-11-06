#include "NWActionInitialization.h"
#include "NWPrimaryGeneratorAction.h"
#include "NWSteppingAction_InletEst.h"
#include "NWSteppingAction_MatrixAtoms.h"

NWActionInitialization::NWActionInitialization() {

}


NWActionInitialization::~NWActionInitialization() {

}


void NWActionInitialization::Build() const{
	SetUserAction(new NWPrimaryGeneratorAction());

	/*
	NWSteppingAction_InletEst *theNWSteppingAction = new NWSteppingAction_InletEst();

	theNWSteppingAction->SetTargetTrackID(1);
	*/

	NWSteppingAction_MatrixAtoms *theNWSteppingAction = new NWSteppingAction_MatrixAtoms();
	theNWSteppingAction->SetTargetAtomNumber(40);

	SetUserAction(theNWSteppingAction);
}

