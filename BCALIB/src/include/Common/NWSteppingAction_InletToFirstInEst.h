#ifndef NWSTEPPINGACTION_INLETTOFIRSTINEST_H
#define NWSTEPPINGACTION_INLETTOFIRSTINEST_H

#include "G4UserSteppingAction.hh"
#include "NWRunInfoStore.h"

using namespace std;

class NWSteppingAction_InletToFirstInEst : public G4UserSteppingAction {

public:
	/*Default constructor*/
	NWSteppingAction_InletToFirstInEst();


	/*Destructor*/
	~NWSteppingAction_InletToFirstInEst();

	/*Copy constructor*/
	NWSteppingAction_InletToFirstInEst(const NWSteppingAction_InletToFirstInEst& r);

	/*reload the operator =*/
	NWSteppingAction_InletToFirstInEst operator = (const NWSteppingAction_InletToFirstInEst &r);

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