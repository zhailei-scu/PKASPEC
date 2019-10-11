#ifndef NWSTACKINGACTION_H
#define NWSTACKINGACTION_H

#include "G4UserStackingAction.hh"

class NWStackingAction : public G4UserStackingAction {
public:
	/*Default constructor*/
	NWStackingAction();

	/*Destructor*/
	~NWStackingAction();

public:

	/*override*/
	virtual void PrepareNewEvent();


};





#endif