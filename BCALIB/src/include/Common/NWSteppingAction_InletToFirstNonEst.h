#ifndef NWSTEPPINGACTION_INLETTOFIRSTNONEST_H
#define NWSTEPPINGACTION_INLETTOFIRSTNONEST_H

#include "G4UserSteppingAction.hh"
#include "NWRunInfoStore.h"

using namespace std;

class NWSteppingAction_InletToFirstNonEst : public G4UserSteppingAction {

public:
	/*Default constructor*/
	NWSteppingAction_InletToFirstNonEst();


	/*Destructor*/
	~NWSteppingAction_InletToFirstNonEst();

	/*Copy constructor*/
	NWSteppingAction_InletToFirstNonEst(const NWSteppingAction_InletToFirstNonEst& r);

	/*reload the operator =*/
	NWSteppingAction_InletToFirstNonEst operator = (const NWSteppingAction_InletToFirstNonEst &r);

public:
	/*override*/
	void UserSteppingAction(const G4Step*);

private:

	OneEvent theOneEvent;

};




#endif