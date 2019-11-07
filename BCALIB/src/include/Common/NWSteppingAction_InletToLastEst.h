#ifndef NWSTEPPINGACTION_INLETTOLASTEST_H
#define NWSTEPPINGACTION_INLETTOLASTEST_H


#include "G4UserSteppingAction.hh"
#include <string>

using namespace std;

class NWSteppingAction_InletToLastEst : public G4UserSteppingAction {

public:
	/*Default constructor*/
	NWSteppingAction_InletToLastEst();


	/*Destructor*/
	~NWSteppingAction_InletToLastEst();

	/*Copy constructor*/
	NWSteppingAction_InletToLastEst(const NWSteppingAction_InletToLastEst& r);

	/*reload the operator =*/
	NWSteppingAction_InletToLastEst operator = (const NWSteppingAction_InletToLastEst &r);

public:
	/*override*/
	void UserSteppingAction(const G4Step*);

	void SetTargetTrackID(int targetTrackID);

private:

	int targetTrackID;

};




#endif