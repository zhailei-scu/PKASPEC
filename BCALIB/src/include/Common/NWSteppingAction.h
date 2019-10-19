#ifndef NWSTEPPINGACTION_H
#define NWSTEPPINGACTION_H


#include "G4UserSteppingAction.hh"
#include <string>
#include "NWInfoStore.h"

using namespace std;

class NWSteppingAction : public G4UserSteppingAction {

public:
	/*Default constructor*/
	NWSteppingAction();


	/*Destructor*/
	~NWSteppingAction();

	/*Copy constructor*/
	NWSteppingAction(const NWSteppingAction& r);

	/*reload the operator =*/
	NWSteppingAction operator = (const NWSteppingAction &r);

public:
	/*override*/
	void UserSteppingAction(const G4Step*);

	void SetTargetTrackID(int targetTrackID);

private:

	int targetTrackID;

	EventInfo eventInfo;
};




#endif