#include "NWActionInitialization.h"
#include "NWPrimaryGeneratorAction.h"
#include "NWSteppingAction.h"

NWActionInitialization::NWActionInitialization() {

}


NWActionInitialization::~NWActionInitialization() {

}


void NWActionInitialization::Build() const{
	SetUserAction(new NWPrimaryGeneratorAction());


	NWSteppingAction *theNWSteppingAction = new NWSteppingAction();

	theNWSteppingAction->SetTargetTrackID(1);

	theNWSteppingAction->SetTargetProcessName("hadElastic");

	SetUserAction(theNWSteppingAction);
}

