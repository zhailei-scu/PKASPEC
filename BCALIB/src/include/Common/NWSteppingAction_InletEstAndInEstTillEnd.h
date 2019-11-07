#ifndef NWSTEPPINGACTION_INLETESTANDINESTTILLEND_H
#define NWSTEPPINGACTION_INLETESTANDINESTTILLEND_H

#include "G4UserSteppingAction.hh"
#include "NWRunInfoStore.h"

using namespace std;


class NWSteppingAction_InletEstAndInEstTillEnd : public G4UserSteppingAction {

public:
	/*Default constructor*/
	NWSteppingAction_InletEstAndInEstTillEnd();


	/*Destructor*/
	~NWSteppingAction_InletEstAndInEstTillEnd();

	/*Copy constructor*/
	NWSteppingAction_InletEstAndInEstTillEnd(const NWSteppingAction_InletEstAndInEstTillEnd& r);

	/*reload the operator =*/
	NWSteppingAction_InletEstAndInEstTillEnd operator = (const NWSteppingAction_InletEstAndInEstTillEnd &r);

public:
	/*override*/
	void UserSteppingAction(const G4Step*);

private:

	int targetAtomNumber;

	OneEvent theOneEvent;

public:
	inline void SetTargetAtomNumber(int AtomNumber) {
		this->targetAtomNumber = AtomNumber;
	}
};




#endif