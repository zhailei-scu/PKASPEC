#ifndef NWSTEPPINGACTION_INLETEST_H
#define NWSTEPPINGACTION_INLETEST_H


#include "G4UserSteppingAction.hh"
#include <string>

using namespace std;

class NWSteppingAction_InletEst : public G4UserSteppingAction {

public:
	/*Default constructor*/
	NWSteppingAction_InletEst();


	/*Destructor*/
	~NWSteppingAction_InletEst();

	/*Copy constructor*/
	NWSteppingAction_InletEst(const NWSteppingAction_InletEst& r);

	/*reload the operator =*/
	NWSteppingAction_InletEst operator = (const NWSteppingAction_InletEst &r);

public:
	/*override*/
	void UserSteppingAction(const G4Step*);

	void SetTargetTrackID(int targetTrackID);

private:

	int targetTrackID;

};




#endif