#ifndef NWEVENTACTION_H
#define NWEVENTACTION_H

#include "G4UserEventAction.hh"


class NWEventAction : public G4UserEventAction {
public:
	/*Default constructor*/
	NWEventAction();

	/*Destructor*/
	~NWEventAction();

public:
	/*override*/
	virtual void BeginOfEventAction(const G4Event* anEvent);
	virtual void EndOfEventAction(const G4Event* anEvent);
};


#endif