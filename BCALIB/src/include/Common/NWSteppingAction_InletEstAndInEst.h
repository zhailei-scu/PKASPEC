#ifndef NWSTEPPINGACTION_INLETESTANDINEST_H
#define NWSTEPPINGACTION_INLETESTANDINEST_H

#include "G4UserSteppingAction.hh"
#include <string>

using namespace std;

class NWSteppingAction_InletEstAndInEst : public G4UserSteppingAction {

public:
	/*Default constructor*/
	NWSteppingAction_InletEstAndInEst();


	/*Destructor*/
	~NWSteppingAction_InletEstAndInEst();

	/*Copy constructor*/
	NWSteppingAction_InletEstAndInEst(const NWSteppingAction_InletEstAndInEst& r);

	/*reload the operator =*/
	NWSteppingAction_InletEstAndInEst operator = (const NWSteppingAction_InletEstAndInEst &r);

public:
	/*override*/
	void UserSteppingAction(const G4Step*);

private:

	int targetAtomNumber;

public:
	inline void SetTargetAtomNumber(int AtomNumber) {
		this->targetAtomNumber = AtomNumber;
	}
};




#endif