#ifndef NWSTEPPINGACTION_MATRIXATOMS_H
#define NWSTEPPINGACTION_MATRIXATOMS_H

#include "G4UserSteppingAction.hh"
#include <string>

using namespace std;

class NWSteppingAction_MatrixAtoms : public G4UserSteppingAction {

public:
	/*Default constructor*/
	NWSteppingAction_MatrixAtoms();


	/*Destructor*/
	~NWSteppingAction_MatrixAtoms();

	/*Copy constructor*/
	NWSteppingAction_MatrixAtoms(const NWSteppingAction_MatrixAtoms& r);

	/*reload the operator =*/
	NWSteppingAction_MatrixAtoms operator = (const NWSteppingAction_MatrixAtoms &r);

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