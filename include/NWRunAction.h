#ifndef NWRUNACTION_H
#define NWRUNACTION_H

#include "G4UserRunAction.hh"


class NWRunAction : public  G4UserRunAction{
public:
	/*Default constructor*/
	NWRunAction();

	/*Deconstrouctor*/
	~NWRunAction();

public:
	/*override*/
	virtual void BeginOfRunAction(const G4Run* aRun);
	virtual void EndOfRunAction(const G4Run* aRun);
};

#endif
